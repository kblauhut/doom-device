#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <inttypes.h>
#include <vector>
#include <cmath>

#include "../io/display.h"

uint16_t rgbColor(int r, int g, int b);
void rasterizeParallelogramInBounds(
        const std::vector<std::vector<float>>& parallelogramTopLine,
        const std::vector<std::vector<float>>& parallelogramBottomLine,
        const std::vector<std::vector<float>>& boundTopLine,
        const std::vector<std::vector<float>>& boundBottomLine,
        unsigned int color,
        bool enableShading
);
void rasterizeRect(
        float x,
        float y,
        float width,
        float height,
        uint16_t color
);

#endif 