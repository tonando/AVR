
 GLUE(a,b) 		a.b
 EVAL(...) 		code
 MERGE(h,l) 	8+8 to 16 bit

 B(b)		
 sbi(b,n) 	
 cbi(b,n) 	
 rbi(b,n) 	
 fbi(b,n) 	

// MATH limits
 MIN(a,b) 			
 MAX(a,b) 			
 RANGE(a,n,x)	 	constrain
 ABS(a) 				
 CA(a,b,c) 			a == b +/- c
 
//	io registers
 ON(p) 			
 OFF(p)  		
 SET(p,v)		
 OUTPUT(p) 		
 INPUT(p) 		
 READ(p) 		
 IS_OUTPUT(p) 	

 BUTTON(p)  		pullup
 RESET 				jump to start



uint lfsr()
bool PRESSED(uchar p)

// Counting bits set (naive way)
uchar count_bits(uint v){

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

/* 
 STR1(x) #x
 STR(x) STR1(x)
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
			 CLK 	2
			 MOSI 	0
			 LATCH 	1
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
  TIMER0_OVF_vect	TIM0_OVF_vect
 #endif
 
 #ifndef TIMSK
  TIMSK TIMSK0
 #endif
 #ifndef TIMSK0
  TIMSK0 TIMSK
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
