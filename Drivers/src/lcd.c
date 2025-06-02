#include "lcd.h"
#include "Gpio.h"
#include "Rcc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Define GPIO pins for LCD
#define LCD_RS_PIN  0  // Register Select pin
#define LCD_RW_PIN  1  // Read/Write pin
#define LCD_EN_PIN  2  // Enable pin
#define LCD_DATA_PORT GPIO_B // Data port (e.g., GPIOA)

// Helper macros
#define LCD_COMMAND_MODE 0
#define LCD_DATA_MODE    1

// Delay function (simple blocking delay)
static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

// Send a command or data to the LCD
static void lcd_send(uint8_t value, uint8_t mode) {
    Gpio_WritePin(LCD_DATA_PORT, LCD_RS_PIN, mode); // Set RS pin
    Gpio_WritePin(LCD_DATA_PORT, LCD_RW_PIN, LOW); // Set RW pin to write
    Gpio_WritePin(LCD_DATA_PORT, LCD_EN_PIN, HIGH); // Enable LCD

    // Send data (assuming 4-bit mode)
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t bit = (value >> (4 + i)) & 0x01;
        Gpio_WritePin(LCD_DATA_PORT, i + 3, bit);
    }
    Gpio_WritePin(LCD_DATA_PORT, LCD_EN_PIN, LOW); // Disable LCD
    delay_ms(2);

    Gpio_WritePin(LCD_DATA_PORT, LCD_EN_PIN, HIGH); // Enable LCD
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t bit = (value >> i) & 0x01;
        Gpio_WritePin(LCD_DATA_PORT, i + 3, bit);
    }
    Gpio_WritePin(LCD_DATA_PORT, LCD_EN_PIN, LOW); // Disable LCD
    delay_ms(2);
}

// Initialize the LCD
void lcd_init(void) {
    // Enable GPIO clock
    Rcc_Enable(RCC_GPIOB); // Assuming GPIOB is used for LCD data pins

    // Configure GPIO pins for LCD
    Gpio_Init(LCD_DATA_PORT, LCD_RS_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_DATA_PORT, LCD_RW_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_DATA_PORT, LCD_EN_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    for (uint8_t i = 0; i < 4; i++) {
        Gpio_Init(LCD_DATA_PORT, i + 3, GPIO_OUTPUT, GPIO_PUSH_PULL);
    }

    // LCD initialization sequence
    delay_ms(50); // Wait for LCD to power up
    lcd_send(0x33, LCD_COMMAND_MODE); // Initialize in 4-bit mode
    lcd_send(0x32, LCD_COMMAND_MODE); // Set to 4-bit mode
    lcd_send(0x28, LCD_COMMAND_MODE); // 2-line, 5x8 font
    lcd_send(0x0C, LCD_COMMAND_MODE); // Display on, cursor off
    lcd_send(0x06, LCD_COMMAND_MODE); // Entry mode set
    lcd_clear(); // Clear display
}

// Clear the LCD
void lcd_clear(void) {
    lcd_send(0x01, LCD_COMMAND_MODE); // Clear display command
    delay_ms(2); // Wait for command to complete
}

// Set cursor position
void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? col : (0x40 + col);
    lcd_send(0x80 | address, LCD_COMMAND_MODE); // Set DDRAM address
}

// Print a string to the LCD
void lcd_print(const char *str) {
    while (*str) {
        lcd_send(*str++, LCD_DATA_MODE); // Send each character
    }
}

// Print formatted text to the LCD
void lcd_printf(const char *format, ...) {
    char buffer[32];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    lcd_print(buffer);
}