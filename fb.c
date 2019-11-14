#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "util.h"
#include "fb.h"

/**
 * Initialize framebuffer
 **/
void fb_init(Framebuffer *buf, uint8_t width, uint8_t height) {
    buf->width = width;
    buf->height = height;
    buf->length = width * height;
    buf->content = malloc(buf->length);

    // Fill buf with zeroes
    uint8_t *p = buf->content;
    for (uint16_t i = 0; i < buf->length; i++) {
        *p++ = 0;
    }
}

void fb_mirror(Framebuffer *buf, uint8_t x, uint8_t y, uint8_t rev_bytes) {
    if (!(x || y))
        return;
    
    uint8_t *pos;

    if (x) { // Horizontal
        uint8_t *new_buf = malloc(buf->length);

        for (uint16_t row_n = 1; row_n <= buf->height; row_n++) {
            // Move to the end of the current row
            pos = buf->content + (buf->width * row_n - 1);

            // Copy columns in a reverse order
            for (uint16_t col_n = 0; col_n < buf->width; col_n++) {
                uint8_t c = *pos--;
                *new_buf++ = rev_bytes ? reverse_byte(c) : c;
            }
        }

        free(buf->content);
        buf->content = new_buf - buf->length;
    }
    
    if (y) {  // Vertical
        uint8_t *new_buf = malloc(buf->length);

        // Iterate over rows in a reverse order
        for (uint16_t row_n = buf->height; row_n > 0; row_n--) {
            // Move to the start of the current row
            pos = buf->content + (buf->width * (row_n - 1));

            // Copy columns
            for (uint16_t col_n = 0; col_n < buf->width; col_n++) {
                *new_buf++ = *pos++;
            }
        }

        free(buf->content);
        buf->content = new_buf - buf->length;
    }
}
