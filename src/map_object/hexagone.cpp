#include <math.h>
#include "hexagone.h"
#include "point.h"
#include "vehicle.h"

namespace v2v
{

    void Hexagon::calculateVertices(int s)
    {
        int a = sqrt(3) * (s / 2);
        this->sommets.push_back(Point(this->getX(), this->getY() - s));
        this->sommets.push_back(Point(this->getX() + a, this->getY() - (s / 2)));
        this->sommets.push_back(Point(this->getX() + a, this->getY() + (s / 2)));
        this->sommets.push_back(Point(this->getX(), this->getY() + s));
        this->sommets.push_back(Point(this->getX() - a, this->getY() + (s / 2)));
        this->sommets.push_back(Point(this->getX() - a, this->getY() - (s / 2)));
    }

    bool Hexagon::InsideHexagon(double testX, double testY)
    {
        int v = 6;
        bool c = false;
        for (int i = 0, j = v - 1; i < v; j = i++)
        {
            if (((sommets[i].d_y > testY) != (sommets[j].d_y > testY)) && (testX < (sommets[j].d_x - sommets[i].d_x) * (testY - sommets[i].d_y) / (sommets[j].d_y - sommets[i].d_y) + sommets[i].d_x))
                c = !c;
        }
        return c;
    }

    void Hexagon::addVehicle(Vehicle v)
    {
        this->vehicles.push_back(v);
    }

    void Hexagon::removeVehicle(Vehicle v)
    {
        this->marqueb = false;

        for (size_t i = 0; i < this->vehicles.size(); i++)
        {
            if (v == this->vehicles[i])
            {
            }
        }
        this->vehicles.clear();
    }

    bool Hexagon::hadVehicle(Vehicle v)
    {
        for (auto &vehicle : this->vehicles)
        {
            if (v == vehicle)
            {
                return true;
            }
        }

        return false;
    }
}
