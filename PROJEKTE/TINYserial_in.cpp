
#include "Macros.h"
#include "tiny_serial_in.h"


int main(void){
	OUTPUT(0);
	OFF(0);
	tiny_serial_init();

	while(1){
		while(!rx_available);
		uchar c = serial_getc();
		
		if(c==65)
			ON(0);
	}

	return 0;
}

