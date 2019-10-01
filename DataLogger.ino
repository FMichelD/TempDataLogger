#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include <DS1307.h>
#include <TimerOne.h>

#include "Button.h"
#include "LCD_Menu.h"
#include "rtc.h"
#include "DateTime.h"
#include "sdcardlog.h"
#include "ThermoparK.h"

const int8_t numberOfSensors = 10;
uint8_t sensorCSPINs[numberOfSensors]{40, 41, 42, 43, 44,
                                      45, 46, 47, 48, 49};
ThermoparK thermoparK;

uint8_t sdcardOK = false;

//Extern declared variables
extern uint8_t logging;
extern uint8_t loggingInterval;
extern uint8_t actualTime;
extern uint8_t nextLoggingTime;

extern HardwareSerial Serial;
extern DS1307 rtc;
extern SdFs sd;
extern FsFile file;
extern char line[40];

String SDCardFileName; //ex. 15d05m2019_15h30

char* strTime = (char*)"00:00:00";
char* strDate = (char*)"00/00/0000";

extern LiquidSystem systemMenu;

const byte pwmPin = 13;
byte pwmLevel = 0;

// Variables for controlling a pin and displaying the state with text.
// char* is used for adding changing text to the LiquidLine object.
const byte ledPin = LED_BUILTIN;
bool ledState = LOW;
char* ledState_text;
const byte SSR = 11;

// Variable 'analogValue' is later configured to be printed on the display.
// This time a static variable is used for holding the previous analog value.
const byte analogPin = A5;
unsigned short analogValue = 0;

void pwm_up() {
	if (pwmLevel < 225) {
		pwmLevel += 25;
	} else {
		pwmLevel = 250;
	}
	analogWrite(pwmPin, pwmLevel);
}

// Function to be attached to the pwm_line object.
void pwm_down() {
	if (pwmLevel > 25) {
		pwmLevel -= 25;
	} else {
		pwmLevel = 0;
	}
	analogWrite(pwmPin, pwmLevel);
}

void copy(char* src, char* dst, int len) {
    for (int i = 0; i < len; i++) {
        *dst++ = *src++;
    }
    dst[len+1] = '\0';
}

void SensorsLogSDCard(uint8_t numberOfSensors, uint8_t* sensorCSPINs){
    Serial.print("Salvando no arquivo: ");
    Serial.println(SDCardFileName);

    char fileName[17];

    SDCardFileName.toCharArray(fileName, 17);
    
    if (!file.open(fileName, FILE_WRITE)) 
    {
        error("open failed");
    }

    file.print(strDate);
    file.print(';');
    file.print(strTime);
    file.print(';');
    
    double temperature;
    for(int8_t i = 0; i < numberOfSensors; i++)
    {
        temperature = static_cast<double>(thermoparK.readCelcius(sensorCSPINs[i]));
        file.print(temperature);        
        file.print(';');
    }

    file.println();
    
    file.close();
}

void SensorsLogSerial(uint8_t numberOfSensors, uint8_t* sensorCSPINs){

    double temperature;
    for(int8_t i = 0; i < numberOfSensors; i++) {

        temperature = static_cast<double>(thermoparK.readCelcius(sensorCSPINs[i]));

        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(temperature);
    }

    Serial.println();
    Serial.println();
}

void updateLoggingTime(){
    nextLoggingTime += loggingInterval;
    nextLoggingTime = nextLoggingTime % 60;
}


void updateLcd(const unsigned int period)
{
     // Periodically updates lcd.
    static unsigned long lastMillisLCD = 0;
    if(millis() - lastMillisLCD > period*2) {
        lastMillisLCD = millis();

        copy(rtc.getDateStr(FORMAT_SHORT, FORMAT_LITTLEENDIAN, '/'), strDate, 11);
        copy(rtc.getTimeStr(FORMAT_LONG), strTime, 9);
        systemMenu.update();
    }
}

void logTemp(const unsigned int period)
{
    // Periodically logging temperature sensors.
    static unsigned long lastMillis = 0;
    if(millis() - lastMillis > 2*period)
    {
        lastMillis = millis();

        SensorsLogSerial(numberOfSensors, sensorCSPINs);
        
        if(logging)
        {
            Time t;

            t = rtc.getTime();
            actualTime = t.min;

            if(actualTime == nextLoggingTime)
            {
                lcd.print("Salvando...");
                updateLoggingTime();
                lcd.clear();
                copy(rtc.getDateStr(FORMAT_SHORT, FORMAT_LITTLEENDIAN, '/'), strDate, 11);
                copy(rtc.getTimeStr(FORMAT_LONG), strTime, 9);

                if(sdcardOK)
                {
                    SensorsLogSDCard(numberOfSensors, sensorCSPINs);
                }                
            }
        }
    }
}

float readOilTemp(void)
{
    //read temperature of sensor on cs pin 40
    return thermoparK.readCelcius(40);
}

void heatOil(void){
    //Timer1.stop();
    
    extern uint8_t ledLogState;
    bool logging = digitalRead(ledLogState); 
    
    double temperature = readOilTemp();

    Serial.print("Oil temp: ");
    Serial.println(temperature);
    
    if(temperature < 60 && logging)
    {
        digitalWrite(SSR, HIGH);
    }else{
        digitalWrite(SSR, LOW);
    }

    //Timer1.start();
}


void setup() {
    Serial.begin(115200);
    lcd.begin(40, 2);

    pinMode(SSR, OUTPUT);
    digitalWrite(SSR, LOW);
  
    lcd.print("Unioeste-CSC");

    initRTC();
    initMenu();

    SPI.begin();

    //Configura os pinos de seleção de sensores como saida em nivel alto.
    for(uint8_t i = 0; i < numberOfSensors; ++i){
        pinMode(sensorCSPINs[i], OUTPUT);
        digitalWrite(sensorCSPINs[i], HIGH);
    }

    delay(1000);

    lcd.clear();
    lcd.println("Initialize the SD");

    //Initialize the SD.
    if (!sd.begin(SD_CONFIG)) {
        sd.initErrorHalt(&Serial);
        sdcardOK = false;
        lcd.print("Falha no SD");
    }else {
        sdcardOK = true;
    }

    delay(1000);

    Timer1.initialize(500000); // Inicializa o Timer1 e configura para um período de 0,5 segundos
    Timer1.attachInterrupt(heatOil);
}

void loop()
{
    static unsigned int period = 500;

    //noInterrupts();
    checkButtons();
    updateLcd(period);
    logTemp(period);
    //interrupts();
    
    //readAmbientTemp(); TODO
}
