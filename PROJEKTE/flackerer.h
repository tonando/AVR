#include <Macros.h>

const unsigned char SINTABLE[16] = { 0, 25, 49, 74, 98, 120, 142, 162, 181, 198, 213, 225, 236, 245, 251, 255};

// #define MIN_PULSE 	2
#define DELAY		60

#define MOTOR_PIN	0
#define MODE_PIN	2

// volatile uchar pwm_cnt = 0;
volatile uchar mode = 1;
volatile uchar pwm = 0;

void pulse(uchar);
void sinus(uchar);
void zufall(uchar);
void peaks(uchar);

void MM(uchar);

uint spd = 512;	
uchar ran = 127;	
uchar str = 50;	

int main(){
	uchar cnt = 0;	
 
	BUTTON(MODE_PIN);
	OUTPUT(MOTOR_PIN);
	OUTPUT(1); 
	
	TCCR0A = (1<<COM0A0)|(1<<COM0A1)|(1<<WGM00); 
	TCCR0B = 2;	 
	
	sei();
	
	while(1){
 
		if(!cnt) 
			ON(1);
		
		if(cnt>127 && !(cnt&31) )
			fbi(PORTB,1);
		
		if(cnt&1)
			spd=adc_read10(2);
		else
			str=MAX(10,adc_read(3));
 
		switch(mode){
			case 0: pulse(cnt); break;
			case 1: sinus(cnt); break;
			case 2: zufall(cnt); break;
			case 3: peaks(cnt); break;
			default: break;
		}
		
		
		cnt++;
		cnt&=255;
		 
		uint d=2+(spd);
		do{
			_delay_us(DELAY);
		}while(--d);

		if(!READ(MODE_PIN)){
			mode++;
			mode&=3; 
			
			uchar i = 1+mode;
			do{
				MM(130);
				_delay_ms(42);
				MM(0);
				_delay_ms(88);			
			}while(!READ(2) && --i);
		}		 
			
		 
	}
	return 0;
}
void pulse(uchar c){
	
	pwm=(c&31)<<3;
	pwm >>= (c>>6);
	
	MM(pwm);
}
void peaks(uchar c){
	if(pwm > 10)
		pwm -= ((255-c)>>4);
	else
		pwm=lfsr()&255;
		
	MM(pwm);
}
void sinus(uchar c){

	uchar i=c>>4;
	
	if(c>127)
		pwm=SINTABLE[15-i];
	else
		pwm=SINTABLE[i];
		
	MM(pwm);
}
void zufall(uchar c){

	
	if(!(c&15)) 
		pwm = (uchar)lfsr();
	if(c<140)	
		pwm &= 31;
		
	MM(pwm);
	
}
void MM(uchar p){
	p=255-p;
	
	if(p)
		pwm = MIN(p,str);
	else
		pwm = 0;
	
	OCR0A = pwm;
}