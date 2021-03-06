#include "SPISensor.h"
#include <avr/wdt.h>

SPISensor::SPISensor()
{

}

uint8_t
SPISensor::read(uint8_t pin){
    digitalWrite(pin, LOW);
    uint8_t data = SPI.transfer(0xFF);
    digitalWrite(pin, HIGH);

    return data;
}

uint16_t
SPISensor::read16(uint8_t pin)
{   
    //discard acutal conversion
    digitalWrite(pin, LOW);
    SPI.transfer16(0xFFFF);
    delay(15);

    //initalize new convervion
    digitalWrite(pin, HIGH);
    delay(280); //wait for conversion time: from datashee 0.17s - 0.22s (max)
    wdt_reset();
    
    //read actual temperature.
    digitalWrite(pin, LOW);
    __asm__("nop"); //waste 62.5ns
    __asm__("nop"); //waste 62.5ns
    __asm__("nop"); //waste 62.5ns
    uint16_t data = SPI.transfer16(0xFFFF);
    digitalWrite(pin, HIGH);

    return data;
}
