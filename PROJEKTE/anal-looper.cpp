#include <t85.h>
#include <EEPROM.h>
//#include <fastA2D.h>
// #include "tiny_serial.h"
// #define B_SPD		PB1
// #define ANAL_IN 	3	
#define B_REC		PB1
#define TEST_LED	PB2
#define VOUT		PB0

#define MIN_DELAY	2400

// #define SAVE_DATA
// #define SHARE_AIN

int8_t abhol;

#if defined(__AVR_ATtiny45__)
	#define REC_SIZE	220
// #endif
#elif defined(__AVR_ATtiny85__)
	#define REC_SIZE	460
#else
	#error MCU NOT SUPPORTED
#endif

// uchar rec[64] = { 0, 6, 12, 18, 25, 31, 37, 43, 49, 55, 61, 68, 74, 80, 85, 91, 97, 103, 109, 114, 120, 125, 131, 136, 141, 146, 151, 156, 161, 166, 171, 175, 180, 184, 188, 193, 197, 201, 204, 208, 212, 215, 218, 221, 224, 227, 230, 233, 235, 237, 240, 242, 244, 245, 247, 248, 250, 251, 252, 253, 253, 254, 254, 255};
uchar rec[REC_SIZE];
uchar idx;
// volatile uchar out;
// volatile uchar cnt;
int length;
bool recording=false;
uint speed=100;
ulong del=1000;

uchar saves=10;
bool recorded=false;
bool overdub=false;

 void pwm_setup (void){
	TCCR0A = (1<<COM0A0)|(1<<COM0A1)|(1<<WGM00); 
 	TCCR0B = 1;
	OCR0A = 0;
	TCNT0 = 0;
	TCCR1 = 2;
	TIMSK = B(TOIE1);
	sei(); 
}
 
  
bool val_set(uchar i, uchar v) {
	if(i >= REC_SIZE /* || (recorded && i >= length) */) return false;
	
	rec[i] = v;
	
	return true;
}

uchar val_get(uchar i) {
	uchar result = 0;
	if(i < length)
		result = rec[i];
	return (result);
}

#ifdef SAVE_DATA
void loadRom(){
	length = EEPROM_read(0) << 8;
	length |= EEPROM_read(1);
	
	if(length){
		for(int i=0; i<length; i++){
			uchar v = EEPROM_read(2+i);
			val_set(i,v);
		}
		
		recorded=true;
		if(length >= (REC_SIZE-16))
			overdub=true;
	}
}	

void saveRom(){
	ON(TEST_LED);
	_delay_ms(100);
	
	if(!saves) return;
	
	if(length){
		uchar l = EEPROM_read(0) << 8;
		l |= EEPROM_read(1);

		if(l != length){
			EEPROM_write(0,(length>>8));
			EEPROM_write(1,(length&0xff));
		}
		for(int i=0; i<length; i++){
			uchar e = EEPROM_read(2+i);
			uchar v = val_get(i);
			if(v != e){
				EEPROM_write(i+2,v);
				OCR0A = v;
				FLIP(TEST_LED);
			}	
			
			_delay_ms(10);
			OCR0A = 255 - (i&7)*16;
		}
	}
	OFF(TEST_LED);
	_delay_ms(1000);
	
	saves--;
}	

void deleteRom(){
	for(int i=0; i<512; i++){
		if(!(i&7))
			FLIP(TEST_LED);
			
		_delay_ms(2);	
		EEPROM_write(i,0);
	}
}	
#endif


void updateSpeed(){
#ifdef SHARE_AIN
	if(recording) return;
	
	uchar s=adc_read(2);
	
	if(abhol){
		if(abhol==1 && s < speed) return;
		else abhol = 0;
		if(abhol==-1 && s > speed) return;
		else abhol = 0;
	}
	
	speed = 300 - s;
#else
	speed = 300 - adc_read(3);
#endif	
	if(recorded)
		speed = constrain(speed,1,254);
	else
		speed = constrain(speed,10,230);
		
	// speed += 800;
	// _delay_us(MIN_DELAY);

	del = (128 * speed) + 1400;
	// del = constrain(del,2000,300000);
	// del = constrain(del,2000,300000);
}	
int main() { 
	
	OUTPUT(VOUT);
	OUTPUT(TEST_LED);
	BUTTON(B_REC);
	INPUT(3);
	INPUT(4);
	
	uchar aval = 0;
	uchar last_aval = 0;
	uchar repeats=0;
	uint cnt=0;
	uchar changes=0;

	bool last_rec_btn=true;
	
#ifdef SAVE_DATA
	if(!READ(B_REC)){
		deleteRom();
		while(!READ(B_REC));
	}
	else
		loadRom();
#endif	

	updateSpeed();
	pwm_setup();

	while (1){
		
		aval = 255-adc_read(2);

		recording = !READ(B_REC);
		
				// play 
		if(!recorded)
			OCR0A = (aval);	
		else
			OCR0A = val_get(idx);			
			
		// recording
		if(recording){
			
			
			
			// abhol = 0;
			// if(aval > speed) abhol = 1;
			// if(aval < speed) abhol = -1;
			
			// start
			if(last_rec_btn && !recorded){
				idx = cnt =	repeats = 0;
			}

			// if(aval == last_aval){
				// if(repeats < 100)
					// repeats++;
				// if(cnt&1) FLIP(TEST_LED);
			// }
			// else{
				// if(repeats){
					// rec[idx] = (repeats) + 128;
					// repeats=0;
				// }
				// else{
					// rec[idx] = overdub ? (rec[idx] + (aval)) >> 1 : (aval);
					
	// if(val_get(idx) != aval){
		
		// if(changes < 250 && !(cnt&3))
			// changes++;
		
	// }
					
			if(val_set(idx,MAX(1,aval))) OFF(TEST_LED);
			else ON(TEST_LED);
				// }
			// }
			
			if(!overdub){
				length++;
				
				if((length > REC_SIZE-64 && !(cnt&3)))
					FLIP(TEST_LED);
				// else if((idx&31)==30)
					// ON(TEST_LED);
					
				if(length  > 16){
					overdub = true;
					recorded = true;
				}		
				
				// full
				if(length == REC_SIZE){
				
#ifdef SAVE_DATA					
					saveRom();
#endif
					OFF(TEST_LED);
					while(!READ(B_REC)) blink(TEST_LED,2);					
				}
				

			}	
				
		}
		else{
			// released
			if(!last_rec_btn){
				if(length >= 6){
					recorded = true;
					// saveRom();
				}
				else
					length = 0;
					
				if(changes >= 200){
					changes=0;
#ifdef SAVE_DATA					
					saveRom();
#endif
				}

			}

			if(repeats)
				repeats--;
				
				
			uchar t = (cnt>>4);
			
			if(!(cnt&7))
				OFF(TEST_LED);
			if((cnt&1) && t>2)
				ON(TEST_LED);
			// else if(t>=5 && cnt >= 40)
				// else
				// if((cnt & 15) == 1 && ((cnt>>3)&3) >= 2)
				// else if(((cnt) & 63) == 10)
					// OFF(TEST_LED);
			// }
		}
	
		
		
	
		if(repeats) 
			repeats--;
		else
			idx++;
			
		if(idx >= REC_SIZE || (recorded && idx >= length))
			idx=0;
			
		delay_us(del);
		
		cnt++;
		if(cnt >= REC_SIZE)
			cnt=0;
		
		if(!(cnt&15) && !recording)
			updateSpeed();
		else
			_delay_us(MIN_DELAY);
			
		last_aval = aval;
		last_rec_btn = recording;
	}
	return 0;
 }
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  