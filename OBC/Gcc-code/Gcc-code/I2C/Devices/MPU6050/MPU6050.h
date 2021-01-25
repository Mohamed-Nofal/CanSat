/*
 * MPU6050.h
 *
 * Created: 11/22/2020 3:55:25 PM
 *  Author: mizo
 */ 


#ifndef MPU6050_H_
#define MPU6050_H_

#include "MPU6050_REG.h"
#include <util/delay.h>
#include <stdint.h>
#include "../../../config.h"
#include "../../Interface/MinI2C.h"
#define START_LOC_MPU_DONE 12

typedef struct MPU_struct {
	float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;
}MPU;

uint8_t initMPU();
uint8_t startLocMPU();
uint8_t readRawMPU(MPU *mpu);


#endif /* MPU6050_H_ */