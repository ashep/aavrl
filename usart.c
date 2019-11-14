#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef F_CPU
    #define F_CPU 8000000
#endif

#ifndef BAUD
    #define BAUD 38400
#endif

#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include "usart.h"

void usart_init() {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #else
        UCSR0A &= ~(1 << U2X0);
    #endif

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    static FILE stdo = FDEV_SETUP_STREAM(usart_write_printf, NULL, _FDEV_SETUP_WRITE);
    stdout = &stdo;
}

void usart_write(uint8_t byte) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Put data into buffer, which sends the data
    UDR0 = byte;
}

uint8_t usart_read(uint8_t echo) {
    while (!(UCSR0A & (1 << RXC0)));
    
    uint8_t ret = UDR0;

    if (echo)
        usart_write(ret);

    return ret;
}

void usart_read_line(char *buf, uint8_t echo) {
    uint8_t buf_cnt = 0;
    char byte;
    
    while(buf_cnt < 256) {
        byte = usart_read(0);

        if (byte != '\r' && byte != '\n') {
            buf[buf_cnt] = byte;
            ++buf_cnt;
        }

        if (byte == '\n')
            break;
        
        if (echo)
            usart_write(byte);
    }

    buf[buf_cnt] = 0x0;
}

void usart_write_line(char *line) {
    char *c = line;
    
    while (*c) {
        usart_write(*c);
        ++c;
    }

    usart_write('\r');
    usart_write('\n');
}

int usart_write_printf(char byte, FILE *stream) {
    if (byte == '\n')
        usart_write('\r');
    
    usart_write(byte);

    return 0;
}