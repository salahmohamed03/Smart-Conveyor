#include "register_map.h"
#include "Std_Types.h"

#define PWM_PORT 'A'
#define PWM_PIN 5

#define PWM_TIMER       TIM2
#define PWM_CHANNEL     1

// Helper: Enable peripheral clocks manually
void Rcc_Enable_GPIOA(void) {
    RCC->AHB1ENR |= (1 << 0);  // GPIOAEN = bit 0
}

void Rcc_Enable_TIM2(void) {
    RCC->APB1ENR |= (1 << 0);  // TIM2EN = bit 0
}

// Helper: Configure GPIOA Pin 5 to AF1 (TIM2_CH1)
void GpioA_Pin5_AF1(void) {
    // 1) Set MODER bits for PA5 to AF (Alternate Function = 10)
    GPIOA->MODER &= ~(3 << (PWM_PIN * 2));   // Clear bits first
    GPIOA->MODER |= (2 << (PWM_PIN * 2));    // Set to '10' = AF mode

    // 2) Set OTYPER to Push-Pull (0)
    GPIOA->OTYPER &= ~(1 << PWM_PIN);

    // 3) Set OSPEEDR to Medium speed (01) or High speed (11), let's pick Medium:
    GPIOA->OSPEEDR &= ~(3 << (PWM_PIN * 2));
    GPIOA->OSPEEDR |= (1 << (PWM_PIN * 2));

    // 4) No pull-up/pull-down (00)
    GPIOA->PUPDR &= ~(3 << (PWM_PIN * 2));

    // 5) Set Alternate Function register for pin 5: AF1 = TIM2_CH1
    // PA5 is pin 5 -> AFR[0] low register (pins 0..7)
    GPIOA->AFR[0] &= ~(0xF << (PWM_PIN * 4));   // Clear
    GPIOA->AFR[0] |= (1 << (PWM_PIN * 4));      // AF1
}

void PWM_Init(void) {
    // Enable clocks
    Rcc_Enable_GPIOA();
    Rcc_Enable_TIM2();

    // Configure GPIOA pin 5 to AF1
    GpioA_Pin5_AF1();

    // Timer configuration
    PWM_TIMER->PSC = 84 - 1;        // Prescaler to get 1 MHz timer clock (assuming 84 MHz clock)
    PWM_TIMER->ARR = 1000 - 1;      // Auto reload: for 1 kHz PWM frequency

    // Configure PWM mode 1 on CH1
    PWM_TIMER->CCMR1 &= ~(0xFF);           // Clear CCMR1 low byte first
    PWM_TIMER->CCMR1 |= (6 << 4);           // OC1M bits (6 = PWM mode 1)
    PWM_TIMER->CCMR1 |= (1 << 3);           // OC1PE bit - enable preload

    PWM_TIMER->CCER |= 1;                   // Enable output on CH1 (CC1E)

    PWM_TIMER->CR1 |= (1 << 7);             // ARPE bit - enable auto reload preload

    PWM_TIMER->EGR |= 1;                    // UG bit - generate update event to load registers

    PWM_TIMER->CR1 |= 1;                    // CEN bit - enable counter
}

void PWM_SetDuty(uint8 percent) {
    if (percent > 100) percent = 100;

    PWM_TIMER->CCR1 = (PWM_TIMER->ARR * percent) / 100;
}
