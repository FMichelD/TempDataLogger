#include "SDCardmanager.h"

uint8_t sdcardOK = false;
char line[20];

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
    str = strtok(str, ",");
    if (!str) 
        return false;

    // Print text field.
    tl->temperature = atoi(str);
    //Serial.println(str);

    // Subsequent calls to strtok expects a null pointer.
    str = strtok(nullptr, ",");
    if (!str) 
        return false;

    // Convert string to long integer.
    int32_t i32 = strtol(str, &ptr, 0);
    if (str == ptr || *skipSpace(ptr)) 
        return false;
    //Serial.println(i32);
    tl->minTime = atoi(str);

    str = strtok(nullptr, ",");
    if (!str) 
        return false;

    // strtoul accepts a leading minus with unexpected results.
    if (*skipSpace(str) == '-') 
        return false;

    // Convert string to unsigned long integer.
    uint32_t u32 = strtoul(str, &ptr, 0);
    if (str == ptr || *skipSpace(ptr))
        return false;
    //Serial.println(u32);
    tl->maxTime = atoi(str);

    // Check for extra fields  
    return strtok(nullptr, ",") == nullptr;
}

void test1(void)
{
    // Remove any existing file.
    if (sd.exists("ReadCsvDemo.csv")) {
        sd.remove("ReadCsvDemo.csv"); 
    }
    
    // Create the file.
    if (!file.open("ReadCsvDemo.csv", FILE_WRITE)) {
        error("open failed");
    }
    
    // Write test data.
    file.print(F(
        "100,45,50\r\n"
        "25,80,120\r\n"
        "80,80,150"));
      
    // Rewind file for read.
    file.rewind();

    TemperatureLimits tl;

    static int currentCicleLine = 0;
    int countLine = 0;    
    while (file.available()) {    
        int n = file.fgets(line, sizeof(line));
        if (n <= 0) {
            error("fgets failed"); 
        }
        if (line[n-1] != '\n' && n == (sizeof(line) - 1))
        {
            error("line too long");
        }
        if (currentCicleLine == countLine)
        {
            Serial.println();
            Serial.println(line);
            Serial.println();
            //parseLine(line, &tl);

            
            if (!parseLine(line, &tl))
            {
                error("parseLine failed");
            }
            Serial.println(tl.temperature);
            Serial.println(tl.minTime);
            Serial.println(tl.maxTime);
            Serial.println();
            
            currentCicleLine++;
            break;
        }
        countLine++;
    }
        
    file.close();
    Serial.println(F("Done"));    
}
