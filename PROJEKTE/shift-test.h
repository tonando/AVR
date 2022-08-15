
#include <avr/io.h>
#include <util/delay.h>
// #include <Anal.h>
// #include <EEPROM.h>
#include <shiftout.h>

#define STPS 3

// const uint8_t STEP_SEQUENCE[8] = {0b1010,0b0010,0b0110,0b0100,0b0101,0b0001,0b1001,0b1000};
// full
// const uint8_t STEP_SEQUENCE[8] = {0b1010,0b0110,0b0101,0b1001,0b1010,0b0110,0b0101,0b1001};

bool dir,half;
int speed;
uchar c_step, output;

#define MIN_MS 4
#define STEP_PULSE 256

// uchar range = 8; // * min
// int delay = MIN_US; // us
// uchar min_delay = 1; // ms

/* 
void setup(){

	DDRB = 0b00001110; // set inputs
	PORTB = 0xff;
	
	int ain=0;
	uchar s=1;
	
	do{
		PORTB = _BV(s) | 1;
		
		while(PINB & 1){
			PORTB ^= _BV(s);
			_delay_ms(200);
			ain = analRead(2); 
		}
		
		if(s==1)
			min_delay = ain >> 6; // set min
		if(s==2)
			range = ain >> 7; // range
		if(s==3)			
			half = ain > 512; // halfstep mode
			
		do{ 
			_delay_ms(500); 
		}while(!(PINB & 1)); // wait
		
		s++;
		
	}while(s!=4);
	
}
 */
void step(){
   if(dir){
	   c_step++;
	   // c_step&=STPS;
   }
   else{
	   // if(c_step && c_step <= STPS)
		c_step --;
	   // else
		// c_step = STPS;
   }
   c_step&=STPS;

   output = STEP_SEQUENCE[c_step];// & 0x0f;
}

int main (void)
{
	// uchar mode=EEPROM_read(2);
	
	DDRB = 0xff;
	PORTB = 0;
	
	// analInit(0);

	// _delay_ms(500);
	
	// mode++;
	// mode&=3;

	// EEPROM_write(2,mode);
	// _delay_ms(5);
	
   while( 1 ) {

	// speed = analRead(0);
	// speed -= 512;

	// dir = speed > 0;
	// if(!dir) speed *= -1;

	// speed -= 12;
	// if(speed <= 5) 	continue; // off
	
	step();
	PORTB = output;

	// uint32_t d = (516 - speed);
	// uint32_t hd = 0;
	// if(mode) hd = d >> mode;	
	
	_delay_ms(5);
	// _delay_ms(MIN_MS);
	// do{--d;
		// _delay_us(STEP_PULSE);
		
		// if(d==hd)
			// PORTB = 0;
	// }while(d);

	PORTB = 0;
	
  }

  return 0;
}

