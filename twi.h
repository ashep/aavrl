#ifndef TWI_H
#define TWI_H

#define TWI_PS_NO 0 // no frequency division
#define TWI_PS_4 1  // divide by 4
#define TWI_PS_16 2 // divide by 16
#define TWI_PS_64 3 // divide by 64

#define TWI_WRITE 0
#define TWI_READ 1

#define TWI_START_OK 0x8         // a START condition has been transmitted
#define TWI_RSTART_OK 0x10       // a repeated START condition has been transmitted
#define TWI_SLA_OK 0x18          // SLA+RW has been transmitted; ACK has been received
#define TWI_SLA_NOT_ACK 0x20     // SLA+RW has been transmitted; NOT ACK has been received
#define TWI_DATA_TX_OK 0x28      // Data byte has been transmitted; ACK has been received
#define TWI_DATA_TX_NOT_ACK 0x30 // Data byte has been transmitted; NOT ACK has been received
#define TWI_ARB_LOST 0x38        // Arbitration lost in SLA+RW or data bytes
#define TWI_STOP_OK 0xf8         // a STOP condition has been transmitted

typedef struct TWI
{
    uint8_t ps; // prescaler factor
} TWI;

uint8_t twi_enable(uint8_t ps);
uint8_t twi_disable();
uint8_t twi_start();
uint8_t twi_stop();
uint8_t twi_sla(uint8_t addr, bool rw);
uint8_t twi_write_byte(uint8_t byte);

#endif