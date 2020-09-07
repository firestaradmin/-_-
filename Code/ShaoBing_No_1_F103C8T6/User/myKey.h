#ifndef __MYKEY_H_
#define __MYKEY_H_
#include <stm32f10x.h>


//���尴������״̬ö��
typedef enum 
{
	KEY_UNPRESSED = 0,
	KEY_PRESSED = ~KEY_UNPRESSED,
}keyState_enum;

//���尴����Ϣ�ṹ��
typedef struct 
{
	//������Ϣ
	unsigned char longPressed ;
	unsigned char shortPressed ;
	unsigned char doublePressed ;
	unsigned char isLongPressing ;
	//��������״̬����
	keyState_enum keyState ;
	//������Ϣ������ʱ
	unsigned int pressedTime5ms ;
	unsigned char pressedTime1s;
}myKey_ValueTypedef;

//����5�������ṹ��
extern myKey_ValueTypedef 			key_RST_Value;
extern myKey_ValueTypedef 			key_UP_Value;
extern myKey_ValueTypedef 			key_DOWN_Value;
extern myKey_ValueTypedef 			key_LEFT_Value;
extern myKey_ValueTypedef 			key_RIGHT_Value;

extern u8 myKey_ValueChangedFlag;//������Ϣ�ı��־λ
extern u8 testNum;


void myKey_Init(void);//������ʼ��
void myKey_GetKeyValue(void);//���°�����Ϣ
void TIM3_Int_Init(u16 arr,u16 psc);//����������ʱ��ʱ����ʼ��

#endif


