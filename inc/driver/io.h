/** 
 * @file io.h 
 * @brief Header only gpio driver 
 * 
 * 
 * Usage: 
 * - Include this file in your project. 
 * - Define CRAP_IO_IMPLEMENTATION in **one** C file to enable the 
 * configuration function io_setup_pin().
 * - io_setup_pin() can only be included once and called only from the same file 
 * 
 * Example: * #define CRAP_IO_IMPLEMENTATION * #include "io.h" 
 *
 * // Configure PA5 as push-pull output, no pull resistor  
 * io_setup_pin(GPIOA, 5, 
 * MODER_output, 
 * 0, // AF not used for output mode 
 * OSPEEDR_medium_speed, 
 * PUPDR_no_pull, 
 * OTYPR_push_pull); 
 * 
 * // Toggle pin 
 * io_pin_set(GPIOA, 5); 
 * io_pin_reset(GPIOA, 5); 
 */
#pragma once
#ifndef CRAP_IO_H
#define CRAP_IO_H

#include <stdint.h>

#include "common.h"


typedef struct{
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
}GPIO_typedef_t;

#define GPIOA_BASE      (0x40020000UL)
#define GPIOA ((__IO GPIO_typedef_t*)GPIOA_BASE)

#define GPIOB_BASE      (0x40020400UL)
#define GPIOB ((__IO GPIO_typedef_t*)GPIOB_BASE)

#define GPIOC_BASE      (0x40020800UL)
#define GPIOC ((__IO GPIO_typedef_t*)GPIOC_BASE)

#define BSSRx_BR(x)     (1<<(x + 16))
#define BSSRx_BS(x)     (1 << x)

ALWAYS_INLINE void io_pin_set(volatile GPIO_typedef_t *GPIO, uint8_t pin_num){
    GPIO->BSRR = BSSRx_BS(pin_num);
}

ALWAYS_INLINE void io_pin_reset(volatile GPIO_typedef_t *GPIO, uint8_t pin_num){
    GPIO->BSRR = BSSRx_BR(pin_num);
}

#ifdef CRAP_IO_IMPLEMENTATION
#define MODERx_MASK(x)  (0x3<<(x * 2))
#define MODERx_SET(val,x)((val & 0x3) <<(x * 2))
#define OSPEEDRx_SET(val,x) ((val & 0x3)<<(x * 2))
#define OSPEEDRx_MASK(x) (0x3<<(x * 2))
#define PUPDRx_SET(val,x) ((val & 0x3)<<(x * 2))
#define PUPDRx_MASK(x) (0x3<<(x * 2))
#define OTx_MASK(x) (1 << x )//1 bit field 0-push pull 1-open drain
#define ODRx_MASK(x)    (1 << x )
#define OTYPERx_SET(val,x) (val << x)

typedef enum {
    MODER_reset_input = 0b00,
    MODER_output = 0b01,
    MODER_af = 0b10,
    MODER_analog = 0b11
}MODERx_states_t;


typedef enum {
    OSPEEDR_low_speed = 0b00,
    OSPEEDR_medium_speed = 0b01,
    OSPEEDR_fast_speed = 0b10,
    OSPEEDR_high_speed = 0b11
}OSPEEDRx_states_t;

#define OSPEEDRx_SET(val,x) ((val & 0x3)<<(x * 2))
#define OSPEEDRx_MASK(x) (0x3<<(x * 2))

typedef enum {
    PUPDR_no_pull = 0b00,
    PUPDR_pullup = 0b01,
    PUPDR_pulldown = 0b10,
    PUPDR_reserved = 0b11
}PUPDRx_states_t;


typedef enum {
    OTYPR_push_pull = 0,
    OTYPR_open_drain = 1
}OTYPRx_state_t;



extern inline void io_setup_pin(__IO GPIO_typedef_t *GPIO, uint8_t pin_num, MODERx_states_t mode, uint8_t af, OSPEEDRx_states_t speed, PUPDRx_states_t pull, OTYPRx_state_t type){
    GPIO->MODER &=~(MODERx_MASK(pin_num));
    GPIO->MODER |= MODERx_SET(mode, pin_num);
    if (mode == MODER_af) {
        if(pin_num >=8 && pin_num <=15){
            GPIO->AFRH &= ~(0xFU << (((pin_num) - 8) * 4));                 
            GPIO->AFRH |=  (af & 0xF) << (((pin_num) - 8) * 4);       
        }else if (pin_num<=7) {
            GPIO->AFRL &= ~(0xFU << (pin_num) * 4);                 
            GPIO->AFRL |=  (af & 0xF) << (pin_num  * 4);  
        }
    }
    GPIO->OTYPER &= ~(OTx_MASK(pin_num));
    GPIO->OTYPER |= OTYPERx_SET(type, pin_num);
    GPIO->OSPEEDR &= ~(OSPEEDRx_MASK(pin_num));
    GPIO->OSPEEDR |= OSPEEDRx_SET(speed,pin_num);
    GPIO->PUPDR &= ~PUPDRx_MASK(pin_num);
    GPIO->PUPDR |= PUPDRx_SET(pull,pin_num);
}
#endif
#endif