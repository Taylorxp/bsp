/***************************************************************************************************
* FILE: spi.c

* DESCRIPTION:  SPI��������

***************************************************************************************************/
#include "spi.h"

/***************************************************************************************************
* Description:  SPI1��ʼ��
***************************************************************************************************/
void SPI1_Init(void)
{
    SPI_InitTypeDef SPI_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    SPI_I2S_DeInit(SPI1);
    
     /* ˫�� */
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    /* ��ģʽ */
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    /* Ƭѡ������� */
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    /* ʱ�� 96M/2=48MHz */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2 ;
    /* ��λ��ǰ */
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    
    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);
}

/***************************************************************************************************
* Description:  SPI1��дһ���ֽ�
***************************************************************************************************/
uint8_t SPI1_WriteReadByte(uint8_t arg)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, arg);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

/****************************************** END OF FILE *******************************************/
