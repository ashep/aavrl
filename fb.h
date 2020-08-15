#ifndef _FB_FB_H
#define _FB_FB_H

#include <avr/io.h>

#define FB_COLOR_MONO 0
#define FB_COLOR_RGB565 1

typedef struct Framebuffer
{
    uint8_t color_mode;
    uint8_t ppw; // pixels per 2-byte word
    uint8_t wpr; // 2-byte words words per row
    uint16_t cols;
    uint16_t rows;
    uint16_t **content;
} Framebuffer;

void fb_init(Framebuffer *buf, uint16_t cols, uint16_t rows, uint8_t color_mode);
void fb_print(Framebuffer *buf);
void fb_clear(Framebuffer *buf);
void fb_set(Framebuffer *buf, uint16_t *content, uint16_t len);
void fb_merge(Framebuffer *dst, Framebuffer *src, uint16_t x, uint16_t y);
void fb_set_px(Framebuffer *buf, uint16_t x, uint16_t y, uint16_t color);
uint16_t fb_get_px(Framebuffer *buf, uint16_t x, uint16_t y);
void fb_rect(Framebuffer *buf, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool fill);

#endif
