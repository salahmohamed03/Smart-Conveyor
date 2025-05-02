/**
 * Gpio.c
 *
 *  Created on: 4/15/2025
 *  Author    : AbdallahDarwish
 */


#include "Std_Types.h"
#include "Gpio.h"
#include "Utils.h"
#include "Bit_Operations.h"
#include "Gpio_Private.h"

void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {
    switch (PortName) {
        case GPIO_A:
            GPIOA_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOA_MODER |= PinMode << (PinNumber * 2);
            if(PinMode == GPIO_OUTPUT){
                GPIOA_OTYPER &= ~(0x01 << (PinNumber));
                GPIOA_OTYPER |= (DefaultState << (PinNumber));
            }else{
                GPIOA_PUPDR &= ~(0x01 << (PinNumber * 2));
                GPIOA_PUPDR |= (DefaultState << (PinNumber * 2));
            }
            break;
        case GPIO_B:
            GPIOB_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOB_MODER |= (PinMode << (PinNumber * 2));
            if (PinMode == GPIO_INPUT) {
                GPIOB_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOB_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOB_OTYPER  &=~(0x1 << PinNumber);
                GPIOB_OTYPER  |= (DefaultState << (PinNumber));
            }
            break;
        case GPIO_C:
            
            GPIOC_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOC_MODER |= (PinMode << (PinNumber * 2));
            if (PinMode == GPIO_INPUT) {
                GPIOC_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOC_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOC_OTYPER  &=~(0x1 << PinNumber);
                GPIOC_OTYPER  |= (DefaultState << (PinNumber));
            }
            break;
        case GPIO_D:
            GPIOD_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOD_MODER |= (PinMode << (PinNumber * 2));
            if (PinMode == GPIO_INPUT) {
                GPIOD_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOD_PUPDR |= (DefaultState << (PinNumber * 2));
            } else {
                GPIOD_OTYPER  &=~(0x1 << PinNumber);
                GPIOD_OTYPER  |= (DefaultState << (PinNumber));
            }
            break;
        default:
            break;
    }

}

uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    unsigned char isInput = 0;
    switch (PortName) {
        case GPIO_A:
            isInput =((GPIOA_MODER & ( 0x3 << (PinNumber * 2)) )== GPIO_INPUT)? 1:0;
            if (isInput) {
                return NOK;
            }
            GPIOA_ODR  &=~(0x1 << PinNumber);
            GPIOA_ODR |= (Data << PinNumber);
            break;
        case GPIO_B:
            isInput =((GPIOB_MODER & ( 0x3 << (PinNumber * 2)) )== GPIO_INPUT)? 1:0;
            if (isInput) {
                return NOK;
            }
            GPIOB_ODR  &=~(0x1 << PinNumber);
            GPIOB_ODR |= (Data << PinNumber);
            break;
        case GPIO_C:
            isInput =((GPIOC_MODER & ( 0x3 << (PinNumber * 2)) )== GPIO_INPUT)? 1:0;
            if (isInput) {
                return NOK;
            }
            GPIOC_ODR  &=~(0x1 << PinNumber);
            GPIOC_ODR |= (Data << PinNumber);
            break;
        case GPIO_D:
            isInput =((GPIOD_MODER & ( 0x3 << (PinNumber * 2)) )== GPIO_INPUT)? 1:0;
            if (isInput) {
                return NOK;
            }
            break;
        default:
            break;
    }
    return OK;
}

uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber)
{
    uint8 data = 0;
    switch (PortName) {
        case GPIO_A:
            data = (GPIOA_IDR & (0x01 << PinNumber)) >> PinNumber;
            break;
        case GPIO_B:
            data = (GPIOB_IDR & (0x01 << PinNumber)) >> PinNumber;
            break;
        case GPIO_C:
            data = (GPIOC_IDR & (0x01 << PinNumber)) >> PinNumber;
            break;
        case GPIO_D:
            data = (GPIOD_IDR & (0x01 << PinNumber)) >> PinNumber;
            break;
        default:
            break;
    }
    return data;
}
