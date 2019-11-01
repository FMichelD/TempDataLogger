#ifndef THERMOPARK_H
#define THERMOPARK_H

#include "SPISensor.h"

class ThermoparK : public SPISensor
{
public:
    ThermoparK();

    double readCelcius(uint8_t pin);
};

#endif // THERMOPARK_H
