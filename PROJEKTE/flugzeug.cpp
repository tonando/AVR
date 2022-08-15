
#include "Macros.h"
#include "stdlib.h"
#include "stdio.h"

// #define DEBUG

#ifdef DEBUG
	#include "tiny_serial.h"
	 
#endif

// #define STEER		0	
#define SWITCH		4	

#define SERVO_A		0
#define SERVO_B		1
#define THROTTLE		3
// #define 	PROP

#define MINMS 8
#define MAXMS 10000

#define MAXCNT 630

volatile uint mcs;
// volatile uint lmcs;

volatile uint values[8];
volatile uchar cnt=0;

volatile int axis[8];
int axis_new[8];

int servos[2];
int motor=50;
// bool dirs[2];

// uint trim[2] = {25000,0};

void update_motors();

void map(){
	// float m = (trim[1]-trim[0]) / 1000;
	int axis_old[6];
	for(uchar i=0; i<6; i++){
		axis_old[i] = axis_new[i];
		
		int v = (values[i] >> 2);
		
		if(i<4){
			v = (axis_new[i]+v)/2;
			
			if(i==2){
				if(v<40) v=0;
				// else v>>=1;
			}
			else{
				v-=255;
				if(v < 40 && v > -40) v=0;
				// v/=1.5;
			}
			
			
			axis_new[i] = MAX(MIN(v,255),-255);
		}
		// sw
		else
			axis_new[i] = MAX(MIN(v>>8,3),0);
		
		axis[i]=axis_old[i];
	}
}


int main(void){
	// OSCCAL = 220;
	
	OUTPUT(SERVO_A);
	OUTPUT(SERVO_B);
	OUTPUT(THROTTLE);
	
	INPUT(2);	// int0 RX
	#ifdef DEBUG
		tiny_serial_init(104);
		serial_print("Passt soweit... \n\r");
	#endif
	
	MCUCR |= (1 << ISC01 );
	GIMSK |= (1 << INT0);
	
	TCNT0 = 0;
	TCNT1 = 0;
	TCCR0A = 0;
	TCCR0B 	= 1;
	TCCR1 = 2;
	TIMSK |= B(TOIE0);
	TIMSK |= B(TOIE1);
	
	uchar cnt=0;
	// do{
		// ON(SERVO_A);
		// _delay_ms(2);
		// OFF(SERVO_A);
		// _delay_us(1000);
		// cnt++;
	// }while(cnt<20);

	sei();

	while(1){

		map();
		update_motors();
	
		cnt++;
		
		// if(axis_new[SWITCH])
			// delay_us(100 * axis_new[SWITCH]);

			
	}

	return 0;
}
		// motors
void update_motors(){
	/* 
	float s = (MAXCNT/512);
	axis_new[0]+=255;
	axis_new[0] = (int)(axis_new[0]*s);
	axis_new[0] >>= 3;
	servos[0] = 31+MIN(MAXCNT/8,axis_new[0]);
	 */
	// servos[1] = 300-axis_new[1];
	// servos[1] = MIN(MAXCNT,2+ABS(255+axis_new[1]));
	// servos[1] = MIN(1000,(axis_new[1]+255)*2);
	motor = MAX(90,MIN(MAXCNT/2,(60+axis_new[2])));
}	

 
volatile int c;

ISR(TIMER0_OVF_vect){
	
	if(!(c&7)){
		if(cnt==0){
			if(READ(2)) 
			sbi(PORTB,SERVO_A);
			// else 
			// if(READ(2)) 
			// cbi(PORTB,SERVO_A);
		}
		if(cnt==1){
			if(READ(2)) sbi(PORTB,SERVO_B);
			// else cbi(PORTB,SERVO_B);
		}
	}
	
	if(c){
		if(c >= motor) OFF(THROTTLE); 
		// if(c >= ( servos[0])) OFF(SERVO_A); 
		// if(c >= ( servos[1])) OFF(SERVO_B); 
	}	
	else{
		// ON(SERVO_A);
		// ON(SERVO_B);
		ON(THROTTLE);		
	}
		
	c++;
	if(c==MAXCNT) 
		c=0;

	// if(!(c&7)){
		// uchar i = c&3;	
		// if(axis_new[i]>axis[i]) axis[i] ++;
		// if(axis_new[i]<axis[i]) axis[i] --;	
	// }
}
 
ISR(TIMER1_OVF_vect){
	
	mcs++;
	

}

ISR(INT0_vect){
	cli();
	if(cnt<2){
		if(cnt==0) 
			ON(SERVO_A);
		if(cnt==1) 
			ON(SERVO_B);
	}
	
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

	mcs = 0;
	TCNT1 = 0;
	sei();
}