/**
 * @file hardfault_handler.c
 * @brief HardFault handler with optional UART logging.
 *
 *
 *
 * Usage:
 *  Just add this to your project
 *
 * Notes:
 *  - This is intended for debugging; in production, consider a safe reset
 *    or recovery strategy instead of infinite loop.
 *  - PC address is masked (`pc & ~0x1`) to align with Thumb instruction set.
 */

#include <stdint.h>

#include "common.h"

#ifdef CRAP_HANDLERS_USE_UART

#include "uart.h"
#ifndef FAULT_LOG_UART
#define FAULT_LOG_UART USART1
#endif
#define FAULT_LOG_UART_SETTINGS (USART_FEATURE_TRANSMIT_EN)
#endif
typedef struct
{
  __IO  uint32_t CPUID;                  
  __IO  uint32_t ICSR;                   
  __IO  uint32_t VTOR;                   
  __IO  uint32_t AIRCR;                  
  __IO  uint32_t SCR;                    
  __IO  uint32_t CCR;                    
  __IO  uint8_t  SHP[12U];               
  __IO  uint32_t SHCSR;                  
  __IO  uint32_t CFSR;                   
  __IO  uint32_t HFSR;                   
  __IO  uint32_t DFSR;                   
  __IO  uint32_t MMFAR;                  
  __IO  uint32_t BFAR;                   
  __IO  uint32_t AFSR;                   
  __IO  uint32_t PFR[2U];                
  __IO  uint32_t DFR;                    
  __IO  uint32_t ADR;                    
  __IO  uint32_t MMFR[4U];               
  __IO  uint32_t ISAR[5U];               
        uint32_t RESERVED0[5U];
  __IO  uint32_t CPACR;                  
} SCB_typedef_t;

#define SCB ((SCB_typedef_t *) 0xE000ED00UL)
void __attribute__((naked)) isr_hardfault(){ 
    __asm volatile(
        "cpsid i        \n"
        "tst lr,#4      \n"
        "ite eq         \n"
        "mrseq r0,msp   \n"
        "mrsne r0,psp   \n"
        "b hardfault_c  \n"
    );
}

void hardfault_c(uint32_t* stack){
    volatile uint32_t r0  = stack[0];
    volatile uint32_t r1  = stack[1];
    volatile uint32_t r2  = stack[2];
    volatile uint32_t r3  = stack[3];
    volatile uint32_t r12 = stack[4];
    volatile uint32_t lr  = stack[5]; 
    volatile uint32_t pc  = stack[6];  
    volatile uint32_t psr = stack[7];
    volatile uint32_t hfsr = SCB->HFSR;
    volatile uint32_t cfsr = SCB->CFSR;
    volatile uint32_t mmfar = SCB->MMFAR;
    volatile uint32_t bfar = SCB->BFAR;
    volatile uint32_t afsr = SCB->AFSR;
    volatile uint32_t dfsr = SCB->DFSR;
#ifdef CRAP_HANDLERS_USE_UART
    uart_disable(FAULT_LOG_UART);
    uart_setup(FAULT_LOG_UART,USART_BRR_9600,FAULT_LOG_UART_SETTINGS,0,0);
    uart_enable(FAULT_LOG_UART);
    uart_send_str_polling(FAULT_LOG_UART,"HARDFAULT\r\n");
    uart_send_str_polling(FAULT_LOG_UART, "R0 = ");
    uart_send_hex_32bit(FAULT_LOG_UART, r0);
 
    uart_send_str_polling(FAULT_LOG_UART, "R1 = ");
    uart_send_hex_32bit(FAULT_LOG_UART, r1);

    uart_send_str_polling(FAULT_LOG_UART, "R2 = ");
    uart_send_hex_32bit(FAULT_LOG_UART, r2);

    uart_send_str_polling(FAULT_LOG_UART, "R3 = ");
    uart_send_hex_32bit(FAULT_LOG_UART, r3);

    uart_send_str_polling(FAULT_LOG_UART, "R12 = ");
    uart_send_hex_32bit(FAULT_LOG_UART, r12);

    uart_send_str_polling(FAULT_LOG_UART, "LR = ");
    uart_send_hex_32bit(FAULT_LOG_UART, lr);

    uart_send_str_polling(FAULT_LOG_UART, "!!PC = ");
    uart_send_hex_32bit(FAULT_LOG_UART, pc&~(0x1));

    uart_send_str_polling(FAULT_LOG_UART, "xPSR =  ");
    uart_send_hex_32bit(FAULT_LOG_UART, psr);

    uart_send_str_polling(FAULT_LOG_UART, "SP = ");
    uart_send_hex_32bit(FAULT_LOG_UART, (uint32_t)stack);



    uart_send_str_polling(FAULT_LOG_UART, "CFSR = ");
    uart_send_hex_32bit(FAULT_LOG_UART, cfsr);

    uart_send_str_polling(FAULT_LOG_UART, "HFSR = ");
    uart_send_hex_32bit(FAULT_LOG_UART, hfsr);

    uart_send_str_polling(FAULT_LOG_UART, "DFSR = ");
    uart_send_hex_32bit(FAULT_LOG_UART, dfsr);

    uart_send_str_polling(FAULT_LOG_UART, "MMFAR = ");
    uart_send_hex_32bit(FAULT_LOG_UART, mmfar);

    uart_send_str_polling(FAULT_LOG_UART,"BFAR = " );
    uart_send_hex_32bit(FAULT_LOG_UART, bfar);

    uart_send_str_polling(FAULT_LOG_UART, "AFSR = ");
    uart_send_hex_32bit(FAULT_LOG_UART,afsr );
#endif
    while (1) {
    
    }
}

