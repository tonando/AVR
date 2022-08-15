#include <Macros.h>

#define TIMEOUT 		500
#define KEYS	 		4
#define LOCKED			true

#define PIN_LR	 		PB0
#define PIN_LG	 		PB1
#define PIN_REL			PB4
#define BUZZ			PB2

// OCR0A = 255 - 63 * keys;	


bool locked = LOCKED;
uchar last_keys, keys;

uchar getKeys(){
	INPUT(3);
	uchar k = adc_read(3);
	uchar result=0;

	if(k > 200) return 0;
	
	if(k < 150) result++;
	if(k < 100) result++;
	if(k < 70) result++;
	if(k < 56) result++;
	
	
	return constrain(result,0,4);
}
void ready() { beep(44,1);beep(48,1); }
void alarm() { 
	_delay_ms(TIMEOUT); 
	while(getKeys()){ 
		beep(4,getKeys(),8); 
		// beep(14,keys,1); 
		_delay_ms(TIMEOUT); 
		OCR0A ^= 0xff; 
	}
	last_keys=0;
	// OCR0A=244;
	ready();
}
int main(){
	
	// OUTPUT(PIN_LG);
	OUTPUT(PIN_LR);
	OUTPUT(PIN_REL);
	OUTPUT(BUZZ);

	TCCR0A = (1<<COM0A0)|(1<<COM0A1)|(1<<WGM00); 
 	TCCR0B = 1;
	OCR0A = 244;
	TCNT0=0;
	
	uchar cnt=0;
	
	alarm();	
	
	while(locked){
		OFF(PIN_REL);
		
		_delay_ms(8);
		OCR0A = cnt<128 ? cnt + 64 : 64+128 -(cnt&127);
		cnt++;
		
		last_keys = keys;
		keys = getKeys();

		if(keys && !last_keys){
			OCR0A=255;
			beep(32,1);
			_delay_ms(TIMEOUT);
			beep(32,1);
			keys = getKeys();
			
			_delay_ms(TIMEOUT);
			
			if(keys >= KEYS && keys == getKeys())
				locked = false;
			else
				beep(16,keys,8);
			
			_delay_ms(TIMEOUT);
				
			// access denied
			if(locked){
				OCR0A=0;
				beep(8,0,12);
				beep(4,0,12);
				beep(2,0,20);

				_delay_ms(TIMEOUT*4);
				// _delay_ms(TIMEOUT);
				alarm();
			
			}
		
		}		
	}

	// success
	for(uchar i=32; i<54; i++){
		// OCR0A=i<<4;
		beep(i);
	}
	beep(58,10,1);

	_delay_ms(1000);
	ON(PIN_REL);
	// _delay_ms(1000);
	// OFF(PIN_REL);
	
	OCR0A=255;
	
	return 0;
}
/* 
	del_timeout++;

	if(del_timeout > TIMEOUT){
		
		if(keys){
			// cli();
			if(keys != 0x0f){
			OFF(4);
			keys=0x0f;
			}
		}
		else{
			ON(4);
			locked=false;
		// sei();
		}
		
	}
}
 */