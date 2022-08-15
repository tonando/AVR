#ifndef SHIFT_FADE_h
#define SHIFT_FADE_h

#include <Macros.h>

// #define USE_LOG
#define PRESCALER 1

#define OVF_VECT TIM0_OVF_vect
#define ALL 0xffff

#define DATA  1
#define CLOCK 2
#define LATCH 3

#ifndef NUM_LEDS
	#define NUM_LEDS 8
#endif


#ifdef USE_LOG
#include <avr/pgmspace.h>
const uchar LOG_VALUES[] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x26, 0x27, 0x29, 0x2B, 0x2C,
0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x43, 0x45, 0x47, 0x4A, 0x4C, 0x4F,
0x51, 0x54, 0x57, 0x59, 0x5C, 0x5F, 0x62, 0x64, 0x67, 0x6A, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7C,
0x7F, 0x82, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x97, 0x9A, 0x9C, 0x9F, 0xA2, 0xA5, 0xA7, 0xAA,
0xAD, 0xAF, 0xB2, 0xB4, 0xB7, 0xB9, 0xBB, 0xBE, 0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
0xD0, 0xD2, 0xD3, 0xD5, 0xD7, 0xD8, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xED, 0xEE, 0xEF, 0xEF, 0xF0, 0xF1, 0xF1, 0xF2,
0xF2, 0xF3, 0xF3, 0xF4, 0xF4, 0xF5, 0xF5, 0xF6, 0xF6, 0xF6, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF};
#endif

static uchar min_led_val=0;
static uchar max_led_val=255;

void set_leds_range(uchar,uchar);

void randomize(uint _mask,uchar _min, uchar _max);
void randomize(uint _mask);
void randomize(uint _mask, uchar  );

void fade(uchar _led, uchar _v);
void fade_mask(uint _mask, uchar _v);
void fade_out(uchar _led, uchar _v);
void fade_in(uchar _led, uchar _v);

void set(uchar led, uchar val);
void set_mask(uint _mask, uchar _v);

void invert(uint _mask);
void add(uchar led, int _v);

bool is_fading(uchar);
uint get_idle();
// void clear();

uchar MAX_VALUE = 255;	
uchar bits_per_led=8;
uchar LEDS[NUM_LEDS][2];

bool smooth = true;
volatile uchar sf_cnt;
volatile uchar sf_cbit;
volatile uchar blend_cnt;

void set_led_val(uchar _n, uchar _i, uchar _v){
	#ifdef USE_LOG	
		_v = pgm_read_byte(&LOG_VALUES[_v]);
	#endif

	// if(_v){
		
		_v=constrain(_v,min_led_val,max_led_val);

		// scale
		if(bits_per_led != 8){
				
			// uchar v=_v;
			// uchar m= _BV(bits_per_led-1);
			
			if(_v > 20)
				_v >>= (8-bits_per_led);
			
			// if(_v && !v)
				// v = 1;
			 _v=MIN(_v,MAX_VALUE);
			 // _v=MAX(_v,_BV(bits_per_led));

		}
	// }
		
	LEDS[_n][_i] = _v;
}

uint get_idle(){
	uchar i=NUM_LEDS;
	uchar result = 0;

	do{i--;
		if(LEDS[i][0] == LEDS[i][1])
			sbi(result,i);
			
	}while(i);
	
	return result;
}
void invert(uint _mask){
	
	uchar i=NUM_LEDS;

	do{i--;
		uchar m = _mask >> i;
		if(!m) return;
		if(m & 1){
			uchar val = MAX_VALUE - LEDS[i][0];
			if(smooth)
				fade(i,val);
			else
				set(i,val);
		}
	}while(i);
}

bool is_fading(uchar i){
	if(LEDS[i][0] == LEDS[i][1])
		return false;
	else
		return true;
}
void shift_init(uchar _bits){
	
	sf_cnt=sf_cbit=0;
	_bits&=7;
	
	MAX_VALUE = 255 >> (8-_bits);
	MAX_VALUE = MAX(32,MAX_VALUE);
	
	DDRB |= _BV(DATA) | _BV(LATCH) | _BV(CLOCK);
	// ON(LATCH);
	
	TCCR0B = PRESCALER;
	TIMSK |= _BV(TOIE0);
	sei();
	// TCCR0B = _BV(CTC1)| PRESCALER;
	// OCR0A = 4;
	// TIMSK |= _BV(OCIE0A);
	
}
void shift_init(){shift_init(8);}
void randomize(uint _mask, uchar _r){
	
	uchar i = NUM_LEDS;
	do{i--;
		// if(rbi(_mask,i)){
		uchar m = _mask >> i;
		if(!m) return;
		if(m & 1){
			uchar v = 0xff & lfsr();
			v = constrain(v,LEDS[i][0]-_r, LEDS[i][0]+_r);
			
			if(smooth)
				fade(i,v);
			else
				set(i,v);
		}
	}while(i);
	
}
void randomize(uint _mask,uchar _min, uchar _max){
	
	uchar i = NUM_LEDS;
	do{i--;
		// if(rbi(_mask,i)){
		uchar m = _mask >> i;
		if(!m) return;
		if(m & 1){
			uchar v = 0xff & lfsr();
			if(v>_max)
				v%=_max;

			if(v < _min){
				v+=_min;
				v=MIN(v,_max);
			}
			
			if(smooth)
				fade(i,v);
			else
				set(i,v);
		}
	}while(i);
}
void randomize(uint _mask){
	randomize(_mask,0,MAX_VALUE);
}
void opposite(uchar _a, uchar _b){
	set(_a, 255 - LEDS[_b][0]);
}
void set(uchar _l, uchar _v){
	if(_l >= NUM_LEDS) return;
	
		// _v = constrain(_v,0,MAX_VALUE);
	_l = (NUM_LEDS-1)-_l;
	
	set_led_val(_l,0,_v);
	set_led_val(_l,1,_v);
	
}
void set_mask(uint _mask, uchar _v){
	
	uchar i = NUM_LEDS;
	do{i--;
	
		uchar m = _mask >> i;
		if(!m) return;
		// if(rbi(_mask,i)){
		if(m & 1)
			set(i,_v);
	}while(i);
	
}
/* void add(uint mask, char _v){
	uchar i=NUM_LEDS;
	do{i--;
		uchar m = _mask >> i;
		if(!m) return;
		
		if(m&1){

			int result = LEDS[i][0];
			result += _v;
			result = constrain(result,0,MAX_VALUE);
			set(i,((uchar)result));
		}
	}while(i);
}
 */
 void blend(uint _mask){
	uchar i=NUM_LEDS;
	do{i--;
		// uchar m = _mask >> i;
		// if(!m) return;
		
		if(rbi(_mask,i)){
			if(LEDS[i][0] != LEDS[i][1]){
				if(LEDS[i][0] < LEDS[i][1])
					LEDS[i][0]++;
				else
					LEDS[i][0]--;
				
			}
		}
	}while(i);	
}
void fade_out(uchar _l, uchar _v){			
	set(_l,_v);
	fade(_l,0);
}
void fade_in(uchar _l, uchar _v){			
	set(_l,0);
	fade(_l,_v);
}
void fade(uchar _l, uchar _v){			

	_l = (NUM_LEDS-1)-_l;
	// _v>>=DIVIDER;
	// _v = constrain(_v,0,MAX_VALUE);
	set_led_val(_l,1,_v);
}
void fade_mask(uint _mask, uchar _v){
	uchar i=NUM_LEDS;
	do{i--;
		uchar m = _mask >> i;
		if(!m) return;
		if(m&1)
			fade(i,_v);
	}while(i);	
}
void set_leds_range(uchar _l,uchar _h){
	if(_h<=_l) return;
	
	max_led_val = _h;
	min_led_val = _l;
}

ISR(TIMER0_OVF_vect){
// ISR(TIMER0_COMPA_vect){
	/* 
		// led 
		sf_cbit++;
		if(sf_cbit >= NUM_LEDS){
			
			
			// led value
			sf_cnt++;
			if(sf_cnt >= MAX_VALUE){
				sf_cnt=sf_cbit=0;
			}
		
			OFF(LATCH);
			ON(LATCH);
			sf_cbit = 0;
		}
	 */
	
		// uchar _current_led;	
		
		// shift out
		sf_cbit=NUM_LEDS;
		
		do{ sf_cbit--;
			
			uchar _current_led = LEDS[sf_cbit][0];
		  
			OFF(CLOCK); 
	#ifdef INVERTED
			if(_current_led < sf_cnt)
				OFF(DATA);
			else
				ON(DATA);

	#else
			if(_current_led > sf_cnt)
				ON(DATA);
			else
				OFF(DATA);
	#endif	  
			ON(CLOCK); 
			 
		}while(sf_cbit);

		PORTB &= ~(_BV(LATCH) | _BV(CLOCK)); 
		ON(LATCH);
	// }
		
		sf_cnt++;
		if(sf_cnt >= MAX_VALUE)
			sf_cnt=0;


}

#endif