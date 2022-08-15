#include "m32.h"
#include "StepSequencer/Sequencer.h"

Sequencer seq;// = new Sequencer();

int main(void){
	// seq.init();
	while(1){
		seq.update();
	}
	return 0;
}