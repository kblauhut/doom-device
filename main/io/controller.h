#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "driver/gpio.h"

#define GPIO_FORWARD (GPIO_NUM_27)
#define GPIO_LEFT (GPIO_NUM_26)
#define GPIO_RIGHT (GPIO_NUM_25)
#define GPIO_FIRE (GPIO_NUM_23)

struct controller_input
{
    int forward;
    int left;
    int right;
    int fire;
};

void set_up_controller();
void read_controller_input(struct controller_input *input);

#endif