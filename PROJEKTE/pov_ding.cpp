#include <Macros.h>
#include <util/delay.h>
#include <math.h>

#define LEDS 16

#define MOSI 	1
#define LATCH 	3
#define CLOCK 	2
#define BUTTON 	0

const unsigned char SINTABLE[32] = {8, 9, 11, 12, 13, 14, 15, 15, 16, 15, 15, 14, 13, 12, 11, 9, 7, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6};

volatile uchar ain=3,cnt=0;
uint output;
uchar mode;
bool last_bt, fill;

void shift(uchar);
void setupAdc();
// void updateAnal();
// void updateBits(int val);

void setupAdc(){
  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX = 2;    // set 2 analog input pin
  ADMUX |= _BV(REFS0);  // set reference voltage
  ADMUX |= _BV(ADLAR);  // left align ADC value to 8 bits from ADCH register

  ADCSRA |= _BV(ADPS2) | _BV(ADPS0);    // 32 prescaler for 38.5 KHz
  // ADCSRA |= _BV(ADPS2);                     // 16 prescaler for 76.9 KHz
  //ADCSRA |= _BV(ADPS1) | _BV(ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= _BV(ADATE); // enable auto trigger
  ADCSRA |= _BV(ADIE);  // enable interrupts when measurement complete
  ADCSRA |= _BV(ADEN);  // enable ADC
  sei();
  start_adc;
}

void shift(uchar output){
   	OFF(LATCH);
   	OFF(CLOCK);
	// PORTB &= ~_BV(CLOCK);
	
       // uchar r=REGS-1;
		 // uchar r = REGS;
		 
       // do{r--;
        uchar i=8;
		do{i--;


			 if(rbi(output,i))
				ON(MOSI);
			 else
				OFF(MOSI);

			 OFF(CLOCK);
			 ON(CLOCK);

		   }while(i);
	   // }while(r);

	OFF(MOSI);
	OFF(CLOCK);
	ON(LATCH);


   // update();

}/* 
void shiftValue(uchar v){
	
   uchar i=RESOLUTION;
   do{i--;
      bool b = last_bt ? (v >= i) : (v == i);
      	 if(b)
	    PORTB |= _BV(MOSI);
	 else
	    PORTB &= ~_BV(MOSI);
PORTB &= ~_BV(CLOCK); 
      PORTB |= _BV(CLOCK);
   }while(i);
   update();
} */
/* void clearOut(){
     uchar i=REGS;
     do{i--;
	output[i] = 0;
     }while(i);
} *//* 
void setOut(uchar* v, uchar l){
        
   
   uchar i = RESOLUTION;
   do{i--;
      bool b = fill ? (v >= i) : (v == i);
      uchar idx = i / REGS;
      if(b)
	 sbi(output[idx], (i % RESOLUTION));
      else
	 cbi(output[idx], (i % RESOLUTION));
      
   }while(i);
}
 */
int main(){ 
   DDRB =  _BV(LATCH) | _BV(MOSI) | _BV(CLOCK);
   PORTB = _BV(BUTTON) | _BV(LATCH);
   
   // last_bt = 1;
   
   shift(0b10100111);
   
   TCCR0B = 2;
   TIMSK = _BV(TOIE0);
   sei();
   //setupAdc();
 //  uchar cnt=0;
   while (1){
/*
	uchar bt = rbi(PINB,BUTTON);
	if(bt != last_bt){
		cli();
		if(bt){
		  mode++;
		  if(mode == 3) mode = 0;
		  BLINK
		  ain++;
		  ain &= 15;
		}
		else{
			cbi(PORTB,1);
			while(!rbi(PINB,BUTTON));
			sbi(PORTB,1);
		}
		last_bt = bt;
		sei();
    }   
*/		
	

   
		// output[0] = _BV(SINTABLE[cnt] & 0x0f);
		
		// uchar i=ain;
      // i <<= 2;
		// do{
			//_delay_ms(100);
		// }while(--i);

	
		cnt++; 
		cnt &= 15;
		// if(cnt == (mode) ? 31 : sizeof(IMAGE))
			// cnt=0;
   }
      
   return 0;
}



ISR(TIM0_OVF_vect){
	// uchar tmp = 0;
	// output = 0;
			output = _BV(SINTABLE[cnt]);
	
   // if(!mode){
      // output = IMAGE[cnt][1];
	  // output <<= 8;
      // output |= 0x00ff & IMAGE[cnt][0];
/* 	  }
	else{
		if(mode == 2)
			output = _BV(SINTABLE[cnt]);
		if(mode == 1)
			output = _BV(ain);
   }
 */			
	shift(IMAGE[cnt][0]);
	shift(IMAGE[cnt][1]);
}

ISR(ADC_vect){
	uint tmpa = ADC;
	
	// if(mode)
		// ain = tmpa >> 4;
	// else
		ain = tmpa>>2;

   start_adc;
} 