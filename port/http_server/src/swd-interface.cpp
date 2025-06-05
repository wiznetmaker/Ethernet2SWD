#include "swdloader.h"
#include <stdio.h>

// GPIO pin configuration
#define SWCLK_PIN		2
#define SWDIO_PIN		3
#define SWD_RESET_PIN		4		// 0 for none

#define SWD_CLOCK_RATE_KHZ	400

#define RP2040_RAM_BASE		0x20000000U


// 👇 This makes the function callable from C files
extern "C" bool swdloader_flash_buffer(const uint8_t* buffer, size_t size) {
    
    CSWDLoader loader(SWCLK_PIN, SWDIO_PIN, SWD_RESET_PIN, 400);  // SWCLK, SWDIO, RESET, 1MHz
    if (!loader.Initialize()) {
        printf("SWD init failed!\n");
        return 0;
    }

    printf("SWD init OK.\n");

    if(!loader.Load(buffer, size, RP2040_RAM_BASE))
        printf("Firmware load failed\r\n");  // Flash address on target MCU

    uint32_t word = 0;
    int result = loader.ReadMem(RP2040_RAM_BASE, &word);
    if (!result) {
        printf("SWD read failed.\n");
        return 0;
    }

    printf("Read flash : 0x%08X\n", word);

    if (word == 0xFFFFFFFF || word == 0x00000000) {
        printf("Flash looks empty or erased.\n");
    } else {
        printf("Flash likely contains firmware.\n");
    }

    return 1;
}

