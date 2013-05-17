/**
 * A module to access the analog pins of the pcDuino Microcontroller.
 *
 * @module analog
 */

var fs = require('fs');

var writePins = [];
var readPins = [];
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
analog.PIN_COUNT = 6;

function init(simulate)
{
	writePins = [];
	readPins = [];
	writePinMax = [];

	var analogWritePath = "/sys/class/leds/";
	var analogReadPath = "/proc/";

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
		analogReadPath = __dirname + "/sim/analog/read.";
	}

	// Create paths to our individual pins
	for(var index = 0; index < analog.PIN_COUNT; index++)
	{
		if(!simulate)
		{
			writePins.push(analogWritePath + "pwm" + index + "/brightness");
			readPins.push(analogReadPath + "adc" + index);

			// Read the max value for the input pin
			writePinMax.push(Number(fs.readFileSync(analogWritePath + "pwm" + index + "/max_brightness", "utf8")));
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
 * @param {number} pin - A pin number. Note that the only valid pins for output are 3, 5, 6, 9, 10, and 11. Any other pin specified will throw an error.
 * @returns {number} - The maximum value that can be written to the analog pin.
 */
analog.maxWriteValue = function(pin)
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
 * @param {number} pin - A pin number. Note that the only valid pins for output are 3, 5, 6, 9, 10, and 11. Any other pin specified will throw an error.
 * @param {number} value - A value of 0 to a value returned from the function maxWriteValue for pin.
 */
analog.analogWrite = function(pin, value)
{
	if(!(pin in writePinMappings))
	{
		throw "Invalid pin.";
	}

	// Map our pin
	pin = writePinMappings[pin];

	fs.writeFileSync(writePins[pin], Math.min(String(value), writePinMax[pin]));
}

analog.analogRead = function(pin)
{
	if(pin < 0 || pin >= analog.PIN_COUNT)
	{
		throw "Invalid pin.";
	}

	// Appears as if the file result is in the format of adcX:value. So I guess we need to parse it
	var output = fs.readFileSync(readPins[pin], "utf8");

	var values = output.split(":");

	return Number(values[values.length - 1]);
}

// Init without simulating by default
init(false);

module.exports = analog;
