#ifndef Arpeggiator_h
#define Arpeggiator_h

#include "Macros.h"
#include "Config.h"


struct Arpeggiator{
		void init();	

		void start();
		void next();
		char getOffset();

		void setStepSize(byte);//	{step_size = min(15,s); };
		void setRepeats(byte);
		void setSpeed(byte);
		void setGate(byte);//		{ gate = constrain(g,1,15); };
		
		bool direction;
		bool ping_pong;

		byte current_step;

		byte num_steps;
		byte gate; 			// note off after x
		byte repeats;
		byte speed;
		byte step_size;	// step_divider / x
};

#endif