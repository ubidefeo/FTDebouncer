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
FTDebouncer::FTDebouncer(uint16_t _debounceTime) : debounceDelay(_debounceTime) {
	debouncedItemsCount = 0;
	firstDebounceItem = lastDebounceItem = nullptr;	
}
FTDebouncer::~FTDebouncer() {
}

/*	METHODS										*/
void FTDebouncer::addPin(
	uint8_t _pinNr, uint8_t _restState) {
	DebounceItem *dbItem = new DebounceItem();

	dbItem->pinNumber = _pinNr;
	dbItem->restState = _restState;

	if (firstDebounceItem == nullptr) {
		firstDebounceItem = dbItem;
	} else {
		lastDebounceItem->nextItem = dbItem;
	}
	lastDebounceItem = dbItem;

	pinMode(_pinNr, INPUT);
	debouncedItemsCount++;

}
void FTDebouncer::addPin(uint8_t _pinNr, uint8_t _restState, uint8_t _pullUpMode) {
	DebounceItem *dbItem = new DebounceItem();

	dbItem->pinNumber = _pinNr;
	dbItem->restState = _restState;

	if (firstDebounceItem == nullptr) {
		firstDebounceItem = dbItem;
	} else {
		lastDebounceItem->nextItem = dbItem;
	}

	lastDebounceItem = dbItem;

	if (_pullUpMode == 0) {
		pinMode(_pinNr, INPUT);
	} else {
		pinMode(_pinNr, _pullUpMode);
	}

	debouncedItemsCount++;

}

void FTDebouncer::init() {
	unsigned long currentMs = millis();
	DebounceItem *dbItem = firstDebounceItem;

	while (1) {
		dbItem->lastTimeChecked = currentMs;
		dbItem->currentState = dbItem->previousState = dbItem->restState;
		dbItem->currentDebouncedState = dbItem->previousDebouncedState = dbItem->restState;
		dbItem = dbItem->nextItem;
		if (dbItem == nullptr) break;
	}
}

void FTDebouncer::run() {
	DebounceItem *dbItem = firstDebounceItem;
	while (1) {
		dbItem->currentState = digitalRead(dbItem->pinNumber);
		dbItem = dbItem->nextItem;
		if (dbItem == nullptr) break;
	}

	debouncePins();
	checkStateChange();
}
void FTDebouncer::debouncePins() {
	unsigned long currentMs = millis();
	DebounceItem *dbItem = firstDebounceItem;
	while (1) {
		if (dbItem->currentState != dbItem->previousState) {
			dbItem->lastTimeChecked = currentMs;
		}
		if (currentMs - dbItem->lastTimeChecked > debounceDelay) {
			if (dbItem->previousState == dbItem->currentState) {
				dbItem->lastTimeChecked = currentMs;
				dbItem->currentDebouncedState = dbItem->currentState;
			}
		}
		dbItem->previousState = dbItem->currentState;
		dbItem = dbItem->nextItem;
		if (dbItem == nullptr) break;
	}
}

void FTDebouncer::checkStateChange() {
	DebounceItem *dbItem = firstDebounceItem;

	while (1) {
		if (dbItem->previousDebouncedState != dbItem->currentDebouncedState) {
			if (dbItem->currentDebouncedState == !dbItem->restState) {
				pinActivated(dbItem->pinNumber);
			}
			if (dbItem->currentDebouncedState == dbItem->restState) {
				pinDeactivated(dbItem->pinNumber);
			}
		}
		dbItem->previousDebouncedState = dbItem->currentDebouncedState;
		dbItem = dbItem->nextItem;
		if (dbItem == nullptr) break;
	}
}
uint8_t FTDebouncer::getPinsCount(){
	return debouncedItemsCount;
}