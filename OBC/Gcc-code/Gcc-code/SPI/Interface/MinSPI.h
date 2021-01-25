/*
 * MinSPI.h
 *
 * Created: 11/22/2020 11:11:50 PM
 *  Author: mizo
 */ 


#ifndef MINSPI_H_
#define MINSPI_H_

#define DDR_SPI DDRB
#define DD_MISO PB4
#define DD_MOSI PB3
#define DD_SCK PB5
#define DD_SS PB2

#include <avr/io.h>
#include <stdint.h>
uint8_t SPI_MasterInit();
uint8_t SPI_MasterTransmit(uint8_t cData);
//uint8_t SPI_SlaveInit(uint8_t);
//char SPI_SlaveReceive(uint8_t);


#endif /* MINSPI_H_ */