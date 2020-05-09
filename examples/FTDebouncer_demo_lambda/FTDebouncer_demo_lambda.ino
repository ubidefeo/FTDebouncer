#if !defined(nullptr)
#define nullptr NULL
#endif

#include "FTDebouncer-Lambda.h"

FTDebouncer pinDebouncer;

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
	delay(1000);
}
void loop(){	
	pinDebouncer.update();	
}
