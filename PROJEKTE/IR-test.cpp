
#include <Macros.h>
#include "ir.h"


int main (void)
{ 

	uchar cnt=0;
	uchar addr, cmd;
	OUTPUT(0);
	
	IR_init();
	

	while(1){
		cnt++;
			
		if (IR_read(&addr, &cmd) == IR_SUCCESS) {
			ON(0);
			delay_ms(1000);
		}			
		else{
			OFF(0);
		}
	}
	
	return 0;
}
