#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "onewire.h"
#include "../gpio/gpio.h"

#include <sys/time.h>

OneWire::OneWire(uint8_t pinNumber) : pin(pinNumber), gpio(new GPIO())
{
	reset_search();
}

OneWire::~OneWire()
{
	delete gpio;
}

inline void _nanosleep(long nanoseconds)
{
	timespec start;
	timespec now;

	clock_gettime(CLOCK_REALTIME, &start);

	do
	{
		clock_gettime(CLOCK_REALTIME, &now);
	}
	while(now.tv_nsec - start.tv_nsec < nanoseconds);
}

inline void _microsleep(long microseconds)
{
	_nanosleep(microseconds * 1000);
}

uint8_t OneWire::reset()
{
    uint8_t r;
    uint8_t retries = 125;

    // wait until the wire is high... just in case
    gpio->pinMode(pin,INPUT);

    do
    {
    	--retries;

		if (retries <= 0)
		{
			return 0;
		}

		_microsleep(2);
    }
    while(gpio->digitalRead(pin) != HIGH);

    gpio->digitalWrite(pin, LOW);   // pull low for 500uS
    gpio->pinMode(pin, OUTPUT);
    _microsleep(500);

    gpio->pinMode(pin, INPUT);		// put back to input and see if it remains low for a moment
    _microsleep(65);

    if(gpio->digitalRead(pin) == LOW)
    	r = 1;
    else
    	r = 0;

    _microsleep(490);
    return r;
}

void OneWire::reset_search()
{
    uint8_t i;

    searchJunction = -1;
    searchExhausted = 0;
    for( i = 7; ; i--) {
	address[i] = 0;
	if ( i == 0) break;
    }
}

void OneWire::write_bit(uint8_t v) {
    static uint8_t lowTime[] = { 60, 1 };
    static uint8_t highTime[] = { 1, 60};

    v = (v&1);

    gpio->pinMode(pin, OUTPUT);      // make pin an output, do first since we
                                    // expect to be at 1
    gpio->digitalWrite(pin, LOW);    // zero

	timespec ts1;
	timespec ts2;

	//clock_gettime(CLOCK_REALTIME, &ts1);
    _microsleep(lowTime[v]);
    gpio->digitalWrite(pin, HIGH);   // one, push pin up - important for
                                    // parasites, they might start in here
    //clock_gettime(CLOCK_REALTIME, &ts2);

    _microsleep(highTime[v]);

    //printf("Elapsed for %d: %d\n", v, (ts2.tv_nsec - ts1.tv_nsec) / 1000);
}

uint8_t OneWire::read_bit()
{
    uint8_t r = 0;

    gpio->pinMode(pin, OUTPUT);      // make pin an output, do first since we expect to be at 1
    gpio->digitalWrite(pin, LOW);    // zero
    _microsleep(1);

    gpio->pinMode(pin, INPUT);     // let pin float, pull up will raise
    _microsleep(1);          // A "read slot" is when 1mcs > t > 2mcs
    r = gpio->digitalRead(pin); // check the bit
    _microsleep(50);        // whole bit slot is 60-120uS, need to give some time*/

    return r;
}

void OneWire::write(uint8_t v, uint8_t power) {
    uint8_t bitMask;

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
		write_bit( (bitMask & v)?1:0);
    }
    if ( !power) {
	gpio->pinMode(pin, INPUT);
	gpio->digitalWrite(pin, LOW);
    }
}

uint8_t OneWire::read() {
    uint8_t bitMask;
    uint8_t r = 0;

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	if (read_bit()) r |= bitMask;
    }
    return r;
}

void OneWire::select( uint8_t rom[8])
{
    int i;

    write(0x55,0);         // Choose ROM

    for( i = 0; i < 8; i++) write(rom[i],0);
}

void OneWire::skip()
{
    write(0xCC,0);         // Skip ROM
}

void OneWire::depower()
{
    gpio->pinMode(pin,INPUT);
}

int OneWire::search(uint8_t *newAddr)
{
    uint8_t i;
    char lastJunction = -1;
    uint8_t done = 1;

    if ( searchExhausted) return 0;

    if ( !reset()) return 0;
    write( 0xf0, 0);

    for( i = 0; i < 64; i++)
    {
		uint8_t a = read_bit( );
		uint8_t nota = read_bit( );
		uint8_t ibyte = i/8;
		uint8_t ibit = 1<<(i&7);

		if ( a && nota) return 0;  // I don't think this should happen, this means nothing responded, but maybe if
		// something vanishes during the search it will come up.
		if ( !a && !nota) {
			if ( i == searchJunction) {   // this is our time to decide differently, we went zero last time, go one.
			a = 1;
			searchJunction = lastJunction;
			} else if ( i < searchJunction) {   // take whatever we took last time, look in address
			if ( address[ ibyte]&ibit) a = 1;
			else {                            // Only 0s count as pending junctions, we've already exhasuted the 0 side of 1s
				a = 0;
				done = 0;
				lastJunction = i;
			}
			} else {                            // we are blazing new tree, take the 0
			a = 0;
			searchJunction = i;
			done = 0;
			}
			lastJunction = i;
	}
	if ( a) address[ ibyte] |= ibit;
	else address[ ibyte] &= ~ibit;

	write_bit( a);
    }
    if ( done) searchExhausted = 1;
    for ( i = 0; i < 8; i++) newAddr[i] = address[i];
    return 1;
}

static uint8_t dscrc_table[] = {
      0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
     35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
     70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
     17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
     50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
     87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

uint8_t OneWire::crc8( uint8_t *addr, uint8_t len)
{
    uint8_t i;
    uint8_t crc = 0;

    for ( i = 0; i < len; i++) {
	crc  = dscrc_table[ crc ^ addr[i] ];
    }
    return crc;
}

static short oddparity[16] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

unsigned short OneWire::crc16(unsigned short *data, unsigned short len)
{
    unsigned short i;
    unsigned short crc = 0;

    for ( i = 0; i < len; i++) {
	unsigned short cdata = data[len];

	cdata = (cdata ^ (crc & 0xff)) & 0xff;
	crc >>= 8;

	if (oddparity[cdata & 0xf] ^ oddparity[cdata >> 4]) crc ^= 0xc001;

	cdata <<= 6;
	crc ^= cdata;
	cdata <<= 1;
	crc ^= cdata;
    }
    return crc;
}