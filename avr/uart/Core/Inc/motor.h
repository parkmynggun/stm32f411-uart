#ifndef MOTOR_H
#define MOTOR_H
// 👇 여기 추가!
typedef enum {
    MODE_MANUAL = 0,
    MODE_AUTO = 1
} MotorMode_t;

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>

// 속도 증가/감소 단계 정의
#define SPEED_STEP 700

// 전역 변수 선언
extern uint8_t rxData;
extern volatile uint8_t newValueFlag;
extern volatile uint16_t currentCcrLeft;
extern volatile uint16_t currentCcrRight;
extern volatile uint16_t targetCcrLeft;
extern volatile uint16_t targetCcrRight;
extern volatile uint16_t lastSpeed;

// 함수 선언
void Motor_Init(void);
void ProcessMotorCommand(void);
void UpdateMotorSpeed(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HandleDistanceAndAutoBehavior(void);
#endif /* MOTOR_H */
