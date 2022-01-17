

#ifndef ROUTE_PLANNER_H_
#define ROUTE_PLANNER_H_

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <string>

#include "mapping/route_model.h"
#include "map_object/map_object.h"

namespace rideshare {

class RoutePlanner {
  public:

    RoutePlanner(RouteModel &model) : model_(model) {};
  
    void AStarSearch(std::shared_ptr<MapObject> map_obj);

  private:

    std::vector<RouteModel::Node*> open_list_;
    RouteModel::Node *start_node_;
    RouteModel::Node *end_node_;

    std::mutex mtx_;
    RouteModel &model_;

  
    static bool Compare(RouteModel::Node* node1, RouteModel::Node* node2);

    void AddNeighbors(RouteModel::Node *current_node);
   
    float CalculateHValue(RouteModel::Node const *node);
   
    std::vector<Model::Node> ConstructFinalPath(RouteModel::Node *);
    RouteModel::Node *NextNode();
};

}  // namespace rideshare

#endif  // ROUTE_PLANNER_H_
