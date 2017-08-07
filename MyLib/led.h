#ifndef __LED_H__
#define __LED_H__

#define LED0_ON  GPIOE->BRR = (1<<5)
#define LED0_OFF GPIOE->BSRR = (1<<5)

#define LED1_ON  GPIOE->BRR = (1<<6)
#define LED1_OFF GPIOE->BSRR = (1<<6)

void LEDInit(void);
void LED_LED0Shine(void);

#endif

