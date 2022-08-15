
#include <Macros.h>
#include <math.h>

#define KPIN	0
#define SPIN	1
#define HPIN	2
#define TPIN	3

uchar KICK[1] = {0b10001000};
uchar SNAR[2] = {0b00001000,0b00100010};
uchar HHAT[4] = {0b10101010,0b11001100,0b10101110,0b01010101};
uchar TOMM[2] = {0b00100000,0b00001001};

uchar beat(uchar c){

	uchar i=c>>3;
	uchar result=0;
	
	if(rbi(KICK[i % sizeof(KICK)],c&7))
		sbi(result,0);
	if(rbi(SNAR[i % sizeof(SNAR)],c&7))
		sbi(result,1);
	if(rbi(HHAT[i % sizeof(HHAT)],c&7))
		sbi(result,2);
	if(rbi(TOMM[i % sizeof(TOMM)],c&7))
		sbi(result,3);
		
	return result;
}

int main (void)
{ 
	uint ain=0;
	uchar cnt=0;
	uchar output=0;
	
	OUTPUT(KPIN);
	OUTPUT(SPIN);
	OUTPUT(HPIN);
	OUTPUT(TPIN);
	
	// TCCR0A |= _BV(WGM01);
	// TCCR0B = 2;
	// OCR0A = 40;
	// TIMSK0 |= _BV(OCIE0A);

	// sei();
	
    while (1) {
		
		output = beat(cnt);
		PORTB = output;
		
		ain = 515- (adc_read10(2)>>1);
		
		delay_ms(ain);
		
		cnt++;
	}	
}

ISR(TIM0_COMPA_vect){
	
}