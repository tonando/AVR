
#include "LCD_1602.h"  
#include "serial.h"
#include "Anal.h"

// #include <bmp085.h>  

const char* MODES[4] = {
	"ANAL",
	"FREQ",
	"CAP ",
	"RXTX"

};

volatile uint16_t ovf;

int a_value, a_last, a_min, a_max;
uint32_t f_value, f_last;

uchar mode,last_mode;

void printAnal(uchar ch);
void printCap();
uint32_t getFreq();
int getVolts(int a);
int getPercent(int a);

int main(void){

	// char counter=0;
// i2c_init();
	Init_Ports();  

	DDRD &= ~_BV(2);
	PORTD |= _BV(2);
	
	Init_Lcd(); 
	Lcd_Clear(); 

	uart_init();
	analInit(0);
	
	TIMSK = _BV(TOV0);
	UCSRB |= _BV(RXCIE);
	sei();
	// Lcd_Cmd(SHOW_CURSOR);

	Lcd_Print("OK");
		delay(10); // a quick delay in ms to allow LCD to display the value for a little bit before refreshing


	while(1){
		// delay(500); // a quick delay in ms to allow LCD to display the value for a little bit before refreshing

		// ON MODE CHANGE
		if(!rbi(PIND,2)){
			mode++; mode&=3;
			
			while(!rbi(PIND,2));
			
			CLR;
		}
	
		
		if(!mode){
			printAnal(0);
			// _delay_ms(500);
		}
		
		if(mode == 1){printCap();}
		if(mode == 2){
			uint32_t f = getFreq();
			CLR;
			Lcd_Print_Int(f);
			Lcd_Print(" Hz");
			
			// _delay_ms(200);
		}
		
			Lcd_Goto(11);
			Lcd_Print(MODES[mode]);
			_delay_ms(10);
		// else
			// Lcd_Print("... ");	

	}	// end while		

	return 0;
}	// end main

uint32_t getFreq(){
	uint32_t f=0;

	cli();
	
	TCCR0 = 6;
	TCNT0 = ovf = 0;
	
	sei();
	
	_delay_ms(1000);
	
	f = TCNT0;
	f += ovf * 256;

	TCCR0 = 0;
	// TIMSK = _BV(TOV0);	
	
	return f;
}	

void printCap(){
	// long f =
	CLR;
	HOME;
	long f = getFreq();
	f = 1000000UL / f;
	// double C = (double)f / (0.693 * 1000000UL);
	double C = (double)f / (1.1 * 1000000UL);
	
	Lcd_Print("Period = ");
	Lcd_Print_Int((long)f);
	Lcd_Print(" us");
	Lcd_Goto(16);
	Lcd_Print("Cap = ");
	Lcd_Print_Int((long)C);
	
		
	_delay_ms(300);
	
}

void printAnal(uchar ch){
	CLR;	
	a_value = analRead(ch);
	if(a_value < a_min)
		a_min = a_value;
	if(a_value > a_max)
		a_max = a_value;

	Lcd_Print("A");
	Lcd_Print_Int(ch);
	Lcd_Print(" = ");
	Lcd_Print_Int(a_value);

	// int nv = getVolts(a_value);
	// Lcd_Print(" = ");
	// Lcd_Print_Int(nv);
	// Lcd_Print(" mV");
	
	Lcd_Goto(16);

	Lcd_Print("MIN=");
	Lcd_Print_Int(a_min);	
	Lcd_Print("MAX=");
	Lcd_Print_Int(a_max);
	
	a_last = a_value;
	
}

int getVolts(int a){
	return (int)(4.888f * (float)a);
}
int getPercent(int a){
	return (int)(0.0976f * (float)a);
}


// ISR(USART_RX_vect){
ISR(TIMER0_OVF_vect){
	ovf++;
	}
ISR(USART_RXC_vect){
	if(mode == 3)
	Lcd_Send(UDR);
	// cursor_pos++;

			/* if(cursor_pos == 16){ 
				cursor_pos=0;
				Lcd_Print("done");
				delay(2000);
				Lcd_Clear();
				Lcd_Return();
			}
	 */	
}