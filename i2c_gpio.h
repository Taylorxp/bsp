/***************************************************************************************************
* FILE: i2c_gpio.h
*
* DESCRIPTION: GPIO¿ÚÄ£ÄâI2C
***************************************************************************************************/
#ifndef __I2C_GPIO_H__
#define __I2C_GPIO_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void GPIOI2C_Init(void);
uint8_t GPIOI2C_ReadOneByte(uint8_t dev, uint16_t addr);
uint8_t GPIOI2C_WriteOneByte(uint8_t dev, uint16_t addr, uint8_t byt);
uint8_t GPIOI2C_WriteOneByteMask(uint8_t dev, uint16_t reg, uint8_t mask, uint8_t dat);
uint8_t GPIOI2C_Read(uint8_t dev, uint8_t addr, uint8_t *buf, uint16_t len);
uint8_t GPIOI2C_Write(uint8_t dev, uint16_t addr, uint8_t *buf, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif
/****************************************** END OF FILE *******************************************/
