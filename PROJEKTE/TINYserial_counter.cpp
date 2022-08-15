#include "Macros.h"
#include "stdlib.h"
#include "tiny_serial.h"
#include <stdio.h>

#define IN		PB1
#define LED		PB2
#define TIMEOUT	28000

volatile uint counter=0;
uint timeout=TIMEOUT;
volatile uint ovf=0,minovf=TIMEOUT,maxovf=0;

void print_int(uint _int){
	char* buffer= (char*) calloc(6,sizeof(char));
	
	sprintf(buffer, "%d ", _int);
	serial_print(buffer);
}	

int main(void){
	
	BUTTON(IN);
	OUTPUT(LED);
	ON(LED);
	
	GIMSK |= B(PCIE);
	PCMSK |= B(IN);
	
	TCCR1 |= 1;
	TCNT1 = 0;
	TIMSK |= B(TOIE1);
	
	tiny_serial_init();
	
	_delay_ms(500);
	serial_print("waiting...!\n\r");
	
	OFF(LED);
	
	while(1){
			
	}

	return 0;
}
void done(){
	cli();
	_delay_ms(100);
	OFF(LED);
	
	serial_print("counter:   ");
	print_int(counter);
	serial_print("\n\r");
	
	serial_print("min: ");
	print_int(minovf);
		
	serial_print("   max: ");
	print_int(maxovf);
	serial_print("\n\r");
}

ISR(TIM1_OVF_vect){
	ovf++;
	if(ovf >= TIMEOUT || ovf >= timeout){
		done();
	}
}

ISR(PCINT0_vect){
	if(!counter)
		ON(LED);
	
	counter++;
	

	if(ovf < minovf)
		minovf=ovf;
	if(ovf > maxovf)
		maxovf=ovf;

	ovf=0;
	TCNT1=0;
}