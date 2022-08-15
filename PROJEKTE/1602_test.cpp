
#include <Macros.h>

#define DATA_PORT 	 	PORTB
#define CONTROL_PORT 	PORTB
#include <1602.h>


char s[10] = "Hallo";
uchar mode=0;

int main (void)
{
	uchar cnt=0;

	// BUTTON(PB0);
	DDRB |= 0xff;
	DDRD &= 0x0f;
	PORTD |= 0xf0;
	// ON(5);
	// _delay_ms(1000);
	// OFF(5);
	lcd_init();
	lcd_print("TEST",0);
	lcd_print(s,1);
	_delay_ms(1000);
	// sei();
	
    while (1) {
			
	char r = 48 + cnt;
	if(!rbi(PIND,0)){
		mode++; mode&=1;
		s[8]=48+mode;
		lcd_print(s,0);
		_delay_ms(1000);
	}	
	if(mode==1){
		lcd_print("TEST",0);
		lcd_print("c"+r,1);
		
	}
	else{
		lcd_print("UART",0);
		lcd_print("ADC:"+(char)(adc_read(0)),1);
		
	}
		
		_delay_ms(500);

		cnt++;
		cnt&=7;
		
	}
}

/* 
	// blend timer
	TCCR1 = _BV(CTC1) | 8;
	TIMSK |= _BV(OCIE1A);
	OCR1A = 20;
	ISR(TIMER1_COMPA_vect){
}
SIGNAL(PCINT0_vect){

}
 */		