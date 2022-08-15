#ifndef Notes_h
#define Notes_h

#include "Macros.h"
#include <math.h>
#include <ctype.h>

/*
	piano = A0-C8
	Midi = 0	127			
	Note = C-1	G9
	Hz	= 8.18	12544
*/

#define CALC_FREQS 1			// calc or progmem (comment)

#define F_CONST 1.059463	// semitone to frequency constant
#define A_TUNING 440 
#define MIDI_A4 69
#define MIDI_FIRST 32		// first note in progmem
#define MIDI_LAST 127		// last note in progmem 


#ifndef CALC_FREQS
	#include <avr/pgmspace.h>
	const uint16_t FREQS[12] PROGMEM= {220,233,247,262,277,294,311,330,349,370,392,415};

#endif

// 0 = halfnote, 1 = fullnote	LSB first
const uint8_t SCALES[8] /* PROGMEM */ = {
	0b01101110,	// ionian maj
	0b01011101,	// dorian min
	0b00111011,	// phrygian min
	0b01110110,	// lydian maj
	0b01101101,	// mixol maj
	0b01011011,	// aeolian min
	0b00110111	// locrian min
};
/* 
uint noteToFreq(int n){
	float a = 1.0594631; // 1.059463094359
	char b = n - 32;
	uint f = 440 * pow(a,b);
	return constrain(f,45,18000);
}
 */
uint midiToFreq(uchar m){

	uint result=0;
	
	#ifdef CALC_FREQS
		uchar o = MIDI_A4;
		o -= (m & 128);
		result = (uint) ( (float)A_TUNING * pow(F_CONST,o) );
		
	#else
		m -= MIDI_FIRST;
		result = pgm_read_word_near( FREQS[m%12] );
		if(m >= 12)
			result *= (m / 12);
		
	#endif
	
	return result;
}

uchar freqToMidi(uint f){

	uchar result=0;
	
	#ifdef CALC_FREQS

	#else

	#endif
	
	return result;
}

bool inScale(uchar note){
	return true;
}
/*


char strToMidi(char* s){
	char result=-1;
	char tone = tolower(s[0]);
	if(isalpha(tone)){
		
	}
	
	return result;
}


uint midiToFreq(uchar m){
	#ifdef CALC_FREQS
		char o = MIDI_A4;
		o -= (m & 128);
		return (uint) ( (float)A_TUNING * pow(F_CONST,o) );
		
	#else
		uint result=0;
		m -= MIDI_FIRST;
		return pgm_read_word_near( freqvals[m] );
		
	#endif
}

*/
/* 
char strToMidi(char* s){
	char result=-1;
	char tone = toupper(s[0]);
	if(isalpha(tone)){
		bool sharp = (s[1] == '#') ? 1 : 0;
		uchar oct = s[1 + sharp];

		if(isdigit(oct) && tone > 64 && tone < 72){
			result = (tone+4) + 12 * (oct-4); // DEFEKT (halbtoene)
		}
	}
	
	return result;
} */
/*
char[] midiToStr(uchar n){
	char result[3] = {0,0,0};
	
	return result;
}
*/
//uint16_t midiToTimerTicks(uint8_t);	// evtl. aus PROGMEM lesen
//uint16_t freqToTimerTicks(uint16_t);

// ???
//int8_t getTranspose(uint8_t b, int8_t t, uint8_t s);


#endif


/*
uint8_t freqToMidi(uint16_t f){
	uint8_t i=128;
	do{i--;
		if(freqvals[i] == f)
			break;
		if(freqvals[i-1] < f && i != 0)
			break;
	}while(i);
	
	return i;
}
uint16_t midiToTimerTicks(uint8_t m){
	return pgm_read_word_near(freqvals[m]);
}

uint16_t freqToTimerTicks(uint16_t f){
	uint8_t pre_vals[6] = {0,0,4,7,8,10};
	return (uint16_t) ( F_CPU / (1<<pre_vals[PRESCALE]) / f);
}

// base, transpose, scale
int8_t getTranspose(uint8_t b, int8_t t, uint8_t s){
	if(!t)
		return 0;
		
	uint8_t result=0;
	uint8_t max_offset = b;
	
	while(b != b+t)
	{
		int8_t o = s[ (t&127) ? 7-i&7 : i&7 ]; // get bit from scale
		
		result++;
		if(result == max_offset) 
			break;
		
		if(o)
			result++;
			
		if(result == max_offset) 
			break;
	}
	
	return (int8_t) result | (t&128);
}
*/