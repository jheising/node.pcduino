#include "gpio.h"
#include <string.h>

using namespace std;

inline void writeFile(fstream* file, int value)
{
	file->seekp(0, ios::beg);
	*file << value;
	file->flush();
}

inline int readFile(fstream* file)
{
	file->seekp(0, ios::beg);

	char state;

	*file >> state;

	return state - 48;
}

GPIO::GPIO()
{
	char path[256];

	for (int index = 0; index < GPIO_PIN_COUNT; index++)
    {
    	memset(path,0,sizeof(path));
    	sprintf(path, "%s%d", GPIO_MODE_PATH, index);
    	pinModeFiles[index] = new fstream(path);

		memset(path,0,sizeof(path));
        sprintf(path, "%s%d", GPIO_PIN_PATH, index);
    	pinDataFiles[index] = new fstream(path);
    }
}

GPIO::~GPIO()
{
	for (int index = 0; index < GPIO_PIN_COUNT; index++)
    {
        pinModeFiles[index]->close();
        delete pinModeFiles[index];

        pinDataFiles[index]->close();
        delete pinDataFiles[index];
    }
}

int GPIO::pinCount()
{
	return GPIO_PIN_COUNT;
}

void GPIO::pinMode(int pin, PIN_MODE mode)
{
	writeFile(pinModeFiles[pin], mode);
}

void GPIO::digitalWrite(int pin, PIN_STATE state)
{
	writeFile(pinDataFiles[pin], state);
}

PIN_STATE GPIO::digitalRead(int pin)
{
	return (PIN_STATE)readFile(pinDataFiles[pin]);
}