#ifndef __gpio__h__
#define __gpio_h__

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

// These are the paths and filenames of the files for GPIO access.
#define GPIO_MODE_PATH "/sys/devices/virtual/misc/gpio/mode/gpio"
#define GPIO_PIN_PATH "/sys/devices/virtual/misc/gpio/pin/gpio"
#define GPIO_PIN_COUNT 20

// Make some aliases to make the code more readable.
typedef enum {
	LOW = 0,
	HIGH = 1
} PIN_STATE;

typedef enum {
	INPUT = 0,
	OUTPUT = 1,
	INPUT_PU = 8
} PIN_MODE;

class GPIO
{
public:

	GPIO();
	~GPIO();

	int pinCount();
	void pinMode(int pin, PIN_MODE mode);
	void digitalWrite(int pin, PIN_STATE state);
	PIN_STATE digitalRead(int pin);

private:
	fstream* pinModeFiles[GPIO_PIN_COUNT];
	fstream* pinDataFiles[GPIO_PIN_COUNT];
};

#endif