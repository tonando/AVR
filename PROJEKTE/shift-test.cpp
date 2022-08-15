
#include <avr/io.h>
#include <util/delay.h>
#include <Anal.h>
// #include <EEPROM.h>
#include <shiftinout.h>
#include <IMAGES.h>


int main (void)
{
	// uchar mode=EEPROM_read(2);
	
	DDRB = 0xff ^ _BV(4);
	PORTB = 1 | _BV(4);
		
		int t=0;
		int tt=0;
		uchar frame = 0;
		bool dir=0,mode=0;
	// initShift(3, 2, 0);
   while( 1 ) {
   mode = rbi(PINB,4);
   
   // if(dir)
	// t--;
   // else
	t++;
	if(t == 16){
		t = ++tt;
		tt &= 15;
	}
	
	
	// t&=15;
	
	// if(t == 0) dir = !dir;

	// shiftOut((1<<t));
	// int in = analRead(0);
	// in >>= 7;
	_delay_ms(1);
	
	PORTB=0;

	uchar i=16;
	do{--i;
		// if(c<0) c*=-1;
		// if(t==i)
		if(mode){
			if((IMAGE[t][i/8] >> (i&7)) & 1)
				sbi(PORTB,2);
			else
				cbi(PORTB,2); // data
		}
		else{
		if((t==i))
			sbi(PORTB,2);
		else
			cbi(PORTB,2); // data
		}	
		sbi(PORTB,3);
		cbi(PORTB,3);

	}while(i);
	
	sbi(PORTB,0);
	
  }

  return 0;
}

