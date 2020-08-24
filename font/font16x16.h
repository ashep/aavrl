#ifndef FONT16x16
#define FONT16x16

#include <stdbool.h>
#include <avr/pgmspace.h>
#include "font.h"

const uint8_t _font16x16[] PROGMEM = {0x00};

// TODO
Font font16x16 = {
    .ascii_offset = 32,
    .size = 0,
    .width = FONT_WIDTH_16,
    .content = _font16x16,
    .height = 16,
    .spacing = 2,
};

#endif