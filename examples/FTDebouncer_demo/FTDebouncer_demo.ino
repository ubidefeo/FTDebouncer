#if !defined(nullptr)
#define nullptr NULL
#endif

#include "FTDebouncer.h"

FTDebouncer pinDebouncer(30);

void setup(){
	Serial.begin(57600);	
	pinDebouncer.addPin(2, HIGH, INPUT, onPinActivated, onPinDeactivated);
	pinDebouncer.addPin(3, LOW, INPUT, onPinActivated, onPinDeactivated);
	pinDebouncer.addPin(4, HIGH, INPUT, onPinActivated, onPinDeactivated);
	pinDebouncer.addPin(5, LOW, INPUT, onPinActivated, onPinDeactivated);
	pinDebouncer.addPin(6, HIGH, INPUT_PULLUP, onPinActivated, onPinDeactivated);

	// NOTE: INPUT_PULLDOWN is only supported by some microcontrollers
	//pinDebouncer.addPin(7, LOW, INPUT_PULLDOWN, onPinActivated, onPinDeactivated); 

	//Use of anonymous inline callback function
	pinDebouncer.addPin(8, HIGH, INPUT_PULLUP, [](int pinNumber) {
		Serial.print("Pin activated: ");
		Serial.println(pinNumber);
	}, []() {
		Serial.print("Pin deactivated: ");
		Serial.println(pinNumber);
	});

	pinDebouncer.begin();
	Serial.print("Size of debouncer instance in bytes: ");	
	Serial.println(sizeof(pinDebouncer));
	Serial.print("Amount of debounced pins: ");
	Serial.println(pinDebouncer.getPinCount());
	delay(5000);
}
void loop(){
	unsigned long us = micros();
	unsigned long usElapsed;

	pinDebouncer.update();

	usElapsed = micros() - us;
	Serial.print("Elapsed microseconds since last debounce: ");
	Serial.println(usElapsed);
}


/**
 * Callback for when a pin is activated after debouncing.
 * @param pinNumber The pin which was activated.
 * */
void onPinActivated(int pinNumber){
	Serial.print("Pin activated: ");
	Serial.println(pinNumber);
}

/**
 * Callback for when a pin is deactivated after debouncing.
 * @param pinNumber The pin which was deactivated.
 * */
void onPinDeactivated(int pinNumber){
	Serial.print("Pin deactivated: ");
	Serial.println(pinNumber);
}
