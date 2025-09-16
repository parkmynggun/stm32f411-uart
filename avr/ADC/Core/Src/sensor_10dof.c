#include "sensor_10dof.h"
#include <math.h> // For BMP180 calculation if needed

#define MPU6050_ADDR  (0x68 << 1) // AD0 low
#define HMC5883L_ADDR (0x1E << 1)
#define BMP180_ADDR   (0x77 << 1)

static I2C_HandleTypeDef *i2c;

// BMP180 Calibration data
static struct {
    int16_t ac1, ac2, ac3;
    uint16_t ac4, ac5, ac6;
    int16_t b1, b2, mb, mc, md;
} bmp180_cal;

// Private function to read calibration data
static HAL_StatusTypeDef BMP180_ReadCalibrationData(void) {
    uint8_t cal_data[22];
    if (HAL_I2C_Mem_Read(i2c, BMP180_ADDR, 0xAA, 1, cal_data, 22, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }

    bmp180_cal.ac1 = (int16_t)(cal_data[0] << 8 | cal_data[1]);
    bmp180_cal.ac2 = (int16_t)(cal_data[2] << 8 | cal_data[3]);
    bmp180_cal.ac3 = (int16_t)(cal_data[4] << 8 | cal_data[5]);
    bmp180_cal.ac4 = (uint16_t)(cal_data[6] << 8 | cal_data[7]);
    bmp180_cal.ac5 = (uint16_t)(cal_data[8] << 8 | cal_data[9]);
    bmp180_cal.ac6 = (uint16_t)(cal_data[10] << 8 | cal_data[11]);
    bmp180_cal.b1 = (int16_t)(cal_data[12] << 8 | cal_data[13]);
    bmp180_cal.b2 = (int16_t)(cal_data[14] << 8 | cal_data[15]);
    bmp180_cal.mb = (int16_t)(cal_data[16] << 8 | cal_data[17]);
    bmp180_cal.mc = (int16_t)(cal_data[18] << 8 | cal_data[19]);
    bmp180_cal.md = (int16_t)(cal_data[20] << 8 | cal_data[21]);

    return HAL_OK;
}

// Return HAL_StatusTypeDef to check for errors
HAL_StatusTypeDef Sensor10DOF_Init(I2C_HandleTypeDef *hi2c_in) {
    i2c = hi2c_in;
    uint8_t data;
    HAL_StatusTypeDef status = HAL_OK;

    // MPU6050 Init: Wake up
    data = 0x00;
    status |= HAL_I2C_Mem_Write(i2c, MPU6050_ADDR, 0x6B, 1, &data, 1, HAL_MAX_DELAY);

    // HMC5883L Init: Continuous measurement mode
    data = 0x70; // 8-average, 15Hz
    status |= HAL_I2C_Mem_Write(i2c, HMC5883L_ADDR, 0x00, 1, &data, 1, HAL_MAX_DELAY);
    data = 0xA0; // Gain
    status |= HAL_I2C_Mem_Write(i2c, HMC5883L_ADDR, 0x01, 1, &data, 1, HAL_MAX_DELAY);
    data = 0x00; // Continuous measurement
    status |= HAL_I2C_Mem_Write(i2c, HMC5883L_ADDR, 0x02, 1, &data, 1, HAL_MAX_DELAY);

    // BMP180 Init: Read calibration data
    status |= BMP180_ReadCalibrationData();

    return status;
}

HAL_StatusTypeDef Sensor10DOF_ReadMPU6050(int16_t *accel, int16_t *gyro) {
    uint8_t buffer[14];
    if (HAL_I2C_Mem_Read(i2c, MPU6050_ADDR, 0x3B, 1, buffer, 14, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }
    accel[0] = (int16_t)(buffer[0] << 8 | buffer[1]);
    accel[1] = (int16_t)(buffer[2] << 8 | buffer[3]);
    accel[2] = (int16_t)(buffer[4] << 8 | buffer[5]);
    gyro[0] = (int16_t)(buffer[8] << 8 | buffer[9]);
    gyro[1] = (int16_t)(buffer[10] << 8 | buffer[11]);
    gyro[2] = (int16_t)(buffer[12] << 8 | buffer[13]);
    return HAL_OK;
}

HAL_StatusTypeDef Sensor10DOF_ReadHMC5883L(int16_t *mag) {
    uint8_t buffer[6];
    if (HAL_I2C_Mem_Read(i2c, HMC5883L_ADDR, 0x03, 1, buffer, 6, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }
    mag[0] = (int16_t)(buffer[0] << 8 | buffer[1]); // X
    mag[2] = (int16_t)(buffer[2] << 8 | buffer[3]); // Z
    mag[1] = (int16_t)(buffer[4] << 8 | buffer[5]); // Y
    return HAL_OK;
}

// Simplified BMP180 pressure reading (now with placeholder for calibration)
// NOTE: Full calculation is complex. This is a simplified version.
HAL_StatusTypeDef Sensor10DOF_ReadBMP180(float *pressure) {
    uint8_t cmd, raw_data[3];

    // Request temperature
    cmd = 0x2E;
    if (HAL_I2C_Mem_Write(i2c, BMP180_ADDR, 0xF4, 1, &cmd, 1, HAL_MAX_DELAY) != HAL_OK) return HAL_ERROR;
    HAL_Delay(5);
    if (HAL_I2C_Mem_Read(i2c, BMP180_ADDR, 0xF6, 1, raw_data, 2, HAL_MAX_DELAY) != HAL_OK) return HAL_ERROR;
    long ut = (raw_data[0] << 8) | raw_data[1];

    // Request pressure (oversampling setting 0)
    cmd = 0x34;
    if (HAL_I2C_Mem_Write(i2c, BMP180_ADDR, 0xF4, 1, &cmd, 1, HAL_MAX_DELAY) != HAL_OK) return HAL_ERROR;
    HAL_Delay(5);
    if (HAL_I2C_Mem_Read(i2c, BMP180_ADDR, 0xF6, 1, raw_data, 2, HAL_MAX_DELAY) != HAL_OK) return HAL_ERROR;
    long up = (raw_data[0] << 8) | raw_data[1];

    // ** Perform calculation using bmp180_cal data **
    // This part is complex, for a fully working code, you need to implement the full calculation
    // from the BMP180 datasheet. For now, we return the raw value for demonstration.
    long x1 = ((ut - bmp180_cal.ac6) * bmp180_cal.ac5) >> 15;
    long x2 = ((long)bmp180_cal.mc << 11) / (x1 + bmp180_cal.md);
    long b5 = x1 + x2;
    // ... more calculations for pressure ...

    *pressure = (float)up; // Placeholder: returning raw pressure
    return HAL_OK;
}
