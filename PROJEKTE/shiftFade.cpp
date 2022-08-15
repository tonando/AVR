


//	ALT !!!!
#include <avr/io.h>
#include <util/sf_delay.h>
#include <Anal.h>
// #include <EEPROM.h>
// #include <shiftout.h>


#define pin_on(p) sbi(PORTB,p)
#define pin_off(p) cbi(PORTB,p)

uchar LEDS[8];// = {10,0,10,2,7,0,6,16};
uchar DIRS = 0b10011101;

uchar sf_del=1,b=1;
uchar skip,cl;

volatile uchar cnt;
volatile uchar sf_cbit;

void fade();

int main (void)	
{	
	DDRB = 0xff;
	DDRC = 0x00;
	PORTB  = 1;
	// initShift(2,1,0);
	// analInit(0);
	TCCR0B = 1;
	TIMSK0 = _BV(TOIE0); 
	sei();
	bool dir = 0;
   while( 1 ) {

	// sf_del = analRead(2);
	// fade();

	sbi(PORTB,4);
	uchar a = min(99,(adc_read(0)));
	uchar i = 100;
	_sf_delay_ms(1);
	do{--i;
	if(i==a) cbi(PORTB,4);
	_sf_delay_us(10);
	}while(i);
	cbi(PORTB,4);
	_sf_delay_ms(18);
	
	b += dir ? 1 : -1; 
	if(!(b&127)){
		// b = cnt;
		dir=!dir;
		}
	// LEDS[cl]=0;
	// s++;
	
	// if(s==50){
		// s=0;
		
		// }
	cl++;
	cl&= 7;
  }

  return 0;
}

void fade(){
	// skip++;
	// if (skip == 17) skip = 0;
	
	uchar i=8;
	do{--i;
		// char a = (analRead(2) >> 2) - 127;
		// if(skip == i) LEDS[i] = 254;
		
		if(rbi(DIRS,i)){
			LEDS[i]++;
			if(LEDS[i]==255) cbi(DIRS,i);
		}
		else{
			LEDS[i]--;
			if(LEDS[i]==0) sbi(DIRS,i);
		}
			
	}while(i);
}

ISR(
TIMER0_OVF_vect){
	cnt++;
	cnt &= 127;
	
	if(b >= cnt)
	sbi(PORTB,5);
	else
	cbi(PORTB,5);
	// if(!cnt) PORTB ^= 1;
	pin_off(0);

	

		// if(i==cl)
		// if(LEDS[cl] > (cnt))
			// sbi(o,i);
	// uint8_t _in = pin_read(miso_pin);
	
    uint8_t i=8;
    do{i--;
      
	  pin_off(2); 
      
	if(i==cl)//|| i==cl)
		pin_on(1);
	  else
		pin_off(1);
      
	  pin_on(2);  
   }while(i);	
	
	

	pin_on(0);
//	pin_on(ps_pin);
	
	pin_off(2);
	
	
	
}

