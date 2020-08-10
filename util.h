#ifndef _UTIL_H
#define _UTIL_H

#include "avr/io.h"

#define BIT_VAL(data, y) (data >> y) & 1U
#define BIT_SET(data, y) data |= 1U << y
#define BIT_CLEAR(data, y) data &= ~(1U << y)
#define BIT_TOGGLE(data, y) data ^= 1U << y
#define BYTE_TOGGLE (data) data = ~data
#define ARRAY_LEN(x)  (sizeof(x) / sizeof((x)[0]))

uint8_t reverse_byte(uint8_t b);

#endif
