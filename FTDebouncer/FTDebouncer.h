/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-05 07:12:01
* @Last Modified by:   ubi
* @Last Modified time: 2017-07-05 07:13:20
*/

/*	Future Tailors' Debouncer Class 											*/
/*	No more manually tracking milliseconds and writing spaghetti monsters		*/

/* 	USAGE


	

	- 	pins to debounce must be pulled-up/down through a 10KOhm resistor to VCC or GND or using internal pull-up/down (see notes in example code)
		The HIGH or LOW conditions established by these resistors will be the "rest state" of your pin
	
	
	- 	include class at the beginning of your arduino sketch
		#include "FTDebouncer.h"
		or
		#include <FTDebouncer.h> // depends on location of the class

	
	- 	instantiate a Debouncer object
		FTDebouncer pinDebouncer;

	
	- 	in your setup() function initiate each pin you want to debounce with
		pinDebouncer.addPin(<pin number>, <rest state>);
	
	-	at the end of your setup() function always initialise the object with 
		pinDebouncer.init();

	example: 
	void setup(){}
		pinDebouncer.addPin(9, LOW); // external pull-down resistor
		pinDebouncer.addPin(5, HIGH); // external pull-up resistor
		pinDebouncer.addPin(3, LOW); // externall pull-down resistor
		// although I favour externall pull-up/down resistor, some like to use internal ones when the cpu allows it.
		// most architectures support INPUT_PULLUP, some implement INPUT_PULLDOWN.
		// If your compiler throws an "undefined" then it's not supported.
		pinDebouncer.addPin(7, HIGH, INPUT_PULLUP); // internal pull-up resistor (architecture dependent)
		pinDebouncer.addPin(8, HIGH, INPUT_PULLDOWN); // internal pull-up resistor (architecture dependent)
		pinDebouncer.init();
	}

	- 	in your loop() function call this method at the beginning. Mind that any delay() will negatively affect the performance
		pinDebouncer.run();

		this will take care of doing the debouncing for you and only calling the pinActivated() and pinDeactivated() functions when the pin state change is stable

	
	- 	implement the following functions in your basic Arduino sketch (copy paste these at the end of your code)

	void pinActivated(uint8_t _pinNR){
		// do something according to the _pinNR that is triggered
		// for insance:
		// Serial.println(_pinNR);
	}
	void pinDeactivated(uint8_t _pinNR){
		// do something according to the _pinNR that is triggered
		// for insance:
		// Serial.println(_pinNR);
	}

	
	This class can easily be reworked to function outside of the Arduino framework if you know how to handle time and Data Directin Registers, HAL etc.
	
*/
#include "Arduino.h"
extern void pinActivated(uint8_t);
extern void pinDeactivated(uint8_t);

struct debounceItem {
	uint8_t pinNumber;
	uint8_t restState;
	uint8_t currentState;
	uint8_t previousState;
	uint8_t currentDebouncedState;
	uint8_t previousDebouncedState;
	uint32_t lastTimeChecked;
	debounceItem *nextItem = NULL;
};
class FTDebouncer {
private:
	debounceItem *firstDebounceItem, *lastDebounceItem;
	uint8_t debounceDelay;
	void readPins();
	void debouncePins();
	void checkStateChange();

public:
	FTDebouncer();
	FTDebouncer(uint16_t _debounceTime);
	~FTDebouncer();
	void run();
	void addPin(uint8_t _pinNr, uint8_t _restState);
	void addPin(uint8_t _pinNr, uint8_t _restState, uint8_t _pullUpMode);
	void init();
};



