/**
 * @file syscfg.h
 * @brief Syscfg header only driver (exti only for now)
 *
 *
 * Usage:
 *  - Include this file in your project.
 *  - Use `syscfg_set_exti_pin(port, pin)` to map an EXTI line to a GPIO port.
 *
 * Example:
 *  // Map EXTI line 5 to port B
 *  syscfg_set_exti_pin(SYSCFG_PBx, 5);
 *
 * Notes:
 *  - Designed for STM32F411CE
 */

#pragma once
#ifndef CRAP_SYSCFG
#define CRAP_SYSCFG

#include <stdint.h>

#include "common.h"

typedef struct {
    __IO uint32_t MEMRM;
    __IO uint32_t PMC;
    __IO uint32_t EXTICR[4];
    __IO uint32_t CMPCR;
} SYSCFG_typedef_t;

typedef enum{
    SYSCFG_PAx = 0,
    SYSCFG_PBx = 1,
    SYSCFG_PCx = 2,
    SYSCFG_PDx = 3,
    SYSCFG_PEx = 4,
    SYSCFG_PHx = 7, 
}SYSCFG_EXTI_port_t;

#define SYSCFG_BASE (0x40013800UL)

#define SYSCFG ((SYSCFG_typedef_t *)SYSCFG_BASE)

ALWAYS_INLINE void syscfg_set_exti_pin(SYSCFG_EXTI_port_t port, uint8_t pin){
    uint8_t crnum = pin >> 2;
    uint8_t shift = (pin & 0x3) << 2;

    SYSCFG->EXTICR[crnum] &= ~(0xF << shift);

    SYSCFG->EXTICR[crnum] |= port << shift;
}

#endif