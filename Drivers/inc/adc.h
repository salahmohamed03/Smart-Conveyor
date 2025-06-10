//
// Created by Lenovo on 6/8/2025.
//


#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
uint8_t ADC_ToPercent(uint16_t value);

#endif // ADC_H
