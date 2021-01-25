/*
 * MinSPI.c
 *
 * Created: 11/22/2020 11:12:09 PM
 *  Author: sharkawi
 */ 
#include "MinSPI.h"
uint8_t SPI_MasterInit()
{
	// Set MOSI and SCK output, all others input
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	// Enable SPI, Master, set clock rate sck/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
uint8_t SPI_MasterTransmit(uint8_t data)
{
	// Start transmission
	SPDR = data;
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)));
}
// uint8_t SPI_SlaveInit(uint8_t)
// {
// 	/* Set MISO output, all others input */
// 	DDR_SPI = (1<<DD_MISO);
// 	/* Enable SPI */
// 	SPCR = (1<<SPE);
// }
// char SPI_SlaveReceive(uint8_t)
// {
// 	/* Wait for reception complete */
// 	while(!(SPSR & (1<<SPIF)))
// 	;
// 	/* Return Data Register */
// 	return SPDR;
// }