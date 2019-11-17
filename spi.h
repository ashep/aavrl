#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

typedef struct _SPI_Config {
    volatile uint8_t *port;
    uint8_t cs;
    uint8_t clk;
    uint8_t din;
} SPI_Config;

void spi_send_byte(SPI_Config *config, uint8_t byte);

void spi_latch(SPI_Config *config);

#endif //_SPI_H
