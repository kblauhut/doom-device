#include "io/display.h"
#include "game/player.h"
#include "render/frame_renderer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static char *TAG = "main";

extern "C" void app_main(void)
{
   
    Player *player = new Player();   
    init_display();
    // int count = SCREEN_HEIGHT * SCREEN_WIDTH;

    // // //PINK
    // // for (size_t i = 0; i < count; i++)
    // // {
    // //         RENDER_BUFFER[i] = 0xA8F0;
    // // }
    // draw_render_buffer_to_display();
    // vTaskDelay(1000 / portTICK_PERIOD_MS);

    // // renderFrame(*player, 0);
    // // draw_render_buffer_to_display();
    // // vTaskDelay(10000 / portTICK_PERIOD_MS);
    
    // // // PURPLE
    // // for (size_t i = 0; i < count; i++)
    // // {
    // //     RENDER_BUFFER[i] = 0xF0AA;
    // // }
    // player->position->at(0) = 2.0;
    // renderFrame(*player, 0);
    // draw_render_buffer_to_display();
    // vTaskDelay(1000 / portTICK_PERIOD_MS);


    TickType_t ticks0 =  xTaskGetTickCount();
    for (size_t i = 0; i < 100; i++)
    {
        player->position->at(0) = i * 0.1;
        renderFrame(*player, 0);
        draw_render_buffer_to_display();
    }
    

    TickType_t ticks1 =  xTaskGetTickCount();
    TickType_t ticks = ticks1 - ticks0;

    uninstall_display();

    ESP_LOGI(TAG, "Ticks: %ld %ld", ticks, ticks / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Ticks: %ld %ld", ticks0 ,ticks1);
    ESP_LOGI(TAG, "END");
}