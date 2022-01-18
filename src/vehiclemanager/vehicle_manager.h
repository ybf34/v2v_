#ifndef VEHICLE_MANAGER_H_
#define VEHICLE_MANAGER_H_

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "vehiclemanager/concurrent_object.h"
#include "object_holder.h"
#include "mapping/coordinate.h"
#include "mapping/route_model.h"
#include "map_object/vehicle.h"
#include "routing/route_planner.h"
#include "map_object/maillage.h"

namespace v2v
{

  class VehicleManager : public ConcurrentObject, public ObjectHolder
  {
  public:
    Maillage maillage;

    VehicleManager(RouteModel *model, std::shared_ptr<RoutePlanner> route_planner, int max_objects);

    const std::unordered_map<int, std::shared_ptr<Vehicle>> &Vehicles() { return vehicles_; }

    void checkVehicleInHexagons(Vehicle v);
    void Simulate();

  private:
    void GenerateNew();

    void Drive();

    void ResetVehicleDestination(std::shared_ptr<Vehicle> vehicle, bool random);

    void SimpleVehicleFailure(std::shared_ptr<Vehicle> vehicle);

    std::unordered_map<int, std::shared_ptr<Vehicle>> vehicles_;

    std::unordered_map<int, Coordinate> new_assignment_locations;
    std::vector<int> to_remove_;
  };

}

#endif // VEHICLE_MANAGER_H_
