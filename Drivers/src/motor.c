//
// Created by Lenovo on 6/10/2025.
//


#include "motor.h"
#include "pwm.h"

// Global variables
volatile uint8_t motor_speed_percent = 0;

void Motor_Init(void)
{
    PWM_Init();
    motor_speed_percent = 0;
}

void Motor_SetSpeed(uint8_t speed_percent)
{
    if (speed_percent > 100) speed_percent = 100;
    motor_speed_percent = speed_percent;
    PWM_SetDuty(speed_percent);
}

void Motor_Stop(void)
{
    motor_speed_percent = 0;
    PWM_SetDuty(0); // 0% duty = no power to motor
}

uint8_t Motor_GetSpeed(void)
{
    return motor_speed_percent;
}