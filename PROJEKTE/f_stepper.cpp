// #define F_CPU 9600000UL

#include <Macros.h>
#include <util/delay.h>
// #include <Anal.h>
#include <EEPROM.h>
// #include <shiftout.h>

#define STEP_RES 1	// full,half,eight...
#define ACCEL false
// #define MICRO false	// experimental

#define MIN_US 		(	150 )
//#define OFF_US 		(	STEP_US / 2)
#define STEP_US 	(	4 		/ STEP_RES)

#if STEP_RES == 4
	#define STPS 63
#elif STEP_RES == 3	
	#define STPS 31
#elif STEP_RES == 2	
	#define STPS 15
	#elif STEP_RES == 1
	#define STPS 7
#else
	#define STPS 3
#endif



const uint8_t STEP_SEQUENCE[8] = {
0b1001,
0b1000,
0b1100,
0b0100,
0b0110,
0b0010,
0b0011,
0b0001
};

bool dir, saved, running;

uchar c_step, output, last_output, saves;
int ain;

volatile int spd;
volatile uint cnt;

void step(){
   if(dir){
		c_step++;
   }
   else{
		c_step--;
   }
	
	c_step&=STPS;
	
	uchar cstp = c_step;
#if MICRO == true
		cstp = (c_step>>3);
		last_output = output;   
#endif

		
	#if STPS == 15
		output = STEP_SEQUENCE[cstp] & 0x0f;
	#else	
		output = STEP_SEQUENCE[(cstp&(~1))] & 0x0f;
	#endif

}
	
void save(){
	if(saved) 
		return;

	saved=true;
	
	EEPROM_write(0,!dir);	
	EEPROM_write(4,++saves);	
	EEPROM_write(8,spd);	
}
int main (void)
{
	
	DDRB = 0x0f;
	PORTB = 0;
	
	dir=EEPROM_read(0);
	saves=EEPROM_read(4);
	
	// if(ACCEL)
		// spd=EEPROM_read(8) >> 1;

	// if(ACCEL || MICRO){
		TCCR0B = 3;
		TIMSK = B(TOIE0);	
		sei();
	// }
	
   while( 1 ) {

		
		// read ain / speed
		ain = adc_read10(2);
		
		// threshold 
		
		running = (ain > 20);

		if(ACCEL == false)
			spd = ain;
		
		ulong d  = (1026-spd) * STEP_US;
		d = constrain(d,16,2500);
		
		PORTB = 0;
		// delay_us(MAX(MIN_US>>2,d >> 4));
		_delay_us(4);
		
		// if(spd < 500){
			// d >>= 2;
			// delay_us(d*3);
		// }
		
		if(running){
			step();
			PORTB = output;
			_delay_us(MIN_US);
			delay_us(d);
		}
		else{
			_delay_ms(10);
		}


			
		
  }

  return 0;
}

ISR(TIM0_OVF_vect){
	/*
	if(running){
		// half
		if(c_step & 1){
			if((cnt&31) < 10)
				PORTB = output;
			else
				PORTB = 0;
		}
		else{
			if((cnt&31) < 26)
				PORTB = output;
			else
				PORTB = 0;		
		}
		
	}
	else
		PORTB &= 0xf0;
	*/
	// accelerate
#if ACCEL == true
		if(spd != ain){
			
			if(ain > spd)
				spd++;
			else
				spd--;		
		}
#endif

	// microstepping
#if MICRO == true
	if(running){
		uchar c = cnt & USTPS;
		if(c_step >= c)
			PORTB = output;
		else
			PORTB = last_output;

	}
	else
		PORTB = 0;
#endif	

	// save after delay
	if(!saved){
		if(cnt >= 250)
			save();
	}
	
	cnt++;
}