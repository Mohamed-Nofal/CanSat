/*
 * CFile1.c
 *
 * Created: 11/22/2020 2:47:17 PM
 *  Author: mizo
 */ 
#include "MinI2C.h"

uint8_t initI2C(uint16_t prescaler){
	switch(prescaler){
		case 1 :
			TWSR = 0b00000000;
			break;
		case 4 :
			TWSR = 0b00000001;
			break;
		case 16 :
			TWSR = 0b00000010;
			break;
		case 64 :
			TWSR = 0b00000011;
			break;
		default:
			return NO_PRESCALER_MATCH;
	}
	TWBR = BITRATE;
	return BITRATE_SET;
};


uint8_t startI2C(uint8_t slave_address){
	/*
		TWEN : enables two-wire
		TWSTA : starts two-wire
		TWINT : clears the two-wire interrupt flag
	*/
	
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
	// wait till TWI finishes the operation
	while (!(TWCR & (1<<TWINT)));
	// check for error 
	if (TWSR & 0xF8 != 0x08) return START_FAILED;
	// if no error write slave address
	TWDR = slave_address;
	// enable TWI and clear TWINT
	TWCR = (1<<TWEN)|(1<<TWINT);
	// wait till TWI finishes the operation
	while (!(TWCR & (1<<TWINT)));
	// check for results
	return TWSR & 0xF8 == 0x18?ACK_RECIEVED:TWSR & 0xF8 == 0x20? NACK_RECIEVED : SLA_W_FAILED;
	
	
}

uint8_t stopI2C(){
	// enable two-wire, send stop and clear TWINT
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					
	while(TWCR & (1<<TWSTO));								
	return TWI_STOPPED;
}

uint8_t writeI2C(uint8_t data){
	// write data to two-wire data register
	TWDR = data;
	// enable TWI and clear TWINT
	TWCR = (1<<TWEN)|(1<<TWINT);
	// wait till TWINT finishes the operation
	while (!(TWCR & (1<<TWINT)));
	// check for errors
	return TWSR & 0xF8 == 0x18?ACK_RECIEVED:TWSR & 0xF8 == 0x20? NACK_RECIEVED : TRANSMITTION_FAILED;

};

uint8_t readI2C(uint8_t ack){
	// enable two-wire, send acknowledge and clear TWINT
	TWCR=(1<<TWEN)|(1<<TWINT)|(ack<<TWEA);
	// wait till TWI finishes the operation
	while (!(TWCR & (1<<TWINT)));
	// return the read data
	return TWDR;
};

