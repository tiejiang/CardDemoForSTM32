#include "stm32f10x.h"

#include "usart1.h"
#include "NVIC.h"
#include "moto.h"
#include "Delay.h"
#include "led.h"
#include "carcontrol.h"

int i=0;

int main(void)
{
	NVIC_MyInit();
	/*
	USART1_Config();
	*/
	DelayInit();
	/*
	Moto_Init();
	*/
	LEDInit();
	TIM3_Configuration();
					
	
	//USART1_printf(USART1, "start!\r\n");
	
	Servo_PWMSet(90);//机械手张开
			  
	while(1)
	{
		
	}
}





