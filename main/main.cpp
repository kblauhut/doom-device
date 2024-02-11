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

    TickType_t last_rail_time = xTaskGetTickCount();
    TickType_t ticks = xTaskGetTickCount();
    std::vector<Rail> rails;
    
    while (1)
    {
        TickType_t new_ticks = xTaskGetTickCount();
        int delta_ticks = new_ticks - ticks;
        ticks = new_ticks;
        read_controller_input(&input);
        player->update_player(&input, delta_ticks);
        renderFrame(*player, new_ticks, rails);
        draw_render_buffer_to_display();

        // if (xTaskGetTickCount() - last_rail_time > 1000) {
        //     last_rail_time = xTaskGetTickCount();
        //     rails.push_back(create_rail_from_player_pos(
        //         player,
        //         ticks
        //     ));
        // }
    }

    uninstall_display();
}