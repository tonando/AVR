
#include <Macros.h>

// #define OCR_VAL BAUD_19200
#define AKEYS	3
#define AVOL	1

#include "tiny_serial.h"

uchar chan = 2;
uchar offset = 30;
uchar ckey = 0;

uchar atokey(uchar v){
	return (offset +((v)>>3));
}

void midi_out(uchar s, uchar a, uchar b){
	serial_write(s+chan);
	serial_write(a);
	serial_write(b);
}

void set_ctrl(uchar c, uchar v){
	midi_out(0xB0,c,v);
}

void note_on(uchar n, uchar v){
	ckey = n;
	midi_out(0x90,n,v);
}

void note_off(uchar n){
	ckey = 0;
	midi_out(0x80,n,0);
}


int main (void)
{ 

	tiny_serial_init();
	INPUT(AKEYS);
	INPUT(AVOL);

	uchar ain = 0;
	uchar lain = 0;
	
	while(1){
			
			ain = adc_read(AKEYS);
			
			if(!CA(ain,lain,20)){
			
				uchar key = atokey(ain);
				
				if(key != ckey){
				
					if(ckey)
						note_off(ckey);
						
					if(ain)
						note_on(key,127);
				}	

				_delay_ms(40);
			}
			
			lain = ain;	
	}
	
	return 0;
}
