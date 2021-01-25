/*
 * UART.h
 *
 * Created: 11/23/2020 2:41:28 PM
 *  Author: mizo
 */ 


#ifndef UART_H_
#define UART_H_

#define RECIEVED_BYTE 42
#define STRING_SENT 43

#include "../../config.h"
#include <avr/io.h>
#include <stdint.h>
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

uint8_t initUART();
uint8_t sendByteUART(uint8_t data );
uint8_t sendStringUART(char *StringPtr);
uint8_t receiveByteUART();
uint8_t *recieveStringUART(char *x);
uint8_t *recieveLineUART(char *x);

#endif /* UART_H_ */