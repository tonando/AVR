#include <Macros.h>
#include <EEPROM.h>
#include <avr/sleep.h>

#define OPEN_PIN 3
#define CLOSE_PIN 4

#define WAKE_PIN 1
#define BTN_PIN 1
#define LIM_PIN 0

#define START_DEL 		100
#define STOP_DEL 		500
#define BOUNCE_DEL 		180

#define MOTOR_TIMEOUT 	350
#define TIMEOUT 		800

#define BAT_LOW			237

// #define LOCKED
bool locked = false;
volatile uchar correct = 0;

volatile uint del_cnt;
volatile uint last_del;

bool closed = true;
bool moving = false;
bool awake = true;

// bool end_stop = true;
bool btn_last = false;

bool check_stop(){

	return !READ(LIM_PIN);
}

void stop(){
	moving = false;	
	OFF(OPEN_PIN);
	OFF(CLOSE_PIN);
}
	
bool move(bool open){
	// sei();
	
	if(moving) 
		return false;
	
	moving = true;
	
	if(open){
		ON(CLOSE_PIN);	
	}
	else{
		ON(OPEN_PIN);
	}
	
	// minimum pulse
	_delay_ms(START_DEL);
	
	// limit triggered
	del_cnt = 0;
	while(!check_stop() && (del_cnt < MOTOR_TIMEOUT));

#ifdef BOUNCE_DEL	
	// if(del_cnt < TIMEOUT){
		PORTB ^= B(CLOSE_PIN) | B(OPEN_PIN);
		_delay_ms(BOUNCE_DEL);
	// }
#endif
	
	stop();
		
	_delay_ms(STOP_DEL);
	
	return true;
}

bool battery(){
	ADMUX = (1 << ADLAR)| B(REFS0) | (1);       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);

	//Start conversion
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 

	uchar a = (ADCH);
	if(a > BAT_LOW || a < 5)
		return true;
	cli();

	while(1){
		ON(CLOSE_PIN);
		_delay_ms(50);
		OFF(CLOSE_PIN);
		_delay_ms(500);
		ON(CLOSE_PIN);
		_delay_ms(50);
		OFF(CLOSE_PIN);
		_delay_ms(500);

	}
	return false;
}
void close(){
	// if(closed) return;
	move(false);
	closed = true;	
}
void open(){
	// if(!closed) return;
	move(true);
	closed = false;	
}
void gosleep(){
	// SLEEP MODE
	cli();
	stop();
 	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable(); 
	awake=false;
	sei();
	sleep_cpu();

}

int main(){
	OUTPUT(OPEN_PIN);
	OUTPUT(CLOSE_PIN);
	
	INPUT(1);
	// BUTTON(BTN_PIN);
	BUTTON(LIM_PIN);
	BUTTON(WAKE_PIN);
	
	
	GIMSK = B(INT0) | B(PCIE);
	PCMSK = B(WAKE_PIN);
	MCUCR = 2; // falling

	TCCR0B = 4;
	TIMSK0 = _BV(TOIE0);
	
	sei();
	
	if(!check_stop())
		close();
	
	gosleep();
 		
	while(1){
 		
 		// open / close button
		bool btn_down = !READ(BTN_PIN) | !READ(WAKE_PIN);
		
		if(btn_down != btn_last){		
			if(awake){
					
				if(btn_down){		
					battery();
					if(closed)
						open();
					else
						close();

				}
			}
		}
		
		btn_last = btn_down;
 
 }
	return 0;
}

ISR(PCINT0_vect){
	sleep_disable();
	awake=true;
}

ISR(INT0_vect){
	awake=true;
	// if(moving) return;
	// if(del_cnt < 2) return;
	// sleep_disable();
	 // if(locked){
			
		// if(correct > 3){
			// locked=false;
	// }
	 // else{
		// if(del_cnt>20){
			
			// if(closed)
				// open();
			// else
				// close();
		// }

	 // }
	 
	del_cnt = 0;
 }


ISR(TIM0_OVF_vect){

		del_cnt++;

		if(del_cnt > TIMEOUT){
			gosleep();
		}
}
