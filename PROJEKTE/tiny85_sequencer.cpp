
#include <Macros.h>
// #include <beep.h>
#include <abuttons.h>
#include <avr/pgmspace.h>

#define PRESCALE 8
#define F_CPU 8000000UL

const uint16_t freq[] PROGMEM = {
  //   C,   C#,    D,   D#,    E,    F,   F#,    G,   G#,    A,   A#,    B
	 // 262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494, //Oktave 5
	 // 523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988, //Oktave 6
	// 1046, 1109, 1175, 1244, 1328, 1397, 1480, 1568, 1661, 1760, 1865, 1975, //Oktave 7
	2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951  //Oktave 8
};

typedef struct step_struct{
	bool on : 1;
	bool portamento : 1;
	uchar note : 6;
	uchar length : 4;
}STEP;

#define MODE_TOGGLE 	0
#define MODE_NOTE 		1
#define MODE_LENGTH		2
#define MODE_PORTA 		3

bool run = true;
bool tone_on = false;

volatile uchar sub_cnt=0;
volatile uchar spd_cnt=0;
 
STEP steps[16];

	uchar cstep=0;
	uchar a=0;
	uchar abtn=0;
	uchar spd=140;
	uint tone_delay = 1;
	uint old_tone_delay = 1;

void init();
void inputs();
void substep();
void step();
uint getDelay(uint f);
// void playNote(uchar _n);
// static void ctc_on(void);
// static void ctc_off(void);
// int f(int n, int o);	


int main ()
{ 
	OUTPUT(0);
	
	BUTTON(3);
	BUTTON(4);
	BUTTON(2);
	// INPUT(2);
	// INPUT(4);
	// INPUT(3);
	init();
	// init_abuttons(6,2);
	
	TCCR0B = 2;
	TIMSK |= B(TOIE0);
	
	sei();
	// uchar cnote=0;
	uchar cnt=0;

    while (1) {
		cnt++;
		cnt&=127;
		if(run){
				
			if(tone_on){
				fbi(PORTB,0);
				if(steps[cstep].portamento){
					
					delay_us(old_tone_delay);

					if(!(cnt&7)){
						
						if(old_tone_delay < tone_delay)
							old_tone_delay++;
						if(old_tone_delay > tone_delay)
							old_tone_delay--;
					}
					
				}
				else{
					delay_us(tone_delay);
					
				}
			}
			else
				OFF(0);
			
		}
}
}
		
/* 		if(!READ(3)){
			while(!READ(3));
			delay_ms(20);
		}
		if(run){
			uint len = (spd|1)<<2;	// total len
			uint d =steps[cstep][1] * (len >> 4); 					// 1 / 16
			
			playNote(steps[cstep][0]);
			delay_ms(d);
			ctc_off();
			delay_ms(len-d);
			
			cstep++;
			cstep&=15;
			
		}
 */


	
void inputs(){
	 
		// abtn=get_button();		
		abtn=adc_read(2);
		abtn >>= 6;
		
		if(abtn){
			a = adc_read(1);	
			
			switch(abtn){
				case MODE_NOTE:
					steps[cstep].note = a>>2;
				
				break;
				case MODE_LENGTH:
					steps[cstep].length = a>>4;
				
				break;
 				case MODE_TOGGLE:
					steps[cstep].on = !steps[cstep].on;
				break;
 				case MODE_PORTA:
					steps[cstep].portamento = !steps[cstep].portamento;
				
				break;
				// case 4:
					// run = !run;
					// delay_ms(1500);
				// break;				
				// case 5:
				// break; *
				default:

					// _delay_ms(200);
					
				break;
				
			}

}}
void step(){
	
	if(steps[cstep].on){
		old_tone_delay = tone_delay;
		tone_delay = getDelay(steps[cstep].note);
		tone_on=true;
	}
	else
		tone_on=false;


	cstep++;
	cstep&=15;
	
}
void substep(){
	if(run){
		uchar len = steps[cstep].length;
		tone_on = (len > sub_cnt);
		
		sub_cnt++;
		sub_cnt&=15;
	}	
}
	
uint getDelay(uint _n){
	if(_n >= 48)
		_n = 47;
	
	uchar n = _n % 12;
	uchar o = MIN(3,(_n / 12));
	
	
	uint f = pgm_read_word(&freq[n]);
	f >>= (3-o);
	
	// ulong tmp = 500000UL / (f);	
	ulong tmp = 1000000UL / (f);	
	uint d = (tmp>>1);
	
	// calc f delay
	return  d;
	
	
	
	// calc f delay

	// uchar note = _n % 12;
	// uchar oktave = _n / 12;
	// Tonfrequenz einstellen
	// _n &= 63;
	// OCR0A = _n << 2;
	// OCR0A = f(note, oktave);
	// OCR0B = f(note, oktave);
	
	// uint f = pgm_read_word(&freq[_n]);
	// OCR0A = (F_CPU/f/2/PRESCALE)-1;
	// ctc_on();				// Ton einschalten
	// uint l = 100 * (20-_l);
	// beep(0,f,l);
}

// void mute(){
	// ctc_off();				
// }

void init(){
	for(uchar i=0;i<16;i++){
		uint r = lfsr();
		steps[i].note = r % 47;
		steps[i].on = true; 
		if(i&3)
			steps[i].length = 1+(i&7);
		else
			steps[i].length = 8;
		
		if(i&1)
			steps[i].portamento = true;
	}
}
/* 
static void ctc_on(void) {
	// CTC mode mit toggeln von OC0A/OC0B (Tiny85)
	TCCR0A |= (1<<COM0A0 | 1<<COM0B0 | 1<<WGM01);
	TCCR0B = 4;
}

static void ctc_off(void) {
	TCCR0A = 0;
	TCCR0B = 0;
}
int f(int n, int o) {
	// lustiger Nebeneffekt des Zugriffs auf den Flash-RAM: die Zeit
	// die dazu notwendig ist, fuegt eine kleine Pause zwischen den 
	// Toenen ein, die auch ganz sinnvoll ist...
	//	
	// lt. Datenblatt fuer CTC-Mode ist
	// Focnx = Fclk / (2 * Prescale * (1 + OCRnx)
	// umgestellt nach OCRnx:
	// OCRnx = (Fclk / Focnx / 2 / Prescale) - 1
	return (F_CPU/pgm_read_word(&freq[n+(o-5)*12])/2/PRESCALE)-1;
}
 */

ISR(TIMER0_OVF_vect){
	if(spd_cnt >= spd){
		spd_cnt = 0;
	
		if(!sub_cnt)
			step();
		
		if(1&sub_cnt)
			inputs();
		
		substep();
	}
	else 
		spd_cnt++;
	
}
