#include <avr/sleep.h>
#include <Macros.h>
#include <joystick.h>

#define OPEN_PIN 3
#define CLOSE_PIN 2
#define TEST_PIN 0
#define BTN_PIN 1
#define IN_PIN 2

// millis
#define DEL_MIN 100
#define DEL_MAX 800
#define DEL_MID 300

#define CODE 0b11000011
bool locked = true;

volatile uchar bcnt;
volatile ulong millis;

bool is_valid(uint);
bool get_bit(uint);

volatile uint del_cnt;
volatile uint last_del;
volatile bool listening;


int main(){
	// OUTPUT(OPEN_PIN);
	// OUTPUT(CLOSE_PIN);
	OUTPUT(TEST_PIN);
	OUTPUT(IN_PIN);
	
	// BUTTON(BTN_PIN);
	// INPUT(IN_PIN);
	
	joystick_init(2,3);
	
	// GIMSK = B(INT0);
	// MCUCR = 1; // any edge

	// 1ms
	// TCCR0B = 3;
	// TIMSK0 |= _BV(OCIE0A);
	// OCR0A = 150;
	
	sei();
 		
	while(1){
		if(joystick_getDir(X_AXIS) != 0)
			ON(TEST_PIN);
		if(joystick_getDir(Y_AXIS) != 0)
			ON(IN_PIN);
		
		delay_ms(500);
		OFF(IN_PIN);
		OFF(TEST_PIN);
/* 
		if(!READ(BTN_PIN)){
			tx(88);
			
			delay_ms(500);
		}
 */			
	}
	return 0;
}
/* 
bool is_valid(uint _d){
	if(_d > DEL_MIN && _d < DEL_MAX)
		return true;
		
	return false;
}
bool get_bit(uint _d){
	if(_d < DEL_MID)
		return true;
	else
		return false;
}
void tx(uchar b){
	uint b=1;
	uchar i=7;
	do{
		ON(SEND_PIN);
		
		if(rbi(b,i))}
			b = (DEL_MIN + DEL_MID) / 2;
		else	
			b = (DEL_MAX + DEL_MID) / 2;
		
		delay_ms(d);
		OFF(SEND_PIN);
		
	}while(--i);
}
 */
ISR(INT0_vect){
/* 	bool high = READ(IN_PIN);
	
	if(listening){
		uchar l = getLength(del_cnt);
		
		if(l != 2){
			ON(CLOSE_PIN);
			if(l == rbi(CODE,bcnt)){
				ON(OPEN_PIN);
				bcnt++;
				if(bcnt==6){
					ON(TEST_PIN);
					locked=!locked;
					bcnt=0;
				}
				
			}
			else{
				
				OFF(OPEN_PIN);
				bcnt=0;
			}
		}
		else
			OFF(CLOSE_PIN);
		
	}
	else{
		listening=true;
		
	}

	last_del = del_cnt;
	del_cnt = 0;
	 */
 }


ISR(TIM0_COMPA_vect){
/* 	// if(del_cnt < 20000)
	millis++;	

	if(listening){
		del_cnt++;

		if(del_cnt > DEL_MAX){
			listening = false;
			del_cnt = 0;
			
			OFF(CLOSE_PIN);
			OFF(OPEN_PIN);
		}
	} */
}
