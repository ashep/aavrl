#include <avr/io.h>
#include "fb.h"

#define MAX7219_NO_OP 0x0
#define MAX7219_DIG_0 0x1
#define MAX7219_DIG_1 0x2
#define MAX7219_DIG_2 0x3
#define MAX7219_DIG_3 0x4
#define MAX7219_DIG_4 0x5
#define MAX7219_DIG_5 0x6
#define MAX7219_DIG_6 0x7
#define MAX7219_DIG_7 0x8
#define MAX7219_DECODE_MODE 0x9
#define MAX7219_INTENSITY 0xA
#define MAX7219_SCAN_LIMIT 0xB
#define MAX7219_SHUTDOWN 0xC
#define MAX7219_TEST 0xF

typedef struct _MAX7219Config {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    uint8_t cs;
    uint8_t clk;
    uint8_t din;
    uint8_t n_disp;
    uint8_t reversed_h;
    uint8_t reversed_v;
    Framebuffer *buf;
} MAX7219Config;

void max7219_init(MAX7219Config *conf, Framebuffer *buf, volatile uint8_t *ddr, volatile uint8_t *port, uint8_t cs, 
                    uint8_t clk, uint8_t din, uint8_t n_disp, uint8_t reversed_h, uint8_t reversed_v);
void max7219_send_byte(MAX7219Config *config, uint8_t byte);
void max_7219_latch(MAX7219Config *config);
void max7219_flush(MAX7219Config *config);
void max7219_command(MAX7219Config *config, uint8_t addr, uint8_t data);
void max7219_command_all(MAX7219Config *config, uint8_t addr, uint8_t data);
void max7219_clear(MAX7219Config *config);
void max7119_set_buf(MAX7219Config *config, uint8_t start, uint8_t *data, uint8_t size);
void max7219_update(MAX7219Config *config);
