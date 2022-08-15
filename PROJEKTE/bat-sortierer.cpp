
#include <Macros.h>
// #include "tiny_serial.h"

#define SERVO 	PB2
#define LEDG 	PB1
#define LOAD 	PB0

#define BEST 1550/20
#define GOOD 300	//	1440/20	
#define BAD  1350/20
#define DEAD 1200/20

#define TESTS	10 
#define PAUSE	200

#define S_GOOD	2
#define S_STOP	400
#define S_BAD	990

void servo(uchar r);
void sort(bool);
uint measure();

uchar s=100;
uint volts=GOOD;
uint ain,lain;

void set_volts(){
	_delay_ms(100);
	int a = adc_read(2) / 4;
	a = 31-a;
	volts=(GOOD+a);
}

int main (void)
{ 

	OUTPUT(LEDG);
	OUTPUT(SERVO);
	OUTPUT(LOAD);
	uchar cnt=0;
	
	//tiny_serial_init(80);
	//	serial_print("OK\n\r");
	// sei();
	
	_delay_ms(100);

	if(!measure()){
 
		servo(1);
		_delay_ms(500);
 		
		_delay_ms(PAUSE);
		sort(0);
		_delay_ms(PAUSE);
		sort(1);
	}
	else
		set_volts();
	
	while(1){
		OFF(LOAD);
		servo(1); 
		cnt++;
		if(!cnt)
			set_volts();
			
		ain = measure();
		
		if(ain){
			if(ain >= volts)
				sort(true);
			else
				sort(false);
		}
		else
			_delay_ms(70);
		
		lain=ain;
	}
	
	return 0;
}

void sort(bool d){

	if(d){
		servo(2);
		_delay_ms(PAUSE);
	}
	else{
		servo(0);
		_delay_ms(PAUSE);
	}

	servo(1);
	
	_delay_ms(333);
	// _delay_ms(PAUSE);
	// _delay_ms(PAUSE);
}

uint measure(){

	_delay_ms(10);
	
	uint r = 0;
	uint a = adc_read10(3);
	
	// new batterz
	if(a >= 220) {
	
		ON(LEDG);
		ON(LOAD);
		_delay_ms(20);
		
		r = a;
		uint la = a;
		
		uchar i = TESTS;
		do{
			la = a;
			a = adc_read10(3);
			_delay_ms(10);
			
			if(a >= 220 && CA(a,la,30)){
				r = (r + a) / 2;
				i--;
			}
		}while(i);

		OFF(LOAD);
		_delay_ms(100);
		OFF(LEDG);
	}
	
	return r;
}

void servo(uchar r){
	
	uchar j=3;
	
	do{j--;
		
		ON(SERVO);
	
		_delay_us(1000);
		switch(r){
			case 0: _delay_us(S_GOOD); OFF(SERVO); _delay_us(1000-S_GOOD); break;
			case 1: _delay_us(S_STOP); OFF(SERVO); _delay_us(1000-S_STOP); break;
			case 2: _delay_us(S_BAD); OFF(SERVO); _delay_us(1000-S_BAD); break;
			default:break;
		}
		
		_delay_us(18000);
	}while(j);
	
	s=r;
}
