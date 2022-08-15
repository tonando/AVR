
#include "Macros.h"
#include "tiny_serial.h"

#define MINMS 8
#define MAXMS 10000

char buf[64];
volatile uint mcs;
// volatile uint lmcs;

volatile uint values[8];
// volatile ulong pulses[6];
volatile uchar cnt=0;
// volatile ulong ovf=0;
// volatile bool update;

uchar motors[2];
bool dirs[2];

void edge();
bool last_in;

int main(void){
	// OSCCAL = 220;
	
	// OUTPUT(0);
	// OUTPUT(1);
	// OUTPUT(DIRA);
	// OUTPUT(DIRB); 
	// TCCR0A = B(COM0A1) | B(COM0B1) | 3;
	// TCCR0B = 4;
	
	
	// INPUT(3);
	// delay_ms(100);
	tiny_serial_init();
	serial_print("Passt soweit... \n\r");

	
	INPUT(2);
	MCUCR |= (1 << ISC01 );
	GIMSK |= (1 << INT0);
	// PCMSK |= B(PCINT2);
	// GIMSK |= B(PCIE);
	
	// TCCR1 = B(CTC1) | 1;
	TCCR1 = 2;
	// OCR1A = 1;
	TCNT1=0;
	// TIMSK |= B(OCIE1A);
	TIMSK |= B(TOIE1);
	

	// sei();

	while(1){
	
		for(uchar i=0; i<6; i++){
			uint v = MIN(255,(values[i] >> 4));
			sprintf(buf,"%d\t",(v));
			const char* p = buf;
			serial_print(p);	
		}
		serial_print("\n\r");	
/* 		
		bool in = READ(2);
		
		if(last_in != in){
			
			edge();
		}
		
		last_in = in;
		 */
		
/* 	
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
				s}
			
				
				sprintf(buf,"%d\t",(values[i]));
				const char* p = buf;
				serial_print(p);
			}

			sprintf(buf,"[%d\t%d] [%d\t%d] %d",motors[0],dirs[0],motors[1],dirs[1],cnt);
			const char* p = buf;
			serial_print(p);
			
			serial_print("\n\r");
			update = false;
		}
		 */

			
	}

	return 0;
}
		// motors
void update_motors(){
/* 		
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
		 */
}

ISR(TIMER1_OVF_vect){
	// mcs++;
	// ovf++;
// }
// ISR(TIMER1_COMPA_vect){
	// if(mcs < 60000)
		mcs++;
}

// void edge(){
// ISR(PCINT0_vect) {
ISR(INT0_vect){
cli();
	// if(!mcs) return; // too short
	// bool in = READ(2);

	// if(in){
	if(mcs > (12000>>8))
		cnt=0;
	else{
		if(cnt < 6){
			uint v = (mcs<<8) | TCNT1;
			v -= MIN(v,4140);
			v = (v + values[cnt])>>1;
// TODO: inc/dec value
			values[cnt] =  v;
			cnt++;
			cnt&=7;
		}
	}
		// if(cnt==7){
			// PORTB ^= 1;
			// serial_print("\n\r");
			// cnt=0;
		// }

	// }
	// else{
	
	// }
				
	// lmcs=mcs;

	mcs = 0;
	TCNT1 = 0;
sei();
}