
#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <cstddef>

#include "coordinate.h"

namespace v2v
{

    class Model
    {
    public:
        struct Node
        {
            double x = 0.f;
            double y = 0.f;
        };

        struct Way
        {
            std::vector<int> nodes;
        };

        struct Road
        {
            enum Type
            {
                Invalid,
                Unclassified,
                Service,
                Residential,
                Tertiary,
                Secondary,
                Primary,
                Trunk,
                Motorway
            };
            int way;
            Type type;
        };

        Model(const std::vector<std::byte> &xml);

        auto MetricScale() const noexcept { return metric_scale_; }

        auto &Nodes() const noexcept { return nodes_; }
        auto &Ways() const noexcept { return ways_; }
        auto &Roads() const noexcept { return roads_; }
        auto &MinLat() const noexcept { return min_lat_; }
        auto &MaxLat() const noexcept { return max_lat_; }
        auto &MinLon() const noexcept { return min_lon_; }
        auto &MaxLon() const noexcept { return max_lon_; }

        Coordinate GetRandomMapPosition() const noexcept;

    private:
        void LoadData(const std::vector<std::byte> &xml);

        std::vector<Node> nodes_;
        std::vector<Way> ways_;
        std::vector<Road> roads_;

        double min_lat_ = 0.;
        double max_lat_ = 0.;
        double min_lon_ = 0.;
        double max_lon_ = 0.;
        double metric_scale_ = 1.f;
    };

}

#endif
