/*
 * DHT.h
 *
 * Created: 1/21/2021 7:41:21 PM
 *  Author: mizo
 */ 


#ifndef DHT_H_
#define DHT_H_
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "../../config.h"
typedef struct DHT_struct{
	float humi;
	float temp[2];
}DHT;
void readDHT(uint8_t pin,DHT* dht);


#endif /* DHT_H_ */