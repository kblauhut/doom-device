#include "game/player.h"
#include "render/frame_renderer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "io/display.h"
#include "io/controller.h"

static char *TAG = "main";

extern "C" void app_main(void)
{
    set_up_controller();
    init_display();

    controller_input input;
    Player *player = new Player();   

    TickType_t ticks = xTaskGetTickCount();
    std::vector<Rail> rails;

    rails.push_back(Rail(
        0,
        0.0,
        std::vector<float>{0.0, -10.0, 0.0},
        std::vector<float>{100.0, 0.0, 0.0}
    ));

    while (1)
    {
        TickType_t new_ticks = xTaskGetTickCount();
        int delta_ticks = new_ticks - ticks;
        ticks = new_ticks;
        read_controller_input(&input);
        player->update_player(&input, delta_ticks);
        renderFrame(*player, new_ticks, rails);
        draw_render_buffer_to_display();
    }

    uninstall_display();
}