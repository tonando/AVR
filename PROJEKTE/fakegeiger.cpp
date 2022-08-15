
#include <Macros.h>

uchar rand=0;
uchar level=0;

void click(){
	uchar d = (18 - level) + (rand & 7);
	// for(uchar i=0;i<16;i++){
		if(rbi(rand,0))
			ON(0);	
		delay_ms(1);
		OFF(0);
		delay_ms(d);
	// }
	
}

int main (void)
{ 

	uchar cnt=0;
	
	OUTPUT(0);
	OUTPUT(3);
	OFF(3);
	// INPUT(3);
	

	while(1){
		cnt++;
		// delay_ms(2);
			
		if(!(cnt&15)){
			// if(!cnt){
				level=(uchar)adc_read(2) >> 4;
				// level++;
				// level&=7;
				// rand = lfsr() & 0xff;
			}
			
		// }	
		rand = lfsr() & 0xff;
		
		if(level > (cnt&15))
			click();
	}
	
	return 0;
}
