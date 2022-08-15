
#include "Macros.h"
#include "stdlib.h"
#include "stdio.h"
#include "tiny_serial.h"

#define DIRA	3
#define DIRB	4

#define MINMS 8
#define MAXMS 10000

char buf[64]; // need a buffer for that
volatile ulong mcs;
volatile ulong lmcs;

int values[6];
volatile ulong pulses[6];
volatile uchar cnt=0;
volatile uint ovf=0;
volatile bool update;

uchar motors[2];
bool dirs[2];

int main(void){
	// OSCCAL = 220;
	
	OUTPUT(0);
	OUTPUT(1);
	OUTPUT(DIRA);
	OUTPUT(DIRB); 
	// tiny_serial_init();
	TCCR0A = B(COM0A1) | B(COM0B1) | 3;
	TCCR0B = 4;
	
	
	
	TCCR1 = 3;
	TCNT1=0;
	// OCR1A = 2;
	TIMSK |= B(TOIE1);
	
	MCUCR |= (1 << ISC00);
	GIMSK |= (1 << INT0);
	
	sei();

	while(1){
		if(update){
			for(uchar i=0;i<6;i++){

				int v = pulses[i];
				v = RANGE((v-580),0,1023);
				v >>= 1;
				v -= 256;
				
				values[i] = v;
				
				if(i < 2 && values[4] < -200 && values[2] < -200){
					v =ABS(v);
					motors[i] = v;
					if(motors[i] < 20) motors[i]=0;
					
					if(v < 0) 	dirs[i] = false;
					else 		dirs[i] = true;
				}
			
				// sprintf(buf,"%d\t",(values[i]));
				// const char* p = buf;
				// serial_print(p);
			}

			// sprintf(buf,"[%d\t%d] [%d\t%d] %d",motors[0],dirs[0],motors[1],dirs[1],cnt);
			// const char* p = buf;
			// serial_print(p);
			
			// serial_print("\n\r");
			update = false;
		}
		
		// motors
		
		OCR0A = motors[0];
		OCR0B = motors[1];
		
		if(!OCR0A) cbi(TCCR0A,COM0A1);	
		else sbi(TCCR0A,COM0A1);	
		if(!OCR0B) cbi(TCCR0A,COM0B1);	
		else sbi(TCCR0A,COM0B1);	
			
		if(dirs[0])
			ON(DIRA);
		else
			OFF(DIRA);
			
			
		if(dirs[1])
			ON(DIRB);
		else
			OFF(DIRB);
		
			
	}

	return 0;
}

ISR(TIMER1_OVF_vect){
	ovf++;
}

ISR(INT0_vect){
	if(!ovf) return; // too short

	TCCR1 = 0;
	
	mcs = ovf << 8;
	mcs |= TCNT1;

	ovf=0;
	TCNT1 = 0;
	
	if(CA(lmcs,390,30)){
		if(mcs > 480 && mcs < 1660 && cnt < 6){
			// if((mcs > pulses[cnt]-20 && mcs < pulses[cnt]+20) || !pulses[cnt]){
			// if(mcs < 100 + pulses[cnt] || pulses[cnt] == 0)
				pulses[cnt] = (pulses[cnt] + mcs);
				pulses[cnt] >>= 1;


			// }
			cnt++;
		}
		
		if(mcs>2000){
			update=true;
			cnt =0;
			// mcs=0;
		}
	}
	else if (!CA(mcs,390,30))
		cnt=0;

	lmcs=mcs;
	mcs = 0;
	TCCR1 = 4;



}