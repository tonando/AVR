#include <Macros.h>
// #include <beep.h>
// #include <tone_decoder.h>
#include <Manchester.h>
int main(){
Manchester man;
 	// tone_decoder_init();
	OUTPUT(3);
	
	while(1){
		// if(get_tone()){
			// ON(3);
		// }
		delay_ms(1000);
		OFF(3);
	}
	return 0;
}
