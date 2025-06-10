//
// Created by Lenovo on 6/9/2025.
//

#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

#include <stdint.h>

// Base addresses
#define PERIPH_BASE           0x40000000U
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000U)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000U)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000U)

// Peripheral base addresses
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000U)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800U)
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000U)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000U)

// --- GPIO Register Definition ---
typedef struct {
    volatile uint32_t MODER;      // GPIO port mode register
    volatile uint32_t OTYPER;     // GPIO port output type register
    volatile uint32_t OSPEEDR;    // GPIO port output speed register
    volatile uint32_t PUPDR;      // GPIO port pull-up/pull-down register
    volatile uint32_t IDR;        // GPIO port input data register
    volatile uint32_t ODR;        // GPIO port output data register
    volatile uint32_t BSRR;       // GPIO port bit set/reset register
    volatile uint32_t LCKR;       // GPIO port configuration lock register
    volatile uint32_t AFR[2];     // AFR[0] = AFRL, AFR[1] = AFRH (alternate function registers)
} GPIO_TypeDef;

// --- RCC Register Definition ---
typedef struct {
    volatile uint32_t CR;         // Clock control register
    volatile uint32_t PLLCFGR;    // PLL configuration register
    volatile uint32_t CFGR;       // Clock configuration register
    volatile uint32_t CIR;        // Clock interrupt register
    volatile uint32_t AHB1RSTR;   // AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR;   // AHB2 peripheral reset register
    uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;   // APB1 peripheral reset register
    volatile uint32_t APB2RSTR;   // APB2 peripheral reset register
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;    // AHB1 peripheral clock enable register
    volatile uint32_t AHB2ENR;    // AHB2 peripheral clock enable register
    uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;    // APB1 peripheral clock enable register
    volatile uint32_t APB2ENR;    // APB2 peripheral clock enable register
} RCC_TypeDef;

// --- TIM2 Register Definition ---
typedef struct {
    volatile uint32_t CR1;        // Control register 1
    volatile uint32_t CR2;        // Control register 2
    volatile uint32_t SMCR;       // Slave mode control register
    volatile uint32_t DIER;       // DMA/interrupt enable register
    volatile uint32_t SR;         // Status register
    volatile uint32_t EGR;        // Event generation register
    volatile uint32_t CCMR1;      // Capture/compare mode register 1
    volatile uint32_t CCMR2;      // Capture/compare mode register 2
    volatile uint32_t CCER;       // Capture/compare enable register
    volatile uint32_t CNT;        // Counter
    volatile uint32_t PSC;        // Prescaler
    volatile uint32_t ARR;        // Auto-reload register
    volatile uint32_t CCR1;       // Capture/compare register 1
    volatile uint32_t CCR2;       // Capture/compare register 2
    volatile uint32_t CCR3;       // Capture/compare register 3
    volatile uint32_t CCR4;       // Capture/compare register 4
    volatile uint32_t DCR;        // DMA control register
    volatile uint32_t DMAR;       // DMA address for full transfer
} TIM_TypeDef;

// --- ADC1 Register Definition ---
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

// --- Peripheral instances ---
#define GPIOA   ((GPIO_TypeDef *) GPIOA_BASE)
#define RCC     ((RCC_TypeDef *) RCC_BASE)
#define TIM2    ((TIM_TypeDef *) TIM2_BASE)
#define ADC1    ((ADC_TypeDef *) ADC1_BASE)

// --- Helper macros for GPIO alternate function modes (optional) ---
#define GPIO_AF         2  // Your GPIO mode for alternate function (usually 2 or defined elsewhere)

#endif // REGISTER_MAP_H
