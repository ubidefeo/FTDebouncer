# Future Tailors' Debouncer [FTDebouncer]
### An efficient, low footprint pin debounce library for Arduino

Debouncing in software can be tedious and is often cause of confusion in beginners.
Advanced users might have an easier life with tracking multiple variables storing milliseconds per each button,
but it can easily get out of hand.
A while back I wrote a routine that facilitated all this using several Arrays but also configuring that could turn into a mess even for me,
so I set to re-write it as a library and make it efficient, dynamic and lightweight.

Have a go at it and let me know what you think.

---
#### Installation
At the moment installation has to be done manually either on a global level (*libraries* folder in the main Arduino Sketch folder) or locally placed in the *libraries* folder inside your Sketch directory.
I'll soon make a Library JSON file to add to the IDE and submit it for review as an endorsed library which can be found via Library Manager.

For the time being when including it you'll have to go with
```#include "FTDebouncer.h"```

#### Usage
Pins to debounce must be pulled-up/down via a 10KOhm resistor to VCC or GND, or enabling internal pull-up/down (see notes in the example code).
The HIGH or LOW state established by these resistors will be the "rest state" of your FTDebouncer's pins.
	
	
* Include library at the beginning of your Arduino sketch
```#include "FTDebouncer.h"```
or
```#include <FTDebouncer.h>``` (depends on location of the class)

* Instantiate an FTDebouncer object
```
FTDebouncer pinDebouncer;
```

This will initialise the debounce time to 40ms by default, but a different interval can be chosen by passing the time in ms to the constructor as in
```
FTDebouncer pinDebouncer(30);
```

* In your ```setup()``` function initiate each pin you want to debounce with
```pinDebouncer.addPin(<pin number>, <rest state>);``` (more detailed examples below).
	
* After all the pins have been added in the ```setup()``` function always initialise the object with
```pinDebouncer.init();```

__example:__
```c++
void setup(){}
	pinDebouncer.addPin(9, LOW); // external pull-down resistor
	pinDebouncer.addPin(5, HIGH); // external pull-up resistor
	pinDebouncer.addPin(3, LOW); // external pull-down resistor
	// although I favour external pull-up/down resistor, some like to use internal ones when the cpu allows it.
	// most architectures support INPUT_PULLUP, some implement INPUT_PULLDOWN.
	// If your compiler throws an "undefined" then it's not supported.
	pinDebouncer.addPin(7, HIGH, INPUT_PULLUP); // internal pull-up resistor (architecture dependent)
	pinDebouncer.addPin(8, HIGH, INPUT_PULLDOWN); // internal pull-down resistor (architecture dependent)
	pinDebouncer.init();
}
```

* At the beginning of your ```loop()``` function call this the library's ```update()``` method (mind that any ```delay()``` in your ```loop()``` will interfere with proper debouncing and state handling).
```pinDebouncer.update();```

* This will take care of doing the debouncing for you and simply call the ```onPinActivated()``` and ```onPinDeactivated()``` functions when the pin state change is steady.

* Implement the following functions in your basic Arduino sketch (copy/paste these at the end of your code).

```c++
void onPinActivated(int pinNR){
	// do something according to the _pinNR that is triggered
	// for instance:
	// Serial.println(pinNR);
}
void onPinDeactivated(int pinNR){
	// do something according to the _pinNR that is triggered
	// for instance:
	// Serial.println(pinNR);
}
```
	
__note__: This class can easily be reworked to function outside of the Arduino framework if you know how to handle time, Data Direction Registers, HAL etc.
	
