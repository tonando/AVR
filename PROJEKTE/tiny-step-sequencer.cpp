#include "T45.h"
#include "delay.h"

#define CLOCK 	PB2
#define MISO 	PB3
#define MOSI 	PB1
#define LATCH 	PB0

#define STEP_DIVIDER	16
#define BYTE_LENGTH 	8
#define PATTERN_LENGTH 	4
#define STEPS_LENGTH 	BYTE_LENGTH * PATTERN_LENGTH

// register unsigned char counter asm("r3");

// uchar patterns[PATTERN_LENGTH];
uchar steps[STEPS_LENGTH];

uchar c_step;//,c_substep,c_pattern;
// uchar buttons, last_buttons;
uchar speed = 200;

// volatile uchar out_byte, in_byte, cnt_shift;

bool playing = true;
bool recording = false;
/* 
void nextStep(){
	c_step++;
	if(c_step >= STEPS_LENGTH)
		c_step = 0;
	
	c_pattern = c_step / BYTE_LENGTH;
}
void nextSubStep()
{
	uchar step = steps[c_step];
	if(step){
		// write_pwm(step);
		
	}
	
	// update_leds();
	out_byte=patterns[c_pattern];
	c_substep++;
	
	// End of step
	if(c_substep == STEP_DIVIDER)
		nextStep();
} */
// Update
// void update_leds(){
	// uchar offset = c_step >> 3;
	// pattern[offset] = STEP_DIVIDER - c_substep;
// }
/* 
void edit_step(uchar _s, uchar _val){
	steps[_s] = _val;
}	
void toggle_steps(uchar _s){
	uchar i = 8;
	do{i--;
		if(rbi(_s,i))
			fbi(steps[i + c_pattern*8],7);
	}while(i);
	
}
void edit_steps(uchar _s, uchar _val){
	uchar i = 8;
	do{i--;
		if(rbi(_s,i))
			edit_step(steps[i + c_pattern*8],_val);
	}while(i);
}
 */
int main(void){
	// DDRB=0xfe ^ _BV(MISO);
	// PORTB=_BV(LATCH) | 0x01;
	DDRB = _BV(PB1);
	ON(PB0);
	ON(PB2);
	
	// TCCR0B = 0x02;
	// TIMSK |= _BV(TOIE0);
	// sei();
	// init_buttons(1);

	adc_setup(APB4);
	pwm_setup(1);
	
	steps[4] = 255;
	steps[1] = 25;
	
	while(1){
		recording = !rbi(PINB,0);
		playing = rbi(PINB,1);
			
		// uchar btns = (in_byte ^ last_buttons);
		// last_buttons = in_byte;
		
		// buttons
		// if(btns){
		uchar a = adc_read() >> 2;

		if(recording){
			steps[c_step] = a;
			// edit_steps(btns,a);
		}
			// else{
				// toggle_steps(btns);
			// }
		// }
		if(playing){		
			// Next substep
			pwm_write(steps[c_step]);
			// nextSubStep();
			c_step++;
			if(c_step >= STEPS_LENGTH)
				c_step=0;
		}
		else{
			speed = MAX(a,1);	
			
		}
		
		delay(speed);
		
	}
	
	return 0;
}


// Settings
/* 
ISR(TIMER0_OVF_vect){
	uchar b = cnt_shift & 7;
	
	if(cnt_shift < 8){
		if(steps[b] > 100)
			ON(MOSI);
		else
			OFF(MOSI);				
		
	}
	else{
		if(b == (c_step & 7))
			ON(MOSI);
		else
			OFF(MOSI);		
	}

	OFF(CLOCK);
	ON(CLOCK);

	if(READ(MISO))
		sbi(in_byte,b);
	else	
		cbi(in_byte,b);
	
	cnt_shift++;
	cnt_shift&=15;
	
	if(!cnt_shift){
		OFF(LATCH);
		ON(LATCH);
	}
	
} */