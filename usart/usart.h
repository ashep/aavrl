void usart_init();
void usart_write(uint8_t byte);
uint8_t usart_read(uint8_t echo);
void usart_read_line(char *buf, uint8_t echo);
void usart_write_line(char *line);
int usart_write_printf(char byte, FILE *stream);
