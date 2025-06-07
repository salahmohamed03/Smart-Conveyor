#include "Gpio.h"
#include "ir_sensor.h"
#include "Gpio.h"
#include "Rcc.h"

static uint8 previous_state = 1;  // Assume the button is unpressed initially (HIGH)

void EdgeDetect_Init(void) {
    Rcc_Enable(RCC_GPIOC);
    Gpio_Init(GPIO_C, 1, GPIO_INPUT, GPIO_PULL_UP);  // Enable internal pull-up
}

uint8 EdgeDetect_CheckFallingEdge(void) {
    uint8 current_state = Gpio_ReadPin(GPIO_C, 1);
    uint8 falling_edge = (previous_state == 1 && current_state == 0);
    previous_state = current_state;
    return falling_edge;
}
