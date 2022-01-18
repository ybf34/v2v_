
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

#include "vehiclemanager//vehicle_manager.h"
#include "mapping/route_model.h"
#include "routing/route_planner.h"
#include "visual/graphics.h"
#include "map_object/hexagone.h"

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if (!is)
        return std::nullopt;

    auto size = is.tellg();
    std::vector<std::byte> contents(size);

    is.seekg(0);
    is.read((char *)contents.data(), size);

    if (contents.empty())
        return std::nullopt;
    return std::move(contents);
}

int main(int argc, char *argv[])
{

    srand((unsigned)time(NULL));
    int nb_vehicles = 10;

    const std::string osm_data_file = "../data/mulhouse_data.osm";
    std::vector<std::byte> osm_data;

    if (osm_data.empty() && !osm_data_file.empty())
    {
        auto data = ReadFile(osm_data_file);
        osm_data = std::move(*data);
    }

    v2v::RouteModel model{osm_data};

    std::shared_ptr<v2v::RoutePlanner> route_planner =
        std::make_shared<v2v::RoutePlanner>(model);

    std::shared_ptr<v2v::VehicleManager> vehicles =
        std::make_shared<v2v::VehicleManager>(&model, route_planner, nb_vehicles);

    vehicles->Simulate();

    v2v::Graphics *graphics = new v2v::Graphics(model.MinLat(), model.MinLon(), model.MaxLat(), model.MaxLon());

    std::string background_img = "../data/map-mulhouse.png";
    graphics->SetBgFilename(background_img);
    graphics->SetVehicles(vehicles);
    graphics->Simulate();

    return 0;
}
