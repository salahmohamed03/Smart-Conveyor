#include "Rcc.h"
#include "lcd.h"
#include "emergency.h"
#include "ir_sensor.h"
#include "timer_capture.h"
#include "adc.h"
#include <stdio.h>
#include "motor.h"

// Global buffers and state
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
    // === Initialization ===
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);

    lcd_init();
    EdgeDetect_Init();           // IR sensor for object detection
    TimerCapture_Init();         // Timer capture for speed measurement
    emergency_button_init();     // Emergency stop button
    Motor_Init();                // Motor PWM control
    ADC_Init();                  // ADC for potentiometer

    // === Startup Message ===
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Smart Conveyor");
    lcd_set_cursor(1, 0);
    lcd_print("System Ready");
    delay_ms(500);
    lcd_clear();

    // === Main Loop ===
    while (1) {
        // Check for emergency stop first
        if (emergency_triggered) {
            Motor_Stop(); // Stop motor immediately
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("EMERGENCY STOP");
            lcd_set_cursor(1, 0);
            lcd_print("System Halted");
            while (1); // Freeze system - only reset can recover
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
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Obj:%3lu", edge_count);
        lcd_print(lcd_buffer);

        // === 3. Read ADC for motor speed control ===
        adc_value = ADC_ReadFiltered(0);          // Read potentiometer on channel 0
        motor_speed = ADC_ToPercent(adc_value);   // Convert to percentage
        Motor_SetSpeed(motor_speed);              // Update PWM duty cycle

        // === 4. Display motor speed ===
        lcd_set_cursor(1, 0);
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Motor:%3d%%", motor_speed);
        lcd_print(lcd_buffer);

        // === 5. Display system status ===
        lcd_set_cursor(1, 10);
        lcd_print(" RUN");

        delay_ms(50); // Update rate: 10Hz
    }

    return 0;
}