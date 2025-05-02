/* pwm.h */
#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void pwm_init(void);
void pwm_set_duty(uint8_t percent);
void pwm_stop(void);

#endif