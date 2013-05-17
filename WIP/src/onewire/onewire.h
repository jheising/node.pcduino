#ifndef __onewire__h__
#define __onewire_h__

class GPIO;

typedef unsigned char uint8_t;

class OneWire
{
public:

	OneWire(uint8_t pinNumber);
	~OneWire();

	uint8_t reset();
	void write(uint8_t v, uint8_t power);
	uint8_t read();
	void select( uint8_t rom[8]);
	void skip();
	void depower();
	int search(uint8_t *newAddr);
	void reset_search();
	uint8_t crc8( uint8_t *addr, uint8_t len);
	unsigned short crc16(unsigned short *data, unsigned short len);

private:
	GPIO* gpio;
	uint8_t pin;
	uint8_t address[8];
	char searchJunction;
    uint8_t searchExhausted;

	void write_bit(uint8_t v);
	uint8_t read_bit();

};

#endif