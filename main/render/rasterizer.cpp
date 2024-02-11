#include "rasterizer.h"

// Convert RGB values to a single integer color value
uint16_t rgbColor(int r, int g, int b) {
        uint16_t green = (g/8) & 0x1f;
        uint16_t red = ((r/16) & 0xf) << 6;
        uint16_t blue = ((b/16) & 0xf) << 11;
        return blue | red | green;
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

void rasterizeParallelogramDepthClip(
        const std::vector<std::vector<float>>& parallelogramTopLine,
        const std::vector<std::vector<float>>& parallelogramBottomLine,
        uint16_t color,
        float start_distance,
        float end_distance,
        float* depth_buffer
) {
    int minPolygonX = static_cast<int>(std::round(parallelogramTopLine[0][0]));
    int maxPolygonX = static_cast<int>(std::round(parallelogramTopLine[1][0]));

    int xDrawRangeStart = static_cast<int>(std::round(minPolygonX));
    int xDrawRangeEnd = static_cast<int>(std::round(maxPolygonX));

    double parallelogramTopLineSlope =
            (parallelogramTopLine[1][1] - parallelogramTopLine[0][1]) /
            (parallelogramTopLine[1][0] - parallelogramTopLine[0][0]);
    double parallelogramBottomLineSlope =
            (parallelogramBottomLine[1][1] - parallelogramBottomLine[0][1]) /
            (parallelogramBottomLine[1][0] - parallelogramBottomLine[0][0]);

    int xParallelogramStartOffset =
            xDrawRangeStart - static_cast<int>(std::round(parallelogramTopLine[0][0]));

    double parallelogramStartY =
            parallelogramTopLine[0][1] +
            parallelogramTopLineSlope * xParallelogramStartOffset;
    double parallelogramEndY =
            parallelogramBottomLine[0][1] +
            parallelogramBottomLineSlope * xParallelogramStartOffset;

    int point_count = xDrawRangeEnd - xDrawRangeStart;

    for (int x = xDrawRangeStart; x < xDrawRangeEnd; x++) {
        parallelogramStartY += parallelogramTopLineSlope;
        parallelogramEndY += parallelogramBottomLineSlope;

        // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation.html
        float lambda = (x - xDrawRangeStart) / point_count;
        float z = (1 / start_distance) * (1 - lambda) + (1 / end_distance) * lambda;
        float z_inverse = 1 / z;

        if (depth_buffer[x] < z_inverse) {
                continue;
        }

        for (
                int y = static_cast<int>(std::round(parallelogramStartY));
                y < static_cast<int>(std::round(parallelogramEndY));
                y++
        ) {
            int pixelIndex = x + y * SCREEN_WIDTH;
            RENDER_BUFFER[pixelIndex] = color;
        }
    }
}

void rasterizeRect(
        float x,
        float y,
        float width,
        float height,
        uint16_t color
) {
    int startX = static_cast<int>(std::round(x));
    int endX = static_cast<int>(std::round(x + width));

    int startY = static_cast<int>(std::round(y));
    int endY = static_cast<int>(std::round(y + height));

    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            int pixelIndex = x + y * SCREEN_WIDTH;

            RENDER_BUFFER[pixelIndex] = color;
        }
    }
}