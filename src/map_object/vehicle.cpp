
#include "vehicle.h"

#include <memory>

#include "mapping/coordinate.h"
#include "mapping/model.h"


namespace  v2v {

void Vehicle::SetPosition(const Coordinate &position) {
    position_ = position;
}

void Vehicle::SetDestination(const Coordinate &destination) {
    destination_ = destination;
   
    ResetPathAndIndex();
}

void Vehicle::IncrementalMove() {
    Model::Node next_pos = path_.at(path_index_);
   
    double distance = std::sqrt(std::pow(next_pos.x - position_.x, 2) + std::pow(next_pos.y - position_.y, 2));

    if (distance <= distance_per_cycle_) {
        SetPosition((Coordinate){.x = next_pos.x, .y = next_pos.y});
        IncrementPathIndex();
    } else {
        SetPosition(GetIntermediatePosition(next_pos.x, next_pos.y));
    }
}

void Vehicle::ResetPathAndIndex() {
    path_.clear();
    path_index_ = 0;
}

}  // namespace  v2v
