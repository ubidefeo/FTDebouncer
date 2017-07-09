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

enum class PinMode : uint8_t {
	#ifdef INPUT_PULLUP
	InputPullup = INPUT_PULLUP,
	#endif
	#ifdef INPUT_PULLDOWN
	InputPullDown = INPUT_PULLDOWN,
	#endif
	Input = INPUT,
	Output = OUTPUT,
};

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
	void addPin(uint8_t pinNr, uint8_t restState, PinMode pullUpMode = PinMode::Input);
	void init();
	uint8_t getPinCount();
};
