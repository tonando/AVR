
#include <Macros.h>
#include <math.h>

#define MASK 0b00000111
#define INVERT
// #define USEGAMMA

#define MAXVAL	255
#define MAXHUE	(MAXVAL * 3)

#ifdef USEGAMMA
#include <gamma.h>
#endif

volatile uchar cnt = 0;

uchar rgb[3];
uint hue;

bool auto_mode=true;

void set_hue(uint _h){
	_h = MIN(_h,MAXHUE);

	uchar x,y;
	x = _h & MAXVAL;	// led val
	y = MAXVAL - x;	// led val
	
#ifdef USEGAMMA
	x = pgm_read_word(&gamma[x]);
	y = pgm_read_word(&gamma[y]);
#endif

	uchar n = 0;			// led num
	
	if(_h > MAXVAL)
		n++;
	if((_h>>1) > MAXVAL)
		n++;
		
	switch(n){		
		case 0:
			rgb[0] = y;
			rgb[1] = x;
			rgb[2] = 0;
		break;

		case 1:
			rgb[0] = 0;
			rgb[1] = y;
			rgb[2] = x;
		break;

		case 2:
			rgb[0] = x;
			rgb[1] = 0;
			rgb[2] = y;
		break;

		default: break;
	}

}
void switch_mode(){
	
	auto_mode = !auto_mode;
	/*
	uint d = 10000;
	
	for(uchar x=0;x<256;x++){
		
		// float dh = sin( 255.0 / (MAX(1,x) / 16.0)) * (10.0 / (MAX(1,x) << 5));
		uint h = (((x&63)>31) ? 31-(x&31) : (x&31)) * MAX(1, (x>>5));
		// if(!(x&15) || (!(x&3) && (x>200)) )
			// d++;
		
		if(d > 100)
			d -= lfsr() & 63;
		
		set_hue(h);
		delay_us(d);
		
		if(!READ(4))
			return;
		// delay_ms((uint)d);
	
		// h = sin(i/80) * (MAXVAL - i);
		// d *= 0.92;
	}
	*/
	_delay_ms(100);
}

int main (void)
{ 
	uint ain=0;
	DDRB = MASK;
	
	BUTTON(4);
	
	TCCR0A |= _BV(WGM01);
	TCCR0B = 1;
	OCR0A = 86;
	TIMSK0 |= _BV(OCIE0A);

	sei();
	
    while (1) {

	
		if(!READ(4)){
			rgb[0] = rgb[1] = rgb[2] = 0;
			
			while(!READ(4)){
			uchar l = MIN(lfsr()&3,2);
				if(rgb[l]<MAXVAL)
					rgb[l]++;
					
				_delay_ms(8);
			}
			
			switch_mode();
			
			// _delay_ms(200);
		}
			
		ain = adc_read10(3);

		if(auto_mode){
			
			hue++;

			uchar r = lfsr() & 0xff;
			
			if(r<127){
				if(r<64)
					hue += r&3;
				else
					hue -= r&3;
			}
			
			if(hue >= MAXHUE)
				hue = 0;
				
			delay_us(ain<<2);
		}
		else{
			if(ain<hue)
				hue--;
			if(ain>hue)
				hue++;
			
			_delay_ms(4);
		}
		
		set_hue(hue);

	}
}

ISR(TIM0_COMPA_vect){
	cnt++;
	cnt&=MAXVAL;

	uchar out=0;

	if((rgb[0]) > cnt)
		sbi(out,0);
	if((rgb[1]) > cnt)
		sbi(out,1);
	if((rgb[2]) > cnt)
		sbi(out,2);

#ifdef INVERT
	PORTB = (PORTB & ~(MASK)) | (~out);
#else
	PORTB = (PORTB & ~(MASK)) | out;
#endif	
	
}