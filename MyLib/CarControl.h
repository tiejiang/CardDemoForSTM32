#ifndef __CARCONTROL_H__
#define __CARCONTROL_H__

void Car_Control(u8 cmd);
void Car_SetTurnSpeed(int p);
void Car_SetGoSpeed(int p);
void Car_PIDPosControl(int pidOut);
void Car_PIDNegControl(int pidOut);
void Car_SetPWMBase(int p);

#endif






