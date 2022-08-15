#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Macros.h>

#define X_AXIS 0
#define Y_AXIS 1

static signed int x_axis = 0;
static signed int y_axis = 0;
static char ach_x=-1;
static char ach_y=-1;
static uchar center_x;
static uchar center_y;
static uchar aval_x;
static uchar aval_y;

void joystick_center();
void joystick_init(uchar _x,uchar _y);
void joystick_update();
int joystick_read(uchar _axis);
char joystick_getDir(uchar _axis);

void joystick_center(){
	if(ach_x == -1 || ach_y == -1)
		return;
	
	center_x = joystick_read(X_AXIS);
	center_y = joystick_read(Y_AXIS);
}
void joystick_init(uchar _x,uchar _y){
	if(_x > 5 || _y > 5)
		return;
	
	ach_x = _x;
	ach_y = _y;
	
	INPUT(ATOP(_x));
	INPUT(ATOP(_y));
	
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);
}

void joystick_update(){
	aval_x = joystick_read(X_AXIS);
	aval_y = joystick_read(Y_AXIS);
	
	x_axis = aval_x - center_x;
	y_axis = aval_y - center_y;
}

int joystick_read(uchar _axis){
	
	ADMUX = _axis ? ach_y : ach_x;
	
	//Start conversion
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 

	int result = ADCL;
	result &= 0xff;
	result |= (ADCH & 0xff) << 8;
	
	return result;
}

char joystick_getDir(uchar _axis){
	int ax = _axis ? y_axis : x_axis;
	if(ax < 0)
		return -1;
	if(ax > 0)
		return 1;
	
	return 0;
}


#endif
