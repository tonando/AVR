
#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"

// #include "midi.h"
// #include "eeprom.h"
#include "util/delay.h"

#define ACH		3
#define B_SCALE	1
#define B_NOTE	0
#define B_TEMPO 2
// tx=4

#define note_on(a) 		midi_out(0x90,a,123)
#define note_off(a) 	midi_out(0x80,a,0)
void midi_out(char c, char a, char b);
void set_scale(uchar s);

bool next_note();
const uchar SCALES[8] /* PROGMEM */ = {
	0b01101110,	// ionian maj
	0b01011101,	// dorian min
	0b00111011,	// phrygian min
	0b01110110,	// lydian maj
	0b01101101,	// mixol maj
	0b01011011,	// aeolian min
	0b00110111	// locrian min
};

// arp
uchar b_note=22;
uchar c_note=b_note;

uchar c_scale=0;
uchar tempo=40;
uchar steps=8;

int main(void){
	BUTTON(B_NOTE);
	BUTTON(B_TEMPO);
	BUTTON(B_SCALE);
	
	uchar cnt=0;
	tiny_serial_init();

	while(1){
		cnt++;
		cnt&=15;
		
		uchar ain = adc_read(ACH);
		
		
		if(!cnt)
			next_note();
		else{
			if(!READ(B_NOTE))
				b_note = (ain>>2)+12;

			if(!READ(B_SCALE)){
				uchar tmp = ain>>5;
				if(!READ(B_TEMPO))
					steps = MAX(1,tmp);
				else
					set_scale(tmp);
			}
			if(!READ(B_TEMPO))
				tempo = ain;

		}

		delay_ms(2+(tempo));
	}

	return 0;
}
7
uchar s_cnt;
bool next_note(){

	note_off(c_note);
	
	// count
	c_note++;
	if(rbi(SCALES[c_scale],s_cnt++))
		c_note++;
	
	// scale end
	if(s_cnt >= steps){
		s_cnt = 0;
		c_note = b_note;
	}
	
	note_on(c_note);


	return true;
}
void set_scale(uchar s){
	if(c_scale==s) return;
	s_cnt=0;
	c_scale = s & 7;
	
	serial_write(0b11000000);
	serial_write(s);
	
}
uchar chan;
void set_chan(uchar c){
	chan=c;
}
void midi_out(char c, char a, char b){
	// c &= 0xf0;
	// c |=  chan;
	char test = 'K';
	
	serial_print("test\n\r");
	// serial_write(~test);
	// serial_write(c);
	// serial_write(~c);
	// serial_write(a);
	// serial_write(~a);
	// serial_write(b);	
	// serial_write(~b);	

}
