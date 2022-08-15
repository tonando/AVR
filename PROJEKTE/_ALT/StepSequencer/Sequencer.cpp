#include "Sequencer.h"

uint noteToFreq(int n){
	float a = 1.0594631; // 1.059463094359
	char b = n - 32;
	uint f = 440 * pow(a,b);
	return constrain(f,45,18000);
}

void setupPins(){
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV32);
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(LSBFIRST);
	
	SET_OUTPUT(LED_PIN);
	SET_OUTPUT(TONE_PIN);
	SET_OUTPUT(PWM_PIN);

	SET_OUTPUT(LATCH);
	ON(LATCH);
}

Sequencer::Sequencer(){
	setupPins();
	initSteps();
	init();
}

void Sequencer::init(){
	setBaudrate(BAUD_MIDI); 
	
	setPattern(0);
	setTempo(30);

	base_note = 32;

	following = true;
	playing = true;

	STEP_BUTTON_MODE = STEPS_PIANO; 

	for(byte i=0; i<8; i++)
		leds_pwm[i] = 4;

	ready = true;

	play();
}

void Sequencer::initSteps(){
		// Fill Array
	for(byte p=0; p<NUM_PATTERNS; p++){
		this->patterns[p].length = INTERFACE_SIZE;
	//	this->patterns[p].arp.on = true;
		for(byte s=0; s<MAX_STEPS; s++){
			this->patterns[p].steps[s].setVelocity(15);
			if(s&1 && !p){
				this->patterns[p].steps[s].active = true;
				if(s & 3 == 0)
					this->patterns[p].steps[s].setLength(12);
				else
					this->patterns[p].steps[s].setLength(4);
				this->patterns[p].steps[s].note = constrain(32 + (s&3)*12, 12,100);
			}
		}
	}
}



// Control
void Sequencer::play(){ 
	playing = !playing;
	if(!playing){
#if MIDI_OUT
		midiOut(176,23);	// all notes off
#endif
#if TONE_OUT
		noBitTone();
#endif
#if PWM_OUT
		analogWrite(PWM_PIN,0);
#endif
	}
}
void Sequencer::scroll(char dir){
	this->following = false;
	
	this->scroll_pos += dir;
}
void Sequencer::nextStep(){ 
	noteOff(cPat->current_note);
	
	cursor++;
	if(cursor == cPat->length)
		cursor = 0;
		
	cStep = getStep(cursor);
		
	substep_counter = 0;
	cPat->arp.reset();
}
void Sequencer::nextSubStep(){
	byte n = cStep->note;		

//	GRUNDTON:
//	if(master_scale)
//		n = limitToScale(n, master_scale, getStep(0)->note);
	
	if(cStep->active){
		// end of note
		if(cStep->getLength() == substep_counter){
			OFF(LED_PIN);
			noteOff(n);
		}

		if(substep_counter == 0){ // first
			ON(LED_PIN);
			noteOn(n);					
		}else{
			if(cPat->arp.on && substep_counter % cPat->arp.skip_steps == 0){
				cPat->arp.next();
				noteOn(n + cPat->arp.getOffset());
			}
		}
	}
	
	//updateLeds();
	substep_counter++;
	
	// End of step
	if(substep_counter == STEP_DIVIDER)
		nextStep();
}
// Update
void Sequencer::updateSpi(byte num_bytes){
	byte out = (byte)shift_out_value;
	out = ~(out);
	OFF(LATCH);
	shift_in_value = SPI.transfer(out);
	ON(LATCH);
//	if(num_bytes == 1 || !num_bytes)
//		shift_in_value = SPI.transfer((byte)shift_out_value);	
//	else	
//		shift_in_value = SPI.transfer(shift_out_value);
}
void Sequencer::updateLeds(){

	byte cursor_index = (cursor & 7);	

//	if(show_steps){
	byte offset;
	//if(following)
	offset = cursor-cursor_index;
	led_values = getActiveSteps(current_pattern, offset); 
//	}
	
	if(following){
		byte tmp = led_values;
		led_values <<= cursor_index;
		led_values |= tmp >> cursor_index;
	}

	if(leds_pwm_enabled)
	{
		// fade cursor
		leds_pwm[cursor_index] = 15-substep_counter;
	}
	else
	{
		sbi(led_values,cursor_index);
		if((cStep->active && substep_counter > cStep->getLength()) ^ (substep_counter < 8))
			cbi(led_values,cursor_index);
	}
}
void Sequencer::ledsPwm(){
	byte brightness = (micros() & 1023) >> 6;
	byte out = 0;	

	byte i = 8;		
	do{ // per led
		i--;
		
		if(leds_pwm[i] > brightness)
			sbi(out,i);
	}while(i);
		
	shift_out_value = out;
}

void Sequencer::update(){
	if(!ready)
		return;
		
	if(playing)
		ON(LED_PIN);
	else
		OFF(LED_PIN);

	updateLeds();
	ledsPwm();
	
	checkButtons();
	checkAnalogButtons();

//	if(STEP_BUTTON_MODE >> 4 == 2)
//		getSelectedStep()->edit(STEP_BUTTON_MODE,read_knob(EDIT_KNOB));
	
	if(playing){
		// Next substep
		if(millis() - last_millis >= step_ms){
			setTempo(readKnob(TEMPO_KNOB));
			last_millis = millis();
			nextSubStep();
		}

//			byte t = readKnob(TEMPO_KNOB)>>2;
//			setTempo(t);
			
//			delay(step_ms);

			
			//delay(step_ms);
		//}
	}
	else{
		delay(step_ms>>1);
		fbi(PORTB,LED_PIN & 7);
	}

#if TONE_OUT
	updateTone();
#endif
}


// Settings

void Sequencer::setTempo(byte t){
	t = max(t>>1,1);
	step_ms = t;
	last_millis = millis();
//	pwm_steps = 64;
//	pwm_delay = (step_ms * 1000) >> 6;
}
void Sequencer::setPattern(byte index){
	current_pattern = constrain(index,0,NUM_PATTERNS-1);	
	cPat = &patterns[current_pattern];
	cStep = getStep(cursor);
	getActiveSteps();
}

bool Sequencer::patternUp(){		
	if(current_pattern == NUM_PATTERNS - 1)
		return false;		

	current_pattern++;
	return true;
}
bool Sequencer::patternDown(){
	if(current_pattern == 0)
		return false;
		
	current_pattern--;
	return true;
}
//void Sequencer::editSelectedSteps(byte val){
/*
	byte stps = step_button_values;
	byte i = 7;
	do{
		if(rbi(stps,i)){
			Step *cs = getStep(i);
			switch(STEP){
				case EDIT_NOTE:	cs->setNote(val >> 2); break;	// knobvalue to 1-63 
				case EDIT_LENGTH:	cs->setLength(val>>4); break;	// knob value to 1-15 (half byte)
				case EDIT_VELOCITY: cs->setVelocity(val>>4); break;
				case EDIT_PORTAMENTO: cs->setPortamento(val>>4); break;
				//case EDIT_PITCHBEND: cs->setPitchbend(val>>4); break;
				default: break;
			}
		}
		i--;
	}while(i);
*/	
//}

// Output
void Sequencer::serialOut(){
#if DEBUG
	info("Note: " + (String)cStep->note);
	info("Length: " + (String)(cStep->getLength() << 3));
	info("Velocity: " + (String)(cStep->getVelocity() << 3));
#endif
}
void Sequencer::pwmOut(){
#if PWM_OUT
	byte b = cStep->note << 2;
	analogWrite(PWM_PIN, b);
#endif
}
void Sequencer::toneOut(byte freq){
#if TONE_OUT
	playBitTone(freq);
#endif
}
void Sequencer::midiOut(byte cmd, byte A, byte B){
#if MIDI_OUT
	Serial.write(cmd);
  	Serial.write(A);
  	if(B < 255)
  		Serial.write(B);
 #endif
}
void Sequencer::midiOut(byte cmd, byte A){	midiOut(cmd,A,255); }

//void Sequencer::noteOn(byte n){noteOn(n,15);}
void Sequencer::noteOn(byte n){

	n = min(n,63);
		
	// turn off last note
	if(cPat->current_note < 64)
		noteOff(cPat->current_note);

	cPat->current_note = n;
	
#if TONE_OUT
		uint freq = noteToFreq(n);
		toneOut(freq);
#endif
#if PWM_OUT
		pwmOut();	
#endif		

#if MIDI_OUT
		midiOut(0x90,n + FIRST_NOTE,(cStep->getVelocity() << 3)); 
#endif

#if DEBUG
		//serialOut();
#endif
}
void Sequencer::noteOff(byte n){
	if(!cPat->current_note)
		return;

	cPat->current_note = 255;
#if MIDI_OUT	
  	midiOut(0x80,n,0);
#endif	
#if PWM_OUT
	OFF(PWM_PIN);	
#endif	
#if TONE_OUT
	noBitTone();
#endif	
}

byte Sequencer::getActiveSteps(){
	byte offset = cursor & (INTERFACE_SIZE-1);
	offset = (cursor - offset);
	if(offset)
		offset /= INTERFACE_SIZE;
	//active_steps = getActiveSteps(current_pattern, offset);
	return getActiveSteps(current_pattern, offset);
}
byte Sequencer::getActiveSteps(byte pat, byte offset){
	byte i = INTERFACE_SIZE; 
	byte steps;
	
	do{
		i--;
		byte cs = i + offset;
		if(patterns[pat].steps[cs].active){
			sbi(steps,cs);
			leds_pwm[cs] = 4;
		}
		else{
			cbi(steps,cs);
			leds_pwm[cs] = 0;
		}
	}while(i);
	
	return steps;
}

//	Events
byte Sequencer::readKnob(byte index){
	byte result;
	result = getAnalogValue(index) >> 2;
	knob_values[index] = result;
	return result;
}

void Sequencer::checkButtons(){
	// Shift in step buttons
	byte inVal = shift_in_value;	

	// detect button change
	if(inVal != step_button_values){
		byte i = 8;
		do{
			i--;
			byte newbit= rbi(inVal,i);
			// changed
			if(newbit != rbi(step_button_values,i)){	
				if(newbit)
					stepPressed(i);
				else
					stepReleased(i);
			}
			// holding
			else if(newbit)	
				button_timers[i]++;

		}while(i);
		step_button_values = inVal;
	}
}
void Sequencer::checkAnalogButtons(){
	byte ain = getAnalogButton();
	if(ain == last_analog_button)
		return;
		
	switch(ain){
		case AB_PLAY: play(); break; 
		case AB_PATTERN_UP: patternUp(); break; 
		case AB_PATTERN_DOWN: patternDown(); break; 
		case AB_PATTERN_MULTIPLY: cPat->multiply(); break; 
		case AB_PATTERN_DIVIDE: cPat->divide(); break; 
		case AB_PATTERN_SOLO: break; 
		case AB_PATTERN_TOGGLE: break; 
		default: break;
	}
}
void Sequencer::stepPressed(byte pin){
	uint s = pin;
	
	if(cursor > INTERFACE_SIZE)
		s += (cursor - ( cursor & (INTERFACE_SIZE-1)));
		
	switch(STEP_BUTTON_MODE & 0xF0){
		case STEPS_TOGGLE: getStep(s)->toggle(STEP_BUTTON_MODE); break;
		case STEPS_EDIT: break;
		case STEPS_PIANO: noteOn(base_note + pin); break;
		case STEPS_LOOP: break;
	}
}
void Sequencer::stepReleased(byte pin){
	button_timers[pin] = 0;

	uint s = pin;
	if(cursor > INTERFACE_SIZE)
		s += (cursor - ( cursor & (INTERFACE_SIZE-1)));
		
	switch(STEP_BUTTON_MODE & 0xF0){
		case STEPS_TOGGLE: break;
		case STEPS_EDIT: break;
		case STEPS_PIANO: noteOff(this->base_note + pin); break;
		case STEPS_LOOP: break;
	}
}
void Sequencer::setBaudrate(ulong b){
	Serial.end();
	delay(1);
	Serial.begin(b); 
}

//////////////////\
// Pattern

bool Pattern::multiply(){
	// + copy falls noch nicht verl'ngert?
	
	//byte length = cPat->length;
	if(length == MAX_STEPS) return false;
	length = length << 1;	
	
	return true;
}
bool Pattern::divide(){
	if(length == 1) return false;
	length = length >> 1;		
	
	return true;
}

//////////////////\
// Step

void Step::toggle(byte flag){
	flag &= 0x0f;
//	this->flags 
}
void Step::edit(byte what,byte val){
	
}

/////////////////////////
//	Arpeggiator

Arp::Arp(){
	gate = 50; // %	
	step_size = 1; // intervall
	skip_steps = 2; // = 1/8
}			
void Arp::setDivider(byte d){
	d = constrain(d,1,STEP_DIVIDER);	// max. step_div ?
	this->skip_steps = STEP_DIVIDER / d;
}
int8_t Arp::getOffset(){
	if(!this->on)
		return 0;
		
	this->current_step /= skip_steps;

	int8_t result = 0;
	int8_t i = this->current_step;
	
	i *= step_size; 
	if(down)
		i = -i;
	// + ping pong code
	
	//result = stepsFromScale(i, *aScale);
	result = i;
	
	return result;
}

//// Debug
#if DEBUG
void Sequencer::printPattern(){
		info("Pattern: " + intToString(current_pattern));
		info("Length: " + intToString(cPat->length));		
}
void Sequencer::printAll(){
	info(boolToString("Playing",playing));
	info(boolToString("Rec", recording));
	info(boolToString("Arp", cPat->arp.on));
	//printBool(cPat->shuffle,"Sh");

	info("step_ms" + intToString(step_ms));
//	print(" abs:");
//	println(abs_ms);

	printPattern();
	printInput();
/*	
		println();
		printBool(playing,"PLAY");
		printBool(recording,"REC");
		printVar(tempo,"BPM");
		printVar(step_ms,"ms");
		printVar(current_pattern,"Pat");
		printVar(cursor,"Step");
		println();
		printBool(arp.on,"ARP");
		printVar(arp.step_size,"Notes"); // note
		printVar(arp.num_steps,"Steps"); // loop length
		printVar(arp.skip_steps,"Div"); // divider
		printBool(arp.up,"UP");
		printBool(arp.ping_pong,"PONG");
		if(arp.aScale)
			printBits(arp.aScale,"Scale");	
		println();
*/
}
void Sequencer::printStep(byte index){
	Step& cs = patterns[current_pattern].steps[cursor];

	String out;
	out += boolToString("active",cs.active);
	out += intToString(cs.note);
	out += noteToString(cs.note);
	out += "Length " + intToString(cs.getLength());

	println(out);		
}
void Sequencer::printNotes(){
	for(byte i=0; i!=64; i++){
		print(noteToString(i));
		print(" = ");
		println(noteToFreq(i));
	}
}
void Sequencer::printInput(){
	//byte b = analogRead(A0) >> 2;
	info("A0:" + intToString(readKnob(8)));	
	info("steps: " + byteToString(step_button_values));	
	info("leds: " + byteToString(led_values));
}
#endif
///////////

//	Utils
uint noteToFreq(byte n){
	return pgm_read_word(freqvals + n);
}
byte freqToNote(uint f){
	byte i = 64;
	byte closest;
	do{
		i--;
		uint freq = pgm_read_word(freqvals + i);
		if(freq == f)
			return i;
		if(freq < f)
			closest = i;
	}while(i);
	return closest;
}
#if DEBUG
String noteToString(byte n){
	String s = "Note ";
	char letters[13] = "CcDdEFfGgAaB"; // falsch
	int8_t note = n - 23;
	n = abs(note);
	byte oct = n / 12;
	
	if(note < 0)
		oct = 4 - oct;
	if(note > 0)
		oct = 4 + oct;
	
	char name = letters[n % 12];
	s += name;
	if(islower(name))
		s += "#";
	s += oct;
	return s;
}
#endif
byte limitToScale(byte note, byte scale_index, byte root){
  while(!inScale(note,scale_index,root)){
    note++;
    if(note == 64)
      note = 0;
  }
  return note;
}
bool inScale(byte note, byte scale_index, byte root){
  byte i=7;
  root %= 12;
  note %= 12;
  do{
  	i--;
    if(note == root)
       return true; 
    root = (root + ((SCALES[scale_index]>>i) & 1) + 1) % 12;
  }while(i);
  return false;
}

byte stepsFromScale(int8_t st, byte sc){ 
	if(st == 0)
		return 0;
	
	byte result = 0;
	byte scale = SCALES[sc];
	
	do{
		bool bit;
		if(st > 7)
			bit = 7-(st % 7);
		else
			bit = 7-st;
			
		if( rbi(sc, bit) )
			result += 2;
		else
			result ++;
			
		st--;
	}while(st);
	
	return result;
}

uint getAnalogButton(){
	
	byte result = getAnalogValue(ANALOG_BUTTONS) >> 4;
	
//	for(result=1; result < sizeof(AVALS); result++){
//		if(a >= AVALS[result]-ANALOG_TOLERANCE)
//			break;	
//	}
	
	return result;
}

uint getAnalogValue(byte ch){
	ADMUX = (1 << REFS0) | ch;
	
	ADCSRA = (6 << ADPS0) | (1 << ADEN);
	while (bit_is_set(ADCSRA, ADSC));
	return (ADCL | (ADCH << 8));
}
