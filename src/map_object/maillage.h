#ifndef MAILLAGE_H_
#define MAILLAGE_H_

#include "hexagone.h"
#include "vehicle.h"
#include <vector>
namespace v2v
{

    class Maillage
    {

    public:
        int s, maxI, maxJ;

        Maillage(int s_, int maxi, int maxj) : hex_grid(20, std::vector<Hexagon>(20, Hexagon(0, 0, 0, 0))), s(s_), maxI(maxi), maxJ(maxj) {}

        void createMaillage();
        void removeVehicleFromTheOtherHex(Vehicle v);

        std::vector<std::vector<Hexagon>> hex_grid;
    };

}

#endif