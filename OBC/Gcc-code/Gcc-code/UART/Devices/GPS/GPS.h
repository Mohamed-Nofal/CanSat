/*
 * GPS.h
 *
 * Created: 1/21/2021 3:13:50 AM
 *  Author: mizo
 */ 


#ifndef GPS_H_
#define GPS_H_
#include "../../Interface/UART.h"
typedef struct gps_struct{
	float longitude;
	float latitude;
	char pp[13];
	int8_t NoS;
}GPS;

uint8_t readGPS(GPS *gps);




#endif /* GPS_H_ */