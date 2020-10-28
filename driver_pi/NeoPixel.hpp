/**
 * @file neoPixel.h
 * @author
 * @brief
 * @version 0.1
 * @date 2020-10-06
 *
 * @copyright Copyright (c) 2020
 *
 */


#ifndef DRIVER_DRIVER_PI3_NEOPIXEL_H
#define DRIVER_DRIVER_PI3_NEOPIXEL_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#include <vector>

#include "../error.hpp"
#include "log.hpp"
#include "types.hpp"

#define NUM_DATA_WORDS 1016

//typedef struct {
struct Color_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color_t(unsigned char r=0, unsigned char g=0, unsigned char b=0)
	: r(r), g(g), b(b)
    {}

    bool operator==(const Color_t& other) const {
        return (r==other.r && g==other.g && b==other.b);
    }

    bool operator!=(const Color_t& other) const {
        return (r!=other.r && g!=other.g && b!=other.b);
    }

//} Color_t;
};

typedef struct {
    unsigned int
        info,
        src,
        dst,
        length,
        stride,
        next,
        pad[2];
} dma_cb_t;

typedef struct {
    uint8_t *virtaddr;
    uint32_t physaddr;
} page_map_t;

typedef struct {
    dma_cb_t cb[1];
    uint32_t sample[NUM_DATA_WORDS];
} control_data_s;

class NeoPixel {
 private:
  std::vector<Color_t> LEDBuffer;
  unsigned int numLEDs;
  float brightness_global;
  unsigned int PWMWaveform[NUM_DATA_WORDS];
  control_data_s *ctl;
  page_map_t *page_map;
  static uint8_t *virtbase;
  volatile unsigned int *pwm_reg;
  volatile unsigned int *clk_reg;
  volatile unsigned int *dma_reg;
  volatile unsigned int *gpio_reg;

  void clearPWMBuffer();
  void* map_peripheral(uint32_t base, uint32_t len);
  unsigned int mem_virt_to_phys(void *virt);
  static Color_t RGB2Color(uint8_t r, uint8_t g, uint8_t b);
  void startTransfer();
  void setPWMBit(unsigned int bitPos, unsigned char bit);
  void show();

 public:
  NeoPixel();
  void init(gpio_num_t gpio, uint16_t neo_count);
  void set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  void set_brightness(uint16_t index, float brightness);

};

// Defines
// Base Addresses
#define DMA_BASE        0x20007000
#define DMA_LEN         0x24
#define PWM_BASE        0x2020C000
#define PWM_LEN         0x28
#define CLK_BASE        0x20101000
#define CLK_LEN         0xA8
#define GPIO_BASE       0x20200000
#define GPIO_LEN        0xB4

// GPIO
#define GPFSEL0         0x20200000          
#define GPFSEL1         0x20200004          
#define GPFSEL2         0x20200008          
#define GPFSEL3         0x2020000C          
#define GPFSEL4         0x20200010          
#define GPFSEL5         0x20200014          
#define GPSET0          0x2020001C          
#define GPCLR0          0x20200028          
#define GPPUD           0x20200094          
#define GPPUDCLK0       0x20200098          
#define GPPUDCLK1       0x2020009C

// Memory Offsets 
#define PWM_CLK_CNTL    40
#define PWM_CLK_DIV     41

// PWM Register Addresses
#define PWM_CTL  0x00
#define PWM_STA  (0x04 / 4)
#define PWM_DMAC (0x08 / 4)
#define PWM_RNG1 (0x10 / 4)
#define PWM_DAT1 (0x14 / 4)
#define PWM_FIF1 (0x18 / 4)
#define PWM_RNG2 (0x20 / 4)
#define PWM_DAT2 (0x24 / 4)

// PWM_CTL register bit offsets
#define PWM_CTL_MSEN2   15
#define PWM_CTL_USEF2   13
#define PWM_CTL_POLA2   12
#define PWM_CTL_SBIT2   11
#define PWM_CTL_RPTL2   10
#define PWM_CTL_MODE2   9
#define PWM_CTL_PWEN2   8
#define PWM_CTL_CLRF1   6
#define PWM_CTL_MSEN1   7
#define PWM_CTL_USEF1   5
#define PWM_CTL_POLA1   4
#define PWM_CTL_SBIT1   3
#define PWM_CTL_RPTL1   2
#define PWM_CTL_MODE1   1
#define PWM_CTL_PWEN1   0

// PWM_STA register bit offsets
#define PWM_STA_STA4    12
#define PWM_STA_STA3    11
#define PWM_STA_STA2    10
#define PWM_STA_STA1    9
#define PWM_STA_BERR    8
#define PWM_STA_GAPO4   7
#define PWM_STA_GAPO3   6
#define PWM_STA_GAPO2   5
#define PWM_STA_GAPO1   4
#define PWM_STA_RERR1   3
#define PWM_STA_WERR1   2
#define PWM_STA_EMPT1   1
#define PWM_STA_FULL1   0

// PWM_DMAC bit offsets
#define PWM_DMAC_ENAB   31
#define PWM_DMAC_PANIC  8
#define PWM_DMAC_DREQ   0

// DMA
#define DMA_CS              (0x00 / 4)
#define DMA_CONBLK_AD       (0x04 / 4)
#define DMA_TI              (0x08 / 4)
#define DMA_SOURCE_AD       (0x0C / 4)
#define DMA_DEST_AD         (0x10 / 4)
#define DMA_TXFR_LEN        (0x14 / 4)
#define DMA_STRIDE          (0x18 / 4)
#define DMA_NEXTCONBK       (0x1C / 4)
#define DMA_DEBUG           (0x20 / 4)

// DMA Control & Status register bit offsets
#define DMA_CS_RESET          31
#define DMA_CS_ABORT          30
#define DMA_CS_DISDEBUG       29
#define DMA_CS_WAIT_FOR       28
#define DMA_CS_PANIC_PRI      20
#define DMA_CS_PRIORITY       16
#define DMA_CS_ERROR          8
#define DMA_CS_WAITING_FOR    6
#define DMA_CS_DREQ_STOPS_DMA 5
#define DMA_CS_PAUSED         4
#define DMA_CS_DREQ           3
#define DMA_CS_INT            2
#define DMA_CS_END            1
#define DMA_CS_ACTIVE         0
// Default CS word
#define DMA_CS_CONFIGWORD   (8 << DMA_CS_PANIC_PRI) | \
                            (8 << DMA_CS_PRIORITY) | \
                            (1 << DMA_CS_WAIT_FOR)

// DREQ lines
#define DMA_DREQ_ALWAYS     0
#define DMA_DREQ_PCM_TX     2
#define DMA_DREQ_PCM_RX     3
#define DMA_DREQ_PWM        5
#define DMA_DREQ_SPI_TX     6
#define DMA_DREQ_SPI_RX     7
#define DMA_DREQ_BSC_TX     8
#define DMA_DREQ_BSC_RX     9

// DMA Transfer Information register bit offsets
#define DMA_TI_NO_WIDE_BURSTS   26
#define DMA_TI_WAITS            21
#define DMA_TI_PERMAP           16
#define DMA_TI_BURST_LENGTH     12
#define DMA_TI_SRC_IGNORE       11
#define DMA_TI_SRC_DREQ         10
#define DMA_TI_SRC_WIDTH        9
#define DMA_TI_SRC_INC          8
#define DMA_TI_DEST_IGNORE      7
#define DMA_TI_DEST_DREQ        6
#define DMA_TI_DEST_WIDTH       5
#define DMA_TI_DEST_INC         4
#define DMA_TI_WAIT_RESP        3
#define DMA_TI_TDMODE           1
#define DMA_TI_INTEN            0
// Default TI word
#define DMA_TI_CONFIGWORD       (1 << DMA_TI_NO_WIDE_BURSTS) | \
                                (1 << DMA_TI_SRC_INC) | \
                                (1 << DMA_TI_DEST_DREQ) | \
                                (1 << DMA_TI_WAIT_RESP) | \
                                (1 << DMA_TI_INTEN) | \
                                (DMA_DREQ_PWM << DMA_TI_PERMAP)

// DMA Debug register bit offsets
#define DMA_DEBUG_LITE                  28
#define DMA_DEBUG_VERSION               25
#define DMA_DEBUG_DMA_STATE             16
#define DMA_DEBUG_DMA_ID                8
#define DMA_DEBUG_OUTSTANDING_WRITES    4
#define DMA_DEBUG_READ_ERROR            2
#define DMA_DEBUG_FIFO_ERROR            1
#define DMA_DEBUG_READ_LAST_NOT_SET     0

#define PAGE_SIZE   4096
#define PAGE_SHIFT  12
#define NUM_PAGES   ((sizeof(control_data_s) + PAGE_SIZE - 1) >> PAGE_SHIFT)

#define SETBIT(word, bit) word |= 1<<bit
#define CLRBIT(word, bit) word &= ~(1<<bit)
#define GETBIT(word, bit) word & (1 << bit) ? 1 : 0
#define true 1
#define false 0

// GPIO
#define INP_GPIO(g) *(gpio_reg+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio_reg+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio_reg+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET *(gpio_reg+7)
#define GPIO_CLR *(gpio_reg+10)

#define DEFAULT_BRIGHTNESS 1.0

#endif /* DRIVER_DRIVER_PI3_NEOPIXEL_H */
