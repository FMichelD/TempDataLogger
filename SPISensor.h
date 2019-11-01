#ifndef SPISENSOR_H
#define SPISENSOR_H

#include <Arduino.h>
#include <SPI.h>
//#include <WString.h>

class SPISensor
{
public:
    SPISensor();

    uint8_t read(uint8_t pin);
    uint16_t read16(uint8_t pin);
};

#endif // SPISENSOR_H
