#pragma once
#ifndef BAD_SCB
#define BAD_SCB

#include <stdint.h>

#include "common.h"


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





#endif
