#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"

uint8_t IsDoorOpen(void);
void Motor_Init(void);
void Motor_HandleInput(void);
void Motor_TestLoop(void);

#endif
