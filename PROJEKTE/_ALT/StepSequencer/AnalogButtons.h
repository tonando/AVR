#ifndef AnalogButtons_h
#define AnalogButtons_h

#define ANALOG_TOLERANCE 5

#define AB_PLAY 0
#define AB_PATTERN_UP 1
#define AB_PATTERN_DOWN 2
#define AB_PATTERN_MULTIPLY 3
#define AB_PATTERN_DIVIDE 4
#define AB_PATTERN_SOLO 5
#define AB_PATTERN_TOGGLE 6

#define AB_EDIT_NOTE 7
#define AB_EDIT_VELOCITY 8
#define AB_EDIT_LENGTH 9
#define AB_EDIT_PORTAMENTO 10

#define AB_EDIT_TEMPO 11
#define AB_EDIT_TRANSPOSE 12
#define AB_EDIT_SCALE 13

#define AB_LEFT 14
#define AB_RIGHT 15

const int AVALS[16] = {64,128,192,256,320,384,448,512,575,639,703,767,831,895,959,1023};

#endif