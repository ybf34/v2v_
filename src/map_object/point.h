#ifndef POINT_H
#define POINT_H

namespace v2v
{

    class Point
    {

    public:
        int d_x, d_y;

        Point(int x, int y) : d_x{x}, d_y{y} {}
    };

}

#endif