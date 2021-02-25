/***************************************************************************************************
* FILE: uart_gpio.c
*
* DESCRIPTION:  GPIO口模拟UART
*
* CREATED: 2020/3/14, by taylor
***************************************************************************************************/
#include "uart_gpio.h"
#include "moer.h"
#include "system.h"

#if defined(MOER_GPIO_UART)
/***************************************************************************************************
* Description:  模拟串口GPIO TX初始化
***************************************************************************************************/
static void UART_GPIO_TX_Init(void)
{
	UART_TX_HIGH();
	
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
    TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseInitStructure.TIM_Prescaler = 95; //1MHz
    TIM_BaseInitStructure.TIM_Period = 100-1;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_BaseInitStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************************
* Description:  模拟串口GPIO RX初始化
***************************************************************************************************/
static void UART_GPIO_RX_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 定时器 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseInitStructure.TIM_Prescaler = 95; //1MHz
    TIM_BaseInitStructure.TIM_Period = 100-1;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 数据脚 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************************
* Description:  模拟串口GPIO发送数据
***************************************************************************************************/
void UART_GPIO_TX_Data(uint8_t cmd, uint8_t data)
{
	uint16_t ucTemp = (cmd << 8) | data;
	static int8_t tx_count = uart_tx_start_bit;
	
	switch(tx_count)
	{
	case uart_tx_start_bit:
		UART_TX_LOW();
		break;
	case uart_tx_bit0:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit1:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit2:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit3:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit4:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit5:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit6:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit7:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit8:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit9:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit10:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit11:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit12:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit13:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit14:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_bit15:
		if((ucTemp >> tx_count) & 0x0001)
			UART_TX_HIGH();
		else
			UART_TX_LOW();
		break;
	case uart_tx_stop_bit:
		UART_TX_HIGH();
		break;
	}
	
	if(tx_count++ > uart_tx_stop_bit)
	{
		//数据发送完后，将总线拉高
		UART_TX_HIGH();
		MOER_TX_TIMER_OFF();
		tx_count = uart_tx_start_bit;
		ucTemp = 0;
		stUartGpioTx.tx_cmd = 0;
		stUartGpioTx.tx_data = 0;
	}
}

/***************************************************************************************************
* Description:  模拟串口GPIO接收数据
***************************************************************************************************/
void UART_GPIO_RX_Data(void)
{
	static uint8_t rx_count = 0;
	static uint32_t temp2 = 0;
	static uint16_t recv_data = 0;
	static uint16_t angle = 0;
	
	if(rx_count < DATA_BIT_LENGTH)
	{
		if(UART_RX_DATA_IN)
		{
			temp2 |= (1 << rx_count);
		}
		else
		{
			temp2 |= (0 << rx_count);
		}
	}
	if((++rx_count > DATA_BIT_LENGTH) && (UART_RX_DATA_IN))
	{
		recv_data = (uint16_t)(temp2 & 0x0000FFFF);
		switch((recv_data & 0xF000) >> 8)
		{
		case ANGLE_RX_CMD:
			angle = recv_data & 0x00FF;
			//角度信息后期再做处理
			break;
			
		case MIC_SWITCH_RX_CMD:
			stSysInfo.audio.mic_switch = (uint8_t)(recv_data & 0x0003);
			if(stSysInfo.audio.mic_switch == MIC_SWITCH_ON)
			{
				stMicSpeakerSwitch.MicSwitchFlag = ON;
				if(stSysInfo.audio.mic_mute == MIC_MUTE_ON)
				{
					stRunInfo.rgb_effects = LED_MIC_MUTE;
				}
				else
				{
					stRunInfo.rgb_effects = LED_MIC_SWITCH;
				}
			}
			else
			{
				stMicSpeakerSwitch.MicSwitchFlag = OFF;
				if(stSysInfo.audio.mic_mute == MIC_MUTE_ON)
				{
					stRunInfo.rgb_effects = LED_MIC_MUTE;
				}
				else
				{
					stRunInfo.rgb_effects = stRunInfo.rgb_flag;
				}
			}
			break;
			
		case SPEAKER_SWITCH_RX_CMD:
			//内外部扬声器切换，暂时没用到
			stSysInfo.audio.speaker_switch = (uint8_t)(recv_data & 0x0003);
			if(stSysInfo.audio.speaker_switch == SPEAKER_SWITCH_ON)	stMicSpeakerSwitch.SpeakerSwitchFlag = ON;
			else													stMicSpeakerSwitch.SpeakerSwitchFlag = OFF;
			break;
			
		case VOL_GRADE_RX_CMD:
			stSysInfo.audio.vol = (uint8_t)(recv_data & 0x000F);
			break;
		}
		temp2 = 0;
		rx_count = 0;
		MOER_RX_TIMER_OFF();
		e_uart_rx_sta = uart_rx_stop_bit;
	}
}

/***************************************************************************************************
* Description:  模拟串口GPIO初始化
***************************************************************************************************/
void UART_GPIO_Init(void)
{
	UART_GPIO_TX_Init();
	UART_GPIO_RX_Init();
}
#endif

/****************************************** END OF FILE *******************************************/
