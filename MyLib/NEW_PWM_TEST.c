/**
new pwm test

*/
/******************************************************************************************
* 函数名称    ： TIM3_Configuration
* 功能描述    ： TIM3初始化函数
* 参数        ：  无
* 返回值      ：  无
* 全局变量    ：  无
* 全局静态变量：  无
* 局部静态变量：  无
******************************************************************************************/
#include "stm32f10x.h"
void TIM3_Configuration( )
{  
  GPIO_InitTypeDef          GPIO_InitStructure;	   //声明一个GPIO_InitStructure结构体
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;   //声明一个TIM_TimeBaseStructure结构体
  TIM_OCInitTypeDef      TIM_OCInitStructure; 		 //指向结构TIM_OCInitTypeDef的指针，包含了TIMx时间基数单位的配置信息
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);   //打开APB2外设
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;    //PA6 PA7  选择待设置的GPIO管脚，使用操作符“|”可以一次选中多个管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //要改为 AF_PP
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	  //设置选中GPIO管脚的速率，最高速率2MHz?
  GPIO_Init(GPIOA, &GPIO_InitStructure);				//根据GPIO_InitStructure中指定的参数初始化外设GPIOA

  TIM_DeInit(TIM3);                           //将TIM3寄存器设为缺省值
  TIM_TimeBaseStructure.TIM_Period=72-1;   //ARR的值,计数周期（TIM_Period设置了在下一个更新事件装入活动的自动重装载寄存器周期的值。它的取值必须在0x0000和0xFFFF之间）   
  TIM_TimeBaseStructure.TIM_Prescaler=36-1;//分频  CK_INT=2MHz 36分频（TIM_Prescaler设置了用来作为TIMx时钟频率除数的预分频值。它的取值必须在0x0000和0xFFFF之间。）
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);    //初始化定时器3
 
  //通道1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;	//选择定时器模式--此处为TIME输出比较触发模式
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
  TIM_OCInitStructure.TIM_Pulse = 1-1 ;      //待捕获比较寄存器的值  CRR
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //高电平有效
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);     //通道初始化--根据TIME-OCInitStruct中指定的参数初始化外设TIM3
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //预装载使能
  
  //通道2
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1  ;	  //TIM脉冲宽度调式模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
  TIM_OCInitStructure.TIM_Pulse = 36-1 ;      //待捕获比较寄存器的值  CRR
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //高电平有效
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);     //通道初始化 ---根据TIME-OCInitStruct中指定的参数初始化外设TIM3 
   
  TIM3->CNT = 0;
  TIM_Cmd(TIM3, ENABLE);//打开定时器
      
}

