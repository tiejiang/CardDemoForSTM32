#include "moto.h"
#include "CarControl.h"
#include "LED.h"

#define LEFT_CMD 0X02
#define RIGHT_CMD 0X03
#define STOP_CMD 0X00
#define GO_CMD 0X01
#define HAND_OPEN 0x04
#define HAND_CLOSE 0x05

int TURN_SPEDD = 800;
int NORMAL_SPEED = 600;
int PWM_BASE = 600;

void Car_SetTurnSpeed(int p)
{
	TURN_SPEDD = p;
}

void Car_SetGoSpeed(int p)
{
	NORMAL_SPEED = p;
}

void Car_SetPWMBase(int p)
{
	PWM_BASE = p;
}

void Car_PIDPosControl(int pidOut)
{
	Moto_PwmRflash(PWM_BASE - pidOut, PWM_BASE + pidOut, PWM_BASE - pidOut, PWM_BASE + pidOut);
}

void Car_PIDNegControl(int pidOut)
{
	Moto_PwmRflash(PWM_BASE + pidOut, PWM_BASE - pidOut, PWM_BASE + pidOut, PWM_BASE - pidOut);
}

void Car_TurnLeft()
{
	Moto_PwmRflash(TURN_SPEDD,0,TURN_SPEDD,0);
	Moto_BackPwmRflash(TURN_SPEDD,0,TURN_SPEDD,0);
	LED0_ON;
	LED1_OFF;
}

void Car_TurnRight()
{
	Moto_PwmRflash(0,TURN_SPEDD,0,TURN_SPEDD);
	Moto_BackPwmRflash(0,TURN_SPEDD,0,TURN_SPEDD);
	LED0_OFF;
	LED1_ON;
}
void Car_GoStraight()
{
	Moto_PwmRflash(NORMAL_SPEED,NORMAL_SPEED,NORMAL_SPEED,NORMAL_SPEED);
	Moto_BackPwmRflash(0,0,0,0);
	LED0_ON;
	LED1_ON;
}	

void Car_Stop()
{
	Moto_PwmRflash(0,0,0,0);
	Moto_BackPwmRflash(0,0,0,0);
	LED0_OFF;
	LED1_OFF;
}

void Car_HandOpen()
{
	Servo_PWMSet(90);
}

void Car_HandClose()
{
	Servo_PWMSet(110);
}

void Car_Control(u8 cmd)
{
	switch(cmd)
	{
		case GO_CMD:Car_GoStraight();
			break;
		case STOP_CMD:Car_Stop();
			break;
		case LEFT_CMD:Car_TurnLeft();
			break;
		case RIGHT_CMD:Car_TurnRight();
			break;
		case HAND_OPEN:Car_HandOpen();
			break;
		case HAND_CLOSE:Car_HandClose();
			break;
		default:break;
	}
}











