/**
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by: sbhklr
* @Last Modified time: 2020-05-01 23:34:07
**/


#include "FTDebouncer.h"

#define DEFAULT_DEBOUNCE_DELAY 40

/*	CONSTRUCTORS/DESTRUCTOR	*/
FTDebouncer::FTDebouncer() : _debounceDelay(DEFAULT_DEBOUNCE_DELAY) {				
}

FTDebouncer::FTDebouncer(uint16_t debounceDelay) : _debounceDelay(debounceDelay) {		
}

FTDebouncer::~FTDebouncer() {
	this->end();
}

/*	METHODS	*/

void FTDebouncer::addPin(uint8_t pinNumber, uint8_t restState, int pullMode) {
	DebounceItem *debounceItem = new DebounceItem();

	debounceItem->pinNumber = pinNumber;
	debounceItem->restState = restState;

	if (_firstDebounceItem == nullptr) {
		_firstDebounceItem = debounceItem;
	} else {
		_lastDebounceItem->nextItem = debounceItem;
	}

	_lastDebounceItem = debounceItem;
	
	pinMode(pinNumber, static_cast<uint8_t>(pullMode));

	++_debouncedItemsCount;
}

void FTDebouncer::setPinEnabled(uint8_t pinNumber, bool enabled){
	for(DebounceItem *debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		if(debounceItem->pinNumber == pinNumber){
			if(enabled) pinMode(debounceItem->pinNumber, debounceItem->pullMode);
			debounceItem->enabled = enabled;
			return;
		}
	}
}
void FTDebouncer::begin(){
	unsigned long currentMilliseconds = millis();

	for(DebounceItem *debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		debounceItem->lastTimeChecked = currentMilliseconds;
		debounceItem->currentState = debounceItem->previousState = debounceItem->restState;
		debounceItem->currentDebouncedState = debounceItem->previousDebouncedState = debounceItem->restState;
		debounceItem->enabled = true;
	}	
}

void FTDebouncer::init() {
	this->begin();
}

void FTDebouncer::run() {
	this->update();
}

void FTDebouncer::update(){
	this->debouncePins();
	this->checkStateChange();	
}

void FTDebouncer::end(){
	for(DebounceItem *debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		delete debounceItem;
	}
}

void FTDebouncer::debouncePins() {
	unsigned long currentMilliseconds = millis();

	for(DebounceItem *debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		debounceItem->currentState = digitalRead(debounceItem->pinNumber);		
		bool stateHasChangedSinceLastCheck = debounceItem->currentState != debounceItem->previousState;
		
		if (stateHasChangedSinceLastCheck) {
			debounceItem->lastTimeChecked = currentMilliseconds;
		}

		bool stateIsStable = currentMilliseconds - debounceItem->lastTimeChecked > _debounceDelay;
		
		if (stateIsStable && !stateHasChangedSinceLastCheck) {
			debounceItem->lastTimeChecked = currentMilliseconds;
			debounceItem->currentDebouncedState = debounceItem->currentState;			
		}

		debounceItem->previousState = debounceItem->currentState;			
	}
}

void FTDebouncer::checkStateChange() {

	for(DebounceItem *debounceItem = _firstDebounceItem; debounceItem != nullptr; debounceItem = debounceItem->nextItem){
		if(!debounceItem->enabled) continue;
		if (debounceItem->previousDebouncedState != debounceItem->currentDebouncedState) {			

			if (debounceItem->currentDebouncedState == debounceItem->restState) {
				onPinDeactivated(static_cast<int>(debounceItem->pinNumber));
			} else {
				onPinActivated(static_cast<int>(debounceItem->pinNumber));				
			}
		}
		debounceItem->previousDebouncedState = debounceItem->currentDebouncedState;		
	}
}

uint8_t FTDebouncer::getPinCount(){
	return _debouncedItemsCount;
}
