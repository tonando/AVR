	#include <Macros.h>
	#include <EEPROM.h>
	#include <beep.h>

	
	// PINS
	#define BUZZER_PIN 	1
	#define BTN_TEN 	4	// hold: 
	#define INT_PIN 	2
	#define MOTOR_PIN 	0
	#define BTN_START 	3	// hold: load last from eeprom
	
	#define BEEP_BUTTON beep(BUZZER_PIN,900,6)
	#define BEEP_BUTTON_LONG beep(BUZZER_PIN,1200,12)
	// #define BEEP_BUTTON_LONG {for(uchar i=1; i<20; i++) beep(BUZZER_PIN,1000+(i*100),200);}
	#define BEEP_HIGH beep_sweep(BUZZER_PIN,1800,2100,0)
	#define BEEP_LOW beep_sweep(BUZZER_PIN,1200,1000,0)
	
	// #define BEEP_ERROR beep_sweep(BUZZER_PIN,600,400,0)
	
	#define BEEP_ERROR {for(uchar j=8; j; j--) beep(BUZZER_PIN,3000-(j*200),500);}

	// SETTINGS
	#define ACCEL		1
	#define MIN_SPD		2
	#define MAX_SPD		15
	#define MAX_TURNS 10000

	void start_motor();
	void stop_motor();	
	void save();
	void load();
	void set_speed(uchar);
	// uchar old_pinb;

	volatile uchar spd_cnt;
	volatile bool mton;
	volatile uchar spd;
	volatile uint turns_remaining;
	uint turns;
	
	volatile ulong millis;
	volatile uint mcnt;
	
	volatile bool running;
	volatile bool paused;
	bool change_speed=true;
	bool changed;
	

	int main (void)	{	
		
		OUTPUT(MOTOR_PIN);
		OUTPUT(BUZZER_PIN);

		BUTTON(BTN_START) 
		BUTTON(BTN_TEN);
		INPUT(INT_PIN);
		// ON(INT_PIN);

		uchar last_pb = PINB;
		
 		// if(!READ(BTN_START))
			// load();
		// cli();
		
		
		// TCCR1 = _BV(CTC1)| 7; //256
		// OCR1A = 32;
		
		TCCR0B = 2;
		MCUCR = 2; // rising
		// MCUCR = 3; // rising
		// PCMSK = B(BTN_START) | B(BTN_TEN);
		
		_delay_ms(100);
		BEEP_HIGH;

		sei();
		
		while(1) {

		// start
		if(!READ(BTN_START)){


			if(running)
				stop_motor();		
			else{
				
				if(rbi(last_pb,BTN_START)){
					BEEP_HIGH;
					start_motor();
					
					while(!READ(BTN_START));
					_delay_ms(100);
				}
			}
			_delay_ms(100);

		}
		
		if(!running){
			
			if(!READ(BTN_TEN) && rbi(last_pb,BTN_TEN)){
				if(turns){
					BEEP_LOW;
					BEEP_LOW;
					turns = 0;
				}
				else{
					
					turns = 24;
					start_motor();
					_delay_ms(1000);
				}
			}
			if(!READ(INT_PIN) && rbi(last_pb,INT_PIN)){
				uchar hold_timer = 50;
				
				while(!READ(INT_PIN) && hold_timer){
					if(hold_timer)
						hold_timer--;
					_delay_ms(10);
				}
				
				if(hold_timer){
					
					turns++;
					BEEP_BUTTON;
				}
				else{
					
					turns *= 10;
					BEEP_BUTTON_LONG;
					_delay_ms(200);
				}
				
				changed=true;
				_delay_ms(200);
			}
		}
/* 
			if(!READ(INT_PIN)){
				BEEP_BUTTON;
				turns_remaining++;
				changed=true;
				
				_delay_ms(700);
				while(!READ(INT_PIN));
			}
			 */
		 last_pb = PINB;

		}
		return 0;
	}


ISR(TIMER0_OVF_vect){
	if(running){
		spd_cnt++;
		spd_cnt&=15;


		if(spd > spd_cnt  && !mton){
			ON(MOTOR_PIN);
			mton=true;
		}
 
		if(spd <= spd_cnt && mton){
			OFF(MOTOR_PIN);
			mton=false;
		}
		
		if(!(spd_cnt&3)){
			mcnt++;
			
			// if(millis<10000)
			millis++;
		}
		// if((millis & 255) == 0) BEEP_BUTTON;	
	}
	// else
		// BEEP_ERROR;
		 
}

ISR(INT0_vect){
		if(running){
			// uint ms = millis;
			uint m = mcnt;
			mcnt = 0;
			
			// debounce
			// if(m<10)
				// return;
			/* 
			
			if(ms > 650){
				spd++;
			}
			if(ms < 50){
				spd = MIN_SPD;
				// BEEP_HIGH;
			}
			 */
			if(turns_remaining){
				
				turns_remaining--;
 				
				if(change_speed){
					if(turns_remaining<20){
						spd=MIN(spd,(MAX_SPD>>1));
						// if(turns_remaining<8)
							// spd=MIN_SPD;
					}
					else{
						if(millis >= 300){
							
							millis=0;
							
							if(spd<MAX_SPD){
								// if(m > 150)
									spd+=ACCEL;
								
								if(m < 60)
									spd>>=1;
								
								
							}
						}
					
					}
				}
				
				if(m > 450)
					spd<<=1;
				
				spd=constrain(spd,MIN_SPD,MAX_SPD);

 			}
			else{
				// done
				stop_motor();
			}
	}
}

void start_motor(){

	if(!turns || turns > MAX_TURNS) {
		BEEP_ERROR;
		return;
	}
	
	// if(changed)
		// save();
	
	// beep_sweep(BUZZER_PIN,880,880+(turns_remaining)*2,0);
	
	// ON(MOTOR_PIN);
	_delay_ms(200);
	
	// OCR1A = 32;
	
	GIMSK = B(INT0) /* | B(PCIE) */;
	TIMSK = B(TOIE0);// | B(OCIE1A);
	// set_speed(MIN_SPD);

	TCNT0 = 0;
	millis=0;
	mcnt=0;
	spd_cnt=0;
	turns_remaining = turns+1;

	running=true;

	if(turns < 20){
		change_speed = false;
		spd = MAX_SPD>>1;
	}
	else{

		change_speed = true;
		spd = MIN_SPD;
	}

	// if(turns_remaining < 20)
		// spd<<=1;
	

}
void stop_motor(){
	
	running=false;
	turns_remaining=0;

	TIMSK = 0;
	GIMSK = 0;
	
	OFF(MOTOR_PIN);

	// BEEP_HIGH;
	BEEP_LOW;

	_delay_ms(1000);
	
}
ISR(PCINT0_vect){
	// stop_motor();
}
/* 
void set_speed(uchar _s){
	spd = constrain(_s,MIN_SPD,MAX_SPD);
}		
 */
void load(){
	uchar bh = (uchar)(EEPROM_read(0x01));
	uchar bl = (uchar)(EEPROM_read(0x00));
	turns = bl | (bh<<8);

	BEEP_HIGH;
	_delay_ms(10);
	start_motor();
}
void save(){
	changed=false;
	
	// save to eeprom
	uchar bh = (uchar)(turns >> 8);
	uchar bl = (uchar)(turns & 0xff);
	EEPROM_write(0x00,bh);
	EEPROM_write(0x01,bl);
	
	beep(BUZZER_PIN,4000,3);
	_delay_ms(200);
}
