#ifndef SENSOR_10DOF_H
#define SENSOR_10DOF_H

#include "stm32f4xx_hal.h"

// Return HAL_StatusTypeDef to allow error checking
HAL_StatusTypeDef Sensor10DOF_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef Sensor10DOF_ReadMPU6050(int16_t *accel, int16_t *gyro);
HAL_StatusTypeDef Sensor10DOF_ReadHMC5883L(int16_t *mag);
HAL_StatusTypeDef Sensor10DOF_ReadBMP180(float *pressure); // Returns status, pressure via pointer

#endif
