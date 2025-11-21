
#pragma once
#ifndef BAD_TIMER
#define BAD_TIMER

#include <stdint.h>

#include "common.h"

#ifndef BAD_TIMER_DEF
#ifdef BAD_TIMER_STATIC
    #define BAD_TIMER_DEF ALWAYS_INLINE
#else
    #define BAD_TIMER_DEF extern
#endif
#endif

typedef struct {
    __IO uint32_t CR1;
    uint32_t PADDING0[2];
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCER;
    uint32_t PADDING1;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    uint32_t PADDING2;
    __IO uint32_t CCR1;
} BTIMER_typedef_t;

typedef enum{
    BTIMER_UPDATE = 0x1,
    BTIMER_CC = 0x2
}BTIMER_interrupts_t;

#define BTIM10_BASE 0x40014400UL

#define BTIM10 ((BTIMER_typedef_t *)BTIM10_BASE)

#define TIM_CR_CEN 0x1

ALWAYS_INLINE void tim_enable(BTIMER_typedef_t* TIM){
    TIM->CR1 |= TIM_CR_CEN; 
}

ALWAYS_INLINE void tim_disable(BTIMER_typedef_t* TIM){
    TIM->CR1 &= ~TIM_CR_CEN;
}
BAD_TIMER_DEF void basic_timer_setup(BTIMER_typedef_t* TIM,uint16_t barr,uint16_t bpsc, BTIMER_interrupts_t intr);
#ifdef BAD_TIMER_IMPLEMENTATION
BAD_TIMER_DEF void basic_timer_setup(BTIMER_typedef_t* TIM,uint16_t barr,uint16_t bpsc, BTIMER_interrupts_t intr){
    TIM->ARR = barr;
    TIM->PSC = bpsc; 
    TIM->EGR = 1;
    TIM->DIER = intr;
}
#endif
#ifdef BTIMER_TIM1_UP_TIM10_ISR_IMPLEMENTATION
#ifdef BTIMER_USE_TIM10_USR
void tim10_usr();
#endif
#define TIM_SR_UIF 0x1
STRONG_ISR(tim1_up_tim10_isr){
    if(BTIM10->SR & TIM_SR_UIF ){
      BTIM10->SR &= ~TIM_SR_UIF;
#ifdef BTIMER_USE_TIM10_USR
      tim10_usr();
#endif
    } 
}
#endif
#endif
