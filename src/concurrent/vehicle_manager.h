/**
 * @file vehicle_manager.h
 * @brief Handles generation of and driving of vehicles around the map.
 *
 * @copyright Copyright (c) 2021, Michael Virgo, released under the MIT License.
 *
 */

#ifndef VEHICLE_MANAGER_H_
#define VEHICLE_MANAGER_H_

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "concurrent_object.h"
#include "object_holder.h"
#include "mapping/coordinate.h"
#include "mapping/route_model.h"
#include "map_object/vehicle.h"
#include "routing/route_planner.h"

// Avoid circular includes
namespace rideshare {
    class RideMatcher;
}

namespace rideshare {

class VehicleManager : public ConcurrentObject, public ObjectHolder {
  public:

    VehicleManager(RouteModel *model, std::shared_ptr<RoutePlanner> route_planner, int max_objects);
    
    const std::unordered_map<int, std::shared_ptr<Vehicle>>& Vehicles() { return vehicles_; }
    
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

}  // namespace rideshare

#endif // VEHICLE_MANAGER_H_
