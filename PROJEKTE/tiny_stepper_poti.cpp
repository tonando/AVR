#include "Macros.h"

// #include "stepper_motor_control.h"
// #include <util/delay.h>

#include "stepper_motor.h"
#include "eeprom.h"

#define A_MIDDLE	512
#define THRES 		16

#define PULSE_MULT 	40
#define PULSE_MAX 	(40000)
// #define PULSE_DIV 	32

bool dir;     
uint PULSE_MIN = 800;
/* 
void setup(){ 
	if(adc_read(2) < 250){
		if(EEPROM_read(3)){
			PULSE_MIN = EEPROM_read(0) << 8;
			PULSE_MIN |= EEPROM_read(1);
			PULSE_MIN = MAX(PULSE_MIN,100);
		}
		return;
	}
		
	uchar c=0;
	while(1){
	
		uint v = 10 + adc_read10(2) * 4;
		
		step(1);
		delay_us(v);
		
		c++;
		if(!c){
			_delay_ms(200);
			EEPROM_write(0,(v>>8));
			EEPROM_write(1,(v&255));
			EEPROM_write(3,true);
		}
		
	}
}
 */
int main(void){ 
	
	stepper_init(0,1,3,2);
	INPUT(4);
	_delay_ms(10);
	// setup();
	
	// for(int i=0;i<2000; i++){
		// step(1);
		// _delay_ms(1);
	// }
	// _delay_ms(100);
	
	uint spd=0;
	bool on=false;
	
	while(1){
		spd = adc_read10(2);					
			
		if(spd > A_MIDDLE){
			dir = true;
			spd = spd - A_MIDDLE;
		}
		else{
			spd = A_MIDDLE - spd;
			dir = false;
		}
	
		if(spd>=THRES){
			if(on){
				spd -= THRES;

				uint d = (A_MIDDLE - spd) * PULSE_MULT + PULSE_MIN;
				
				step(dir);	
				delay_us(d);
			}
		}
		else{
			on=true;
			_delay_ms(10);
			PORTB &= 0xf0;	
		}

 }
 return 0;
}