#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include "util.h"
#include "fb.h"
#include "max7219.h"

/**
 * Send a single byte
*/
void max7219_send_byte(MAX7219Config *config, uint8_t byte) {
    BIT_CLEAR(*(volatile uint8_t *) config->port, config->din);  

    for (uint8_t i = 8; i > 0; i--) { // The MSB should go first
        BIT_CLEAR(*(volatile uint8_t *) config->port, config->clk); // Prepare to clock
        
        // Set the data bit
        if (BIT_VAL(byte, (i - 1))) 
            BIT_SET(*(volatile uint8_t *) config->port, config->din);
        else
            BIT_CLEAR(*(volatile uint8_t *) config->port, config->din);

        BIT_SET(*(volatile uint8_t *) config->port, config->clk); // Clock
    }
}

/**
 * Latch loaded data into registers
 */
void max_7219_latch(MAX7219Config *config) {
    BIT_SET(*(volatile uint8_t *) config->port, config->cs);
    BIT_CLEAR(*(volatile uint8_t *) config->port, config->cs);
}

 /**
  * Send a command
  */
void max7219_command(MAX7219Config *config, uint8_t addr, uint8_t data) {
    max7219_send_byte(config, addr);
    max7219_send_byte(config, data);
}

 /**
  * Send a command to all displays
  */
void max7219_command_all(MAX7219Config *config, uint8_t addr, uint8_t data) {
    for (uint8_t i = 0; i < config->n_disp; i++) {
        max7219_command(config, addr, data);
    }
    
    max_7219_latch(config);
}

/**
 * Initialize the device
 */
void max7219_init(MAX7219Config *conf, Framebuffer *buf, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t cs, 
                    uint8_t clk, uint8_t din, uint8_t n_disp, uint8_t reversed_h, uint8_t reversed_v) {
    conf->ddr = ddr;
    conf->port = port;
    conf->cs = cs;
    conf->clk = clk;
    conf->din = din;
    conf->n_disp = n_disp;
    conf->reversed_h = reversed_h;
    conf->reversed_v = reversed_v;
    conf->buf = buf;

    // Initializae framebuffer
    fb_init(buf, n_disp, 8);

    // Set data direction
    *(volatile uint8_t *) ddr |= (1 << cs) | 1 << clk | (1 << din);

    // Perform initialization of each display
    max7219_command_all(conf, MAX7219_SCAN_LIMIT, 0x7);
    max7219_command_all(conf, MAX7219_INTENSITY, 0x0);
    max7219_command_all(conf, MAX7219_DECODE_MODE, 0x0);
    max7219_command_all(conf, MAX7219_SHUTDOWN, 0);
    max7219_command_all(conf, MAX7219_SHUTDOWN, 1);
}

/**
 * Clear all displays
 */
void max7219_clear(MAX7219Config *config) {
    uint8_t *pos = config->buf->content;
    
    for (uint8_t i = 0; i < config->buf->length; i++) {
        *pos = 0;
    }
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
        for (uint8_t disp = 0; disp < config->n_disp; disp++) {
            max7219_command(config, row, *p++);
        }

        max_7219_latch(config);
    }
}
