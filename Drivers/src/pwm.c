#include <Gpio.h>
#include "pwm.h"
#include "Std_Types.h"
#include "Rcc.h"

#define PWM_PORT 'A'
#define PWM_PIN 5

#define PWM_TIMER       TIM2
#define PWM_CHANNEL     1

void PWM_Init(void) {
    Rcc_Enable(RCC_TIM2); // TIM

    Gpio_Init(GPIO_A, 5, GPIO_AF, GPIO_PULL_UP);

    // Timer configuration
    PWM_TIMER->PSC = 84 - 1;        // Prescaler to get 1 MHz timer clock (assuming 84 MHz clock)
    PWM_TIMER->ARR = 1000 - 1;      // Auto reload: for 1 kHz PWM frequency

    PWM_TIMER->CCR1 = 0;   //  Set CH1 (PA5) duty to 0%

    // Configure PWM mode 1 on CH1
    PWM_TIMER->CCMR1 &= ~(0xFF);           // Clear CCMR1 low byte first
    PWM_TIMER->CCMR1 |= (6 << 4);           // OC1M bits (6 = PWM mode 1)
    PWM_TIMER->CCMR1 |= (1 << 3);           // OC1PE bit - enable preload

    PWM_TIMER->CCER |= 1;                   // Enable output on CH1 (CC1E)

    PWM_TIMER->CR1 |= (1 << 7);             // ARPE bit - enable auto reload preload

    PWM_TIMER->EGR |= 1;                    // UG bit - generate update event to load registers

    PWM_TIMER->CR1 |= 1;                    // CEN bit - enable counter

    // // Timer configuration
    // PWM_TIMER->PSC = 84 - 1;        // Prescaler to get 1 MHz timer clock
    // PWM_TIMER->ARR = 1000 - 1;      // Auto reload: for 1 kHz PWM frequency
    //
    // // Configure PWM mode 1 on CH1
    // PWM_TIMER->CCMR1 &= ~(0xFF);           // Clear CCMR1 low byte
    // PWM_TIMER->CCMR1 |= (6 << 4);          // OC1M bits (6 = PWM mode 1)
    // PWM_TIMER->CCMR1 |= (1 << 3);          // OC1PE bit - enable preload
    //
    // // Enable output on CH1 and set polarity (active high)
    // PWM_TIMER->CCER |= (1 << 0);           // CC1E - Enable CH1 output
    // PWM_TIMER->CCER &= ~(1 << 1);          // CC1P - Active high polarity
    //
    // // Set initial duty cycle (50% for testing)
    // PWM_TIMER->CCR1 = 500;                 // 50% duty cycle
    //
    // // Enable auto-reload preload
    // PWM_TIMER->CR1 |= (1 << 7);            // ARPE bit
    //
    // // Generate update event to load all registers
    // PWM_TIMER->EGR |= (1 << 0);            // UG bit
    //
    // // Finally, enable the counter
    // PWM_TIMER->CR1 |= (1 << 0);
}

void PWM_SetDuty(uint8 percent) {
    if (percent > 100) percent = 100;

    PWM_TIMER->CCR1 = (PWM_TIMER->ARR * percent) / 100;
}