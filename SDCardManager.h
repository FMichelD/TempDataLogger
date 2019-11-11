#ifndef SDCARD_MANAGER_H
#define SDCARD_MANAGER_H
#pragma once

#include "DataStructs.h"

#include <SdFs.h>

// SD_FAT_TYPE = 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 3

/*
  Change the value of SD_CS_PIN if you are using SPI and
  your hardware does not use the default value, SS.
  Common values are:
  Arduino Ethernet shield: pin 4
  Sparkfun SD shield: pin 8
  Adafruit SD shields and modules: pin 10
*/
// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = A8;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = A8;
#endif  // SDCARD_SS_PIN

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
    #define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
    #define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI)
#else  // HAS_SDIO_CLASS
    #define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI)
#endif  // HAS_SDIO_CLASS

#if SD_FAT_TYPE == 1
   extern SdFat sd;
   extern File file;
#elif SD_FAT_TYPE == 2
   extern SdExFat sd;
   extern ExFile file;
#elif SD_FAT_TYPE == 3
   extern SdFs sd;
   extern FsFile file;
#else  // SD_FAT_TYPE
#error SD_FAT_TYPE
#endif  // SD_FAT_TYPE

extern char line[15];
extern uint8_t sdcardOK;
extern TemperatureLimits tl;
extern bool heater;

//------------------------------------------------------------------------------
// Store error strings in flash to save RAM.
#define error(s) sd.errorHalt(&Serial, F(s))

void initSDCard(void);
char* skipSpace(char* str);
bool parseLine(char* str, TemperatureLimits *tl);
int getNewTempCondiction(TemperatureLimits *tl);

#endif //SDCARD_MANAGER_H
