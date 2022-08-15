#include <util/delay.h>
#include <Macros.h>
#include <EEPROM.h>
	

#define BUZZER_PIN 	PB4
#define BTN_TEN 	PB3
#define INT_PIN 	PB2
#define MOTOR_PIN 	PB1
#define BTN_START 	PB0		// hold: load last from eeprom

#define MIN_SPD		20
#define MAX_SPD		100
#define START_PULSE	30		// ms
#define MAX_TURNS 20000
#define DEL_BEEP 4

// #define BEEP 


// uint read_adc();
void set_speed(uchar);
void start();
void stop();
void check_buttons();
void beep(uchar);

uchar last_btn_start;
uchar last_btn_ten;
bool running;

uchar spd;
volatile uint turns = 50;

int main (void)	{	


	TCCR0A = _BV(COM0B1) | (3); //fast pwm
	TCCR0B = 1;
	
	GIMSK  |= _BV(INT0);
	MCUCR = 2;	// falling
	
	last_btn_ten = 1;
	last_btn_start = 1;
	
	DDRB = _BV(MOTOR_PIN) |_BV(BUZZER_PIN);
	PORTB |= _BV(BTN_TEN) | _BV(BTN_START) | _BV(INT_PIN);
	
	start();
	
	sei();
		
	while( 1 ) {
				
		// Buttons
		check_buttons();
	
		if(turns < spd) 
			spd=turns;
		else{
			if(spd < MAX_SPD)
				spd++;
		}
		set_speed(spd);

		// _delay_ms(10);
	}
	return 0;
}
void save(){
	// save to eeprom
	uchar bh = (uchar)(turns >> 8);
	uchar bl = (uchar)(turns & 0xff);
	EEPROM_write(0x00,bh);
	EEPROM_write(0x01,bl);
	_delay_ms(10);
}
void start(){
	if(!turns) return;
	save();

	running=true;
	// pulse
	set_speed(MAX_SPD);
	_delay_ms(START_PULSE);
	
	set_speed(MIN_SPD);
}
void stop(){
	set_speed(0);
	running=false;
	_delay_ms(200);
}
		
void set_speed(uchar _s){
	spd = MIN(_s,MAX_SPD);
	OCR0B = spd;
}
void check_buttons(){

	uchar b_start = READ(BTN_START);
	//	state changed
	if(last_btn_start |= b_start){
		last_btn_start = b_start;
		if(b_start) return;
		// BTN_START
			if(running){
				stop();
				beep(50);
			}
			else{				
				// hold / press 
				uchar i=255;
				do{
					_delay_ms(5);
					i--;
				}while(!READ(BTN_START) && i);
				
				if(!i){
						turns = EEPROM_read(0x00);
						turns <<= 8;
						turns |= EEPROM_read(0x01);
						beep(20);
				}
			
				// debounce
				_delay_ms(200);
				
				start();
			}
	}
	
	// BTN_TEN
		uchar btn_ten = READ(BTN_TEN);	
		if(last_btn_ten |= btn_ten){
			last_btn_ten = btn_ten;
			if(btn_ten)return;
			
			if(turns) 
				turns *= 10;

			beep(30);
			
			// debounce
			_delay_ms(200);
			
		}	
	
}

void beep(uchar _f){
#ifdef BEEP
	uint l=300 - _f;
	uchar f=_f;
	do{
		do{
			ON(BUZZER_PIN);
			_delay_us(DEL_BEEP);
			OFF(BUZZER_PIN);
			_delay_us(DEL_BEEP);
		}while(--f);
		f=_f;
	}while(--l);
#endif
}

ISR(INT0_vect){
	OCR0B=255;
	_delay_ms(100);
	OCR0B=1;
	_delay_ms(300);
	reti();
	if(running){
		if(turns){
			turns--;
			set_speed(MIN_SPD);
		}
		else{
			stop();
		}
	}
	else{
		if(turns < MAX_TURNS)
			turns++;
		// beep(160);
	}
	
	
}




/* 
uint get_henry(){
				// ON(READ_PIN);
			
			//charge 
			ON(PULSE_PIN);
			_delay_us(5);
			OFF(PULSE_PIN);
			_delay_us(5);
			
			//read
			read_adc();
			
			// discharge
			// OFF(READ_PIN);
			_delay_us(50);
}
 */
/* 

 uint read_adc(){

	// set up ADC
    ADMUX |= 3;
    // ADMUX |= (1 << ADLAR);
	
	// set up ADC
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // 1/128 prescaler
	ADCSRA |= _BV(ADEN); // enable ADC

	// Start the conversion
    ADCSRA |= (1 << ADSC);
 
    // Wait for it to finish
    while (ADCSRA & (1 << ADSC));
	uint result = ADCH;
	ADCSRA &= ~(_BV(ADEN)); // enable ADC
	
    return result;
}

 */