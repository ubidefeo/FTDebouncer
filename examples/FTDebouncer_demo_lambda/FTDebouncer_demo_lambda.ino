#if !defined(nullptr)
#define nullptr NULL
#endif

#include "FTDebouncer-Lambda.h"

FTDebouncer pinDebouncer(30);

void setup(){
	Serial.begin(57600);
	pinDebouncer.addPin(2, HIGH, INPUT_PULLUP, []() {
		Serial.print("Pin activated: ");
		Serial.println(2);
	}, []() {
		Serial.print("Pin deactivated: ");
		Serial.println(2);
	});
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
