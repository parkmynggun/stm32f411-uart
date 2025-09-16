#ifndef IR_H
#define IR_H

#include "stm32f4xx_hal.h"

#define IR_NUM 4

// 센서 인덱스 정의
#define IR_LEFT  0
#define IR_RIGHT 1
#define IR_UP    2
#define IR_DOWN  3

// 기존 변수들
extern volatile float ir_distance[IR_NUM];
extern volatile float ir0_distance;
extern volatile float ir1_distance;
extern volatile float ir2_distance;
extern volatile float ir3_distance;

// 기존 함수들
void IR_Init(void);
void IR_StartReadAll(void);
void IR_IRQHandler(void);
void IR_SetDummyValue(uint8_t idx, float value);

// 새로운 물체 탐지 관련 함수들
uint8_t IR_IsObjectDetected(void);           // 물체 탐지 상태 반환
uint32_t IR_GetObjectLostTime(void);         // 물체를 잃어버린 시간 반환 (ms)
float IR_GetMinDistance(void);               // 가장 가까운 센서의 거리 반환
int8_t IR_GetObjectDirection(void);          // 물체가 어느 방향에 있는지 반환
void IR_SetDummyMode(uint8_t enable);        // 더미 모드 설정/해제
void IR_ResetDetectionState(void);           // 물체 탐지 상태 강제 리셋

#endif /* IR_H */
