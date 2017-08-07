#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

extern unsigned char USART_RXDATA[64];
extern unsigned char USART_TXDATA[64];

void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, char *Data,...);
void USART_AnalyzeFrame(u8 data);

#endif /* __USART1_H */
