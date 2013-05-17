/**
 * A module to access the analog pins of the pcDuino Microcontroller.
 *
 * @module analog
 */

var fs = require('fs');

var writePins = [];
var writePinMax = [];

// These are the pin mappings to their filesystem name/indexes
var writePinMappings = {
	3 : 0,
	5 : 1,
	6 : 2,
	9 : 3,
	10 : 4,
	11 : 5
}

var analog = {};
analog.PIN_COUNT = 5;

function init(simulate)
{
	writePins = [];
	writePinMax = [];

	var analogWritePath = "/sys/class/leds/";

	if(simulate)
	{
		// Make a directory to hold our simulated files
		if(!fs.existsSync(__dirname + "/sim"))
		{
			fs.mkdirSync(__dirname + "/sim");

			if(!fs.existsSync(__dirname + "/sim/analog"))
			{
				fs.mkdirSync(__dirname + "/sim/analog");
			}
		}

		analogWritePath = __dirname + "/sim/analog/write.";
	}

	// Create paths to our individual pins
	for(var index = 0; index < analog.PIN_COUNT; index++)
	{
		if(!simulate)
		{
			writePins.push(analogWritePath + "pwm" + index + "/brightness");

			// Read the max brightness for the pin
			writePinMax.push(fs.readFileSync(analogWritePath + "pwm" + index + "/max_brightness", {encoding:"utf8"}));
		}
		else
		{
			writePinMax.push(255);
			writePins.push(analogWritePath + "pwm" + index);

			if(!fs.existsSync(writePins[index]))
			{
				fs.writeFileSync(writePins[index], "0");
			}
		}
	}
}

/**
 * Simulate reading or writing values to the analog pins. Can be used to simulate values when running on a non pcDuino system.
 *
 * @param {boolean} simulate - Set to true to simulate, or false to not simulate.
 */
analog.simulate = init;

/**
 * Get the maximum value that can be written to an analog pin.
 *
 * @param pin
 * @returns {number} - The maximum value that can be written to the analog pin.
 */
analog.maxDigitalWriteValue = function(pin)
{
	if(!(pin in writePinMappings))
	{
		throw "Invalid pin.";
	}

	// Map our pin
	pin = writePinMappings[pin];

	return writePinMax[pin];
}

/**
 * Set the value of an analog pin.
 *
 * @param {number} pin - A pin number from 0 to PIN_COUNT. Note that the only valid pins for output are 3, 5, 6, 9, 10, and 11. Any other pin specified will throw an error.
 * @param {number} value - A value of 0 to a value returned from the function maxDigitalWriteValue for pin.
 */
analog.digitalWrite = function(pin, value)
{
	if(!(pin in writePinMappings))
	{
		throw "Invalid pin.";
	}

	// Map our pin
	pin = writePinMappings[pin];

	fs.writeFileSync(writePins[pin], Math.min(String(value), writePinMax[pin]));
}

// Init without simulating by default
init(false);

module.exports = analog;
