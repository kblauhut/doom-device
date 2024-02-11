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

Rail create_rail_from_player_pos(
        Player* player,
        int tick_count
);

#endif // RAIL_H