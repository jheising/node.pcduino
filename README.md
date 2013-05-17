# node.pcduino

A node.js module for accessing the Arduino compatible pins on the pcDuino microcontroller. It tries to maintain as close a match as possible to the original Arduino C++ functions.

## Installation

    $ npm install pcduino

## Limitations

The current version of this module uses the standard unix filesystem mappings to the GPIO pins, which, while fast enough for most general purposes, are not fast enough to perform high-speed protocols (like 1-wire for example).
There is a subdirectory called WIP, which is a start to some pure C++ native code access to the pins— although even it appears to be too slow and will require the use of interrupts or DMA at some point.

