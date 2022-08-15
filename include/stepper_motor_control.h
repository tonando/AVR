#ifndef STEPPER_MOTOR_CONTROL_h
#define STEPPER_MOTOR_CONTROL_h

#include "Macros.h"

#define FULLSTEPS 4
#define HALFSTEPS 8

//asm volatile ("rol %0" : "=r" (example) : "0" (example));

uchar STEP_SEQUENCE[HALFSTEPS] = {0b1010,0b0010,0b0110,0b0100,0b0101,0b0001,0b1001,0b1000};

wstruct motor_struct {

	bool halfstep;	// mode
	bool reversed;	// dir reversed
	bool hold;		// hold current
	// int target;		// relative
	uchar current_step;
//	uchar acceleration;
	// uchar strength;	// pwm

	bool running;	


};

	void step(bool _dir){
		if(reversed) _dir = !_dir;
		
		if(_dir) current_step++;
		else current_step--;
		current_step &= (halfstep ? 7 : 3);
	}
	void steps(int _t){
		target = _t;
	}
	void update(){
		if(target == 0) return;
		
		if(target<0){
			step(true);
			target++;
		} 
			
		else{
			step(false);
			target--;
		} 
	}
//	uchar timer;
//	uchar speed;
//	uchar acc_type:2;
	
//	void (*step)(stepper_motor_struct*);
//	uchar (*getStep)(stepper_motor_struct*);	
// static uchar getStep(StepperMotor* st);

// void initStepper(StepperMotor* st){
	// st->halfstep_mode = true;
//   	st->total_steps = HALFSTEPS;
	// st->reversed = false;
	// st->current_step = 0;
//	st->steps_done =
//	st->target =
//	st->acceleration =
//	st->timer = 0;
//	st->acc_type = LINEAR;		//impl
	
	// imp minmax
//	st->speed = 40;
	
//	st->step = &step;
//	st->getStep = &getStep;
// }

// StepperMotor* createStepper(){
	// StepperMotor* result = (StepperMotor*)calloca(sizeof(StepperMotor));
	// result->total_steps = FULLSTEP_MODE;
	// return result;
// }

// static void step(StepperMotor* st){
//	if(st->target == 0)
//		return false;
   // uchar cs = st->current_step;
   
	// if(st->reversed == true){
		// if(cs != 0)
		  // cs--;
	       // else{
		  // cs = st->halfstep_mode ? 7 : 3;
	       // }
//		st->target++;
		
		// if(st->current_step) {
			// st->current_step--;
		// }
		// else {
			// st->current_step = st->total_steps;	
		// }
	// }
	// else{
//		st->target--;
//	        if(cs != st->total_steps-1)
		  // cs++;
//		else
//		  cs = 0;
	       // cs &= st->halfstep_mode ? 7 : 3;
	// }
		
	//st->current_step &= st->total_steps;
	// st->current_step = cs;
// }

// static uchar getStep(StepperMotor* st){
	
   // uchar result = STEP_SEQUENCE[ st->current_step ];
   // if( (st->current_step & 1) && (st->halfstep_mode == true) )
		// result >>= 4;
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
   
	// return (0x0f & result);
	//return ( result );
// }
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
// }StepperMotor;

#endif
