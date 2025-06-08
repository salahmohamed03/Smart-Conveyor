#include "Rcc.h"
#include "Gpio.h"
#include "Std_Types.h"
#include "lcd.h"
#include "emergency.h"
#include "ir_sensor.h"
#include "timer_capture.h"
#include <stdio.h>

// Buffer for LCD display
char lcd_buffer[17]; // For 16-character line

// State variables
volatile uint8_t emergency_triggered = 0;
uint32_t edge_count = 0;
uint32_t pulse_us = 0;

// Simple blocking delay (adjusted per system speed)
static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

// Emergency button ISR handler
void emergency_button_handler(void) {
    emergency_triggered = 1;
}

int main(void) {
    // Initialize system
    Rcc_Init();
    lcd_init();
    EdgeDetect_Init();
    TimerCapture_Init();
    emergency_button_init();

    // Initial welcome message
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Smart Conveyor");
    lcd_set_cursor(1, 0);
    lcd_print("System Ready");
    delay_ms(750);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Pulse:        ");
    lcd_set_cursor(1, 0);
    lcd_print("Count:        ");

    while (1) {
        if (emergency_triggered) {
            // Emergency stop state
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_print("EMERGENCY STOP");
            while (1); // System halt
        }

        // Measure pulse width (speed)
        pulse_us = Timer_Capture_Measure_Pulse_Period();
        lcd_set_cursor(0, 7);
        snprintf(lcd_buffer, sizeof(lcd_buffer), "%lu us ", pulse_us);
        lcd_print(lcd_buffer);

        // IR object detection
        if (EdgeDetect_CheckFallingEdge()) {
            edge_count++;
            lcd_set_cursor(1, 7);
            snprintf(lcd_buffer, sizeof(lcd_buffer), "%lu    ", edge_count);
            lcd_print(lcd_buffer);
        }

        delay_ms(50);
    }

    return 0;
}
