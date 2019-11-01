#include "LCDManager.h"
#include "SDCardManager.h"
#include "LCD_Menu.h"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <LiquidMenu.h>



extern HardwareSerial Serial;

String SDCardFileName; //ex. 15d05m2019_15h30
char* strTime = (char*)"00:00:00";
char* strDate = (char*)"00/00/0000";

// SD_FAT_TYPE = 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 3

#if SD_FAT_TYPE == 1
   SdFat sd;
   File file;
#elif SD_FAT_TYPE == 2
   SdExFat sd;
   ExFile file;
#elif SD_FAT_TYPE == 3
   SdFs sd;
   FsFile file;
#else  // SD_FAT_TYPE
#error SD_FAT_TYPE
#endif  // SD_FAT_TYPE

const int8_t numberOfSensors = 18;                                     
uint8_t sensorCSPINs[numberOfSensors]{ 40, 41, 42, 43, 44,
                                       45, 46, 47, 48, 49,
                                       39, 38, 37, 36, 35, 
                                       34, 33, 31};

void configureCSPinsForSensors(void)
{
    //Configura os pinos de seleção de sensores como saida em nivel alto.
    for(uint8_t i = 0; i < numberOfSensors; ++i){
        pinMode(sensorCSPINs[i], OUTPUT);
        digitalWrite(sensorCSPINs[i], HIGH);
        Serial.print("Pino ");
        Serial.print(sensorCSPINs[i]);
        Serial.print(":");
        Serial.println(digitalRead(sensorCSPINs[i]));
    }  
}

void copy(char* src, char* dst, int len) {
    for (int i = 0; i < len; i++) {
        *dst++ = *src++;
    }
    dst[len+1] = '\0';
}

void updateLcd()
{
    noInterrupts();
    unsigned long period = 1000; // every 1000ms updates lcd.
    
    static unsigned long lastMillisLCD = 0;
    if(millis() - lastMillisLCD >= period) {
        lastMillisLCD = millis();

        copy(rtc.getDateStr(FORMAT_SHORT, FORMAT_LITTLEENDIAN, '/'), strDate, 11);
        copy(rtc.getTimeStr(FORMAT_LONG), strTime, 9);
        systemMenu.update();
    }
    interrupts();
}

void setup() {
    Serial.begin(115200);
    configureCSPinsForSensors(); 
    pinMode(ledLogState, OUTPUT);   
    delay(50);    
    lcd.begin(40, 2);
    lcd.clear();
    lcd.print("Initialize the SD");    
    SPI.begin(); 
    initSDCard();
}

void loop() {
    // put your main code here, to run repeatedly:
    updateLcd();
    checkButtons();
    //test1();
}
