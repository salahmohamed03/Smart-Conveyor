#include "Timer_Capture.h"
#include "Gpio.h"
#include "Rcc.h"

#define TIM2_BASE_ADDRESS    0x40000000
#define TIM2_CR1             *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x00))
#define TIM2_CCMR1           *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x18))
#define TIM2_CCER            *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x20))
#define TIM2_CNT             *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x24))
#define TIM2_PSC             *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x28))
#define TIM2_ARR             *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x2C))
#define TIM2_CCR1            *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x34))
#define TIM2_SR              *((volatile uint32*)(TIM2_BASE_ADDRESS + 0x10))

// Peripheral IDs based on your RCC implementation
#define TIM2_PERIPHERAL_ID   28  // TIM2 is APB1 bit 0 (APB1 starts at ID 32*2=64? Need clarification)
#define GPIOA_PERIPHERAL_ID  0   // GPIOA is AHB1 bit 0

void Timer_Capture_Init(void)
{
    /* Enable clocks using your RCC functions */
    Rcc_Enable(GPIOA_PERIPHERAL_ID);  // Enable GPIOA clock
    Rcc_Enable(TIM2_PERIPHERAL_ID);   // Enable TIM2 clock

    /* Configure PA0 as alternate function (TIM2_CH1) */
    Gpio_Init(GPIO_A, 0, GPIO_AF, GPIO_PULL_DOWN);

    /* Timer configuration */
    TIM2_PSC = 16 - 1;         // Prescaler for 1MHz clock (assuming 16MHz system clock)
    TIM2_ARR = 0xFFFFFFFF;     // Maximum auto-reload value

    /* Input capture configuration */
    TIM2_CCMR1 |= (0x01 << 0); // CC1 channel is configured as input, IC1 is mapped on TI1
    TIM2_CCMR1 |= (0x00 << 2); // No prescaler
    TIM2_CCMR1 |= (0x00 << 4); // Input capture filter (no filtering)

    TIM2_CCER |= (1 << 0);     // CC1 enable
    TIM2_CCER &= ~(1 << 1);    // Capture on rising edge

    TIM2_CR1 |= (1 << 0);      // Enable TIM2
}

uint32 Timer_Capture_Measure_Pulse_Period(void)
{
    uint32 first_edge = 0;
    uint32 second_edge = 0;

    /* Wait for first rising edge */
    while(!(TIM2_SR & (1 << 1)));  // Wait for CC1IF flag
    first_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);           // Clear CC1IF

    /* Wait for second rising edge */
    while(!(TIM2_SR & (1 << 1)));  // Wait for CC1IF flag
    second_edge = TIM2_CCR1;
    TIM2_SR &= ~(1 << 1);           // Clear CC1IF

    /* Calculate period (in microseconds since we have 1MHz timer clock) */
    if(second_edge > first_edge) {
        return second_edge - first_edge;
    } else {
        /* Handle timer overflow */
        return (0xFFFFFFFF - first_edge) + second_edge;
    }
}