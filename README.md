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
	
	
* Include the library at the beginning of your Arduino sketch
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
	
* **After** all the pins have been added in the ```setup()``` function always initialise the object with
```pinDebouncer.begin();```

**Example:**
```c++
void setup(){}
	pinDebouncer.addPin(9, LOW, onPinActivated, onPinDeactivated); // external pull-down resistor
	pinDebouncer.addPin(5, HIGH, onPinActivated, onPinDeactivated); // external pull-up resistor
	pinDebouncer.addPin(3, LOW, onPinActivated, onPinDeactivated); // external pull-down resistor
	// although I favour external pull-up/down resistor, some like to use internal ones when the cpu allows it.
	// most architectures support INPUT_PULLUP, some implement INPUT_PULLDOWN.
	// If your compiler throws an "undefined" then it's not supported.
	pinDebouncer.addPin(7, HIGH, onPinActivated, onPinDeactivated, INPUT_PULLUP); // internal pull-up resistor (architecture dependent)
	pinDebouncer.addPin(8, HIGH, onPinActivated, onPinDeactivated, INPUT_PULLDOWN); // internal pull-down resistor (architecture dependent)
	pinDebouncer.begin();
}
```

* At the beginning of your ```loop()``` function call this the library's ```update()``` method (mind that any ```delay()``` in your ```loop()``` will interfere with proper debouncing and state handling):
```pinDebouncer.update();```

* This will take care of doing the debouncing for you and simply call the callback functions when the pin state change is steady. In the above example ```onPinActivated()``` and ```onPinDeactivated()``` were used as callback function names.

* Implement the callback functions according to what you've defined as function names for the callback parameters. If you like to use the same function names as in the example, you may copy the following code and past it at the end of your Arduino sketch:

```c++
void onPinActivated(int pinNumber){
	// do something according to the _pinNR that is triggered
	// for instance:
	Serial.println(pinNumber);
}
void onPinDeactivated(int pinNumber){
	// do something according to the _pinNR that is triggered
	// for instance:
	Serial.println(pinNumber);
}
```

### Using Closures
If you're familiar with the concept of closures, you can use them with FTDebouncer. The benefits are more cohesive code and it avoids evaluation of the pin number using `if / else if` blocks or a `switch` statement.

The setup procedure remains the same as described above. Only the callbacks are different. Instead of implementing named functions like ```onPinActivated()``` / ```onPinDeactivated()``` you can use lambdas as shown in the following example:

**Example:**
```c++
#include "FTDebouncer-Lambda.h"

FTDebouncer pinDebouncer;

void setup(){
	Serial.begin(57600);

	pinDebouncer.addPin(2, HIGH, INPUT_PULLUP, [](int pin)) {
		Serial.print("Pin activated: ");
		Serial.println(2);
	}, [](int pin) {
		Serial.print("Pin deactivated: ");
		Serial.println(2);
	});
	pinDebouncer.begin();	
	delay(1000);
}
void loop(){	
	pinDebouncer.update();	
}
```
When accessing variables inside the lambda functions make sure to use the proper capturing specifiers:

- `[]`captures no variables used in the lambda
- `[&]` captures all variables used in the lambda by reference
- `[=]` captures all variables used in the lambda by value

	
**Note**: This class can easily be reworked to function outside of the Arduino framework if you know how to handle time, Data Direction Registers, HAL etc.
