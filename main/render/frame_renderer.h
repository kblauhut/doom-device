#ifndef FRAME_RENDERER_H
#define FRAME_RENDERER_H

#include "../game/player.h"
#include "../game/crosshair.h"
#include "../io/display.h"
#include "portal_renderer.h"
#include "render_config.h"

#define MAX_PORTAL_RENDERS 32

void renderFrame(Player &player, double delta);

#endif