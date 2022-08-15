#ifndef TINY_MIDI_h
#define TINY_MIDI_h

#include "tiny_serial.h"

#define CC_PITCH 0b11100000

static uchar chan = 0;

void midi_out(uchar s, uchar a, uchar b){

	if(s <= 224) s += chan;
	
	serial_write(s);
	serial_write(a);
	serial_write(b);
	
}

void set_ctrl(uchar c, uchar v){midi_out(176,c,v);}
void note_on(uchar n, uchar v){	midi_out(0x90,n,v);}
void note_off(uchar n){	midi_out(0x80,n,0);}

#endif