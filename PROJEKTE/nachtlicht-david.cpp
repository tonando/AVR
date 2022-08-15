
#include <Macros.h>
#include <shift_pwm.h>

#define SINSIZE 64
#include <sintable.h>

#include <avr/sleep.h>
// #include <avr/power.h>

#define FADE_OCR 160
#define YE	5
#define UV	4
#define ORANGE	(0xff ^ ( B(UV) | B(YE) ))

bool mode=true;
uchar sin_speed = 60;
uchar speed = 10;
uchar ain = 100;

uchar sin_cnt;
volatile uchar fade_cnt=0;

void fire();
void rider();

int main (void)
{ 

	uchar cnt=0;
	// uchar rmask=lfsr();
	
	BUTTON(0);
	INPUT(4);
			
	TCCR1 = _BV(CTC1) | 8;
	TIMSK |= _BV(OCIE1A);
	OCR1A = FADE_OCR;
	
	shift_init(5);
	
	while (1) {
			
		cnt++;
		
		// if(cnt>=sin_speed){
		if(!(cnt&15)){
			sin_cnt++;
		
			if(ain) 
				ain--;
		}

		uchar a = adc_read(3)>>1;
		if(a>ain){
			ain = a;
			fade(YE,ain);
		}
		
		switch(mode){
			case 0: rider(); break;
			case 1: fire(); break;
			default:break;
		}
		
		uchar i=speed;
		do{i--;
			_delay_us(100);
			
			if(!READ(0)){
				// speed = 15+(adc_read(2));
				fade_mask(255,0);
				while((!READ(0)));
				mode=!mode;
				_delay_ms(800);
			}
		}while(i);
		

	}
	
	return 0;
}
void rider(){
	uchar s = get_sin(sin_cnt)>>3;
	uchar r = lfsr()&255;
	if(!(r&3)){
		fade_out((r&3),20+ain);
	}
	// fade(0,s);
	fade(4,s);
	// fade(1,255-s);
	fade(5,r&31);
}

void fire(){
	uchar o = get_idle();
	uchar r = lfsr() & 127;
	
	
	fade_mask(o,12+ain);
	fade_mask(r,0);
	fade(UV,sin_cnt);
	fade(YE,r);
	// randomize(0xff,4);
}				

ISR(TIMER1_COMPA_vect){
		blend(255);
		
		fade_cnt++;
		// if(fade_cnt == sin_speed){
			// fade_cnt=0;
			// sin_cnt++;
				
			
		// }
}