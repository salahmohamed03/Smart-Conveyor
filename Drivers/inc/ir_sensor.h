#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>

void ir_sensor_init(void);
void ir_sensor_update(void);
uint16_t ir_sensor_get_count(void);

#endif