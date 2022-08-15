// #include <avr/sleep.h>
#include <Macros.h>

// #define BTN_PIN 1
#define TPIN 4

#define DHI	(1302>>1)
#define DLO (5181>>1)

const float DST = ((DLO-DHI) / 1024);

int main(){
	OSCCAL=250;
	OUTPUT(TPIN);
	// INPUT(3);
	 		
	while(1){
		for(int i=1023;i;i--){
			int d = DHI + (int)(i*DST);t
			
			for(int l=0; l < 10; l++){
			ON(TPIN);
			delay_us(d);
			OFF(TPIN);
			delay_us(d);
			}
		}
		
		delay_ms(1000); 
	}
	
	return 0;
}