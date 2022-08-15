
#include "Macros.h"
#include "stdlib.h"
#include "stdio.h"

// #define DEBUG

#ifdef DEBUG
	#include "tiny_serial.h"
	char buf[80];
#endif

#define MOT_RES		31

#define THROTTLE	1
#define STEER		0	
#define SWITCH		4	

#define LEFT_F			0
#define LEFT_B			1
#define RIGHT_F			3
#define RIGHT_B			4

#define MINMS 8
#define MAXMS 10000

volatile uint mcs;
// volatile uint lmcs;

volatile uint values[8];
volatile uchar cnt=0;

volatile int axis[8];
int axis_new[8];

int motors[2];
// bool dirs[2];

uint trim[2] = {25000,0};

void update_motors();

void map(){
	// float m = (trim[1]-trim[0]) / 1000;
	
	for(uchar i=0; i<6; i++){
		int v = (values[i] >> 2);
		
		if(i<4){
			v = (axis[i]+v)/2;
			
			if(i==2){
				if(v<40) v=0;
				else v>>=1;
			}
			else{
				v-=255;
				if(v < 40 && v > -40) v=0;
				// v/=1.5;
			}
			
			
			axis_new[i] = MAX(MIN(v,255),-255);
		}
		else
			axis_new[i] = MAX(MIN(v>>8,3),0);

		// v -= 20;
		// axis[i] >>= 2;
		// float v = values[i] - trim[0];
		// v /= m;
		// if(i<4 && ABS(axis[i] - v) > 50) continue;
		
	}
}

void update_servos(){
	int spd = MIN(511,(values[0] >> 3));
	int dir = MIN(511,(values[1] >> 3));
	
	spd-=255;
	dir-=255;
	
	motors[0] = (spd - dir)/2;
	motors[1] = motors[0]*-1;
	
	uchar ma = ABS(motors[0]);
	uchar mb = ABS(motors[1]);
	
	if(ma < 30) motors[0] = 0;
	if(mb < 30) motors[1] = 0;
	
	OFF(0);
	OFF(1);	
	if(motors[0] > 0) ON(0);
	if(motors[0] < 0) ON(1);
	
	#ifndef DEBUG
	OFF(3);
	OFF(4);	
	if(motors[1] > 0) ON(3);
	if(motors[1] < 0) ON(4);
	#endif
	
	for(uchar i=0; i<200; i++){
		if(i>ma){ OFF(0); OFF(1); }
		#ifndef DEBUG
		if(i>mb){ OFF(3); OFF(4); }
		#endif
		
		_delay_ms(2);
	}
}

int main(void){
	// OSCCAL = 220;
	
	OUTPUT(RIGHT_B);
	OUTPUT(RIGHT_F);
	OUTPUT(LEFT_B);
	OUTPUT(LEFT_F);
	
	INPUT(2);	// int0 RX
	#ifdef DEBUG
		tiny_serial_init(104);
		serial_print("Passt soweit... \n\r");
	#endif
	
	MCUCR |= (1 << ISC01 );
	GIMSK |= (1 << INT0);
	
	TCCR1 = 2;
	TCNT1=0;
	// TCCR0A = 3;
	TIMSK |= B(TOIE1) | B(TOIE0);
	uchar c=0;

	sei();

	while(1){
	#ifdef DEBUG
		// sprintf(buf,"trim: %d - %d \n\r",trim[0],trim[1]);
		// const char* p = buf;
		// serial_print(p);	
		for(uchar i=0; i<6; i++){
			sprintf(buf,"[%d] \t",axis[i]);
			const char* p = buf;
			serial_print(p);	
		}
		serial_print("\n\r");	
	#else
		update_motors();
	#endif
	
		// if(values[2] < trim[0] && values[2] > 100)
			// trim[0] = values[2];
		// if(values[2] > trim[1])
			// trim[1] = values[2];
			
		map();
		update_motors();
		
		if(c){
			if( (ABS(motors[0])) < c ){
				OFF(LEFT_B);
				OFF(LEFT_F);
			}
			if( (ABS(motors[1])) < c ){
				OFF(RIGHT_B);
				OFF(RIGHT_F);
			}
		
		}
		else{
			if(motors[0] != 0){
				if(motors[0] > 0) ON(LEFT_F);
				else ON(LEFT_B);
			}
			
			if(motors[1] != 0){
				if(motors[1] > 0) ON(RIGHT_F);
				else ON(RIGHT_B);
			}
		}
	
		c++;
		
		if(axis_new[SWITCH])
			delay_us(100 * axis_new[SWITCH]);
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

	int steer = axis_new[STEER];
	int spd = constrain(axis_new[THROTTLE],-255,255);

	motors[0] = spd;
	motors[1] = spd;
	
	if(steer != 0){
		if(steer>0){
			motors[0] -= steer;
		}
		else{
			motors[1] -= steer;
		}
	}

	motors[0] = constrain(motors[0],-255,255);
	motors[1] = constrain(motors[1],-255,255);
	

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
/* 
volatile uchar c;

ISR(TIMER0_OVF_vect){
		

	c++;
	c&=255;
}
 */
ISR(TIMER1_OVF_vect){
	mcs++;
	
	uchar i = mcs&3;
	
				
		// int d = ABS(axis_new[i]-axis[i])>>6;
		// d=MAX(1,d);
		
		if(axis_new[i]>axis[i]) axis[i] ++;
		if(axis_new[i]<axis[i]) axis[i] --;

}

ISR(INT0_vect){
	cli();
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