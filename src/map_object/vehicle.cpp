
#include "vehicle.h"

#include <memory>

#include "mapping/coordinate.h"
#include "mapping/model.h"


namespace rideshare {

void Vehicle::SetPosition(const Coordinate &position) {
    position_ = position;
}

void Vehicle::SetDestination(const Coordinate &destination) {
    destination_ = destination;
    // Reset the path and index so will properly route on a new path
    ResetPathAndIndex();
}


void Vehicle::IncrementalMove() {
    Model::Node next_pos = path_.at(path_index_);
    // Check distance to next position vs. distance can go b/w timesteps
    double distance = std::sqrt(std::pow(next_pos.x - position_.x, 2) + std::pow(next_pos.y - position_.y, 2));

    if (distance <= distance_per_cycle_) {
        // Don't need to calculate intermediate point, just set position as next_pos
        SetPosition((Coordinate){.x = next_pos.x, .y = next_pos.y});
        IncrementPathIndex();
    } else {
        // Calculate an intermediate position
        SetPosition(GetIntermediatePosition(next_pos.x, next_pos.y));
    }
}

void Vehicle::ResetPathAndIndex() {
    path_.clear();
    path_index_ = 0;
}

}  // namespace rideshare
