#include "Rcc.h"
#include "Gpio.h"
#include "lcd.h"
#include "ir_sensor.h"
#include "timer_capture.h"
#include "Std_Types.h"

//
// int main(void) {
//     Rcc_Init();
//
//     lcd_init();
//     Timer_Capture_Init();
//     EdgeDetect_Init();
//
//     lcd_clear();
//     lcd_set_cursor(0, 0);
//     lcd_print("Pulse: ");
//     lcd_set_cursor(1, 0);
//     lcd_print("Objects: 0");
//
//     while (1) {
//         // Measure pulse width on timer input capture
//         pulseWidth = Timer_Capture_Measure_Pulse_Period();
//
//         // Convert pulse width to time in microseconds (timer freq = 1 MHz)
//         // So pulseWidth ticks = pulseWidth us
//         lcd_set_cursor(0, 7);
//         char buf[10];
//         snprintf(buf, sizeof(buf), "%4lu", pulseWidth);
//         lcd_print(buf);
//
//         // Check IR sensor falling edge (object detection)
//         if (IRSensor_CheckFallingEdge()) {
//             objectCount++;
//             lcd_set_cursor(1, 9);
//             snprintf(buf, sizeof(buf), "%4lu", objectCount);
//             lcd_print(buf);
//         }
//     }
//
//     return 0;
// }



// static void delay_ms(uint32_t ms) {
//     for (volatile uint32_t i = 0; i < ms * 1000; i++);
// }
//
// int main(void) {
//     Rcc_Init();
//     lcd_init();
//     TimerCapture_Init();
//
//     uint32 pulse_us = 0;
//
//     lcd_clear();
//     lcd_print("Pulse Width:");
//
//     while (1) {
//         pulse_us = Timer_Capture_Measure_Pulse_Period();
//
//
//         lcd_set_cursor(1, 0);
//         if (pulse_us == 0)
//             lcd_print("No signal    ");
//         else
//             lcd_printf("%lu us       ", pulse_us);
//
//         delay_ms(500);
//     }
//
//     return 0;
// }


#include "Gpio.h"
#include "ir_sensor.h"
#include "Rcc.h"
#include "lcd.h"  // Assuming you have an LCD library

#include "Gpio.h"
#include "ir_sensor.h"
#include "Rcc.h"
#include "lcd.h"
#include "timer_capture.h"
#include <stdio.h>

static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

int main(void) {
    // Initialize peripherals
    Rcc_Init();
    lcd_init();
    EdgeDetect_Init();
    TimerCapture_Init();

    uint32_t edge_count = 0;
    uint32_t pulse_us = 0;
    char buf[17];  // LCD line buffer (assuming 16x2 LCD)

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("Pulse:        "); // line 0
    lcd_set_cursor(1, 0);
    lcd_print("Edges:        "); // line 1

    while (1) {
        // Measure pulse width from timer capture
        pulse_us = Timer_Capture_Measure_Pulse_Period();

        // Update pulse width on first row
        lcd_set_cursor(0, 7);
        snprintf(buf, sizeof(buf), "%lu us   ", pulse_us);
        lcd_print(buf);

        // Check for IR falling edge (object detection)
        if (EdgeDetect_CheckFallingEdge()) {
            edge_count++;
            lcd_set_cursor(1, 7);
            snprintf(buf, sizeof(buf), "%lu     ", edge_count);
            lcd_print(buf);
        }

        delay_ms(100);  // Reasonable delay to reduce LCD flicker and bounce
    }

    return 0;
}






