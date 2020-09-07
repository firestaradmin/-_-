/************************************************************************************
*  Copyright (c), 2019, LXG.
*
* FileName		:myKey.c
* Author		:firestaradmin
* Version		:1.0
* Date			:2019.12.11
* Description	:�жϰ�����ֵ����
* History		:
*
*
*************************************************************************************/


#include "myKey.h"
#include "delay.h"
#include "OLED_I2C_Buffer.h"
#include "myShaoBingApp.h"
#include "DS18B20.h"

#define KEY_RST_GPIO_PortSource		GPIO_PortSourceGPIOA
#define KEY_UP_GPIO_PortSource 		GPIO_PortSourceGPIOB
#define KEY_DOWN_GPIO_PortSource	GPIO_PortSourceGPIOB
#define KEY_LEFT_GPIO_PortSource 	GPIO_PortSourceGPIOB
#define KEY_RIGHT_GPIO_PortSource 	GPIO_PortSourceGPIOB

#define KEY_RST_GPIO_PinSource		GPIO_PinSource0
#define KEY_UP_GPIO_PinSource		GPIO_PinSource15
#define KEY_DOWN_GPIO_PinSource 	GPIO_PinSource14
#define KEY_LEFT_GPIO_PinSource 	GPIO_PinSource13
#define KEY_RIGHT_GPIO_PinSource 	GPIO_PinSource12


#define KEY_RST_GPIO_PORT		GPIOA
#define KEY_UP_GPIO_PORT 		GPIOB
#define KEY_DOWN_GPIO_PORT		GPIOB
#define KEY_LEFT_GPIO_PORT 		GPIOB
#define KEY_RIGHT_GPIO_PORT 	GPIOB

#define KEY_RST_GPIO_PIN		GPIO_Pin_0
#define KEY_UP_GPIO_PIN 		GPIO_Pin_15
#define KEY_DOWN_GPIO_PIN 		GPIO_Pin_14
#define KEY_LEFT_GPIO_PIN 		GPIO_Pin_13
#define KEY_RIGHT_GPIO_PIN 		GPIO_Pin_12

#define KEY_RST_EXTI_LINE 		EXTI_Line0
#define KEY_UP_EXTI_LINE 		EXTI_Line15
#define KEY_DOWN_EXTI_LINE 		EXTI_Line14
#define KEY_LEFT_EXTI_LINE 		EXTI_Line13
#define KEY_RIGHT_EXTI_LINE 	EXTI_Line12
//5ms������
unsigned short delay_Time5ms = 0;
//���������±�־
unsigned char myKey_IsPressed_Flag = 0;
//����ɨ����ʱ����
unsigned char myKey_GetKeyValue_delayTime5ms;

unsigned char delay_EXTI0_delayTime5ms;

unsigned char delay_EXTI15_10_delayTime5ms;


myKey_ValueTypedef key_RST_Value;
myKey_ValueTypedef key_UP_Value;
myKey_ValueTypedef key_DOWN_Value;
myKey_ValueTypedef key_LEFT_Value;
myKey_ValueTypedef key_RIGHT_Value;
u8 myKey_ValueChangedFlag = 0;;


void TIM3_IRQHandler(void);   //TIM3�ж�
void TIM3_Int_Init(u16 arr,u16 psc);



void myKey_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
		/* EXTI��ʱ��Ҫ����AFIO�Ĵ��� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE) ;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	
	GPIO_InitStructure.GPIO_Pin =  KEY_RST_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  KEY_UP_GPIO_PIN | KEY_DOWN_GPIO_PIN | KEY_LEFT_GPIO_PIN | KEY_RIGHT_GPIO_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ��ʼ��EXTI���� */
	/* ѡ����ΪEXTI�ߵ�GPIO���� */
	GPIO_EXTILineConfig( KEY_RST_GPIO_PortSource , KEY_RST_GPIO_PinSource) ;
	/* �����ж�or�¼��� */
	EXTI_InitStruct.EXTI_Line = KEY_RST_EXTI_LINE ;
	/* ����ģʽ���ж�or�¼� */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	/* ���ñ��ش��� ����or�½� */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling ;
	/* ʹ��EXTI�� */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_Init(&EXTI_InitStruct) ;
	
	GPIO_EXTILineConfig( KEY_UP_GPIO_PortSource ,  KEY_UP_GPIO_PinSource) ;
	EXTI_InitStruct.EXTI_Line = KEY_UP_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling ;
	EXTI_Init(&EXTI_InitStruct);
	
	GPIO_EXTILineConfig( KEY_DOWN_GPIO_PortSource ,  KEY_DOWN_GPIO_PinSource) ;
	EXTI_InitStruct.EXTI_Line = KEY_DOWN_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling ;
	EXTI_Init(&EXTI_InitStruct);
	
	GPIO_EXTILineConfig( KEY_LEFT_GPIO_PortSource ,  KEY_LEFT_GPIO_PinSource) ;
	EXTI_InitStruct.EXTI_Line = KEY_LEFT_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling ;
	EXTI_Init(&EXTI_InitStruct);
	
	GPIO_EXTILineConfig( KEY_RIGHT_GPIO_PortSource ,  KEY_RIGHT_GPIO_PinSource) ;
	EXTI_InitStruct.EXTI_Line = KEY_RIGHT_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling ;
	EXTI_Init(&EXTI_InitStruct);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	//�жϵ�ʱ��Ϊt��t = (arr * psc / APB1*2) * 1000 ms
	TIM3_Int_Init(1000,360); //5ms
}

  


void EXTI0_IRQHandler(void)
{
	delay_EXTI0_delayTime5ms = 0;
	while(delay_EXTI0_delayTime5ms > 2);
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		myKey_IsPressed_Flag = 1;
	}
	
	EXTI_ClearITPendingBit(KEY_RST_EXTI_LINE);
	
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(KEY_UP_EXTI_LINE)!=RESET)
	{
		delay_EXTI15_10_delayTime5ms = 0;
		while(delay_EXTI15_10_delayTime5ms > 2);
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15))
		{
			myKey_IsPressed_Flag = 1;
	
		}
	}
	if(EXTI_GetITStatus(KEY_DOWN_EXTI_LINE)!=RESET)
	{
		delay_EXTI15_10_delayTime5ms = 0;
		while(delay_EXTI15_10_delayTime5ms > 2);
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
		{
			myKey_IsPressed_Flag = 1;
		
		}
	}
	if(EXTI_GetITStatus(KEY_LEFT_EXTI_LINE)!=RESET)
	{
		delay_EXTI15_10_delayTime5ms = 0;
		while(delay_EXTI15_10_delayTime5ms > 2);
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
		{
			myKey_IsPressed_Flag = 1;
			
		}
	}
	if(EXTI_GetITStatus(KEY_RIGHT_EXTI_LINE)!=RESET)
	{
		delay_EXTI15_10_delayTime5ms = 0;
		while(delay_EXTI15_10_delayTime5ms > 2);
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
		{
			myKey_IsPressed_Flag = 1;
		
		}
	}
	
		
	EXTI_ClearITPendingBit(KEY_UP_EXTI_LINE);
	EXTI_ClearITPendingBit(KEY_DOWN_EXTI_LINE);
	EXTI_ClearITPendingBit(KEY_LEFT_EXTI_LINE);
	EXTI_ClearITPendingBit(KEY_RIGHT_EXTI_LINE);

}


void judge_KeyValue(GPIO_TypeDef * keyGPIO, uint16_t keyGPIO_Pin, myKey_ValueTypedef * keyValue)
{
	//������
	if(!GPIO_ReadInputDataBit(keyGPIO, keyGPIO_Pin))//����������ڰ���״̬
	{
		if(keyValue->keyState == KEY_UNPRESSED)
		{
			keyValue->keyState = KEY_PRESSED;
			keyValue->pressedTime5ms = 0;
		}
		else if(keyValue->keyState == KEY_PRESSED && keyValue->pressedTime5ms >= 200)
		{
			keyValue->isLongPressing = 1;	
		}
	}
	
	//����������ڵ���״̬״̬����֮ǰ�б����£��򽫸��ݰ���ʱ��ȷ����ֵ
	else if(GPIO_ReadInputDataBit(keyGPIO, keyGPIO_Pin) && keyValue->keyState == KEY_PRESSED)
	{
		keyValue->keyState = KEY_UNPRESSED;
		keyValue->isLongPressing = 0;	
		if(keyValue->pressedTime5ms >= 200)
		{
			keyValue->longPressed = 1;
			keyValue->shortPressed = 0;
		}
		else
		{
			keyValue->longPressed = 0;
			keyValue->shortPressed = 1;
		}
		myKey_IsPressed_Flag = 0;	
		myKey_ValueChangedFlag = 1;
	}
}

//ɨ�谴����Ϣ����
void myKey_GetKeyValue()
{
	if(myKey_IsPressed_Flag == 1 && myKey_GetKeyValue_delayTime5ms > 10) //�ж�����а����������� �����Ұ���ɨ�����ڵ�����Ӧʱ������ʼɨ�谴��
	{
		
		
		judge_KeyValue(KEY_RST_GPIO_PORT, KEY_RST_GPIO_PIN, &key_RST_Value);
		judge_KeyValue(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN, &key_UP_Value);
		judge_KeyValue(KEY_DOWN_GPIO_PORT, KEY_DOWN_GPIO_PIN, &key_DOWN_Value);
		judge_KeyValue(KEY_LEFT_GPIO_PORT, KEY_LEFT_GPIO_PIN, &key_LEFT_Value);
		judge_KeyValue(KEY_RIGHT_GPIO_PORT, KEY_RIGHT_GPIO_PIN, &key_RIGHT_Value);

		
		myKey_GetKeyValue_delayTime5ms = 0;//�������ɨ�����ڼ���
		
	}
	
}


//������ʱ�ж�
//��ʱ��3�жϷ������ 5msһ��
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			
			delay_Time5ms++;
			if(delay_Time5ms == 200)//1S
			{
				delay_Time5ms = 0;
				//testDynamicNum ++;
				
				
			}
			
			if(key_RST_Value.keyState == KEY_PRESSED)
			{
				key_RST_Value.pressedTime5ms++;
			}
			
			if(key_UP_Value.keyState == KEY_PRESSED)
			{
				key_UP_Value.pressedTime5ms++;
			}
			if(key_DOWN_Value.keyState == KEY_PRESSED)
			{
				key_DOWN_Value.pressedTime5ms++;
			}
			if(key_LEFT_Value.keyState == KEY_PRESSED)
			{
				key_LEFT_Value.pressedTime5ms++;
			}
			if(key_RIGHT_Value.keyState == KEY_PRESSED)
			{
				key_RIGHT_Value.pressedTime5ms++;
			}
			
			delay_EXTI0_delayTime5ms++;
			delay_EXTI15_10_delayTime5ms++;
			myKey_GetKeyValue_delayTime5ms++;
			
//			if(delay_EXTI0_delayTime5ms > 200)delay_EXTI0_delayTime5ms = 0;
//			if(delay_EXTI15_10_delayTime5ms > 200)delay_EXTI15_10_delayTime5ms = 0;
//			if(myKey_GetKeyValue_delayTime5ms > 200)myKey_GetKeyValue_delayTime5ms = 0;
		}
}


//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//һ���жϵ�ʱ��Ϊt��t = (arr * psc / APB1*2) * 1000 ms
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
 
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3					 
}


