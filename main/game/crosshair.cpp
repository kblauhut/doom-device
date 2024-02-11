#include "crosshair.h"



float screen_width_2 = SCREEN_WIDTH / 2;
float screen_height_2 = SCREEN_HEIGHT / 2;
float crosshair_size = 6;
float crosshair_thickness = 2;
float gap = 3;

void draw_crosshair() {
    rasterizeRect(
        screen_width_2 - crosshair_size - gap,
        screen_height_2 - crosshair_thickness / 2,
        crosshair_size,
        crosshair_thickness,
        rgbColor(255, 255, 255)
    );

    rasterizeRect(
        screen_width_2 + gap,
        screen_height_2 - crosshair_thickness / 2,
        crosshair_size,
        crosshair_thickness,
        rgbColor(255, 255, 255)
    );

    rasterizeRect(
        screen_width_2 - crosshair_thickness / 2,
        screen_height_2 - crosshair_size - gap,
        crosshair_thickness,
        crosshair_size,
        rgbColor(255, 255, 255)
    );

    rasterizeRect(
        screen_width_2 - crosshair_thickness / 2,
        screen_height_2 + gap,
        crosshair_thickness,
        crosshair_size,
        rgbColor(255, 255, 255)
    );
}