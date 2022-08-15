#ifndef RF433_H
#define RF433_H

#define SEND_PIN 0
#define IN_PIN 1

#define PRESCALER 2 	// 1ms = 3
#define TIMER_OCR 32 	// 1ms = 3

// delays
#define DEL_COND 8
#define DEL_OFF 1
#define DEL_MIN 5
#define DEL_MID 30
#define DEL_MAX 80
#define DEL_STARTSTOP	150
#define TIMEOUT 200

#define DEL_HIGH 6
#define DEL_LOW	 20
// #define DEL_HIGH ((DEL_MIN + DEL_MID) / 2)
// #define DEL_LOW ((DEL_MAX + DEL_MID) / 2)

// timer
// volatile ulong millis;
volatile uchar ticks;
volatile uchar last_ticks;
// volatile uchar receiving_bits;

volatile bool sending;
volatile bool receiving;
// volatile bool polarity;
// bool stop_tx;

// byte
#define MSG_LEN	4	// bits
volatile uchar bcnt = 0;
volatile uchar buffer;
// volatile uchar last_byte;
// volatile bool listening;

void init_rf(bool _receiver);
void init_timer(uchar);
void timer_on(){TCCR0B = PRESCALER;	TCNT0 = 0;}
void timer_off(){TCCR0B = 0;}
bool get_bit(uint);
void tx(uchar,uchar);
void reset_buffer();
bool is_valid(uint);

void init_rf(){
	
	init_timer(TIMER_OCR);

#ifdef IN_PIN	
	INPUT(IN_PIN);

	GIMSK = B(INT0);
	MCUCR = 1; // any edge		
#endif

#ifdef SEND_PIN	
	OUTPUT(SEND_PIN);
#endif
	
	sei();
	
}
void init_timer(uchar _ocr){
	TIMSK0 |= _BV(OCIE0A);
	OCR0A = _ocr;
}
/* 
bool is_valid(uint _d){
	
	if(_d < DEL_MIN)
		return false;
	if(_d > DEL_MAX)
		return false;
		
	return true;
}
bool get_bit(uint _d){
	if(_d < DEL_MID)
		return true;
	else
		return false;
}
 */
void reset_buffer(){
	bcnt = 0;
	buffer = 0;
	// del_cnt = 0;
	// last_ticks = 0;
	millis=0;////////
}
/*
 void tx(uchar b, uchar r){
	
	// stop_tx = s;
	// ON(SEND_PIN);
	// OFF(SEND_PIN);
	// delay_ms(DEL_STARTSTOP);
 	do{		
		
		for(uchar i=0;i<8;i++){
			ON(SEND_PIN);
			
			if(rbi(b,i))
				delay_ms(DEL_HIGH);
			else
				delay_ms(DEL_LOW);
			
			OFF(SEND_PIN);
			delay_ms(DEL_OFF);
			
		}
		
	}while(--r);
	 */
	// delay_ms(DEL_STARTSTOP);
	
	// reset_buffer();

	// buffer=b;
	// sending = true;
	// TCNT0=0;
	// TCCR0B = PRESCALER;

}


SIGNAL(INT0_vect){
	
	bool pin_high = READ(IN_PIN);
	
	uint d = ticks;
	ticks=0;

	if(d>=DEL_COND && receiving_bits == 0){
		polarity=pin_high;
		receiving_bits = 8;
		
		cbi(GIMSK,INT0);
	}
	else{
		
	}
	// if(pin_high){
		/* 
		
		if(is_valid(d)){

			if(get_bit(d)){
				ON(3);
				OFF(4);
				sbi(buffer,bcnt);
			}
			else{
				ON(4);
				OFF(3);				
				cbi(buffer,bcnt);
			}
			
			bcnt++;
			bcnt&=7;
			
			if(!bcnt){
				if(buffer)
					last_byte = buffer;
				
				// reset_buffer();
			}
		}
		// else */
			// reset_buffer();
			
		
		
	// }
	// else{
		// if(del_cnt == TIMEOUT)
			// reset_buffer();
		
	// }
	
	// del_cnt = 0;
}

ISR(TIM0_COMPA_vect){
	
	// millis++;	
	if(ticks < TIMEOUT)
		ticks++;
	
	if(receiving_bits){
		
	}
	else{
	
	}
	// if(del_cnt < TIMEOUT)
		// del_cnt++;

/* 
	if(sending){
		
		uint d;
		
		if(rbi(buffer,bcnt))
			d = DEL_HIGH;
		else	
			d = DEL_LOW;

		if(del_cnt < d && !rbi(PORTB,SEND_PIN))
			ON(SEND_PIN);
		if(del_cnt == d)
			OFF(SEND_PIN);
		
		// next bit
		if(del_cnt > (d + DEL_OFF)){
			
			bcnt++;
			bcnt&=7;
			
			del_cnt=0;
			
			// done
			if(!bcnt){
				
				bcnt=0;
				if(stop_tx)
					sending=false;
			}
		}
	}
	 */
}

#endif