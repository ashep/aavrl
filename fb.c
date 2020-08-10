#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "fb.h"

/**
 * Initialize framebuffer
 **/
void fb_init(Framebuffer *buf, uint16_t width, uint16_t height, uint8_t color_mode)
{
    buf->width = width;
    buf->height = height;
    buf->color_mode = color_mode;

    if (color_mode == FB_COLOR_MONO)
    {
        // 8 pixels per byte
        buf->length = buf->width / 8 * buf->height;
    }
    else if (color_mode == FB_COLOR_RGB565)
    {
        // 2 bytes per pixel
        buf->length = buf->width * 2 * buf->height;
    }

    buf->content = malloc(buf->length * sizeof(buf->content));

    fb_clear(buf);
}

void fb_clear(Framebuffer *buf) {
    uint8_t *p = buf->content;

    // Fill buf with zeroes
    for (uint16_t i = 0; i < buf->length; i++)
    {
        *p++ = 0;
    }
}

void fb_pixel(Framebuffer *buf, uint16_t x, uint16_t y, uint16_t color)
{
    if (x > buf->width || y > buf->height)
    {
        return;
    }

    if (buf->color_mode == FB_COLOR_MONO)
    {
        // Absolute dot position
        int byte_pos = (buf->width * y + x) / 8;
        int bit_pos = x % 8;

        if (color)
        {
            BIT_SET(buf->content[byte_pos], bit_pos);
        }
        else
        {
            BIT_CLEAR(buf->content[byte_pos], bit_pos);
        }
    }
    else if (buf->color_mode == FB_COLOR_RGB565)
    {
        /* TODO */
    }
}

void fb_rect(Framebuffer *buf, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool fill)
{
    // Top line
    for (uint16_t x = x1; x <= x2; x++)
    {
        fb_pixel(buf, x, y1, color);
    }

    // Bottom line
    for (uint16_t x = x1; x <= x2; x++)
    {
        fb_pixel(buf, x, y2, color);
    }

    // Left line
    for (uint16_t y = y1; y <= y2; y++)
    {
        fb_pixel(buf, x1, y, color);
    }

    // Right line
    for (uint16_t y = y1; y <= y2; y++)
    {
        fb_pixel(buf, x2, y, color);
    }

    // Fill
    if (fill)
    {
        for (uint16_t x = x1 + 1; x < x2; x++)
        {
            for (uint16_t y = y1 + 1; y < y2; y++)
            {
                fb_pixel(buf, x, y, color);
            }
        }
    }
}