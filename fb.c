#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "fb.h"

/**
 * Initialize framebuffer
 **/
void fb_init(Framebuffer *buf, uint16_t cols, uint16_t rows, uint8_t color_mode)
{
    buf->cols = cols;
    buf->rows = rows;
    buf->color_mode = color_mode;
    buf->content = calloc(rows, sizeof(*buf->content));

    if (color_mode == FB_COLOR_MONO)
        buf->ppw = 16;
    else
        buf->ppw = 1;

    buf->wpr = cols / buf->ppw;

    if (buf->wpr == 0)
        buf->wpr = 1;

    for (uint16_t r = 0; r < rows; r++)
        buf->content[r] = calloc(buf->wpr, sizeof(uint16_t));
}

/**
 * Print framebuffer's content
 */
void fb_print(Framebuffer *buf)
{
    for (uint16_t r = 0; r < buf->rows; r++)
    {
        for (uint16_t w = buf->wpr; w > 0; w--)
        {
            if (w == buf->wpr)
                printf("[%4u] ", r);

            dump_bin(16, buf->content[r][w - 1]);

            if (w == 1)
            {
                printf(" ");
                for (uint16_t w2 = buf->wpr; w2 > 0; w2--)
                {
                    printf("%u", buf->content[r][w2 - 1]);
                    if (w2 == 1)
                        printf("\n");
                    else
                        printf("|");
                }
            }
            else
                printf("|");
        }
    }
}

/**
 * Fill the framebuffer with zeroes
 */
void fb_clear(Framebuffer *buf)
{
    for (uint16_t r = 0; r < buf->rows; r++)
        for (uint16_t w = 0; w < buf->wpr; w++)
            buf->content[r][w] = 0;
}

/**
 * Set framebuffer's content
 * 
 * The length of content must equal buf->rows * buf->cols
 */
void fb_set(Framebuffer *buf, uint16_t *data, uint16_t len)
{
    for (uint16_t i = 0, n = 0; i < buf->rows && n < len; i++)
    {
        uint16_t *word = buf->content[i];
        for (uint16_t cn = 0; cn < buf->wpr && n < len; cn++, n++)
        {
            *word++ = *data++;
        }
    }
}

/**
 * Merge two framebuffers
 */
void fb_merge(Framebuffer *dst, Framebuffer *src, uint16_t x, uint16_t y)
{
    if (x >= dst->cols || y >= dst->rows)
        return;

    for (uint16_t kx = 0; kx < src->cols; kx++)
        for (uint16_t ky = 0; ky < src->rows; ky++)
            fb_set_px(dst, x + kx, y + ky, fb_get_px(src, kx, ky));
}

/**
 * Set pixel's value for FB_COLOR_MONO color mode
 */
void _set_px_mono(Framebuffer *buf, uint16_t x, uint16_t y, uint16_t color)
{
    uint16_t word = x / buf->ppw;
    uint16_t bit = x % 16;

    if (color > 0)
        BIT_SET(buf->content[y][word], bit);
    else
        BIT_CLEAR(buf->content[y][word], bit);
}

/**
 * Set pixel's value
 */
void fb_set_px(Framebuffer *buf, uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= buf->cols || y >= buf->rows)
        return;

    if (buf->color_mode == FB_COLOR_MONO)
        _set_px_mono(buf, x, y, color);
    else if (buf->color_mode == FB_COLOR_RGB565)
        buf->content[y][x] = color;
}

/**
 * Get pixel's value for FB_COLOR_MONO color mode
 */
uint16_t _get_px_mono(Framebuffer *buf, uint16_t x, uint16_t y)
{
    return (uint16_t)BIT_VAL(buf->content[y][x / buf->ppw], x);
}

/**
 * Get pixel's value
 */
uint16_t fb_get_px(Framebuffer *buf, uint16_t x, uint16_t y)
{
    if (buf->color_mode == FB_COLOR_MONO)
        return _get_px_mono(buf, x, y);
    else if (buf->color_mode == FB_COLOR_RGB565)
        return buf->content[y][x];
}

/**
 * Draw a rectangle
 */
void fb_rect(Framebuffer *buf, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, bool fill)
{
    // Top line
    for (uint16_t x = x1; x <= x2; x++)
        fb_set_px(buf, x, y1, color);

    // Bottom line
    for (uint16_t x = x1; x <= x2; x++)
        fb_set_px(buf, x, y2, color);

    // Left line
    for (uint16_t y = y1; y <= y2; y++)
        fb_set_px(buf, x1, y, color);

    // Right line
    for (uint16_t y = y1; y <= y2; y++)
        fb_set_px(buf, x2, y, color);

    // Fill
    if (fill)
    {
        for (uint16_t x = x1 + 1; x < x2; x++)
            for (uint16_t y = y1 + 1; y < y2; y++)
                fb_set_px(buf, x, y, color);
    }
}
