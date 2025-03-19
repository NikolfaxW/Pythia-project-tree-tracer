//
// Created by nikol on 3/18/2025.
//

#include "blocks.h"



blocks::blocks(jetStructure & js){
    _blocks.reserve(100);
    _arrows.reserve(100);
    _js = &js;
    createBlocks();
}

blocks::~blocks() {
    *htmlFile << R"(
        </svg>
</body>
</html>)";

    std::cout << "HTML file created: block_diagram.html" << std::endl;
    htmlFile->close();
    delete htmlFile;
}

void blocks::createBlocks() {
    int j, k, l;
    for(int i = 0; i < _js->particleColumns.size(); ++i){
        for(j = 0; j < _js->particleColumns[i].size(); ++j){
            this->createBlock(100 + i * 200 , 100 + j * 160, _js->particleColumns[i][j]._id, _js->particleColumns[i][j]._st, _js->particleColumns[i][j]._Ei);
            if(_js->particleColumns[i][j].n_mothers > 0){
                for(l = 0; l < _js->particleColumns[_js->particleColumns[i][j].mother1->depth - 1].size(); ++l){
                    if(_js->particleColumns[_js->particleColumns[i][j].mother1->depth - 1][l] == *_js->particleColumns[i][j].mother1){
                        std::cout << "from" << i << ";"<< j << " to "  << _js->particleColumns[i][j].mother1->depth - 1 << ";" << l << std::endl;
                        this->createArrow(100 + i * 200, 150 + j * 160, 250 + (_js->particleColumns[i][j].mother1->depth - 1) * 200, 150 + l * 160);

                        break;
                    }
                }
            }
            if(_js->particleColumns[i][j].n_mothers == 2){
                for(l = 0; l < _js->particleColumns[_js->particleColumns[i][j].mother2->depth - 1].size(); ++l) {
                    if (_js->particleColumns[_js->particleColumns[i][j].mother2->depth - 1][l] ==
                        *_js->particleColumns[i][j].mother2) {
                        std::cout << "from " << i << ";"<< j << " to "  << _js->particleColumns[i][j].mother2->depth - 1 << ";"<< l << std::endl;
                        this->createArrow(100 + i * 200, 150 + j * 160, 250 + (_js->particleColumns[i][j].mother2->depth - 1) * 200, 150 + l * 160);
                        break;
                    }
                }
            }
        }
    }
}

void blocks::createBlock(const unsigned int x, const unsigned int y, const unsigned int id, const unsigned int status, const unsigned int evi) {
    block b(x, y, id, status, evi);
    _blocks.push_back(b);
}

void blocks::createArrow(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2) {
    arrow a(x1, y1, x2, y2);
    _arrows.push_back(a);
}

void blocks::createFile() {
    std::remove(path.c_str());

    // Create an HTML file
   htmlFile = new std::ofstream("../results/block_diagram.html");

    if (!htmlFile) {
        std::cerr << "Error creating file." << std::endl;
        return;
    }
    *htmlFile<< R"(
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
                svg {
                    overflow: visible;
                }

            </style>
</head>
<body>
    <svg viewBox="0 0 1980 1080" style="width: 100%; height: 100%;">
)";

}

void blocks::printToFile() {
    for(auto & b : _blocks){
        *htmlFile << b.print();
    }
    for(auto & a : _arrows){
        *htmlFile << a.print();
    }
}
