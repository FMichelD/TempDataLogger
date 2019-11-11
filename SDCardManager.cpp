#include "SDCardmanager.h"

uint8_t sdcardOK = false;
char line[15];

void initSDCard(void)
{
    //Initialize the SD.
    if(!sd.begin(SD_CONFIG)) {
        sd.initErrorHalt(&Serial);
        sdcardOK = false;        
        delay(200);
    }else {
        sdcardOK = true;        
    }
    delay(200);
    Serial.println(digitalRead(SS));  
}
    

// Check for extra characters in field or find minus sign.
char* skipSpace(char* str) {
  while (isspace(*str)) str++;
  return str;
}

bool parseLine(char* str, TemperatureLimits *tl)
{
    char* ptr;

    // Set strtok start of line.
    str = strtok(str, ";");
    if (!str) 
        return false;
    tl->heater = atoi(str);


    // Subsequent calls to strtok expects a null pointer.
    str = strtok(nullptr, ";");
    if (!str) 
        return false;
    tl->temperature = atoi(str);


    str = strtok(nullptr, ";");
    if (!str) 
        return false;
    tl->minTime = atoi(str);

    
    str = strtok(nullptr, ";");
    if (!str) 
        return false;
    tl->maxTime = atoi(str);


    // Check for extra fields  
    return strtok(nullptr, ";") == nullptr;
}

int getNewTempCondiction(TemperatureLimits *tl)
{
    // Create the file.
    if (sd.exists("TempCicle.csv"))
    {
        Serial.println("Lendo arquivo: TempCicle.csv");
        if (!file.open("TempCicle.csv", FILE_READ)) 
        {
            Serial.println("open file failed");
            return 0;
        }
    }else{
        Serial.println("O arquivo não existe!");
        return 0;
    }
    
    //file.rewind();
    
    static int currentCicleLine = 0;
    int countLine = 0;    
    while (file.available()) {    
        int n = file.fgets(line, sizeof(line));
        if (n <= 0) {
            Serial.println("fgets failed");
            return 0; 
        }
        if (line[n-1] != '\n' && n == (sizeof(line) - 1))
        {
            Serial.println("line too long");
            return 0;
        }
        if (currentCicleLine == countLine)
        {                       
            if (!parseLine(line, tl))
            {
                Serial.println("parseLine failed");
                return 0;
            }
            Serial.println("Dados do arquivo de temperatura");
            Serial.println(tl->heater ? "aquecer" : "resfriar");
            Serial.println(tl->temperature);
            Serial.println(tl->minTime);
            Serial.println(tl->maxTime);
            
            currentCicleLine++;
            
            file.close();
            
            Serial.println(F("Done"));
            return 1;
        }
        countLine++;
    }
        
    file.close();
    Serial.println(F("Done")); 
    return 0;   
}
