# node.pcduino

A node.js module for accessing the Arduino compatible pins on the pcDuino microcontroller. It tries to maintain as close a match as possible to the original Arduino C++ functions.

## Installation

From the command line:

    $ npm install pcduino

Or in your package.json file:

	{
    	"dependencies" : {
    		"pcduino" : "*",
    	}
    }

## A digital example

```js
var pcduino = require("pcduino");
var digital = pcduino.digital;

// digital.simulate(true); - Can be used for testing purposes when you're not running on a pcDuino. This will read and write to dummy files.

console.log("There are " + digital.PIN_COUNT + " digital GPIO pins on the pcDuino.");


digital.pinMode(10, digital.INPUT); // Set pin #10 to input
digital.pinMode(10, digital.INPUT_PU); // Set pin #10 to input with pull-up


var state = digital.digitalRead(10);
console.log("The value of pin #10 is " + (state == digital.HIGH ? "HIGH" : "LOW"));


digital.pinMode(10, digital.OUTPUT);          // Set pin #10 to output


digital.digitalWrite(10, digital.HIGH);       // Set pin #10 HIGH

state = digital.digitalRead(10);
console.log("The value of pin #10 is now " + (state == digital.HIGH ? "HIGH" : "LOW"));


digital.digitalWrite(10, digital.LOW);        // Set pin #10 LOW

state = digital.digitalRead(10);
console.log("The value of pin #10 is now " + (state == digital.HIGH ? "HIGH" : "LOW"));
```

## An analog example

```js
var pcduino = require("pcduino");
var analog = pcduino.analog;

// analog.simulate(true); - Can be used for testing purposes when you're not running on a pcDuino. This will read and write to dummy files.

console.log("There are " + analog.PIN_COUNT + " analog pins on the pcDuino.");

// Note that the only valid pins for analog output are 3, 5, 6, 9, 10, and 11. These are PWM (digital) pins on the pcDuino and not to be confused with the analog input pins. Any other pin specified will throw an error.
var maxValue = analog.maxWriteValue(5);
console.log("The maximum number that can be written to analog pin #5 is " + maxValue);

analog.analogWrite(5, maxValue);            // Turn our pin fully up

// Note this is not the same as the analog write pin #5! This maps to the analog input pins on the pcDuino.
var readValue = analog.analogRead(5);
console.log("The analog value of pin #5 is " + readValue + ". Note this is not the same as the analog write pin #5!");
```

## Limitations

The current version of this module uses the standard unix filesystem mappings to the GPIO pins which, while fast enough for most general purposes, are not fast enough to perform high-speed protocols (like 1-wire for example).
There is a subdirectory called WIP, which is a start to some pure C++ native code access to the pins— although even it appears to be too slow and will probably require the use of interrupts or DMA at some point.

