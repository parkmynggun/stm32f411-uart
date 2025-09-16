#ifndef INC_PID_MOTOR_H_
#define INC_PID_MOTOR_H_

#include "stm32f4xx_hal.h"

void PID_Motor_Init(void);
void Turret_PID_Control(float dt, float ir_distances[]);

// 디버깅용 변수
extern float pid_output_debug;

#endif /* INC_PID_MOTOR_H_ */
