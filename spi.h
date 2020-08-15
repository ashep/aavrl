#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

/**
 * SPI configuration
 */
typedef struct SPI
{
    volatile uint8_t *ddr;  // data direction register
    volatile uint8_t *port; // port register
    uint8_t cs;             // CS pin number
    uint8_t clk;            // CLS pin number
    uint8_t din;            // DIN pin number
} SPI;

/**
 * Initialize an SPI configuration
 */
void spi_init(SPI *config);

/**
 * Send a single byte
 */
void spi_send_byte(SPI *config, uint8_t byte);

/**
 * Latch the registers
 */
void spi_latch(SPI *config);

#endif
