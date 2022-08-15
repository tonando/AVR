#ifndef Stepper_h
#define Stepper_h

//#include <alloca.h>
#include "Macros.h"

#define FULLSTEPS 4
#define HALFSTEPS 8

//const byte HALFSTEPS[8] = {0b1010,0b0010,0b0110,0b0100,0b0101,0b0001,0b1001,0b1000};

const uint8_t STEP_SEQUENCE[HALFSTEPS] = {0b1010,0b0010,0b0110,0b0100,0b0101,0b0001,0b1001,0b1000};
	
//const byte HALFSTEPS[4] = {0b10100010,0b01100100,0b01010001,0b10011000};

typedef struct stepper_motor_struct{
	//byte total_steps:3;
	bool halfstep_mode:1;
	bool reversed:1;
//	bool running:1;	
	signed char current_step:4;
//	signed long steps_done;
//	signed long target;
	
//	byte timer;
//	byte speed;
//	byte acceleration;
//	byte acc_type:2;
	
//	void (*step)(stepper_motor_struct*);
//	byte (*getStep)(stepper_motor_struct*);	
}StepperMotor;

static void step(StepperMotor* st);
static byte getStep(StepperMotor* st);

void initStepper(StepperMotor* st){
	st->halfstep_mode = true;
//   	st->total_steps = HALFSTEPS;
	st->reversed = false;
	st->current_step = 0;
//	st->steps_done =
//	st->target =
//	st->acceleration =
//	st->timer = 0;
//	st->acc_type = LINEAR;		//impl
	
	// imp minmax
//	st->speed = 40;
	
//	st->step = &step;
//	st->getStep = &getStep;
}

// StepperMotor* createStepper(){
	// StepperMotor* result = (StepperMotor*)calloca(sizeof(StepperMotor));
	// result->total_steps = FULLSTEP_MODE;
	// return result;
// }

static void step(StepperMotor* st){
//	if(st->target == 0)
//		return false;
   byte cs = st->current_step;
   
	if(st->reversed == true){
		if(cs != 0)
		  cs--;
	       else{
		  cs = st->halfstep_mode ? 7 : 3;
	       }
//		st->target++;
		
		// if(st->current_step) {
			// st->current_step--;
		// }
		// else {
			// st->current_step = st->total_steps;	
		// }
	}
	else{
//		st->target--;
//	        if(cs != st->total_steps-1)
		  cs++;
//		else
//		  cs = 0;
	       cs &= st->halfstep_mode ? 7 : 3;
	}
		
	//st->current_step &= st->total_steps;
	st->current_step = cs;
}

static byte getStep(StepperMotor* st){
	
   byte result = STEP_SEQUENCE[ st->current_step ];
   if( (st->current_step & 1) && (st->halfstep_mode == true) )
		result >>= 4;
   /*
	
	if(st->total_steps == HALFSTEP_MODE){
		if((st->current_step&1) == 1)
			result = HALFSTEPS[ st->current_step>>1 ] & 0x0f;
		else
			result = (HALFSTEPS[ st->current_step>>1 ] >> 4) & 0x0f;
	}
	else{
		result = 0x0f & (HALFSTEPS[ st->current_step ] >> 4);
	}
   */
   
	return (0x0f & result);
	//return ( result );
}
/*
bool stepperMoveTo(StepperMotor* st, long t){
	if(t == st->steps_done)
		return false;

		// imp minmax
	st->reversed = (t < st->steps_done);
	st->running = true;

	return true;
}

bool stepperMove(StepperMotor* st, long t){
	return stepperMoveTo(st,(st->steps_done + t));
}

void updateStepper(StepperMotor* st){
	if(st->running){
		st->timer++;
		if(st->timer >= st->speed){
			st->timer = 0;
			st->running = step(st);
		}
	}
}
*/

#endif
