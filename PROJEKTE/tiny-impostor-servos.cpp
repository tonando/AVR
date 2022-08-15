
#include <avr/io.h>
#include <util/delay.h>
#include <Anal.h>

#define S_DEL 10
#define S_MIN 600
#define S_PULSE 1000

#define S_U ( S_PULSE >> 8)

uchar rec[2][256];

void servo(uchar,uchar);

int main (void)
{
  DDRB = (1<<PB1) | (1<<PB2);
  PORTB = 1;
  
	int ain[2]={0,0};
	bool recording;
	int index;
	
	analInit(3);
	
	// TEST
	servo(0,0);
	_delay_ms(100);
	
	for(uchar i=0; i<256; i++){
		servo(i,i>>2);
		_delay_ms(30);
		}
	
   while( 1 ) {
	
	ain[0] = analRead(3);
	ain[1] = analRead(2);
	ain[0] >>= 2;
	ain[1] >>= 2;
	
    //button change
	recording = ~(PINB & 1);
	
	//button down
	if(recording){
		rec[0][index] = ain[0];
		rec[1][index] = ain[1];
		// rec[rec_index] = ain;
		//rec_index &= 255;
	}
	
	// 		SERVO
	servo(rec[0][index], rec[1][index]);
	
	index++;	

//speed control ?
  }

  return 0;
}

void servo(uchar v, uchar b){
	// AN
	PORTB |= (1<<PB1) | (1<<PB2);		
	_delay_us( S_MIN);
    
	// for(int i=0; i<v; i++)
	for(int i=0; i<256; i++){
		_delay_us(S_U);    
		if(i>v)
			PORTB &= ~((1<<PB1));
		if(i>b)
			PORTB &= ~((1<<PB2));
	}
	// AUS
	PORTB &= ~((1<<PB1) | (1<<PB2));
	_delay_ms( S_DEL);
	
}