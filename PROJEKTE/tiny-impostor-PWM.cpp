
#include <t45.h>
#include <delay.h>
#include <Anal.h>

#define ANAL_IN 3	// PB4	
#define OUT 	PB1	// OC1A  / 0B
#define B_SPEED PB2
#define B_REC	PB0

#define BUFFER_SIZE 128
int speed = 1023;

uchar rec[BUFFER_SIZE] = { 0, 3, 6, 9, 12, 15, 18, 21, 24, 28, 31, 34, 37, 40, 43, 46, 49, 52, 55, 58, 61, 64, 68, 71, 74, 77, 79, 82, 85, 88, 91, 94, 97, 100, 103, 106, 109, 111, 114, 117, 120, 122, 125, 128, 131, 133, 136, 139, 141, 144, 146, 149, 151, 154, 156, 159, 161, 164, 166, 168, 171, 173, 175, 178, 180, 182, 184, 186, 188, 191, 193, 195, 197, 199, 201, 202, 204, 206, 208, 210, 212, 213, 215, 217, 218, 220, 221, 223, 224, 226, 227, 229, 230, 231, 233, 234, 235, 236, 237, 239, 240, 241, 242, 243, 244, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 254, 255};
uchar index;

void init_pwm(){
	//Output compare OC0B 8 bit non inverted PWM
   TCCR0A=(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);;

   TCCR0B = 1;
   // TIMSK = _BV(OCIE0B);
	OCR0B=0;

 
}

int main (void)
{
  DDRB = _BV(OUT)  | _BV(B_SPEED);
  PORTB = _BV(B_REC) | _BV(B_SPEED);
   
	
	uchar ain=0;

	bool recording=false;
	index = 0;
	
	init_pwm();
	
	while( 1 ) {
		
		//button down
		recording = bit_is_clear(PINB,B_REC);
		
		if(recording){
			ain = adc_read(3);
			
			rec[index] = ain;
		}
		else{
			ain = adc_read(2);
			speed = 256-ain;
		}

		// out
		OCR0B = rec[index];	


		
		//speed control 
/* TODO: interpolate
		calc delta
		char d = rec[index] - OCR0B;
			int d = (rec[index] - OCR0B) / speed;
			if(rec[index] != OCR0B){
				int d = (rec[index] - OCR0B) / 2;
				OCR0B += d;
			}
 */			
		// blink 
		// if((index & (BUFFER_SIZE>>3)) == 0)
			// PORTB ^= _BV(B_SPEED);
			

		uchar i = speed;
		do{i--;
			_delay_us(100);
		}while(i);

		index++;	
		if(index == BUFFER_SIZE){
			index = 0;		
			PORTB ^= _BV(B_SPEED);
		}

	}

  return 0;
}
 