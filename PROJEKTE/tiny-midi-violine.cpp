
#include <Macros.h>
#include "tiny_midi.h"

char buf[80];

int pitch;
uchar vol,last_vol;
volatile int length;
volatile int l_length;
volatile uchar cnt;
volatile uint ovf;
volatile ulong accum;
volatile bool tmo;
volatile bool ready;

bool playing = false;

int main (void)
{ 
	
	uchar cnt=0;
	chan=1;
	uint pitch=0;
	uint last_pitch=0;
	
	// BUTTON(2);
	BUTTON(2);
	// OUTPUT(0);
	

	
	tiny_serial_init();	// 9600
	serial_print("ok\r\n");
	delay_ms(100);
	
	// uint min=60000;
	// uint max=10;
#ifdef TIMER_MODE				
	MCUCR |= (1 << ISC00);
	GIMSK |= (1 << INT0);
#endif		
	// TCCR1 = B(CTC1) | 1;
	TCCR1 = 1;
	TCNT1=0;
	TIMSK |= B(TOIE1);
	
	// sei();
	while(1){
	
		if(!cnt)
			vol = adc_read(4);

#ifdef TIMER_MODE			
		if(tmo){
	//		serial_print("timeout\n\r");
	//		while(tmo);
		}
		else
		if(length != l_length && length > 0 && ready){
		
			
			// cli();
			if(length >= 90)
				length -= 90;
			length = MIN(4000,length);
			
			// if(!pitch) pitch = length;
			
				// if(	pitch < length) pitch++;
				// if(pitch > length) pitch--;
			// pitch = (pitch+length)>>1;
			if(ABS(length-l_length) < 500){
			// uint p = pitch-230;
			// p<<=1;
			// p=MIN(16000,p);
			sprintf(buf,"%d %d\n\r",(int)length, (length>>4));
			
			const char* os = buf;
			serial_print(os);
			}
			ready=false;
			// sei();
			// midi_out(0b11100000,(pitch&127),(pitch>>7)&127);
		}
#else
	
#endif		
		uint ain = adc_read10(3) << 4;
		
		last_pitch = pitch;
		pitch += ain;
		pitch >>= 1;
		
		if(pitch != last_pitch){
			// uchar pl = 127 & length;
			// uchar pm = 127 & (length >> 7);
			
			// midi_out(0b11100000,pl,pm);
			// if(playing)
			

		}
		
		if(vol){
			if(!playing){
				note_on(43,100);
				playing=true;
			}		
			if(vol != last_vol){
				set_ctrl(1,(vol>>1));
			}
		}
		else{
			if(playing){
				note_off(43);
				playing=false;
			}
		}
		/*
		*/
		
		cnt++;
	}
	
	return 0;
}

ISR(TIMER1_OVF_vect){
	if(ovf<1000)
	ovf++;
	else
	tmo=true;
// ISR(TIMER1_COMPA_vect){
	// if(vol)	vol--;
	// 2047
			// if(pitch < length) pitch++;
		// if(pitch > length) pitch--;
	pitch = (pitch+length)>>1;
}

ISR(INT0_vect) {
if(ovf || TCNT1 > 10){
	// last_vol = vol;
	// last_length = length;
	uint l = (uint)((ovf<<8) | TCNT1);
	
	if(l < 10000){
	
	if(cnt == 8){
		l_length=length;
		length=accum/8;
		accum=0;
		ready=true;
		tmo=false;
		cnt=0;
	}
	else{
		accum += l;
	}
	
	cnt++;
	
	}
	// length = (tmr<<8) | TCNT1;
	// tmr=TCNT1=0;
	// if(vol < 254) vol+=2;
	// if(vol < 127) vol = 127;
	}
	else 
		ready=false;

	TCNT1=0;
	ovf=0;
}