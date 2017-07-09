/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by:   ubi
* @Last Modified time: 2017-07-09 16:41:17
*/


#include "FTDebouncer.h"
/*	CONSTRUCTORS/DESTRUCTOR						*/
FTDebouncer::FTDebouncer() : debounceDelay(40) {
	debouncedItemsCount = 0;
	firstDebounceItem = lastDebounceItem = nullptr;	
}
FTDebouncer::FTDebouncer(uint16_t _debounceDelay) : debounceDelay(_debounceDelay) {
	debouncedItemsCount = 0;
	firstDebounceItem = lastDebounceItem = nullptr;	
}
FTDebouncer::~FTDebouncer() {
}

/*	METHODS										*/
void FTDebouncer::addPin(uint8_t _pinNr, uint8_t _restState) {
	this->addPin(_pinNr, _restState, PinMode::Input);
}
void FTDebouncer::addPin(uint8_t _pinNr, uint8_t _restState, PinMode _pullUpMode) {
	DebounceItem *debounceItem = new DebounceItem();

	debounceItem->pinNumber = _pinNr;
	debounceItem->restState = _restState;

	if (firstDebounceItem == nullptr) {
		firstDebounceItem = debounceItem;
	} else {
		lastDebounceItem->nextItem = debounceItem;
	}

	lastDebounceItem = debounceItem;
	
	pinMode(_pinNr, static_cast<uint8_t>(_pullUpMode));

	debouncedItemsCount++;

}

void FTDebouncer::init() {
	unsigned long currentMilliseconds = millis();
	DebounceItem *debounceItem = firstDebounceItem;

	while (debounceItem != nullptr) {
		debounceItem->lastTimeChecked = currentMilliseconds;
		debounceItem->currentState = debounceItem->previousState = debounceItem->restState;
		debounceItem->currentDebouncedState = debounceItem->previousDebouncedState = debounceItem->restState;
		debounceItem = debounceItem->nextItem;		
	}
}

void FTDebouncer::run() {
	DebounceItem *debounceItem = firstDebounceItem;
	while (debounceItem != nullptr) {
		debounceItem->currentState = digitalRead(debounceItem->pinNumber);
		debounceItem = debounceItem->nextItem;		
	}

	this->debouncePins();
	this->checkStateChange();
}
void FTDebouncer::debouncePins() {
	unsigned long currentMilliseconds = millis();
	DebounceItem *debounceItem = firstDebounceItem;
	while (debounceItem != nullptr) {
		if (debounceItem->currentState != debounceItem->previousState) {
			debounceItem->lastTimeChecked = currentMilliseconds;
		}
		if (currentMilliseconds - debounceItem->lastTimeChecked > debounceDelay) {
			if (debounceItem->previousState == debounceItem->currentState) {
				debounceItem->lastTimeChecked = currentMilliseconds;
				debounceItem->currentDebouncedState = debounceItem->currentState;
			}
		}
		debounceItem->previousState = debounceItem->currentState;
		debounceItem = debounceItem->nextItem;		
	}
}

void FTDebouncer::checkStateChange() {
	DebounceItem *debounceItem = firstDebounceItem;

	while (debounceItem != nullptr) {
		if (debounceItem->previousDebouncedState != debounceItem->currentDebouncedState) {
			if (debounceItem->currentDebouncedState == !debounceItem->restState) {
				pinActivated(debounceItem->pinNumber);
			}
			if (debounceItem->currentDebouncedState == debounceItem->restState) {
				pinDeactivated(debounceItem->pinNumber);
			}
		}
		debounceItem->previousDebouncedState = debounceItem->currentDebouncedState;
		debounceItem = debounceItem->nextItem;		
	}
}
uint8_t FTDebouncer::getPinCount(){
	return debouncedItemsCount;
}