#ifndef SHIFT_PWM_h
#define SHIFT_PWM_h

#include <Macros.h>

// #define USE_LOG
// const uchar LOG_VALUES[] PROGMEM = {};
	
#define DATA  1
#define CLOCK 2
#define LATCH 3

#define PRESCALER 1
uchar NUM_LEDS=8;
uchar OCR_VAL = 200;
uchar LED_DIV=1;


uchar LEDS[8][2];

 uchar min_led_val=0;
 uchar max_led_val=255;

void each(void* _func(uchar*), uchar _mask);
 
void set_leds_range(uchar,uchar);
void randomize(uint _mask,uchar _min, uchar _max);
void randomize(uint _mask);
void randomize(uint _mask, uchar  );
void fade(uchar _led, uchar _v);
void set(uchar led, uchar val);
void fade_out(uchar _led, uchar _v);
void fade_in(uchar _led, uchar _v);
void fade_mask(uint _mask, uchar _v);
void set_mask(uint _mask, uchar _v);
void invert(uint _mask);
void add(uchar led, int _v);
bool is_fading(uchar);
uchar get(uchar);
uint get_idle();
uchar get_off();

uchar get(uchar _l){
	return MIN(LEDS[_l][0] * NUM_LEDS,255);
}
void set_led_val(uchar _n, uchar _i, uchar _v){
	#ifdef USE_LOG	
		_v = pgm_read_byte(&LOG_VALUES[_v]);
	#endif

		_v /= NUM_LEDS;
		_v=constrain(_v,min_led_val,max_led_val);

	LEDS[_n][_i] = _v;
}

uint get_idle(){
	uchar i=NUM_LEDS;
	uint result = 0;

	do{i--;
		if(LEDS[i][0] == LEDS[i][1])
			sbi(result,i);
			
	}while(i);
	
	
	return result;
}
uchar get_off(){
	uchar i=NUM_LEDS;
	uchar result = 0;

	do{i--;
		if(!LEDS[i][0])
			sbi(result,i);
			
	}while(i);
	
	
	return result;
}

bool is_fading(uchar i){
	if(LEDS[i][0] == LEDS[i][1])
		return false;
	else
		return true;
}
void shift_init(uchar _leds, uchar _maxval, uchar _ocr){
	
	cli();
	
	OCR_VAL = _ocr;
	NUM_LEDS = _leds;
	max_led_val = _maxval;
	
	DDRB |= _BV(DATA) | _BV(LATCH) | _BV(CLOCK);
	TCCR0B = _BV(CTC1) | PRESCALER;
	OCR0A = OCR_VAL;
	TIMSK |= _BV(OCIE0A);
	
	sei();
}
void shift_init(uchar _leds){shift_init(_leds,255/_leds,OCR_VAL);}
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
			
				fade(i,v);
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
			
			
				fade(i,v);
			
		}
	}while(i);
}
void randomize(uint _mask){
	randomize(_mask,0,max_led_val);
}

void set(uchar _l, uchar _v){
	if(_l >= NUM_LEDS) return;
	
		// _v = constrain(_v,0,max_led_val);
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
 void add(uchar _l, int _v){
	int result = get(_l);
	result += _v;
	result = constrain(result,min_led_val,max_led_val);
	fade(_l,(uchar)result);
 }

void blend(uint _mask){
	uchar i=NUM_LEDS;
	do{i--;
		// uchar m = _mask >> i;
		// if(!m) return;
		
		if(rbi(_mask,i) && is_fading(i)){
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
	// _v = constrain(_v,0,max_led_val);
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

volatile uchar sf_cnt;
volatile uchar sf_cbit;

SIGNAL(TIMER0_COMPA_vect){

	// led counter
	OFF(CLOCK); 
	if(LEDS[sf_cbit][0] > sf_cnt)
		ON(DATA);
	else
		OFF(DATA);
	ON(CLOCK); 

	sf_cbit++;

	// brightness counter
	if(sf_cbit >= NUM_LEDS){
		OFF(LATCH);
		ON(LATCH);

		sf_cbit=0;
		
		sf_cnt++;
		if(sf_cnt >= max_led_val)
			sf_cnt=0;

	}
	
}

#endif