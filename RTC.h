#ifndef RTC_H
#define RTC_H
#pragma once

//Carrega a biblioteca do RTC DS1307
#include <DS1307.h>
 
//Modulo RTC DS1307 ligado as portas 20 e 21 do Arduino Mega
extern DS1307 rtc;
extern Time t;

void initRTC();
uint32_t getAcumulatedSecs(void);

#endif //RTC_H
