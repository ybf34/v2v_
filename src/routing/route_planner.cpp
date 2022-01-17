#include "route_planner.h"

#include <algorithm>
#include <memory>
#include <mutex>

#include "mapping/route_model.h"
#include "map_object/map_object.h"

namespace  v2v {


float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->Distance(*end_node_);
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
 
    current_node->FindNeighbors();
   
    for (auto node: current_node->neighbors_) {

        node->parent_ = current_node;
        node->h_value_ = CalculateHValue(node);
        node->g_value_ = current_node->g_value_ + node->Distance(*current_node); 
       
        this->open_list_.emplace_back(node);
        node->visited_ = true;
    }
}

bool RoutePlanner::Compare(RouteModel::Node* node1, RouteModel::Node* node2) {
  
    return (node1->g_value_ + node1->h_value_) > (node2->g_value_ + node2->h_value_);
}

RouteModel::Node *RoutePlanner::NextNode() {
  
    std::sort(open_list_.begin(), open_list_.end(), Compare);

    auto current = open_list_.back();
    open_list_.pop_back();

    return current;
}

std::vector<Model::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    std::vector<Model::Node> path_found;
    
    RouteModel::Node follow_node = *current_node;
    path_found.emplace_back(follow_node); 
    while (follow_node.parent_ != nullptr) {
        
        follow_node = *(follow_node.parent_);

        path_found.emplace_back(follow_node);
    }

    std::reverse(path_found.begin(), path_found.end());

    return path_found;
}

void RoutePlanner::AStarSearch(std::shared_ptr<MapObject> map_obj) {
    RouteModel::Node *current_node = nullptr;

    auto start_pos = map_obj->GetPosition();
    auto dest_pos = map_obj->GetDestination();

    std::lock_guard<std::mutex> lck(mtx_);

    this->start_node_ = &model_.FindClosestNode(start_pos);
    this->end_node_ = &model_.FindClosestNode(dest_pos);

    start_node_->visited_ = true;
    open_list_.emplace_back(start_node_);


    while (open_list_.size() > 0) {

        current_node = NextNode();
    
        if (current_node->x == end_node_->x && current_node->y == end_node_->y) {
            map_obj->SetPath(ConstructFinalPath(current_node));
            break; 
        }
        AddNeighbors(current_node);
    }
    open_list_.clear();
    model_.ResetNodes();
}

}
