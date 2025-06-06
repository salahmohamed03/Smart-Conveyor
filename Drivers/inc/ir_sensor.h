#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>

void IRSensor_Init(void);
uint8_t IRSensor_CheckFallingEdge(void);  // Returns 1 if falling edge detected since last call

#endif
