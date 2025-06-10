#include <stdio.h>
#include <stdint.h>
#include "Rcc.h"
#include "lcd.h"
#include "emergency.h"
#include "ir_sensor.h"
#include "timer_capture.h"
#include "adc.h"
#include "pwm.h"
#include "motor.h"

// Global buffers and state variables
char lcd_buffer[17];
volatile uint8_t emergency_triggered = 0;
uint32_t edge_count = 0;
uint32_t pulse_period_us = 0;
uint16_t adc_value = 0;
uint8_t motor_speed = 0;

// Delay function (rough, blocking)
static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

// Emergency ISR - called from emergency.c
void emergency_button_handler(void) {
    emergency_triggered = 1;
}

int main(void) {
    // === System Initialization ===
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);

    // Initialize all subsystems
    lcd_init();
    EdgeDetect_Init();           // IR sensor for object detection
    TimerCapture_Init();         // Timer capture for conveyor speed measurement
    emergency_button_init();     // Emergency stop button
    Motor_Init();                // Motor PWM control
    ADC_Init();                  // ADC for potentiometer speed control

    // === Startup Sequence ===
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Smart Conveyor");
    lcd_set_cursor(1, 0);
    lcd_print("System Starting");
    delay_ms(500);

    lcd_clear();

    while (1) {
        // === Emergency Stop Check ===
        if (emergency_triggered) {
            Motor_Stop();
            PWM_SetDuty(0);

            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("*** EMERGENCY ***");
            lcd_set_cursor(1, 0);
            lcd_print("SYSTEM HALTED");

            while (1) {
                delay_ms(500);
                lcd_clear();
                delay_ms(500);
                lcd_set_cursor(0, 0);
                lcd_print("*** EMERGENCY ***");
                lcd_set_cursor(1, 0);
                lcd_print("RESET REQUIRED");
            }
        }

        // === 1. Read Pulse Period (conveyor speed) ===
        pulse_period_us = Timer_Capture_Measure_Pulse_Period();

        // Display conveyor speed (pulse period in microseconds)
        lcd_set_cursor(0, 0);
        // Calculate frequency in Hz: f = 1,000,000 / period_us
        uint32_t frequency_hz = 1000000UL / pulse_period_us;
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Spd:%4luHz", frequency_hz);

        lcd_print(lcd_buffer);

        // === 2. IR Edge Detection (object count) ===
        if (EdgeDetect_CheckFallingEdge()) {
            edge_count++;
        }

        // Display object count
        lcd_set_cursor(1, 0);
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Obj:%2lu", edge_count);
        lcd_print(lcd_buffer);

        // === Motor Speed Control via Potentiometer ===
        adc_value = ADC_Read(0);
        motor_speed = ADC_ToPercent(adc_value);
        Motor_SetSpeed(motor_speed);
        PWM_SetDuty(motor_speed);

        lcd_set_cursor(1, 7);
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Mtr:%3d%%", motor_speed);
        lcd_print(lcd_buffer);

        // === System Update Rate ===
        delay_ms(50); // 20Hz refresh
    }

    return 0; // Should never reach here
}


