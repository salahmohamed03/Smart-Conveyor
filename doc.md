# Driver Header File Overview

This project uses modular, bare-metal STM32 drivers. Each header file defines a clean API for a hardware subsystem.

---

## `adc.h` – Potentiometer via ADC

| Function                                        | Purpose                                                                            |
| ----------------------------------------------- | ---------------------------------------------------------------------------------- |
| `void adc_init(void);`                          | Initializes ADC peripheral (clock, resolution, sampling time, channel selection).  |
| `uint16_t adc_read(void);`                      | Reads and returns a 12-bit ADC value (0–4095) representing potentiometer position. |
| `uint8_t map_adc_to_percent(uint16_t adc_val);` | Converts raw ADC value into a 0–100% duty cycle value.                             |

---

## `pwm.h` – Motor Speed Control

| Function                              | Purpose                                                          |
| ------------------------------------- | ---------------------------------------------------------------- |
| `void pwm_init(void);`                | Sets up the timer for PWM output, initializes motor control pin. |
| `void pwm_set_duty(uint8_t percent);` | Sets PWM duty cycle to control motor speed (0–100%).             |
| `void pwm_stop(void);`                | Forces motor to stop by setting PWM output to 0%.                |

---

## `timer_capture.h` – Conveyor Speed Measurement

| Function                               | Purpose                                                                 |
| -------------------------------------- | ----------------------------------------------------------------------- |
| `void timer_capture_init(void);`       | Configures timer in Input Capture mode to measure time between pulses.  |
| `float timer_capture_get_speed(void);` | Calculates and returns conveyor speed from the captured pulse interval. |

---

## `ir_sensor.h` – Object Detection (Polling)

| Function                              | Purpose                                                        |
| ------------------------------------- | -------------------------------------------------------------- |
| `void ir_sensor_init(void);`          | Initializes GPIO pin connected to IR sensor as digital input.  |
| `void ir_sensor_update(void);`        | Polls the IR sensor and detects falling edges (software-only). |
| `uint16_t ir_sensor_get_count(void);` | Returns the total number of detected objects.                  |

---

## `emergency.h` – Emergency Stop Button (Interrupt-Based)

| Function                            | Purpose                                                                    |
| ----------------------------------- | -------------------------------------------------------------------------- |
| `void emergency_button_init(void);` | Sets up EXTI interrupt for emergency stop button (falling edge detection). |

> The corresponding ISR should be implemented in `main.c` or `stm32f1xx_it.c`.

---

## `lcd.h` – 16x2 Character LCD

| Function                                         | Purpose                                                        |
| ------------------------------------------------ | -------------------------------------------------------------- |
| `void lcd_init(void);`                           | Initializes LCD in 4-bit or 8-bit mode via GPIO.               |
| `void lcd_clear(void);`                          | Clears the display and resets cursor position.                 |
| `void lcd_set_cursor(uint8_t row, uint8_t col);` | Sets the LCD cursor to a specific row and column.              |
| `void lcd_print(const char *str);`               | Prints a plain string to the LCD.                              |
| `void lcd_printf(const char *format, ...);`      | Prints formatted output (like `printf`). Useful for variables. |

---
