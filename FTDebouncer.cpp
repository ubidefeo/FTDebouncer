/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by:   ubi
* @Last Modified time: 2018-03-17 09:09:22
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

void FTDebouncer::addPin(uint8_t pinNr, uint8_t restState, int inputMode) {
	DebounceItem *debounceItem = new DebounceItem();

	debounceItem->pinNumber = pinNr;
	debounceItem->restState = restState;

	if (_firstDebounceItem == nullptr) {
		_firstDebounceItem = debounceItem;
	} else {
		_lastDebounceItem->nextItem = debounceItem;
	}

	_lastDebounceItem = debounceItem;
	
	pinMode(pinNr, static_cast<uint8_t>(inputMode));

	++_debouncedItemsCount;
}

void FTDebouncer::init() {
	unsigned long currentMilliseconds = millis();	

	for(DebounceItem * debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		debounceItem->lastTimeChecked = currentMilliseconds;
		debounceItem->currentState = debounceItem->previousState = debounceItem->restState;
		debounceItem->currentDebouncedState = debounceItem->previousDebouncedState = debounceItem->restState;	    
	}
}

void FTDebouncer::run() {
	for(DebounceItem * debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		debounceItem->currentState = digitalRead(debounceItem->pinNumber);		
	}

	this->debouncePins();
	this->checkStateChange();
}

void FTDebouncer::debouncePins() {
	unsigned long currentMilliseconds = millis();

	for(DebounceItem * debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
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
	}
}

void FTDebouncer::checkStateChange() {

	for(DebounceItem * debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		if (debounceItem->previousDebouncedState != debounceItem->currentDebouncedState) {			

			if (debounceItem->currentDebouncedState == debounceItem->restState) {
				pinDeactivated(debounceItem->pinNumber);
			} else {
				pinActivated(debounceItem->pinNumber);				
			}
		}
		debounceItem->previousDebouncedState = debounceItem->currentDebouncedState;		
	}
}

uint8_t FTDebouncer::getPinCount(){
	return _debouncedItemsCount;
}
