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

#include "concurrent/vehicle_manager.h"
#include "mapping/route_model.h"
#include "routing/route_planner.h"
#include "visual/graphics.h"
#include "map_object/hexagone.h"

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

    
    srand((unsigned) time(NULL));

    int nb_vehicles = 2;
    
    const std::string osm_data_file = "../data/mulhouse_data.osm";
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

    std::shared_ptr<rideshare::RoutePlanner> route_planner =
      std::make_shared<rideshare::RoutePlanner>(model);

    std::shared_ptr<rideshare::VehicleManager> vehicles =
      std::make_shared<rideshare::VehicleManager>(&model, route_planner, nb_vehicles);

    vehicles->Simulate();

    rideshare::Graphics *graphics =
      new rideshare::Graphics(model.MinLat(), model.MinLon(), model.MaxLat(), model.MaxLon());


  
    std::string background_img = "../data/map-mulhouse.png";
    graphics->SetBgFilename(background_img);
    graphics->SetVehicles(vehicles);
    graphics->Simulate();
   

    return 0;
}

