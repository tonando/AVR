#define T2313_h
#define TINY2313

#define UCSR0A UCSRA
#define UCSR0B UCSRB
#define UBRR0H	UBRRH
#define UBRR0L	UBRRL
#define U2X0 U2X 
#define UDR0 UDR
#define UDRE0 UDRE
#define UCSR0C UCSRC
#define UCSZ01 UCSZ1
#define UCSZ00 UCSZ0
#define RXC0 RXC
#define RXCIE0 RXCIE
#define RXEN0 RXEN
#define TXCIE0 TXCIE
#define TXEN0 TXEN

#define USART_RX_vect	USART_RXC_vect
#define USART_TX_vect	USART_TXC_vect

#include <Macros.h>