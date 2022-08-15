// alt
#include <delay.h>
#include <Macros.h>
#include <Anal.h>

#define DSIZE 64

#define BAUD 57600
#include <util/setbaud.h>

uchar cp;
// uchar data[32][2];
uint data[DSIZE];
uchar cb=1;
uchar del = 40;

// void addByte(uchar b);
void addByte(uint b);
void clear();
void init();
void shift();
void uart_putchar(char c);

int main (void){
	init();
	analInit(0);
	
	sei();
	
	// delay(10);
	// uart_putchar('o');
	
	while( 1 ) {
	
		PORTB ^= _BV(5);
		shift();
		
		if(!rbi(PIND,4))
			clear();
		if(!rbi(PIND,5))
			del=analRead(0);
	}
	return 0;
}

void init(){
DDRB = 0xff;
DDRD &= ~3;
PORTD |= 0xff<<2;
	PORTB = 1;

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    // UCSRB = (1<<TXEN) | _BV(RXCIE) | _BV(RXEN);//Enable transmitter
    UCSR0C = 128 | (1 << UCSZ00) | (1 << UCSZ01);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0)| _BV(RXCIE0) ;   /* Enable RX and TX */
}

// void addByte(uchar b){
void addByte(uint b){
	cb ^= 1;

	if(cp<DSIZE){
		// data[cp][cb] = b;
		if(cb){ 
			data[cp] <<= 8;
			data[cp] |= b;
			cp++;
		}
		else
			data[cp] = b>>8;
	// if(cb)
		// cb++;
}
}

void clear(){
	cp=0;
	cb=1;
	del=1;
	data[cp]=0;
}

void shift(){
	// for(uchar i=0; i<=cp; i++){
	for(int i=0; i<=31250; i++){
		// uchar b=2;
		// do{b--;
			// uchar j=8;
		
			uchar j=16;
			ON(1);
			do{j--;
				// if(rbi(data[i][b],j))
				// if(rbi(data[i],j))
					// ON(1);
				// else
					// OFF(1);
				
				if(_BV(j) >= i)
				OFF(1);
					
				OFF(2);
				ON(2);
			}while(j);
		// }while(b);
		//next byte
		OFF(2);
		OFF(0);
		ON(0);
		// delay(del);
		
	}
	
	
}

void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}

ISR(USART_RX_vect)
{
  // get the new byte
	char rx_byte = UDR0;
	if(rx_byte == 65)
		del=analRead(0);
	if(rx_byte == 13)
		clear();
	
	addByte(rx_byte);
}
