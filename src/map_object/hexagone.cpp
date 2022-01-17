

#include <math.h>
#include "hexagone.h"
#include "point.h"


namespace rideshare {

void Hexagon::calculateVertices(int s){

    int a = sqrt(3)*(s/2);

    this->sommets.push_back(Point(this->getX(), this->getY() - s));
    this->sommets.push_back(Point(this->getX() + a, this->getY() - (s/2)));
    this->sommets.push_back(Point(this->getX() + a,this->getY() + (s/2)));
    this->sommets.push_back(Point(this->getX(), this->getY() + s));
    this->sommets.push_back(Point(this->getX() - a, this->getY() + (s/2)));
    this->sommets.push_back(Point(this->getX() - a, this->getY() - (s/2)));
    
   /* this->sommets.push_back(Point(this->getX() + (s/2), this->getY() - a));
    this->sommets.push_back(Point(this->getX() + s, this->getY()));
    this->sommets.push_back(Point(this->getX() + (s/2),this->getY() + a));
    this->sommets.push_back(Point(this->getX() - (s/2), this->getY() + a));
    this->sommets.push_back(Point(this->getX() - s, this->getY()));
    this->sommets.push_back(Point(this->getX() - (s/2), this->getY() - a));*/

    }


}
