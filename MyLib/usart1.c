#include "usart1.h"
#include <stdarg.h>

#include "carcontrol.h"
#include "moto.h"
#include "LED.h"

unsigned char USART_RXDATA[64];
unsigned char USART_TXDATA[64];

/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//USART端口设置
	USART_InitTypeDef USART_InitStructure;//USART设置
//	NVIC_InitTypeDef NVIC_InitStructure;//USART中断设置

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
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//USART1接收中断使能
}
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
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
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
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

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
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
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
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
	static char state = 0;//状态机
	static int dataType;//指令类型
	static int buf[2], result;
	
	switch(state)
	{
		case WAIT_HEADER:
			if(data == HEADER)
			{
				ENTER_STATE(WAIT_DATATYPE);//收到帧头，进入下一个状态机，等待数据类型
				#ifdef DEBUG
					USART1_printf(USART1, "receive the head\n\r");
				#endif
			}
			break;
		case WAIT_DATATYPE:	
			if(data == HEADER)//收到帧头，保持原状态，等待数据类型
			{
				ENTER_STATE(WAIT_DATATYPE);
				#ifdef DEBUG
					USART1_printf(USART1, "wrong frmae\n\r");
					USART1_printf(USART1, "receive the head\n\r");
				#endif
			}
			else if(data != PID_OUT && data != PID_NEGOUT && data != CONTROL_CMD
						&& data != SET_GOPWM && data != SET_TURNPWM)//不是已知数据类型，错误，
			{														//状态机复位					
				ENTER_STATE(WAIT_HEADER);
				#ifdef DEBUG
					USART1_printf(USART1, "wrong frmae\n\r");
				#endif
			}
			else
			{
				dataType = data;//保存数据类型
				ENTER_STATE(WAIT_MSB);//进入下一个状态机，接收数据高八位
				#ifdef DEBUG
					USART1_printf(USART1, "receive the datatype\n\r");
				#endif
			}
			break;
		case WAIT_MSB:
			buf[0] = data;//保存数据高八位，进入下一个状态机，等待接收数据低八位
			ENTER_STATE(WAIT_LSB);
			break;
		case WAIT_LSB:
			buf[1] = data;
			ENTER_STATE(WAIT_TAIL);//收到高低字节，进入下一个状态机,接收帧尾
			#ifdef DEBUG
				USART1_printf(USART1, "receive msb and lsb\n\r");
			#endif
		break;
		case WAIT_TAIL:
		if(data == HEADER)//收到帧头，一帧错误，状态机等待数据类型
		{
			ENTER_STATE(WAIT_DATATYPE);
			#ifdef DEBUG
				USART1_printf(USART1, "wrong frame\n\r");
				USART1_printf(USART1, "receive the head\n\r");
			#endif
		}
		else if(data == TAIL)//收到帧尾,一帧正确，处理数据
		{
			result = (buf[0] << 8) | buf[1];//高低字节合成16位原数据
			if(dataType == PID_OUT)//指令为PID正输出
				Car_PIDPosControl(result);
			else if(dataType == PID_NEGOUT)//指令类型为PID负输出
				Car_PIDNegControl(result);
			else if(dataType == CONTROL_CMD)//指令类型为小车控制
				Car_Control(result);
			else if(dataType == SET_GOPWM)//指令类型为设置前进PWM
				Car_SetGoSpeed(result);
			else if(dataType == SET_TURNPWM)//指令类型为设置转向PWM
				Car_SetTurnSpeed(result);
			else if(dataType == SET_PWMBASE)//指令类型为设置PID基准PWM
				Car_SetPWMBase(result);
			#ifdef DEBUG
				USART1_printf(USART1, "success to receive a frame\n\r");
				USART1_printf(USART1, "datatype is:%d\n\r",dataType);
				USART1_printf(USART1, "data is:%d\n\r",result);
			#endif		
		}
		ENTER_STATE(WAIT_HEADER);//状态机复位，接收下一帧
		break;
		default:break;
	}
}





