
#include <Macros.h>
#include "tiny_midi.h"

// #define DEBUG

// #define OCR_VAL 102

#define CLOCK	2
#define DATA	1
#define PS		0

uchar getNote(uchar n);


bool altmode=true;	// TODO: add buttons

uchar volume=127;

uchar offset=48;	// absolute
// uchar base=0;		// relative (12)

uchar on_keys=0;

uchar scale=3;
uchar mode=0;

/*
 ionian 
1 dorian
2  phrygian
3   lydian
4    mixolydian
5     aeolian / minor
6      locrian / weird
*/
#define ANZ_SCALES		8

const uchar SCALES[ANZ_SCALES] ={
	0b10010100,	// maj penta
	// 0b11001010,	// min penta
	
	0b01101110,	// maj	ionian 
	0b01011011,	// min
		// 0b01011101,	// dorian min
	// 0b01010101,	// 8n dim
	0b01101101,	// mixol maj
	0b00111011,	// phrygian min
	0b01101100,	// mixol min
	0b01110110,	// lydian maj
	0b01011110	// mel min
	// 0b01011011	// min 7b5
		// 0b01111111,	// whole
	// 0b01011011,	// aeolian min
	// 0b00101111,	// super locrian
	// 0b00110111	// locrian min
};


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

void all_off(){
	if(!on_keys) return;
	
	for(uchar i=0;i<8;i++){
		if(rbi(on_keys,i))
			note_off(getNote(i));
	}
}

void transpose(char s){
	all_off();
	
	chan=13;

	if((offset + s > 16) && (offset + s < 100)){
		offset += s;
	}
	
	note_on(offset,volume);
	_delay_ms(130); 
	note_off(offset);
	
	midi_out(176,112,offset);
	
	chan=0;
}

uchar getNote(uchar n){
	uchar result = offset;
	
	if(scale && n){

		uchar sc=SCALES[scale-1];
		
		// if(mode){
			// sc = 127 & ((sc >> mode) | (sc << (6-mode)));
			// sc |= SCALES[scale-1] & 128;
		// }
			
		uchar i=n;
		do{i--;
			result++;
			
			// penta
			if(rbi(sc,7))
				result++;
				
			if( rbi(sc,i) ) 
				result++;

		}while(i);
		
	}
	else
		result+=n;
	
	return result;
}

void changeScale(){

	all_off();
	
	scale += altmode ? 1 : -1;
	// if(altmode){
		// scale++;
		scale%=ANZ_SCALES; 
	// }		
	// else{
		// mode++; 
		// mode %= 6;
	// }
	
	chan=13;
	
	for(uchar i=0; i<8; i++){
		uchar v=volume - ((i&3)<<4);
		uchar n= (altmode) ? getNote(i) : getNote(7-i);
		// uchar n= getNote(i);
			
		note_on(n,v);
		delay_ms(75+v); 
		note_off(n);
		_delay_ms(10); 
	}
	
	midi_out(176,113,scale);
	
	chan=0;
}

void switch_mode(){
	altmode = !altmode; 
	// chan=5;
	// uchar n= altmode ? 90 : 80;
	// note_on(n,40);
	// _delay_ms(30); 
	// note_off(n);
	// chan=0;
}

int main (void)
{ 
	//todo: eeprom 
	
	INPUT(DATA);
	OUTPUT(CLOCK);
	OUTPUT(PS);
	ON(PS);

	uchar cnt=0;
	
	uchar last_keys=0;
	uchar keys=0;
	
	uchar ain = 0;
	uchar lain = 0;
	
	// start modifier
	ain = lain = adc_read(3)>>5;
	
	if(ain)
		tiny_serial_init(31);	// midi
	else
		tiny_serial_init();	// 9600
	
	while(1){
			cnt++;
			
			on_keys = keys = getKeys();

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
						case 5: altmode ? transpose(12) : transpose(1); break;
						case 7: altmode ? transpose(-12) : transpose(-1); break;
						 case 3: switch_mode(); break;	// start song midi_out(250,0,0); 
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
