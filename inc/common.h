#pragma once
#ifndef BAD_COMMON_H
#define BAD_COMMON_H
//common defines and other stuff
#define __IO volatile

#define CLOCK_SPEED 100000000UL  // 100 MHz
#define FALLBACK_CLOCK_SPEED 16000000 //16MHZ
//hw interrupts (triggered by hardware and handled in drivers)
#define STRONG_ISR(x) void x(void)
#define WEAK_ISR(x) void x(void) __attribute__((weak, alias("default_isr")))
#define STRONG_USER_ISR(x,...) void x(__VA_ARGS__)
#define WEAK_USER_ISR(x,...) void x(__VA_ARGS__) __attribute__((weak, alias(#x"_default")))
#define DEFAULT_USER_ISR(x,...) void x##_default(__VA_ARGS__)
#define WEAK_PERIPH_USER_ISR(x,default_isr,...) void x(__VA_ARGS__) __attribute__((weak, alias(#default_isr"_default")))
#define ATTR_RAMFUNC __attribute__((section(".ramfunc")))
#define ALWAYS_INLINE static inline

#define __ENABLE_INTERUPTS __asm volatile ("cpsie i")
#define __DISABLE_INTERUPTS __asm volatile ("cpsid i")
#endif
