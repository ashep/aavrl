#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdbool.h>

#define FONT_WIDTH_8 8
#define FONT_WIDTH_16 16

typedef struct Font
{
    uint8_t ascii_offset;   // char code offset relative to ASCII table
    uint8_t size;           // font array size
    uint8_t width;          // number of bits per row
    uint8_t height;         // number of bytes per row
    uint8_t spacing;        // spce between characters in pixels
    const uint8_t *content; // pointer to content in program space
} Font;

#endif