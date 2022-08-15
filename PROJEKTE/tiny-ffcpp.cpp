
#include <Macros.h>
#include <button.h>
// #include <util/delay.h>
// #include <Anal.h>

#define BTN_NEXT 	0
#define LIMIT 		3
#define DIR 		4


uchar val;
uchar tspd=1;
bool dir;
uchar ain;
uchar accel;

int main (void)
{
	init_button(BTN_NEXT);
	init_button(LIMIT);
	sbi(DDRB,1);
	sbi(DDRB,DIR);
	TCCR0A = _BV(COM0B0) | (3); //fast pwm

	sei();
	
	uchar cnt=0;
   
   while( 1 ) {
	   if(button_pressed(LIMIT)){
		   dir=!dir;
		   if(dir) ON(DIR);
		   else OFF(DIR);
		   OCR0B = 0;
		   
		   accel=255;
		   do{accel--;
			OCR0B++;
			_delay_ms(10);
		   }while(accel);
		   _delay_ms(200);
	   }
	   if(button_pressed(BTN_NEXT)){
		   tspd ++;
		   tspd &= 7;
		   
		   TCCR0B = tspd;
		   _delay_ms(100);
	   }
		   
		cnt++;
		if(!cnt){
			ain = adc_read(2);
		
		if(ain>OCR0B){
		OCR0B++;
		_delay_ms(20);
		} 
			
		// OCR0B = ain;
		// else
		// OCR0B = 20;
   }	   
  }

  return 0;
}
