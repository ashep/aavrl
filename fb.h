#ifndef _FB_FB_H
#define _FB_FB_H

#include <avr/io.h>

typedef struct _Framebuffer {
    uint16_t width;
    uint16_t height;
    uint32_t length;
    uint8_t *content;
} Framebuffer;

Framebuffer *fb_init(Framebuffer *b);
void fb_mirror(Framebuffer *buf, uint8_t x, uint8_t y, uint8_t rev_bytes);

#endif
