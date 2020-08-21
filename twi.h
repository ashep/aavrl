#ifndef TWI_H
#define TWI_H

#include <stdio.h>
#include <stdbool.h>

#define TWI_PS_NO 0 // no frequency division
#define TWI_PS_4 1  // divide by 4
#define TWI_PS_16 2 // divide by 16
#define TWI_PS_64 3 // divide by 64

#define TWI_WRITE 0
#define TWI_READ 1

#define TWI_START 0x8         // START condition has been transmitted
#define TWI_RSTART 0x10       // Repeated START condition has been transmitted
#define TWI_SLA_W_ACK 0x18    // SLA+W has been transmitted; ACK has been received
#define TWI_SLA_W_NACK 0x20   // SLA+W has been transmitted; NACK has been received
#define TWI_DATA_TX_ACK 0x28  // Data byte has been transmitted; ACK has been received
#define TWI_DATA_TX_NACK 0x30 // Data byte has been transmitted; NACK has been received
#define TWI_ARB_LOST 0x38     // Arbitration lost in SLA+RW or data bytes
#define TWI_SLA_R_ACK 0x40    // SLA+R has been transmitted; ACK has been received
#define TWI_SLA_R_NACK 0x48   // SLA+R has been transmitted; NACK has been received
#define TWI_DATA_RX_ACK 0x50  // Data byte has been received; ACK has been returned
#define TWI_DATA_RX_NACK 0x58 // Data byte has been received; NACK has been returned
#define TWI_STOP 0xf8         // STOP condition has been transmitted

typedef struct TWI
{
    uint8_t ps; // prescaler factor
} TWI;

uint8_t twi_enable(uint8_t ps);
uint8_t twi_disable();
uint8_t twi_start();
uint8_t twi_stop();
uint8_t twi_sla(uint8_t addr, uint8_t rw);
uint8_t twi_write_byte(uint8_t byte);
uint8_t twi_read_byte(uint8_t *byte, bool is_last_byte);

#endif