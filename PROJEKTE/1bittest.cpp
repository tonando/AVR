//#include "bitsynth.h"

int main(){
	init_1bit();
	PORTB |= 1;
	char cnt=0;
char last=1;
	
	while(1){
		update_1bit();
		
		char b = (PINB&1);
		if(b){
			lfofreq=0;			
			pitch=200;
		}
		else{
			lfofreq=100;
			pitch=100+cnt;	
		}
		if(b != last && last){
			wavenum++;
			wavenum &= 7;
			last = b;
		}
		
		cnt++;
		if(cnt==100)
			cnt=0;
	}
return 0;
}