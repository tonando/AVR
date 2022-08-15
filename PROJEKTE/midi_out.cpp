#include <Macros.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define TX PB1

volatile unsigned char buffer[3];
volatile unsigned char bits_left, c_byte;

void setMidi(unsigned char num, ...){
   
   bits_left=num * 8;
   }
   
void midiOut(unsigned char c, unsigned char a, unsigned char b){
      if(buffer[0]) return;
	 bits_left = 8;
	 c_byte=0;
   buffer[0] = c;
   buffer[1] = a;
   buffer[2] = b;
}
   
int main()
 { 
    
    DDRB = _BV(TX);
	PORTB = 1;
   TCCR0B = 1;
   TIMSK = _BV(TOIE0);
    sei();
   // Write your code here
   while (1){
      midiOut(0x90,64,127);
      
      _delay_ms(1000);
      midiOut(0x80,64,1);
      _delay_ms(1000);
      }
      
   return 0;
 }
 
 
ISR(TIM0_OVF_vect){
   if(!buffer[0]) return;
	
   if(bits_left == 8 && c_byte == 0){
	ON(TX);
	}
   else{
   if( (buffer[c_byte] >> bits_left) & 1)
      OFF(TX);
   else
      ON(TX);
	  }
	
	if(!bits_left){
		if(c_byte < 3){
			c_byte++;
			bits_left=8;
			}
		else{
			buffer[0] = 0;	
		}
	}
	else
		bits_left--;
}