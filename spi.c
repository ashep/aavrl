//
// Created by ashep on 11/17/19.
//

#include "util.h"
#include "spi.h"

/**
 * Send a single byte
*/
void spi_send_byte(SPI_Config *config, uint8_t byte) {
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
void spi_latch(SPI_Config *config) {
    BIT_SET(*(volatile uint8_t *) config->port, config->cs);
    BIT_CLEAR(*(volatile uint8_t *) config->port, config->cs);
}