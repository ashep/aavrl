#include <string.h>
#include <stdio.h>
#include "spi.h"
#include "max7219.h"


/**
  * Send a command to the display
  */
void max7219_command(MAX7219Config *config, uint8_t addr, uint8_t data, uint8_t latch) {
    spi_send_byte(config->spi, addr);
    spi_send_byte(config->spi, data);

    if (latch)
        spi_latch(config->spi);
}

/**
 * Send a command to all displays
 */
void max7219_command_all(MAX7219Config *config, uint8_t addr, uint8_t data, uint8_t latch) {
    for (uint16_t i = 0; i < config->buf->width; i++)
        max7219_command(config, addr, data, 0);

    if (latch)
        spi_latch(config->spi);
}

/**
 * Initialize the device
 */
void max7219_init(MAX7219Config *conf) {
    // Perform initialization of each display
//    max7219_command_all(conf, MAX7219_SCAN_LIMIT, 0x7, 1);
//    max7219_command_all(conf, MAX7219_INTENSITY, 0x0, 1);
//    max7219_command_all(conf, MAX7219_DECODE_MODE, 0x0, 1);
//    max7219_command_all(conf, MAX7219_SHUTDOWN, 0, 1);
//    max7219_command_all(conf, MAX7219_SHUTDOWN, 1, 1);
}

/**
 * Set output buffer content
 */
void max7119_set_buf(MAX7219Config *config, uint8_t start, uint8_t *data, uint8_t size) {
    memcpy(config->buf->content + start, data, size);
}

/**
 * Update all displays
 */
void max7219_update(MAX7219Config *config) {
    uint8_t *p = config->buf->content;

    // Each row
    for (uint8_t row = 1; row <= 8; row++) {
        // Each display
        for (uint16_t display_n = 0; display_n < config->buf->width; display_n++)
            max7219_command(config, row, *p++, 0);

        spi_latch(config->spi);
    }
}
