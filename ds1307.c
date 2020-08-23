#include <stdio.h>
#include <stdbool.h>
#include "twi.h"
#include "ds1307.h"
#include "util.h"

/*
 * Get date and time
*/
uint8_t ds1307_get_time(uint8_t *h, uint8_t *m, uint8_t *s, bool *pm, uint8_t *dow, uint8_t *dom,
                        uint8_t *mo, uint16_t *y)
{
    uint8_t b;

    twi_enable(TWI_PS_64);

    // Set DS1307 start register
    twi_start();
    twi_sla(0x68, TWI_WRITE);
    twi_write_byte(0);

    // Switch to read mode
    twi_start();
    twi_sla(0x68, TWI_READ);

    // 0x0 -- seconds
    ///
    // 7th bit: CH value, should be masked.
    // Bits 6-4: second decimal digit (0-5)
    // Bits 3-0: first decimal digit (0-9)
    if (twi_read_byte(&b, true) != TWI_DATA_RX_ACK)
    {
        twi_disable();
        return DS1307_SEC_ERROR;
    }
    *s = (((b & 0x70) >> 4) * 10) + (b & 0xf);

    // 0x1 -- minutes
    //
    // 7th bit is always 0.
    // Bits 6-4: second decimal digit (0-5).
    // Bits 3-0: first decimal digit (0-9)
    if (twi_read_byte(&b, true) != TWI_DATA_RX_ACK)
    {
        twi_disable();
        return DS1307_MIN_ERROR;
    }
    *m = ((b >> 4) * 10) + (b & ~0xf0);

    // 0x2 -- hours
    //
    // 7th bit is always 0
    // 6th bit: mode: 24H (0) or AM/PM (1)
    // 5th bit: second bit of second decimal digit if mode is 24h, AM/PM flag otherwise
    // 4th bit: first bit of second decimal digit,
    // Bits 3-0: first decimal digit (0-9)
    if (twi_read_byte(&b, true) != TWI_DATA_RX_ACK)
    {
        twi_disable();
        return DS1307_MIN_ERROR;
    }
    else
    {
        if (b >> 6)
        { // AM/PM mode
            *pm = (b >> 5) & 0x1;
            *h = (((b >> 4) & 0x1) * 10) + (b & 0xf);
        }
        else
        { // 24H mode
            *h = ((b >> 4) * 10) + (b & 0xf);
        }
    }

    // 0x3 -- day of the week
    //
    // Bits 7-4: always 0
    // Bits 3-0: day of the week
    if (twi_read_byte(dow, true) != TWI_DATA_RX_ACK)
    {
        twi_disable();
        return DS1307_DOW_ERROR;
    }

    // 0x4 -- day of the month
    //
    // Bits 7-5: always 0
    // Bit 4: second decimal digit (0-3)
    // Bits 3-0: first decimal digit (0-9)
    if (twi_read_byte(&b, true) != TWI_DATA_RX_ACK)
    {
        twi_disable();
        return DS1307_DOM_ERROR;
    }
    *dom = ((b >> 4) * 10) + (b & 0xf);

    // 0x5 -- month
    //
    // Bits 7-5: always 0
    // Bit 4: second decimal digit (0-1)
    // Bits 3-0: first decimal digit (0-9)
    if (twi_read_byte(&b, true) != TWI_DATA_RX_ACK)
    {
        twi_disable();
        return DS1307_MO_ERROR;
    }
    *mo = ((b >> 4) * 10) + (b & ~0xf0);

    // 0x6 -- year
    //
    // Bits 7-4: decades
    // Bits 3-0: years
    if (twi_read_byte(&b, false) != TWI_DATA_RX_NACK)
    {
        twi_disable();
        return DS1307_YR_ERROR;
    }
    *y = 2000 + ((b >> 4) * 10) + (b & 0xf);

    twi_stop();
    twi_disable();

    return DS1307_OK;
}

/**
 * Set date and time
 */
uint8_t ds1307_set_time(uint8_t mode, uint8_t h, uint8_t m, uint8_t s, bool pm, uint8_t dow, uint8_t dom,
                        uint8_t mo, uint16_t y)
{
    twi_enable(TWI_PS_64);

    // Switch to master write mode
    twi_start();
    twi_sla(0x68, TWI_WRITE);
    twi_write_byte(0x0);

    // 0x0 -- second
    if (twi_write_byte((1 << 7) | ((s / 10) << 4) | (s % 10)) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_SEC_ERROR;
    }

    // 0x1 -- minute
    if (twi_write_byte(((m / 10) << 4) | (m % 10)) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_MIN_ERROR;
    }

    // 0x2 -- hour
    uint8_t b = mode;
    if (mode == DS1307_MODE_12H)
        b |= (pm << 5) | ((h / 10) << 4) | (h % 10);
    else
        b |= ((h / 10) << 4) | (h % 10);
    if (twi_write_byte(b) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_HR_ERROR;
    }

    // 0x3 -- day of week
    if (twi_write_byte(dow) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_DOW_ERROR;
    }

    // 0x4 -- day of month
    if (twi_write_byte(((dom / 10) << 4) | (dom % 10)) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_DOM_ERROR;
    }

    // 0x5 -- month
    if (twi_write_byte(((mo / 10) << 4) | (mo % 10)) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_MO_ERROR;
    }

    // 0x6 -- year
    y = y - 2000;
    if (twi_write_byte(((y / 10) << 4) | (y % 10)) != TWI_DATA_TX_ACK)
    {
        twi_disable();
        return DS1307_YR_ERROR;
    }

    twi_stop();
    twi_disable();

    return DS1307_OK;
}