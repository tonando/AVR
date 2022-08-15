#ifndef Step_h
#define Step_h

#include "Config.h"

#define INIT_NOTE_LENGTH 	8
#define INIT_NOTE_VELOCITY 	8
#define INIT_TRANSPOSE 		16

#define LENGTH_BITS 	4
#define VELOCITY_BITS 	4

///////////////////////////////////////////////////////////////
//	Step

typedef struct {

	bool on 	: 1;
	bool link 	: 1;
	
	byte note 		: 6; 
	byte length 	: LENGTH_BITS;
	byte velocity	: VELOCITY_BITS;
	
	//byte pattern : 6; // 64
}Step;

void stepToggle( Step* s){ s->on = !s->on; }
void stepTranspose(Step* s, int8_t t){
	s->note++;
}
bool inScale(byte note){
	return true;
}

#endif

	//	void toggle(){ 				fbi(note,7); }; 
//	void setLength(byte b){ 	lenvel = ((lenvel & 0xF0) | (constrain(b,1,15))); 		}; 
//	void setVelocity(byte b){ 	lenvel = ((lenvel & 0x0F) | (constrain(b,1,15) << 4));  };
//	void setNote(byte b) { 		note = constrain(b,1,127); 	};
//	byte getLength(){ 			return (0x0F & lenvel); 	};
//	byte getVelocity(){ 		return (lenvel >> 4); 		};
//	bool isActive() {	return bitRead(note,7); }; 
//	void setPortamento(byte b){ bitWrite(note,6,b); };
//	bool portamento(){ 	return bitRead(note,6); };

//struct Step * stepCreate(){	return malloc(sizeof(Step));}
//typedef struct step_struct Step;
//extern byte stepsFromScale(char,byte);
// finde naehesten grundton. durchlaufe halbtoene und schliesse falsche toene aus
//extern byte limitToScale(byte note, byte scale_index, byte root);
//extern bool inScale(byte,byte,byte);
//extern byte stepsFromScale(char,byte);
// finde naehesten grundton. durchlaufe halbtoene und schliesse falsche toene aus
//extern byte limitToScale(byte note, byte scale_index, byte root);
//extern bool inScale(byte,byte,byte);