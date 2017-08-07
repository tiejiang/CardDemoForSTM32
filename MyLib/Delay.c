#include "stm32f10x.h"

#include "Delay.h"

#define SYSCLK_FREQ 72

void DelayInit(void)
{
	SysTick->CTRL &= ~(1<<2) & ~(1<<1);	//外部时钟源，无异常请求
}

void DelayUs(u32 us)
{
	u32 temp;
	
	SysTick->LOAD = SYSCLK_FREQ * us / 8;//SysTick时钟为系统时钟8分频
	SysTick->VAL = 0;//清空计数器
	SysTick->CTRL |= (1<<0);//开启定时器
	do
	{
		temp = SysTick->CTRL;
	}while(!(temp&(1<<16)));
	SysTick->CTRL &= ~(1<<0);//关闭定时器
	SysTick->VAL = 0;
}
void DelayMs(u32 ms)
{
	u32 temp;
	
	ms *= 1000;
	SysTick->LOAD = SYSCLK_FREQ * ms / 8;//SysTick时钟为系统时钟8分频
	SysTick->VAL = 0;//清空计数器
	SysTick->CTRL |= (1<<0);//开启定时器
	do
	{
		temp = SysTick->CTRL;
	}while(!(temp&(1<<16)));
	SysTick->CTRL &= ~(1<<0);//关闭定时器
	SysTick->VAL = 0;
}





