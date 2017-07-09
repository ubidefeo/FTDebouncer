#include "FTDebouncer.h"
FTDebouncer pinDebouncer(30);
void setup(){
	Serial.begin(57600);
	pinDebouncer.addPin(2, HIGH, INPUT_PULLUP);
	pinDebouncer.addPin(4, LOW);
	pinDebouncer.addPin(5, HIGH);
	pinDebouncer.init();
	
}
void loop(){
	pinDebouncer.run();
}
void pinActivated(uint8_t _pinNr){
	Serial.print("Pin activated: ");
	Serial.println(_pinNr);
}
void pinDeactivated(uint8_t _pinNr){
	Serial.print("Pin deactivated: ");
	Serial.println(_pinNr);
}