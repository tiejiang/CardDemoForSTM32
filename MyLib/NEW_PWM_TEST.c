/**
new pwm test

*/
/******************************************************************************************
* ��������    �� TIM3_Configuration
* ��������    �� TIM3��ʼ������
* ����        ��  ��
* ����ֵ      ��  ��
* ȫ�ֱ���    ��  ��
* ȫ�־�̬������  ��
* �ֲ���̬������  ��
******************************************************************************************/
#include "stm32f10x.h"
void TIM3_Configuration( )
{  
  GPIO_InitTypeDef          GPIO_InitStructure;	   //����һ��GPIO_InitStructure�ṹ��
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;   //����һ��TIM_TimeBaseStructure�ṹ��
  TIM_OCInitTypeDef      TIM_OCInitStructure; 		 //ָ��ṹTIM_OCInitTypeDef��ָ�룬������TIMxʱ�������λ��������Ϣ
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);   //��APB2����
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //��ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;    //PA6 PA7  ѡ������õ�GPIO�ܽţ�ʹ�ò�������|������һ��ѡ�ж���ܽ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //Ҫ��Ϊ AF_PP
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	  //����ѡ��GPIO�ܽŵ����ʣ��������2MHz?
  GPIO_Init(GPIOA, &GPIO_InitStructure);				//����GPIO_InitStructure��ָ���Ĳ�����ʼ������GPIOA

  TIM_DeInit(TIM3);                           //��TIM3�Ĵ�����Ϊȱʡֵ
  TIM_TimeBaseStructure.TIM_Period=72-1;   //ARR��ֵ,�������ڣ�TIM_Period����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ������ȡֵ������0x0000��0xFFFF֮�䣩   
  TIM_TimeBaseStructure.TIM_Prescaler=36-1;//��Ƶ  CK_INT=2MHz 36��Ƶ��TIM_Prescaler������������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ������ȡֵ������0x0000��0xFFFF֮�䡣��
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);    //��ʼ����ʱ��3
 
  //ͨ��1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;	//ѡ��ʱ��ģʽ--�˴�ΪTIME����Ƚϴ���ģʽ
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���ʹ��
  TIM_OCInitStructure.TIM_Pulse = 1-1 ;      //������ȽϼĴ�����ֵ  CRR
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //�ߵ�ƽ��Ч
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);     //ͨ����ʼ��--����TIME-OCInitStruct��ָ���Ĳ�����ʼ������TIM3
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //Ԥװ��ʹ��
  
  //ͨ��2
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1  ;	  //TIM�����ȵ�ʽģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���ʹ��
  TIM_OCInitStructure.TIM_Pulse = 36-1 ;      //������ȽϼĴ�����ֵ  CRR
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //�ߵ�ƽ��Ч
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);     //ͨ����ʼ�� ---����TIME-OCInitStruct��ָ���Ĳ�����ʼ������TIM3 
   
  TIM3->CNT = 0;
  TIM_Cmd(TIM3, ENABLE);//�򿪶�ʱ��
      
}

