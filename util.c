#include <stdio.h>
#include <avr/io.h>
#include "util.h"

/**
 * Reverse a byte
 *
 * https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
 */
uint8_t reverse_byte(uint8_t b)
{
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

   return b;
}

void dump_bin(uint16_t v, uint8_t width)
{
   for (uint8_t i = width; i > 0; i--)
      printf("%u", (v >> (i - 1)) & 1);
}
