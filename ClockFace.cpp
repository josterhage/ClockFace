#include "ClockFace.h"

ClockFace::ClockFace(LiquidCrystal liquidCrystal)
{
    lcd = &liquidCrystal;
    // for this constructor we expect an lcd that hasn't been started
    lcd->begin(16, 2);
    Wire.begin();
    highlightedView = none;
}

void ClockFace::showFace()
{
    faceShowing = true;
    //clear the lcd
    lcd->clear();

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

    bool discard;
    clock.getHour(isH12,discard);
    if (isH12)
    {
        printAMPM();
    }

    //print the day
    printDay();

    //print the month
    printMonth();

    //print the year
    printYear();

    //print the day of the week
    printDoW();
}

void ClockFace::showHold()
{
    faceShowing = false;
    lcd->clear();

    lcd->setCursor(4, 0);
    lcd->print("Hold...");

    lcd->setCursor(7, 1);
    lcd->print("3");
}

void ClockFace::tick(bool tock)
{
    bool _h12, _pm, _century;
    if (faceShowing)
    {
        if (!tock)
        {
            printColons();
            if (clock.getSecond() == 0)
            {
                if (clock.getMinute() == 0)
                {
                    if (clock.getHour(_h12, _pm) == 0)
                    {
                        if (clock.getDate() == 1)
                        {
                            if (clock.getMonth(_century) == 1)
                            {
                                printYear();
                            }
                            printMonth();
                        }
                        printDay();
                        printDoW();
                    }
                    printHour();
                }
                printMinute();
            }
            printSecond();
        }
        else
        {
            hideColons();
            hideHighlighted;
        }
    }
}

void ClockFace::incrementHour()
{
    bool _h12, _pm;
    uint8_t value = clock.getHour(_h12, _pm);
    value++;
    if (value == 24)
    {
        value = 0;
    }
    clock.setHour(value);
    printHour();
}

void ClockFace::incrementMinute()
{
    uint8_t value = clock.getMinute();
    value++;
    if (value == 60)
    {
        value = 0;
    }
    clock.setMinute(value);
    printMinute();
}

// programmers should probably call getMillisAlignment() after this
void ClockFace::zeroSecond()
{
    clock.setSecond(0);
    printSecond();
}

void ClockFace::incrementDay()
{
    bool century;
    uint8_t value = clock.getDate();
    uint8_t month = clock.getMonth(century);
    uint8_t year = clock.getYear();
    if (month != 2)
    {
        uint8_t days = pgm_read_byte_near(daysInMonth[month]);
    }
    else
    {
        if (year % 4 == 0)
        {
            days = 29;
        }
        else
        {
            days = 28;
        }
    }
    if (value == days)
    {
        value = 1;
    }
    else
    {
        value++;
    }
    clock.setDate(value);
    printDay();
}

void ClockFace::incrementMonth(){
    bool century;
    uint8_t value = clock.getMonth(century);
    if(value == 12){
        value = 1;
    } else {
        value++;
    }
    clock.setMonth(value);
    printMonth();
}

void ClockFace::incrementYear(){
    uint8_t value = clock.getYear();
    value++;
    clock.setYear(value);
    printYear();
}

void ClockFace::incrementDoW(){
    uint8_t value = clock.getDoW();
    if(value == 7){
        value = 1;
    } else {
        value++;
    }
    clock.setDoW(value);
    printDoW();
}

void ClockFace::flipH12(){
    isH12 = ! isH12;
    clock.setClockMode(isH12);
    if(isH12){
        printAMPM();
    } else {
        hideAMPM();
    }
}

void ClockFace::highlightView(ViewSelector view){
    highlightedView = view;
}

void ClockFace::setDateSeparator(char separator){
    dateSeparator = separator;
    printDateSeparators();
}

// not implemented
// void ClockFace::setMonthFormat(MonthFormat fmt){

// }

uint16_t ClockFace::getMillisAlignment(){
    uint8_t s = clock.getSecond();
    while(s = clock.getSecond());
    return (uint16_t)(millis() % 1000);
}

// private functions
void ClockFace::printColons(){
    lcd->setCursor(M_COL-1,T_ROW);
    lcd->print(":");
    lcd->setCursor(S_COL-1,T_ROW);
    lcd->print(":");
}

void ClockFace::hideColons(){
    lcd->setCursor(M_COL-1,T_ROW);
    lcd->print(" ");
    lcd->setCursor(S_COL-1,T_ROW);
    lcd->print(" ");
}

void ClockFace::printDateSeparators(){
    lcd->setCursor(M_COL-1,D_ROW);
    lcd->print(dateSeparator);
    lcd->setCursor(S_COL-1,D_ROW);
    lcd->print(dateSeparator);
}

void ClockFace::printHour(){
    char digits[2];
    bool h12,pm;
    uint8_t value = clock.getHour(h12,pm);
    sprintf(digits,"%02d",value);
    lcd->setCursor(H_COL,T_ROW);
    lcd->print(digits);
}

void ClockFace::printMinute(){
    uint8_t value = clock.getMinute();
    sprintf(digits,"%02d",value);
    lcd->setCursor(M_COL,T_ROW);
    lcd->print(digits);
}

void ClockFace::printSecond(){
    uint8_t value = clock.getSecond();
    sprintf(digits,"%02d",value);
    lcd->setCursor(S_COL,T_ROW);
    lcd->print(digits);
}

void ClockFace::printAMPM(){
    bool h12,pm;
    uint8_t value = clock.getHour(h12,pm);
    lcd->setCursor(AP_COL,T_ROW);
    lcd->print(pm ? "PM" : "AM");
}

void ClockFace::printDay(){
    char digits[2];
    uint8_t value = clock.getDate();
    sprintf(digits,"%02d",value);
    lcd->setCursor(H_COL,D_ROW);
    lcd->print(digits);
}

void ClockFace::printMonth(){
    char digits[2];
    bool century;
    uint8_t value = clock.getMonth(century);
    sprintf(digits,"%02d",value);
    lcd->setCursor(M_COL,D_ROW);
    lcd->print(digits);
}

void ClockFace::printYear(){
    char digits[2];
    uint8_t value = clock.getYear();
    sprintf(digits,"%02d",value);
    lcd->setCursor(S_COL,D_ROW);
    lcd->print(digits);
}

void ClockFace::printDoW(){
    char day[5];
    uint8_t value = clock.getDoW();
    strcpy_P(day,(char *)pgm_read_word(&(daysOfWeek[value])));
    lcd->setCursor(AP_COL,D_ROW);
    lcd->print(day);
}

void ClockFace::hideHighlighted(){
    switch(highlightedView){
        case hour:
            lcd->setCursor(H_COL,T_ROW);
            break;
        case minute:
            lcd->setCursor(M_COL,T_ROW);
            break;
        case second:
            lcd->setCursor(S_COL,T_ROW);
            break;
        case day:
            lcd->setCursor(H_COL,D_ROW);
            break;
        case month:
            lcd->setCursor(M_COL,D_ROW);
            break;
        case year:
            lcd->setCursor(S_COL,D_ROW);
            break;
        case dow:
            lcd->setCursor(AP_COL,D_ROW);
            lcd->print("   ");
            return;
        case ampm:
            lcd->setCursor(AP_COL,T_ROW);
            break;
        default:
            return;
    }
    lcd->print("  ");
}

void ClockFace::hideAMPM(){
    lcd->setCursor(AP_COL,T_ROW);
    lcd->print("  ");
}

ViewSelector &operator++(ViewSelector &orig)
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