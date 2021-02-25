/***************************************************************************************************
* FILE: spi.c

* DESCRIPTION:  SPI基本操作

***************************************************************************************************/
#include "spi.h"

/***************************************************************************************************
* Description:  SPI1初始化
***************************************************************************************************/
void SPI1_Init(void)
{
    SPI_InitTypeDef SPI_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    SPI_I2S_DeInit(SPI1);
    
     /* 双向 */
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    /* 主模式 */
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    /* 片选软件控制 */
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    /* 时钟 96M/2=48MHz */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2 ;
    /* 低位在前 */
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    
    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);
}

/***************************************************************************************************
* Description:  SPI1读写一个字节
***************************************************************************************************/
uint8_t SPI1_WriteReadByte(uint8_t arg)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, arg);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

/****************************************** END OF FILE *******************************************/
