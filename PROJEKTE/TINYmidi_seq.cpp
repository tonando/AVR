
#include "Macros.h"
#include "stdlib.h"
#include "tiny_midi.h"
#include "shiftinout.h"

// #include "eeprom.h"

#define CLK		2
#define IN		1
#define OUT		3
#define PS		0

void midi_out(char c, char a, char b);
#define note_on(a) 		midi_out(0x90,a,123)
#define note_off(a) 	midi_out(0x80,a,0)

volatile uchar in,out;
// uchar shift(uchar);

int main(void){
	
	uchar cnt=0;
	
	// TCCR1 = 2;
	// TIMSK = B(TOIE1);
	
	// if(false)
		// tiny_serial_init(31);	// midi
	// else
		// tiny_serial_init();	// 9600
	bool first=true;
	initShift(CLK,IN, OUT, PS);
	in=shift(2);
	_delay_ms(3000);
	
	while(1){
		cnt++;
		cnt&=15;
		
		// serial_print("test\n");
		if(first){
			// out = ;
			_delay_ms(1800);
			shift(cnt&7);
		}
			
		// shift();
		
		if(!cnt) first = false;
	}

	return 0;
}
/* 
// ISR(TIM1_ovf_vect){
uchar shift(uchar out){
	
	OFF(PS);
	uchar result=READ(IN);
	
	uchar i=8;
	do{i--;
		result <<= 1;
		OFF(CLK);
      
	  if(rbi(out,i))
		ON(OUT);
	  else
		OFF(OUT);
       
		if(READ(IN))
			result |= 1;
		
		ON(CLK);
	}while(i);
	
	OFF(CLK);
	ON(PS);

	return result;
}
 */