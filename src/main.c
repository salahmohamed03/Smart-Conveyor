#include "lcd.h"
#include "timer_capture.h"
#include "ir_sensor.h"
#include "Rcc.h"
#include "Gpio.h"
#include <stdio.h>

volatile uint32_t pulseWidth = 0;
volatile uint32_t objectCount = 0;

int main(void) {
    Rcc_Init();

    lcd_init();
    TimerCapture_Init();
    IRSensor_Init();

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Pulse: ");
    lcd_set_cursor(1, 0);
    lcd_print("Objects: 0");

    while (1) {
        // Measure pulse width on timer input capture
        pulseWidth = TimerCapture_GetPulseWidth();

        // Convert pulse width to time in microseconds (timer freq = 1 MHz)
        // So pulseWidth ticks = pulseWidth us
        lcd_set_cursor(0, 7);
        char buf[10];
        snprintf(buf, sizeof(buf), "%4lu", pulseWidth);
        lcd_print(buf);

        // Check IR sensor falling edge (object detection)
        if (IRSensor_CheckFallingEdge()) {
            objectCount++;
            lcd_set_cursor(1, 9);
            snprintf(buf, sizeof(buf), "%4lu", objectCount);
            lcd_print(buf);
        }
    }

    return 0;
}
