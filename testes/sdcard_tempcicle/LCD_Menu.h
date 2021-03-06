#ifndef LCD_MENU_H
#define LCD_MENU_H
#pragma once

#include "LCDManager.h"
#include "Button.h"
#include "DateTime.h"
#include "rtc.h"

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <DS1307.h>
#include <stdint.h>

//Pin 12 as led state for logging
static uint8_t ledLogState = 12;
static uint8_t dateTimveSalvable = false;

static bool TempFixed = true;
static bool TempCicled = !TempFixed;

uint8_t logging = false;
uint8_t loggingInterval = 1;
uint8_t actualTime;
uint8_t nextLoggingTime;

extern HardwareSerial Serial;
extern Time time;
extern DS1307 rtc;

extern char* strTime;
extern char* strDate;
extern String SDCardFileName; //ex. 15d05m2019_15h30

// Indicator symbol definition, it will change the default symbol for the right focus.
// http://omerk.github.io/lcdchargen/
static uint8_t chrUpDown[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00000,
    0b00000,
    0b11111,
    0b01110,
    0b00100
};

static uint8_t chrNone[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

extern uint16_t year;
extern uint8_t month;
extern uint8_t dayOfMonth;
extern uint8_t dayOfWeek;
extern char * dayOfWeekText;
extern uint8_t hour;
extern uint8_t minute;

extern char* strDate;
extern char* strTime;

enum keys {
    keyUp = 1,
    keyDown = 2
};

//Button objects instantiation
const bool pullup = true;
static Button leftButton(25, pullup);
static Button rightButton(26, pullup);
static Button upButton(22, pullup);
static Button downButton(23, pullup);
static Button enterButton(24, pullup);
static Button logButton(27, pullup);



bool isTempFixed(void)
{
    return  TempFixed;
}

// A LiquidLine object can be used more that once.
static LiquidLine backLine(11, 1, "Voltar");

//Welcome-Screen
static LiquidLine welcomeLine1(1, 0, "Unioeste-Cscvl");
static LiquidLine welcomeLine2(1, 1, "TempDataLogger");
static LiquidScreen welcomeScreen(welcomeLine1, welcomeLine2);

//Default-Screen
static LiquidLine defaultLine1(3, 0, strDate);
static LiquidLine defaultLine2(3, 1, strTime);
static LiquidScreen defaultScreen(defaultLine1, defaultLine2);

//Date-Time and Logging Interval Menu
static LiquidLine dateLine(1, 0, "Ajustar data-hora");
static LiquidLine loggingLine(1, 1, "Config LOG");
static LiquidScreen dateTimeScreen(dateLine, loggingLine);

static LiquidMenu mainMenu(lcd, defaultScreen, dateTimeScreen, 1);

//Set Date-Time sub-menu
static LiquidLine setYearLine(1, 0, "Ano: ", year);
static LiquidLine setMonthLine(1, 1, "Mes: ", month);
static LiquidScreen setYearMonthScreen(setYearLine, setMonthLine);

static LiquidLine setDayOfMonthLine(1, 0, "Dia do mes: ", dayOfMonth);
static LiquidLine setDayOfWeekLine(1, 1, "Dia da Semana: ", dayOfWeekText);
static LiquidScreen setDaysScreen(setDayOfMonthLine, setDayOfWeekLine);

static LiquidLine setHourLine(1, 0, "Hora: ", hour);
static LiquidLine setMinuteLine(1, 1, "Minuto: ", minute);
static LiquidScreen setHourScreen(setHourLine, setMinuteLine);

static LiquidScreen saveDateTimeScreen(backLine);

//Set logging interval sub-menu
static LiquidLine setLoggingLine(1, 0, "Log invervalo: ", loggingInterval);
static LiquidScreen loggingScreen(setLoggingLine, backLine);

//Set logging interval sub-menu
static LiquidLine setTempFixLine(1, 0, "Temp. Fixo: ", TempFixed);
static LiquidLine setTempCicleLine(1, 1, "Temp Ciclos: ", TempCicled);
static LiquidScreen setTempModeScreen(setLoggingLine, backLine);

//Create System-Menu
static LiquidMenu dateTimeMenu(lcd, setYearMonthScreen, setDaysScreen, setHourScreen, saveDateTimeScreen);
static LiquidMenu loggingMenu(lcd, loggingScreen);
static LiquidMenu temperatureTypeMenu(lcd, loggingScreen);

static LiquidSystem systemMenu(mainMenu, dateTimeMenu, loggingMenu, temperatureTypeMenu);

void gotoDateMenu() {
    dateTimveSalvable = true;
    systemMenu.change_menu(dateTimeMenu);
}

void saveDateTime(){
    Serial.println("Saving Date and Time");
    rtc.setDOW(dayOfWeek);      //Define o dia da semana
    rtc.setTime(hour, minute, 0);     //Define o horario
    rtc.setDate(dayOfMonth, month, year);   //Define o dia, mes e ano
    dateTimveSalvable = false;
}

void goBack() {
    if(dateTimveSalvable)
        saveDateTime();

    // This function takes reference to the wanted menu.
    systemMenu.change_menu(mainMenu);
    systemMenu.change_screen(1);
}

void gotoLogMenu(){
    systemMenu.change_menu(loggingMenu);
}


void upLogInterval(){
    //logging interval in minutes

    if(loggingInterval < 60){
        if(loggingInterval == 1){
            loggingInterval = 5;
        }

        loggingInterval += 5;

    }else{
        loggingInterval = 1;
    }
}

void downLogInterval(){
    //logging interval in minutes

    if(loggingInterval > 5){
        loggingInterval -= 5;
    }else{
        if(loggingInterval == 1){
            loggingInterval = 60;
        }

        if(loggingInterval == 5){
            loggingInterval = 1;
        }
    }
}

void initMenu() {
    // Sets the focus position for this lines to be on the left.
    dateLine.set_focusPosition(Position::LEFT);
    setYearLine.set_focusPosition(Position::LEFT);
    setMonthLine.set_focusPosition(Position::LEFT);
    setDayOfWeekLine.set_focusPosition(Position::LEFT);
    setDayOfMonthLine.set_focusPosition(Position::LEFT);
    setHourLine.set_focusPosition(Position::LEFT);
    setMinuteLine.set_focusPosition(Position::LEFT);

    backLine.set_focusPosition(Position::LEFT);

    loggingLine.set_focusPosition(Position::LEFT);
    setLoggingLine.set_focusPosition(Position::LEFT);

    // Function to attach functions to LiquidLine objects.
    dateLine.attach_function(keyUp, gotoDateMenu);
    dateLine.attach_function(keyDown, gotoDateMenu);

    setYearLine.attach_function(keyUp, yearUp);
    setYearLine.attach_function(keyDown, yearDown);

    setMonthLine.attach_function(keyUp, monthUp);
    setMonthLine.attach_function(keyDown, monthDown);

    setDayOfMonthLine.attach_function(keyUp, dayOfMonthUp);
    setDayOfMonthLine.attach_function(keyDown, dayOfMonthDown);

    setDayOfWeekLine.attach_function(keyUp, dayOfWeekUp);
    setDayOfWeekLine.attach_function(keyDown, dayOfWeekDown);

    setHourLine.attach_function(keyUp, hourUp);
    setHourLine.attach_function(keyDown, hourDown);

    setMinuteLine.attach_function(keyUp, minuteUp);
    setMinuteLine.attach_function(keyDown, minuteDown);

    backLine.attach_function(keyUp, goBack);
    backLine.attach_function(keyDown, goBack);

    loggingLine.attach_function(keyUp, gotoLogMenu);
    loggingLine.attach_function(keyDown, gotoLogMenu);

    setLoggingLine.attach_function(keyUp, upLogInterval);
    setLoggingLine.attach_function(keyDown, downLogInterval);

    // Changes the left focus symbol.
    systemMenu.set_focusSymbol(Position::LEFT, chrUpDown);

    systemMenu.update();

    pinMode(ledLogState, OUTPUT);
    digitalWrite(ledLogState, LOW);
}

void checkButtons(){
    // Check all the buttons
    if(rightButton.check() == LOW) {
        Serial.println("Button RIGHT pressed");
        // Changes the left focus symbol.
        systemMenu.set_focusSymbol(Position::LEFT, chrNone);
        systemMenu.next_screen();
    }
    if(leftButton.check() == LOW) {
        Serial.println("Button LEFT pressed");
        // Changes the left focus symbol.
        systemMenu.set_focusSymbol(Position::LEFT, chrNone);
        systemMenu.previous_screen();
    }
    if(upButton.check() == LOW) {
        Serial.println("Button UP pressed");
        systemMenu.call_function(1);
    }
    if(downButton.check() == LOW) {
        Serial.println("Button DOWN pressed");
        systemMenu.call_function(2);
    }
    if(enterButton.check() == LOW) {
        Serial.println("Button ENTER pressed");
        
        // Changes the left focus symbol.
        systemMenu.set_focusSymbol(Position::LEFT, chrUpDown);
        systemMenu.switch_focus();
    }
    
    if(logButton.check() == LOW){
        Serial.println("Button LOG pressed");
        digitalWrite(ledLogState, !digitalRead(ledLogState));
        logging = !logging;

        Time t;
        t = rtc.getTime();
        actualTime = nextLoggingTime = t.min;

        SDCardFileName = strDate;
        SDCardFileName.concat("Y_");
        SDCardFileName.concat(strTime);
        SDCardFileName.setCharAt(2, 'D');
        SDCardFileName.setCharAt(5, 'M');
        SDCardFileName.setCharAt(12, 'h');
        SDCardFileName.setCharAt(15, 'm');
        SDCardFileName.concat("s.csv");
        Serial.println(SDCardFileName);                
    }
}

# endif //LCD_MENU_H
