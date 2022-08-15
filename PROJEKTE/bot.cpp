
#include <Macros.h>
// #include <util/delay.h>
// #include <math.h>

#define LEN 0
#define LDIR 2
#define REN 1
#define RDIR 3

#define MAX_CNT	31
 

volatile uchar ovf;
uchar cnt;
uchar l_ain;

uchar spd[2]={0,0};
bool dir[2]={0,0};

int main(){ 
   
   INPUT(4);
	OUTPUT(LEN);
	OUTPUT(REN);
	OUTPUT(LDIR);
	OUTPUT(RDIR);
   
   TCCR0B = 2;
   TIMSK0 = _BV(TOIE0);
   
   sei();
   
   while (1){

		cnt++;
		cnt &= 31;
		
		uchar ain = adc_read(2);
		
		uchar r=lfsr()&63;
		if(!cnt && (r<20)){
			spd[0] = spd[1] = 0;
		
			delay_ms(300*(r+1));
			}
		r&=7;	
			
		
		if(ain < 70){
			if(ain < 10){
				spd[0] = spd[1] = 0;
				_delay_ms(500);
				
				spd[0] = 14;
				spd[1] = 14;
				
				dir[0] = 0;
				dir[1] = 0;				
				
				// _delay_ms(700);
			}
			else{
				dir[0] = 1;
				dir[1] = 0;
					
				spd[1] = 6;
				spd[0] = 10;
			}
				
		}
		else{
			if(ain < l_ain){
				spd[0] = 2;
				spd[1] = 2;			
			}
			else{
				spd[0] = r+3;
				spd[1] = r+3;
			}
			dir[0] = 1;
			dir[1] = 1;
		}
			
		
		l_ain = ain;
		_delay_ms(20);
	
   }
	return 0;
}

ISR(TIMER0_OVF_vect){

	
	if(spd[0] > ovf)
		ON(LEN);
	else	
		OFF(LEN);
		
	if(spd[1] > ovf)
		ON(REN);
	else	
		OFF(REN);
		
	
	ovf++;
	ovf&=MAX_CNT;

	if(!ovf){		
		if(dir[0])
			OFF(LDIR);
		else
			ON(LDIR);
			
		if(dir[1])
			OFF(RDIR);
		else
			ON(RDIR);
		
	}
}