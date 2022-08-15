
#include <Macros.h>
#include <shift_fade.h>
#include <sintable.h>
#include <EEPROM.h>
// #include <util/delay.h>
#define DEBUG 	1

#define MODE_DIM 	0
#define MODE_RAND 1
#define MODE_SINUS	2
#define MODE_BRIGHT 3
#define MODE_DEBUG 	4

#define UV1 0
// #define UV2 2
#define LED_R 3
#define LED_G 2
#define LED_B 1
#define R1 7
#define R2 5
#define Y2 6
#define Y1 4

#define BLUMEN (_BV(R1) | _BV(R2) | _BV(Y1) | _BV(Y2) )
#define UVMASK (_BV(UV1))
#define RGB_LEDS (_BV(LED_R) | _BV(LED_G) | _BV(LED_B) )

// uchar level,min,max;
uchar ain=100;
uchar mode;
uchar hue;
int speed = 100;

uchar rgb[3];
void fade_rgb();
void set_mode(uchar);
void set_hue(uchar,uchar);
void set_hue(uchar _h){set_hue(_h,0); }
void set_rgb(uchar,uchar,uchar);

void dim_rgb(uchar _v){
	rgb[0] >>= _v;
	rgb[1] >>= _v;
	rgb[2] >>= _v;
}
void set_rgb(uchar _r,uchar _g, uchar _b){
	rgb[0]=_r;
	rgb[1]=_g;
	rgb[2]=_b;

	fade_rgb();
}
void set_hue(uchar _h,uchar _v){
	int hue = (_h & 63)<<2;
	
	rgb[0]=rgb[1]=rgb[2]=0;
	
	uchar trans = _h >> 6;
	
	switch(trans){
		case 0:
			rgb[0] = 255-hue;
			rgb[1] = hue;
			// rgb[2] = 0;
		break;
		case 1:
			rgb[0] = hue;
			rgb[1] = 255-hue;
			// rgb[2] = 0;
		break;
		case 2:
			// rgb[0] = 0;
			rgb[1] = hue;
			rgb[2] = 255-hue;
		break;
		case 3:
			// rgb[0] = 0;
			rgb[1] = 255-hue;
			rgb[2] = hue;
		break;
	}
	
	if(_v) 
		dim_rgb(_v);
	
	fade_rgb();
}
void fade_rgb(){
	fade(LED_R,rgb[0]);
	fade(LED_G,rgb[1]);
	fade(LED_B,rgb[2]);
}
void set_mode(uchar _mode){
	
	// save rom
	EEPROM_write(14,hue);
	EEPROM_write(12,mode);
	_delay_ms(10);

	OCR1A = 100;
}
void nextMode(){
	
	mode++;
	mode&=3;

	
	set(4+mode,255);
	set_hue(mode<<6);

	_delay_ms(50);
	while(!READ(0));

	set_mode(mode);

	fade_mask(ALL,0);
	
}

int main (void)
{ 
	// local variables
	uchar cnt=0;
	uchar sin_cnt=0;
	uchar sinus = 0;

	// button
	BUTTON(PB0);
	// BUTTON(PB4);

	shift_init();
	
	// blend timer
	TCCR1 = _BV(CTC1) | 8;
	TIMSK |= _BV(OCIE1A);

	// ADC
	ADMUX = (1 << ADLAR) | (1 << MUX1);       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);
			
	mode = EEPROM_read(12);
	hue = EEPROM_read(14);
	
	set_mode(mode);
	sei();
	
    while (1) {
					
		int r = (lfsr()&0x1f);
		r-=16;

		switch(mode){

			case MODE_DIM:
				if(!(cnt&31)){
					if(ain<128){
						set(4+(lfsr()&3),255);
						// if(SKIP(cnt,3))
							fade_mask(BLUMEN,0);
						
					}
					else{
						fade(4+(lfsr()&3),100);
						fade(4+(lfsr()&3),0);
						// if(SKIP(cnt,6))
							// fade_out(BLUMEN);
					
					}
				
				hue += r;
				set_hue(hue,((ain&127)>>4));
				}
				
				fade(UV1,sinus);
				
				// OCR1A = MAX(1,sinus);
				delay_us(100 + (ain<<3));
			break;

			case MODE_RAND:

				if(!(cnt&7))
					randomize(BLUMEN,20);
				
				if(ain<128)
					set(UV1,lfsr()&0xff);
				else
					fade(UV1,lfsr()&0xff);
				
				if(!(cnt&31))
					set_hue(lfsr()&0xff);
				
				OCR1A = 20+(ain>>2);
				delay_us(100 + (ain<<5));
			break;

			case MODE_SINUS:
				
				fade(R1,sinus);
				fade(R2,sinus);
				fade(Y1,255-sinus);
				fade(Y2,255-sinus);
				set_hue(((sinus+hue)>>1));
				
				delay_us(600 + (ain<<4));
			break;

			case MODE_BRIGHT:
				fade(R1,ain);
				fade(R2,ain);
				fade(Y1,255-ain);
				fade(Y2,255-ain);
				set(UV1,(ain&15)<<4);
				hue=ain;
				set_hue(ain);
			break;
			
		}

		// buttons
		if(!READ(0)) {
			nextMode();
		}
		
		// adc
		// if(!(cnt&31)){
		if(!(cnt&3)){
			ADCSRA |= (1 << ADSC);         // start ADC measurement
			while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 
			
			ain = ADCH;
		// }
		
		// sinus
			
			sin_cnt++;
			sin_cnt&=255;
				
			sinus = get_sin(sin_cnt);			
		}
	
		// counter
		cnt++;
		cnt&=255;
		
	}
}

ISR(TIMER1_COMPA_vect){
		blend(ALL);
}