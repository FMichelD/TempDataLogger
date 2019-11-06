#include "ThermoparK.h"

ThermoparK::ThermoparK()
{

}

float ThermoparK::readCelcius(uint8_t pin){
    uint16_t rawTemp = read16(pin);

    //Check if sensor is not connected
    if(rawTemp & 0x0004){
        return NAN;
    }

    // sensor connected
    // remove sensor data from raw readed temperature
    rawTemp >>= 3;

    // Convert raw temperature to Celcius
    return  rawTemp *= 0.25F;
}
