#include "Button.h"
#include "LCD_Menu.h"
#include "rtc.h"
#include "DateTime.h"
#include "SDCardManager.h"
#include "ThermoparK.h"
#include "Ciclico.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include <DS1307.h>
#include <TimerOne.h>



const int8_t numberOfSensors = 18;

//uint8_t sensorCSPINs[numberOfSensors]{40};
                                      
uint8_t sensorCSPINs[numberOfSensors]{ 40, 41, 42, 43, 44,
                                       45, 46, 47, 48, 49,
                                       39, 38, 37, 36, 35, 
                                       34, 33, 31};
ThermoparK thermoparK;

double temperature[numberOfSensors];

//Extern declared variables
extern uint8_t logging;
extern uint8_t loggingInterval;
extern uint8_t actualTime;
extern uint8_t nextLoggingTime;

TemperatureLimits tl;
uint32_t actualTempTime;
//static int currentTempTime; 
uint32_t minTempTime; 
uint32_t maxTempTime;

extern HardwareSerial Serial;
extern DS1307 rtc;

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

void SensorsLogSDCard(uint8_t numberOfSensors){
  
    noInterrupts();    
    
    char fileName[17];

    SDCardFileName.toCharArray(fileName, 17);

    Serial.println("Abrindo arquivo arquivo...");
    if (!file.open(fileName, FILE_WRITE)) 
    {
        //error("open failed");
        Serial.println("SD File fail");
    }

    lcd.clear();
    lcd.print("Salvando...");
    
    Serial.print("Salvando no arquivo: ");
    Serial.println(SDCardFileName);
    
    file.print(strDate);
    file.print(';');
    file.print(strTime);
    file.print(';');    
    
    for(int8_t i = 0; i < numberOfSensors; i++)
    {       
        file.print(temperature[i]);  
        file.print(';');
    }

    file.println();

    Serial.println("Fechando arquivo...");
    file.close();

    Serial.println("Salvamento OK");

    delay(500);
    Serial.println(digitalRead(SS));
    interrupts();
}

void SensorsLogSerial(uint8_t numberOfSensors, uint8_t* sensorCSPINs)
{
    noInterrupts();
    
    for(int8_t i = 0; i < numberOfSensors; i++) {

        temperature[i] = static_cast<double>(thermoparK.readCelcius(sensorCSPINs[i]));

<<<<<<< HEAD
        switch(sensorCSPINs[i]){
            case 34:
                Serial.print("Oleo_01: ");
                break;
            
            case 33:
                Serial.print("Oleo_02: ");
                break;
            
            case 31:
                Serial.print("TAmb: ");
                break;
            
            default:
                Serial.print("Sensor ");
                Serial.print(i);
                Serial.print(": ");
        }
        
        Serial.println(temperature[i]);
        digitalWrite(sensorCSPINs[i], HIGH);
        delay(250);
=======
//        switch(sensorCSPINs[i]){
//            case 34:
//                Serial.print("Oleo_01: ");
//                break;
//            
//            case 33:
//                Serial.print("Oleo_02: ");
//                break;
//            
//            case 31:
//                Serial.print("TAmb: ");
//                break;
//            
//            default:
//                Serial.print("Sensor ");
//                Serial.print(i);
//                Serial.print(": ");
//        }
        
//        Serial.println(temperature[i]);
//        digitalWrite(sensorCSPINs[i], HIGH);
//        delay(250);
>>>>>>> d8899fa... Iniicio do  Cliclico bugadaco
    }
    Serial.println();

    interrupts();
}

void updateLoggingTime(){
    nextLoggingTime += loggingInterval;
    nextLoggingTime = nextLoggingTime % 60;
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

double readOilTemp(void)
{
    //read temperature of sensor on cs pin 34
    return thermoparK.readCelcius(34);
}

void heatOil(void)
<<<<<<< HEAD
{ 
    extern uint8_t ledLogState;
    bool logging = digitalRead(ledLogState);

    double OilTemperature = readOilTemp();
    //static double OilTemperature = 50;//readOilTemp();

    

    if(logging)
    {        
        if(OilTemperature < 85)
        {
            digitalWrite(SSR, HIGH);
        }else{
            digitalWrite(SSR, LOW);
        } 
=======
{
    Time t;    
    extern uint8_t ledLogState;
    bool logging = digitalRead(ledLogState);

    //double OilTemperature = readOilTemp();
    static double OilTemperature = 50;//readOilTemp();

    static bool aquecido = false;
    static bool aquecer = true;
    static bool resfriado = false;
    static bool resfriar = false;
    static bool highTemp = false;
    static bool lowTemp = false;
    static uint32_t maxHighTempTime = 999999;
    static uint32_t minLowTempTime = 999999;
    static uint32_t maxLowTempTime = 999999;

    if(logging)
    {
        if(TempFixed)
        {
            if(OilTemperature < 85)
            {
                digitalWrite(SSR, HIGH);
            }else{
                digitalWrite(SSR, LOW);
            }
        }else{

            Serial.print("Oil temp: ");
            Serial.print(OilTemperature);                       
            Serial.print("   TemperatureTarget: ");
            Serial.print(tl.temperature);
            Serial.print("   minLowTempTime: ");
            Serial.print(minLowTempTime);
            Serial.print("   maxLowTempTime: ");
            Serial.print(maxLowTempTime);            
            Serial.print("   actualTempTime: ");
            Serial.print(actualTempTime);
            Serial.print("   maxHighTempTime: ");
            Serial.println(maxHighTempTime);

            if(OilTemperature <= tl.temperature && !aquecido)
            {
                aquecer = true;
                resfriar = false;
            }

            if(OilTemperature > tl.temperature && !resfriado)
            {
                resfriar = true;
                aquecer = false;
            }
            
            if(OilTemperature >= tl.temperature && actualTempTime >= maxHighTempTime && aquecer)
            {                
                aquecido = true;
                aquecer = false;
                resfriado = false;
                resfriar = true;
                Serial.println("Aquecido desligando SSR");
                digitalWrite(SSR, LOW); //desliga a resistencia do oleo
                
                Serial.println("Aquecido ler novo dado...");
                getNewTempCondiction(&tl);
                
                t = rtc.getTime();
                actualTempTime = (t.date * 24 * 3600) + (t.hour * 360) + (t.min * 60) + t.sec;
                minLowTempTime = actualTempTime + tl.minTime;

                uint32_t maxHighTempTime = 999999;
                uint32_t minLowTempTime = 999999;
                uint32_t maxLowTempTime = 999999;

                Serial.println("\n\n\n");
                Serial.print("Oil temp: ");
                Serial.print(OilTemperature);                       
                Serial.print("   TemperatureTarget: ");
                Serial.print(tl.temperature);
                Serial.print("   minLowTempTime: ");
                Serial.print(minLowTempTime);
                Serial.print("   maxLowTempTime: ");
                Serial.print(maxLowTempTime);            
                Serial.print("   actualTempTime: ");
                Serial.print(actualTempTime);
                Serial.print("   maxHighTempTime: ");
                Serial.println(maxHighTempTime);   
                Serial.println("\n\n\n");
                delay(3000);             
                
            }

            Serial.println((((OilTemperature <= tl.temperature && actualTempTime >= minLowTempTime) || actualTempTime >= maxLowTempTime) && resfriar)?"\nif true\n" : "\nif false\n");
            if(((OilTemperature <= tl.temperature && actualTempTime >= minLowTempTime) || actualTempTime >= maxLowTempTime) && resfriar) 
            {
                aquecido = false;
                aquecer = true;
                resfriado = true;
                resfriar = false;
                Serial.println("Resfriado, religando SSR");
                digitalWrite(SSR, HIGH); //liga a resistencia do oleo
                
                Serial.println("Resfriado ler novo dado...");
                getNewTempCondiction(&tl);
                t = rtc.getTime();
                actualTempTime = (t.date * 24 * 3600) + (t.hour * 360) + (t.min * 60) + t.sec;
                maxHighTempTime = actualTempTime + tl.minTime;

                uint32_t maxHighTempTime = 999999;
                uint32_t minLowTempTime = 999999;
                uint32_t maxLowTempTime = 999999;                

                Serial.println("\n\n\n");
                Serial.print("Oil temp: ");
                Serial.print(OilTemperature);                       
                Serial.print("   TemperatureTarget: ");
                Serial.print(tl.temperature);
                Serial.print("   minLowTempTime: ");
                Serial.print(minLowTempTime);
                Serial.print("   maxLowTempTime: ");
                Serial.print(maxLowTempTime);            
                Serial.print("   actualTempTime: ");
                Serial.print(actualTempTime);
                Serial.print("   maxHighTempTime: ");
                Serial.println(maxHighTempTime);   
                Serial.println("\n\n\n");
                delay(3000);
            }
            
            if(!aquecido && aquecer)
            {
                if(OilTemperature >= tl.temperature)
                {
                    Serial.println("Temperatura do oleo alcançada:");
                    Serial.println(OilTemperature);
                    Serial.println("estabilizando...");
                    aquecer = false;   
                                  
                    if(!highTemp)
                    {                
                        t = rtc.getTime();
                        actualTempTime = (t.date * 24 * 3600) + (t.hour * 360) + (t.min * 60) + t.sec;                                    
                        maxHighTempTime = actualTempTime + tl.maxTime;
    
                        Serial.print("\nmaxTempTimeCorrigido: ");
                        Serial.println(maxHighTempTime);
                    }
                    highTemp = true; 
                    lowTemp = false; 
                }else{
                    OilTemperature += 25;
                    Serial.println("aquecendo o oleo...");                
                    digitalWrite(SSR, HIGH); //aquece o oleo
                }         
            }
                       
            if(!resfriado && resfriar)
            {                                 
                if(OilTemperature <= tl.temperature ||  actualTempTime > minLowTempTime)
                {
                    Serial.println("Temperatura do oleo alcançada:");
                    Serial.println(OilTemperature);
                    Serial.println("estabilizando...");                 
                    resfriar = false;

                    if(!lowTemp)
                    {                
                        t = rtc.getTime();
                        actualTempTime = (t.date * 24 * 3600) + (t.hour * 360) + (t.min * 60) + t.sec;
                        maxLowTempTime = actualTempTime + tl.maxTime;                    
                        Serial.print("\nmaxLowTempTimeCorrigido: ");
                        Serial.println(maxLowTempTime);
                    }
                    highTemp = false; 
                    lowTemp = true;
                }else{
                    OilTemperature -= 10;
                    Serial.println("resfriando o oleo...");                
                    digitalWrite(SSR, LOW); //aquece o oleo
                }                             
            }          
        } 
        t = rtc.getTime();
        actualTempTime = (t.date * 24 * 3600) + (t.hour * 360) + (t.min * 60) + t.sec;    
>>>>>>> d8899fa... Iniicio do  Cliclico bugadaco
    }
}

void treatstTimer1interruption()
{
    noInterrupts();  
     
    heatOil();
    
<<<<<<< HEAD
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
                SensorsLogSDCard(numberOfSensors);
            }
        }
    }
    interrupts();
}

=======
//    SensorsLogSerial(numberOfSensors, sensorCSPINs);
    
//    if(logging)
//    {
//        Time t;
//        t = rtc.getTime();
//        actualTime = t.min;
//
//        if(actualTime == nextLoggingTime)
//        {
//            lcd.print("Salvando...");
//            updateLoggingTime();
//            lcd.clear();
//            copy(rtc.getDateStr(FORMAT_SHORT, FORMAT_LITTLEENDIAN, '/'), strDate, 11);
//            copy(rtc.getTimeStr(FORMAT_LONG), strTime, 9);
//
//            if(sdcardOK)
//            {
//                SensorsLogSDCard(numberOfSensors);
//            }
//        }
//    }
    interrupts();
}

>>>>>>> d8899fa... Iniicio do  Cliclico bugadaco
void setup()
{  
     Serial.begin(115200);
     
    //Configura os pinos de seleção de sensores como saida em nivel alto.
    for(uint8_t i = 0; i < numberOfSensors; ++i){
        pinMode(sensorCSPINs[i], OUTPUT);
        digitalWrite(sensorCSPINs[i], HIGH);
        Serial.print("Pino ");
        Serial.print(sensorCSPINs[i]);
        Serial.print(":");
        Serial.println(digitalRead(sensorCSPINs[i]));
    }
<<<<<<< HEAD
    delay(500);
=======
    delay(100);
>>>>>>> d8899fa... Iniicio do  Cliclico bugadaco
    
    lcd.begin(40, 2);
    lcd.clear();
    lcd.print("Initialize the SD");
    
    SPI.begin();    
<<<<<<< HEAD
    delay(500);
=======
    delay(300);
>>>>>>> d8899fa... Iniicio do  Cliclico bugadaco
    //Initialize the SD.
    if(!sd.begin(SD_CONFIG)) {
        sd.initErrorHalt(&Serial);
        sdcardOK = false;
        lcd.print("Falha no SD");
        delay(200);
    }else {
        sdcardOK = true;        
    }
    delay(200);


    Serial.println(digitalRead(SS));
                                
    pinMode(SSR, OUTPUT);
    digitalWrite(SSR, LOW);
  
    initRTC();
    initMenu();
    
    lcd.clear();
    lcd.print("Unioeste-CSC");
        
    Timer1.initialize(5000000); // Inicializa o Timer1 e configura para um período de 5.0 segundos
    Timer1.attachInterrupt(treatstTimer1interruption);

    //getTempCondiction(&tl);
    delay(1000);
    interrupts();
}

void loop()
{
    checkButtons();
    updateLcd();
<<<<<<< HEAD
       
=======
   
    
>>>>>>> d8899fa... Iniicio do  Cliclico bugadaco
    //readAmbientTemp(); TODO
}
