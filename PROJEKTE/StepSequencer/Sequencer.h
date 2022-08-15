#ifndef Sequencer_h
#define Sequencer_h

#define CPLUSPLUS 1

#include "Macros.h"
#include "Config.h"
#include "Step.h"
#include "Pattern.h"
#include "Notes.h"
#include "shiftinout.h"
//#include "SPI.h"

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
		
		void setTempo(byte);

		byte current_pattern;
		byte cursor;
		
		Step* getStep(byte st){ return getStep(current_pattern, st); };
		Step* getStep(byte pat, byte st){ return &patterns[pat].steps[st]; };	
		Step *cStep; 
		Pattern *cPat; 

		// Data array
		Pattern patterns[NUM_PATTERNS];

		bool ready;
		bool playing;
		
		//private
//		byte arp_edit_mode;  // ?
//		byte part_edit_mode; //?
//		byte step_button_mode; 
				
		
//	DEBUG ///
#if DEBUG
		void printAll();
		void printNotes();
		void printPattern();
		void printInput();
		void printStep(byte index);
#endif
/////////////		
		
		void update();
//#if !DEBUG
	private:
//#endif
		void setBaudrate(ulong);
		void setPattern(byte index);
		void initSteps();
		
		void updateSpi(byte);
		void updateLeds();
		
		bool song_mode; 

		byte base_note;
		
	// TIMING
		byte substep_counter; 
		uint last_millis;
		byte step_ms; // * divider
		byte step_us; // * divider


		uint shift_out_value;
		uint shift_in_value;
	
	// LEDS
		void ledsPwm();
		bool leds_pwm_enabled;
		byte leds_pwm[8];	// div2 -> red green
		byte led_values;	// BIN	??
		byte ui_led_values;	
		byte scroll_pos;
		
	// OUTPUT 
		//	MIDI
		void midiSetChannel(byte ch){midi_channel = ch;};
		void midiOut(byte cmd, byte a, byte b);
		void midiOut(byte cmd, byte a);


		void serialOut(); // ALT
		void pwmOut();
		
		void toneOut(byte);	// note
		void toneOut(byte, byte); // note & length

		void noteOn(byte);
		void noteOff(byte);
		
	// INPUT
		void stepPressed(byte);	
		void stepReleased(byte);

		byte readKnob(byte);
		void checkAnalogButtons();
		void checkButtons();		

		byte STEP_BUTTON_MODE; 

		byte knob_values[5]; 
		byte step_button_values;	// shift in
		byte last_analog_button;
		uint button_timers[INTERFACE_SIZE];	
					
		byte midi_channel;
		byte active_patterns; 
	// UTILS		

		byte getActiveSteps();
		byte getActiveSteps(byte,byte);
		
		byte scale;	// index ...oder kopie?

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
		byte pin;
		bool last_value;
		uint hold_time;
		ulong last_millis;
	
//	shift
//	push
//	edge triggered
};
#else
	typedef struct {
		byte current_step;
		byte current_pattern;
		
		byte num_patterns;
		byte tempo;
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

		byte current_pattern;
		byte cursor;
		
		Step* getStep(byte st){ return getStep(current_pattern, st); };
		Step* getStep(byte pat, byte st){ return &patterns[pat].steps[st]; };	
		Step *cStep; 
		Pattern *cPat; 

		// Data array
		Pattern patterns[NUM_PATTERNS];

		bool ready;
		bool playing;
		
		//private
//		byte arp_edit_mode;  // ?
//		byte part_edit_mode; //?
//		byte step_button_mode; 
				
		
//	DEBUG ///
#if DEBUG
		void printAll();
		void printNotes();
		void printPattern();
		void printInput();
		void printStep(byte index);
#endif
/////////////		
		
		void update();
//#if !DEBUG
	private:
//#endif
		void setBaudrate(ulong);
		void setPattern(byte index);
		void initSteps();
		
		void updateSpi(byte);
		void updateLeds();
		
		bool song_mode; 

		byte base_note;
		
	// TIMING
		byte substep_counter; 
		uint last_millis;
		byte step_ms; // * divider
		byte step_us; // * divider


		uint shift_out_value;
		uint shift_in_value;
	
	// LEDS
		void ledsPwm();
		bool leds_pwm_enabled;
		byte leds_pwm[8];	// div2 -> red green
		byte led_values;	// BIN	??
		byte ui_led_values;	
		byte scroll_pos;
		
	// OUTPUT 
		//	MIDI
		void midiSetChannel(byte ch){midi_channel = ch;};
		void midiOut(byte cmd, byte a, byte b);
		void midiOut(byte cmd, byte a);


		void serialOut(); // ALT
		void pwmOut();
		
		void toneOut(byte);	// note
		void toneOut(byte, byte); // note & length

		void noteOn(byte);
		void noteOff(byte);
		
	// INPUT
		void stepPressed(byte);	
		void stepReleased(byte);

		byte readKnob(byte);
		void checkAnalogButtons();
		void checkButtons();		

		byte STEP_BUTTON_MODE; 

		byte knob_values[5]; 
		byte step_button_values;	// shift in
		byte last_analog_button;
		uint button_timers[INTERFACE_SIZE];	
					
		byte midi_channel;
		byte active_patterns; 
	// UTILS		

		byte getActiveSteps();
		byte getActiveSteps(byte,byte);
		
		byte scale;	// index ...oder kopie?

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
		byte pin;
		bool last_value;
		uint hold_time;
		ulong last_millis;
	
//	shift
//	push
//	edge triggered
};

*/

#endif
