#ifndef PORTAL_RENDERER_H
#define PORTAL_RENDERER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../io/display.h"
#include "../game/player.h"
#include "../game/map.h"
#include "../math/intersect.h"
#include "../math/math.h"
#include "render_util.h"
#include "rasterizer.h"
#include "render_config.h"

struct Portal {
    Sector* sector;
    int previousSectorId;
    std::vector<float> frustumLeft;
    std::vector<float> frustumRight;
    std::vector<std::vector<float>> renderBoundTop;
    std::vector<std::vector<float>> renderBoundBottom;
};

void renderPortal(Player& player, Portal& portal, std::vector<Portal>& portalQueue);


#endif 