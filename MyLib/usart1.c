#include "usart1.h"
#include <stdarg.h>

#include "carcontrol.h"
#include "moto.h"
#include "LED.h"

unsigned char USART_RXDATA[64];
unsigned char USART_TXDATA[64];

/*
 * ��������USART1_Config
 * ����  ��USART1 GPIO ����,����ģʽ���á�115200 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//USART�˿�����
	USART_InitTypeDef USART_InitStructure;//USART����
//	NVIC_InitTypeDef NVIC_InitStructure;//USART�ж�����

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* USART1 GPIO config */
   /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
  USART_Cmd(USART1, ENABLE);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//USART1�����ж�ʹ��
}
/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART1_printf(USART_TypeDef* USARTx, char *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//ʮ����
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

//#define DEBUG 1

#define WAIT_HEADER 0
#define WAIT_DATATYPE 1
#define WAIT_MSB 2
#define WAIT_LSB 3
#define WAIT_TAIL 4


#define HEADER 0XAA
#define TAIL   0xBB

#define PID_OUT 0x00
#define PID_NEGOUT 0x01
#define CONTROL_CMD 0x02
#define SET_GOPWM 0x03
#define SET_TURNPWM 0x04
#define SET_PWMBASE 0x05

#define ENTER_STATE(x) (state = (x))

void USART_AnalyzeFrame(u8 data)
{
	static char state = 0;//״̬��
	static int dataType;//ָ������
	static int buf[2], result;
	
	switch(state)
	{
		case WAIT_HEADER:
			if(data == HEADER)
			{
				ENTER_STATE(WAIT_DATATYPE);//�յ�֡ͷ��������һ��״̬�����ȴ���������
				#ifdef DEBUG
					USART1_printf(USART1, "receive the head\n\r");
				#endif
			}
			break;
		case WAIT_DATATYPE:	
			if(data == HEADER)//�յ�֡ͷ������ԭ״̬���ȴ���������
			{
				ENTER_STATE(WAIT_DATATYPE);
				#ifdef DEBUG
					USART1_printf(USART1, "wrong frmae\n\r");
					USART1_printf(USART1, "receive the head\n\r");
				#endif
			}
			else if(data != PID_OUT && data != PID_NEGOUT && data != CONTROL_CMD
						&& data != SET_GOPWM && data != SET_TURNPWM)//������֪�������ͣ�����
			{														//״̬����λ					
				ENTER_STATE(WAIT_HEADER);
				#ifdef DEBUG
					USART1_printf(USART1, "wrong frmae\n\r");
				#endif
			}
			else
			{
				dataType = data;//������������
				ENTER_STATE(WAIT_MSB);//������һ��״̬�����������ݸ߰�λ
				#ifdef DEBUG
					USART1_printf(USART1, "receive the datatype\n\r");
				#endif
			}
			break;
		case WAIT_MSB:
			buf[0] = data;//�������ݸ߰�λ��������һ��״̬�����ȴ��������ݵͰ�λ
			ENTER_STATE(WAIT_LSB);
			break;
		case WAIT_LSB:
			buf[1] = data;
			ENTER_STATE(WAIT_TAIL);//�յ��ߵ��ֽڣ�������һ��״̬��,����֡β
			#ifdef DEBUG
				USART1_printf(USART1, "receive msb and lsb\n\r");
			#endif
		break;
		case WAIT_TAIL:
		if(data == HEADER)//�յ�֡ͷ��һ֡����״̬���ȴ���������
		{
			ENTER_STATE(WAIT_DATATYPE);
			#ifdef DEBUG
				USART1_printf(USART1, "wrong frame\n\r");
				USART1_printf(USART1, "receive the head\n\r");
			#endif
		}
		else if(data == TAIL)//�յ�֡β,һ֡��ȷ����������
		{
			result = (buf[0] << 8) | buf[1];//�ߵ��ֽںϳ�16λԭ����
			if(dataType == PID_OUT)//ָ��ΪPID�����
				Car_PIDPosControl(result);
			else if(dataType == PID_NEGOUT)//ָ������ΪPID�����
				Car_PIDNegControl(result);
			else if(dataType == CONTROL_CMD)//ָ������ΪС������
				Car_Control(result);
			else if(dataType == SET_GOPWM)//ָ������Ϊ����ǰ��PWM
				Car_SetGoSpeed(result);
			else if(dataType == SET_TURNPWM)//ָ������Ϊ����ת��PWM
				Car_SetTurnSpeed(result);
			else if(dataType == SET_PWMBASE)//ָ������Ϊ����PID��׼PWM
				Car_SetPWMBase(result);
			#ifdef DEBUG
				USART1_printf(USART1, "success to receive a frame\n\r");
				USART1_printf(USART1, "datatype is:%d\n\r",dataType);
				USART1_printf(USART1, "data is:%d\n\r",result);
			#endif		
		}
		ENTER_STATE(WAIT_HEADER);//״̬����λ��������һ֡
		break;
		default:break;
	}
}





