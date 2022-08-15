
#include <Macros.h>
#include <button.h>
#include <usi-uart.h>


#define START_BYTE	0xf3
#define STOP_BYTE	0xf2
#define REPEAT		8

uchar data[4];

volatile uchar cnt;

void send(uchar);
bool sending;

int main (void)
{
	init_button(0);
	sbi(DDRB,1);

	init_usi(2,1);
	// TCCR0B = 2; //ctc
	// TIMSK0 |= B(TOIE0);
	
	sei();
	// uchar ain=0;
	sending=true;
	
   
   while( 1 ) {
	   if(button_pressed(0)){
			// send(255);
			sending=!sending;
			_delay_ms(500);
	   }
	   
		// if(!sending) 
		
		uchar a = adc_read(2);
		send(a);	
		// if(a != ain)
		// ain = a;
		
		_delay_ms(10);
	      
	}	   
  

  return 0;
}
void send(uchar _v){
	data[0]=START_BYTE;
	data[1]=_v;
	data[2]=~(_v);
	data[3]=STOP_BYTE;
	
	// cnt=0;
	// sending=true;
}
ISR(TIM0_OVF_vect){
	if(sending){
			
		// byte
		// uchar b = ;
		if(rbi(data[(cnt>>7)],cnt&7))
			ON(1);
		else
			OFF(1);

		cnt++;
		cnt&=31;
		// if(!cnt){
			// cnt=0;
			// sending=false;
		// } 
	}
	else
		OFF(1);
}