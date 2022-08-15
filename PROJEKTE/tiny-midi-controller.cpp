
#include <Macros.h>
#include "tiny_midi.h"

// #define DEBUG
#define MASK 0b00000111

#define BUTTONS	3
#define PITCH	1
#define EXT_A	0
#define EXT_B	6

const uchar CCS[8] = {0,4,0,6,3,2,5,1};					

// int integral[8] = {0,0,0,0,0,0,0,0};
uint ain[8][2];
uchar current;
// uchar enabled = 0xff ^ (B(EXT_A) | B(EXT_B));

uchar shift;

uchar read_inputs(){
	uchar result = 0;
	uchar c=8;
	
	do{c--;
		PORTB &= ~(MASK);
		PORTB |= c;

		ain[c][1] = ain[c][0];
		ain[c][0] = adc_read10(3);
		
		if(ain[c][0] != ain[c][0])
			sbi(result,c);
	}while(c);
	
	return result;
}

int main (void)
{ 
	DDRB |= MASK;
	
	tiny_serial_init();	// 9600
	chan=0;
	
	uchar cnt=0;
	uint anew = 0;
	uint aold = 0;
	
	// read_inputs();
	// read_inputs();
	
	
	while(1){

			PORTB &= ~(MASK);
			PORTB |= current;
			// _delay_us(300);
			
			
			aold = adc_read10(3);
			anew = adc_read10(3);
			aold = ain[current][0];
		
// buttons
		if(current == BUTTONS){
			anew >>= 3;
			
			anew += 30;
			anew >>= 5;
			
			if(anew!=aold)
				shift=0;
			else
				shift = anew;					
			
		}
// regler
		else{
			if(current == PITCH){
			
				if( anew != aold){
		
					uint pitch = (anew << 4) & 0x3FFF;
					uchar low = 127 & (pitch);
					uchar high= 127 & (pitch>>7);
					
					// deadzone
					if(pitch > 8160 || pitch < 8000)
						midi_out(CC_PITCH,low,high);
					
				}
			}
	// controllers
			else{
				anew += 3;
				anew >>= 3;
				
				if(anew > 126) 
					anew = 127;
					
				if( anew != aold){
					set_ctrl((CCS[current]+shift*10),(anew));
				}
			}
			
		}	
	
	
	
// TEST
/*
	set_ctrl(80+current,ABS(integral[current]));
	if(aold > anew && integral[current] > -64)
		integral[current]--;
	if(aold < anew && integral[current] < 64)
		integral[current]++;
*/
////////////////		

		ain[current][1] = aold;	
		ain[current][0] = anew;	

		current++;
		current&=7;
		cnt++;	
		
	}
	
	return 0;
}
