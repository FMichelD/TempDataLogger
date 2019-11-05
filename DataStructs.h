#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <Arduino.h>

typedef struct{
  bool heater;
  uint8_t temperature;
  uint32_t minTime;
  uint32_t maxTime;
}TemperatureLimits;

#endif //DATA_STRUCTS_H
