/**
 * @file vehicle.h
 * @brief Vehicle to request, pick up and drop off passengers; shown on the map.
 *
 * @copyright Copyright (c) 2021, Michael Virgo, released under the MIT License.
 *
 */

#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <memory>
#include <mutex>
#include <vector>

#include "map_object.h"
#include "mapping/coordinate.h"
#include "mapping/route_model.h"


namespace rideshare {


class Vehicle: public MapObject {
  public:
  
    Vehicle(double distance_per_cycle) : MapObject(distance_per_cycle) {}

    int Shape() { return shape_; }

    int PathIndex() { return path_index_; }

    void SetPosition(const Coordinate &position);
    void SetDestination(const Coordinate &destination);

    void IncrementalMove();
    void IncrementPathIndex() { ++path_index_; }

  private:
  
    void ResetPathAndIndex();

    int puissance_radio = 5;
    int shape_ = DrawMarker::diamond;
    int path_index_ = 0;
};

} 

#endif  // VEHICLE_H_
