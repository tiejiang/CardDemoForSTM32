#include "stm32f10x.h"
#include "NVIC.h"

void NVIC_MyInit(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
		/* NVIC_PriorityGroup */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		//´®¿Ú
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}


