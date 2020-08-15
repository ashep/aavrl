#include "util.h"
#include "spi.h"

/**
 * Initialize an SPI configuration
 */
void spi_init(SPI *config)
{
    // Setup data direction register for output
    *config->ddr |= 1 << config->cs | 1 << config->clk | 1 << config->din;
}

/**
 * Send a single byte
 */
void spi_send_byte(SPI *config, uint8_t byte)
{
    BIT_CLEAR(*config->port, config->din);

    for (uint8_t i = 8; i > 0; i--) // The MSB should go first
    {
        BIT_CLEAR(*config->port, config->clk); // Prepare to clock

        // Set the data bit
        if (BIT_VAL(byte, (i - 1)))
            BIT_SET(*config->port, config->din);
        else
            BIT_CLEAR(*config->port, config->din);

        BIT_SET(*config->port, config->clk); // Clock
    }
}

/**
 * Latch the registers
 */
void spi_latch(SPI *config)
{
    BIT_SET(*config->port, config->cs);
    BIT_CLEAR(*config->port, config->cs);
}
