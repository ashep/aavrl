#include <stdio.h>
#include <stdbool.h>
#include "spi.h"
#include "max7219.h"
#include "util.h"

/**
  * Send a command to the display
  */
void max7219_command(MAX7219Config *config, uint8_t addr, uint8_t data)
{
    spi_send_byte(config->spi, addr);
    spi_send_byte(config->spi, data);
}

/**
 * Send a command to all displays
 */
void max7219_command_all(MAX7219Config *config, uint8_t addr, uint8_t data)
{
    for (uint8_t i = 0; i < config->n_displays; i++)
    {
        max7219_command(config, addr, data);
    }

    spi_latch(config->spi);
}

/**
 * Initialize the device
 */
void max7219_init(MAX7219Config *conf)
{
    // Perform initialization of each display
    max7219_command_all(conf, MAX7219_SCAN_LIMIT, 0x7);
    max7219_command_all(conf, MAX7219_INTENSITY, 0x0);
    max7219_command_all(conf, MAX7219_DECODE_MODE, 0x0);
    max7219_command_all(conf, MAX7219_SHUTDOWN, 0);
    max7219_command_all(conf, MAX7219_SHUTDOWN, 1);
}

/**
 * Update all displays
 */
void max7219_update(MAX7219Config *config)
{
    uint8_t *p = config->buf->content;
    uint8_t row_start = 1;
    uint8_t row_stop = config->buf->height + 1;
    uint8_t row_inc = 1;
    uint8_t disp_start = 1;
    uint8_t disp_stop = config->n_displays + 1;
    uint8_t disp_inc = 1;

    if (config->reverse_y)
    {
        row_start = config->buf->height;
        row_stop = 0;
        row_inc = -1;
    }

    if (config->reverse_y)
    {
        disp_start = config->n_displays;
        disp_stop = 0;
        disp_inc = -1;
    }

    // Each row
    for (uint8_t row = row_start; row != row_stop; row = row + row_inc)
    {
        // Each display
        for (uint8_t disp = disp_start; disp != disp_stop; disp = disp + disp_inc)
        {
            int byte_pos = (row - 1) * config->n_displays + disp - 1;
            uint8_t byte = config->buf->content[byte_pos];

            if (config->reverse_y)
            {
                byte = reverse_byte(byte);
            }

            max7219_command(config, row, byte);
        }

        spi_latch(config->spi);
    }
}
