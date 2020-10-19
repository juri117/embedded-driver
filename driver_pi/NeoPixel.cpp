
#include "NeoPixel.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char* TAG = "neoPixel";

NeoPixel::NeoPixel() {}

void NeoPixel::init(gpio_num_t gpio, uint16_t neo_count) {
    LEDBuffer.resize(neo_count);  // Set the number of neopixels in the vector
    brightness_global = DEFAULT_BRIGHTNESS;  // Set default brightness_global
    int i = 0;
    int pid;
    int fd;
    char pagemap_fn[64];

    // Clear the PWM buffer
    clearPWMBuffer();

    // Set up peripheral access
    dma_reg = (unsigned int*) map_peripheral(DMA_BASE, DMA_LEN);
    dma_reg += 0x000;
    pwm_reg = (unsigned int*)map_peripheral(PWM_BASE, PWM_LEN);
    clk_reg = (unsigned int*)map_peripheral(CLK_BASE, CLK_LEN);
    gpio_reg = (unsigned int*)map_peripheral(GPIO_BASE, GPIO_LEN);


    // Set PWM alternate function for GPIO18
    SET_GPIO_ALT(18, 5);

    // Allocate memory for the DMA control block & data to be sent
    virtbase = (uint8_t*) mmap(
        NULL,
        NUM_PAGES * PAGE_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED |
        MAP_ANONYMOUS |
        MAP_NORESERVE |
        MAP_LOCKED,
        -1,
        0);

    if (virtbase == MAP_FAILED) {
        log_e(TAG, "Failed to mmap physical pages: %m\n");
    }

    if ((unsigned long)virtbase & (PAGE_SIZE-1)) {
        log_e(TAG, "Virtual address is not page aligned\n");
    }

    // Allocate page map (pointers to the control block(s) and data for each CB
    page_map =(page_map_t*) malloc(NUM_PAGES * sizeof(*page_map));
    if (page_map == 0)
        log_e(TAG, "Failed to malloc page_map: %m\n");

    pid = getpid();
    sprintf(pagemap_fn, "/proc/%d/pagemap", pid);
    fd = open(pagemap_fn, O_RDONLY);

    if (fd < 0) {
        log_e(TAG, "Failed to open %s: %m\n", pagemap_fn);
    }

    if (lseek(fd, (unsigned long)virtbase >> 9, SEEK_SET) != (unsigned long)virtbase >> 9) {
        log_e(TAG, "Failed to seek on %s: %m\n", pagemap_fn);
    }

    for (i = 0; i < NUM_PAGES; i++) {
        uint64_t pfn;
        page_map[i].virtaddr = virtbase + i * PAGE_SIZE;

        page_map[i].virtaddr[0] = 0;

        if (read(fd, &pfn, sizeof(pfn)) != sizeof(pfn)) {
            log_e(TAG, "Failed to read %s: %m\n", pagemap_fn);
        }

        if ((pfn >> 55)&0xfbf != 0x10c) {
            log_e(TAG, "Page %d not present (pfn 0x%016llx)\n", i, pfn);
        }

        page_map[i].physaddr = (unsigned int)pfn << PAGE_SHIFT | 0x40000000;
    }

    // Set up control block
    ctl = (struct control_data_s *)virtbase;
    dma_cb_t *cbp = ctl->cb;
    unsigned int phys_pwm_fifo_addr = 0x7e20c000 + 0x18;

    cbp->info = DMA_TI_CONFIGWORD;

    cbp->src = mem_virt_to_phys(ctl->sample);   

    cbp->dst = phys_pwm_fifo_addr;

    cbp->length = ((numLEDs * 2.25) + 1) * 4;
    if(cbp->length > NUM_DATA_WORDS * 4) {
        cbp->length = NUM_DATA_WORDS * 4;
    }

    cbp->stride = 0;  
    cbp->pad[0] = 0;
    cbp->pad[1] = 0;   
    cbp->next = 0;

    dma_reg[DMA_CS] |= (1 << DMA_CS_ABORT);
    usleep(100);
    dma_reg[DMA_CS] = (1 << DMA_CS_RESET);
    usleep(100);


    // PWM Clock
    clk_reg[PWM_CLK_CNTL] = 0x5A000000 | (1 << 5);
    usleep(100);

    CLRBIT(pwm_reg[PWM_DMAC], PWM_DMAC_ENAB);
    usleep(100);

    unsigned int idiv = 400;
    unsigned short fdiv = 0;
    clk_reg[PWM_CLK_DIV] = 0x5A000000 | (idiv << 12) | fdiv;
    usleep(100);

    clk_reg[PWM_CLK_CNTL] = 0x5A000015;
    usleep(100);


    // PWM
    pwm_reg[PWM_CTL] = 0;

    pwm_reg[PWM_RNG1] = 32;
    usleep(100);
    
    pwm_reg[PWM_DMAC] =
        (1 << PWM_DMAC_ENAB) |
        (8 << PWM_DMAC_PANIC) |
        (8 << PWM_DMAC_DREQ);
    usleep(1000);
    
    SETBIT(pwm_reg[PWM_CTL], PWM_CTL_CLRF1);
    usleep(100);
    
    CLRBIT(pwm_reg[PWM_CTL], PWM_CTL_RPTL1);
    usleep(100);
    
    CLRBIT(pwm_reg[PWM_CTL], PWM_CTL_SBIT1);
    usleep(100);
    
    CLRBIT(pwm_reg[PWM_CTL], PWM_CTL_POLA1);
    usleep(100);
    
    SETBIT(pwm_reg[PWM_CTL], PWM_CTL_MODE1);
    usleep(100);
    
    SETBIT(pwm_reg[PWM_CTL], PWM_CTL_USEF1);
    usleep(100);

    CLRBIT(pwm_reg[PWM_CTL], PWM_CTL_MSEN1);
    usleep(100);   

    SETBIT(dma_reg[DMA_CS], DMA_CS_INT);
    usleep(100);
    
    SETBIT(dma_reg[DMA_CS], DMA_CS_END);
    usleep(100);
    
    dma_reg[DMA_CONBLK_AD] = mem_virt_to_phys(ctl->cb);
    usleep(100);

    dma_reg[DMA_DEBUG] = 7;
    usleep(100);
}

void NeoPixel::set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if(index < 0) {
        printf("Unable to set pixel %d (less than zero?)\n", index);
        //return false;
    }
    if(index > numLEDs - 1) {
        printf("Unable to set pixel %d (LED buffer is %d pixels long)\n", index, numLEDs);
        //return false;
    }
    LEDBuffer[index] = RGB2Color(r, g, b);
    //return true;
}

void NeoPixel::set_brightness(uint16_t index, float brightness) {
    if(brightness < 0) {
        printf("Brightness can't be set below 0.\n");
        //return false;
    }
    if(brightness > 1) {
        printf("Brightness can't be set above 1.\n");
        //return false;
    }
    brightness_global = brightness;
    //return true;
}

// Private
void NeoPixel::clearPWMBuffer(){
    memset(PWMWaveform, 0, NUM_DATA_WORDS * 4);
}

void* NeoPixel::map_peripheral(uint32_t base, uint32_t len){
    int fd = open("/dev/mem", O_RDWR);
    void * vaddr;

    if (fd < 0)
        log_e(TAG, "Failed to open /dev/mem: %m\n");
    vaddr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, base);
    if (vaddr == MAP_FAILED)
        log_e(TAG, "Failed to map peripheral at 0x%08x: %m\n", base);
    close(fd);

    return vaddr;    
}

unsigned int NeoPixel::mem_virt_to_phys(void *virt){
    unsigned int offset = (uint8_t *)virt - virtbase;
    return page_map[offset >> PAGE_SHIFT].physaddr + (offset % PAGE_SIZE);    
}

Color_t NeoPixel::RGB2Color(uint8_t r, uint8_t g, uint8_t b){
    //Color_t color = { r, g, b };
    Color_t color(r, g, b);
    return color;
}

#endif