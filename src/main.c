#include <motor.h>
#include <Rcc.h>

#include "lcd.h"
#include "adc.h"
#include "pwm.h"


void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

int main(void) {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);

    // Initialize LCD, ADC, and PWM
    lcd_init();
    ADC_Init();
    Motor_Init();

    lcd_set_cursor(0, 0);
    lcd_print("Motor Speed:");

    while (1) {
        // Read potentiometer value from channel 0 (PA0)
        uint16_t adc_value = ADC_Read(0);
        uint8_t speed_percent = ADC_ToPercent(adc_value);

        // Update PWM duty cycle
        PWM_SetDuty(speed_percent);

        // Display speed percentage
        lcd_set_cursor(1, 0);
        lcd_printf("Speed: %3d%%   ", speed_percent); // Clear old chars with spaces

        delay_ms(200); // Update every 200ms
    }
}
