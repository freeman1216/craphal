/**
 * @file rcc.h
 * @brief RCC header only driver
 *
 *
 * Usage:
 *  - Include this file in your project.
 *  - Define `CRAP_RCC_IMPLEMENTATION` in **one** C file to include
 *    inline function definitions.
 *  - Only one function (rcc_fallback_to_hsi()) can be called from other files
 *  - Use provided functions to configure clocks, PLL, and enable peripherals.
 *
 * Example:
 *  #define CRAP_RCC_IMPLEMENTATION
 *  #include "rcc.h"
 *  
 *   rcc_enable_hse();
 *   rcc_pll_setup( PLLP4, 25, 400, 10, PLL_SOURCE_HSE);
 *   rcc_bus_prescalers_setup(HPRE_DIV_1, PPRE_DIV_2, PPRE_DIV_1);
 *  
 *   rcc_enable_and_switch_to_pll();
 *
 */

#pragma once

#ifndef CRAP_RCC_H
#define CRAP_RCC_H


extern void rcc_fallback_to_hsi();
#ifdef CRAP_RCC_IMPLEMENTATION
#include <stdint.h>

#include "common.h"

typedef struct RCC_regs_t{
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    uint32_t RESERVED0;
    uint32_t RESERVED1;
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t RESERVED3;
    uint32_t RESERVED4;
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    uint32_t RESERVED5;
    uint32_t RESERVED6;
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
    uint32_t RESERVED7;
    uint32_t RESERVED8;
    __IO uint32_t AHB1LPENR;
    __IO uint32_t AHB2LPENR;
    uint32_t RESERVED9;
    uint32_t RESERVED10;
    __IO uint32_t APB1LPENR;
    __IO uint32_t APB2LPENR;
    uint32_t RESERVED11;
    uint32_t RESERVED12;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    uint32_t RESERVED13;
    uint32_t RESERVED14;
    __IO uint32_t SSCGR;
    __IO uint32_t PLLI2SCFGR;
}RCC_typedef_t;

#define HSEON_MASK  (0x10000U)
#define HSERDY_MASK (0x20000U)
#define HSION_MASK  (0x1U)
#define HSIRDY_MASK (0x2U)
#define PLLON_MASK  (0x1U << 24)
#define PLLRDY_MASK (0x1U << 25)

typedef enum {
    PLL_SOURCE_HSI = 0x0,
    PLL_SOURCE_HSE = 0x400000
} PLL_source_t;

#define PLLSRC_MASK (0x1U << 22)
#define PLLN_SET(val)((val & 0x1FF) << 6)
#define PLLM_SET(val)((val & 0x3F))

typedef enum{
    PLLP2 = 0x0,
    PLLP4 = 0x10000,
    PLLP6 = 0x20000,
    PLLP8 = 0x40000
} PLLP_states_t;


#define PLLQ_SET(val)((val & 0xFU) << 24)


#define PPRE1_SET(val)((val & 0x7)<<10)
#define PPRE2_SET(val)((val & 0x7)<<13)
#define PPRE1_MASK (0x7<<10)
#define PPRE2_MASK (0x7<<13)
typedef enum{
    PPRE_DIV_1 = 0x0, //actually it is not DIVided if top(2) bit is not set
    PPRE_DIV_2 = 0b100,
    PPRE_DIV_4 = 0b101,
    PPRE_DIV_8 = 0b110,
    PPRE_DIV_16 = 0b111
} PPRE_state_t;

#define HPRE_SET(val)((val & 0xFU)<<4)
#define HPRE_MASK (0xF<<4)
typedef enum{
    HPRE_DIV_1 = 0x0, //actually it is not DIVided if top(15) bit is not set
    HPRE_DIV_2 = 0b1000,
    HPRE_DIV_4 = 0b1001,
    HPRE_DIV_8 = 0b1010,
    HPRE_DIV_16 = 0b1011,
    HPRE_DIV_64 = 0b1100,
    HPRE_DIV_128 = 0b1101,
    HPRE_DIV_256 = 0b1110,
    HPRE_DIV_512 = 0b1111
} HPRE_state_t;

typedef enum{
    SW_HSI = 0x0,
    SW_HSE = 0x1,
    SW_PLL = 0x2,
    SW_NOT_ALLOWED = 0x3
}SW_state_t;

typedef enum{
    RCC_AHB1_GPIOA = 0x1,
    RCC_AHB1_GPIOB = 0x2,
    RCC_AHB1_GPIOC = 0x4,
    RCC_AHB1_GPIOD = 0x8,
    RCC_AHB1_CRCEN = 0x1000,
    RCC_AHB1_DMA1  = 0x200000,
    RCC_AHB1_DMA2  = 0x400000  
}RCC_AHB1_peripherals_t;

typedef enum{
    RCC_AHB2_USB_OTG_FS = 0x80
}RCC_AHB2_peripherals_t;

typedef enum{
    RCC_APB1_TIM2   = 0x1,
    RCC_APB1_TIM3   = 0x2,
    RCC_APB1_TIM4   = 0x4,
    RCC_APB1_TIM5   = 0x8,
    RCC_APB1_WWDG   = 0x800,
    RCC_APB1_SPI2   = 0x4000,
    RCC_APB1_SPI3   = 0x8000,
    RCC_APB1_USART2 = 0x20000,
    RCC_APB1_I2C1   = 0x200000,
    RCC_APB1_I2C2   = 0x400000,
    RCC_APB1_I2C3   = 0x800000,
    RCC_APB1_PWR    = 0x1000000
}RCC_APB1_peripherals_t;


typedef enum{
    RCC_APB2_TIM1       = 0x1,
    RCC_APB2_USART1     = 0x10, 
    RCC_APB2_USART6     = 0x20,
    RCC_APB2_ADC1       = 0x100, 
    RCC_APB2_SDIO       = 0x800,
    RCC_APB2_SPI1       = 0x1000,
    RCC_APB2_SPI4       = 0x2000,
    RCC_APB2_SYSCFGEN   = 0x4000,
    RCC_APB2_TIM9       = 0x10000,
    RCC_APB2_TIM10      = 0x20000,
    RCC_APB2_TIM11      = 0x40000,
    RCC_APB2_SPI5       = 0x100000
}RCC_APB2_peripherals_t;

#define SWS_MASK (0x3 << 2)
#define SW_MASK SW_NOT_ALLOWED

#define RCC_BASE (0x40023800UL)
#define RCC ((__IO RCC_typedef_t *)RCC_BASE)

extern inline void rcc_enable_hsi(void) {
    RCC->CR |= HSION_MASK;
    while (!(RCC->CR & HSIRDY_MASK));
}

extern void rcc_fallback_to_hsi(){
    rcc_enable_hsi();
    RCC->CFGR &= ~(SW_NOT_ALLOWED);
    RCC->CFGR |= SW_HSI;
    while ((RCC->CFGR & SWS_MASK)  != SW_HSI<<2);

}

extern inline void rcc_enable_hse(void) {
    RCC->CR |= HSEON_MASK;
    while (!(RCC->CR & HSERDY_MASK));
}

extern inline void rcc_set_ahb1_clocking(RCC_AHB1_peripherals_t ahb1_mask){
    RCC->AHB1ENR = ahb1_mask;
}

extern inline void rcc_set_ahb2_clocking(RCC_AHB2_peripherals_t ahb2_mask){
    RCC->AHB2ENR = ahb2_mask;
}

extern inline void rcc_set_apb1_clocking(RCC_APB1_peripherals_t apb1_mask){
    RCC->APB1ENR = apb1_mask;
}

extern inline void rcc_set_apb2_clocking(RCC_APB1_peripherals_t apb2_mask){
    RCC->APB2ENR = apb2_mask;
}

extern inline void rcc_enable_and_switch_to_pll(){
    RCC->CR |= PLLON_MASK;
    while (!(RCC->CR & PLLRDY_MASK));
    RCC->CFGR &= ~(SW_MASK);
    RCC->CFGR |= SW_PLL;
    while ((RCC->CFGR & SWS_MASK)  != SW_PLL<<2);// SWS repots same bits as SW just 2 bits farther
}

extern inline void rcc_bus_prescalers_setup(HPRE_state_t ahb_prescaler,PPRE_state_t apb1_prescaler,
    PPRE_state_t apb2_prescaler)
{
    RCC->CFGR &= ~(PPRE1_MASK | PPRE2_MASK | HPRE_MASK);
    RCC->CFGR |= PPRE1_SET(apb1_prescaler) | PPRE2_SET(apb2_prescaler) | HPRE_SET(ahb_prescaler);
}

extern inline void rcc_pll_setup(PLLP_states_t PLLP,uint8_t PLLM,uint16_t PLLN,uint8_t PLLQ, PLL_source_t source){
    
    if(!(RCC->CR & HSION_MASK) && source == PLL_SOURCE_HSI){
        rcc_enable_hsi();
    }

    if(RCC->CR & PLLON_MASK){
        RCC->CR &=~(PLLON_MASK); 
        while (RCC->CR & PLLRDY_MASK);
    }
    if(!(RCC->CR & HSEON_MASK) && source == PLL_SOURCE_HSE){
        rcc_enable_hse();
    }
    RCC->PLLCFGR = PLLP | PLLM_SET(PLLM) | PLLN_SET(PLLN) | PLLQ_SET(PLLQ) | source;  
    

}

#endif
#endif