/**
 * A module to access the analog pins of the pcDuino Microcontroller.
 *
 * @module analog
 */

var fs = require('fs');
var analogNative = require('./../build/Release/node_pcduino_analog');

var analog = {};

/**
 * Set the value of an analog pin.
 *
 * @param {number} pin - A pin number. Note that the only valid pins for output are 3, 5, 6, 9, 10, and 11. Any other pin specified will throw an error.
 * @param {number} value - A value of 0 to a value returned from the function maxWriteValue for pin.
 */
analog.analogWrite = function(pin, value)
{
	analogNative.analogWrite(pin, value);
}

/**
 * Reads the value of an analog pin.
 *
 * @param {number} pin - A pin number. Note that the only valid pins for output are 0, 1, 2, 3, 4, 5. Any other pin specified will throw an error.
 */
analog.analogRead = function(pin)
{
	return analogNative.analogRead(pin);
}

module.exports = analog;
