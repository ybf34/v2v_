
#ifndef COORDINATE_H_
#define COORDINATE_H_

namespace  v2v {

struct Coordinate {
    double x;
    double y;

    bool operator==(const Coordinate &other_coord) const {
        return x == other_coord.x && y == other_coord.y;
    }
};

}  // namespace  v2v

#endif  // COORDINATE_H_
