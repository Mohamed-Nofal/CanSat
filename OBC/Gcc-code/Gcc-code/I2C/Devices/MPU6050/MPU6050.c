/*
 * MPU6050.c
 *
 * Created: 11/22/2020 6:24:51 PM
 *  Author: mizo
 */ 
#include "MPU6050.h"
#include "../../I2C_Master_H_file.h"
uint8_t s;
uint8_t initMPU(){
	_delay_ms(150);										/* Power up time >100ms */
	I2C_Start_Wait(0xD0);								/* Start with device write address */
	I2C_Write(SMPLRT_DIV);								/* Write to sample rate register */
	I2C_Write(0x07);									/* 1KHz sample rate */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								/* Write to power management register */
	I2C_Write(0x01);									/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									/* Write to Configuration register */
	I2C_Write(0x00);									/* Fs = 8KHz */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								/* Write to Gyro configuration register */
	I2C_Write(0x18);									/* Full scale range +/- 2000 degree/C */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
}

uint8_t startLocMPU(){
	I2C_Start_Wait(0xD0);								/* I2C start with device write address */
	I2C_Write(ACCEL_XOUT_H);							/* Write start location address from where to read */
	I2C_Repeated_Start(0xD1);							/* I2C start with device read address */
}

uint8_t readRawMPU(MPU *mpu){
	startLocMPU();									
	(*mpu).Acc_x = (readI2C(ACK)<<8) | readI2C(ACK);
	(*mpu).Acc_y = (readI2C(ACK)<<8) | readI2C(ACK);
	(*mpu).Acc_z = (readI2C(ACK)<<8) | readI2C(ACK);
	(*mpu).Temperature = (readI2C(ACK)<<8) | readI2C(ACK);
	(*mpu).Gyro_x = (readI2C(ACK)<<8) | readI2C(ACK);
	(*mpu).Gyro_y = (readI2C(ACK)<<8) | readI2C(ACK);
	(*mpu).Gyro_z = (readI2C(ACK)<<8) | readI2C(NACK);
	stopI2C();
}