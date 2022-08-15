
#include <Macros.h>

uchar div=1;
uchar ocr=1;

uchar ain,lain;

int main (void)
{ 
	OUTPUT(0);
	INPUT(3);
	BUTTON(1);
	BUTTON(2);
	
	TCCR0A |= (1<<COM0A0); 
	TCCR0A |= B(WGM01); 
	TCCR0B = div;

	while(1){

		ain = (ain + adc_read(3)) >> 1;
		
		if(ain != lain){
			ocr = RANGE(ain,1,255);
			OCR0A = ocr;
			lain = ain;
		}
		
		_delay_ms(5);
		
		if(!READ(1)){
			div++;
			if(div==7) div=0;
			TCCR0B = div;
			
			while(!READ(1));
			_delay_ms(100);
		}
		
	}
	
	return 0;
}
