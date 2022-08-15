#ifndef Timers_h
#define Timers_h

#include <Macros.h>
// #include <avr/interrupt.h>

// FALSCH ?
#define STOPPED 		0
#define PRE_1 			1
#define PRE_8 			2
#define PRE_64 			3
#define PRE_256 		4
#define PRE_1024 		5	
#define PRE_EXT_FALLING 6
#define PRE_EXT_RISING 	7

//#define MODE_NORMAL 	0
#define MODE_COMPARE 	(0x01 << 3)
#define MODE_PWM 		(0x02 << 3)
#define MODE_FAST_PWM 	(0x03 << 3)

#define OC_INV 		0x01 << 5
#define OC_CLEAR 	0x02 << 5
#define OC_SET 		0x03 << 5

#define PWM8 	0x01
#define PWM9 	0x02
#define PWM10 	0x03

// #define OVERFLOW OVF
// #define COMPARE CMP
// #define T_INT(t,event) ISR(TIMER ## t ## _ ## event ## _vect)
// static bool CONFIGURED=false;
static uchar MODE = 0;
static uchar PRESCALER=1;

void pwm(int _v);
void setupPwm();
uint getPrescaler(byte p){
	switch(p){
		case PRE_1: return 1; break;	
		case PRE_8: return 8; break;	
		case PRE_64: return 64; break;
		case PRE_256: return 128; break;
		case PRE_1024: return 1024; break;
		default: return 0; break;
	}
}
// frequenz, clock mhz, PRE_scaler
uint freqToTicks(uint f, byte c, byte p){
	ulong result = 1000000UL * c;
	result /= getPrescaler(p);
	result /= f;
	return (uint)result;
}
uint msToTicks(uint m, byte c, byte p){
	ulong result = 1000000UL * c;
	result /= getPrescaler(p);
	float d = (float)result / 1000.0;
	result = (ulong)((float)m * d);
	return (uint)result;
}
void setupPwm(){
	TCCR0A = MODE_PWM;
	TCCR0B = 1;
	
	MODE=MODE_PWM;
	// CONFIGURED=true;
}
void pwm(int _v){
	if(MODE!=MODE_PWM)
		setupPwm();
	OCR0A = _v;
}

/*
void initTimer(byte t, byte m){
	switch(t){
		case 0:
			TCCR0B = m;
		break;
		case 1:
			TCCR1B = m;
		break;
		case 2:
#ifdef TCCR2		
			TCCR2B = m;
#endif			
		break;
		}
}

void setTimerFreq(byte t,uint16_t f){
	byte *tm;
	switch(t){
		case 0:
			tm = &OCR0A;
		break;
		case 1:
			tm = &OCR1A;
		break;
#ifdef TCCR2		
		case 2:
			tm = &OCR2A;;
		break;
#endif			
		}
	
	if(f < 256){}
	else
		
}
*/
/*
typedef struct timer_struct{
	byte id : 2;
	byte prescaler : 3;
	byte mode : 3;
	uint counter;	//TCNT1H TCNT1L
	uint compare;
	void (*onMatch)(void);
	void (*onOverflow)(void);
	
	uint overflows;
		//TIMSK = _BV(OCIE1A) (1<<TOIE0);
	
	void init(byte p){
		cli();
		getReg(id) = getMode(mode) | prescaler;
		sei();
	};
}tm1,tm2,tm3;
ISR(TIMER1_OVF_vect){ tm1.onOverflow(); }
*/
/*
 f = cf / p 
 o = ttt / tr2 
 rtt = t - (o * tr2) 
 rt = ttt / f 
 nf = f / ttt  

 f = Frequency 
 cf = System Clock Frequency 
 p = Prescaler Clock Value 
 o =Overflow Count 
 tr = Timer Resolution 
 ttt = Total Timer Ticks 
 rtt = Remainder Timer Ticks 
 rt = Real Time 
 nf = New Frequency
 
*/

#endif