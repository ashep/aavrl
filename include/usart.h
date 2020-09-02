#ifndef USART_H
#define USART_H

#include <stdbool.h>

void usart_init(bool setup_stdout);
void usart_write(uint8_t byte);
uint8_t usart_read(uint8_t echo);
void usart_read_line(char *buf, uint8_t echo);
void usart_write_line(char *line);
int usart_stdout_writer(char byte, FILE *stream);

#endif
