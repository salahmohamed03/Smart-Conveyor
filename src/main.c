#include "Rcc.h"
#include "Gpio.h"
#include "Std_Types.h"
#include "lcd.h"
#include "emergency.h"

// Emergency button handler function
void emergency_button_handler(void) {
    lcd_clear();
    lcd_print("Emergency!");
}

int main() {
    // Initialize LCD
    lcd_init();
    lcd_print("Smart Conveyor");
    lcd_set_cursor(1, 0);
    lcd_print("Ready!");

    // Initialize Emergency Button
    emergency_button_init();

    while (1) {
        // Main loop
    }

    return 0;
}