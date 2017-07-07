/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-07 21:40:49
* @Last Modified by:   ubi
* @Last Modified time: 2017-07-07 21:41:08
*/


#include "FTDebouncer.h"
/*	CONSTRUCTORS/DESTRUCTOR						*/
FTDebouncer::FTDebouncer() {
	//pinsCount = 0;
	firstDebounceItem = lastDebounceItem = NULL;
	debounceDelay = 40;
}
FTDebouncer::FTDebouncer(uint16_t _debounceTime) {
	//pinsCount = 0;
	firstDebounceItem = lastDebounceItem = NULL;
	debounceDelay = _debounceTime;
}
FTDebouncer::~FTDebouncer() {
}


void FTDebouncer::addPin(
	uint8_t _pinNr, uint8_t _restState) {
	//Serial.println("add pin");
	debounceItem *dbItem = new debounceItem();

	dbItem->pinNumber = _pinNr;
	dbItem->restState = _restState;

	if (firstDebounceItem == NULL) {
		firstDebounceItem = dbItem;
	} else {
		lastDebounceItem->nextItem = dbItem;
	}

	lastDebounceItem = dbItem;

	pinMode(_pinNr, INPUT);


}
void FTDebouncer::addPin(uint8_t _pinNr, uint8_t _restState, uint8_t _pullUpMode) {
	//Serial.println("add pin");
	debounceItem *dbItem = new debounceItem();

	dbItem->pinNumber = _pinNr;
	dbItem->restState = _restState;

	if (firstDebounceItem == NULL) {
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


}

void FTDebouncer::init() {
	unsigned long currentMs = millis();
	debounceItem *dbItem = firstDebounceItem;

	while (1) {
		dbItem->lastTimeChecked = currentMs;
		dbItem->currentState = dbItem->previousState = dbItem->restState;
		dbItem->currentDebouncedState = dbItem->previousDebouncedState = dbItem->restState;
		dbItem = dbItem->nextItem;
		if (dbItem == NULL) break;
	}
}

void FTDebouncer::run() {
	debounceItem *dbItem = firstDebounceItem;
	while (1) {
		dbItem->currentState = digitalRead(dbItem->pinNumber);
		dbItem = dbItem->nextItem;
		if (dbItem == NULL) break;
	}

	debouncePins();
	checkStateChange();
}
void FTDebouncer::debouncePins() {
	unsigned long currentMs = millis();
	debounceItem *dbItem = firstDebounceItem;
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
		if (dbItem == NULL) break;
	}
}
void FTDebouncer::checkStateChange() {
	debounceItem *dbItem = firstDebounceItem;

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
		if (dbItem == NULL) break;
	}
}
