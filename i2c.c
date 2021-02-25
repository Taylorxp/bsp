/***************************************************************************************************
* FILE: i2c.c
*
* DESCRIPTION:  STM32 硬件I2C驱动

***************************************************************************************************/
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "i2c.h"
#include "system.h"

extern void delay_us(uint16_t nus);
#define CHECK_STATUS(func, times) \
do \
{ \
    int32_t count = (times); \
    while(func) \
    { \
        if(--count < 0) \
            return 1; \
        delay_us(10); \
    } \
} while(0) \

enum HI2C_MODE
{
    HI2C_MASTER,
    HI2C_SLAVE
};

enum HI2C_STATUS
{
    HI2C_STATUS_STANDBY,
    HI2C_STATUS_START,
    HI2C_STATUS_ADDRESS,
    HI2C_STATUS_WRITE,
    HI2C_STATUS_DATA,
    HI2C_STATUS_STOP,
    HI2C_STATUS_ERROR,
};


struct
{
    enum HI2C_STATUS status;
    uint8_t mode;
    uint8_t address;
    uint8_t *data_buffer;
    uint8_t data_length;
}HI2C_INFO[2];


/***************************************************************************************************
* Description:  I2C初始化，I2Cx：I2C指针，speed：总线速度，own_addr：从模式时的8位地址
***************************************************************************************************/
int32_t HI2C_Init(I2C_TypeDef* I2Cx, uint32_t speed, uint8_t own_addr)
{
    I2C_InitTypeDef I2C_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    /* 去初始化 */
    I2C_DeInit(I2Cx);
    
    /* 模式配置 */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = speed;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = own_addr;
    I2C_Init(I2Cx, &I2C_InitStructure);
    
	/* 中断配置 */
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;//事件中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;//错误中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

    I2C_ITConfig(I2Cx, I2C_IT_BUF | I2C_IT_EVT, ENABLE);//启用中断
    I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE);//启用中断
    
    return 0;
}

/***************************************************************************************************
* Description:  硬件I2C向指定位置写1个字节数据
***************************************************************************************************/
int32_t HI2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t slave, uint8_t reg, uint8_t dat)
{   
    if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET)
    {
        return -1;
    }
    
    /* 关中断 */
    I2C_ITConfig(I2Cx, I2C_IT_BUF | I2C_IT_EVT, DISABLE);
    I2C_ITConfig(I2Cx, I2C_IT_ERR, DISABLE);
    
    /* 起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);        
    CHECK_STATUS(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT), 10);
    delay_us(50);
    
    /* 发送从机地址，写模式 */
    I2C_Send7bitAddress(I2Cx, slave, I2C_Direction_Transmitter);
    CHECK_STATUS(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 100);
    delay_us(50);
    
    /* 发送从机寄存器地址 */
    I2C_SendData(I2Cx, reg);     
    CHECK_STATUS(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED), 100);
    delay_us(50);
    
    /* 发送数据 */
    I2C_SendData(I2Cx, dat);     
    CHECK_STATUS(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED), 100);
    delay_us(50);
    
    /* 产生停止信号 */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    /* 启用中断 */
    I2C_ITConfig(I2Cx, I2C_IT_BUF | I2C_IT_EVT, ENABLE);
    I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE);
    return 0;
}

/***************************************************************************************************
* Description:  硬件I2C读取指定位置1个字节数据
***************************************************************************************************/
//uint8_t I2C1_ReadOneByte(uint8_t slave, uint8_t offset, uint8_t *dat)
//{
//    //起始条件
//    I2C_GenerateSTART(I2C1, ENABLE);        
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT), 1000);
////    delay_us(10);
//    //发送slave地址
//    I2C_Send7bitAddress(I2C1, slave, I2C_Direction_Transmitter);     //发送地址，主机写
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 1000);
//    //发送片内地址
//    I2C_SendData(I2C1, offset);     
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED), 1000);
//    //重新产生起始条件
//    I2C_GenerateSTART(I2C1, ENABLE);        //I2C 重新开始以读取数据
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT), 1000);
//    //发送从机地址，读模式
//    I2C_Send7bitAddress(I2C1, slave, I2C_Direction_Receiver);     //发送地址，主机读
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED), 1000);
//    //接收数据
//    *dat = 0;
//    I2C_AcknowledgeConfig(I2C1, DISABLE);   //设置接收到数据不产生ACK， 停止接收数据
//    RETRY(!I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE), 1000);
//    *dat = I2C_ReceiveData(I2C1);      
//    //停止条件
//    I2C_GenerateSTOP(I2C1, ENABLE);         //I2C 发送停止信号
//    
//    return 0;
//}

/***************************************************************************************************
* Description:  硬件I2C接收事件中断处理
***************************************************************************************************/
uint16_t i2c_data = 0;
void I2C1_EV_IRQHandler(void)
{
	uint32_t i2c_status;

	static uint8_t i2c_data_index = 0;
	static uint8_t i2c_data_buf[2] = {0};

	i2c_status = I2C_GetLastEvent(I2C1);
	
	switch(i2c_status)
	{
	case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
		break;

	case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
		i2c_data_index = 0;
		break;

	case I2C_EVENT_SLAVE_BYTE_RECEIVED:
		i2c_data = I2C_ReceiveData(I2C1);
		i2c_data_buf[i2c_data_index++] = i2c_data;
		break;

	case I2C_EVENT_SLAVE_STOP_DETECTED:
		I2C1->CR1 |= 0x0001;

		i2c_data_index = 0;
		if(i2c_data_buf[0] == ANGLE_REG)
		{
			MoerRunToB_Angle(i2c_data_buf[1]);
		}
		if(i2c_data_buf[0] == MIC_IN_REG)
		{
			if(i2c_data_buf[1] == MIC_IN_ON)
			{
				MoerMic_ProcessHandler(MIC_IN_ON);
			}
			else if(i2c_data_buf[1] == MIC_IN_OFF)
			{
				MoerMic_ProcessHandler(MIC_IN_OFF);
			}
		}
		break;

	case I2C_EVENT_MASTER_MODE_SELECT:
		break;

	case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
		break;

	case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
		break;

	case I2C_EVENT_MASTER_BYTE_RECEIVED:
		break;

	case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
		break;

	default:

		break;
	}
}

void I2C1_ER_IRQHandler(void)
{
    
     if (I2C_GetITStatus(I2C1, I2C_IT_AF)) 
    { 
        I2C_ClearITPendingBit(I2C1, I2C_IT_AF); 
    } 

    if (I2C_GetITStatus(I2C1, I2C_IT_BERR))
    { 
        I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
    }
    
}

void I2C2_EV_IRQHandler(void)
{
    
}

void I2C2_ER_IRQHandler(void)
{
    
}


/****************************************** END OF FILE *******************************************/




