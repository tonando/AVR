
#include <Macros.h>

#define DELMIN		100
#define DELMAX		1000
// #define TIMEOUT		10 

// #define BOOST_ON

#define BUZZER		0
#define TRIGGER		1
#define LED			2
// #define BOOST		1


#define LOW_VOLTAGE		220

void predator(uchar);
void tone(uchar _f, uint _d);
void tone(uchar _f){tone(_f,200);}
void sweep(uint _start, uint _stop, uchar _speed);
void beep(uint _f, uchar _r);
void noise(uint l);
// void boost();

uchar minutes;
/* 
uchar mel[8]={223,35,62,134,166,77,43,211};
void melody(){
	uchar i=32;
	
	do{i--;
		if(!(i&3))
			noise(50);
			
		tone(mel[i&7],110);
		_delay_ms(140);
			
	}while(i);
} 
*/


uchar on_timer=10;
bool trigger=true;
uint voltage;

int main (void)
{ 
	// OSCCAL = 140;
	OUTPUT(BUZZER);
	OUTPUT(LED);
	INPUT(3);
	
	BUTTON(TRIGGER);
	trigger = !READ(TRIGGER);
	
	uchar c=0;
	
	TCCR0A |= (1<<COM0A0); 
	TCCR0A |= B(WGM01); 
	
	uint del = 1000;

	
	while(1){
	
		voltage = adc_read(3);

		if(voltage < LOW_VOLTAGE && !(c&3)){
				beep(130,3);
				// beep(180,2);
				beep(170,1);
				// beep(140,2);
				// beep(40,4);
		}
	
		if(trigger){
			while(!READ(TRIGGER)){
				
				beep(180,2);
				beep(80,2);
				_delay_ms(500);
				
				beep(200,2);
				beep(240,2);
				_delay_ms(500);
				
				sweep(100,240,8);
				_delay_ms(20);
				sweep(130,240,5);
				_delay_ms(20);
				sweep(30,200,2);
				
				_delay_ms(1200);
				// on_timer = 55;
			}
		}
		
		// count
		c++;
		c&=31;
		
		// if(on_timer)
			// on_timer--;
		// else
			// OFF(LED);
		
		if(READ(TRIGGER))
		_delay_ms(2000);		

		
		if(!c){
			if(minutes < 8)
				minutes++;
			if(del > DELMIN)
				del -= 10;
		}
		
		if(minutes>3 && !(c&7)){		
			sweep(95,90,10);
			
			if(minutes>6){
				
				for(uchar i=0;i<4;i++){
					sweep(70,80,5);
					beep(20,1);
					sweep(90,60,5);
					beep(60,1);
				}
				
			}
		}
			/*
				
			
			if(minutes>3 || on_timer){		
				uchar rand = 64 + (127+lfsr());
				
				if(minutes > 4 || on_timer){
					sweep(rand,(rand-40),12);
					
					if(minutes > 6){
						beep(255-(rand>>2),3);
	
						for(uchar i=0;i<6;i++){
							sweep(rand,rand+10,30);
							sweep(rand-20,rand,30);
						}
					}
					else{
						predator(34);
						for(uchar i=0;i<8;i++){
							noise(200);
							_delay_ms(200);
							}
						
					}
				}
				
				sweep(255,rand,30);
				predator(10);		
				
			}
			else{
				// heartbeat
				// uchar t=30+(c<<2);
				if(!(c & 7))
						predator(5);
				else
					// if(c&3)
						tone(100,35);
					// else 
						// tone(t<<1,40);
			}
			*/
		
		
	}
	
	return 0;
}

void tone(uchar _f, uint _d){

ON(LED);
  	_f = RANGE((255-_f),5,250);
	_d = MAX(1,_d);
#ifdef BOOST_ON
	boost();
#endif
	
	TCCR0B = 3;
	OCR0A = _f;
	
OFF(LED);
	do{	_delay_us(300);	}while(--_d);
	
	TCCR0B = 0;
	OFF(BUZZER);
}

void sweep(uint _start, uint _stop, uchar _speed){
	_speed = MAX(_speed,1);
	uchar diff = _start > _stop ? _start-_stop : _stop - _start;
		ON(LED);
	for(uchar i=0; i< diff; i++){
		tone(_start,_speed);
		if(_start < _stop)
			_start++;
		else
			_start--;
	}
		OFF(LED);
}

void predator(uchar s){

	uchar e = (10+s);
		noise(e);
	for(uchar i=0;i<e;i++){
		uchar t = s + e + ((i&3)<<1);
		if(i&1)
		// noise(1);
		sweep(t+8,t,2);
		delay_ms(2+(i*2));
	}
}

void beep(uint _f, uchar _r){
	do{
	ON(LED);
		tone(_f,400);
		OFF(LED);
		_delay_ms(100);
	}while(--_r);
}

void noise(uint l){
	TCCR0B = 2;
#ifdef BOOST_ON
	boost();
#endif
	do{
		for(uint i=0;i<50;i++){
			uchar r = lfsr();
			OCR0A = r;
			_delay_us(100);
		}
	}while(--l);
	
	TCCR0B = 0;
	OFF(BUZZER);
}
