//
// Created by lenovo on 24-11-2.
//
#include <math.h>
#include "BMI088.h"
#include "spi.h"
void BMI088_ACCEL_NS_L(void){
    HAL_GPIO_WritePin(SPI_ACC_GPIO_Port, SPI_ACC_Pin, GPIO_PIN_RESET);
}

void BMI088_ACCEL_NS_H(void){
    HAL_GPIO_WritePin(SPI_ACC_GPIO_Port, SPI_ACC_Pin, GPIO_PIN_SET);
}

void BMI088_GYRO_NS_L(void){
    HAL_GPIO_WritePin(SPI_GYRO_GPIO_Port, SPI_GYRO_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void){
    HAL_GPIO_WritePin(SPI_GYRO_GPIO_Port, SPI_GYRO_Pin, GPIO_PIN_SET);
}

float linearMapping(int16_t in, int in_min, int in_max, float out_min, float out_max) {
    float out,k,b;
    k=(out_max-out_min)/(float)(in_max-in_min);
    b=out_max-k*(float)in_max;
    out=k*(float)in+b;
    return out;
}

void BMI088_ReadReg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length) {
    BMI088_ACCEL_NS_L();  // 选通加速度计
    HAL_Delay(10);
    reg |= 0x80; //读操作
    HAL_SPI_Transmit(&hspi1, &reg, 1, 1000);
   // while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);   // 发送读操作的地址

    HAL_SPI_Receive(&hspi1, return_data, length, HAL_MAX_DELAY);// 接收寄存器数据
    //HAL_SPI_Receive(&hspi1, return_data, length, 1000);
    //while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_RX);

    BMI088_ACCEL_NS_H();  // 禁用加速度计
    HAL_Delay(10);
}

void BMI088_Calc_ACCEL(const uint8_t *return_data,float *acc,uint8_t magnification){
    acc[0] = (int16_t)((return_data[2] << 8) | return_data[1]) *1000*pow(2,(magnification+1))*1.5/32768;
    acc[1] = (int16_t)((return_data[4] << 8) | return_data[3]) *1000*pow(2,(magnification+1))*1.5/32768;
    acc[2] = (int16_t)((return_data[6] << 8) |return_data[5]) *1000*pow(2,(magnification+1))*1.5/32768;
}

void BMI088_ReadReg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length) {
    BMI088_GYRO_NS_L();  // 选通角速度计
    HAL_Delay(10);
    reg |= 0x80;
    HAL_SPI_Transmit(&hspi1, &reg, 1, 1000);
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);   // 发送读操作的地址

    HAL_SPI_Receive(&hspi1, return_data, length, 1000);// 接收寄存器数据
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_RX);

    BMI088_GYRO_NS_H();  // 禁用角速度计
    HAL_Delay(10);
}

//片选在函数调用的时候写在外面
void BMI088_WriteReg(uint8_t reg, uint8_t write_data) {
     reg &= 0x7F;

    HAL_SPI_Transmit(&hspi1, &reg, 1, 1000);
    //while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);   // 发送写操作的地址

    HAL_SPI_Transmit(&hspi1, &write_data, 1, 1000);
    //while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);

}

void BMI088_Init(void) {
    // Soft Reset ACCEL
    BMI088_ACCEL_NS_L();
    BMI088_WriteReg(0x7E, 0xB6); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();

    // Soft Reset GYRO
    BMI088_GYRO_NS_L();
    BMI088_WriteReg(0x14, 0xB6); // Write 0xB6 to GYRO_SOFTRESET(0x14)
    HAL_Delay(30);
    BMI088_GYRO_NS_H();

    // Switch ACCEL to Normal Mode
    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_WriteReg(0x7D, 0x04); // Write 0x04 to ACC_PWR_CTRL(0x7D)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();
}