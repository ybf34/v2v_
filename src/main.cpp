/**
 * @file main.cpp
 * @brief Read OpenStreetMap data and simulate ridesharing on the input map.
 *
 * @cite OSM reading code adapted from https://github.com/udacity/CppND-Route-Planning-Project
 * @cite Simulation/drawing code adapted from https://github.com/udacity/CppND-Program-a-Concurrent-Traffic-Simulation
 * @copyright Copyright (c) 2021, Michael Virgo, released under the MIT License.
 *
 */

#include <cstdlib>
#include <ctime>
#include <optional>
#include <fstream>
#include <iostream>
#include <cmath>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "argparser/simple_parser.h"
#include "concurrent/passenger_queue.h"
#include "concurrent/ride_matcher.h"
#include "concurrent/vehicle_manager.h"
#include "mapping/route_model.h"
#include "routing/route_planner.h"
#include "visual/graphics.h"

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path) {   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if ( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if ( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}



int main(int argc, char *argv[]) {
    // Parse any arguments
    std::unordered_map<std::string, std::string> settings = rideshare::SimpleParser().ParseArgs(argc, argv);

    // Get map data
    const std::string osm_data_file = "../data/" + settings["map"] + ".osm";
    std::vector<std::byte> osm_data;
 
    if ( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if ( !data ) {
            std::cout << "Failed to read." << std::endl;
        } else {
            osm_data = std::move(*data);
        }
    }

    rideshare::RouteModel model{osm_data};

    srand((unsigned) time(NULL)); // Seed random number generator

    // Create a shared route planner
    std::shared_ptr<rideshare::RoutePlanner> route_planner =
      std::make_shared<rideshare::RoutePlanner>(model);

    // Create vehicles
    std::shared_ptr<rideshare::VehicleManager> vehicles =
      std::make_shared<rideshare::VehicleManager>(&model, route_planner, std::stoi(settings["vehicles"]));

    // Create passenger queue
    std::shared_ptr<rideshare::PassengerQueue> passengers =
      std::make_shared<rideshare::PassengerQueue>(&model, route_planner, std::stoi(settings["passengers"]),
                                                  std::stoi(settings["wait"]), std::stoi(settings["wait_range"]));

    // Calculate the average map dimension used by the ride matcher
    const double MAP_DIM = (std::abs(model.MaxLat() - model.MinLat()) + std::abs(model.MaxLon() - model.MinLon())) / 2.0;

    // Create the ride matcher
    std::shared_ptr<rideshare::RideMatcher> ride_matcher =
      std::make_shared<rideshare::RideMatcher>(passengers, vehicles, MAP_DIM, settings["match"]);

    // Attach ride matcher to the other two
    vehicles->SetRideMatcher(ride_matcher);
    passengers->SetRideMatcher(ride_matcher);

    // Start the simulations
    ride_matcher->Simulate();
    vehicles->Simulate();
    passengers->Simulate();

    // Draw the map
    rideshare::Graphics *graphics =
      new rideshare::Graphics(model.MinLat(), model.MinLon(), model.MaxLat(), model.MaxLon());
    std::string background_img = "../data/" + settings["map"] + ".png";
    graphics->SetBgFilename(background_img);
    graphics->SetPassengers(passengers);
    graphics->SetVehicles(vehicles);
    graphics->Simulate();

    return 0;
}
