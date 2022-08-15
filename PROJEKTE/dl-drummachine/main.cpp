/* Step Sequencer Drum Machine
 * ECE 4760 Cornell University Final Project
 * Dan Felicetta & Arun Swain
 * 5/2/2011
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "samples.h"
// #include "lcd_lib.h"

#define BUTTON_SCAN_TIME 150 // in 1/2 milliseconds

// UI Control SM states
#define TRACK_SEL 0
#define SOUND_SEL 1
#define TEMPO_SEL 2
#define PRESET_SEL 3
#define SV_SEL 4
#define LD_SEL 5
#define SAVE_PRESET 6
#define LOAD_PRESET 7

// UI bit masks
#define PLAY_PAUSE 0x1
#define CLEAR_TRACK 0x2
#define CLEAR_ALL 0x4
#define LEFT 0x8
#define RIGHT 0x10
#define UP 0x20
#define DOWN 0x40

// Playtrack bit masks
#define TRACK1 0x1
#define TRACK2 0x2
#define TRACK3 0x4
#define TRACK4 0x8
#define TRACK5 0x10
#define TRACK6 0x20

#define NUM_PRESETS 50
#define NUM_DEFAULT_PRESETS 4
#define MIN_BPM 60
#define MAX_BPM 220
#define NUM_TRACKS 6

// timers
volatile unsigned int input_timer = BUTTON_SCAN_TIME;
volatile unsigned int seq_timer = 0;
unsigned int tempo = 250; // 120 bpm default: 250 = 30000/120

typedef struct setting {
	uint16_t seq_bmp[NUM_TRACKS]; // bitmaps for each track
	uint8_t sound[NUM_TRACKS]; // sound id for each track
	uint8_t bpm;
} setting;

// default settings - cleared 120 bpm
setting settings = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, 120}; 

// EEPROM presets
setting EEMEM presets[NUM_PRESETS];

setting default_presets[NUM_DEFAULT_PRESETS] = 
		{{{0x0581, 0x1010, 0x1515, 0x4040, 0, 0}, {14, 17, 12, 10, 0, 0}, 120},
		 {{0x2105, 0x1010, 0x1111, 0, 0, 0}, {1, 16, 12, 0, 0, 0}, 120},
		 {{0x1111, 0x1090, 0xdddd, 0x4444, 0, 0}, {14, 15, 10, 12, 0, 0}, 130},
		 {{0x1111, 0x4444, 0x0101, 0, 0, 0}, {13, 10, 6, 0, 0, 0}, 120}};

// EEPROM dummy block to check if its been wiped after programming
unsigned char EEMEM ee_init;
unsigned char ee_init_ram;
		 
unsigned char preset_load_num = 0;
unsigned char preset_save_num = NUM_DEFAULT_PRESETS;

unsigned char step_number = 0; // sequencer step number
unsigned char current_track = 0; // current track for input/display

// Audio playback variables
volatile unsigned int audio_out = 0;
volatile unsigned char spi_junk;
volatile unsigned int sample_count[NUM_TRACKS] = {0, 0, 0, 0, 0, 0};
unsigned char play_track = 0; // b0 - t1, b1 - t2, b2 - t3, b3 - t4, b4 - t5, b5 - t6

// UI variables
unsigned int seq_button_scan;
unsigned int last_seq_button_scan = 0xffff;

unsigned char ctrl_button_scan;
unsigned char last_ctrl_button_scan = 0xff;

unsigned char ctrl;
unsigned char play = 1;

unsigned char ctrl_state = TRACK_SEL;

// LCD variables
// uint8_t lcd_track_buffer[4];
// uint8_t lcd_tempo_buffer[7];
// uint8_t lcd_preset_buffer[11];

const uint8_t save_as_str[] = "Save as...\0";
const uint8_t load_str[] = "Load...\0";
const uint8_t presets_str[] = "Presets\0";

// loop variables
unsigned char i, j, k, h;

void do_step(void);
void led_update(void);

void lcd_track(void);
void lcd_pattern(void);
void lcd_tempo(void);
void lcd_preset(void);
void lcd_return(void);

// Audio Playback ISR (22 KHz)
ISR (TIMER1_COMPA_vect)	{
	audio_out = 0;
	
	// if ((play_track & TRACK1) && sample_count[0] < sample_len[settings.sound[0]])	
		audio_out += (pgm_read_byte(&(sample[settings.sound[0]][sample_count[0]++])));
	
	// if ((play_track & TRACK2) && sample_count[1] < sample_len[settings.sound[1]])
		// audio_out += (pgm_read_byte(&(sample[settings.sound[1]][sample_count[1]++])));
			
	// if ((play_track & TRACK3) && sample_count[2] < sample_len[settings.sound[2]])	
		// audio_out += (pgm_read_byte(&(sample[settings.sound[2]][sample_count[2]++])));
	
	// if ((play_track & TRACK4) && sample_count[3] < sample_len[settings.sound[3]])	
		// audio_out += (pgm_read_byte(&(sample[settings.sound[3]][sample_count[3]++])));
	
	// if ((play_track & TRACK5) && sample_count[4] < sample_len[settings.sound[4]])	
		// audio_out += (pgm_read_byte(&(sample[settings.sound[4]][sample_count[4]++])));
	
	// if ((play_track & TRACK6) && sample_count[5] < sample_len[settings.sound[5]])	
		// audio_out += (pgm_read_byte(&(sample[settings.sound[5]][sample_count[5]++])));

	// send audio_out to 10-bit DAC on SPI
	PORTB &= ~(1<<PINB4); // B.4 (DAC /CS)
	SPDR = (char) ((audio_out >> 6) & 0x000f); //byte 1 0 0 0 0 b9 b8 b7 b6
	while (!(SPSR & (1<<SPIF)));
	spi_junk = SPDR;

	SPDR = (char) ((audio_out & 0x003f) << 2); //byte 2 b5 b4 b3 b2 b1 b0 0 0
	while (!(SPSR & (1<<SPIF)));
	spi_junk = SPDR;
	PORTB |= (1<<PINB4);
}

// Sequencer Timing ISR
ISR (TIMER0_COMPA_vect)	{
	if (input_timer>0) input_timer--;
	if (seq_timer>0) seq_timer--;
}

/*
 * Sequencer step - update the LEDs and audio playback
 */
void do_step(void)	{
	led_update();

	play_track = (((settings.seq_bmp[0]>>step_number) & 0x1) | 
				  (((settings.seq_bmp[1]>>step_number) & 0x1)<<1) |
				  (((settings.seq_bmp[2]>>step_number) & 0x1)<<2) |
				  (((settings.seq_bmp[3]>>step_number) & 0x1)<<3) |
				  (((settings.seq_bmp[4]>>step_number) & 0x1)<<4) |
				  (((settings.seq_bmp[5]>>step_number) & 0x1)<<5));
	
	sample_count[0] = sample_count[1] = sample_count[2] = 0;
	sample_count[3] = sample_count[4] = sample_count[5] = 0;

	step_number = step_number < 15 ? step_number + 1 : 0;
}

/*
 * Update the LEDs
 * Data is sent to the two 74HC595s by bit-banging
 */
void led_update()	{
	
	for (i=0; i<16; i++)	{
		// LED lights on sequencer step and XORs selected steps
		if (((settings.seq_bmp[current_track] ^ (1 << step_number))>>i) & 0x1) 
			PORTB |= (1<<PINB2);
		else PORTB &= ~(1<<PINB2);
		
		// pulse the shift clk
		PORTB |= (1<<PINB1);
		PORTB &= ~(1<<PINB1);
	}
	// pulse the storage clk
	PORTB |= (1<<PINB0);
	PORTB &= ~(1<<PINB0);
}

/* LCD update functions */
/*
void lcd_track(void)	{
	//sprintf(lcd_track_buffer, "T#%d", current_track+1);
	LCDstring(lcd_track_buffer, strlen(lcd_track_buffer));
	LCDGotoXY(0, 0);
}

void lcd_pattern(void)	{
	LCDstring("            ", 12);
	CopyStringtoLCD(sample_text[settings.sound[current_track]], 5, 0);
	LCDGotoXY(5, 0);
}

void lcd_tempo(void)	{
	//sprintf(lcd_tempo_buffer, "%03dBPM", settings.bpm);
	LCDstring(lcd_tempo_buffer, strlen(lcd_tempo_buffer));
	LCDGotoXY(0, 1);
}

void lcd_preset_save(void)	{
	//sprintf(lcd_preset_buffer, "Preset #%02d", preset_save_num + 1);
	LCDstring(lcd_preset_buffer, strlen(lcd_preset_buffer));
	LCDGotoXY(0, 1);
}

void lcd_preset_load(void)	{
	//sprintf(lcd_preset_buffer, "Preset #%02d", preset_load_num + 1);
	LCDstring(lcd_preset_buffer, strlen(lcd_preset_buffer));
	LCDGotoXY(0, 1);
}

void lcd_return(void)	{
	LCDclr();
	LCDGotoXY(0, 0);
	lcd_track();
	LCDGotoXY(5, 0);
	lcd_pattern();
	LCDGotoXY(0, 1);
	lcd_tempo();
	CopyStringtoLCD(presets_str, 9, 1);
	LCDGotoXY(9, 1);
	ctrl_state = PRESET_SEL;
}
*/
/* Scan for pushbutton input 
 * 
 * Runs every BUTTON_SCAN_TIME
 * 
 * Button values are read from 74HC165s by bit-banging
 */
void scan_buttons(void)	{
	// bitmaps representing current scan of active low buttons
	seq_button_scan = 0;
	ctrl_button_scan = 0;

	// pulse /PL to load button values to shift regs
	PORTB &= ~(1<<PINB3);
	PORTB |= (1<<PINB3);

	// read the values
	for (j=0; j<24; j++)	{
		if (j<16 && (PINC & 0x2)) seq_button_scan |= (1<<j);
		else if (PINC & 0x2) ctrl_button_scan |= (1<<(j-16));

		// pulse the clock to shift
		PORTC &= ~(1<<PINC0);
		PORTC |= (1<<PINC0);
	}
	
	// & with ~last_scan to act on button release
	
	// ctrl vector
	// b0 - play/pause 0x1
	// b1 - clear track 0x2
	// b2 - clear all 0x4
	// b2 - left 0x8
	// b3 - right 0x10
	// b4 - up 0x20
	// b5 - down 0x40
	ctrl = (~last_ctrl_button_scan) & ctrl_button_scan; 
	
	// sequence buttons toggle
	settings.seq_bmp[current_track] ^= (~last_seq_button_scan) & seq_button_scan; 
	
	// control actions
	// play/pause
	play ^= ctrl & PLAY_PAUSE;
	
	// clear track
	if (ctrl & CLEAR_TRACK)	settings.seq_bmp[current_track] = 0; 

	//clear all
	if (ctrl & CLEAR_ALL) {
		settings.seq_bmp[0] = settings.seq_bmp[1] = settings.seq_bmp[2] = 0;
		settings.seq_bmp[3] = settings.seq_bmp[4] = settings.seq_bmp[5] = 0;
		settings.sound[0] = settings.sound[1] = settings.sound[2] = 0;
		settings.sound[3] = settings.sound[4] = settings.sound[5] = 0;
		settings.bpm = 120;
		tempo = 30000/settings.bpm;
		current_track = 0;
		// LCDGotoXY(5, 0);
		// lcd_pattern();
		// LCDhome();
		// lcd_track();
		// LCDGotoXY(0,1);
		// lcd_tempo();
		// LCDhome();
		ctrl_state = TRACK_SEL;
	}
	
	// Up/Down/L/R input is processed via state machine
	if (ctrl > 4)	{
		switch (ctrl_state)	{
		case TRACK_SEL:
			if (ctrl & LEFT)	{
				// LCDGotoXY(9, 1);
				ctrl_state = PRESET_SEL;
			} else if (ctrl & RIGHT)	{
				// LCDGotoXY(5, 0);
				ctrl_state = SOUND_SEL;
			} else if (ctrl & UP)	{
				current_track = current_track < NUM_TRACKS-1 ? current_track + 1 : 0;
				// LCDGotoXY(5, 0);
				// lcd_pattern();
				// LCDhome();
				// lcd_track();
			} else if (ctrl & DOWN)	{
				current_track = current_track > 0 ? current_track - 1 : NUM_TRACKS-1;
				// LCDGotoXY(5, 0);
				// lcd_pattern();
				// LCDhome();
				// lcd_track();
			}
			break;
		case SOUND_SEL:
			if (ctrl & LEFT)	{
				// LCDGotoXY(0, 0);
				ctrl_state = TRACK_SEL;
			} else if (ctrl & RIGHT)	{
				// LCDGotoXY(0, 1);
				ctrl_state = TEMPO_SEL;
			} else if (ctrl & UP)	{
				settings.sound[current_track] = settings.sound[current_track] < NUM_SAMPLES-1 ? settings.sound[current_track] + 1 : 0;
				// lcd_pattern();
			} else if (ctrl & DOWN)	{
				settings.sound[current_track] = settings.sound[current_track] > 0 ? settings.sound[current_track] - 1 : NUM_SAMPLES-1;
				// lcd_pattern();
			}
			break;
		case TEMPO_SEL:
			if (ctrl & LEFT)	{
				// LCDGotoXY(5, 0);
				ctrl_state = SOUND_SEL;
			} else if (ctrl & RIGHT)	{
				// LCDGotoXY(9, 1);
				preset_load_num = 0;
				preset_save_num = NUM_DEFAULT_PRESETS;
				ctrl_state = PRESET_SEL;
			} else if ((ctrl & UP) && settings.bpm < MAX_BPM)	{
				tempo = 30000/(++(settings.bpm));
				// lcd_tempo();
			} else if ((ctrl & DOWN) && settings.bpm > MIN_BPM)	{
				tempo = 30000/(--(settings.bpm));
				// lcd_tempo();
			}
			break;
		case PRESET_SEL:
			if (ctrl & LEFT)	{
				// LCDGotoXY(0, 1);
				ctrl_state = TEMPO_SEL;
			} else if (ctrl & RIGHT)	{
				// LCDGotoXY(0, 0);
				ctrl_state = TRACK_SEL;
			} else if ((ctrl & UP) || (ctrl & DOWN))	{
				// LCDclr();
				// CopyStringtoLCD(save_as_str, 0, 0);
				// LCDGotoXY(0, 1);
				// lcd_preset_save();
				// LCDGotoXY(0, 0);
				ctrl_state = SV_SEL;
			}
			break;
		case SV_SEL:
			if (ctrl & LEFT)	{ 
				// lcd_return();
			} else if (ctrl & RIGHT)	{
				// LCDGotoXY(0, 1);
				ctrl_state = SAVE_PRESET;
			} else if ((ctrl & UP) || (ctrl & DOWN))	{
				// LCDstring("          ", 10);
				// CopyStringtoLCD(load_str, 0, 0);
				// LCDGotoXY(0, 1);
				// lcd_preset_load();
				// LCDhome();
				ctrl_state = LD_SEL;
			}
			break;
		case LD_SEL:
			if (ctrl & LEFT)	{
				// lcd_return();
			} else if (ctrl & RIGHT)	{
				// LCDGotoXY(0, 1);
				ctrl_state = LOAD_PRESET;
			} else if ((ctrl & UP) || (ctrl & DOWN))	{
				// LCDstring("          ", 10);
				// CopyStringtoLCD(save_as_str, 0, 0);
				// LCDGotoXY(0, 1);
				// lcd_preset_save();
				// LCDhome();
				ctrl_state = SV_SEL;
			}
			break;
		case SAVE_PRESET:
			if (ctrl & LEFT)	{
				// LCDGotoXY(0, 0);
				ctrl_state = SV_SEL;
			} else if (ctrl & RIGHT)	{
				// do the save to eeprom
				eeprom_write_block((const void*) &settings, (void*) &(presets[preset_save_num]), sizeof(setting));
				// lcd_return();
			} else if (ctrl & UP)	{
				preset_save_num = preset_save_num < NUM_PRESETS-1 ? preset_save_num + 1 : NUM_DEFAULT_PRESETS;
				// lcd_preset_save();
			} else if (ctrl & DOWN)	{
				preset_save_num = preset_save_num > NUM_DEFAULT_PRESETS ? preset_save_num - 1 : NUM_PRESETS - 1;
				// lcd_preset_save();
			}
			break;
		case LOAD_PRESET:
			if (ctrl & LEFT)	{
				// LCDGotoXY(0, 0);
				ctrl_state = LD_SEL;
			} else if (ctrl & RIGHT)	{
				// do the load from eeprom
				eeprom_read_block((void*) &settings, (const void*) &(presets[preset_load_num]), sizeof(setting));
				tempo = 30000/(settings.bpm);
				current_track = 0;
				step_number = 0;
				// lcd_return();
			} else if (ctrl & UP)	{
				preset_load_num = preset_load_num < NUM_PRESETS-1 ? preset_load_num + 1 : 0;
				// lcd_preset_load();
			} else if (ctrl & DOWN)	{
				preset_load_num = preset_load_num > 0 ? preset_load_num - 1 : NUM_PRESETS - 1;
				// lcd_preset_load();
			}
			break;
		}
	}
	
	led_update();
	
	last_seq_button_scan = seq_button_scan;
	last_ctrl_button_scan = ctrl_button_scan;
}

int main(void)	{
	
	// set up timer1 for audio sample play out 
	TIMSK1 = (1<<OCIE1A);	// cmp match ISR
	// turn on clear-on-match
	TCCR1B = (1<<WGM12) | (1<<CS11);
	OCR1A = 91;  		// ~22068 Hz
	
	//set up timer0 for .5 ms timebase
	TIMSK0 = (1<<OCIE0A);
	TCCR0A = (1<<WGM01);
	TCCR0B = 3; // divide by 64
	OCR0A = 125;

	// UI I/O Setup
	// B.7 - DAC CLK (2)
	// B.5 - DAC IN (1)
	// B.4 - DAC /CS (3)
	// B.3 - 165 /PL (1)
	// B.2 - 595 Din (14)
	// B.1 - 595 SH_CLK (11)
	// B.0 - 595 ST_CLK (12)
	// C.0 - 165 CLK (2)
	// C.1 - 165 Q7 IN (9)
	DDRB = ~(1<<PINB6);
	DDRC = (1<<PINC0);
	
	PORTB = (1<<PINB4); // active low cs
	PORTC = (1<<PINC0); // 165 clock high idle
	
	// SPI Init
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPOL);  // master, 8 MHz
	SPSR = (1<<SPI2X);
	
	// EEPROM initialization if it's been wiped by programming
	eeprom_read_block(&ee_init_ram, &ee_init, 1);
	if (ee_init_ram != 0x11) {
		ee_init_ram = 0x11;
		eeprom_write_block(&ee_init_ram, &ee_init, 1);
		for (k=0; k<NUM_PRESETS; k++)	{
			if (k < NUM_DEFAULT_PRESETS)	{
				eeprom_write_block((const void*) &(default_presets[k]), (void*) &(presets[k]), sizeof(setting));
			} else	{
				eeprom_write_block((const void*) &settings, (void*) &(presets[k]), sizeof(setting));
			}
		}
	}
	
	// LCD initialization
	// LCDinit();
	// LCDcursorOn();
	// LCDclr();
	// LCDGotoXY(0,0);
	
	//sprintf(lcd_track_buffer, "T#%d", current_track+1);
	//sprintf(lcd_tempo_buffer, "%3dBPM", settings.bpm);
	
	// LCDstring(lcd_track_buffer, strlen(lcd_track_buffer));
	// LCDGotoXY(0,1);
	// LCDstring(lcd_tempo_buffer, strlen(lcd_tempo_buffer));
	
	// CopyStringtoLCD(sample_text[settings.sound[current_track]], 5, 0);
	// CopyStringtoLCD(presets_str, 9, 1);

	// LCDhome();

	sei();

	while (1)	{
		if ((seq_timer == 0)  && play) {seq_timer = tempo; do_step();}
		if (input_timer == 0) {input_timer = BUTTON_SCAN_TIME; scan_buttons();};
	}
}