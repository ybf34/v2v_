

#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <memory>
#include <mutex>
#include <vector>

#include "map_object.h"
#include "mapping/coordinate.h"
#include "mapping/route_model.h"

namespace v2v
{

  class Vehicle : public MapObject
  {
  public:
    Vehicle(double distance_per_cycle) : MapObject(distance_per_cycle) {}

    int PathIndex() { return path_index_; }

    void SetPosition(const Coordinate &position);
    void SetDestination(const Coordinate &destination);

    void IncrementalMove();
    void IncrementPathIndex() { ++path_index_; }
    int radio_wave = 5;

  private:
    void ResetPathAndIndex();

    int path_index_ = 0;
  };

}

#endif // VEHICLE_H_
