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
    for (int16_t row_i = 1; row_i <= buf->rows; row_i++)
    {
        uint8_t disp_cnt = 0;

        for (uint8_t word_n = buf->wpr + 1 - config->displays_x / 2; word_n <= buf->wpr; word_n++)
        {
            // 2 bytes per word
            for (uint8_t byte_cnt = 0; byte_cnt < 2 && disp_cnt < config->displays_x; byte_cnt++)
            {
                uint16_t word = buf->content[row_i - 1][word_n - 1];

                uint8_t byte = 0;
                if (byte_cnt % 2)
                    byte = MSB(word);
                else
                    byte = LSB(word);

                max7219_command(config, row_i, byte);
                disp_cnt++;
            }
        }

        spi_latch(config->spi);
    }
}
