/*
 * BMP180.c
 *
 * Created: 11/23/2020 11:07:51 AM
 *  Author: mizo
 */ 
#include "BMP180.h"

uint8_t initBMP180(BMP180 *bmp180){
	for (int i = 0; i < 11; i++){
		//start communication with BMP180
		startI2C(BMP180_ADDR_w);
		//send target register (for coof)
		writeI2C((170 + (2 * i)));
		//send stop signal
		stopI2C();
		
		//start communication with BMP180
		startI2C(BMP180_ADDR_r);
		//read into coof
		switch(i){
			case 0:
				(*bmp180).coof.ac1 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 1:
				(*bmp180).coof.ac2 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 2:
				(*bmp180).coof.ac3 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 3:
				(*bmp180).coof.ac4 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 4:
				(*bmp180).coof.ac5 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 5:
				(*bmp180).coof.ac6 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 6:
				(*bmp180).coof.b1 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 7:
				(*bmp180).coof.b2 = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 8:
				(*bmp180).coof.mb = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 9:
				(*bmp180).coof.mc = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
			case 10:
				(*bmp180).coof.md = (readI2C(ACK) << 8) + readI2C(NACK);
				break;
		}
		//send stop signal
		stopI2C();
		//delay said in data sheet to be 50ms
		_delay_ms(50);
// 		//checksum
// 		if (((*bmp180).coof[i] > 32767) && (i != 3) && (i != 4) && (i != 5))
// 		{
// 			(*bmp180).coof[i] -= 65536;
// 		}
	}
	//delay 300ms
	_delay_ms(300);
	//init successful
	return INIT_COMPLETE;
}
uint8_t readBMP180(BMP180 *bmp180){
	//temporary data var
	uint8_t data[3];
	//start communicating with BMP180
	startI2C(BMP180_ADDR_w);
	//select measurement control register
	writeI2C(BMP180_REG_CONTROL);     
	//enable temperature measurement
	writeI2C(BMP180_COMMAND_TEMPERATURE); 
	//send stop signal                
	stopI2C();
	//delay 100ms
	_delay_ms(100);

	//start communication with MBP180
	startI2C(BMP180_ADDR_w);
	//select data register
	writeI2C(BMP180_REG_RESULT); 
	//send stop signal
	stopI2C();

	//start communication with BMP180
	startI2C(BMP180_ADDR_r);
	//start reading temprature
	int32_t temp = ((readI2C(ACK) <<8) + readI2C(NACK));
	//send stop signal
	stopI2C();

	//start communication with BMP180
	startI2C(BMP180_ADDR_w);
	//select measurement control register
	writeI2C(BMP180_REG_CONTROL); 
	//enable pressure measurement, OSS = 1
	writeI2C(BMP180_COMMAND_PRESSURE+(OSS<<6));
	//send stop signal
	stopI2C();
	//delay 100ms
	_delay_ms(100);
	//start communication with BMP180
	startI2C(BMP180_ADDR_w);
	//select data register
	writeI2C(BMP180_REG_RESULT); 
	//send stop signal
	stopI2C();
	//request 3 bytes of data
	//start communication with BMP180
	startI2C(BMP180_ADDR_r);
	//read pressure
	int32_t pres = readI2C(ACK);
	pres = (pres<<8) + readI2C(ACK);
	pres = (pres<<8) + readI2C(NACK);
	pres = pres >>(8-OSS); 
	//send stop signal
	stopI2C();

	// Convert the data
	

	// Calibration for Temperature
	int32_t X1 = (temp - (*bmp180).coof.ac6) * (*bmp180).coof.ac5 / pow(2,15);
	int32_t X2 = ((*bmp180).coof.mc * pow(2,11)) / (X1 + (*bmp180).coof.md);
	int32_t B5 = X1 + X2;
	float cTmp = ((B5 + 8.0) / pow(2,4)) / 10.0;
	float fTmp = cTmp * 1.8 + 32;

	// Calibration for Pressure
	int32_t B6 = B5 - 4000;
	X1 = ((*bmp180).coof.b2 * (B6 * B6 / pow(2,12))) / pow(2,11);
	X2 = (*bmp180).coof.ac2 * B6 / pow(2,11);
	int32_t X3 = X1 + X2;
	int32_t B3 = ((((*bmp180).coof.ac1 * 4 + X3) <<OSS) + 2) / 4;
	X1 = (*bmp180).coof.ac3 * B6 / pow(2,13);
	X2 = ((*bmp180).coof.b1 * (B6 * B6 / pow(2,12))) / pow(2,16);
	X3 = ((X1 + X2) + 2) / pow(2,2);
	uint32_t B4 = (*bmp180).coof.ac4 * (uint32_t)(X3 + 32768) / pow(2,15);
	uint32_t B7 = ((uint32_t)pres - B3) * (50000>>OSS);
	int32_t pressure ;
	if (B7 < 0x80000000)
	{
		pressure = (B7 * 2) / B4;
	}
	else
	{
		pressure = (B7 / B4) * 2;
	}
	X1 = (pressure / pow(2,8)) * (pressure / pow(2,8));
	X1 = (X1 * 3038) / pow(2,16);
	X2 = ((-7357) * pressure) / pow(2,16);
	pressure = (pressure + (X1 + X2 + 3791) / pow(2,4)) ;

	// Calculate Altitude
	float height = 44330 * (1 - pow((pressure / 1013.25), 0.1903));
	(*bmp180).ctemp=cTmp;
	(*bmp180).ftemp=fTmp;
	(*bmp180).pressure=pressure;
	(*bmp180).altitude=height;
	return READ_SUCCESSFUL;
}


