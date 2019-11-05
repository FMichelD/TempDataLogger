#include "DateTime.h"

extern HardwareSerial Serial;

uint16_t year = 2019;
uint8_t month = 1;
uint8_t dayOfMonth = 1;
uint8_t dayOfWeek = 0;
char * dayOfWeekText = (char*)"Domingo";
uint8_t hour = 13;
uint8_t minute = 45;

void setDayOfWeekText() {
    switch (dayOfWeek) {
    case Sunday:
        dayOfWeekText = (char*)"Domingo";
        break;

    case Monday:
        dayOfWeekText = (char*)"Segunda-feira";
        break;

    case Tuesday:
        dayOfWeekText = (char*)"Terca-feira";
        break;

    case Wednesday:
        dayOfWeekText = (char*)"Quarta-feira";
        break;

    case Thursday:
        dayOfWeekText = (char*)"Quinta-feira";
        break;

    case Friday:
        dayOfWeekText = (char*)"Sexta-feira";
        break;

    case Saturday:
        dayOfWeekText = (char*)"Sabado";
        break;
    }
}

void yearUp() {
    year++;
}

void yearDown() {
    year--;
}

void monthUp() {
    if (month < 12)
        month++;
    else
        month = 1;
}

void monthDown() {
    if (month > 1)
        month--;
    else
        month = 12;
}

void dayOfMonthUp() {
    Serial.print("Ajustando dia do mes UP ...");
    if (month == 2) {
        if ((((year % 400) == 0) || (((year % 4) == 0) && ((year % 100) != 0)))) {
            if (dayOfMonth < 29)
                dayOfMonth++;
        } else {
            if (dayOfMonth < 28)
                dayOfMonth++;
            else
                dayOfMonth = 1;
        }
    }

    if( (month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12) ){
        if (dayOfMonth < 31)
            dayOfMonth++;
        else
            dayOfMonth = 1;
    }

    if( (month == 4) || (month == 6) || (month == 9) || (month == 12) ){
        if (dayOfMonth < 30)
            dayOfMonth++;
        else
            dayOfMonth = 1;
    }
    Serial.println(dayOfMonth);
}

void dayOfMonthDown() {
    Serial.print("Ajustando dia do mes DOWN ...");
    if(month == 2){
        if ( (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)) ){
            if (dayOfMonth > 1)
                dayOfMonth--;
            else
                dayOfMonth = 29;
        }else{
            if (dayOfMonth > 1)
                dayOfMonth--;
            else
                dayOfMonth = 28;
        }
    }

    if( (month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12) ){
        if (dayOfMonth > 1)
            dayOfMonth--;
        else
            dayOfMonth = 31;
    }

    if( (month == 4) || (month == 6) || (month == 9) || (month == 12) ){
        if (dayOfMonth > 1)
            dayOfMonth--;
        else
            dayOfMonth = 30;
    }
    Serial.println(dayOfMonth);
}

void hourUp() {
    if (hour < 23)
        hour++;
    else
        hour = 0;
}

void hourDown() {
    if (hour > 0)
        hour--;
    else
        hour = 23;
}

void minuteUp() {
    if (minute < 59)
        minute++;
    else
        minute = 0;
}

void minuteDown() {
    if (minute > 0)
        minute--;
    else
        minute = 59;
}

void dayOfWeekUp() {
    //Sunday = 0; Monday = 1; ... ; Saturday = 6;
    if (dayOfWeek < Saturday)
        dayOfWeek++;
    else
        dayOfWeek = Sunday;

    setDayOfWeekText();
}

void dayOfWeekDown() {
    //Sunday = 0; Monday = 1; ... ; Saturday = 6;
    if (dayOfWeek > Sunday) 
        dayOfWeek--;
    else
        dayOfWeek = Saturday;

    setDayOfWeekText();
}
