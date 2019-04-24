DEFINES += ARDUINO=162

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\tools\avr\avr\include"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal\src"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\LiquidMenu-master\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Bridge\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Esplora\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Ethernet\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Firmata\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\GSM\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\LiquidCrystal\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Robot_Control\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\RobotIRremote\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Robot_Motor\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\SD\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Servo\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\SpacebrewYun\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Stepper\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\Temboo\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\TFT\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\libraries\WiFi\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\EEPROM\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SoftwareSerial\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src"
INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src"

INCLUDEPATH += "C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\mega"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\DS1307-master"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src\common"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src\ExFatLib"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src\FatLib"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src\iostream"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src\SdCard"
INCLUDEPATH += "C:\Users\FMichelD2\Documents\Arduino\libraries\SdFs-master\src\SpiDriver"

INCLUDEPATH += "E:\FMichelD\Workspace\DataLogger"

SOURCES += \
    DataLogger.ino \
    rtc.cpp \
    DateTime.cpp \
    SPISensor.cpp \
    ThermoparK.cpp

HEADERS += \
    Button.h \
    DateTime.h \
    LCD_Menu.h \
    rtc.h \
    sdcardlog.h \
    SPISensor.h \
    ThermoparK.h


