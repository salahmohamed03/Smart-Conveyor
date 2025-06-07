#include "timer_capture.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"

#define TIM2_BASE        0x40000000
#define TIM2_CR1         (*(volatile uint32 *)(TIM2_BASE + 0x00))
#define TIM2_DIER        (*(volatile uint32 *)(TIM2_BASE + 0x0C))
#define TIM2_SR          (*(volatile uint32 *)(TIM2_BASE + 0x10))
#define TIM2_CCMR1       (*(volatile uint32 *)(TIM2_BASE + 0x18))
#define TIM2_CCER        (*(volatile uint32 *)(TIM2_BASE + 0x20))
#define TIM2_CNT         (*(volatile uint32 *)(TIM2_BASE + 0x24))
#define TIM2_PSC         (*(volatile uint32 *)(TIM2_BASE + 0x28))
#define TIM2_ARR         (*(volatile uint32 *)(TIM2_BASE + 0x2C))
#define TIM2_CCR1        (*(volatile uint32 *)(TIM2_BASE + 0x34))

void TimerCapture_Init(void) {
    // Enable clocks for GPIOA and TIM2
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_TIM2);

    // Configure PA0 as Alternate Function Input (TIM2_CH1)
    Gpio_Init(GPIO_A, 0, GPIO_AF, GPIO_PUSH_PULL);  // AF mode for TIM2_CH1

    // Timer configuration: 72 MHz / (71+1) = 1 MHz (1 µs per tick)
    TIM2_PSC = 71;
    TIM2_ARR = 0xFFFFFFFF;  // Max reload value (32-bit counter)

    // Input Capture on Channel 1 (TI1)
    TIM2_CCMR1 &= ~(0xFF);  // Clear CC1S and IC1F
    TIM2_CCMR1 |= (1 << 0); // CC1S = 01 (TI1 input)
    TIM2_CCMR1 |= (0 << 4); // IC1F = 0000 (no filter)

    // Rising edge detection
    TIM2_CCER &= ~(1 << 1); // CC1P = 0 (rising edge)
    TIM2_CCER |= (1 << 0);  // CC1E = 1 (enable capture)

    TIM2_CNT = 0;           // Reset counter
    TIM2_CR1 |= (1 << 0);   // Enable timer (CEN = 1)
}

uint32 TimerCapture_MeasurePulseWidth(void) {
    uint32 first_edge = 0;
    uint32 second_edge = 0;
    uint32 timeout = 1000000;  // 1 second timeout (adjust if needed)

    // Wait for first rising edge
    while (!(TIM2_SR & (1 << 1)) && timeout--);
    if (timeout == 0) return 0;  // Timeout
    first_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);  // Clear CC1IF

    // Wait for second rising edge
    timeout = 1000000;
    while (!(TIM2_SR & (1 << 1)) && timeout--);
    if (timeout == 0) return 0;  // Timeout
    second_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);  // Clear CC1IF

    // Handle counter overflow
    if (second_edge >= first_edge) {
        return second_edge - first_edge;  // Normal case
    } else {
        return (0xFFFFFFFF - first_edge) + second_edge + 1;  // Overflow case
    }
}