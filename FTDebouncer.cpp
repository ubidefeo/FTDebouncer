/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by:   ubi
* @Last Modified time: 2017-07-09 16:41:17
*/


#include "FTDebouncer.h"

/*	CONSTRUCTORS/DESTRUCTOR						*/
FTDebouncer::FTDebouncer() : _debounceDelay(40) {				
}

FTDebouncer::FTDebouncer(uint16_t debounceDelay) : _debounceDelay(debounceDelay) {		
}

FTDebouncer::~FTDebouncer() {
}

/*	METHODS										*/

void FTDebouncer::addPin(uint8_t pinNr, uint8_t restState, PinMode pullUpMode) {
	DebounceItem *debounceItem = new DebounceItem();

	debounceItem->pinNumber = pinNr;
	debounceItem->restState = restState;

	if (_firstDebounceItem == nullptr) {
		_firstDebounceItem = debounceItem;
	} else {
		_lastDebounceItem->nextItem = debounceItem;
	}

	_lastDebounceItem = debounceItem;
	
	pinMode(pinNr, static_cast<uint8_t>(pullUpMode));

	++_debouncedItemsCount;
}

void FTDebouncer::init() {
	unsigned long currentMilliseconds = millis();
	DebounceItem *debounceItem = _firstDebounceItem;

	while (debounceItem != nullptr) {
		debounceItem->lastTimeChecked = currentMilliseconds;
		debounceItem->currentState = debounceItem->previousState = debounceItem->restState;
		debounceItem->currentDebouncedState = debounceItem->previousDebouncedState = debounceItem->restState;
		debounceItem = debounceItem->nextItem;		
	}
}

void FTDebouncer::run() {
	DebounceItem *debounceItem = _firstDebounceItem;
	while (debounceItem != nullptr) {
		debounceItem->currentState = digitalRead(debounceItem->pinNumber);
		debounceItem = debounceItem->nextItem;		
	}

	this->debouncePins();
	this->checkStateChange();
}

void FTDebouncer::debouncePins() {
	unsigned long currentMilliseconds = millis();
	DebounceItem *debounceItem = _firstDebounceItem;
	while (debounceItem != nullptr) {
		if (debounceItem->currentState != debounceItem->previousState) {
			debounceItem->lastTimeChecked = currentMilliseconds;
		}
		if (currentMilliseconds - debounceItem->lastTimeChecked > _debounceDelay) {
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
	DebounceItem *debounceItem = _firstDebounceItem;

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
	return _debouncedItemsCount;
}
