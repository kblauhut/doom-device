#include <vector>
#include <cmath>

#include "frame_renderer.h"
#include "render_config.h"

extern float DEPTH_BUFFER[SCREEN_WIDTH] = {0.0};

void renderFrame(Player &player, int tick_count, std::vector<Rail> rails) {  
    // Build the view frustum
    float frustumLeft[2] = {
            std::cos(player.yaw - FOV_RAD / 2),
            std::sin(player.yaw - FOV_RAD / 2)
    };

    float frustumRight[2] = {
            std::cos(player.yaw + FOV_RAD / 2),
            std::sin(player.yaw + FOV_RAD / 2)
    };

    std::vector<float> playerPosition = {(*player.position)[0], (*player.position)[1]};
    
    // Find the sector the player is in
    Sector initialSector;
    for (const Sector &s: MAP) {
        initialSector = s;
        if (isPointInPolygon(initialSector.vertices, playerPosition)) {
            break;
        }
    }


    const auto initialTopRenderBound = std::vector<std::vector<float>>{{{0, 0}, {float(SCREEN_WIDTH), 0}}};

    const auto initialBottomRenderBound = std::vector<std::vector<float>>{
            {{0, float(SCREEN_HEIGHT)}, {float(SCREEN_WIDTH), float(SCREEN_HEIGHT)}}};

    std::vector<float> nextFrustumLeft = {frustumLeft[0], frustumLeft[1]};
    std::vector<float> nextFrustumRight = {frustumRight[0], frustumRight[1]};


    Portal nextPortal = {
            const_cast<Sector *>(&initialSector),
            -1,
            nextFrustumLeft,
            nextFrustumRight,
            initialTopRenderBound,
            initialBottomRenderBound
    };
    // Set up the portal queue
    std::vector<Portal> portalQueue = {
            nextPortal
    };


// Temporary for debugging, reset the render buffer
   for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
       RENDER_BUFFER[i] = 0; 
   }


    int renderedPortals = 0;
    while (renderedPortals < MAX_PORTAL_RENDERS && !portalQueue.empty()) {
        Portal portal = portalQueue.back();
        portalQueue.pop_back();

        renderPortal(player, portal, portalQueue);
        renderedPortals++;
    }

    for (Rail &rail: rails) {
        rail.render(tick_count, &player, nextFrustumLeft, nextFrustumRight);
    }

    draw_crosshair();
}