/***************************************************************************************************
* FILE: isr.c
*
* DESCRIPTION:  Unified interrupt service routine.
***************************************************************************************************/
#include "bsp.h"
#include "system.h"
#include "wlan.h"
#include "uart_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
* Description:  外部中断0
***************************************************************************************************/
void SonyIr_EdgeHandler(int32_t level);
void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & EXTI_Line0)
    {
        EXTI->PR = EXTI_Line0;
        
        SonyIr_EdgeHandler(IR_DATA_IN);
    }
}

/***************************************************************************************************
* Description:  外部中断1
***************************************************************************************************/
void EXTI1_IRQHandler(void)
{
    if(EXTI->PR & EXTI_Line1)
    {
        EXTI->PR = EXTI_Line1;
        
        
    }
}

/***************************************************************************************************
* Description:  外部中断2
***************************************************************************************************/
void EXTI2_IRQHandler(void)
{
    if(EXTI->PR & EXTI_Line2)
    {
        EXTI->PR = EXTI_Line2;
    }
}

/***************************************************************************************************
* Description:  外部中断3
***************************************************************************************************/
void EXTI3_IRQHandler(void)
{
    if(EXTI->PR & EXTI_Line3)
    {
        EXTI->PR = EXTI_Line3;
    }
}

/***************************************************************************************************
* Description:  外部中断4
***************************************************************************************************/
void EXTI4_IRQHandler(void)
{
    if(EXTI->PR & EXTI_Line4)
    {
        EXTI->PR = EXTI_Line4;
    }
}

/***************************************************************************************************
* Description:  外部中断5~9
***************************************************************************************************/
void EXTI9_5_IRQHandler(void)
{
    /* EXTI5 */
    if(EXTI->PR & EXTI_Line5)
    {
        EXTI->PR = EXTI_Line5;
        
        
    }
    
    /* EXTI6 */
    if(EXTI->PR & EXTI_Line6)
    {
        EXTI->PR = EXTI_Line6;
        
        
    }
    
    /* EXTI7 */
    if(EXTI->PR & EXTI_Line7)
    {
        EXTI->PR = EXTI_Line7;
        
        
    }
    
    /* EXTI8 */
    if(EXTI->PR & EXTI_Line8)
    {
        EXTI->PR = EXTI_Line8;
        
    }
    
    /* EXTI9 */
    if(EXTI->PR & EXTI_Line9)
    {
        EXTI->PR = EXTI_Line9;
        
    }
}

/***************************************************************************************************
* Description:  外部中断10~15
***************************************************************************************************/
/* 红外地址检测 */
void IR_AddrDetection(void);

void EXTI15_10_IRQHandler(void)
{
    /* EXTI10 */
    if(EXTI->PR & EXTI_Line10)
    {
        EXTI->PR = EXTI_Line10;
        
    }
    
    /* EXTI11 */
    if(EXTI->PR & EXTI_Line11)
    {
        EXTI->PR = EXTI_Line11;
        
    }
    
    /* EXTI12 */
    if(EXTI->PR & EXTI_Line12)
    {
        EXTI->PR = EXTI_Line12;
        
    }
    
    /* EXTI13 */
    if(EXTI->PR & EXTI_Line13)
    {
        EXTI->PR = EXTI_Line13;
        IR_AddrDetection();
    }
    
    /* EXTI14 */
    if(EXTI->PR & EXTI_Line14)
    {
        EXTI->PR = EXTI_Line14;
		IR_AddrDetection();
    }
    
    /* EXTI15 */
    if(EXTI->PR & EXTI_Line15)
    {
        EXTI->PR = EXTI_Line15;

    }
}

/***************************************************************************************************
* Description: 电机驱动MS41909脉冲中断
***************************************************************************************************/
void MS41909_VdFzPulseHandler(void);
void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR & 0x0001)
    {
        TIM1->SR &= ~0x0001;
        
        MS41909_PULSE = 0;
        MS41909_PULSE = 1;
        
        MS41909_VdFzPulseHandler();
    }
}

/***************************************************************************************************
* Description:  系统时钟
***************************************************************************************************/
void TIM2_IRQHandler(void)
{
    if(TIM2->SR & 0x0001)
    {
        TIM2->SR &= ~0x0001;
        stRunInfo.time++;
		LED_Handler();
		if (stRunInfo.standby == OFF) {
			Wlan_LED_Handler();
			MOER_Audio_LED_Handler();
			MOER_MIC_Insert_LED_Handler();
		}
    }
}

/***************************************************************************************************
* Description:  模拟GPIO UART TX 定时器
***************************************************************************************************/
void TIM4_IRQHandler(void)
{
	static uint8_t tx_cnt = 0;
	if(TIM4->SR & 0x0001)
	{
		if(++tx_cnt >= BAUD_RATE_2400)
		{
			tx_cnt = 0;
			Moer_UART_TX_Handler();
		}
		TIM4->SR &= ~0x0001;
	}	
}
#ifdef __cplusplus
}
#endif

/****************************************** END OF FILE *******************************************/
