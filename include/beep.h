#ifndef BEEP_h
#define BEEP_h

#include <Macros.h>
#include <avr/pgmspace.h>

// ARGS:
// 	pin number
// 	frequency (hertz)
// 	duration (millis)
	
void beep(uchar _p, uint _f, uint _l);
void beep_sweep(uchar _p, uint _from, uint _to, uint _l);
// void beep(uchar _p, uint _l);
// void beep(uchar _p);

void beep(uchar _p, uint _f, uint _l){
	// OUTPUT(_p);	
	
	// calc f delay
	ulong d = 1000000UL / (_f);	
	d = (d/2);
	
	// calc ms delay
	ulong m = 1000UL * _l;
	m /= d;

	if(!_l) m = 2;
	// do{
		// _l--;
		
		// 1ms
		for(ulong i=0; i < m; i++){
			ON(_p);
			delay_us(d);
			OFF(_p);
			delay_us(d);
		}
	// }while(_l);
	
	// OFF(_p);
}
void beep_sweep(uchar _p, uint _from, uint _to, uint _l){
	
	while(_from != _to){
		
		beep(_p,_from, 0);
		
		if(_from < _to)
			_from++;
		else
			_from--;
	}
	
}

// Tonfrequenzen
const uint16_t freq[] PROGMEM = {
  //   C,   C#,    D,   D#,    E,    F,   F#,    G,   G#,    A,   A#,    B
	 // 262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494, //Oktave 5
	 // 523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988, //Oktave 6
	// 1046, 1109, 1175, 1244, 1328, 1397, 1480, 1568, 1661, 1760, 1865, 1975, //Oktave 7
	2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951  //Oktave 8
};
#endif