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
uint32_t pulse_us = 0;
uint16_t adc_value = 0;
uint8_t motor_speed = 0;

// Delay function (rough, blocking)
static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

// Emergency ISR
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
    EdgeDetect_Init();
    TimerCapture_Init();
    emergency_button_init();
    // Motor_Init();
    // ADC_Init();

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
        if (emergency_triggered) {
            // Emergency Stop
            Motor_Stop(); // Stop motor
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("EMERGENCY STOP");
            while (1); // Freeze system
        }

        // === 1. Read Pulse Width (speed) ===
        pulse_us = Timer_Capture_Measure_Pulse_Period();
        lcd_set_cursor(0, 0);
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Spd:%5luus", pulse_us);
        lcd_print(lcd_buffer);

        // === 2. IR Edge Detection (object count) ===
        if (EdgeDetect_CheckFallingEdge()) {
            edge_count++;
        }
        lcd_set_cursor(1, 0);
        snprintf(lcd_buffer, sizeof(lcd_buffer), "Obj:%2lu", edge_count);
        lcd_print(lcd_buffer);

        // // === 3. Read ADC for motor speed ===
        // adc_value = ADC_ReadFiltered(0);                      // Read potentiometer
        // motor_speed = ADC_ToPercent(adc_value);       // Convert to %
        // Motor_SetSpeed(motor_speed);                     // Update PWM
        //
        // // === 4. Display motor speed ===
        // lcd_set_cursor(1, 9);
        // snprintf(lcd_buffer, sizeof(lcd_buffer), "M:%3d%%", motor_speed);
        // lcd_print(lcd_buffer);

        delay_ms(100);
    }

    return 0;
}
