#ifndef Config_h
#define Config_h

#include "Macros.h"
#include "Inputs.h"

/************************/
/*	SETTINGS			*/
/************************/

#define DEBUG_MODE 			true
#define BAUD_RATE 			31250 // 8 / 16 mhz

#define ENABLE_ARP 			true // ???????
#define ENABLE_SPI 			true // ???????
#define ENABLE_LED_FADING 	true // ???????

// Output
#define STEP_TIMER  0
#define TONE_OUT 	1
#define CV_TIMER	2
//#define MIDI_OUT 	true	

// Size
#define INTERFACE_SIZE 8	// Number of Steps on Controller

// Limits
#define MAX_MEMORY 1024

/************************/
/*	INPUT				*/
/************************/

#define RX_PIN 			0
#define TX_PIN 			1
#define TEST_BUTTON_PIN 2
#define CV_A_PIN 		3
#define TEST_LED_PIN 	4
#define SHIFT_PS_PIN	8
#define TONE_PIN 		9
#define LATCH_PIN 		10
#define SHIFT_OUT_PIN 	11
#define SHIFT_IN_PIN 	12
#define SHIFT_CLOCK_PIN 13

#define CV_B_PIN 		-1
#define EXT_CLOCK_PIN 	-1 // T0
#define DEMUX_A_PIN		-1
#define DEMUX_B_PIN		-1
#define DEMUX_C_PIN		-1

// Analog in
#define TEMPO_KNOB 		0
#define STEP_KNOB 		1
#define PERFORM_A_KNOB 	2
#define PERFORM_B_KNOB 	3
#define BUTTONS_A 		5 
#define BUTTONS_B 		-1
#define DEMUX_IN 		-1

///////////////////////
// Mapping	
// AB 	= Analog Button
// SH_x = shift in #

#define BT_PLAY			AB_1
#define BT_SHIFT		SH_B_1

#define BT_PERFORM_A	SH_B_2	// set
#define BT_PERFORM_B	SH_B_3

#define BT_STEPS_KNOB	SH_B_4	// enable knob

#define BT_STEPS_NOTE	AB_4	// + toggle
#define BT_STEPS_LENGTH AB_5	// + link
#define BT_STEPS_VEL 	AB_6	// + portamento
#define BT_STEPS_MOD 	AB_7	// 

#define BT_MODE_PIANO 	AB_8
#define BT_MODE_LOOP 	AB_9
#define BT_MODE_EDIT 	AB_10

#define BT_TRACK_TOGGLE	AB_12	
#define BT_TRACK_SOLO	AB_13	
#define BT_TRACK_UP		AB_14	
#define BT_TRACK_DOWN	AB_15	
#define BT_TRACK_MORE	AB_16	
#define BT_TRACK_LESS	AB_17	

#define BT_ARP_DIR		AB_21	
#define BT_ARP_PING		AB_22


/************************/
/*	INITIAL VALUES		*/
/************************/







#endif