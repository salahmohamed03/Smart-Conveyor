#ifndef TIMER_CAPTURE_H
#define TIMER_CAPTURE_H

#include "Std_Types.h"

#define TIM_CH1    1
#define TIM_CH2    2

void TimerCapture_Init(void);
uint32 Timer_Capture_Measure_Pulse_Period(void);

#endif /* TIMER_CAPTURE_H */
