#ifndef _FB_FB_H
#define _FB_FB_H

#include <avr/io.h>

#define FB_COLOR_MONO 0
#define FB_COLOR_RGB565 1

typedef struct Framebuffer
{
    uint8_t color_mode;
    uint16_t width;
    uint16_t height;
    uint16_t length;
    uint8_t *content;
} Framebuffer;

void fb_init(Framebuffer *buf, uint16_t width, uint16_t height, uint8_t color_mode);
void fb_clear(Framebuffer *buf);
void fb_pixel(Framebuffer *buf, uint16_t x, uint16_t y, uint16_t color);
void fb_rect(Framebuffer *buf, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool fill);

#endif
