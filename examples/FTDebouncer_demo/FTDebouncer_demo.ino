#if !defined(nullptr)
#define nullptr NULL
#endif

#include "FTDebouncer.h"

FTDebouncer pinDebouncer(30);

void setup(){
	Serial.begin(57600);
	pinDebouncer.addPin(2, HIGH, INPUT_PULLUP);
	// NOTE: 
	// INPUT_PULLDOWN is only supported by some microcontrollers
	//pinDebouncer.addPin(3, LOW, INPUT_PULLDOWN); 
	pinDebouncer.addPin(4, HIGH);
	pinDebouncer.addPin(5, LOW);
	pinDebouncer.addPin(6, HIGH);
	pinDebouncer.addPin(7, LOW);
	pinDebouncer.addPin(8, HIGH);
	pinDebouncer.addPin(9, LOW);
	pinDebouncer.addPin(10, HIGH);
	pinDebouncer.addPin(11, LOW);
	pinDebouncer.addPin(12, HIGH);
	pinDebouncer.addPin(13, LOW);
	pinDebouncer.addPin(14, HIGH);
	pinDebouncer.addPin(15, LOW);
	pinDebouncer.addPin(16, HIGH);
	pinDebouncer.addPin(17, LOW);
	pinDebouncer.addPin(18, HIGH);
	pinDebouncer.addPin(19, LOW);
	pinDebouncer.begin();
	Serial.println(sizeof(pinDebouncer));
	Serial.println(pinDebouncer.getPinCount());
	delay(5000);
}
void loop(){
	unsigned long us = micros();
	unsigned long usElapsed;
	pinDebouncer.update();
	usElapsed = micros() - us;
	Serial.println(usElapsed);
}
void onPinActivated(int pinNr){
	Serial.print("Pin activated: ");
	Serial.println(pinNr);
}
void onPinDeactivated(int pinNr){
	Serial.print("Pin deactivated: ");
	Serial.println(pinNr);
}
