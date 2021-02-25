/***************************************************************************************************
* FILE: i2c.c
*
* DESCRIPTION:  STM32 Ӳ��I2C����

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
* Description:  I2C��ʼ����I2Cx��I2Cָ�룬speed�������ٶȣ�own_addr����ģʽʱ��8λ��ַ
***************************************************************************************************/
int32_t HI2C_Init(I2C_TypeDef* I2Cx, uint32_t speed, uint8_t own_addr)
{
    I2C_InitTypeDef I2C_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    /* ȥ��ʼ�� */
    I2C_DeInit(I2Cx);
    
    /* ģʽ���� */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = speed;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = own_addr;
    I2C_Init(I2Cx, &I2C_InitStructure);
    
	/* �ж����� */
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;//�¼��ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;//�����ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

    I2C_ITConfig(I2Cx, I2C_IT_BUF | I2C_IT_EVT, ENABLE);//�����ж�
    I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE);//�����ж�
    
    return 0;
}

/***************************************************************************************************
* Description:  Ӳ��I2C��ָ��λ��д1���ֽ�����
***************************************************************************************************/
int32_t HI2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t slave, uint8_t reg, uint8_t dat)
{   
    if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET)
    {
        return -1;
    }
    
    /* ���ж� */
    I2C_ITConfig(I2Cx, I2C_IT_BUF | I2C_IT_EVT, DISABLE);
    I2C_ITConfig(I2Cx, I2C_IT_ERR, DISABLE);
    
    /* ��ʼ�ź� */
    I2C_GenerateSTART(I2Cx, ENABLE);        
    CHECK_STATUS(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT), 10);
    delay_us(50);
    
    /* ���ʹӻ���ַ��дģʽ */
    I2C_Send7bitAddress(I2Cx, slave, I2C_Direction_Transmitter);
    CHECK_STATUS(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 100);
    delay_us(50);
    
    /* ���ʹӻ��Ĵ�����ַ */
    I2C_SendData(I2Cx, reg);     
    CHECK_STATUS(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED), 100);
    delay_us(50);
    
    /* �������� */
    I2C_SendData(I2Cx, dat);     
    CHECK_STATUS(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED), 100);
    delay_us(50);
    
    /* ����ֹͣ�ź� */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    /* �����ж� */
    I2C_ITConfig(I2Cx, I2C_IT_BUF | I2C_IT_EVT, ENABLE);
    I2C_ITConfig(I2Cx, I2C_IT_ERR, ENABLE);
    return 0;
}

/***************************************************************************************************
* Description:  Ӳ��I2C��ȡָ��λ��1���ֽ�����
***************************************************************************************************/
//uint8_t I2C1_ReadOneByte(uint8_t slave, uint8_t offset, uint8_t *dat)
//{
//    //��ʼ����
//    I2C_GenerateSTART(I2C1, ENABLE);        
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT), 1000);
////    delay_us(10);
//    //����slave��ַ
//    I2C_Send7bitAddress(I2C1, slave, I2C_Direction_Transmitter);     //���͵�ַ������д
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 1000);
//    //����Ƭ�ڵ�ַ
//    I2C_SendData(I2C1, offset);     
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED), 1000);
//    //���²�����ʼ����
//    I2C_GenerateSTART(I2C1, ENABLE);        //I2C ���¿�ʼ�Զ�ȡ����
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT), 1000);
//    //���ʹӻ���ַ����ģʽ
//    I2C_Send7bitAddress(I2C1, slave, I2C_Direction_Receiver);     //���͵�ַ��������
//    RETRY(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED), 1000);
//    //��������
//    *dat = 0;
//    I2C_AcknowledgeConfig(I2C1, DISABLE);   //���ý��յ����ݲ�����ACK�� ֹͣ��������
//    RETRY(!I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE), 1000);
//    *dat = I2C_ReceiveData(I2C1);      
//    //ֹͣ����
//    I2C_GenerateSTOP(I2C1, ENABLE);         //I2C ����ֹͣ�ź�
//    
//    return 0;
//}

/***************************************************************************************************
* Description:  Ӳ��I2C�����¼��жϴ���
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




