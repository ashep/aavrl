#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
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
 * Free memory allocated by fb_init()
 */
void fb_free(Framebuffer *buf)
{
    // Rows
    for (uint16_t r = 0; r < buf->rows; r++)
        free(buf->content[r]);

    // Pointers to rows
    free(buf->content);
}

/**
 * Print framebuffer's content
 */
void fb_dump(Framebuffer *buf)
{
    for (uint16_t r = 0; r < buf->rows; r++)
    {
        for (uint16_t w = buf->wpr; w > 0; w--)
        {
            if (w == buf->wpr)
                printf("[%4u] ", r);

            dump_bin(buf->content[r][w - 1], 16);

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

    // TODO: implement more effective algorithm
    for (uint16_t kx = 0; kx < src->cols; kx++)
        for (uint16_t ky = 0; ky < src->rows; ky++)
            fb_set_px(dst, x + kx, y + ky, fb_get_px(src, kx, ky));
}

/**
 * Shift content to the left
 */
void fb_shift_h(Framebuffer *buf, uint8_t direction, uint16_t pixels)
{
    if (buf->color_mode == FB_COLOR_MONO)
    {
        uint8_t lbp = sizeof(**buf->content) * 8 - 1; // last bit position

        for (uint16_t r = 0; r < buf->rows; r++)
        {
            bool *bt = calloc(buf->wpr, sizeof(bool)); // bit transfer

            for (uint16_t w = 0; w < buf->wpr; w++)
            {
                // If last bit is set and it's not the last word in a row,
                // that bit must be transfered to the next word
                if (BIT_VAL(buf->content[r][w], lbp) && w + 1 < buf->wpr)
                    bt[w + 1] = true;
                else
                    bt[w + 1] = false;

                buf->content[r][w] = buf->content[r][w] << pixels;

                // Transfer last bit from previous word
                if (bt[w] == true)
                {
                    BIT_SET(buf->content[r][w], 0);
                    bt[w] = false;
                }
            }

            free(bt);
        }
    }
}

/**
 * Set pixel's value for FB_COLOR_MONO color mode
 */
void _set_px_mono(Framebuffer *buf, uint16_t x, uint16_t y, uint16_t color)
{
    uint16_t word = buf->wpr - 1 - x / buf->ppw;
    uint16_t bit = 15 - x % 16;

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
    uint16_t word = buf->wpr - 1 - x / buf->ppw;
    uint16_t bit = 15 - x % 16;

    return (uint16_t)BIT_VAL(buf->content[y][word], bit);
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

/**
 * Place a character into the buffer
 *
 * TODO: work with 16-bit wide fonts
 * 
 * 1. Font starts from fisrt prinatble char with code 32.
 * 2. Each character represented by sequence of (font->height + 1) bytes/words.
 * 3. First byte/word of the sequence contains character width.
 * 4. Other font->height bytes/words contain rows of the character.
 *
 * Returns width of the printed character
 */
uint8_t fb_putc(Framebuffer *buf, uint16_t x, uint16_t y, Font *font, uint8_t ch)
{
    // If character code is beyond the size of the font
    
    if (ch - font->ascii_offset + 1 > font->size) {
        printf("ch: %u, fsize: %u\n", ch - font->ascii_offset, font->size);
        return 0;
    }
        

    // Offset from the beginning of the font content.
    // ASCII-fonts starts from fisrt prinatble char with code 32.
    uint16_t offset = (ch - font->ascii_offset) * (font->height + 1);

    // First byte/word is character width
    uint8_t ch_width;
    if (font->width == FONT_WIDTH_8)
    {
        ch_width = pgm_read_byte(&font->content[offset]);
    }
    else
    {
        // TODO
    }

    // Framebuffer to store character's content into it
    Framebuffer ch_buf;
    fb_init(&ch_buf, ch_width, font->height, buf->color_mode);

    // Place each row of character into buffer
    uint16_t row;
    for (uint8_t i = 0; i < font->height; i++)
    {
        if (font->width == FONT_WIDTH_8)
        {
            // Recall that row is 16-bit wide and shift content of the 8-bit value
            row = pgm_read_byte(&font->content[1 + offset + i]) << 8;
        }
        else
        {
            // TODO
        }

        if (buf->color_mode == FB_COLOR_MONO)
        {
            ch_buf.content[i][0] = row;
        }
        else
        {
            // TODO
        }
    }

    // Place character content into target buffer
    fb_merge(buf, &ch_buf, x, y);

    // Free character buffer
    fb_free(&ch_buf);

    return x + ch_width + font->spacing;
}

uint16_t fb_puts(Framebuffer *buf, uint16_t x, uint16_t y, Font *font, char *s)
{
    uint16_t pos = x;

    while (*s)
        pos = fb_putc(buf, pos, y, font, *s++);

    return pos;
}
