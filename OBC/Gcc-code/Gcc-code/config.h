/*
 * config.h
 *
 * Created: 11/22/2020 5:05:34 PM
 *  Author: mizo
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_
#define F_CPU 16000000UL
#define SCL_CLK 100000
#define MPU_ADDRESS 0xD0
#define MPU_SAMPLE_RATE 0x07 //1kHz
#define MPU_POWER_MAN_VALUE 0 //
#define CONFIG_DATA 0x00 //Fs = 8kHz
#define GYRO_CONFIG_DATA 0x18 //Full scale range +/- 2000 degree/C
#define INTERRUPT_REG_DATA 0 // inturrupt

#define USART_BAUDRATE 9600
#define OSS 0

#endif /* CONFIG_H_ */
