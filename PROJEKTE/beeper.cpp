
#include <Macros.h>

#define TIMEOUT			160 // 4min (33 ~ 1min)

#define EN_LOW_VOLT		true
#define EN_TIMEOUT		true
#define EN_HEARTBEAT	true

#define DELMAX	20000
#define DELMIN	10

#define BUZZER		3
#define SENSE		2
#define TRIGGER		0

void predator(uchar);
void tone(uint _f, uchar _d);
void sweep(uint _start, uint _stop, uchar _speed);
void beep(uint _f, uchar _r);

#ifdef EN_LOW_VOLT
uchar LOW_VOLT;
uchar ain = 0;
uchar l_ain = 255;
bool enable_sense = false;
#endif

void test(){
	tone(200,10);
	sweep(100,180,4);
	sweep(30,40,28);
	
	delay_ms(300);
	
	for(uchar i=4;i<50;i++)
		beep(i,1);
	
	sweep(250,100,32);
	sweep(50,10,8);
	
	test();
}

int main (void)
{ 
OSCCAL =200;
	OUTPUT(BUZZER);
	INPUT(SENSE);
	BUTTON(TRIGGER);
	
	_delay_ms(10);
	if(!READ(TRIGGER))
		test();
		
#ifdef EN_LOW_VOLT		
	ain = adc_read(SENSE);
	if(ain > 10){
		enable_sense = true;
		LOW_VOLT = ain - (ain>>3);
		sweep(250,100,32);
		}
#endif	
	uchar c = 0;

#ifdef EN_TIMEOUT
	uint timeout = TIMEOUT;
#endif

	beep(9,8);
	_delay_ms(1500);
	predator(80);	
	
	while(1){

		c++;
		_delay_ms(50);
		
#ifdef EN_HEARTBEAT
		if(!(c&63)){
			// running out
			if(timeout < (TIMEOUT>>2)){
				sweep(122,88,5);
				beep(111,5);			
			}
			else{
				// normal
				sweep(22,77,1);
				beep(60,2);			
				// sweep(33,22,2);
			}
		}
#endif
	
#ifdef EN_LOW_VOLT
		if(!(c&31) && enable_sense){
			ain = adc_read(SENSE);
			if(ain>120){
				// ain = (ain + l_ain) >> 1;

				// almost low
				if(ain < LOW_VOLT+(LOW_VOLT>>2))
					beep(130-(ain>>2),2);

				// warning
				if(ain < LOW_VOLT && l_ain == ain){
					for(uchar i=0;i<3;i++)
						sweep(75-(i<<1),60,1+i);
				}
			}	
			l_ain = ain;
		}
#endif
		
#ifdef EN_TIMEOUT
		if(!(c&15)){
			if(!timeout)
				break;
			else
				timeout--;
				
		}
#endif

	if(!READ(TRIGGER))
		break;
		
	}
	
//	BEACON TONES	
	tone(60,20);
	
	while(1){
		for(uchar i=0;i<8;i++){
			sweep(1500,50+(i<<1),2+(i&1));
			_delay_ms(50);
		}

		predator(30);
		
		sweep(10,99,4);
		beep(30,8);
		sweep(150,120,4);
		
		predator(40);	
		
		if(!READ(TRIGGER))
			break;
		
		_delay_ms(2000);
	}
	
	
	return 0;
}

void tone(uint _f, uchar _d){
	_f = MAX(10,MIN(_f,30000));
	_d = MAX(1,_d);
	
	ulong del = 40000UL / _f;
	del = MAX(DELMIN,MIN(del/2,DELMAX));
	
	_d *= _d + _f/100;
	
	do{
		ON(BUZZER);
		delay_us(del);
		OFF(BUZZER);
		delay_us(del);
		
		_d--;
	}while(_d > 1);
}
/* void tone(uchar _f, uchar _d){
	_f = MAX(1,_f);
	_d = MAX(1,_d);
	
	ulong del = 20000UL / _f;
	
	_d += MIN(30,(_f>>3)) * _d;
	
	do{
		ON(BUZZER);
		delay_us(del);
		OFF(BUZZER);
		delay_us(del);
		
		_d--;
	}while(_d > 1);
}
 */
void sweep(uint _start, uint _stop, uchar _speed){
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
	uchar e = (20+s);
	for(uchar i=0;i<e;i++){
		uchar t = s + e - ((i&3)<<1);
		sweep(t+5,t,1);
		delay_ms(3+(i<<1));
	}
}
void beep(uint _f, uchar _r){
	do{
		tone(_f,10);
		_delay_ms(60);
	}while(--_r);
}