#include <digitled.h>
#include <eeprom.h>

#define BTN 		PB4
#define TIME 		60
#define NUM_DIGITS	6

#define DEL_MS  (((TIME * 100UL) / NUM_DIGITS)>>7)

// todo: ziffer segment f[r segment, fade in digits
void show_digits(uchar n);

uchar ain;
uchar seconds;
uchar digits[NUM_DIGITS]={7,4,3,5,8,3};

void setup(){
	
	print("setup   ");
	while(!READ(BTN));
	delay_ms(500);	
	
	clear();
		
	do{
		uchar a = adc_read(3);
		if(a == ain) _delay_ms(1);
		
		ain += a;
		ain >>= 1;
		
		write_digit(8,ain/100);
		write_digit(7,ain/10);
		write_digit(6,ain);
		
	
	}while(READ(BTN));
	
	EEPROM_write(0,ain);
	
	blink(22);
	delay_ms(1000);
	clear();

	show_digits(NUM_DIGITS);
	
	blink(22);
	while(READ(BTN));
	
	_delay_ms(50);
	
}

// #ifdef RANDOMIZE		
void randomize(){
	for (uchar i=0;i<NUM_DIGITS;i++){
		uchar ra = (lfsr()%10);
		digits[i]=ra;
		EEPROM_write(1+i, ra);
	}
}
// #endif

void show_digits(uchar n){
	if(!n) return;
	
	for (uchar i=0;i<n;i++)
		write_digit(8-i,digits[i]);
}

void psst(){

	print("pssst!!!");	
	blink(20);
	seconds = TIME;	
}

int main(){
	// randomize();
	init_digits(0,1,2);
	
	INPUT(3);	
	BUTTON(BTN);
	
	_delay_ms(1);
		
	uchar level = EEPROM_read(0);
	
	if(!level){
		level=adc_read10(3)>>2;
		EEPROM_write(0,level);
	}
#ifndef RANDOMIZE		
	for (uchar ei=0;ei<NUM_DIGITS;ei++){
		digits[ei] = EEPROM_read(1+ei);
	}
#endif		
	uchar cnt=0;
	uchar lcnt=0;
	
	psst();
	_delay_ms(500);
	uchar digs = 0;
	
	while(1){
		
		clear();

		ain = adc_read(3);

		if(!READ(BTN))
			setup();
		
		if(lcnt)
			print("!!!!!!  ");
			
		// uchar digs = (TIME+1-seconds) / (TIME / NUM_DIGITS);
		
		if(digs < NUM_DIGITS)
			write(8-digs, (2<<(cnt%7)));//(lfsr() & 127));
			
		show_digits(digs);
			
		write_digit(2,seconds/10);
		write_digit(1,seconds%10);
		
		// if(lcnt)
			// blink(1);
					
		_delay_ms(DEL_MS);
		
		cnt++;
		if(!(cnt&63)){
			seconds--;
			if(seconds%10==0)
				digs++;
			// lcnt--;
		}
		
		
		// success
		if(!seconds){
			clear();
			show_digits(NUM_DIGITS);
			blink(20);
				
			while(READ(BTN));
		}
		
		// fail
		if((ain-12) > level && seconds < 60){
			blink(1);
			seconds++;
			// digs--;
			// lcnt++;
		}
			
		// if(lcnt > 100){
	// #ifdef RANDOMIZE		
			// randomize();
	// #endif
			// psst();
		// }
	
		
	}
	return 1;
}