
#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"

// #include "midi.h"
// #include "eeprom.h"
#include "util/delay.h"

#define ACH		3
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define note_on(a) 		midi_out(0x90,a,123)
#define note_off(a) 	midi_out(0x80,a,0)
void midi_out(char c, char a, char b);

uint timeout[2];
uchar ain[2];
uchar l_ain[2];
bool note_on[2];

int main(void){
	// BUTTON(B_NOTE);
	// BUTTON(B_TEMPO);
	// BUTTON(B_SCALE);
	DDRB=1;
	
	uchar cnt=0;
	tiny_serial_init();
	
	while(1){
		cnt++;

		uchar chn = cnt&1;
		
		ain[chn] = adc_read(chn?3:1);
	
		l_ain[chn] = ain[chn];
	
		if(ain[chn] && !l_ain[chn]){
			midi_out(NOTE_ON,63+chn,MAX(ain[chn],20));
			note_on[chn]=true;
			timeout[chn] = 20000;
		}
		if(!ain[chn] && l_ain[chn]){
			midi_out(NOTE_OFF,63+chn,0);
			note_on[chn]=false;
			ON(0);
		}
		
		if(	note_on[chn])
			midi_out(0x0a,1,ain[chn]);
			
		if(timeout[chn]){
			timeout[chn]--;
			if(!timeout[chn]){
				midi_out(NOTE_OFF,63+chn,0);
				note_on[chn]=false;
			}
		}		
			
		if(	!note_on[0] && !note_on[1] )
			OFF(0);
		
	}

	return 0;
}

void midi_out(char c, char a, char b){
	// c &= 0xf0;
	// c |=  chan;
	// char test = 'K';
	
	// serial_print("test\n\r");
	// serial_write(~test);
	serial_write(c);
	// serial_write(~c);
	serial_write(a);
	// serial_write(~a);
	serial_write(b);	
	// serial_write(~b);	

}
