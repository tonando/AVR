#include <Macros.h>

#define OUT		1
#define PRG		0
#define REC		2
#define APAR	2
#define AVAL	3

#define MAXSTEPS	16

uint waves[] = {
	0b1010001010110001,
	0b1011000010101001
};
void play(uchar);
void init();

uchar c_step;
uchar steps[MAXSTEPS][3];

int main(){
	OSCCAL=180;
	OUTPUT(OUT);
	INPUT(3);
	INPUT(4);
	BUTTON(REC);
	BUTTON(PRG);
	
	uchar ss=0;
	 		
	init();
	
	while(1){
	
		ss++;
		ss&=31;
		
		if(!ss)
			c_step++;
		if(c_step >= MAXSTEPS)
			c_step=0;
		
		if(!READ(REC))
			steps[c_step][0] = MIN(250,MAX(5,adc_read(AVAL)));
			
		play(ss);
	}
	
	return 0;
}

ulong getDel(uchar n){
	ulong res = 40 + (n+10)*3;
	
	return res;
}

void play(uchar ss){

	uchar c=0;

	// uchar* cs = steps[c_step];
	
	ulong del = getDel(steps[c_step][0]);
	uint r = 255 - steps[c_step][0];
	r>>=4;
	
	uint wave = waves[steps[c_step][1]];
	
	do{
		if(ss < steps[c_step][2]){
			for(uchar i=0;i<8;i++){
			if(rbi(wave,c)){
				ON(OUT);
				delay_us(del);
				OFF(OUT);
				}
			else{
				OFF(OUT);
				delay_us(del);
				ON(OUT);
				}
			}
		}
		else
			delay_us(del*8);
		
		// c++;
		// c&=15;
		// if(!c) 
		r--;
		
			
	}while(r);
	
	OFF(OUT);
}


void init(){
	for(uchar i=0; i<MAXSTEPS; i++){
		steps[i][0] = 50 + ((i&3)<<3);
		steps[i][1] = (i&1);
		steps[i][2] = 4 + (i&7);
	}
}
