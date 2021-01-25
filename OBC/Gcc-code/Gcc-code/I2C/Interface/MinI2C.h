/*
 * MinI2C.h
 *
 * Created: 11/22/2020 2:47:54 PM
 *  Author: mizo
 */ 


#ifndef MINI2C_H_
#define MINI2C_H_
#include <avr/io.h>
#include <math.h>
#include "../../config.h"
#define BITRATE	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR & 0b00000011)))

#define START_FAILED 42
#define ACK_RECIEVED 23
#define NACK_RECIEVED 24
#define SLA_W_FAILED 25
#define TWI_STOPPED 26
#define TRANSMITTION_FAILED 27
#define ACK 1
#define NACK 0
#define NO_PRESCALER_MATCH 28
#define BITRATE_SET 28

#include <stdint.h>
uint8_t initI2C(uint16_t prescaler);
uint8_t startI2C(uint8_t slave_address);
uint8_t stopI2C();
uint8_t writeI2C(uint8_t data);
uint8_t readI2C(uint8_t ack);
#endif /* MINI2C_H_ */