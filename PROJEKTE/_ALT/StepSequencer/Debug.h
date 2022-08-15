#ifndef Debug_h
#define Debug_h

#include <Arduino.h>

static String error_string;
static String info_string;

static String boolToString(String s, bool b){ return (String)(s + " " + (b?"[x]":"[ ]"));}
static String boolToString(bool b){return (String)((b?"[x]":"[ ]"));}

static void error(String s){
 	error_string.reserve(32);

	if(!Serial)
		return;
	delay(1);
	if(s == error_string)
		return;

	if(s.length() >= 16)
		s = s.substring(0,15);
	
	error_string = s;
	
	println(error_string);
}
static void error(long s){error(String(s));}

static void info(String s){
	info_string.reserve(64);
	if(!Serial)
		return;
	delay(1);
	if(s == info_string)
		return;

	if(s.length() >= 32)
		s = s.substring(0,31);
	
	info_string = s;
	
	println(info_string);
}
static void info(long s){info(String(s));}

static String intToString(ulong i){
	String result;
	result = (String)i;
	return result;
}

static String byteToString(byte b){
	String result = "";
	for(byte i=0; i<8; i++)
		result += (String)bitRead(b,7-i);
	
	return result;
}

static void printVar(int i, String st){
	if(!st.length())
		info(intToString(i));
	else
		info(st + ": " + intToString(i));
}

#endif
