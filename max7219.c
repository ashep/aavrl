#include <stdio.h>
#include <stdbool.h>
#include "spi.h"
#include "max7219.h"
#include "util.h"

/**
  * Send a command to a display
  */
void max7219_command(MAX7219 *config, uint8_t addr, uint8_t data)
{
    spi_send_byte(config->spi, addr);
    spi_send_byte(config->spi, data);
}

/**
 * Send a command to all displays
 */
void max7219_command_all(MAX7219 *config, uint8_t addr, uint8_t data)
{
    for (uint8_t i = 0; i < config->n_displays; i++)
        max7219_command(config, addr, data);

    spi_latch(config->spi);
}

/**
 * Initialize the device
 */
void max7219_init(MAX7219 *conf)
{
    // Perform initialization of each display
    max7219_command_all(conf, MAX7219_SCAN_LIMIT, 0x7);
    max7219_command_all(conf, MAX7219_INTENSITY, 0x0);
    max7219_command_all(conf, MAX7219_DECODE_MODE, 0x0);
    max7219_command_all(conf, MAX7219_SHUTDOWN, 0);
    max7219_command_all(conf, MAX7219_SHUTDOWN, 1);
}

/**
 * Draw a framefuffer on the screen(s)
 */
void max7219_draw(MAX7219 *config, Framebuffer *buf)
{
    uint16_t **p = buf->content;
    int8_t row_start = 1;
    int8_t row_inc = 1;
    int8_t disp_start = 1;
    int8_t disp_stop = config->n_displays + 1;
    int8_t disp_inc = 1;

    if (config->reverse_y)
    {
        row_start = buf->rows;
        row_inc = -1;
    }

    if (config->reverse_x)
    {
        disp_start = config->n_displays;
        disp_stop = 0;
        disp_inc = -1;
    }

    // Each row
    for (int16_t row = row_start, i = 1; i <= buf->rows; row = row + row_inc, i = i + 1)
    {
        // Each display
        for (uint8_t disp = disp_start; disp != disp_stop; disp = disp + disp_inc)
        {
            uint16_t word_pos = (disp - 1) / sizeof(**buf->content);
            uint16_t word = buf->content[row - 1][word_pos];

            uint8_t byte = 0;
            if ((disp - 1) % 2 == 0)
                byte = LSB(word);
            else
                byte = MSB(word);

            if (config->reverse_x)
                byte = reverse_byte(byte);

            max7219_command(config, i, byte);
        }

        spi_latch(config->spi);
    }
}
