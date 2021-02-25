/***************************************************************************************************
* FILE: i2c.h
*
* DESCRIPTION:  MCU I2CÇý¶¯
***************************************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include "stdint.h"
#include "stm32f10x_i2c.h"
#include "bsp.h"
#include "moer.h"


int32_t HI2C_Init(I2C_TypeDef* I2Cx, uint32_t speed, uint8_t own_addr);
int32_t HI2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t slave, uint8_t reg, uint8_t dat);
uint8_t I2C1_ReadOneByte(uint8_t slave, uint8_t offset, uint8_t *dat);


#endif
/****************************************** END OF FILE *******************************************/
