
#include <Macros.h>

// #define OCR_VAL BAUD_19200

#include "tiny_serial.h"

uchar chan = 0;
uchar decay=10;
uchar offset=36;
uchar ain = 0;
uchar lain = 0;
uchar cnote = 0;	

void midi_out(uchar s, uchar a, uchar b){
	serial_write(s+chan);
	serial_write(a);
	serial_write(b);
}

void set_ctrl(uchar c, uchar v){	midi_out(0xB0,c,v);}
void note_on(uchar n, uchar v){	midi_out(0x90,n,v);cnote=n;}
void note_off(uchar n){	midi_out(0x80,n,0); cnote=0;}
void drum(uchar,uchar);

uchar read_piezo(){
	uchar result =0;
	
	ain = adc_read(3);
	ain <<= 3;
	ain = constrain(ain,0,127);
	
	if(ain > 3 && ain < lain)
		result = ain;

	lain = ain;
		
	return result;
}

int main (void)
{ 
	tiny_serial_init();
	INPUT(3);
	
	uchar r=0;
	uchar p=0;
	uchar cnt=0;
	
	while(1){
		
		p = read_piezo();
		r = lfsr();
		
		if( p > (cnt>>3)){				
			if(cnote)
				note_off(cnote);
				
			note_on(offset+(r&3)*7,RANGE(p,1,127));
			cnt = p;
			
			// _delay_ms(60);
		}
		
		if(cnt){
			cnt--;
			delay_ms(decay);
		}
		else if(cnote) note_off(cnote);
			
	}
	
	return 0;
}
