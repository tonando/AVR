#include <t85.h>
#include <EEPROM.h>
#include <string.h>

// #define ANAL_IN 	3	
#define B_REC		1
#define TEST_LED	2
#define VOUT		0

#define MIN_DELAY	600
#define REC_SIZE	448U

// uchar rec[64] = { 0, 6, 12, 18, 25, 31, 37, 43, 49, 55, 61, 68, 74, 80, 85, 91, 97, 103, 109, 114, 120, 125, 131, 136, 141, 146, 151, 156, 161, 166, 171, 175, 180, 184, 188, 193, 197, 201, 204, 208, 212, 215, 218, 221, 224, 227, 230, 233, 235, 237, 240, 242, 244, 245, 247, 248, 250, 251, 252, 253, 253, 254, 254, 255};
uchar rec[REC_SIZE];
// uchar idx;

uint length=0;
bool recording=0;
bool recorded=0;
bool overdub=0;

uint speed=100;
uint l_speed=speed;
ulong del=1000;

uint idx=0;
uchar aval,l_aval;
uchar dv = 2;

void pwm_setup (uchar d){
	cli();
	TCCR0A = (1<<COM0A0)|(1<<COM0A1)|(1<<WGM00); 
 	TCCR0B = d;
	OCR0A = 0;
	TCNT0 = 0;
	sei();
	// TCCR1 = 2;
	// TIMSK = B(TOIE1);
	// sei(); 
}
 
bool val_set(uint i, uchar v) {
	//if(i >= REC_SIZE /* || (recorded && i >= length) */) return false;
	
	rec[i] = v;
	
	return true;
}

uchar val_get(uint i) {
	uchar result = 0;
	if(i < length)
		result = rec[i];
	return (result);
}

void updateOutput(){
	if(recorded && !recording)
		OCR0A = val_get(idx);				
	else
		OCR0A = (aval);	
}
	  
void updateBPM(uint cnt){
	  uchar t = REC_SIZE>>4;
	  uchar l = cnt % t;
	  
	  if(l == 0)
		OFF(TEST_LED);
	
	  if(recorded){
		if(cnt > REC_SIZE-t && !(l&7))
			FLIP(TEST_LED);
		else{
			if(((cnt/t) & 3) == 3){
				if(l == (t>>1))
					ON(TEST_LED);
			}
			else{
				if(l == (t>>2))		// off at 1/4
					ON(TEST_LED);
			}
		}
	  }
	  else{
		  // if(recording && !(cnt&7))
			  // FLIP(TEST_LED);
		if(l >= (t>>2))		// off at 1/4
			ON(TEST_LED);
	  }		
}
  
void updateSpeed(){
	
	speed = adc_read10(3);
	if(speed == l_speed) return;

	// if(recorded)
		// speed = 300 - constrain(speed,1,250);
	// else
		// speed = constrain(speed,10,230);
		
	// speed += 800;
	// _delay_us(MIN_DELAY);
	del = 1040-speed;
	del *= 64;
	del += MIN_DELAY;
	// del = (128 * speed) + 1400;
	
	l_speed = speed;
	// del = constrain(del,2000,300000);
	// del = constrain(del,2000,300000);
}	

int main() { 
	
	OUTPUT(VOUT);
	BUTTON(B_REC);
	INPUT(3);
	INPUT(4);
	
	bool last_rec_btn=true;
	
	//memset(&rec,0,REC_SIZE);

	updateSpeed();
	
	dv = EEPROM_read(0);
	if(!dv || dv>5)
		dv = 2;
	
	pwm_setup(dv);
	uchar a = dv;
	
	while(!READ(1)){
		uchar a = adc_read(3) >> 4;
		a = constrain(a,1, 5);
		FLIP(TEST_LED);
		pwm_setup(a);
		OCR0A = a;
		delay_ms(20);
		a = adc_read(2); 
		delay_ms(20);
	}
	if(a != dv)
		EEPROM_write(0,a);

	OUTPUT(TEST_LED);
	ON(TEST_LED);
	
	while (1){
		
		aval = adc_read(2);
		recording = !READ(B_REC);
		
		// recording
		if(recording){
			
			// start
			if(!last_rec_btn && !recorded){
				idx = 0;
				length = 0;
			}
			
			val_set(idx,aval);
			
			if(!overdub){
				length++;
				
				// warning
				if((length > REC_SIZE-64 && !(idx&3)))
					FLIP(TEST_LED);
						
				// full
				if(length == REC_SIZE){
					OFF(TEST_LED);
					while(!READ(B_REC)) blink(TEST_LED,2);					
				}
				

			}	
				
		}
		else{
			// released / stop recording
			if(last_rec_btn){
				if(!recorded && length >= 6){
					recorded = true;
					overdub = true;
				}
				
			}

		}
	
		// play 
		updateOutput();
		updateBPM(idx);
		
		idx++;
					
		if(recorded){
			if(idx >= length)
				idx=0;
		}
		else{
			if(idx >= REC_SIZE)
				idx=0;
		}
		
		if(!recording)
			updateSpeed();
		// else
			// _delay_us(MIN_DELAY);
			
		l_aval = aval;
		last_rec_btn = recording;
		
		delay_us(del);
	}
	return 0;
 }
  

	  
	  
	  
	  
	  