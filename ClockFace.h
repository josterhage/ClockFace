#ifndef CLOCKFACE_H
#define CLOCKFACE_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <DS3231.h>
#include <LiquidCrystal.h>
#include <Wire.h>

enum ViewSelector
{
    none = 0,
    hour = 1,
    minute = 2,
    second = 3,
    day = 4,
    month = 5,
    year = 6,
    dow = 7,
    h12 = 8,
    ampm = 9
};

ViewSelector &operator++(ViewSelector &orig);

enum DayOfWeek
{
    Sunday = 1,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

const char sun[] PROGMEM = "SUN";
const char mon[] PROGMEM = "MON";
const char tue[] PROGMEM = "TUE";
const char wed[] PROGMEM = "WED";
const char thu[] PROGMEM = "THU";
const char fri[] PROGMEM = "FRI";
const char sat[] PROGMEM = "SAT";

const char *const daysOfWeek[] PROGMEM = {
    sun, mon, tue, wed, thu, fri, sat
};

//not implemented
// enum MonthFormat
// {
//     m,  //two-digit
//     mm, //three-letter
// };

// const char jan[] PROGMEM = "JAN";
// const char feb[] PROGMEM = "FEB";
// const char mar[] PROGMEM = "MAR";
// const char apr[] PROGMEM = "APR";
// const char may[] PROGMEM = "MAY";
// const char jun[] PROGMEM = "JUN";
// const char jul[] PROGMEM = "JUL";
// const char aug[] PROGMEM = "AUG";
// const char sep[] PROGMEM = "SEP";
// const char oct[] PROGMEM = "OCT";
// const char nov[] PROGMEM = "NOV";
// const char dec[] PROGMEM = "DEC";

// const char *const months[] PROGMEM = {
//     jan, feb, mar, apr,
//     may, jun, jul, aug,
//     sep, oct, nov, dec
// }

const uint8_t daysInMonth[] PROGMEM = {
    31, 28, 31, 30,
    31, 30, 31, 31,
    30, 31, 30, 31
};

//positions
//assumes 16x2 lcd
#define T_ROW 0
#define D_ROW 1

#define H_COL 0
#define M_COL 3
#define S_COL 6
#define AP_COL 9

class ClockFace
{
public:
    ClockFace(LiquidCrystal);

    void showFace();

    void showHold();

    void tick(bool tock);

    void incrementHour();

    void incrementMinute();

    void zeroSecond();

    void incrementDay();

    void incrementMonth();

    void incrementYear();

    void incrementDoW();

    void flipH12();

    void highlightView(ViewSelector view);

    void setDateSeparator(char separator);

// not implemented
    // void setMonthFormat(MonthFormat fmt);

    uint16_t getMillisAlignment();

private:
    void printColons();
    void hideColons();

    void printDateSeparators();

    void printHour();
    void printMinute();
    void printSecond();
    void printAMPM();

    void printDay();
    void printMonth();
    void printYear();
    void printDoW();

    void hideHighlighted();
    void hideAMPM();

    DS3231 clock;

    LiquidCrystal *lcd;

    ViewSelector highlightedView;

    bool isH12;

    bool faceShowing;

    char dateSeparator;
};

#endif