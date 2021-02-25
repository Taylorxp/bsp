/***************************************************************************************************
* FILE: spi.h

* DESCRIPTION:
  --

****************************************************************************************************
* MODIFY:  --

* DATE: --

* DESCRIPTION: --

***************************************************************************************************/
#ifndef SPI_H
#define SPI_H
#include "bsp.h"
/***************************************************************************************************
* DEFINES
***************************************************************************************************/

/***************************************************************************************************
* VARIABLES DECLARATION
***************************************************************************************************/

/***************************************************************************************************
* FUNCTION DECLARATION
***************************************************************************************************/
void SPI1_Init(void);
void SPI2_Init(void);
void SPI3_Init(void);

uint8_t SPI1_WriteReadByte(uint8_t arg);
uint8_t SPI2_WriteReadByte(uint8_t arg);
uint8_t SPI3_WriteReadByte(uint8_t arg);

#endif
/****************************************** END OF FILE *******************************************/
