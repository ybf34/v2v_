
#ifndef MAILLAGE_H_
#define MAILLAGE_H_

#include "hexagone.h"
#include <vector>
namespace rideshare{

class Maillage{

    public:


    int s,maxI,maxJ;

    Hexagon **hex_grid;
    //Maillage(int s_,int maxi,int maxj) : hex_grid{},s(s_),maxI(maxi),maxJ(maxj) {}
    Maillage(int s_,int maxi,int maxj);

    void createMaillage(int s);
    std::vector<std::vector<Hexagon>> hex_grid;
};

}

#endif