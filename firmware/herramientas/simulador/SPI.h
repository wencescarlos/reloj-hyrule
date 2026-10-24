#pragma once
#include "Arduino.h"
struct SPISim{void begin(){} void end(){} void setFrequency(int){} void setBitOrder(int){} void setDataMode(int){} uint8_t transfer(uint8_t b);};
extern SPISim SPI;
