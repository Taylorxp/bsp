/***************************************************************************************************
* FILE: i2c_gpio.c
*
* DESCRIPTION:  GPIO口模拟I2C
***************************************************************************************************/
#include "bsp.h"


/* MCU IO口操作 */
#define _I2C_SCL_1     do { PBout(8) = 1; } while(0)
#define _I2C_SCL_0     do { PBout(8) = 0; } while(0)
#define _I2C_SCL_DAT   PBin(8)
#define _I2C_SCL_IN    do { GPIOB->CRH &= 0xFFFFFFF0;  GPIOB->CRH |= 0x00000004; } while(0)
#define _I2C_SCL_OUT   do { GPIOB->CRH &= 0xFFFFFFF0;  GPIOB->CRH |= 0x00000003; } while(0)

#define _I2C_SDA_1     do { PBout(9) = 1; } while(0)
#define _I2C_SDA_0     do { PBout(9) = 0; } while(0)
#define _I2C_SDA_DAT   PBin(9)
#define _I2C_SDA_IN    do { GPIOB->CRH &= 0xFFFFFF0F;  GPIOB->CRH |= 0x00000040; } while(0)
#define _I2C_SDA_OUT   do { GPIOB->CRH &= 0xFFFFFF0F;  GPIOB->CRH |= 0x00000030; } while(0)

/***************************************************************************************************
* Description:  内部延时
***************************************************************************************************/
static void _I2C_Delay(uint8_t n)
{
    uint8_t i;
    while(n--)
        for(i=0; i<96; i++);
}

/***************************************************************************************************
* Description:  产生起始条件
***************************************************************************************************/
static void _I2C_Start(void)
{
    _I2C_SDA_1;
    _I2C_SCL_1;
    _I2C_Delay(2);
    _I2C_SDA_0;
    _I2C_Delay(2);
    _I2C_SCL_0;
    _I2C_Delay(1);
}

/***************************************************************************************************
* Description:  产生停止条件
***************************************************************************************************/
static void _I2C_Stop(void)
{
    _I2C_SDA_0;
    _I2C_Delay(1);
    _I2C_SCL_1;
    _I2C_Delay(2);
    _I2C_SDA_1;
    _I2C_Delay(2);
}

/***************************************************************************************************
* Description:  检查ack 返回0成功，返回1失败
***************************************************************************************************/
static uint8_t _I2C_WaitAck(void)
{
    uint8_t ack;
    _I2C_SDA_IN;
    _I2C_SDA_1;
    _I2C_Delay(1);
    _I2C_SCL_1;
    _I2C_Delay(2);
    if(_I2C_SDA_DAT)
        ack = 1;
    else 
        ack = 0;
    _I2C_SCL_0;
    _I2C_Delay(1);
    _I2C_SDA_OUT;
    return ack;
}

/***************************************************************************************************
* Description:  发送ACK
***************************************************************************************************/
static void _I2C_SendAck(uint8_t ack)
{
    if(ack) _I2C_SDA_0;
    else _I2C_SDA_1;
    _I2C_Delay(1);
    _I2C_SCL_1;
    _I2C_Delay(2);
    _I2C_SCL_0;
    _I2C_Delay(1);
}

/***************************************************************************************************
* Description:  写一个字节
***************************************************************************************************/
static uint8_t _I2C_WriteByte(uint8_t byt, uint8_t ack)
{
    uint8_t i;
    for(i=0; i<8; i++)
    {
        if(byt & 0x80) _I2C_SDA_1;
        else _I2C_SDA_0;
        _I2C_Delay(1);
        byt <<= 1;
        _I2C_SCL_1;
        _I2C_Delay(2);
        _I2C_SCL_0;
        _I2C_Delay(1);
    }
    ack = _I2C_WaitAck();
    return ack;
}

/***************************************************************************************************
* Description:  读一个字节
***************************************************************************************************/
static uint8_t _I2C_ReadByte(uint8_t ack)
{
    uint8_t i, data;
    data = 0;
    _I2C_SDA_IN;
    for(i=0; i<8; i++)
    {
        _I2C_Delay(1);
        _I2C_SCL_1;
        _I2C_Delay(2);
        data <<= 1;
        if(_I2C_SDA_DAT)
            data |= 0x01;
        _I2C_SCL_0;
        _I2C_Delay(1);
    }
    _I2C_SDA_OUT;
    _I2C_SendAck(ack);
    return data;
}

/***************************************************************************************************
* Description:  读取数据
***************************************************************************************************/
uint8_t GPIOI2C_Read(uint8_t dev, uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    
    _I2C_Start();
    if(_I2C_WriteByte(dev, 1))  //发送写命令
        return 1;        
    if(_I2C_WriteByte(addr, 1)) //发送地址
        return 1;        
	_I2C_Start();
	if(_I2C_WriteByte(dev+1, 1))//进入接收模式
        return 1;
    
    /* 接收数据 */
    len -= 1;
    for(i=0; i<len; i++)
        buf[i] = _I2C_ReadByte(1);
    buf[len] = _I2C_ReadByte(0);
    
    _I2C_Stop();
	return 0;
}

/***************************************************************************************************
* Description: 写数据
***************************************************************************************************/
uint8_t GPIOI2C_Write(uint8_t dev, uint16_t addr, uint8_t *buf, uint8_t len)
{
    uint16_t i;
    
    _I2C_Start();
    if(_I2C_WriteByte(dev, 1))  //发送地址＋写命令
        return 1;        
    if(_I2C_WriteByte(addr, 1)) //发送地址
        return 1;       
    
    for(i=0; i<len; i++)
    {
        if(_I2C_WriteByte(buf[i], 1))   //发送数据
            return 1;    
    }
    
    _I2C_Stop();
    return 0;
}

/***************************************************************************************************
* Description: 在指定地址读出一个字节
***************************************************************************************************/
uint8_t GPIOI2C_ReadOneByte(uint8_t dev, uint16_t addr)
{
    uint8_t dat;
    
    _I2C_Start();
    if(_I2C_WriteByte(dev, 1))  //发送写命令
        return 1;
    if(_I2C_WriteByte(addr, 1)) //发送地址
        return 1;        
	_I2C_Start();
	if(_I2C_WriteByte(dev+1, 1))//进入接收模式
        return 1;        
    dat=_I2C_ReadByte(0);
    _I2C_Stop();
	return dat;
}

/***************************************************************************************************
* Description: 在指定地址写入一个字节
***************************************************************************************************/
uint8_t GPIOI2C_WriteOneByte(uint8_t dev, uint16_t addr, uint8_t byt)
{
    _I2C_Start();
    if(_I2C_WriteByte(dev, 1))  //发送地址＋写命令
        return 1;        
    if(_I2C_WriteByte(addr, 1)) //发送地址
        return 1;      
	if(_I2C_WriteByte(byt, 1))  //发送数据
        return 1;        
    _I2C_Stop();
    return 0;
}

/***************************************************************************************************
* Description:  写一个字节，带屏蔽
***************************************************************************************************/
uint8_t GPIOI2C_WriteOneByteMask(uint8_t dev, uint16_t reg, uint8_t mask, uint8_t dat)
{
    uint8_t value = 0;
    if(mask != 0xFF)
    {
        value = GPIOI2C_ReadOneByte(dev, reg);
    }
    value &= ~mask;
    value |= dat;
    return GPIOI2C_WriteOneByte(dev, reg, value);
}

/***************************************************************************************************
* Description:  GPIO模拟I2C初始化
***************************************************************************************************/
void GPIOI2C_Init(void)
{
    _I2C_SCL_OUT;
    _I2C_SDA_OUT;
    _I2C_SCL_1;
    _I2C_SDA_1;
}

/****************************************** END OF FILE *******************************************/
