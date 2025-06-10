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

/* GPIO registers for alternate function configuration */
#define GPIOA_BASE     0x40020000
#define GPIOA_AFRL     (*(volatile uint32 *)(GPIOA_BASE + 0x20))

#define TIM_CH1 1

void TimerCapture_Init(void)
{
    /* 1. Enable TIM2 clock */
    Rcc_Enable(RCC_TIM2);

    /* 2. Configure GPIO for timer input (PA0 - TIM2_CH1) */
    // PA0 configured as alternate function with no pull-up/down
    Gpio_Init(GPIO_A, 0, GPIO_AF, GPIO_NO_PULL_DOWN);

    /* 3. Configure alternate function for PA0 (AF1 for TIM2_CH1) */
    GPIOA_AFRL &= ~(0xF << (0 * 4));  // Clear bits for PA0
    GPIOA_AFRL |=  (0x1 << (0 * 4));  // Set AF1 (TIM2_CH1) for PA0

    /* 4. Configure timer base */
    TIM2_CR1 = 0x00;       // Disable timer, reset configuration
    TIM2_PSC = 83;         // 84MHz/84 = 1MHz (1µs resolution)
    TIM2_ARR = 0xFFFFFFFF; // Maximum count value (32-bit timer)

    /* 5. Configure input capture on channel 1 */
    TIM2_CCMR1 &= ~(0xFF);        // Clear all CCMR1 bits first
    TIM2_CCMR1 |= (1 << 0);       // CC1S = 01: IC1 mapped to TI1
    TIM2_CCMR1 |= (0 << 2);       // IC1PSC = 00: no prescaler
    TIM2_CCMR1 |= (0 << 4);       // IC1F = 0000: no filter

    /* 6. Configure capture/compare enable register */
    TIM2_CCER &= ~(1 << 1);       // CC1P = 0: capture on rising edge
    TIM2_CCER |= (1 << 0);        // CC1E = 1: enable capture

    /* 7. Clear any pending flags */
    TIM2_SR = 0;

    /* 8. Enable timer counter */
    TIM2_CR1 |= (1 << 0);  // CEN = 1: counter enable
}

uint32 Timer_Capture_Measure_Pulse_Period(void)
{
    static uint32 first_edge = 0;
    static uint8 first_edge_captured = 0;
    uint32 second_edge = 0;
    uint32 timeout = 500000; // Timeout counter (reduced for better responsiveness)

    // If we haven't captured the first edge yet, wait for it
    if (!first_edge_captured) {
        // Wait for first rising edge
        TIM2_SR &= ~(1 << 1);  // Clear CC1IF flag
        timeout = 500000;
        while (!(TIM2_SR & (1 << 1)) && timeout--) {
            // Non-blocking timeout to prevent system freeze
        }

        if (timeout == 0) {
            return 0; // No signal detected
        }

        first_edge = TIM2_CCR1;
        TIM2_SR &= ~(1 << 1);  // Clear CC1IF flag
        first_edge_captured = 1;
        return 0; // Return 0 until we have a period measurement
    }

    // Wait for second rising edge
    timeout = 500000;
    while (!(TIM2_SR & (1 << 1)) && timeout--) {
        // Non-blocking timeout
    }

    if (timeout == 0) {
        // Reset state if timeout occurs
        first_edge_captured = 0;
        return 0;
    }

    second_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);  // Clear CC1IF flag

    // Calculate period and reset for next measurement
    uint32 period;
    if (second_edge >= first_edge) {
        period = second_edge - first_edge;
    } else {
        // Handle counter overflow (32-bit timer)
        period = (0xFFFFFFFF - first_edge) + second_edge + 1;
    }

    // Reset for next measurement
    first_edge_captured = 0;

    return period; // Period in microseconds
}