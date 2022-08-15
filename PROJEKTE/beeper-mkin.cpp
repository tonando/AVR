
#include <Macros.h>

#define TIMEOUT		20 // (33 ~ 1min)
#define DELTICK		100
#define DELBOOST	330	// us

// #define BOOST_ON

#define DELMAX	20000
#define DELMIN	10

#define BUZZER		3
#define TRIGGER		0
#define BOOST		1
#define SENSE		2

void predator(uchar);
void tone(uint _f, uchar _d);
void sweep(uint _start, uint _stop, uchar _speed);
void beep(uint _f, uchar _r);
void noise(uint l){
	uint rand = lfsr() | (lfsr() << 8);
	do{
		for(uchar i=0;i<16;i++){
			if(rbi(rand,i))
				ON(BUZZER);
			else
				OFF(BUZZER);
				
			_delay_us(17);
		}
	}while(--l);
	
	OFF(BUZZER);
}

uchar minutes;

uchar mel[8]={23,35,62,34,66,77,43,11};
void melody(){
	uchar i=8;
	
	do{i--;
		// for(uchar j=0;j<8;j++)
			tone(mel[i],26);
			
		_delay_ms(DELTICK<<1);
	}while(i);
}
#ifdef BOOST_ON
void boost(uchar t){
	uchar ain = adc_read(SENSE);
	uchar r = 100;
	while(ain < t && r){
		for(uchar i=0;i<40; i++){
			ON(BOOST);
			_delay_us(DELBOOST);
			OFF(BOOST);
			_delay_us(DELBOOST);
		}
		
		ain = adc_read(SENSE);
		r--;
	}
}
#endif
int main (void)
{ 
// OSCCAL = 190;
	OUTPUT(BUZZER);
	OUTPUT(BOOST);
	// BUTTON(TRIGGER);
	INPUT(SENSE);
	
	uchar c=0;
	uint timeout = TIMEOUT;
	
	 // TCCR0A |= _BV(WGM01); 
	 // TCCR0B = 4;
	 // OCR0A = 54; // set Timer's counter max value
     // TIMSK |= _BV(TOIE0); // enable Timer CTC interrupt
     // sei(); // enable global interrupts
	 
	 melody();
	 
	for(uchar i=0;i<40; i++)
		lfsr();

	while(1){
	

		// count
		c++;
		_delay_ms(DELTICK);
		
		if(c==30){
			c=0;
			
			if(timeout)
				timeout--;
			else {
				if(minutes < 10)
					minutes++;
					
				timeout = TIMEOUT;
			}
				
			uchar rand = 60 + (63+lfsr());
			
			if(minutes){		
				sweep(rand,rand+40,1);
				
				if(minutes > 1){
					tone(rand,10);
					
					if(minutes > 2){
						noise(300);
						// for(uchar i=0;i<8;i++){
							// uchar f = (i<<3) + 50 + (rand&63);
							predator(4);
						// }
					}
					else{
						beep((rand&63)+80,minutes);
					}
				}
				
				sweep(rand-30,rand,2);
				
			}
			else{
				if(!(timeout & 3))
					predator(40);		
				else
					beep((rand&31)+20,2);
			}
			
		
		}
#ifdef BOOST_ON		
		boost(20);
#endif	
	}
	
	return 0;
}

void tone(uint _f, uchar _d){
	_f = MAX(10,MIN(_f,3000));
	_d = MAX(1,_d);
	
	ulong del = 40000UL / _f;
	del = MAX(DELMIN,MIN(del/2,DELMAX));
	
	_d *= _d + (_f>>4);
	
	do{
		ON(BUZZER);
		delay_us(del);
		OFF(BUZZER);
		delay_us(del);
		
		_d--;
	}while(_d > 1);
}

void sweep(uint _start, uint _stop, uchar _speed){
	_speed = MIN(_speed,3);
	uchar diff = _start > _stop ? _start-_stop : _stop - _start;
	for(uchar i=0; i< diff; i++){
		tone(_start,_speed);
		
		if(_start < _stop)
			_start++;
		else
			_start--;
	}
}

void predator(uchar s){

	uchar e = (30+s);
	for(uchar i=0;i<e;i++){
		noise(e-25);
		uchar t = s + e - ((i&3)<<1);
		sweep(t+5,t,1);
		delay_ms(3+(i<<1));
	}
}

void beep(uint _f, uchar _r){
	do{
		tone(_f,20);
		_delay_ms(30);
	}while(--_r);
}



// ISR(TIMER0_OVF_vect){

	// PORTB ^= 2;
// }