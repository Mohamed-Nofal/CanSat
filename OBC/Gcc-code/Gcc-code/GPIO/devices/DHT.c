/*
 * DHT.c
 *
 * Created: 1/21/2021 7:41:38 PM
 *  Author: mizo
 */ 
#include "DHT.h"
#define MAX_TIMINGS	85
 
int data[5] = { 0, 0, 0, 0, 0 };
 
void readDHT(uint8_t pin,DHT* dht)
{
	uint8_t laststate	= 1;
	uint8_t counter		= 0;
	uint8_t j			= 0, i;
 
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;
 
	/* pull pin down for 18 milliseconds */
	DDRD |= 1<<pin;
	PORTD &= ~(1<<pin);
	_delay_ms( 18 );
 
	/* prepare to read the pin */
	DDRD &= ~(1<<pin);
 
	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ )
	{
		counter = 0;
		while ( (PIND &= (1<<pin))>>pin == laststate )
		{
			counter++;
			_delay_us( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = (PIND &= (1<<pin))>>pin;
 
		if ( counter == 255 )
			break;
 
		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) )
		{
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
		}
	
 
	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
	{
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 )
		{
			h = data[0];	// for DHT11
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 )
		{
			c = data[2];	// for DHT11
		}
		if ( data[2] & 0x80 )
		{
			c = -c;
		}
		float f = c * 1.8f + 32;
		(*dht).temp[0]=c;
		(*dht).temp[1]=f;
		(*dht).humi=h;
		
	}
	}
}