#include <stdint.h>
#include <stdbool.h>
#include "Std_Types.h"
#include "Rcc.h"
#include "gpio.h"
#include "adc.h"

#include <stdio.h>

// Define ADC1 as default ADC peripheral (CMSIS compatibility)
#define ADC1    ADC

// Alpha value for simple low-pass filter (higher = smoother but slower)
#define FILTER_ALPHA 8

// 12-bit ADC max value = 2^12 - 1 = 4095
#define ADC_MAX_VALUE 4095

// ADC Channel for PA1 (avoiding PA0 and PA5)
#define ADC_CHANNEL_PA1 1

// Internal state flags and variables
static bool adc_ready = false;        // Prevents re-initialization
static uint16_t adc_filtered = 0;     // Holds filtered ADC value between readings


void ADC_Init(void) {
    // Enable clock for GPIOA and ADC1
    // Note: GPIOA should be enabled in main before calling this
    Rcc_Enable(RCC_ADC1);

    // Configure PA1 (ADC Channel 1) as analog input with no pull-up/down
    // This avoids PA0 and PA5 which may be used for other purposes
    Gpio_Init(GPIO_A, 1, GPIO_ANALOG, GPIO_NO_PULL_DOWN);

    // Reset ADC control registers to default
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    // Set ADC to perform one conversion (not sequence)
    ADC1->SQR1 = 0;    // One conversion in regular sequence (L[3:0] = 0000)
    ADC1->SQR3 = ADC_CHANNEL_PA1;    // First (and only) channel is channel 1 (PA1)

    // Set sample time for channel 1 for stable results (84 cycles)
    ADC1->SMPR2 &= ~(7 << (ADC_CHANNEL_PA1 * 3));         // Clear channel 1 sample time bits
    ADC1->SMPR2 |=  (4 << (ADC_CHANNEL_PA1 * 3));         // Set sample time to 84 cycles

    // Enable the ADC by setting the ADON bit
    ADC1->CR2 |= (1 << 0);

    // Wait for ADC to stabilize after enabling
    for (volatile int i = 0; i < 10000; i++);

    // Set ready flag
    adc_ready = true;
}


uint16_t ADC_ReadRaw(uint8_t channel) {
    if (!adc_ready || channel > 18) return 0; // Validate state and input

    // Select ADC channel for conversion
    ADC1->SQR3 = channel;

    // Clear the End Of Conversion (EOC) flag before starting
    ADC1->SR &= ~(1 << 1);

    // Start ADC conversion using software trigger
    ADC1->CR2 |= (1 << 30); // SWSTART

    // Wait until conversion is complete (EOC flag is set)
    while (!(ADC1->SR & (1 << 1)));

    // Return the 12-bit conversion result
    return (uint16_t)(ADC1->DR & 0x0FFF);
}


uint16_t ADC_ReadFiltered(uint8_t channel) {
    // Read a new raw ADC value from the selected channel
    uint16_t new_value = ADC_ReadRaw(channel);

    // Initialize filter value on first call
    if (adc_filtered == 0) {
        adc_filtered = new_value;
    }

    // Apply low-pass filter: smooth out noise by averaging
    adc_filtered = (adc_filtered * (FILTER_ALPHA - 1) + new_value) / FILTER_ALPHA;

    return adc_filtered;
}


// Convenience function to read from PA1 (avoiding PA0 and PA5)
uint16_t ADC_ReadPA1Raw(void) {
    return ADC_ReadRaw(ADC_CHANNEL_PA1);
}


// Convenience function to read filtered value from PA1 (avoiding PA0 and PA5)
uint16_t ADC_ReadPA1Filtered(void) {
    return ADC_ReadFiltered(ADC_CHANNEL_PA1);
}


uint8_t ADC_ToPercent(uint16_t value) {
    // Limit input value to maximum 4095
    if (value > ADC_MAX_VALUE) value = ADC_MAX_VALUE;

    // Scale to 0–100% using 32-bit math to prevent overflow
    uint32_t temp = ((uint32_t)value * 100UL) / ADC_MAX_VALUE;

    // Cap percentage to 100
    if (temp > 100) temp = 100;

    return (uint8_t)temp;
}