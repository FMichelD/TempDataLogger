#include "SPISensor.h"

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
SPISensor::read16(uint8_t pin){

//    //discard acutal conversion
//    digitalWrite(pin, LOW);
//    delay(1);
//
//    //initalize new convervion
//    digitalWrite(pin, HIGH);
//    delay(350); //wait for conversion time: from datashee 0.17s - 0.22s (max)
//    
    //read actual temperature.
    digitalWrite(pin, LOW);
    delayMicroseconds(100);
    uint16_t data = SPI.transfer16(0xFFFF);
    digitalWrite(pin, HIGH);
    delay(10);

    return data;
}
