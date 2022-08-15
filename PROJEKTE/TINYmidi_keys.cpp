
#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"

// #include "midi.h"
// #include "eeprom.h"
// #include "util/delay.h"

#define AKEY		3
#define AVOL		2

// #define note_on(a) 		midi_out(0x90,a,123)
// #define note_off(a) 	midi_out(0x80,a,0)
void midi_out(char c, char a, char b);
void set_scale(uchar s);

const uchar SCALES[8] /* PROGMEM */ = {
	0b01101110,	// ionian maj
	0b01011101,	// dorian min
	0b00111011,	// phrygian min
	0b01110110,	// lydian maj
	0b01101101,	// mixol maj
	0b01011011,	// aeolian min
	0b00110111	// locrian min
};

char c_scale=-1;

int main(void){
	
	uchar cnt=0;
	tiny_serial_init();

	while(1){
		cnt++;
		cnt&=15;
		
		serial_print("test\n");
	//	serial_write(cnt+48);
	//	uchar ain = adc_read(ACH);

	}

	return 0;
}