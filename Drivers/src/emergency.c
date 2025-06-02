#include "emergency.h"
#include "rcc.h"

typedef struct {
    uint32 MEMRMP;
    uint32 PMC;
    uint32 EXTICR[4];
    uint32 CMPCR;
} _SYSCFG_TypeDef;
#define _SYSCFG ((_SYSCFG_TypeDef *)0x40013800)

typedef struct {
    uint32 IMR;
    uint32 EMR;
    uint32 RTSR;
    uint32 FTSR;
    uint32 SWIER;
    uint32 PR; 
} _EXTI_TypeDef;
#define _EXTI ((_EXTI_TypeDef *)0x40013C00)

typedef struct {
    uint32 ISER[8];
    uint32 ICER[8];
} _NVIC_TypeDef;
#define _NVIC ((_NVIC_TypeDef *)0xE000E100)

void emergency_button_init(void) {
    Rcc_Enable(EMERGENCY_RCC);
    Gpio_Init(EMERGENCY_BUTTON_PORT, EMERGENCY_BUTTON_PIN, GPIO_INPUT, GPIO_PULL_UP);
    Exti_Init(EMERGENCY_BUTTON_PIN, EMERGENCY_BUTTON_PORT, EXTI_TRIGGER_FALLING);
    Exti_Enable(EMERGENCY_BUTTON_PIN);
}
void EXTI4_IRQHandler(void) {
    if (_EXTI->PR & (1 << EMERGENCY_BUTTON_PIN)) { // Check if the interrupt was triggered by the emergency button
        _EXTI->PR |= (1 << EMERGENCY_BUTTON_PIN); // Clear the pending interrupt
        emergency_button_handler(); // Call the handler function
    }
}


void Exti_Init(uint32 pin, char Port, Exti_TriggerType triggerType) {
    Rcc_Enable(RCC_SYSCFG);
    
    // Map EXTI line to the corresponding GPIO port
    uint32 EXTICR_NUM = pin / 4;   
    uint32 PORT = Port - 'A'; 
    _SYSCFG->EXTICR[EXTICR_NUM] &= ~(0xF << (4 * (pin % 4))); 
    _SYSCFG->EXTICR[EXTICR_NUM] |= (PORT << (4 * (pin % 4)));

    // Enable the EXTI line
    _EXTI->IMR |= (1 << pin); 
    // _EXTI->EMR &= ~(1 << pin); 

    //trigger type
    if (triggerType == EXTI_TRIGGER_RISING || triggerType == EXTI_TRIGGER_BOTH) {
        _EXTI->RTSR |= (1 << pin);
    }
    if (triggerType == EXTI_TRIGGER_FALLING || triggerType == EXTI_TRIGGER_BOTH) {
        _EXTI->FTSR |= (1 << pin);
    }
}

void Exti_Enable(uint8 pin){
    uint32 irqNumber;
    
    if (pin == 0) {
        irqNumber = 6;       // EXTI0
    } else if (pin == 1) {
        irqNumber = 7;       // EXTI1
    } else if (pin == 2) {
        irqNumber = 8;       // EXTI2
    } else if (pin == 3) {
        irqNumber = 9;       // EXTI3
    } else if (pin == 4) {
        irqNumber = 10;      // EXTI4
    } else if (pin >= 5 && pin <= 9) {
        irqNumber = 23;      // EXTI9_5
    } else if (pin >= 10 && pin <= 15) {
        irqNumber = 40;      // EXTI15_10
    }
    
    uint32 NVIC_ISER_NUM = irqNumber / 32;
    uint32 NVIC_ISER_BIT = irqNumber % 32;
    
    _NVIC->ISER[NVIC_ISER_NUM] |= (1 << NVIC_ISER_BIT);
}

void Exti_Disable(uint8 pin){
    uint32 irqNumber;
    
    if (pin == 0) {
        irqNumber = 6;       // EXTI0
    } else if (pin == 1) {
        irqNumber = 7;       // EXTI1
    } else if (pin == 2) {
        irqNumber = 8;       // EXTI2
    } else if (pin == 3) {
        irqNumber = 9;       // EXTI3
    } else if (pin == 4) {
        irqNumber = 10;      // EXTI4
    } else if (pin >= 5 && pin <= 9) {
        irqNumber = 23;      // EXTI9_5
    } else if (pin >= 10 && pin <= 15) {
        irqNumber = 40;      // EXTI15_10
    }
    
    uint32 NVIC_ICER_NUM = irqNumber / 32;
    uint32 NVIC_ICER_BIT = irqNumber % 32;
    
    _NVIC->ICER[NVIC_ICER_NUM] |= (1 << NVIC_ICER_BIT);
}