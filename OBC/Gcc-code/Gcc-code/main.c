/*
 * GccApplication2.c
 *
 * Created: 11/22/2020 2:27:43 PM
 * Author : mizo
 */ 

#include <avr/io.h>
#include "config.h"
#include "UART/Interface/UART.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "UART/Devices/GPS/GPS.h"
#include "I2C/Devices/MPU6050/MPU6050.h"
#include "I2C/Devices/BMP180/BMP180.h"
#include "SPI/Devices/SD_CARD_MODULE/mmc_avr.h"
#include "GPIO/devices/DHT.h" //PORTD
#define BUFFER_SIZE 300
int main(void)
{
	//define sensors
	BMP180 bmp;
	MPU mpu;
	GPS gps;
	DHT dht;
	//define SD
	DSTATUS ds;
	FRESULT fr;
	FATFS FatFs;		/* File system object for each logical drive */
	FIL File;		/* File object */
	//initialize peripherals
	initUART();
	initI2C(16);
	initMPU(&mpu);
	initBMP180(&bmp);
	ds = disk_initialize(0);
	

	// buffer
	char buffer[BUFFER_SIZE];
	UINT written_bytes = 0;
	
	//mount SD and open file
	
	f_mount(&FatFs, "", 0);
	fr = f_open(&File, "test.txt", FA_WRITE);
	if (fr == FR_OK) {
		/* write all lines and display it */
		 while (1){
// 			 _delay_ms(100);
 			 readBMP180(&bmp);
 			 readRawMPU(&mpu);
			 readGPS(&gps);
			 readDHT(PIND2,&dht);
			 sprintf(buffer,
					"GPS:%f,%f,%d\n"
					"DHT:%f,%f\n"
					"MPU:%f,%f,%f,%f,%f,%f,%f\n"
					"BMP:%ld,%f,%f\n\r",
					gps.latitude,gps.longitude,gps.NoS,
					dht.temp,dht.humi,
					mpu.Acc_x/16384.0,mpu.Acc_y/16384.0,mpu.Acc_z/16384.0,
					mpu.Gyro_x,mpu.Gyro_y,mpu.Gyro_z,
					(mpu.Temperature/340.00)+36.53,
					bmp.pressure,bmp.ctemp,bmp.altitude);
			 sendStringUART(buffer);
			fr=f_write(&File,buffer,BUFFER_SIZE,&written_bytes);
		 }
		/* Close the file */
		f_close(&File);
	}else{
		//print Error
		//to be filled
	}
   
}

