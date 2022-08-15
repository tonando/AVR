#include "Macros.h"

// #define BAR_MODE
// #define AVERAGE
#define PEAK

volatile uchar ain=1,l_ain=2;

uchar min,max;
uint avg;
bool min_set,max_set,auto_range;
uchar adc();

void shift_out(){
	static uchar c=0;
	
	uchar v=ain>>4;

#ifdef AVERAGE
	v=avg>>4;
#endif

#ifdef PEAK
	
	if(!(c&3))
		v=(max>>4);
		
	if(!c && max)
		max--;
		
	c++;
	c&=63;
#endif

	uchar i=16;
	OFF(1);
	do{
#ifdef BAR_MODE	
		if(v>=i)
#else
		if(v==i)
#endif
			ON(0);
		else
			OFF(0);
		OFF(2);
		ON(2);
	}while(--i);
	ON(1);
	
}
int main (void)
{
	uchar cnt=0;
	uchar a=0;
	
	DDRB = 0xff;
	INPUT(4);
	BUTTON(3);
	if(PRESSED(3)){
		auto_range=min_set=max_set=true;
		max=0;
		min=255;
	}
	TCCR0B=3;
	TIMSK0=B(TOIE0);
	
	sei();	
	
   while( 1 ) {
    a = adc();
	
	if(ain != a){
		if(!cnt) 
			l_ain=ain;
		ain=a;
		cnt++;
	}
	
	// if(auto_range){
		// if(a > max)
			// max=a;
		// if(a < min)
			// min=a;
	// }
	// else{
		if(PRESSED(3)){
#ifdef RANGE
			if(!min_set){
				min=a;
				min_set = true;
				ain=0;
			}
			if(!max_set && a > (min+15)){
				max=a;
				max_set = true;
				ain=15;
			}
			shift_out();
			_delay_ms(800);
#endif
		}	
	// }
  }

  return 0;
}
uchar adc(){
	uchar result = adc_read(2);
	

#ifdef AVERAGE	
	static uchar avg_cnt=0;
	avg_cnt++;
	avg_cnt&=63;
	avg += result;
	avg >>= 1;
	
	return (avg>>4);
#endif	


#ifdef RANGE
	uchar range = max-min;
	result -= min;
	range = MAX(range>>4,1);
	return (result/range);
#else
	return result>>4;
#endif
}
// volatile uchar pwm=0;
ISR(TIM0_OVF_vect){
	shift_out();
	
	// pwm++;
	// pwm&=15;
}