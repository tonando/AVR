
#include <Macros.h>
// #include "tiny_serial.h"

#define LOCK 	PB4
#define BUZZ 	PB2
#define LEDR 	PB1
#define LEDG 	PB0

#define TIMEOUT 		2200
#define HOLDTIME 		220
// #define STAY_UNLOCKED	1

#define CLEN 4
uchar CODE[CLEN] = {0,4,3,6};
uchar cnum;
uchar holding;
bool active;

uint timeout;
uchar ain,lain;

void fail();
void success();
bool update();

int main (void){ 
	OUTPUT(LOCK);
	SET(LOCK,0);

	OUTPUT(BUZZ);
	OUTPUT(LEDR);
	OUTPUT(LEDG);
	
	uint cnt=0;
	
	ON(LEDR);

	beep(6,1);
	beep(12);
	
	// while( !update());
	OFF(LEDR);
	
	while(1){
		
		// heartbeat
		if(!cnt && !timeout){
			ON(LEDR);
			// OFF(LEDG);
		}
		if(cnt== 4){
			OFF(LEDR);
		}
		
		if(cnum){
			if(!(cnt&255))
				ON(LEDG);
			else if ((cnt&255) == 127)
				OFF(LEDG);
		}
		
		
		// bewegt
		if( update() ){
			ON(BUZZ); _delay_us(512); OFF(BUZZ); _delay_us(256);	
			holding=0;
			active=true;
		}
		
		// nicht bewegt
		else{
			if(active){
				if(holding == HOLDTIME){
					// holding=0;
					
					// richtige ziffer
					if(ain == CODE[cnum]){
						timeout=TIMEOUT;
						cnum++;
						ON(LEDG);
						// fertig
						if(cnum==CLEN){
							success();
						}
						else
							beep(4+cnum,1);
							beep(8+cnum);
							beep(12+cnum);
						
						active = false;
						
						_delay_ms(64);
						OFF(LEDG);
					}
					// falsch
					// else{
						// if(cnum)
							// fail();
					// }
				}
				else
					holding++;	// hold timer
			}
		}
			
		_delay_ms(4);
		cnt++; cnt &= 1023;
		
		if(timeout){
		
			timeout--;
			
			// blink
			if(timeout < (TIMEOUT>>1) && !(timeout&31)){
			
				if(timeout&1)
					FLIP(LEDG);
				
				if(timeout < (TIMEOUT>>2))
					FLIP(LEDR);
			}
				
			// reset
			if(!timeout){
				fail();
			}
		}
	}
	
	return 0;
}

void fail(){
	OFF(LEDG);
	ON(LEDR);
	beep(7);
	beep(6);
	beep(5);
	beep(4);
	beep(3);
	beep(2);
	_delay_ms(1000);				
	OFF(LEDR);
	// _delay_ms(500);				

	cnum=0;
	timeout=0;
	holding=0;
	active=false;
}
void success(){

	OFF(LEDR);
	ON(LEDG);
	beep(7,3);
	beep(9,3);
	beep(14);
	FLIP(LOCK);
	_delay_ms(2000);
	
#ifdef STAY_UNLOCKED
	while(1){
		ON(LEDR);	
		beep(7,1);
		FLIP(LEDR);	
		beep(5);
		FLIP(LEDR);	
		beep(8,2);
		FLIP(LEDR);	

		_delay_ms(10000);
	}
#else
	ON(LEDR);	
	beep(17,4);
	beep(19,2);
	beep(21,1);
	
	_delay_ms(1000);
	FLIP(LOCK);
	fail();
	
	// cnum=holding=timeout=0;

	// OFF(LEDR);	
#endif
}
bool update(){
	lain=ain;
	ain = adc_read(3) / 16;
	
	if(ain!=lain)
		return true;
		
	return false;
}