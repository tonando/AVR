
#include <Macros.h>
#define NUM_LEDS 16
#include <shift_fade.h>
//#include <sintable.h>
//#include <EEPROM.h>
// #include <util/delay.h>
#define DEBUG 	1
#define PRESSED (!READ(0))

void drawGradient(uchar,uchar ,uchar, uchar );

uchar rot = 0;

int main (void)
{ 
	// local variables
	uchar cnt=0;
	uint del=500;

	// button
	BUTTON(PB0);
	// BUTTON(PB4);
	
	shift_init();
	
	// blend timer
	TCCR1 = _BV(CTC1) | 10;
	TIMSK |= _BV(OCIE1A);
	OCR1A = 20;

	// ADC
	// ADMUX = (1 << ADLAR) | (1 << MUX1);       // use ADC2 for input (PB4), MUX bit 0
	// ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);
			
	sei();
	
    while (1) {
					


		if(PRESSED){
			set_mask(ALL ^ (cnt|3),255-cnt);
			// drawGradient(0,NUM_LEDS-1,111,5);
		}
		// counter
		cnt++;
		cnt&=255;
		
		if(!(cnt&15)){
			
			if(PRESSED)
				rot--;
			else
				rot++;
			
			rot%=NUM_LEDS;
			
			
			set(rot,80);//+(cnt&127));
			fade_mask(ALL ^ (B(rot)),0);
		}
	
		
		// end
		if(!cnt && !rot){
			set(NUM_LEDS-1,255);
			fade_mask(ALL,2);
			_delay_ms(1000);
			fade_mask(ALL,0);
			_delay_ms(1000);
			del = lfsr();
			del &= 0x002f;
			del *= 100;
			// del = constrain(del,100,10000);
		}
		
		if(PRESSED)
			_delay_us(6000);
		else
			delay_us(del);
		

		
		
		}
}
/* 
void drawGradient(uchar L1, uchar L2, uchar V1, uchar V2){
  // null
  if(L1==L2){
    // average
    
    set(L1,V1);
    return;
  }
  
  // switch
  if(L1 > L2){
    int tmp = L1;
    L1 = L2;
    L2 = tmp;
    tmp = V1;
    V1 = V2;
    V2 = tmp;
  }
  // num leds
  int dNum = L2-L1;
  // step size
  int divider = ABS(V2-V1) / dNum;
  
  // value
  int v = V1;
  for(int i=L1; i<=L2; i++){
    if(V1<V2)
      v += divider;
    else
      v -= divider;
      
    set(i, v);
  }
}
 */

ISR(TIMER1_COMPA_vect){
		blend(ALL);
}