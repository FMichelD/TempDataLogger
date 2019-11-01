#ifndef DATE_TIME_H
# define DATE_TIME_H
# pragma once

#include <stdint.h>

#include <DS1307.h>

extern uint16_t year;
extern uint8_t month;
extern uint8_t dayOfMonth;
extern uint8_t dayOfWeek;
extern char* dayOfWeekText;
extern uint8_t hour;
extern uint8_t minute;

enum daysOfWeek {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

void setDayOfWeekText();

void yearUp();

void yearDown();

void monthUp();

void monthDown();

void dayOfMonthUp();

void dayOfMonthDown();

void hourUp();

void hourDown();

void minuteUp();

void minuteDown();

void dayOfWeekUp();

void dayOfWeekDown();

# endif //DATE_TIME_H
