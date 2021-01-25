/*
 * UART.c
 *
 * Created: 11/23/2020 2:42:09 PM
 *  Author: mizo
 */ 
#include "UART.h"

uint8_t initUART(){
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}
uint8_t sendByteUART(uint8_t data ){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
uint8_t sendStringUART(char *StringPtr){
	//&&*StringPtr != '\r'
	while(*StringPtr != '\0')
	{
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) );
		/* Put data into buffer, sends the data */
		UDR0 = *StringPtr;
		StringPtr++;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = *StringPtr;
	return STRING_SENT;
}
uint8_t receiveByteUART(){
	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};
	// Return received data
	return UDR0;
}
uint8_t *recieveStringUART(char *x){
	while ( !(UCSR0A & (1<<RXC0)) );
	while(UDR0 != '\0'&&UDR0 != '\r')
	{
		*x = UDR0;
		x++;
		while ( !(UCSR0A & (1<<RXC0)) );
	}
	*x = UDR0;
	return RECIEVED_BYTE;
}
uint8_t *recieveLineUART(char *x){
	while ( !(UCSR0A & (1<<RXC0)) );
	while(UDR0 != '\0'&&UDR0 != '\n')
	{
		*x = UDR0;
		x++;
		while ( !(UCSR0A & (1<<RXC0)) );
	}
	*x = UDR0;
	return RECIEVED_BYTE;
}