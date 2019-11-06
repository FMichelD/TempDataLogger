#include "rtc.h"

DS1307 rtc(20, 21);
Time t;

void initRTC(){
  //Aciona o relogio
  rtc.halt(false);

  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
//  rtc.setDOW(FRIDAY);      //Define o dia da semana
//  rtc.setTime(20, 37, 0);     //Define o horario
//  rtc.setDate(6, 6, 2014);   //Define o dia, mes e ano

  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
}

uint32_t getAcumulatedSecs(void)
{
    t = rtc.getTime();

    uint32_t days = uint32_t(t.date) * 24L * 3600L;    
    uint32_t hours = uint32_t(t.hour) * 3600L;
    uint16_t mins = (t.min * 60);
    uint8_t secs = (t.sec);
    uint32_t acms = days + hours + mins + secs;
    
//    Serial.print("\nDia: ");
//    Serial.print(days);
//    Serial.print("   hora: ");
//    Serial.print(hours);
//    Serial.print("   min: ");
//    Serial.print(mins);
//    Serial.print("   sec: ");
//    Serial.println(secs);
//    
//    Serial.print("Acum Secs: ");
//    Serial.println(acms); 

    return acms;
}
