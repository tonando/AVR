
#include <Macros.h>
#include <EEPROM.h>

#define BUFFER_SIZE 450

#define ACH 	2	// PB4	
#define OUT_A 	1	// OC0B  
// #define LED 	4	// OC0B  

#define B_REC	0	// alt: ? freilauf
#define B_SAVE	2	// alt: clear
#define B_SPEED	3	// alt: ext_clock


uchar buffer[BUFFER_SIZE];
uint index;
uchar last_value;

uchar speed;
uchar spd_cnt;
uint length;

bool first=true;
bool recording=false;
bool ext_clock=false;

bool saved=true;
void save();
bool load();

void init_buffer();
void init_pwm();
void set_pwm(uchar);

int main (void)
{
	
#ifndef DYN_LENGTH
	length = BUFFER_SIZE;
#endif
	speed = 127;
	
	BUTTON(B_REC);
	BUTTON(B_SPEED);
	BUTTON(B_SAVE);

	OUTPUT(OUT_A);
	// OUTPUT(LED);
	
	// ADC
	ADMUX = (1 << ADLAR) | (1 << MUX1);       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);
	
	delay_ms(10);
	
	// startup options
	if(!READ(B_SPEED))
		ext_clock = true;
		
	if(!load() || !READ(B_SAVE))
		init_buffer();
	
	uchar ain=0;
	uchar old_pb=PINB;
	uchar value=0;
	last_value=buffer[index];;
		
	init_pwm();
	
	while( 1 ) {

		
		// ain
		ADCSRA |= (1 << ADSC);         // start ADC measurement
		while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 
		ain = ADCH;

		// Buttons
		uchar pb=PINB;
		uchar pb_d=pb^old_pb;
		// uchar pressed=pb_d & (~pb);
		
		
		// record
		recording = !READ(B_REC);
		
		if(recording){
			saved = false;
			buffer[index] = ain;
#ifdef DYN_LENGTH
			if(first)
				length++;
#endif
		}
		else{	
#ifdef DYN_LENGTH
			// set length
			if(!rbi(pressed,B_REC)); 
				first = false;
#endif
			if(ext_clock == false){

				// speed
				if(!READ(B_SPEED)){
					speed = constrain(ain,2,250);
				}
				
			}
		}
		
		value = buffer[index];	
		
		// save
		// if(rbi(pressed,B_SAVE))
			// save();

		// delay
		if(ext_clock){
			while(READ(B_SPEED));
			_delay_us(100);
		}
		else{
			
			set_pwm(value);			
				
			uint d = 800 + (speed << 6);
			delay_us(d);
				
		}
		

		last_value=value;
		old_pb=pb;
		
		index++;	
		if(index == length)
			index = 0;		
		
		// if(!(index&15))
			// fbi(PORTB,LED);
		
	}

  return 0;
}


void init_pwm(){
	
	
	DDRB|=B(OUT_A);

	TCCR0A = (1<<COM0B1) | 3; // fast pwm, clear pb1
	TCCR0B = 1;
	OCR0B=0;
}
void set_pwm(uchar _v){
	OCR0B = _v;		
}
bool load(){

	bool loaded = false;
	
	uint i=BUFFER_SIZE;
	speed = EEPROM_read(i);
	do{i--;
		buffer[i] = EEPROM_read(i);
		if(buffer[i])
			loaded = true;
	}while(i);
	
	return loaded;
} 

 void save(){
	 
	 
	OCR0B = 0;

	if(!saved){
		uint i=BUFFER_SIZE;
		EEPROM_write(i,speed);
		do{ i--;
			EEPROM_write(i,buffer[i]);
			_delay_ms(1);
			OCR0B = (i>>1);
		}while(i);	
		
		_delay_ms(200);
		saved = true; 
	} 
	else{
		
		// blink
		uchar i=8;
		do{i--;
			OCR0B=255;
			_delay_ms(100);	
			OCR0B=0;
			_delay_ms(100);	
		}while(i);
		
		_delay_ms(1000);
	}


	
		
 }
void init_buffer(){
	uint i=BUFFER_SIZE;
	do{i--;
		buffer[i] = i&0xff;
	}while(i);
	speed=10;
} 