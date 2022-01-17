#ifndef HEXAGON_H_
#define HEXAGON_H_


#include <memory>
#include <mutex>
#include <vector>
#include "point.h"

namespace rideshare {

class Hexagon{
 
    public:

    Point center;
    std::vector<Point> sommets;
    int a;
    int i,j;

    Hexagon(int x,int y,int i_,int j_) : sommets(),center(Point(x,y)),i(i_),j(j_) {}
    Hexagon();
    
    bool InsideHexagon(double x, double y);
    void setCenter(int x,int y){ center.d_x=x; center.d_y=y;}
    void calculateVertices(int s);

    int getX(){return center.d_x;}
    int getY(){return center.d_y;}

};

}

#endif 