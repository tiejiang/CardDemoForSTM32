#include "LED.h"
#include "stm32f10x.h"

void LEDInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
	GPIO_SetBits(GPIOE, GPIO_Pin_4);
}

void LED_LED0Shine(void)
{
	static char flag;
	if(flag)
		LED0_ON;
	else
		LED0_OFF;
	flag = !flag;
}



