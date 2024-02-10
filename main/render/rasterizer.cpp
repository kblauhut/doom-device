#include "rasterizer.h"

// Convert RGB values to a single integer color value
uint16_t rgbColor(int r, int g, int b) {
    unsigned int rVal = static_cast<unsigned int>(std::round(std::max(std::min(r, 255), 0))) & 0xFF;
    unsigned int gVal = (static_cast<unsigned int>(std::round(std::max(std::min(g, 255), 0))) << 8) & 0xFF00;
    unsigned int bVal = (static_cast<unsigned int>(std::round(std::max(std::min(b, 255), 0))) << 16) & 0xFF0000;
    unsigned int aVal = (255 << 24) & 0xFF000000;

    return aVal + bVal + gVal + rVal;
}

// Rasterize a parallelogram within bounds
void rasterizeParallelogramInBounds(
        const std::vector<std::vector<float>>& parallelogramTopLine,
        const std::vector<std::vector<float>>& parallelogramBottomLine,
        const std::vector<std::vector<float>>& boundTopLine,
        const std::vector<std::vector<float>>& boundBottomLine,
        unsigned int color,
        bool enableShading
) {
    int minPolygonX = static_cast<int>(std::round(parallelogramTopLine[0][0]));
    int maxPolygonX = static_cast<int>(std::round(parallelogramTopLine[1][0]));

    int xDrawRangeStart = static_cast<int>(std::round(std::max(static_cast<float>(minPolygonX), boundTopLine[0][0])));
    int xDrawRangeEnd = static_cast<int>(std::round(std::min(static_cast<float>(maxPolygonX), boundTopLine[1][0])));

    double parallelogramTopLineSlope =
            (parallelogramTopLine[1][1] - parallelogramTopLine[0][1]) /
            (parallelogramTopLine[1][0] - parallelogramTopLine[0][0]);
    double parallelogramBottomLineSlope =
            (parallelogramBottomLine[1][1] - parallelogramBottomLine[0][1]) /
            (parallelogramBottomLine[1][0] - parallelogramBottomLine[0][0]);

    double boundTopLineSlope =
            (boundTopLine[1][1] - boundTopLine[0][1]) /
            (boundTopLine[1][0] - boundTopLine[0][0]);
    double boundBottomLineSlope =
            (boundBottomLine[1][1] - boundBottomLine[0][1]) /
            (boundBottomLine[1][0] - boundBottomLine[0][0]);

    int xParallelogramStartOffset =
            xDrawRangeStart - static_cast<int>(std::round(parallelogramTopLine[0][0]));
    int xBoundStartOffset = xDrawRangeStart - static_cast<int>(std::round(boundTopLine[0][0]));

    double boundMinY = boundTopLine[0][1] + boundTopLineSlope * xBoundStartOffset;
    double boundMaxY = boundBottomLine[0][1] + boundBottomLineSlope * xBoundStartOffset;
    double parallelogramStartY =
            parallelogramTopLine[0][1] +
            parallelogramTopLineSlope * xParallelogramStartOffset;
    double parallelogramEndY =
            parallelogramBottomLine[0][1] +
            parallelogramBottomLineSlope * xParallelogramStartOffset;

    for (int x = xDrawRangeStart; x < xDrawRangeEnd; x++) {
        parallelogramStartY += parallelogramTopLineSlope;
        parallelogramEndY += parallelogramBottomLineSlope;
        boundMinY += boundTopLineSlope;
        boundMaxY += boundBottomLineSlope;

        for (
                int y = static_cast<int>(std::round(std::max(parallelogramStartY, boundMinY)));
                y < static_cast<int>(std::round(std::min(parallelogramEndY, boundMaxY)));
                y++
                ) {
            int pixelIndex = x + y * SCREEN_WIDTH;

            RENDER_BUFFER[pixelIndex] = color;

            if (
                    enableShading &&
                    (y == static_cast<int>(std::round(std::max(parallelogramStartY, boundMinY))) ||
                     y == static_cast<int>(std::round(std::min(parallelogramEndY, boundMaxY))) ||
                     x == xDrawRangeStart ||
                     x == xDrawRangeEnd - 1)
                    ) {
                RENDER_BUFFER[pixelIndex] = rgbColor(0, 0, 0);
            }
        }
    }
}