#include "ir_sensor.h"
#include "Rcc.h"
#include "Gpio.h"
#include <stdint.h>
// Assume IR sensor input on PC13 (user button pin on many MCUs)

#define IR_GPIO_PORT GPIO_C
#define IR_PIN 13

static uint8_t lastState = 1;

void IRSensor_Init(void) {
    Rcc_Enable(RCC_GPIOC);

    // Configure pin as input floating or input pull-up depending on IR sensor wiring
    Gpio_Init(IR_GPIO_PORT, IR_PIN, GPIO_INPUT, GPIO_PULL_UP);

    lastState = 1; // Assume idle high initially
}

uint8_t IRSensor_CheckFallingEdge(void) {
    uint8_t currentState = Gpio_ReadPin(IR_GPIO_PORT, IR_PIN);

    if (lastState == 1 && currentState == 0) {
        lastState = currentState;
        return 1; // Falling edge detected
    }

    lastState = currentState;
    return 0;
}
