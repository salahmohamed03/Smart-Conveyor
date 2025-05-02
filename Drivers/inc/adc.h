/* adc.h */
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_init(void);
uint16_t adc_read(void);
uint8_t map_adc_to_percent(uint16_t adc_val);

#endif