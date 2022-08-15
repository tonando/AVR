
#include <Macros.h>
#include <button.h>
#include <usi-uart.h>


#define IN_PIN		PB2
#define LED_PIN		PB1

#define START_BYTE	0xf3
#define STOP_BYTE	0xf2
uchar data[2];

volatile uchar cnt;
volatile uchar inbyte;

uchar read();
void reset();
bool found;
int main (void)
{
	init_button(0);
	// sbi(DDRB,1);
	sbi(DDRB,3);
	sbi(DDRB,4);
	init_usi(2,1);
	
	TCCR0B = 2; //ctc
	TIMSK0 |= B(TOIE0);
	
	sei();
	reset();
   
   while( 1 ) {
	   if(button_pressed(0)){
		// ain = adc_read(2);
			reset();
			_delay_ms(500);
	   }
	   
	   if(found) ON(LED_PIN);
	   else OFF(LED_PIN);
		   
	}	   
  

  return 0;
}
void reset(){
	data[0]=data[1]=0;//data[2]=data[3]=0;
	cnt=0;
	inbyte=0;
	found=false;
}
uchar read(){
	if(!found) return 0;
	
	if(	data[1] == (255-data[0]))
		reset();
	else
		return 0;
	
	return data[0];
	
}
ISR(TIM0_OVF_vect){
		fbi(PORTB,4);
		if(found){
			if(READ(IN_PIN)){
				sbi(data[(cnt>>7)],(cnt&7)); // data arraz
				ON(3);
			}else
				OFF(3);
			cnt++;
			cnt&=31;
		}
		else{
			
			if(READ(IN_PIN)){
				inbyte |= 1;
				ON(3);
			}else
				OFF(3);
			inbyte <<= 1;

			if(inbyte == START_BYTE){
				reset();
				found=true;
			}
		}
}