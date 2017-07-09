/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by:   ubi
* @Last Modified time: 2017-07-09 16:34:07
*/

/*	Future Tailors' Debouncer Library 											*/
/*	No more manually tracking milliseconds and writing spaghetti monsters		*/

#include "Arduino.h"
extern void pinActivated(uint8_t);
extern void pinDeactivated(uint8_t);

struct DebounceItem {
	uint8_t pinNumber;
	uint8_t restState;
	uint8_t currentState;
	uint8_t previousState;
	uint8_t currentDebouncedState;
	uint8_t previousDebouncedState;
	uint32_t lastTimeChecked;
	DebounceItem *nextItem = nullptr;
};
class FTDebouncer {
private:
	DebounceItem *firstDebounceItem, *lastDebounceItem;
	uint8_t debounceDelay;
	uint8_t debouncedItemsCount;
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
	uint8_t getPinsCount();
};



