#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <memory>
#include <string>
#include <vector>
#include <opencv2/core.hpp>

#include "vehiclemanager/vehicle_manager.h"
#include "map_object/hexagone.h"

namespace v2v
{

  class Graphics
  {
  public:
    // Constructor
    Graphics(float min_lat, float min_lon, float max_lat, float max_lon);

    // Setters
    void SetBgFilename(std::string filename) { bgFilename_ = filename; }
    void SetVehicles(const std::shared_ptr<VehicleManager> &vehicle_manager) { vehicle_manager_ = vehicle_manager; }

    // Concurrent drawing simulation
    void Simulate();

  private:
    void LoadBackgroundImg();

    void DrawSimulation();

    void DrawHexagon(Hexagon h1);

    void DrawMaillage();

    void DrawVehicles(float img_rows, float img_cols);

    float min_lat_, min_lon_, max_lat_, max_lon_;
    std::shared_ptr<VehicleManager> vehicle_manager_;

    std::string bgFilename_;
    std::string windowName_;
    std::vector<cv::Mat> images_;
  };

} // namespace  v2v

#endif // GRAPHICS_H_
