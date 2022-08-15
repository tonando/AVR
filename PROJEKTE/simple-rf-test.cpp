#include <Macros.h>
#include <simple-rf433.h>
#include <tiny_serial.h>
#include <stdlib.h>
#include <string.h> 

char* tmp;
int main(){
	tiny_serial_init();
	init_rf();
 	uchar v=0;	
 	uchar lc=0,rp=0;
	tmp = (char*)malloc(8);
	while(1){
		// serial_print("test");
		/* 
		serial_write(0b10010011);
		serial_write(10+v);
		serial_write(127);
		delay_ms(300);
		
		serial_write(0b10000011);
		serial_write(10+v);
		serial_write(0);
		delay_ms(900);
		 */
		 uchar rep = repeats;
		 if(lc != last_tcnt || rp != rep){
			 if(rep){
				 
			 if(rep > 3){
				 
				serial_print("BINGO  ");
				tmp = utoa(last_tcnt,tmp,10);
				serial_print(tmp);
				serial_print("\r\n");
				delay_ms(100);
			 }
			 else{
				 
			// serial_print("cnt: ");
			// tmp = utoa(TCNT1,tmp,10);
			// serial_print(tmp);
			
			// serial_print(" rep.: ");
			// tmp = utoa(rep,tmp,10);
			// serial_print(tmp);
			
			// if(timeout)
				// serial_print(" timeout! ");
			
			// serial_print("\r\n");
			 }
			
			 }
			lc=last_tcnt;
			rp=rep;
		 }
		 
		v++;
		v&=15;
	}
}

