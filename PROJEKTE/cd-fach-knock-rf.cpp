#include <Macros.h>
// #include <beep.h>
#include <rf433.h>
#include <avr/sleep.h>

#define RECEIVER 

#define OPEN_PIN 3
#define CLOSE_PIN 2
#define BTN_PIN 4

bool locked = true;
uchar in_byte=0;

void test_code(uchar);

int main(){
	
#ifdef RECEIVER	
	OUTPUT(OPEN_PIN);
	OUTPUT(CLOSE_PIN);
	init_rf(true);
#else
	BUTTON(BTN_PIN);
	BUTTON(OPEN_PIN);
	BUTTON(CLOSE_PIN);	
	init_rf(false);
#endif
 		
	while(1){

#ifdef RECEIVER		
		// if(last_byte != in_byte){
		in_byte = buffer;
		
		if(in_byte)
			test_code(in_byte);
			// ON(OPEN_PIN);
		// else
			// OFF(OPEN_PIN);
		// }
		
		// _delay_ms(500);OFF(3);
#else
		uchar code = 0;
		
		if(!READ(BTN_PIN))
			code=12;

		if(!READ(OPEN_PIN))
			code=88;
		
		if(!READ(CLOSE_PIN))
			code=200;
		
		if(code)
			tx(code,1);	
		
		_delay_ms(50);
#endif			
	}
	return 0;
}

#ifdef RECEIVER	
	void test_code(uchar c){
		OFF(OPEN_PIN);
		OFF(CLOSE_PIN);

		if(c==2)
			ON(OPEN_PIN);
		if(c==(88))
			fbi(PORTB,CLOSE_PIN);
		if(c==(200))
			ON(SEND_PIN);
		
	}

#else
#endif
