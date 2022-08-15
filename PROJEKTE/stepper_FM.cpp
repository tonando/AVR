#include <t45.h>

#define MIN_DELAY	600


// uint speed=100;
// uint l_speed=speed;
ulong del=1000;
uchar aval,l_aval;
bool running;
bool dir;
uint max_steps = 0;
uint c_step = 0;

void step(uint n){
	
	ON(0);
	delay_us(600);
	OFF(0);
	delay_us(200);
	delay_ms(del);
	
	n--;
	if(n)
		step(n);
}	
	  
void step(){ step(1); }
	  
void changeDir(){
	dir = !dir;
	SET(1, dir);
	step(20);
}

int main() { 
	
	BUTTON(2);
	BUTTON(4);
	INPUT(3);
	OUTPUT(0);
	OUTPUT(1);
	
	while (1){
		
		aval = adc_read(3);
	
		if(READ(4)){
			del = (aval << 3);
			del = constrain(del,2,400);
		}
		else{
			if(aval < 5) aval = 0;
			// else 
				// max_steps = aval << 2;
		}
			
		l_aval = aval;
		
		if(running){
			// if(c_step >= max_steps && max_steps){
				// changeDir();
				// c_step = 0;
			// }
			
			step(4);
			// if(max_steps && dir)
				// c_step++;
		}
		else{
			step(40);
			changeDir();
		}
		
		if(!READ(2)){
			changeDir();
		}
		
		if(!READ(4)){
			running = true; 
		}
	}
	return 0;
 }
  
