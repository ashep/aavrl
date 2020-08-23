#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include "twi.h"
#include "util.h"

// TWCR -- TWI Control Register
// TWSR -- TWI Status Register
// TWDR -- TWI Data Register

/**
 * Enable TWI hardware
 */
uint8_t twi_enable(uint8_t ps)
{
    TWCR = 1 << TWEN;
    TWBR = ps;
    TWSR |= ps;
}

/**
 * Disable TWI hardware
 */
uint8_t twi_disable()
{
    TWCR &= ~(1 << TWEN);
}

/**
 * Send START condition
 */
uint8_t twi_start()
{
    // Setup START condition
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);

    // Wait while the operation ends
    while (!(TWCR & (1 << TWINT)))
        ;

    // Status register with prescaler bits masked
    return TWSR & ~3;
}

/**
 * Send STOP condition
 */
uint8_t twi_stop()
{
    // Setup STOP condition
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);

    // Status register with prescaler bits masked
    return TWSR & ~3;
}

/**
 * Send address packet
 * 
 * rw == 0 is master WRITE mode
 * rw == 1 is master READ mode
 */
uint8_t twi_sla(uint8_t addr, uint8_t rw)
{
    // Load 7-bit address + R/W bit to the TWDR
    TWDR = (addr << 1) | rw;

    // Initiate the transmission
    TWCR = (1 << TWEN) | (1 << TWINT);

    // Wait while the operation ends
    while (!(TWCR & (1 << TWINT)))
        ;

    // Status register with prescaler bits masked
    return TWSR & ~3;
}

uint8_t twi_write_byte(uint8_t byte)
{
    // Prepare data for transmission
    TWDR = byte;

    // Initiate the transmission
    TWCR = (1 << TWEN) | (1 << TWINT);

    // Wait while the operation ends
    while (!(TWCR & (1 << TWINT)))
        ;

    // Status register with prescaler bits masked
    return TWSR & ~3;
}

uint8_t twi_read_byte(uint8_t *byte, bool send_ack)
{
    // Initiate the transmission
    if (send_ack)
        TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        TWCR = (1 << TWEN) | (1 << TWINT);

    // Wait while current operation ends
    while (!(TWCR & (1 << TWINT)))
        ;

    // Read a byte
    *byte = TWDR;

    // Status register with prescaler bits masked
    return TWSR & ~3;
}