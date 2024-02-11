#ifndef RAIL_H
#define RAIL_H

#include <vector>
#include "player.h"
#include "../io/display.h"
#include "../render/render_util.h"
#include "../render/render_config.h"
#include "../render/rasterizer.h"

class Rail
{
private:
public:
    void render(
        int tick_count,
        Player* player,
        std::vector<float> frustum_left,
        std::vector<float> frustum_right
    );

    // Constructor
    Rail(
        int creation_tick_count,
        float height,
        std::vector<float> origin,
        std::vector<float> destination
    );

    int creation_tick_count;
    float height;
    std::vector<float> origin;
    std::vector<float> destination;
};

#endif // RAIL_H