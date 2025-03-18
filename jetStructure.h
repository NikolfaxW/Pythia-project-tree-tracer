//
// Created by Nikol on 11/20/2024.
//

#ifndef PYTHIA_PROJECT_TREE_CREATOR_JETSTRUCTURE_H
#define PYTHIA_PROJECT_TREE_CREATOR_JETSTRUCTURE_H

#include "Pythia8/Pythia.h"

#include <vector>

struct particle{
//    Pythia8::Particle& p;
    int _id, _st, _Ei,  n_mothers;
    particle * mother1 = nullptr, * mother2 = nullptr;

    double depth;

    void printParticle(){
        std::cout << "[id : " << _id << " ; st : " << _st << " ; Ei : " << _Ei << " ; depth : " << depth << "]";
    }
    particle& operator=(const particle& other) {
        if (this != &other) {
            _id = other._id;
            _st = other._st;
            _Ei = other._Ei;
            depth = other.depth;
        }
        return *this;
    }
    bool operator==(const particle& other) {
        if (this == &other) {
            return true;
        }
        return false;
    }
};

class jetStructure {
public:
    std::vector<std::vector<particle>> particleColumns;

    jetStructure();

    //    void addParticle(Pythia8::Particle& p, double depth);
    void pushBackParticle(particle & p);
    void addParticle(int id, int st, int Ei, int depth, int n_mothers, particle *& mother1, particle *& mother2);
    void addParticle(particle & p);
    void print();
};


#endif //PYTHIA_PROJECT_TREE_CREATOR_JETSTRUCTURE_H
