#include "FTDebouncer.h"

FTDebouncer pinDebouncer(30);

void setup(){
	Serial.begin(57600);
	pinDebouncer.addPin(2, HIGH, PinMode::InputPullup);
	pinDebouncer.addPin(3, LOW);
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
	pinDebouncer.init();
	Serial.println(sizeof(pinDebouncer));
	Serial.println(pinDebouncer.getPinCount());
	delay(5000);
}
void loop(){
	unsigned long us = micros();
	unsigned long usElapsed;
	pinDebouncer.run();
	usElapsed = micros() - us;
	Serial.println(usElapsed);
}
void pinActivated(uint8_t pinNr){
	Serial.print("Pin activated: ");
	Serial.println(pinNr);
}
void pinDeactivated(uint8_t pinNr){
	Serial.print("Pin deactivated: ");
	Serial.println(pinNr);
}