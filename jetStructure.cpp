//
// Created by Nikol on 11/20/2024.
//

#include "jetStructure.h"

jetStructure::jetStructure() { //TODO
//    this->particles.reserve(100);
    particleColumns.reserve(10);
    for(int i = 0; i < particleColumns.size(); ++i)
        particleColumns[i].reserve(100);
}

void jetStructure::pushBackParticle(particle &p) {
    if(p.depth > particleColumns.size()){
        particleColumns.resize(p.depth, std::vector<particle> (0) );
    }
    particleColumns[p.depth - 1].push_back(p);
}

void jetStructure::addParticle(int id, int st, int Ei, int depth, int n_mothers, particle *& mother1, particle *& mother2) {
    particle p;
    p._id = id;
    p._st = st;
    p._Ei = Ei;
    p.depth = depth;
    p.mother1 = mother1;
    p.mother2 = mother2;
    pushBackParticle(p);
}


void jetStructure::print(){

    for(int i = 0; i < particleColumns.size(); ++i){
        for(int j = 0; j < particleColumns[i].size(); ++j){
            particleColumns[i][j].printParticle();

        }
        std::cout << std::endl;
    }
}

