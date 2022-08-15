
#include <Macros.h>
#include <rf433.h>
// #include <avr/sleep.h>
// #include <avr/power.h>
#define MSG_SIZE 4
#define STARTBITS 64

#define LED1  3
#define LED2  1

int main ()
{ 

	OUTPUT(LED1);
	OUTPUT(LED2);
	init_rf(true);
	
    while (1) {
		if(receiving_bits)
			ON(LED1);
		else
			OFF(LED1);
			
		_delay_ms(100);	
	}
}
// volatile uint rfcnt=0;
/* 	// cli();

ISR(INT0_vect){
	if(receiving && !incoming)
		startbits = STARTBITS;
	uint rc = rfcnt;
	rfcnt=0;
	
	// low pass
	if(rc > 400){
		ON(0);


		if(rc < 4000)
			ON(3);
		else
			ON(4);
			// getpulse(rc);
	}
	else
		OFF(0);
	// sei();
 */	
/* 
}


ISR(TIMER0_OVF_vect){
	
	uchar m_idx = bcnt >> 3;	// div 8
	uchar idx = bcnt & 7;		// 0-7
	
	if(sending){
		if(startbits)
			startbits--;
		else{
			
			if(rbi(msg[m_idx],idx) == toggle)
				OFF(SEND_PIN);
			else
				OFF(SEND_PIN);

			bcnt++;
		}
	}
	
	if(receiving){
		
		if(incoming){
				
			if(READ(REC_PIN))
				sbi(in_buffer[m_idx],idx);
			else
				cbi(in_buffer[m_idx],idx);
			
			bcnt++;
		}
		else{
			startbits--;
			if(startbits < 8){
				
				bcnt = 0;
				incoming=true;
			}
		}
	}
	
	
	if(bcnt == MSG_SIZE){
		
		bcnt = 0;
		
		if(sending)
			stop_tx();

		if(receiving)
			check_msg();		
	}	
}	
	
	
	if(rfcnt < 8000)
		rfcnt++;
		else{
			rfcnt=0;
			OFF(0);
		}
			// timeout();	
	// if(rfcnt >= 10)
		// GIMSK = B(INT0);
		// GIMSK = B(INT0);
	bcount++;
	if(bcount == 16)
		bcount=0;
	
	if(sendmode){		
		if(rbi(buffer,bcount))
			ON(1);
		else
			OFF(1);
	}
	else{
		if(READ(0))
			buffer |= 1;
		
		buffer <<= 1;
		
		test(buffer & 255);
	}
 }
void timeout(){
	OFF(0);
				OFF(4);
			OFF(3);
}
void getpulse(uint p){
	if(p < 500)
		ON(4);
	else
		ON(3);
}
 */