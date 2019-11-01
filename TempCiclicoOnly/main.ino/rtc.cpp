#include "rtc.h"

DS1307 rtc(20, 21);

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
