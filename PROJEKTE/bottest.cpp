#define PSIZE 7

int program[PSIZE+1][3] = {
	{-50,-50,170},
	{-220,-110,20},
	{-55,-34,220},
	{-80,-10,80},
	{200,-200,10},
	{154,140,140},
	{160,180,170},
	{60,80,250}
};


#include <Macros.h>
#include <math.h>


#define LEN 0
#define LDIR 4
#define REN 1
#define RDIR 2

#define MIN_SPD	50
#define MAX_SPD 255

#define CONST_SPD 160

#define DEL_US 200

#define COLLIDE	true
// #define AUTO true
 
volatile uchar ovf;
uchar cnt;


int new_spd[2]={0,0};
int spd[2]={0,0};
bool mode=1;
uint DEL = 100;

void stop();
void move(int l, int r);
void go(int l, int r, int d);

int main(){ 

	OUTPUT(LEN);
	OUTPUT(REN);
	OUTPUT(LDIR);
	OUTPUT(RDIR);
	OFF(LEN);
	OFF(REN);
	
	
    TCCR0A = (1<<COM0A0)|(1<<COM0A1)|(1<<COM0B0)|(1<<COM0B1)|(1<<WGM00);
    TCCR0B = 1;

	_delay_ms(1);
	
	INPUT(3);
	mode=READ(3);
	
	//PCMSK |= B(3);
	//GIMSK |= B(PCINT0);

	// TCCR1 = 8;
	// TIMSK = _BV(TOIE1);
	
	bool pdir=false;
			
   sei();
   
   while (1){
   
		if(!(cnt&7))
			DEL = 10 + (adc_read(3) << 4);
			
		if(!mode){

			uchar i = cnt & PSIZE;
				
			if(!i)
				pdir = !pdir;
			
			if(pdir){
				i = PSIZE-i;
				new_spd[0] = -program[i][0];
				new_spd[1] = -program[i][1];	
			}
			else{
				new_spd[0] = program[i][0];
				new_spd[1] = program[i][1];	
			}
			
			go(new_spd[0],new_spd[1],program[i][2]);
			
		
		}
		else{
			switch(lfsr()&15){
				case 0:	go(255,255,DEL); break;
				case 1:	go(-255,-255,DEL); break;
				case 2:	go(255,-255,DEL); break;
				case 3:	go(-255,255,DEL); break;
				default: break;
			}
			
			
		}
		
		cnt++;
			
   }
   
	return 0;
}

void stop(){ 

	OCR0A = OCR0B = 0;	
	
	go(spd[0] * -1,spd[1] * -1,40);
	
	go(0,0,40);
}

void go(int l, int r, int d){

	// dir
	if((l >= 0))
		OFF(LDIR);
	else
		ON(LDIR);

	if((r >= 0))
		OFF(RDIR);
	else
		ON(RDIR);

	if(!mode){
		if(l != 0)
		OCR0A = CONST_SPD;
		if(r != 0)
		OCR0B = CONST_SPD;
	}
	else{
		// scale
		uchar ll,rr;

		ll = MIN(ABS(l),MAX_SPD);
		rr = MIN(ABS(r),MAX_SPD);
			
		// limit
		if(l)
			ll=MAX(ll,MIN_SPD);
		if(r)
			rr=MAX(rr,MIN_SPD);
			
		OCR0A = ll;
		OCR0B = rr;
	}
	
	do{_delay_us(DEL_US);}while(--d);

	spd[0]=l;
	spd[1]=r;
	
	OCR0A = OCR0B = 0;
}

// accelerate
/*
ISR(TIMER1_OVF_vect){

	
	ovf++;
	if(!ovf) go(0,0);
	
	if(mode){
		int l=0,r=0;
		
		if(spd[0] != new_spd[0]){

				if(spd[0] < new_spd[0])
					l = spd[0] + 8;
				else
					l = spd[0] - 1;
		}

		if(spd[1] != new_spd[1]){

				if(spd[1] < new_spd[1])
					r = spd[1] + 8;
				else
					r = spd[1] - 1;

		}
		
		l = MIN(l,MAX_SPD);
		r = MIN(r,MAX_SPD);

		go(l,r);
}
}
*/

volatile uchar hit=0;
/*
SIGNAL(PCINT0_vect){

	if(ovf > 5){
		if(ovf==lovf){
			hit++;
			if(hit >= 8){
				hit=0;
				
				if(lovf < 30) go(200,200);
				else go(-100,-100);
			}
			
		}else 
			hit=0;

		lovf = ovf;
	}
		
	ovf = 0;

}

*/