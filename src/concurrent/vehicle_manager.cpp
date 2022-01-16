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
    auto start = model_->GetRandomMapPosition();
    auto destination = model_->GetRandomMapPosition();
    
    auto nearest_start = model_->FindClosestNode(start);
    auto nearest_dest = model_->FindClosestNode(destination);

    std::shared_ptr<Vehicle> vehicle = std::make_shared<Vehicle>(distance_per_cycle_);

    vehicle->SetPosition((Coordinate){.x = nearest_start.x, .y = nearest_start.y});
    vehicle->SetDestination((Coordinate){.x = nearest_dest.x, .y = nearest_dest.y});
    vehicle->SetId(idCnt_++);

    vehicles_.emplace(vehicle->Id(), vehicle);

    std::lock_guard<std::mutex> lck(mtx_);
    std::cout << "Vehicle #" << idCnt_ - 1 << " now driving from: " << nearest_start.y << ", " << nearest_start.x << "." << std::endl;
}

void VehicleManager::ResetVehicleDestination(std::shared_ptr<Vehicle> vehicle, bool random) {
    Coordinate destination;
    if (random) {
        destination = model_->GetRandomMapPosition();
    } else {
        destination = vehicle->GetDestination();
    }
    auto nearest_dest = model_->FindClosestNode(destination);
    vehicle->SetDestination((Coordinate){.x = nearest_dest.x, .y = nearest_dest.y});
}

void VehicleManager::Simulate() {
    threads.emplace_back(std::thread(&VehicleManager::Drive, this));
}

void VehicleManager::SimpleVehicleFailure(std::shared_ptr<Vehicle> vehicle) {
    // Note: This should only be called when vehicle has not yet picked up a passenger
    // Check if enough failures to delete
    bool remove = vehicle->MovementFailure();
    if (remove) {
        // Plan to erase the vehicle
        to_remove_.emplace_back(vehicle->Id());
        // Note to console
        std::lock_guard<std::mutex> lck(mtx_);
        std::cout << "Vehicle #" << vehicle->Id() <<" is stuck, leaving map." << std::endl;
    } else {
        // Try a new route
        ResetVehicleDestination(vehicle, true);
    }
}

void VehicleManager::Drive() {
    while (true) {

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        for (auto & [id, vehicle] : vehicles_) {     
            if (vehicle->Path().empty()) {           
                route_planner_->AStarSearch(vehicle);     
                if (vehicle->Path().empty()) {
                        SimpleVehicleFailure(vehicle);
                        continue;
                }          
            }
            vehicle->IncrementalMove();   

            if (vehicle->GetPosition() == vehicle->GetDestination()) {               
                ResetVehicleDestination(vehicle, true); 
            }

        }

          if (to_remove_.size() > 0) {
            for (int id : to_remove_) {
               
                vehicles_.erase(id);
            }
            // Clear the to_remove_ vector for next time
            to_remove_.clear();
        }

        // Make sure to keep max vehicles on the road
        if (vehicles_.size() < MAX_OBJECTS_) {
            GenerateNew();
        }
    }
}


}
