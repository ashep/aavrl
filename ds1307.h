#ifndef DS1307_H
#define DS1307_H

#include <stdio.h>
#include <stdbool.h>

#define DS1307_OK 0x0
#define DS1307_READ_ERROR 0x1
#define DS1307_SEC_ERROR 0x2
#define DS1307_MIN_ERROR 0x3
#define DS1307_HR_ERROR 0x4
#define DS1307_DOW_ERROR 0x5
#define DS1307_DOM_ERROR 0x6
#define DS1307_MO_ERROR 0x7
#define DS1307_YR_ERROR 0x8

#define DS1307_MODE_24H 0x00
#define DS1307_MODE_12H 0x40

uint8_t ds1307_get_time(uint8_t *h, uint8_t *m, uint8_t *s, bool *pm, uint8_t *dow, uint8_t *dom,
                        uint8_t *mo, uint16_t *y);
uint8_t ds1307_set_time(uint8_t mode, uint8_t h, uint8_t m, uint8_t s, bool pm, uint8_t dow, uint8_t dom,
                        uint8_t mo, uint16_t y);

#endif