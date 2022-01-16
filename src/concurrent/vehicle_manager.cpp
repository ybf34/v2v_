/**
 * @file vehicle_manager.cpp
 * @brief Implementation of generating vehicles and driving them around the map.
 *
 * @copyright Copyright (c) 2021, Michael Virgo, released under the MIT License.
 *
 */

#include "vehicle_manager.h"

#include <memory>

#include "mapping/coordinate.h"
#include "mapping/route_model.h"
#include "map_object/vehicle.h"
#include "routing/route_planner.h"

namespace rideshare {

VehicleManager::VehicleManager(RouteModel *model,
                               std::shared_ptr<RoutePlanner> route_planner,
                               int max_objects) : ObjectHolder(model, route_planner, max_objects) {

    distance_per_cycle_ = std::abs(model_->MaxLat() - model->MinLat()) / 1000.0;

    for (int i = 0; i < MAX_OBJECTS_; ++i) {
        GenerateNew();
    }
}

void VehicleManager::GenerateNew() {
    // Get random start position
    auto start = model_->GetRandomMapPosition();
    // Set a random destination until they have a passenger to go pick up
    auto destination = model_->GetRandomMapPosition();
    // Find the nearest road node to start and destination positions
    auto nearest_start = model_->FindClosestNode(start);
    auto nearest_dest = model_->FindClosestNode(destination);
    // Set road position, destination and id of vehicle
    std::shared_ptr<Vehicle> vehicle = std::make_shared<Vehicle>(distance_per_cycle_);
    vehicle->SetPosition((Coordinate){.x = nearest_start.x, .y = nearest_start.y});
    vehicle->SetDestination((Coordinate){.x = nearest_dest.x, .y = nearest_dest.y});
    vehicle->SetId(idCnt_++);
    vehicles_.emplace(vehicle->Id(), vehicle);
    // Output id and location of vehicle looking to give rides
    std::lock_guard<std::mutex> lck(mtx_);
    std::cout << "Vehicle #" << idCnt_ - 1 << " now driving from: " << nearest_start.y << ", " << nearest_start.x << "." << std::endl;
}

void VehicleManager::ResetVehicleDestination(std::shared_ptr<Vehicle> vehicle, bool random) {
    Coordinate destination;
    // Depending on `random`, either get a new random position or set current destination onto nearest node
    if (random) {
        destination = model_->GetRandomMapPosition();
    } else {
        destination = vehicle->GetDestination();
    }
    auto nearest_dest = model_->FindClosestNode(destination);
    vehicle->SetDestination((Coordinate){.x = nearest_dest.x, .y = nearest_dest.y});
}

void VehicleManager::Simulate() {
    // Launch Drive function in a thread
    threads.emplace_back(std::thread(&VehicleManager::Drive, this));
}

void VehicleManager::Drive() {
    while (true) {
        // Sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Drive the vehicles
        for (auto & [id, vehicle] : vehicles_) {
            // Get a route if none yet given
            if (vehicle->Path().empty()) {
                route_planner_->AStarSearch(vehicle);
                if (vehicle->Path().empty()) {
                    if (vehicle->State() == VehicleState::no_passenger_requested || vehicle->State() == VehicleState::no_passenger_queued) {
                        SimpleVehicleFailure(vehicle);
                        continue;
                    }
                }
            }
            vehicle->IncrementalMove();
            
            if (vehicle->GetPosition() == vehicle->GetDestination()) {               
                ResetVehicleDestination(vehicle, true); 
            }
        }
    }
}

void VehicleManager::SimpleVehicleFailure(std::shared_ptr<Vehicle> vehicle) {

    bool remove = vehicle->MovementFailure();
    if (remove) {
      
        to_remove_.emplace_back(vehicle->Id());

        std::lock_guard<std::mutex> lck(mtx_);
        std::cout << "Vehicle #" << vehicle->Id() <<" is stuck, leaving map." << std::endl;
    } else {
        // Try a new route
        ResetVehicleDestination(vehicle, true);
    }
}


}  // namespace rideshare
