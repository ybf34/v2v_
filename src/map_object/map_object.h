#ifndef MAP_OBJECT_H_
#define MAP_OBJECT_H_

#include <cmath>
#include <cstdlib>
#include <vector>

#include "mapping/coordinate.h"
#include "mapping/model.h"

namespace v2v
{

  class MapObject
  {
  public:
    MapObject(double distance_per_cycle) : distance_per_cycle_(distance_per_cycle)
    {
      SetRandomColors();
    }

    void SetPosition(const Coordinate &position) { position_ = position; }
    void SetDestination(const Coordinate &destination) { destination_ = destination; }
    void SetColors(int blue, int green, int red)
    {
      blue_ = blue;
      green_ = green;
      red_ = red;
    }
    void SetId(int id) { id_ = id; }
    void SetPath(std::vector<Model::Node> path) { path_ = path; }
    Coordinate GetPosition() { return position_; }
    Coordinate GetDestination() { return destination_; }
    int Blue() { return blue_; }
    int Green() { return green_; }
    int Red() { return red_; }
    int Id() { return id_; }
    std::vector<Model::Node> Path() { return path_; }

    virtual void IncrementalMove(){};

    bool MovementFailure()
    {
      ++failures_;
      return failures_ >= MAX_FAILURES_;
    }

  protected:
    Coordinate GetIntermediatePosition(double next_x, double next_y)
    {
      double angle = std::atan2(next_y - position_.y, next_x - position_.x);
      double new_pos_x = position_.x + (distance_per_cycle_ * std::cos(angle));
      double new_pos_y = position_.y + (distance_per_cycle_ * std::sin(angle));
      return (Coordinate){.x = new_pos_x, .y = new_pos_y};
    }

    int id_;
    int failures_ = 0;
    const double distance_per_cycle_;
    int MAX_FAILURES_ = 10;
    Coordinate position_;
    Coordinate destination_;
    int blue_, green_, red_;
    std::vector<Model::Node> path_;

  private:
    void SetRandomColors()
    {
      blue_ = (int)(((float)rand() / RAND_MAX) * 255);
      green_ = (int)(((float)rand() / RAND_MAX) * 255);
      red_ = (int)(((float)rand() / RAND_MAX) * 255);
    }
  };

} // namespace  v2v

#endif // MAP_OBJECT_H_
