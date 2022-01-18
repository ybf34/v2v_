#ifndef OBJECT_HOLDER_H_
#define OBJECT_HOLDER_H_

#include <memory>

#include "mapping/route_model.h"
#include "routing/route_planner.h"

namespace v2v
{

  class ObjectHolder
  {
  public:
    ObjectHolder(RouteModel *model, std::shared_ptr<RoutePlanner> route_planner,
                 int max_objects) : model_(model), route_planner_(route_planner), MAX_OBJECTS_(max_objects){};

  protected:
    virtual void GenerateNew(){};
    const int MAX_OBJECTS_;
    RouteModel *model_;
    double distance_per_cycle_;
    int idCnt_ = 0;
    std::shared_ptr<RoutePlanner> route_planner_;
  };

} // namespace  v2v

#endif // OBJECT_HOLDER_H_
