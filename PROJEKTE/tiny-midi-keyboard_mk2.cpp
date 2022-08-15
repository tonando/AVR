
#include <Macros.h>
#include "tiny_serial.h"

// #define DEBUG

// #define OCR_VAL 102

#define CLOCK	2
#define DATA	1
#define PS		0


// bool altmode=true;
uchar chan = 0;
uchar volume=127;

uchar offset=48;	// absolute
uchar base=0;		// relative (12)

#define ANZ_SCALES		4
uchar scale=0;

const uchar SCALES[ANZ_SCALES][8] ={
	{3,2,1,1,3,2,3,2},	// pentatonic
	{2,2,1,2,2,2,1,2},	// major
	{2,1,2,2,1,2,2,2},	// minor
	{1,1,1,1,1,1,1,1}	// none
};
/*
const uchar SCALES[8] ={
	0b01101110,	// ionian maj
	0b01011101,	// dorian min
	0b01110110,	// lydian maj
	0b00111011,	// phrygian min
	0b01101101,	// mixol maj
	0b01011011,	// aeolian min
	0b00110111	// locrian min
};
*/
void midi_out(uchar s, uchar a, uchar b){
	
	if(s <= 224) s += chan;
	
	serial_write(s);
	serial_write(a);
	serial_write(b);
	
}
void note_on(uchar n, uchar v){	midi_out(0x90,n,v);}
void note_off(uchar n){	midi_out(0x80,n,0);}
uchar getKeys(){
	
	OFF(PS);
	uchar result=READ(DATA);
	
	uchar i=8;
	do{i--;
		

		result <<= 1;
		if(READ(DATA))
			result |= 1;
		
		ON(CLOCK);
		// _delay_us(50);
		OFF(CLOCK);
	}while(i);
	
	ON(PS);
	_delay_ms(1);

	return result;
}
void transpose(char s){
	chan=5;
	midi_out(255,0,0);

	if((offset + s > 16) && (offset + s < 100)){
		offset += s;
	}
	
	note_on(offset,volume);
	_delay_ms(200); 
	note_off(offset);
	chan=0;
}

uchar getNote(uchar n){
	uchar result = offset;
	
	// if(scale && n){
	if(n){

		// uchar sc=SCALES[scale-1];
		
		for(uchar i=0; i<n; i++){
			result += SCALES[scale][i];
		}
		
		// uchar i=n;
		// do{i--;
			// result++;
			// if( rbi(sc,i) ) 
				// result++;
		// }while(i);
		
	}
	// else
		// result+=n;
	
	return result;
}

void changeScale(){
	
	// if(altmode)
		scale++; 
	// else
		// scale--; 
	
	scale%=ANZ_SCALES; 
	
	chan=5;
	
	for(uchar i=0; i<8; i++){
		uchar v=volume - ((i&3)<<4);
		note_on(getNote(i),v);
		delay_ms(70+v); 
		note_off(getNote(i));
		// _delay_ms(50); 
	}
	
	midi_out(176,12,scale);
	
	chan=0;
}


int main (void)
{ 
	//todo: eeprom 
	
	INPUT(DATA);
	OUTPUT(CLOCK);
	OUTPUT(PS);
	ON(PS);

	uchar cnt=0;
	
	uchar keys=0;
	uchar last_keys=0;
	
	uchar ain = 0;
	uchar lain = 0;
	
	// start modifier
	ain = lain = adc_read(3)>>5;
	
	if(ain)
		tiny_serial_init(31);	// midi
	else
		tiny_serial_init(102);	// 9600
	
	while(1){
			cnt++;
			
			keys = getKeys();

			uchar changed = last_keys ^ keys;
			
			uchar i = 8;
			do{i--;
				if(rbi(changed,i)){
				
					if(rbi(keys,i))
						note_on(getNote(i),volume);
					else
						note_off(getNote(i));
				}
				
			}while(i);
			
			last_keys = keys;
		
		
			
			if(!(cnt&7)){
				ain = adc_read(3) >> 5; 
 				
				if(!ain && lain){
				
					// test
					//midi_out(176,16,lain);
					
					// a buttons
					switch(lain){
						// case 5: altmode ? transpose(12) : transpose(1); break;
						// case 7: altmode ? transpose(-12) : transpose(-1); break;
						 // case 3: altmode = !altmode; break;	// start song midi_out(250,0,0); 
						// case 1: changeScale(); break;
						case 5: transpose(12); break;
						case 7: transpose(-12); break;
						case 3: transpose(1); base++; if(base == 12){ base=0; transpose(-12); } break;	// start song midi_out(250,0,0); 
						case 1: changeScale(); break;
						default:break;
					}
					
					_delay_ms(50);
				}
				
				lain=ain;
			}
			
	}
	
	return 0;
}
