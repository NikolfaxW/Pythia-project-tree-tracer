//
// Created by nikol on 3/18/2025.
//

#ifndef PYTHIA_PROJECT_TREE_TRACER_BLOCKS_H
#define PYTHIA_PROJECT_TREE_TRACER_BLOCKS_H

#include <string>
#include <vector>
#include <fstream>

#include "jetStructure.h"



struct block{
    const unsigned int x,  y, id, status, evi;
    std::string fill;
    block(const unsigned int x,  const unsigned int y, const unsigned int id, const unsigned int status, const unsigned int evi) : x(x), y(y), id(id), status(status), evi(evi), fill("yellow"){}
    std::string print(){
        std::string result = "<rect x = \"" + std::to_string(x) + "\" y = \"" + std::to_string(y) + "\" width = \"150\" height = \"100\" fill = \"" + fill + "\" stroke = \"black\" stroke-width = \"2\"></rect>\n";
        result.append("<text x = \"" + std::to_string(x + 75) + "\" y = \"" + std::to_string(y + 60) + "\" font-size = \"16\" text-anchor = \"middle\" fill = \"black\">" + "Id : " + std::to_string(id) + " ; St : " + std::to_string(status) + " ; Ei : " + std::to_string(evi) + "</text>\n");
        return result;
    }
};

struct arrow{
    const unsigned int x1,  y1, x2, y2;
    arrow(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2) : x1(x1), y1(y2), x2(x2), y2(y2){}
    std::string print(){
        std::string result = "<line x1 = \"" + std::to_string(x1) + "\" y1 = \"" + std::to_string(y1) + "\" x2 = \"" + std::to_string(x2) + "\" y2 = \"" + std::to_string(y2) + "\" stroke = \"black\" stroke-width = \"2\"></line>\n";
        result.append("<polygon points = \"" + std::to_string(x2) + "," + std::to_string(y2 - 5) + " " + std::to_string(x2) + "," + std::to_string(y2 + 5) + " " + std::to_string(x2 + 10) + "," + std::to_string(y2) + "\" fill = \"black\"></polygon>\n");
        return result;
    }
};

class blocks {
    std::vector<block> _blocks;
    std::vector<arrow> _arrows;
    jetStructure * _js;

    std::ofstream * htmlFile;
    std::string path = "../results/block_diagram.html";


    void createBlocks();
    void createBlock(const unsigned int x, const unsigned int y, const unsigned int id, const unsigned int status, const unsigned int evi);
    void createArrow(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
public:
    blocks(jetStructure & js);
    ~blocks();

    void createFile();
    void printToFile();
};


#endif //PYTHIA_PROJECT_TREE_TRACER_BLOCKS_H
