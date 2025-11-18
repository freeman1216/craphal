/**
 * @file exti.h
 * @brief Header only driver for exti
 *
 *
 *
 * Usage Example:
 
 *  // Configure EXTI line 5 for both edges
 *  exti_configure_line(5, EXTI_TRIGGER_BOTH);
 *
 *  // Make sure you also configure SYSCFG and NVIC!
 *
 * Notes:
 *  - This module configures only the EXTI peripheral.
 *    You must map the GPIO pin to the EXTI line using SYSCFG_EXTICR.
 *  - You must enable the interrupt in NVIC separately.
 */

#pragma once 
#ifndef CRAP_EXTI
#define CRAP_EXTI

#include "stdint.h"

#include "common.h"

#ifdef CRAP_EXTI_STATIC
    #define CRAP_EXTI_DEF static inline
#else
    #define CRAP_EXTI_DEF extern
#endif

typedef struct{
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
} EXTI_typedef_t;

typedef enum {
    EXTI_TRIGGER_RISING  = 1,
    EXTI_TRIGGER_FALLING = 2,
    EXTI_TRIGGER_BOTH = EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING, // 0x3
} EXTI_trigger_t;

#define EXTI_BASE   (0x40013C00UL)
#define EXTI        ((__IO EXTI_typedef_t *)EXTI_BASE)
CRAP_EXTI_DEF void exti_configure_line(uint8_t line, EXTI_trigger_t trigger);
#ifdef CRAP_EXTI_IMPLEMENTATION

CRAP_EXTI_DEF void exti_configure_line(uint8_t line, EXTI_trigger_t trigger)
{
    
    EXTI->RTSR &= ~(1 << line);
    EXTI->FTSR &= ~(1 << line);

    
    if (trigger & EXTI_TRIGGER_RISING)
        EXTI->RTSR |= (1 << line);
    if (trigger & EXTI_TRIGGER_FALLING)
        EXTI->FTSR |= (1 << line);

    // Unmask the interrupt
    EXTI->PR  |= (1 << line);
    EXTI->IMR |= (1 << line);
}

#endif
#ifdef CRAP_EXTI1_ISR_IMPLEMENTATION

void exti1_usr();

STRONG_ISR(exti1_isr){
    EXTI->PR |= (1 << 1);
    exti1_usr();
}

#endif

#endif