var fs = require('fs');



var pinModes = [];
var pinStates = [];

var gpio = {};
gpio.INPUT = 0;
gpio.OUTPUT = 1;
gpio.INPUT_PU = 8;
gpio.LOW = 0;
gpio.HIGH = 1;
gpio.PIN_COUNT = 20;

function init(simulate)
{
	pinModes = [];
	pinStates = [];

	var gpioModePath = "/sys/devices/virtual/misc/gpio/mode/";
	var gpioStatePath = "/sys/devices/virtual/misc/gpio/pin/";

	if(simulate)
	{
		// Make a directory to hold our simulated files
		if(!fs.existsSync(__dirname + "/sim"))
		{
			fs.mkdirSync(__dirname + "/sim");

			if(!fs.existsSync(__dirname + "/sim/gpio"))
			{
				fs.mkdirSync(__dirname + "/sim/gpio");
			}
		}

		gpioModePath = __dirname + "/sim/gpio/mode.";
		gpioStatePath = __dirname + "/sim/gpio/pin.";
	}

	// Create paths to our individual pins
	for(var index = 0; index < gpio.PIN_COUNT; index++)
	{
		pinModes.push(gpioModePath + "gpio" + index);
		pinStates.push(gpioStatePath + "gpio" + index);

		if(simulate)
		{
			if(!fs.existsSync(pinModes[index]))
			{
				fs.writeFileSync(pinModes[index], "0");
			}

			if(!fs.existsSync(pinStates[index]))
			{
				fs.writeFileSync(pinStates[index], "0");
			}
		}
	}
}

function validatePin(pin)
{
	if(pin < 0 || pin >= gpio.PIN_COUNT)
	{
		throw "Invalid pin";
	}
}

/**
 *
 * @param simulate
 */
gpio.simulate = init;

/**
 *
 * @param pin
 * @param mode
 */
gpio.pinMode = function(pin, mode)
{
	validatePin(pin);
	fs.writeFileSync(pinModes[pin], String(mode));
}

/**
 *
 * @param pin
 */
gpio.digitalRead = function(pin)
{
	validatePin(pin);
	return Number(fs.readFileSync(pinStates[pin], {encoding:'utf8'}));
}

/**
 *
 * @param pin
 * @returns {boolean}
 */
gpio.isHIGH = function(pin)
{
	return (gpio.digitalRead(pin) === gpio.HIGH);
}

/**
 *
 * @param pin
 * @returns {boolean}
 */
gpio.isLOW = function(pin)
{
	return (gpio.digitalRead(pin) === gpio.LOW);
}

/**
 *
 * @param pin
 * @param value
 */
gpio.digitalWrite = function(pin, value)
{
	validatePin(pin);
	fs.writeFileSync(pinStates[pin], String(value));
}

// Init without simulating
init(false);

module.exports = gpio;
