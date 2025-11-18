/**
 * @file flash.h
 * @brief Header only flash configuration
 *
 *
 * Usage:
 *  - include this in only one C file
 *  - Call `flash_acceleration_setup()` early during system initialization,
 *    typically immediately after configuring system clocks.
 * Example:

 *  flash_acceleration_setup(FLASH_LATENCY_3ws,
 *                           FLASH_DCACHE_ENABLE,
 *                           FLASH_ICACHE_ENABLE);
 *
 * Notes:
 *  - Designed for STM32F411CE.
 *  - This writes directly to the FLASH->ACR register, replacing its previous state.
 *    Ensure that all required bits are included in the parameters.
 */


#pragma once
#ifndef CRAP_FLASH_H

#include <stdint.h>
#include "common.h"

typedef struct flash_regs_t{
    __IO uint32_t ACR;
    __IO uint32_t KEYR;
    __IO uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t OPTCR;
}FLASH_regs_typedef_t;
typedef enum{
    FLASH_LATENCY_0ws   = 0x0,
    FLASH_LATENCY_1ws   = 0x1,
    FLASH_LATENCY_2ws   = 0x2,
    FLASH_LATENCY_3ws   = 0x3,
    FLASH_LATENCY_4ws   = 0x4,
    FLASH_LATENCY_5ws   = 0x5,
    FLASH_LATENCY_6ws   = 0x6,
    FLASH_LATENCY_7ws   = 0x7,
    FLASH_LATENCY_8ws   = 0x8,
    FLASH_LATENCY_9ws   = 0x9,
    FLASH_LATENCY_10ws  = 0xA,
    FLASH_LATENCY_11ws  = 0xB,
    FLASH_LATENCY_12ws  = 0xC,
    FLASH_LATENCY_13ws  = 0xD,
    FLASH_LATENCY_14ws  = 0xE,
    FLASH_LATENCY_15ws  = 0xF
}FLASH_latency_t;

typedef enum{
    FLASH_ICACHE_DISABLE    = 0x0, 
    FLASH_ICACHE_ENABLE     = 0x200
}FLASH_icache_state_t;

typedef enum{
    FLASH_DCACHE_DISABLE    = 0x0, 
    FLASH_DCACHE_ENABLE     = 0x400
}FLASH_dcache_state_t;

typedef enum{
    FLASH_PREFETCH_DISABLE    = 0x0, 
    FLASH_PREFETCH_ENABLE     = 0x100
}FLASH_prefetch_state_t;

#define FLASH_REG_BASE  (0x40023c00UL)
#define FLASH_REGS ((__IO FLASH_regs_typedef_t *)FLASH_REG_BASE)

extern inline void flash_acceleration_setup(FLASH_latency_t latency ,FLASH_dcache_state_t dcache, FLASH_icache_state_t icache){
    FLASH_REGS->ACR = latency | dcache | icache;
}

#endif