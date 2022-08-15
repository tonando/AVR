#ifndef Interface_h
#define Interface_h

#include "Macros.h"
#include "Sequencer.h"
#include "SPI.h"
#include "tone.h"

#define UI_NUM_LEDS 8
#define UI_MUX_SHIFT 1
#define UI_MUX_SHIFT_PIN PB2
#define UI_MUX_SHIFT_PIN PB2

typedef struct {
	uchar port	: 2;
	uchar pin	: 3;
	uchar on	: 1;
}Led;

typedef struct {
	uchar port	: 2;
	uchar pin	: 3;
	uchar on	: 1;
	uchar pullup: 1;
}Button;

typedef struct {
	uchar in;
	uchar out;
	uchar last_in;
	uchar last_out;
}ShiftRegister;



#ifdef CPLUSPLUS

class Interface{
	public:
		Interface(Sequencer);
		void update();
		
	private:
		void updatePorts(uchar);
		
	// SHIFT 
		uchar num_shift;
		ShiftRegister registers[];
		uchar spi(uchar);
	
	// LEDS
		uchar num_leds;
		Led leds[];
		
	// PWM
		void pwm_write(uchar,uchar);
		
	// ANALOG
		uchar num_knobs;
		Knob knobs[];
		uchar readKnob(uchar);
		uchar getAnalogButton();
	
	// BUTTONS
		uchar checkButtons();				
	
	// UTILS		
	//	DEBUG ///


};

Interface::Interface(Sequencer s){
	this.seq = &s;
	this.num_buttons = 0;
	this.num_knobs = 0;
	this.num_leds = 0;
	this.num_shift = 0;
}
void Interface::addButton(uchar _pin,uchar _cmd){
	// this.buttons[num_buttons].port = _port;
	this.buttons[num_buttons].pin = _pin;
	this.buttons[num_buttons].event = _cmd;
	
	this.num_buttons++;

	DDRB &= ~(_BV(_pin));
	PORTB |= (_BV(_pin));
}

void Interface::update(){
	if(num_buttons){
		uchar n = num_buttons;
		do{n--;
			if(!rbi(PINB,buttons[n].pin))
				this.seq.event(buttons[n].event);
		}while(n);
	}
	if(num_knobs){
		
	}
	if(num_leds){
		
	}
}







#endif

#endif
