#include "Button.h"
#include "LCD_Menu.h"
#include "rtc.h"
#include "DateTime.h"
#include "SDCardManager.h"
#include "ThermoparK.h"
//#include "Ciclico.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include <DS1307.h>
#include <TimerOne.h>
#include <avr/wdt.h>


uint8_t numUnusedPins = 42;
const int unusedPins[] = { 40, 41, 42, 43, 44,
                           45, 46, 47, 48, 49,
                           39, 38, 37, 36, 35,

                           14, 15, 
                           16, 17, 18, 19, 28,
                           29, 30, 32,  53, 
                           
                           A0, A1, A2, A3, A4,
                           A5, A6, A7, A9,
                           A10, A11, A12, A13,
                           A14, A15, 10,                          
                           };


const int8_t numberOfSensors = 3;
//uint8_t sensorCSPINs[numberOfSensors]{ 40, 41, 42, 43, 44,
//                                       45, 46, 47, 48, 49,
//                                       39, 38, 37, 36, 35, 
//                                       34, 33, 31};

uint8_t sensorCSPINs[numberOfSensors]{34, 33, 31};   
                                    
ThermoparK thermoparK;
double temperature[numberOfSensors];

#define OIL_TEMP_01 temperature[0]
#define OIL_TEMP_02 temperature[1]

//Extern declared variables
extern uint8_t logging;
extern uint8_t loggingInterval;
extern uint8_t actualTime;
extern uint8_t nextLoggingTime;

TemperatureLimits tl;
uint32_t actualTempTime;
//uint32_t minTempTime; 
//uint32_t maxTempTime;

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
const byte CoolerPin01 = 8;
const byte CoolerPin02 = 9;
bool NormalOperation = true;


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

void SensorsLogSDCard(uint8_t numberOfSensors)
{    
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
}

void getTemperatures(void)
{
    for(int8_t i = 0; i < numberOfSensors; i++)
    {
        temperature[i] = static_cast<double>(thermoparK.readCelcius(sensorCSPINs[i]));
    }
}

void SensorsLogSerial()
{    
    for(int8_t i = 0; i < numberOfSensors; i++) {
              
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
    }
    Serial.println();
    Serial.println(digitalRead(SS));
}

void updateLoggingTime(){
    nextLoggingTime += loggingInterval;
    nextLoggingTime = nextLoggingTime % 60;
}

void updateLcd()
{
    copy(rtc.getDateStr(FORMAT_SHORT, FORMAT_LITTLEENDIAN, '/'), strDate, 11);
    copy(rtc.getTimeStr(FORMAT_LONG), strTime, 9);
    systemMenu.update();
}

bool oilHeaterFail(void)
{
    if( isnan(OIL_TEMP_01) || OIL_TEMP_01 < 10 || OIL_TEMP_01 > 150)
    {
        return true;
    }
    
    if( isnan(OIL_TEMP_02) || OIL_TEMP_02 < 10 || OIL_TEMP_02 > 150)
    {
        return true;
    }

    return false;
}

void heaterOil(void)
{
    digitalWrite(SSR, HIGH);//Liga Resistencia do oleo
    digitalWrite(CoolerPin01, HIGH);//Desliga ventoinha
    digitalWrite(CoolerPin02, HIGH);//Desliga ventoinha
}


void heatOil(void)
{   
    extern uint8_t ledLogState;
    bool logging = digitalRead(ledLogState);
    float OilTemperature;
    bool heater;
    static bool highTemp = false;
    static bool lowTemp = false;
    static uint32_t maxHighTempTime = ULONG_MAX;
    static uint32_t minLowTempTime = ULONG_MAX;
    static uint32_t maxLowTempTime = ULONG_MAX;

    getTemperatures();
    
    OilTemperature = max(OIL_TEMP_01, OIL_TEMP_02);
    
    if(oilHeaterFail())
    {
        heater == false;
        digitalWrite(SSR, LOW); //Desliga Resistencia oleo
        digitalWrite(CoolerPin01, LOW);//liga ventinhas
        digitalWrite(CoolerPin02, LOW);//liga ventinhas

        NormalOperation = false;
        Serial.print("\n\n \t\t *****Erro de leitura nos sensores de oleo*****\n\n");
    }

    if(logging)
    {
        if(TempFixed)
        {
            if(OilTemperature < 85)
            {
                heaterOil();
            }else{
                digitalWrite(SSR, LOW); //Desliga Resistencia oleo
                digitalWrite(CoolerPin01, LOW);//liga ventinhas
                digitalWrite(CoolerPin02, LOW);//liga ventinhas
            }
        }else{            
            Serial.print("Oil temp: ");
            Serial.print(OilTemperature);                       
            Serial.print("   TemperatureTarget: ");
            Serial.print(tl.temperature);
            
            if(tl.heater)
            {
                if(maxHighTempTime != ULONG_MAX)
                {
                    Serial.print("   maxHighTempTime: ");
                    Serial.println(maxHighTempTime - actualTempTime);
                }else{
                  Serial.println();
                }
                
                Serial.println("Aquencendo Ventoinha desligada\n");
                digitalWrite(CoolerPin01, HIGH);//Desliga ventoinha
                digitalWrite(CoolerPin02, HIGH);//Desliga ventoinha
            
                if(OilTemperature < tl.temperature)
                {
                    digitalWrite(SSR, HIGH); //liga a resistencia do oleo
                }
                                
                if(OilTemperature >= tl.temperature)
                {
                    digitalWrite(SSR, LOW); //desliga a resistencia do oleo
                    
                    if(!highTemp)
                    {                     
                        actualTempTime = getAcumulatedSecs();                                    
                        maxHighTempTime = actualTempTime + tl.maxTime;
    
                        Serial.print("\nmaxTempTimeCorrigido: ");
                        Serial.println(maxHighTempTime);
                    }
                    highTemp = true; 
                    lowTemp = false; 
                }
            }

            if(highTemp && actualTempTime >= maxHighTempTime)
            {
                getNewTempCondiction(&tl);
                tl.heater = false;  
                minLowTempTime = actualTempTime + tl.minTime;
                maxHighTempTime = ULONG_MAX;
            }
            
            if(!tl.heater)
            {                                                
                if(OilTemperature >= tl.temperature)
                {   
                    digitalWrite(SSR, LOW); //desliga a resistencia do oleo
                }
                                         
                if(OilTemperature < tl.temperature || actualTempTime > minLowTempTime)
                {                   
                    if(!lowTemp)
                    {   
                        actualTempTime = getAcumulatedSecs();                                    
                        maxLowTempTime = actualTempTime + tl.maxTime;
    
                        Serial.print("\nmaxLowTempTimeCorrigido: ");
                        Serial.println(maxLowTempTime - actualTempTime);
                    }else{
                        if(maxLowTempTime != ULONG_MAX)
                        {
                            Serial.print("   maxLowTempTime: ");
                            Serial.println(maxLowTempTime - actualTempTime);
                        }else{
                            Serial.println();
                        }
                    }                    
                    highTemp = false; 
                    lowTemp = true; 
                }else{
                    if(minLowTempTime != ULONG_MAX)
                    {
                        Serial.print("   minLowTempTime: ");
                        Serial.println(minLowTempTime - actualTempTime);
                    }else{
                        Serial.println();
                    }
                }
                
                Serial.println("Resfriando Ventoinha ligada");
                digitalWrite(CoolerPin01, LOW);//Liga ventoinha  
                digitalWrite(CoolerPin02, LOW);//Liga ventoinha               
            }
           
            if(lowTemp && actualTempTime >= maxLowTempTime)
            {
                if(!getNewTempCondiction(&tl))
                {
                    tl.heater = false;
                }
                maxLowTempTime = ULONG_MAX;
                minLowTempTime = ULONG_MAX;  
            }
        } 
        actualTempTime = getAcumulatedSecs();    
    }
    wdt_reset();
}

void sdCardLogging()
{
    if(logging)
    {
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
}

void serialLogging(void)
{
     #define LOG_TIME 5
     static uint8_t count = 0;
     
    // A cada LOG_TIME interrupções envia os dados dos
    // sensores para porta serial.
    if(count % LOG_TIME == 0)
    {
        SensorsLogSerial();
    }
    
    count++;
    
    if(count > 250)
    {
       count = 0;
    }
}

void treatstTimer1interruption()
{   
    noInterrupts();  

    if(!NormalOperation)
    {
        lcd.print("Erro!!");
        digitalWrite(SSR, LOW); //desliga a resistencia do oleo
        digitalWrite(CoolerPin01, LOW);//Liga ventoinha  
        digitalWrite(CoolerPin02, LOW);//Liga ventoinha    

        wdt_reset();
    }else{  
        updateLcd();
        heatOil();    
        serialLogging();        
        sdCardLogging();        
        wdt_reset();
    }
    interrupts();
}

void setup()
{  
     Serial.begin(115200);
         //Configura os pinos não usados como saida digital em nivel alto.
    for(uint8_t i = 0; i < numUnusedPins; ++i){
        pinMode(unusedPins[i], OUTPUT);
        digitalWrite(unusedPins[i], HIGH);
        Serial.print("Não usado ");
        Serial.print(unusedPins[i]);
        Serial.print(":");
        Serial.println(digitalRead(unusedPins[i]));
    }
    delay(200);
     
    //Configura os pinos de seleção de sensores como saida em nivel alto.
    for(uint8_t i = 0; i < numberOfSensors; ++i){
        pinMode(sensorCSPINs[i], OUTPUT);
        digitalWrite(sensorCSPINs[i], HIGH);
        Serial.print("Pino ");
        Serial.print(sensorCSPINs[i]);
        Serial.print(":");
        Serial.println(digitalRead(sensorCSPINs[i]));
    }



    //Configura a saida para a ventoinha.
    pinMode(CoolerPin01, OUTPUT);
    digitalWrite(CoolerPin01, HIGH);
    pinMode(CoolerPin02, OUTPUT);
    digitalWrite(CoolerPin02, HIGH);

    //Configura a saida para o Rele de Estado Sólido.
    pinMode(SSR, OUTPUT);
    digitalWrite(SSR, LOW);
    delay(100);

    //Configura o display.
    lcd.begin(40, 2);
    lcd.clear();
    lcd.print("Initialize the SD");
    
    SPI.begin();
    delay(100);

    //Initialize the SD.
    if(!sd.begin(SD_CONFIG)) {
        sd.initErrorHalt(&Serial);
        sdcardOK = false;
    }else {
        sdcardOK = true;        
    }
    delay(100);
    Serial.println(digitalRead(SS));
                                 
    initRTC();
    initMenu();
    
    lcd.clear();
    lcd.print("Unioeste-CSC");
        
    Timer1.initialize(2000000); // Inicializa o Timer1 e configura para um período de 1.0 segundos
    Timer1.attachInterrupt(treatstTimer1interruption);

    //getNewTempCondiction(&tl);
    delay(500);

    wdt_enable(WDTO_8S);

    if( (MCUSR >> 3) & 1 == 1)
    {
        lcd.print("Wdt Rst");
        Serial.println("\n\n***** Houve reset por estouro do watchdog *****");
    }
    interrupts();
}

void loop()
{
    checkButtons(); 
    wdt_reset();   
}
