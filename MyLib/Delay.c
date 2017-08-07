#include "stm32f10x.h"

#include "Delay.h"

#define SYSCLK_FREQ 72

void DelayInit(void)
{
	SysTick->CTRL &= ~(1<<2) & ~(1<<1);	//�ⲿʱ��Դ�����쳣����
}

void DelayUs(u32 us)
{
	u32 temp;
	
	SysTick->LOAD = SYSCLK_FREQ * us / 8;//SysTickʱ��Ϊϵͳʱ��8��Ƶ
	SysTick->VAL = 0;//��ռ�����
	SysTick->CTRL |= (1<<0);//������ʱ��
	do
	{
		temp = SysTick->CTRL;
	}while(!(temp&(1<<16)));
	SysTick->CTRL &= ~(1<<0);//�رն�ʱ��
	SysTick->VAL = 0;
}
void DelayMs(u32 ms)
{
	u32 temp;
	
	ms *= 1000;
	SysTick->LOAD = SYSCLK_FREQ * ms / 8;//SysTickʱ��Ϊϵͳʱ��8��Ƶ
	SysTick->VAL = 0;//��ռ�����
	SysTick->CTRL |= (1<<0);//������ʱ��
	do
	{
		temp = SysTick->CTRL;
	}while(!(temp&(1<<16)));
	SysTick->CTRL &= ~(1<<0);//�رն�ʱ��
	SysTick->VAL = 0;
}





