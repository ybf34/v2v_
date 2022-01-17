
#ifndef MAILLAGE_H_
#define MAILLAGE_H_

#include "hexagone.h"
#include "vehicle.h"
#include <vector>
namespace rideshare{

class Maillage{

    public:
    int s,maxI,maxJ;

    
    Maillage(int s_,int maxi,int maxj) : hex_grid(20,std::vector<Hexagon>(20,Hexagon(0,0,0,0))),s(s_),maxI(maxi),maxJ(maxj) {}

    void createMaillage();

    std::vector<std::vector<Hexagon>> hex_grid;
    
};

}

#endif