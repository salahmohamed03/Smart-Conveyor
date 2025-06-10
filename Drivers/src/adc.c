#include "register_map.h"
#include "Gpio.h"
#include "Std_Types.h"
#include <stdint.h>
#include <stdbool.h>

static bool adc_ready = false;

void ADC_Init(void) {
    if (adc_ready) return;

    // Enable clocks
    RCC->APB2ENR |= (1 << 8);    // ADC1EN bit 8
    RCC->AHB1ENR |= (1 << 0);    // GPIOAEN bit 0

    // Configure PA1 as analog mode using GPIO module
    Gpio_Init(GPIO_A, 1, GPIO_ANALOG, GPIO_NO_PULL_DOWN);
    // Configure PA0 as analog mode (MODER = 11)
    // GPIOA->MODER &= ~(3 << (0 * 2));
    // GPIOA->MODER |= (3 << (0 * 2));
    //
    // // No pull-up / pull-down
    // GPIOA->PUPDR &= ~(3 << (0 * 2));

    // Reset ADC registers
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    // One conversion
    ADC1->SQR1 = 0;
    ADC1->SQR3 = 1;  // Channel 1

    // Sample time maximum for channel 1 (bits 5:3 in SMPR2)
    ADC1->SMPR2 &= ~(7 << (1 * 3));
    ADC1->SMPR2 |= (7 << (1 * 3));

    // Enable ADC
    ADC1->CR2 |= (1 << 0); // ADON bit

    // Small delay for stabilization
    for (volatile int i = 0; i < 10000; i++);

    adc_ready = true;
}

uint16_t ADC_Read(uint8_t channel) {
    if (!adc_ready || channel > 18) return 0;

    // Select channel
    ADC1->SQR3 = channel;

    // Clear EOC flag
    ADC1->SR &= ~(1 << 1);

    // Start conversion (SWSTART bit)
    ADC1->CR2 |= (1 << 30);

    // Wait for conversion to complete
    while (!(ADC1->SR & (1 << 1)));

    // Read 12-bit data
    return (uint16_t)(ADC1->DR & 0x0FFF);
}

uint8_t ADC_ToPercent(uint16_t value) {
    if (value > 4095) value = 4095;
    return (uint8_t)((value * 100UL) / 4095);
}