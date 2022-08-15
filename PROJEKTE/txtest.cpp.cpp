
#include <Macros.h>

#define TX		0
#define BITS	24
#define SHORT 	240	
#define LONG	610
#define START	4000

#define REPEAT	4

uint out = 0;

void send(ulong _o, uchar _b){

	uchar r=REPEAT;
	
	do{r--;
		
		OFF(TX);
		_delay_us(START);
		
		uchar i=_b; 
		do{i--;
			
		
			ON(TX);
			if(rbi(_o,i)){
				_delay_us(LONG);
				OFF(TX);
				_delay_us(SHORT);
			}
			else{
				_delay_us(SHORT);
				OFF(TX);
				_delay_us(LONG);
			}		
			
		}while(i);
	
		ON(TX);
		_delay_ms(20);
	
	}while(r);
}
void send(ulong _o){ send(_o,BITS); }

// void sendString(const char* txt){

	// send((uint)'$',16);
	// uchar l = sizeof(txt);
	// do{
		// send((uint)txt[l],16);
	// }while(--l);
	// send((ulong)'$',8);
// }
	
int main (void)
{ 
	OUTPUT(TX);
	OFF(TX);
	OSCCAL = 180;
	// uchar cnt=0;
		
	while(1){
	
		send(out);
		
		_delay_ms(920);
		out++;
		
		// if(!(out&7)){
		// sendString("Test");
			// send((ulong)'T',8);
			// send((ulong)'e',8);
		// }
	

	}
	
	return 0;
}
