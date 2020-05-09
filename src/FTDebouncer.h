/*
* @Author: Ubi de Feo | Future Tailors
* @Date:   2017-07-09 16:34:07
* @Last Modified by:   sbhklr
* @Last Modified time: 2020-05-01 23:34:07
* @Maintainers: Ubi de Feo, Sebastian Hunkeler
*/

/*	Future Tailors' Debouncer Library 											*/
/*	No more manually tracking milliseconds and writing spaghetti monsters		*/

#include "Arduino.h"

/**
 * Callback for when a pin is activated after debouncing.
 * @param pinNumber The pin which was activated.
 * */
extern void onPinActivated(int);

/**
 * Callback for when a pin is deactivated after debouncing.
 * @param pinNumber The pin which was deactivated.
 * */
extern void onPinDeactivated(int);

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

	//The amount of milliseconds during which the debouncing happens
	const uint8_t _debounceDelay;

	//Stores the amount of registered pins for debouncing
	uint8_t _debouncedItemsCount = 0;	

	/**
	 * Handles the state machines for pin debouncing.
	 * */
	void debouncePins();

	/**
	 * Checks the registered pins' for state changes and calls the onPinDeactivated() / onPinActivated()
	 * callbacks if it detects a state change.
	 * */
	void checkStateChange();

public:
	FTDebouncer();

	/**
	 * FTDebouncer's constructor which allows to override the default debounce time.
	 * @param debounceTime The amount of milliseconds for which a pin's logic level needs to be
	 * stable to be considered debounced.
	 * */
	FTDebouncer(uint16_t debounceTime);
	~FTDebouncer();

	/**
	 * Adds a pin to the debouncer. FTDebouncer will take care of setting up the pin by calling pinMode.
	 * 
	 * @param pinNumber The logical pin number on the board which shall be debounced.
	 * @param restState The logical rest state of the pin when the input component is not active 
	 * (e.g. when a connected button is not pressed). This value can be either HIGH or LOW.
	 * @param pullMode The pull mode that shall be used for the connected input. 
	 * Example: if a button is connected and INPUT_PULLUP is used no external resistor needs to be connected as the
	 * board will use an internal resistor. In that case the rest state needs to be set to HIGH as the logical level 
	 * of the input pin will be pulled high by default.
	 * */
	void addPin(uint8_t pinNumber, uint8_t restState, int pullMode = INPUT);

	/**
	 * Toggles the enabled state of one of the registered pins. 
	 * When disabled no state change callbacks will be executed for the specified pin.
	 * */
	void setPinEnabled(uint8_t pinNumber, bool enabled);

	/**
	 * Prepares the debouncing by initializing timestamps and enabling debouncing for all registered pins.
	 * */
	void begin();

	/**
	 * Ends the debouncing deregisters all pins and frees up the allocated memory.
	 * */
	void end();

	/**
	 * Tells the debouncer to debounce the pins' logic level and call the onPinDeactivated() / onPinActivated()
	 * callbacks if it detects a state change for a pin.
	 * */
	void update();

	/**
	 * Returns the amount of registered pins for debouncing.
	 * */
	uint8_t getPinCount();	

	[[deprecated("Replaced by begin, which uses Arduino API compliant naming")]]
	void init();

	[[deprecated("Replaced by update, which uses Arduino API compliant naming")]]
	void run();
};
