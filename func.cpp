//
// Created by Nikol on 11/26/2024.
//

#include "func.h"
#include "jetStructure.h"
#include "particleUnit.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <deque>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Pythia8/Pythia.h"
#include "TVector2.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "TCanvas.h"
#include "TH2F.h"


std::string createBlock( const unsigned int x, const unsigned int y, std::string fill, const unsigned int id, const unsigned int status, const unsigned int evi){
    std::string result = "<rect x = \"" + std::to_string(x) + "\" y = \"" + std::to_string(y) + "\" width = \"150\" height = \"100\" fill = \"" + fill + "\" stroke = \"black\" stroke-width = \"2\"></rect>\n";
    result.append("<text x = \"" + std::to_string(x + 75) + "\" y = \"" + std::to_string(y + 60) + "\" font-size = \"16\" text-anchor = \"middle\" fill = \"black\">" + "Id : " + std::to_string(id) + " ; St : " + std::to_string(status) + " ; Ei : " + std::to_string(evi) + "</text>\n");
    return result;
}

std::string createArrow(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2){
    std::string result = "<line x1 = \"" + std::to_string(x1) + "\" y1 = \"" + std::to_string(y1) + "\" x2 = \"" + std::to_string(x2) + "\" y2 = \"" + std::to_string(y2) + "\" stroke = \"black\" stroke-width = \"2\"></line>\n";
    result.append("<polygon points = \"" + std::to_string(x2) + "," + std::to_string(y2 - 5) + " " + std::to_string(x2) + "," + std::to_string(y2 + 5) + " " + std::to_string(x2 + 10) + "," + std::to_string(y2) + "\" fill = \"black\"></polygon>\n");
    return result;
}

void drawBlocksTest() {
    std::remove("../results/block_diagram.html");
    // Create an HTML file
    std::ofstream htmlFile("../results/block_diagram.html");

    if (!htmlFile) {
        std::cerr << "Error creating file." << std::endl;
        return;
    }

    // Write the HTML content
    htmlFile << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Block Diagram</title>
           <style>
                body {
                    font-family: 'Arial', sans-serif;
                }
            </style>
</head>
<body>
    <svg viewBox="0 0 500 200" style="width: 20%; height: 20%;">)";
//    <!-- Block 1 -->
    htmlFile << createBlock(50,50, "lightblue", 1, 1, 1);
    htmlFile << createBlock(300,50, "lightgreen", 2, 1, 1);
    htmlFile << createArrow(200, 100, 300, 100);

    htmlFile << R"(
        </svg>
</body>
</html>)";
    htmlFile.close();

    std::cout << "HTML file created: block_diagram.html" << std::endl;
}

void showProgressBar(int progress, int total) {
    double ratio = static_cast<double>(progress) / total;
    std::cout << "(required number of D_0 found: " << progress << " | " << total << ") ";
    std::cout << int(ratio * 100.0) << "%\r";
    std::cout.flush();
}

void learnD_0JetsOrigin(const unsigned int requiredNumberOfD_0){
    particleDictionarry dict;
    Pythia8::Pythia pythia; //create pythia object
    pythia.readFile("../config1.cmnd"); //read config file and intialize pythia
    pythia.init();

    std::list<particleUnit> j_constituents;
    std::map<TString, fastjet::JetDefinition> jetDefs; //map to store jet definitions

    //parameters for jet finding and to make program less hardcoded
    double R = 0.4; //jet radius
    double pTmin_jet = 5, pThadron = 0.2; //minimum pT for jets and hadrons
    int triggerId = 421; //!D_0 //pdg code of the particle to be found
    double pTMinTrig = 1; //minimum pT for the particle to be found
    double pTMaxTrig = 5.0;
    double mTemp; //This variable are needed to recount momentum after particle mass resets
    Pythia8::Vec4 pTemp; //This variable are needed to recount momentum after particle mass resets

    //define jet finding algorithms here:
    jetDefs["#it{k_{t}} jets, #it{R} = " + std::to_string(R)] = fastjet::JetDefinition(
            fastjet::kt_algorithm, R, fastjet::E_scheme, fastjet::Best);
    auto &event = pythia.event; //create a reference to the Pythia8 collision event
    unsigned int numberOfD_0Found = 0;

    std::ofstream file("../results/D_0-origins-structure.txt");
    if (!file.is_open()){
        std::cout <<  "Error opening file for writing." << std::endl;
        return;
    }
    TH1F *h1 = new TH1F("?", "?",
                        30, 0, 30); // 50 bins from -5 to 5
    h1->GetXaxis()->SetNdivisions(15);

    std::deque<int> * mothers, * daughters, * temp;
    mothers = new std::deque<int>;
    daughters = new std::deque<int>;
    std::vector<int> history;

    for (int iEvent = 0; numberOfD_0Found < requiredNumberOfD_0; ++iEvent) { //loop over needed number of events
        if (!pythia.next()) continue; //generate next event, if it is not possible, continue
        int idxD = -1; // to store index of the D_0 particle in the event
        int i = 0;
        //! pt cut removed
        for (i = pythia.event.size() - 1; i > 0; i--) { //goes through all particles generated in event
            if (pythia.event[i].idAbs() == triggerId) { //finds D_0 particle with required pT cut
                idxD = i; //saved its index in the event
                break;
            }
        }
        if (idxD == -1) //if there is no D_0 particle in the event, skip it to not waste resources
        {
            continue;
        }
        ++numberOfD_0Found;
        auto p = pythia.event[i];

        mothers->clear();
        daughters->clear();
        history.clear();

        daughters->push_back(i);
        file << "\nD_0 # : " << numberOfD_0Found << " ; D_0 pT : " << p.pT() << std::endl;

        while(!daughters->empty()){
            while(!daughters->empty()){
                i = daughters->front();
                daughters->pop_front();
                if(i == 0){
                    continue;
                }
                p = pythia.event[i];
                file << "[ idAbs : " << dict.getIdABSName(p.idAbs()) << "; id : " << p.id()  << " ; stat : " << dict.getStatusName(p.status()) << " ; EvI : " << i << " ; m1 : " << p.mother1() << " ; m2 : " << p.mother2() << " ; d1 " << p.daughter1() << " ; d2 : " << p.daughter2() << "  ] ";
                if(p.status() == 21 || p.status() == -21){
                    h1->Fill(p.idAbs());
                }
                if(history.end() == std::find(history.begin(), history.end(), p.mother1())){
                    mothers->push_back(p.mother1());
                    history.push_back(p.mother1());
                }
//                    mothers->push_back(p.mother1());
//                    history.push_back(p.mother1());
                if(p.mother1() != p.mother2()){
                    if(history.end() == std::find(history.begin(), history.end(), p.mother2())){
                        mothers->push_back(p.mother2());
                        history.push_back(p.mother2());
                    }
//                    mothers->push_back(p.mother2());
//                    history.push_back(p.mother2());
                }
            }
            file << "\n => \n";
            temp = mothers;
            mothers = daughters;
            daughters = temp;
        }


        file << " X \n\n";
        showProgressBar(numberOfD_0Found, requiredNumberOfD_0);
    }
    TCanvas *c1 = new TCanvas("?", "?", 800, 600);
    h1->Draw();
    c1->Update();

    c1->SaveAs("../results/1DHistogram.png");

    delete mothers;
    delete daughters;
    file.close();

}

void learnD_0JetsOriginTest(const unsigned int requiredNumberOfD_0){
    particleDictionarry dict;
    Pythia8::Pythia pythia; //create pythia object
    pythia.readFile("../config1.cmnd"); //read config file and intialize pythia
    pythia.init();

    std::list<particleUnit> j_constituents;
    std::map<TString, fastjet::JetDefinition> jetDefs; //map to store jet definitions

    //parameters for jet finding and to make program less hardcoded
    double R = 0.4; //jet radius
    double pTmin_jet = 5, pThadron = 0.2; //minimum pT for jets and hadrons
    int triggerId = 421; //!D_0 //pdg code of the particle to be found
    double pTMinTrig = 1; //minimum pT for the particle to be found
    double pTMaxTrig = 5.0;
    double mTemp; //This variable are needed to recount momentum after particle mass resets
    Pythia8::Vec4 pTemp; //This variable are needed to recount momentum after particle mass resets

    //define jet finding algorithms here:
    jetDefs["#it{k_{t}} jets, #it{R} = " + std::to_string(R)] = fastjet::JetDefinition(
            fastjet::kt_algorithm, R, fastjet::E_scheme, fastjet::Best);
    auto &event = pythia.event; //create a reference to the Pythia8 collision event
    unsigned int numberOfD_0Found = 0;

    std::ofstream file("../results/D_0-origins-structure.txt");
    if (!file.is_open()){
        std::cout <<  "Error opening file for writing." << std::endl;
        return;
    }
    TH1F *h1 = new TH1F("?", "?",
                        30, 0, 30); // 50 bins from -5 to 5
    h1->GetXaxis()->SetNdivisions(15);

    std::deque<int> * mothers, * daughters, * temp;
    mothers = new std::deque<int>;
    daughters = new std::deque<int>;
    std::vector<int> history;

    for (int iEvent = 0; numberOfD_0Found < requiredNumberOfD_0; ++iEvent) { //loop over needed number of events
        if (!pythia.next()) continue; //generate next event, if it is not possible, continue
        int idxD = -1; // to store index of the D_0 particle in the event
        int i = 0;
        //! pt cut removed
        for (i = pythia.event.size() - 1; i > 0; i--) { //goes through all particles generated in event
            if (pythia.event[i].idAbs() == triggerId) { //finds D_0 particle with required pT cut
                idxD = i; //saved its index in the event
                break;
            }
        }
        if (idxD == -1) //if there is no D_0 particle in the event, skip it to not waste resources
        {
            continue;
        }
        ++numberOfD_0Found;
        auto p = pythia.event[i];

        mothers->clear();
        daughters->clear();
        history.clear();

        daughters->push_back(i);
        file << "\nD_0 # : " << numberOfD_0Found << " ; D_0 pT : " << p.pT() << std::endl;

        while(!daughters->empty()){
            while(!daughters->empty()){
                i = daughters->front();
                daughters->pop_front();
                if(i == 0){
                    continue;
                }
                p = pythia.event[i];
                file << "[ idAbs : " << dict.getIdABSName(p.idAbs()) << "; id : " << p.id()  << " ; stat : " << dict.getStatusName(p.status()) << " ; EvI : " << i << " ; m1 : " << p.mother1() << " ; m2 : " << p.mother2() << " ; d1 " << p.daughter1() << " ; d2 : " << p.daughter2() << "  ] ";
                if(p.status() == 21 || p.status() == -21){
                    h1->Fill(p.idAbs());
                }
                if(history.end() == std::find(history.begin(), history.end(), p.mother1())){
                    mothers->push_back(p.mother1());
                    history.push_back(p.mother1());
                }
//                    mothers->push_back(p.mother1());
//                    history.push_back(p.mother1());
                if(p.mother1() != p.mother2()){
                    if(history.end() == std::find(history.begin(), history.end(), p.mother2())){
                        mothers->push_back(p.mother2());
                        history.push_back(p.mother2());
                    }
//                    mothers->push_back(p.mother2());
//                    history.push_back(p.mother2());
                }
            }
            file << "\n => \n";
            temp = mothers;
            mothers = daughters;
            daughters = temp;
        }


        file << " X \n\n";
        showProgressBar(numberOfD_0Found, requiredNumberOfD_0);
    }
    TCanvas *c1 = new TCanvas("?", "?", 800, 600);
    h1->Draw();
    c1->Update();

    c1->SaveAs("../results/1DHistogram.png");

    delete mothers;
    delete daughters;
    file.close();
}