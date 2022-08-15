
#include <avr/io.h>
#include <util/delay.h>
#include <Anal.h>

#define S_DEL 10
#define S_MIN 600
#define S_PULSE 64

#define S_U ( S_PULSE >> 8)

bool dir,lastp;
uchar index;

// void pwm(uchar a, uchar b);
void pwm(uchar a);

int main (void)
{
  DDRB = (1<<PB1) | (1<<PB2)| (1<<PB3);
  PORTB = 3;
	
   while( 1 ) {
	index++;	
	bool p = PINB & 1;
	if(p != lastp)
	PORTB ^= (1<<PB2)| (1<<PB3);
	lastp=p;

//speed control ?
	_delay_ms(10);
  }

  return 0;
}
