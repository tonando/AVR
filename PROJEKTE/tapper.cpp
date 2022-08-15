#include <t45.h>
// #include <delay.h>
//#include <fastA2D.h>

#define B_STOP		PB4
#define B_REC		PB2
#define LED			PB1	


#define ANAL_IN 	2	
#define MIN_DELAY	7

#if defined(__AVR_ATtiny45__)
	#define REC_SIZE	240
// #endif
#elif defined(__AVR_ATtiny85__)
	#define REC_SIZE	500
#else
	#error MCU NOT SUPPORTED
#endif

uint rec[REC_SIZE/2];
uchar idx;
volatile uchar out;
volatile uint cnt;

uint speed=20;
bool last_btn;

void adc_setup (void)
{
    // Set the ADC input to PB2/ADC1
    ADMUX = (1 << ADLAR);
    ADMUX |= 2;
    ADCSRA |= (1 << ADEN);
}

uchar adc(void)
{
    // Start the conversion
    ADCSRA |= (1 << ADSC);
    // ADMUX |= 3;
 
    // Wait for it to finish
    while (ADCSRA & (1 << ADSC));
    return (ADCH & 0xff);
 
}


ISR(TIMER0_OVF_vect){
	cnt++;
}
 
int main() { 
	
	OUTPUT(LED);
	BUTTON(B_REC);
	INPUT(2);
	
	adc_setup();
	
	TCCR0B = 3;
	TIMSK = B(TOIE0);
	sei();

	last_btn = READ(B_REC);
	uint del=32;
	uchar aval=0;
	bool started=false;
	bool done=false;
	
	// main
	while (1){
		// speed
		// aval = adc();	
		// del = (aval>>2) & 63;
		
		if(done){
		
		}
		else{
			// recording	
			if(READ(B_REC) != last_btn){
				rec[idx] = cnt;
				cnt=0;
				idx++;
				
				if(idx >= REC_SIZE)
					idx=0;
			}
			// stop
			if(PRESSED(B_STOP))
				done=true;
		}
				
		// delay_ms(del);
		
	}
	return 0;
 }
	  
	 
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  