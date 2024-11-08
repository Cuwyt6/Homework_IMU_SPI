//
// Created by lenovo on 24-11-2.
//
#include "main.h"

#ifndef PROJECT_BMI088_H
#define PROJECT_BMI088_H
void BMI088_ACCEL_NS_L(void);
void BMI088_ACCEL_NS_H(void);
void BMI088_GYRO_NS_L(void);
void BMI088_GYRO_NS_H(void);

void BMI088_ReadReg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length);
void BMI088_Calc_ACCEL(const uint8_t *return_data,float *acc,uint8_t magnification);
void BMI088_ReadReg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length);
void BMI088_WriteReg(uint8_t reg, uint8_t write_data);

void BMI088_Init(void);

#endif //PROJECT_BMI088_H
