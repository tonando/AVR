#ifndef MACROS_H
#define MACROS_H

#include <avr/io.h>
#include <stdbool.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// div
#define GLUE(a,b) (a) ## (b)	
#define EVAL(...) __VA_ARGS__
#define MERGE(h,l) 	( (h)<<8 | (l) )	// MDB | LSB

// bits
#define B(b)		_BV((b))
#define sbi(b,n) 	(b) |= _BV( (n) )
#define cbi(b,n) 	(b) &= ~(_BV( (n) ))
#define rbi(b,n) 	(((b) >> (n)) & 1)
#define fbi(b,n) 	(b) ^= _BV( (n) )
// #define SKIP(b,n) 	(!( (b) & (_BV( ((n)&7) )-1) ))	// odd, 4th, 8th, etc...

// MATH limits
#define MIN(a,b) 			(a)<(b)?(a):(b)
#define MAX(a,b) 			(a)>(b)?(a):(b)
#define ABS(a) 				(a)<0?(a)*-1:(a)
#define constrain(a,n,x)	(a)<(n)?(n):((a)>(x)?(x):(a))
#define RANGE(a,n,x)	 	constrain((a),(n),(x))
#define CA(a,b,c) 			( (a) >= ((b) - (c)) && (a) <= ((b) + (c))) ? 1 : 0	// circa: a ~= b (+/-) c

//	io registers
#define ON(p) 			( sbi( PORTB , ((p)) ))
#define OFF(p)  		( cbi( PORTB , ((p)) ))
#define SET(p,v)		( v? ON((p)) : OFF((p)))
#define FLIP(p)			( fbi( PORTB , ((p)) ))
#define OUTPUT(p) 		( sbi(DDRB,(p))  )
#define INPUT(p) 		( cbi(DDRB,(p))  )
#define READ(p) 		( rbi( PINB , (p)) )
#define IS_OUTPUT(p) 	( rbi(DDRB , ((p)) )

#define BUTTON(p)  		{ INPUT((p)); ON((p)); }
#define RESET 			{ cli(); asm("rjmp 0x0000"); }

//	power
/*
#define IDLE				MCUCR = B(SE) 
#define ADC_NOISE			MCUCR = B(SE) | B(SM0)
#define POWER_DOWN			MCUCR = B(SE) | B(SM1)
#define DISABLE_ADC			PRR	= B(PRADC)
#define DISABLE_USI			PRR	= B(PRUSI)
#define DISABLE_TIMER0		PRR	= B(PRTIM0)
#define DISABLE_TIMER1		PRR	= B(PRTIM1)
*/

typedef unsigned char uchar;
typedef uchar uch;
typedef unsigned int uint;
typedef unsigned long ulong;


// random
static inline uint16_t lfsr(){
  static uint16_t result = 0xACE1u;
  result = (result >> 1) ^ (-(result & 1u) & 0xB400u);
  return result;
}

// delay
static inline void delay_ms(ulong _ms){
	do{
		_delay_ms(1);
	}while(--_ms);
}
static inline void delay_us(ulong _ms){
	_ms >>= 4;
	do{
		_delay_us(16);
	}while(--_ms);
}

//	ADC 8
static inline uchar adc_read(uchar _c){
	ADMUX = (1 << ADLAR) | (_c);       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);

	//Start conversion
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 

	return (ADCH);
}

//	ADC 10
static inline uint adc_read10(uchar _c){
	ADMUX = (_c);       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);

	//Start conversion
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 

	return (ADC);
}
// utils
void blink(uchar _p, uchar _n){
	OUTPUT(_p);
	while(_n){
		ON(_p); 	_delay_ms(200);
		OFF(_p);	_delay_ms(200);
		_n--;
	}
	_delay_ms(200);
} 
/* 
*/
static void beep(uchar _f, uchar _r, uchar _l){

#ifndef BUZZ
	#define BUZZ PB2
	// OUTPUT(BUZZ);
#endif

	uint d = constrain(64-_f,2,64);
	uint n = (512 / d)*2;
	 d *= 32;
	
	// if(!_r) n *= 2;
	if(_l)	n *= _l;
	
	while(--n){
		fbi(PORTB,BUZZ);
		delay_us(d);
	}
	
	OFF(BUZZ);
	
	if(_r){
		_delay_ms(128);
		
		if(_r > 1)
			beep(_f,--_r,_l);
	}
}
static void beep(uchar _f, uchar _r){ beep(_f, _r, 0); }
static inline void beep(uchar _f){ beep(_f,0); }

/* 
static inline bool NEAR(int _a,int _b, int _s){
	if(_a >= (_b - _s) && _a <= (_b + _s))
		return true;
	return false;
}
static inline bool PRESSED(uchar p){
	if(rbi( PINB , p))
		return false;
		
	while(!rbi( PINB , p));
	
	return true;
}
 */
 
// Counting bits set (naive way)
static inline uchar count_bits(uint v){
	uchar c;
	for (c = 0; v; v >>= 1)
	  c += v & 1;

	return c;
}

//	Conditionally set or clear bits without branching
// f: condition,	m: bitmask,		w: var to change
static inline void IFSET(bool _f, uchar _m, uchar* _w){
	*_w ^= (-_f ^ *_w) & _m;
}
static inline uchar BIT(uint _v){
	uchar result = 0;
	while(_v){
		_v >>= 1;
		result++;
	}
	return result;
}
#ifdef PB4
// analog channel -> pin number 	vice-versa
static inline char PTOA(uchar _p){
	switch(_p){
		case PB2: return 1;
		case PB3: return 3;
		case PB4: return 2;
		case PB5: return 0;
		default: return -1;
	}
}
static inline char ATOP(uchar _a){
	switch(_a){
		case 1: return PB2;
		case 3: return PB3;
		case 2: return PB4;
		case 0: return PB5;
		default: return -1;
	}	
}
#endif
/* 
#define STR1(x) #x
#define STR(x) STR1(x)
asm ("pushl " STR(CS));

static inline uchar average(uchar _v){
	static uint _result = _v;
	
	_result += _v;
	_result >>= 1;
	
	return (uchar)(_result);
}
 */
/* 
// admux 0x0F = temperatur
inline uint adc_read (uchar ch){
	//DIDR0 = ADC0D 	// disable digital in
	//ADCSRA |= _BV(ADEN); // adc enable
    ADCSRA =  _BV(ADEN) | (0x02);	// enable adc, start, prescaler(32)
	ADMUX =   _BV(REFS0) |  _BV(ADLAR) | (ch); 				// left adjust, ch
	ADCSRA |= _BV(ADSC);
	
	while (ADCSRA & _BV(ADSC));					// Wait for it to finish
	//ADCSRA ^= _BV(ADEN); // adc disable
	return (ADCL | (ADCH << 8));
}


static inline void adc_setup (uchar _chan)
{
	
// Set the ADC input to PB2/ADC1, left adjust result
ADMUX |= _chan;
ADMUX |= (1 << ADLAR);

// Set the prescaler to clock/128 & enable ADC
ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

static inline int adc_read (void)
{
    // Start the conversion
    ADCSRA |= (1 << ADSC);

    // Wait for it to finish - blocking
    while (ADCSRA & (1 << ADSC));

    return ADCH;
} 
 */

 /* 
 #ifdef TCCR0B
static inline void pwm_setup (bool _chan)
{
    if(_chan)
		sbi(DDRB,1);
	else
		sbi(DDRB,0);
	// Set Timer 0 prescaler to clock/8.
	// At 9.6 MHz this is 1.2 MHz.
	TCCR0B |= (1 << CS00);
	// Set to 'Fast PWM' mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);

	// Clear OC0B output on compare match, upwards counting.
	if(_chan)
		TCCR0A |= (1 << COM0B1);
	else
		TCCR0A |= (1 << COM0A1);
}
 
static inline void pwm_write (uchar _val)
{
	if(rbi(TCCR0A,COM0B1))
		OCR0B = 255-_val;
	else
		OCR0A = 255-_val;
}
 #endif
 */
/* 
void initCTC(){
	// TCNT0 = 0; // initialize timer counter value to 0
	// TCCR0A = 0; // write 0 to timer 0 control registers
	// TCCR0B = 0;
	// TCCR0A |= (1 << COM0A0); //Timer0 in toggle mode (Table 11-2 of datasheet)
	// TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode (Table 11-5 )
	// TCCR0B |= (1 << CS00); // Prescaler table 11.6

	// OCR0A=27; //CTC Compare value (27 gives 153 khz)
}

inline void shift_out(uint8_t _data){
		#ifndef MOSI
			#define CLK 	2
			#define MOSI 	0
			#define LATCH 	1
		#endif

		uint8_t i=8;
		do{i--;
			PORTB &= ~(CLK | MOSI);
			if(rbi(_data,i))
				ON(MOSI);
			ON(CLK);
		}while(i);
		PORTB &= ~(CLK);
		OFF(LATCH);
		ON(LATCH);
}
 */
 
 
 #ifndef TIMER0_OVF_vect
 #define TIMER0_OVF_vect	TIM0_OVF_vect
 #endif
 
 #ifndef TIMSK
 #define TIMSK TIMSK0
 #endif
 #ifndef TIMSK0
 #define TIMSK0 TIMSK
 #endif
 
 
/*  
 
 
#if DEBUG_MODE
char* byteToString(byte b){
	char* result = (char*)"00000000";
	byte i=8;
	do{i--;
		result[i] = rbi(b,i) ? '1' : '0';
	}while(i);
	
	return result;
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif
 */

#endif
