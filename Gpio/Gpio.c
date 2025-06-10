#include <Std_Types.h>
#include "GPIO.h"
#include "GPIO_PRIVATE.h"
#define GPIO_REG(PORT_ID, REG_ID) ((volatile uint32 *) ((PORT_ID) + (REG_ID)))

uint32 addressMap[4] = {GPIOA_BASE_ADDR, GPIOB_BASE_ADDR, GPIOC_BASE_ADDR, GPIOD_BASE_ADDR};

void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {
    // Select port base using your address map
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];

    // 1. Set MODER
    gpioDevice->GPIO_MODER &= ~(0x03 << (PinNumber * 2));
    gpioDevice->GPIO_MODER |=  (PinMode << (PinNumber * 2));

    if (PinMode == GPIO_INPUT) {
        // 2. Configure pull-up/pull-down
        gpioDevice->GPIO_PUPDR &= ~(0x03 << (PinNumber * 2));
        gpioDevice->GPIO_PUPDR |=  (DefaultState << (PinNumber * 2));

    } else if (PinMode == GPIO_OUTPUT) {
        // 3. Configure output type (Push-Pull or Open-Drain)
        gpioDevice->GPIO_OTYPER &= ~(1 << PinNumber);
        gpioDevice->GPIO_OTYPER |=  (DefaultState << PinNumber);

    } else if (PinMode == GPIO_AF) {
        // 4. Set output type: push-pull
        gpioDevice->GPIO_OTYPER &= ~(1 << PinNumber); // Push-pull

        // 5. Set speed: medium speed
        gpioDevice->GPIO_OSPEEDR &= ~(0x03 << (PinNumber * 2));
        gpioDevice->GPIO_OSPEEDR |=  (0x01 << (PinNumber * 2)); // Medium speed

        // 6. Disable pull-up/pull-down
        gpioDevice->GPIO_PUPDR &= ~(0x03 << (PinNumber * 2));

        // 7. Set alternate function number (AF0–AF15)
        if (PinNumber < 8) {
            gpioDevice->GPIO_AFRL &= ~(0xF << (PinNumber * 4));
            gpioDevice->GPIO_AFRL |=  (DefaultState << (PinNumber * 4));
        } else {
            gpioDevice->GPIO_AFRH &= ~(0xF << ((PinNumber - 8) * 4));
            gpioDevice->GPIO_AFRH |=  (DefaultState << ((PinNumber - 8) * 4));
        }

    } else if (PinMode == GPIO_ANALOG) {
        // 8. Analog mode = MODER already set to 11, disable pull-up/down
        gpioDevice->GPIO_PUPDR &= ~(0x03 << (PinNumber * 2));
    }
}



uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    uint8 status = NOK;
    uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];
    if (((gpioDevice->GPIO_MODER & (0x03 << (PinNumber * 2))) >> (PinNumber * 2)) != GPIO_INPUT) {
        gpioDevice->GPIO_ODR &= ~(0x1 << PinNumber);
        gpioDevice->GPIO_ODR |= (Data << PinNumber);
        status = OK;
    } return status;
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNum) {
    uint8 data = 0; uint8 addressIndex = PortName - GPIO_A;
    GpioType* gpioDevice = (GpioType*) addressMap[addressIndex];
    data = (gpioDevice->GPIO_IDR & (0x1 << PinNum)) >> PinNum;
    return data;
}