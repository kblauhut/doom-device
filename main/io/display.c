/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_lcd_panel_io_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

#include "esp_lcd_ili9341.h"

#include "display.h"

#define TEST_LCD_HOST SPI2_HOST
#define TEST_LCD_BIT_PER_PIXEL (16)

#define TEST_PIN_NUM_LCD_CS (GPIO_NUM_2)
#define TEST_PIN_NUM_LCD_PCLK (GPIO_NUM_14)
#define TEST_PIN_NUM_LCD_DATA0 (GPIO_NUM_13)
#define TEST_PIN_NUM_LCD_DC (GPIO_NUM_2)
#define TEST_PIN_NUM_LCD_RST (GPIO_NUM_1)
#define TEST_PIN_NUM_LCD_BL (GPIO_NUM_16)

static char *TAG = "ili9341";
static SemaphoreHandle_t refresh_finish = NULL;
static esp_lcd_panel_handle_t panel_handle;
static esp_lcd_panel_io_handle_t io_handle = NULL;

extern uint16_t RENDER_BUFFER[SCREEN_HEIGHT * SCREEN_WIDTH] = {0};

IRAM_ATTR static bool
notify_refresh_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    BaseType_t need_yield = pdFALSE;

    xSemaphoreGiveFromISR(refresh_finish, &need_yield);
    return (need_yield == pdTRUE);
}

void draw_render_buffer_to_display()
{
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RENDER_BUFFER));
    xSemaphoreTake(refresh_finish, portMAX_DELAY);
}

void init_display()
{
    // BACKLIGHT
    ESP_LOGI(TAG, "Turn on the backlight");
    gpio_config_t io_conf = {
        .pin_bit_mask = BIT64(TEST_PIN_NUM_LCD_BL),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(TEST_PIN_NUM_LCD_BL, 1);

    // SPI
    ESP_LOGI(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = ILI9341_PANEL_BUS_SPI_CONFIG(TEST_PIN_NUM_LCD_PCLK, TEST_PIN_NUM_LCD_DATA0,
                                                                 SCREEN_HEIGHT * 80 * TEST_LCD_BIT_PER_PIXEL / 8);
    ESP_ERROR_CHECK(spi_bus_initialize(TEST_LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // IO
    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_spi_config_t io_config = ILI9341_PANEL_IO_SPI_CONFIG(TEST_PIN_NUM_LCD_CS, TEST_PIN_NUM_LCD_DC,
                                                                          notify_refresh_ready, NULL);
    // io_config.pclk_hz = 80 * 1000 * 1000;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)TEST_LCD_HOST, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install ili9341 panel driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TEST_PIN_NUM_LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = TEST_LCD_BIT_PER_PIXEL,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));

    // Create a semaphore to notify the refresh ready
    refresh_finish = xSemaphoreCreateBinary();
}

void uninstall_display()
{
    gpio_set_level(TEST_PIN_NUM_LCD_BL, 0);
    ESP_ERROR_CHECK(esp_lcd_panel_del(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_io_del(io_handle));
    ESP_ERROR_CHECK(spi_bus_free(TEST_LCD_HOST));

    vSemaphoreDelete(refresh_finish);
    refresh_finish = NULL;
}