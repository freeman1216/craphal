# CRAPHAL – Bare-Metal Drivers for STM32

**Why “CRAPHAL”?**  
Because honestly, the standard HAL library drove me nuts. Too much boilerplate, too many places where the state changes, some unexplained delays (DMA busy still haunts me). So I decided to just write my own lightweight drivers that actually make sense.

## What is this?  
CRAP is a collection of simple, mostly header-only (stb style) drivers for STM32F411CEU6. It includes:  

- GPIO (`io.h`) - easy pin setup, set/reset pins, configure alternate functions.  
- NVIC (`nvic.h`) - enable/disable interrupts, simple as that.  
- SPI (`spi.h`) - full SPI setup, DMA support, interrupt-friendly.  
- EXTI (`exti.h`) - external interrupts with configurable trigger.  
- Assert (`assert.h`) - prints messages over UART if things go wrong.  
- ILI9341 (`ili9341.h`) - basic LCD driver with DMA framebuffer support.  
- DMA (`dma.h`) - DMA control and interrupts
- UART (`uart.h`) - Basic uart stuff
- SYSCFG (`syscfg.h`) - Syscfg, for now only for exti
- Flash (`flash.h`) - setup latency, caches, and prefetch.
- RCC  (`rcc.h`) - clock configuration 
- Handlers (`handlers.c`) - for now only implements a hardfault that reports the registers state at fault
- Startup (`startup_stm32f411ceu6.c`) - startup file, plain and simple
- Simple linker script (`stm32f411ceu6.ld`)

All drivers are **lightweight**, **easy to read**, and **easy to modify**, also easily inlinable by the compiler.
## How to use it  
1. Include the header in your project.  
2. If a driver has a `_IMPLEMENTATION` define, enable it in **one C file**, or use it statically:

```c
#define CRAP_IO_IMPLEMENTATION
#include "io.h"
```
3. Use the function!
```c
// Configure PA5 as push-pull output
io_setup_pin(GPIOA, 5,
             MODER_output,
             0, // AF not used
             OSPEEDR_medium_speed,
             PUPDR_no_pull,
             OTYPR_push_pull);

// Toggle pin
io_pin_set(GPIOA, 5);
io_pin_reset(GPIOA, 5);
```
Do note that some of the functions can only be included once and used only in the file its included (io_setup_pin and all the rcc and flash stuff).
Why is it this way? I just find it nicer when all the setup is in one place.

## Notes

All the setup functions except io simply overwrite the configuration register.
Why? 90% of projects just don`t change the peripheral settings or just have at most two modes and the added cruft of managing the state of the settings doesnt make much sense at that point, if you wanna see how bad it gets just look up hal SPI read/write functions.
If you need multiple settings for your project just do this
```c
#define ILI9341_SPI_FEATURES_CMD                \
                    (SPI_FEATURE_MASTER|        \
                    SPI_FEATURE_PRECALER_div_2| \
                    SPI_FEATURE_SOFTWARE_CS|    \
                    SPI_FEATURE_BIDIOE_TX_ONLY| \
                    SPI_FEATURE_BIDIRECTIONAL|  \
                    SPI_FEATURE_FRAME_FORMAT_8bit)

#define ILI9341_SPI_FEATURES_DMA                \
                (SPI_FEATURE_MASTER|            \
                SPI_FEATURE_PRECALER_div_2|     \
                SPI_FEATURE_SOFTWARE_CS|        \
                SPI_FEATURE_BIDIOE_TX_ONLY|     \
                SPI_FEATURE_BIDIRECTIONAL|      \
                SPI_FEATURE_FRAME_FORMAT_16bit| \
                SPI_FEATURE_FORMAT_MSB )
```
And then just make some helpers to apply them
```c
static inline void ili9341_spi_fb_transmition_mode(){
    spi_disable(ILI9341_SPI);
    spi_setup(ILI9341_SPI,ILI9341_SPI_FEATURES_DMA,0,0);
    spi_enable(ILI9341_SPI);
}

static inline void ili9341_spi_control_transmition_mode(){
    spi_disable(ILI9341_SPI);;
    spi_setup(ILI9341_SPI,ILI9341_SPI_FEATURES_CMD,0,0);
    spi_enable(ILI9341_SPI);
}
```
Which is what HAL does anyway just with smothering it all over the read/write functions. Don`t turn a simple ldr imm str r into a hundred branches please. 

The main(`main.c`) file implements an example that 
- Setups main clock at 100mhz
- Setups apropriate latency / caching
- Setups peripheral clocks
- Setups pins for SPI
- Setups ILI9341 display
- Fills the screen in polling
- Produces a bitmap
- Sends it over dma
- Produces another one and polls till dma is finished
- Repeats the cycle



You can freely modify or copy whatever you need.