#include <stdio.h>
#include <stdbool.h>
#include "spi.h"
#include "max7219.h"
#include "util.h"

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
    for (uint8_t i = 0; i < config->displays_x; i++)
        max7219_command(config, addr, data);

    spi_latch(config->spi);
}

/**
 * Clear all displays
 */
void max7219_clear(MAX7219 *conf)
{
    for (uint8_t k = 1; k <= 8; k++)
    {
        for (uint16_t i = 0; i < conf->displays_x * conf->displays_y; i++)
        {
            max7219_command(conf, k, 0);
        }
        spi_latch(conf->spi);
    }
}

/**
 * Draw a framefuffer on the screen(s)
 */
void max7219_draw(MAX7219 *config, Framebuffer *buf)
{
    uint16_t **p = buf->content;

    // Each row
    for (int16_t row_i = buf->rows; row_i > 0; row_i--)
    {
        // Push a row into each display byte by byte
        for (uint8_t disp_i = 0; disp_i < config->displays_x; disp_i++)
        {
            // Framebuffer stores rows as 16-bit values, but MAX7219 stores rows as 8-bit values.
            // We need to determine word index in a row, dividing current display by 2
            uint16_t word = buf->content[row_i - 1][disp_i / 2];

            uint8_t byte = 0;
            if (disp_i % 2)
                byte = MSB(word);
            else
                byte = LSB(word);

            max7219_command(config, 9 - row_i, byte);
        }

        spi_latch(config->spi);
    }
}
