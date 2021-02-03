#include "ClockFace.h"

ClockFace::ClockFace(LiquidCrystal lcd)
{
    _lcd = &lcd;
    // for this constructor we expect an lcd that hasn't been started
    _lcd->begin(16,2);
    Wire.begin();
    highlightedView = none;
}

void ClockFace::showFace(){
    //clear the lcd
    _lcd->clear();

    //print the colons
    printColons();

    //print the dateSeparators
    printDateSeparators();

    //print the hour
    printHour();

    //print the minute
    printMinute();

    //print the second
    printSecond();

    //if 12-hour mode, print the half-day
    if(h12){
        printAMPM();
    }

    //print the day
    printDay();

    //print the month
    printMonth();

    //print the year
    printYear();

    //print the day of the week
    printDow();
}

void ClockFace::showHold(){
    _lcd->clear();
    
    _lcd->setCursor(4,0);
    _lcd->print("Hold...");

    _lcd->setCursor(7,1);
    _lcd->print("3");
}

void ClockFace::tick() {
    
}


ViewSelector& operator++(ViewSelector &orig)
{
    orig = static_cast<ViewSelector>(orig + 1);
    if (orig > h12)
    {
        //none and ampm are not part of the rotation
        orig = hour;
    }

    return (orig);
}

//strcpy_P(buffer, (char *)pgm_read_word(&(daysOfWeek[dow])));