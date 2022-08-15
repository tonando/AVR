#ifndef Sequencer_h
#define Sequencer_h

#include "Macros.h"
#include "Config.h"
#include "Step.h"
#include "Pattern.h"
#include "Notes.h"
#include "shiftinout.h"
#include "SPI.h"

#define NUM_PATTERNS 	8

#ifdef CPLUSPLUS

class Sequencer{
	public:
		Sequencer();
		void init();

		void play();
		bool toggleFollow() { following = !following; return following; };
		
		void nextStep();
		void nextSubStep();
		
		bool patternUp();
		bool patternDown();

		void scroll(char);
		
		void setTempo(uchar);

		uchar current_pattern;
		uchar cursor;
		
		Step* getStep(uchar st){ return getStep(current_pattern, st); };
		Step* getStep(uchar pat, uchar st){ return &patterns[pat].steps[st]; };	
		Step *cStep; 
		Pattern *cPat; 

		// Data array
		Pattern patterns[NUM_PATTERNS];

		bool ready;
		bool playing;
		
		//private
//		uchar arp_edit_mode;  // ?
//		uchar part_edit_mode; //?
//		uchar step_button_mode; 
				
		
//	DEBUG ///
#if DEBUG
		void printAll();
		void printNotes();
		void printPattern();
		void printInput();
		void printStep(uchar index);
#endif
/////////////		
		
		void update();
//#if !DEBUG
	private:
//#endif
		void setBaudrate(ulong);
		void setPattern(uchar index);
		void initSteps();
		
		void updateSpi(byte);
		void updateLeds();
		
		bool song_mode; 

		uchar base_note;
		
	// TIMING
		uchar substep_counter; 
		uint last_millis;
		uchar step_ms; // * divider
		uchar step_us; // * divider


		uint shift_out_value;
		uint shift_in_value;
	
	// LEDS
		void ledsPwm();
		bool leds_pwm_enabled;
		uchar leds_pwm[8];	// div2 -> red green
		uchar led_values;	// BIN	??
		uchar ui_led_values;	
		uchar scroll_pos;
		
	// OUTPUT 
		//	MIDI
		void midiSetChannel(uchar ch){midi_channel = ch;};
		void midiOut(uchar cmd, uchar a, uchar b);
		void midiOut(uchar cmd, uchar a);


		void serialOut(); // ALT
		void pwmOut();
		
		void toneOut(byte);	// note
		void toneOut(byte, byte); // note & length

		void noteOn(byte);
		void noteOff(byte);
		
	// INPUT
		void stepPressed(byte);	
		void stepReleased(byte);

		uchar readKnob(byte);
		void checkAnalogButtons();
		void checkButtons();		

		uchar STEP_BUTTON_MODE; 

		uchar knob_values[5]; 
		uchar step_button_values;	// shift in
		uchar last_analog_button;
		uint button_timers[INTERFACE_SIZE];	
					
		uchar midi_channel;
		uchar active_patterns; 
	// UTILS		

		uchar getActiveSteps();
		uchar getActiveSteps(byte,byte);
		
		uchar scale;	// index ...oder kopie?

		bool following; 
		
};
#endif
// ???
/*
struct Button{
	public:
		Button();
		void update();
	private:
		uchar pin;
		bool last_value;
		uint hold_time;
		ulong last_millis;
	
//	shift
//	push
//	edge triggered
};
#else
	typedef struct {
		uchar current_step;
		uchar current_pattern;
		
		uchar num_patterns;
		uchar tempo;
		bool use_ext_clock;
		bool following;
		
		
		// functions
		void (*start)();
		void (*stop)();
		
		void (*command)(uint32_t);
	}Sequencer;

	void initSequencer();
#endif

// DEBUG
///////////////////////////////////////////////////////////////
//	Step Sequenzer

class Sequencer{
	public:
		Sequencer();
		void init();

		void play();
		bool toggleFollow() { following = !following; return following; };
		
		void nextStep();
		void nextSubStep();
		
		bool patternUp();
		bool patternDown();

		void scroll(char);
		
		void setTempo(byte);

		uchar current_pattern;
		uchar cursor;
		
		Step* getStep(uchar st){ return getStep(current_pattern, st); };
		Step* getStep(uchar pat, uchar st){ return &patterns[pat].steps[st]; };	
		Step *cStep; 
		Pattern *cPat; 

		// Data array
		Pattern patterns[NUM_PATTERNS];

		bool ready;
		bool playing;
		
		//private
//		uchar arp_edit_mode;  // ?
//		uchar part_edit_mode; //?
//		uchar step_button_mode; 
				
		
//	DEBUG ///
#if DEBUG
		void printAll();
		void printNotes();
		void printPattern();
		void printInput();
		void printStep(uchar index);
#endif
/////////////		
		
		void update();
//#if !DEBUG
	private:
//#endif
		void setBaudrate(ulong);
		void setPattern(uchar index);
		void initSteps();
		
		void updateSpi(byte);
		void updateLeds();
		
		bool song_mode; 

		uchar base_note;
		
	// TIMING
		uchar substep_counter; 
		uint last_millis;
		uchar step_ms; // * divider
		uchar step_us; // * divider


		uint shift_out_value;
		uint shift_in_value;
	
	// LEDS
		void ledsPwm();
		bool leds_pwm_enabled;
		uchar leds_pwm[8];	// div2 -> red green
		uchar led_values;	// BIN	??
		uchar ui_led_values;	
		uchar scroll_pos;
		
	// OUTPUT 
		//	MIDI
		void midiSetChannel(uchar ch){midi_channel = ch;};
		void midiOut(uchar cmd, uchar a, uchar b);
		void midiOut(uchar cmd, uchar a);


		void serialOut(); // ALT
		void pwmOut();
		
		void toneOut(byte);	// note
		void toneOut(byte, byte); // note & length

		void noteOn(byte);
		void noteOff(byte);
		
	// INPUT
		void stepPressed(byte);	
		void stepReleased(byte);

		uchar readKnob(byte);
		void checkAnalogButtons();
		void checkButtons();		

		uchar STEP_BUTTON_MODE; 

		uchar knob_values[5]; 
		uchar step_button_values;	// shift in
		uchar last_analog_button;
		uint button_timers[INTERFACE_SIZE];	
					
		uchar midi_channel;
		uchar active_patterns; 
	// UTILS		

		uchar getActiveSteps();
		uchar getActiveSteps(byte,byte);
		
		uchar scale;	// index ...oder kopie?

		bool following; 
		
};
*/
// ???
/*
struct Button{
	public:
		Button();
		void update();
	private:
		uchar pin;
		bool last_value;
		uint hold_time;
		ulong last_millis;
	
//	shift
//	push
//	edge triggered
};

*/

#endif
