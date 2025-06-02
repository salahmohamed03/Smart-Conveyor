#ifndef EMERGENCY_H
#define EMERGENCY_H


#include "Std_Types.h"
#include "Gpio.h"
#include "Rcc.h"

typedef enum {
    EXTI_TRIGGER_RISING,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} Exti_TriggerType;

#define EMERGENCY_BUTTON_PIN 4
#define EMERGENCY_BUTTON_PORT GPIO_A
#define EMERGENCY_RCC RCC_GPIOA



void emergency_button_init(void);
void Exti_Init(uint32 pin, char Port, Exti_TriggerType triggerType);
void Exti_Enable(uint8 pin);
void Exti_Disable(uint8 pin);
void emergency_button_handler(void);


#endif