#include "timer_capture.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"

/* Timer 2 registers */
#define TIM2_BASE       0x40000000
#define TIM2_CR1        (*(volatile uint32 *)(TIM2_BASE + 0x00))
#define TIM2_CCMR1      (*(volatile uint32 *)(TIM2_BASE + 0x18))
#define TIM2_CCER       (*(volatile uint32 *)(TIM2_BASE + 0x20))
#define TIM2_CNT        (*(volatile uint32 *)(TIM2_BASE + 0x24))
#define TIM2_PSC        (*(volatile uint32 *)(TIM2_BASE + 0x28))
#define TIM2_ARR        (*(volatile uint32 *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1       (*(volatile uint32 *)(TIM2_BASE + 0x34))
#define TIM2_SR         (*(volatile uint32 *)(TIM2_BASE + 0x10))

/* Input capture configuration */
#define IC1_SELECT_TI1  0x01    // CC1S = 01 in CCMR1, input channel and connect it to TI1
#define IC1_RISING_EDGE 0x01    // CC1P = 0 in CCER, capture on rising edge

#define TIM_CH1 1

void TimerCapture_Init(void)
{
    /* 2. Configure GPIO for timer input (PA1 - TIM2_CH1) */
    Gpio_Init(GPIO_A, 1, GPIO_AF, GPIO_NO_PULL_DOWN);

    // Configure alternate function for PA0 (AF1 for TIM2_CH1)
    #define GPIOA_BASE     0x40020000
    #define GPIOA_AFRL     (*(volatile uint32 *)(GPIOA_BASE + 0x20))
    GPIOA_AFRL &= ~(0xF << (0 * 4));  // Clear bits for PA0
    GPIOA_AFRL |=  (0x1 << (0 * 4));  // Set AF1 (TIM2_CH1) for PA0

    /* 3. Configure timer base */
    TIM2_CR1 = 0x00;       // Disable timer
    TIM2_PSC = 79;         // 80MHz/80 = 1MHz (1µs resolution)
    TIM2_ARR = 0xFFFFFFFF; // Maximum count value (32 bit)

    /* 4. Configure input capture on channel 1 */
    TIM2_CCMR1 &= ~(0x03 << 0);   // Clear CC1S bits
    TIM2_CCMR1 |= (1 << 0);       // CC1 as input, IC1 mapped to TI1 (01 in CC1S)
    TIM2_CCER &= ~(1 << 1);       // Capture on rising edge (CC1P = 0)
    TIM2_CCER |= (1 << 0);        // Enable capture (CC1E = 1)

    /* 5. Enable timer */
    TIM2_CR1 |= (1 << 0);  // Counter enable
}

uint32 Timer_Capture_Measure_Pulse_Period(void)
{
    static uint32 first_edge = 0;
    uint32 second_edge = 0;
    uint32 timeout = 1000000; // 1 second timeout

    // Wait for first rising edge
    while (!(TIM2_SR & (1 << 1)) && timeout--);
    if (timeout == 0) return 0;
    first_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);  // Clear CC1IF

    // Wait for second rising edge
    timeout = 1000000;
    while (!(TIM2_SR & (1 << 1)) && timeout--);
    if (timeout == 0) return 0;
    second_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);  // Clear CC1IF

    // Handle counter overflow
    if (second_edge >= first_edge) {
        return second_edge - first_edge;
    } else {
        return (0xFFFFFFFF - first_edge) + second_edge + 1;
    }
}
