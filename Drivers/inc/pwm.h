/* pwm.h */
#include <stdint.h>

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

#define PERIPH_BASE           0x40000000U
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000U)
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000U)

#define TIM2    ((TIM_TypeDef *) TIM2_BASE)

#ifndef PWM_H
#define PWM_H


void PWM_Init(void);
void PWM_SetDuty(uint8_t percent);

#endif // PWM_H
