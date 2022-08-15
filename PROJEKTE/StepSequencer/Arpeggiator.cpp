///////////////////////////////////////////////////////////////
//	Arpeggiator

#include "Arpeggiator.h"

void Arpeggiator::init(){
		direction = 1;
		repeats = 0;
		num_steps = 4;
		ping_pong = false;
		current_step = 0;
		step_size = 1;	
		gate = 4; 		
		speed = 2;
}

void Arpeggiator::start(){
	current_step = 0;
}
void Arpeggiator::next(){
	current_step++;
}
char Arpeggiator::getOffset(){
	return direction * current_step * step_size;
}

void Arpeggiator::setStepSize(byte s){
	step_size = min(15,s); 
}
void Arpeggiator::setRepeats(byte s){
	repeats = min(15,s); 
}
void Arpeggiator::setSpeed(byte s){
	speed = s;
}
void Arpeggiator::setGate(byte s){
	gate = constrain(s,1,15); 
}

/*
///////////////////////////////////////////////////////////////
//	Pattern	( parent of steps)
struct Pattern{
	byte length;	// 3 bits = 2^1-5 ... 5bits = 0-31
	bool mute;
	byte current_note;
	Arp arp;

	struct Step steps[MAX_STEPS];

	bool multiply();
	bool divide();	
};
*/