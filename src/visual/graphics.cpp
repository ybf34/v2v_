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

namespace v2v
{

    Graphics::Graphics(float min_lat, float min_lon, float max_lat, float max_lon)
    {
        min_lat_ = min_lat;
        min_lon_ = min_lon;
        max_lat_ = max_lat;
        max_lon_ = max_lon;
    }

    void Graphics::Simulate()
    {
        this->LoadBackgroundImg();
        while (true)
        {

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            this->DrawSimulation();
        }
    }

    void Graphics::LoadBackgroundImg()
    {

        windowName_ = "V2V_s";
        cv::namedWindow(windowName_, cv::WINDOW_NORMAL);

        cv::Mat background = cv::imread(bgFilename_);
        images_.push_back(background);
        images_.push_back(background.clone());
        images_.push_back(background.clone());
    }

    void Graphics::DrawSimulation()
    {

        images_.at(1) = images_.at(0).clone();
        images_.at(2) = images_.at(0).clone();

        float img_rows = images_.at(0).rows;
        float img_cols = images_.at(0).cols;

        DrawMaillage();
        DrawVehicles(img_rows, img_cols);

        cv::imshow(windowName_, images_.at(2));
        cv::waitKey(33);
    }

    void Graphics::DrawMaillage()
    {

        vehicle_manager_->maillage.createMaillage();

        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
            {
                DrawHexagon(vehicle_manager_->maillage.hex_grid[i][j]);
            }
    }

    void Graphics::DrawHexagon(Hexagon h1)
    {

        if (h1.marqueb == false)
        {
            cv::line(images_.at(1), cv::Point(h1.sommets[0].d_x, h1.sommets[0].d_y), cv::Point(h1.sommets[1].d_x, h1.sommets[1].d_y), cv::Scalar(0, 0, 0), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[5].d_x, h1.sommets[5].d_y), cv::Point(h1.sommets[0].d_x, h1.sommets[0].d_y), cv::Scalar(0, 0, 0), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[4].d_x, h1.sommets[4].d_y), cv::Point(h1.sommets[5].d_x, h1.sommets[5].d_y), cv::Scalar(0, 0, 0), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[3].d_x, h1.sommets[3].d_y), cv::Point(h1.sommets[4].d_x, h1.sommets[4].d_y), cv::Scalar(0, 0, 0), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[2].d_x, h1.sommets[2].d_y), cv::Point(h1.sommets[3].d_x, h1.sommets[3].d_y), cv::Scalar(0, 0, 0), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[1].d_x, h1.sommets[1].d_y), cv::Point(h1.sommets[2].d_x, h1.sommets[2].d_y), cv::Scalar(0, 0, 0), 2);
        }
        else
        {
            cv::line(images_.at(1), cv::Point(h1.sommets[0].d_x, h1.sommets[0].d_y), cv::Point(h1.sommets[1].d_x, h1.sommets[1].d_y), cv::Scalar(0, 0, 255), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[5].d_x, h1.sommets[5].d_y), cv::Point(h1.sommets[0].d_x, h1.sommets[0].d_y), cv::Scalar(0, 0, 255), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[4].d_x, h1.sommets[4].d_y), cv::Point(h1.sommets[5].d_x, h1.sommets[5].d_y), cv::Scalar(0, 0, 255), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[3].d_x, h1.sommets[3].d_y), cv::Point(h1.sommets[4].d_x, h1.sommets[4].d_y), cv::Scalar(0, 0, 255), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[2].d_x, h1.sommets[2].d_y), cv::Point(h1.sommets[3].d_x, h1.sommets[3].d_y), cv::Scalar(0, 0, 255), 2);
            cv::line(images_.at(1), cv::Point(h1.sommets[1].d_x, h1.sommets[1].d_y), cv::Point(h1.sommets[2].d_x, h1.sommets[2].d_y), cv::Scalar(0, 0, 255), 2);
        }
    }
    void Graphics::DrawVehicles(float img_rows, float img_cols)
    {

        for (auto const &[id, vehicle] : vehicle_manager_->Vehicles())
        {
            Coordinate position = vehicle->GetPosition();

            position.x = (position.x - min_lon_) / (max_lon_ - min_lon_);
            position.y = (max_lat_ - position.y) / (max_lat_ - min_lat_);

            cv::Scalar color = cv::Scalar(vehicle->Blue(), vehicle->Green(), vehicle->Red());

            vehicle_manager_->checkVehicleInHexagons(*vehicle);

            cv::circle(images_.at(1), cv::Point((int)(position.x * img_cols), (int)(position.y * img_rows)), vehicle->radio_wave * 10, cv::Scalar(225, 105, 65), 2);
            cv::circle(images_.at(1), cv::Point((int)(position.x * img_cols), (int)(position.y * img_rows)), 17, color, -1);
        }

        float opacity = 0.85;
        cv::addWeighted(images_.at(1), opacity, images_.at(0), 1.0 - opacity, 0, images_.at(2));
    }

}
