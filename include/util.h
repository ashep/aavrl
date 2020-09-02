#ifndef _UTIL_H
#define _UTIL_H

#include "avr/io.h"

#define BIT_VAL(x, n) (x >> n) & 1U
#define BIT_SET(x, n) x |= 1U << n
#define BIT_CLEAR(x, n) x &= ~(1U << n)
#define BIT_TOGGLE(x, n) x ^= 1U << n
#define BYTE_TOGGLE (x) x = ~x
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
#define LSB(x) (x & 0xff)
#define MSB(x) (x >> 8)

uint8_t reverse_byte(uint8_t b);
void dump_bin(uint16_t v, uint8_t width);

#endif
