#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

/**
 * SPI configuration
 */
typedef struct SPI_Config
{
    volatile uint8_t *ddr;  // data direction register
    volatile uint8_t *port; //
    uint8_t cs;             // CS pin number
    uint8_t clk;            // CLS pin number
    uint8_t din;            // DIN pin number
} SPI_Config;

void spi_init(SPI_Config *config);
void spi_send_byte(SPI_Config *config, uint8_t byte);
void spi_latch(SPI_Config *config);

#endif //_SPI_H
