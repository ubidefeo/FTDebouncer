/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by:   ubi
* @Last Modified time: 2017-07-09 16:34:07
* @Maintainers: Ubi de Feo, Sebastian Hunkeler
*/

/*	Future Tailors' Debouncer Library 											*/
/*	No more manually tracking milliseconds and writing spaghetti monsters		*/

#include "Arduino.h"

extern void onPinActivated(uint8_t);
extern void onPinDeactivated(uint8_t);

struct DebounceItem {
	uint8_t pinNumber;
	uint8_t restState;
	uint8_t currentState;
	uint8_t previousState;
	uint8_t currentDebouncedState;
	uint8_t previousDebouncedState;
	uint32_t lastTimeChecked;
	bool enabled;
	uint8_t pullMode;
	DebounceItem *nextItem = nullptr;
};

class FTDebouncer {

private:
	DebounceItem *_firstDebounceItem = nullptr, *_lastDebounceItem = nullptr;
	const uint8_t _debounceDelay;
	uint8_t _debouncedItemsCount = 0;
	void readPins();
	void debouncePins();
	void checkStateChange();

public:
	FTDebouncer();
	FTDebouncer(uint16_t debounceTime);
	~FTDebouncer();
	void run();	
	void update();
	void addPin(uint8_t pinNr, uint8_t restState, int pullMode = INPUT);
	void setPinEnabled(uint8_t pinNr, bool enabled);
	void init();
	void begin();
	uint8_t getPinCount();
};
