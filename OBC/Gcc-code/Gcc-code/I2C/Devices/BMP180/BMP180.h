/*
 * BMP180.h
 *
 * Created: 11/23/2020 11:08:01 AM
 *  Author: mizo
 */ 


#ifndef BMP180_H_
#define BMP180_H_

#define INIT_COMPLETE 24
#define READ_SUCCESSFUL 25

#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include "BMP180_REG.h"
#include "../../Interface/MinI2C.h"
#include "../../../config.h"
typedef struct coof_struct{
	int16_t ac1;
	int16_t ac2;
	int16_t ac3;
	uint16_t ac4;
	uint16_t ac5;
	uint16_t ac6;
	int16_t b1;
	int16_t b2;
	int16_t mb;
	int16_t mc;
	int16_t md;
}Coof;
typedef struct BMP180_struct{
	Coof coof;
	float ctemp;
	float ftemp;
	int32_t pressure;
	float altitude;
}BMP180;

uint8_t initBMP180(BMP180 *bmp180);
uint8_t readBMP180(BMP180 *bmp180);

#endif /* BMP180_H_ */