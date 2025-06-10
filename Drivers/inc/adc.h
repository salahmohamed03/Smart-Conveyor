/* adc.h */
#include <stdint.h>

typedef struct {
    volatile uint32_t SR;         // ADC status register
    volatile uint32_t CR1;        // ADC control register 1
    volatile uint32_t CR2;        // ADC control register 2
    volatile uint32_t SMPR1;      // ADC sample time register 1
    volatile uint32_t SMPR2;      // ADC sample time register 2
    volatile uint32_t JOFR1;      // ADC injected channel data offset register 1
    volatile uint32_t JOFR2;      // ADC injected channel data offset register 2
    volatile uint32_t JOFR3;      // ADC injected channel data offset register 3
    volatile uint32_t JOFR4;      // ADC injected channel data offset register 4
    volatile uint32_t HTR;        // ADC watchdog higher threshold register
    volatile uint32_t LTR;        // ADC watchdog lower threshold register
    volatile uint32_t SQR1;       // ADC regular sequence register 1
    volatile uint32_t SQR2;       // ADC regular sequence register 2
    volatile uint32_t SQR3;       // ADC regular sequence register 3
    volatile uint32_t JSQR;       // ADC injected sequence register
    volatile uint32_t JDR1;       // ADC injected data register 1
    volatile uint32_t JDR2;       // ADC injected data register 2
    volatile uint32_t JDR3;       // ADC injected data register 3
    volatile uint32_t JDR4;       // ADC injected data register 4
    volatile uint32_t DR;         // ADC regular data register
} ADC_TypeDef;

#define PERIPH_BASE           0x40000000U
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00000000U)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000U)

#define ADC    ((ADC_TypeDef *) ADC1_BASE)

#ifndef ADC_H

#define ADC_H

// General ADC functions
void     ADC_Init(void);
uint16_t ADC_ReadRaw(uint8_t channel);
uint16_t ADC_ReadFiltered(uint8_t channel);
uint8_t  ADC_ToPercent(uint16_t value);

// Convenience functions for PA1 (avoiding PA0 and PA5)
uint16_t ADC_ReadPA1Raw(void);
uint16_t ADC_ReadPA1Filtered(void);

#endif // ADC_H