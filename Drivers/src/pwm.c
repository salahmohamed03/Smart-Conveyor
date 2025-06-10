#include <Gpio.h>
#include "pwm.h"
#include "Std_Types.h"
#include "Rcc.h"

#define PWM_PORT GPIO_A
#define PWM_PIN 5

#define PWM_TIMER       TIM2
#define PWM_CHANNEL     2  // Using channel 2 for PA5

/* GPIO registers for alternate function configuration */
#define GPIOA_BASE     0x40020000
#define GPIOA_AFRL     (*(volatile uint32 *)(GPIOA_BASE + 0x20))

void PWM_Init(void) {
    // Enable TIM2 clock
    Rcc_Enable(RCC_TIM2);

    // Configure PA5 as alternate function for TIM2_CH2
    Gpio_Init(GPIO_A, 5, GPIO_AF, GPIO_NO_PULL_DOWN);

    // Set alternate function AF1 for PA5 (TIM2_CH2)
    GPIOA_AFRL &= ~(0xF << (5 * 4));  // Clear bits for PA5
    GPIOA_AFRL |=  (0x1 << (5 * 4));  // Set AF1 (TIM2) for PA5

    // Timer base configuration
    PWM_TIMER->PSC = 83;        // Prescaler: 84MHz/84 = 1MHz
    PWM_TIMER->ARR = 999;       // Auto reload: 1MHz/1000 = 1kHz PWM frequency
    PWM_TIMER->CCR2 = 0;        // Set CH2 (PA5) duty to 0% initially

    // Configure PWM mode 1 on CH2 (PA5)
    PWM_TIMER->CCMR1 &= ~(0xFF00);     // Clear CCMR1 high byte (CH2 config)
    PWM_TIMER->CCMR1 |= (6 << 12);     // OC2M bits (6 = PWM mode 1)
    PWM_TIMER->CCMR1 |= (1 << 11);     // OC2PE bit - enable preload

    // Enable output on CH2
    PWM_TIMER->CCER |= (1 << 4);       // CC2E - Enable CH2 output
    PWM_TIMER->CCER &= ~(1 << 5);      // CC2P - Active high polarity

    // Enable auto-reload preload
    PWM_TIMER->CR1 |= (1 << 7);        // ARPE bit - enable auto reload preload

    // Generate update event to load registers
    PWM_TIMER->EGR |= (1 << 0);        // UG bit - generate update event

    // Enable the counter
    PWM_TIMER->CR1 |= (1 << 0);        // CEN bit - enable counter
}

void PWM_SetDuty(uint8 percent) {
    if (percent > 100) percent = 100;

    // Calculate duty cycle: CCR2 = (ARR + 1) * percent / 100
    uint32 duty_value = ((PWM_TIMER->ARR + 1) * percent) / 100;
    PWM_TIMER->CCR2 = duty_value;
}