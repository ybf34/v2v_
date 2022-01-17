/**
 * @file graphics.cpp
 * @brief Implementation of drawing vehicles and passengers on a map image.
 *
 * @cite Adapted from https://github.com/udacity/CppND-Program-a-Concurrent-Traffic-Simulation
 * @copyright Copyright (c) 2021, Michael Virgo, released under the MIT License.
 *
 */

#include "graphics.h"

#include <chrono>
#include <memory>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "map_object/maillage.h"
#include "map_object/hexagone.h"
#include "mapping/coordinate.h"

namespace rideshare {

Graphics::Graphics(float min_lat, float min_lon, float max_lat, float max_lon) {
    min_lat_ = min_lat;
    min_lon_ = min_lon;
    max_lat_ = max_lat;
    max_lon_ = max_lon;
}

void Graphics::Simulate() {
    this->LoadBackgroundImg();
    while (true) {
       
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // update graphics
        this->DrawSimulation();
    }
}

void Graphics::LoadBackgroundImg() {
    // create window
    windowName_ = "V2V_s";
    cv::namedWindow(windowName_, cv::WINDOW_NORMAL);

    // load image and create copy to be used for semi-transparent overlay
    cv::Mat background = cv::imread(bgFilename_);
    images_.push_back(background);         // first element is the original background
    images_.push_back(background.clone()); // second element will be the transparent overlay
    images_.push_back(background.clone()); // third element will be the result image for display
}

void Graphics::DrawSimulation() {
    // reset images
    images_.at(1) = images_.at(0).clone();
    images_.at(2) = images_.at(0).clone();

    float img_rows = images_.at(0).rows;
    float img_cols = images_.at(0).cols;

    DrawMaillage();
    DrawVehicles(img_rows, img_cols);
   
    cv::imshow(windowName_, images_.at(2));
    cv::waitKey(33);
}

void Graphics::DrawMaillage(){
    Maillage maillage(90,20,20);
    maillage.createMaillage(90);
   
    
    /*for (int i=0; i<20; i++)
        for (int j=0; j<20; j++){
            std::cout << i << std::endl;
            DrawHexagon(maillage.hex_grid[i][j]);
  }*/
    /*for (int i=0; i < 6; i++) {
        std::cout << "i = " << i << "  " <<h1.sommets[i].d_x << " + " << h1.sommets[i].d_y << std::endl;
    }*/

    Hexagon h1(100,100,0,0);
    h1.calculateVertices(90);
    DrawHexagon(h1);
}

void Graphics::DrawHexagon(Hexagon h1){

    cv::line(images_.at(1),cv::Point(h1.sommets[0].d_x,h1.sommets[0].d_y),cv::Point(h1.sommets[1].d_x,h1.sommets[1].d_y),cv::Scalar(0,255,0),5);
    cv::line(images_.at(1),cv::Point(h1.sommets[5].d_x,h1.sommets[5].d_y),cv::Point(h1.sommets[0].d_x,h1.sommets[0].d_y),cv::Scalar(0,255,0),5);
    cv::line(images_.at(1),cv::Point(h1.sommets[4].d_x,h1.sommets[4].d_y),cv::Point(h1.sommets[5].d_x,h1.sommets[5].d_y),cv::Scalar(0,255,0),5);
    cv::line(images_.at(1),cv::Point(h1.sommets[3].d_x,h1.sommets[3].d_y),cv::Point(h1.sommets[4].d_x,h1.sommets[4].d_y),cv::Scalar(0,255,0),5);
    cv::line(images_.at(1),cv::Point(h1.sommets[2].d_x,h1.sommets[2].d_y),cv::Point(h1.sommets[3].d_x,h1.sommets[3].d_y),cv::Scalar(0,255,0),5);
    cv::line(images_.at(1),cv::Point(h1.sommets[1].d_x,h1.sommets[1].d_y),cv::Point(h1.sommets[2].d_x,h1.sommets[2].d_y),cv::Scalar(0,255,0),5);
    
}
void Graphics::DrawVehicles(float img_rows, float img_cols) {
    // create overlay from vehicles
    for (auto const & [id, vehicle] : vehicle_manager_->Vehicles()) {
        Coordinate position = vehicle->GetPosition();
    
        position.x = (position.x - min_lon_) / (max_lon_ - min_lon_);
        position.y = (max_lat_ - position.y) / (max_lat_ - min_lat_);

        cv::Scalar color = cv::Scalar(vehicle->Blue(),vehicle->Green(),vehicle->Red());
      // cv::circle(images_.at(1),cv::Point((int)(position.x * img_cols),(int)(position.y * img_rows)),50,color,-1);
        cv::drawMarker(images_.at(1), cv::Point2d((int)(position.x * img_cols), (int)(position.y * img_rows)), color, vehicle->Shape(), 25, 15);
    }

    float opacity = 0.85;
    cv::addWeighted(images_.at(1), opacity, images_.at(0), 1.0 - opacity, 0, images_.at(2));
}

}  // namespace rideshare
