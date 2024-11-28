//
// Created by Nikol on 11/26/2024.
//

#ifndef PYTHIA_PROJECT_TREE_TRACER_FUNC_H
#define PYTHIA_PROJECT_TREE_TRACER_FUNC_H

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


std::string createBlock( const unsigned int x, const unsigned int y, std::string fill, const unsigned int id, const unsigned int status, const unsigned int evi);
std::string createArrow(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
void drawBlocksTest();
void learnD_0JetsOrigin(const unsigned int requiredNumberOfD_0);
void learnD_0JetsOriginTest(const unsigned int requiredNumberOfD_0);



#endif //PYTHIA_PROJECT_TREE_TRACER_FUNC_H
