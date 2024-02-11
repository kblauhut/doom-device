#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
#include <memory>

#include "map.h"
#include "../io/controller.h"
#include "../math/intersect.h"

class Player
{
private:
public:
    void update_player(controller_input *input, int delta_ticks);
    // Constructor
    Player();

    float yaw = 0.0;
    float forward_velocity = 0.0;
    std::unique_ptr<std::vector<float>> position;
    const Sector *current_sector;
};

#endif // PLAYER_H