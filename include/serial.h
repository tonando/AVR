#ifndef serial_h
#define serial_h

#include "Macros.h"
#include "port_names.h"

#ifndef BAUD
#define BAUD 57600
#endif

#include <util/setbaud.h>

// #define print(s) txs((char*)" ## (s) ## ")
// #define println(s) print( ((s) ## \t) )
bool INITED;

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = 128 | (1 << UCSZ0) | (1 << UCSZ1);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}
void uart_put(uchar c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = (char)c;
}
// void uart_putchar(char c) {
    // UDR0 = c;
    // loop_until_bit_is_set(UCSR0A, TXC0); /* Wait until transmission ready. */
// }
 
char uart_getchar(void) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}
/* 
volatile byte rx_byte;
byte rx_buffer[8];
byte rx_bytes_ready;

void rx_clear(){
	rx_bytes_ready=0;
}

void initUART(ulong baudrate, bool x2){
	uint bauddiv = ((F_CPU+(baudrate*8L))/(baudrate*16L)-1);
	UBRRL = (unsigned char)bauddiv;
//	#ifdef UBRRH
	UBRRH = (unsigned char)bauddiv>>8;
//	#endif

    UCSRA = x2;
	// enable rx, turn on interrupt
	// 8 data bits, no parity, 1 stop bit
    UCSRB = (1<<TXEN) | _BV(RXCIE) | _BV(RXEN);//Enable transmitter
    UCSRC = (3<<UCSZ0) | _BV(UCSZ1);//N81
}

void tx(char s){
  if (UCSRB != (1<<TXEN))
	initUART();  
  
    while ( !( UCSRA & (1<<UDRE)) );
		UDR = s;
}

void txs(const char* str){
	do{
		tx(*str++);
	}while(str);
}

byte rx(){
	if(rx_bytes_ready){
		rx_bytes_ready--;
		return rx_byte;
	}else
		return '\0';
}

ISR(USART_RX_vect)
{
  // get the new byte
  rx_byte = UDR0;
  if(rx_bytes_ready == 8)
	rx_bytes_ready=0;
	
  rx_buffer[rx_bytes_ready] = rx_byte;
  rx_bytes_ready++;
}
 */
#endif 