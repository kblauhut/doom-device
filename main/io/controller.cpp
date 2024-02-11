#include "controller.h"


void set_up_controller()
{
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask = (1ULL << GPIO_FORWARD) | (1ULL << GPIO_LEFT) | (1ULL << GPIO_RIGHT) | (1ULL << GPIO_FIRE);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;

    gpio_config(&io_conf);
}

void read_controller_input(struct controller_input *input)
{


    input->forward = gpio_get_level(GPIO_FORWARD);
    input->left = gpio_get_level(GPIO_LEFT);
    input->right = gpio_get_level(GPIO_RIGHT);
    input->fire = 0;
    // input->fire = gpio_get_level(GPIO_FIRE);
}