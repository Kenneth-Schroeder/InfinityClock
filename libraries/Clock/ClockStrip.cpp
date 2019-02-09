#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <string.h>
#include "types.h"
#include "LEDcolor.h"
#include "Counter.h"
#include "ComplexCounter.h"
#include "timeCounter.h"
#include "ClockStrip.h"

// setLED setzt beliebige LED (0-65) und beachtet lediglich, dass Änderungen an den KreisLEDs, verdoppelt werden.----------------------------------

void ClockStrip::setLED(byte num, byte red, byte green, byte blue) {
    led_strip.setPixelColor(num, led_strip.Color(green, red, blue));
    if (num >= 6)                           //dann das zusätzlich, damit zwei gleiche Punkte untereinander
        led_strip.setPixelColor(num + 60, led_strip.Color(red, green, blue));
}

void ClockStrip::setLED(byte num, LEDcolor color) {
    led_strip.setPixelColor(num, led_strip.Color(color.green(), color.red(), color.blue()));
    if (num >= 6)                           //dann das zusätzlich, damit zwei gleiche Punkte untereinander
        led_strip.setPixelColor(num + 60, led_strip.Color(color.red(), color.green(), color.blue()));
}

// setClockLED mapped jede Zahl auf Bereich von 0 bis 59 und beachtet bereits Orientierung der Uhr, wenn aufgehangen.
void ClockStrip::setClockLED(int num, byte red, byte green, byte blue) {
    setLED(getPhysicalLEDnum(num), red, green, blue);
}

// setClockLED mit union
void ClockStrip::setClockLED(int num, LEDcolor color) {
    setLED(getPhysicalLEDnum(num), color);
}

int ClockStrip::getPhysicalLEDnum(int num) {  // Bsp.  num = 0
    num = num - 19;                 // num = -19
    if (num <= 0)                   // -19 <= 0
        return 65 + num;              // return 46, also oberste LED, wenn Uhr angehangen
    else
        return (num + 5) % 60;        // Bsp. num = 60 ... num = 41 ... num > 0 ... return 46, also gleich
}

void ClockStrip::resetColorPerLED() {
    for (int i = 0; i < 66; i++) {
        _colorPerLED[i] = { 0,0,0 };
    }
}

void ClockStrip::setAllLEDs() {
    for (int i = 0; i < 6; i++) {
        setLED(i, _colorPerLED[i] * *_brightness);
    }
    for (int i = 6; i < 66; i++) {
        setClockLED(i - 6, _colorPerLED[i] * *_brightness);
    }
}

void ClockStrip::myDelay(unsigned int duration) {
    unsigned long startMillis = millis();
    while (millis() < startMillis + duration) {}
}

// Fill the dots one after the other with a color
void ClockStrip::colorWipe(LEDcolor c, uint8_t wait) {
    for (uint16_t i = 0; i < 60; i++) {
        if (runDemo) {
            setClockLED(i, c);
            led_strip.show();
            myDelay(wait);
        }
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void ClockStrip::rainbowCycle(int reps, uint8_t wait) {
    uint16_t i, j;
    
    for (j = 0; j < 256 * reps; j++) { // 5 cycles of all colors on wheel
        if (runDemo) {
            for (i = 0; i < 60; i++) {
                setClockLED(i, Wheel(((i * 256 / 60) + j) & 255));
            }
            
            led_strip.show();
            myDelay(wait);
        }
    }
}

//Theatre-style crawling lights.
void ClockStrip::theaterChase(LEDcolor c, int reps, uint8_t wait) {
    for (int j = 0; j < reps; j++) {  //do 10 cycles of chasing
        for (int q = 0; q < 3; q++) {
            if (runDemo) {
                for (uint16_t i = 0; i < 60; i = i + 3) {
                    setClockLED(i + q, c);    //turn every third pixel on
                }
                led_strip.show();
                
                myDelay(wait);
                
                for (uint16_t i = 0; i < 60; i = i + 3) {
                    setClockLED(i + q, LEDcolor(0, 0, 0));        //turn every third pixel off
                }
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void ClockStrip::theaterChaseRainbow(uint8_t wait) {
    for (int j = 0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++) {
            if (runDemo) {
                for (uint16_t i = 0; i < 60; i = i + 3) {
                    setClockLED(i + q, Wheel((i + j) % 255));    //turn every third pixel on
                }
                led_strip.show();
                
                myDelay(wait);
                
                for (uint16_t i = 0; i < 60; i = i + 3) {
                    setClockLED(i + q, LEDcolor(0, 0, 0));        //turn every third pixel off
                }
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
LEDcolor ClockStrip::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return LEDcolor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return LEDcolor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return LEDcolor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

ClockStrip::ClockStrip(int datapin, int length, int sck_clock, int mosi_data, struct colorTriple &colors, bool(&shouldDisplay)[7], bool(&alarmState)[2], double &brightness)
:_datapin(datapin),
_length(length),
_sck_clock(sck_clock),
_mosi_data(mosi_data),
_shouldDisplay(&shouldDisplay),
_alarmState(&alarmState),
_colors(&colors),
_brightness(&brightness)
{
    
    led_strip = Adafruit_NeoPixel(_length, _datapin, NEO_GRB + NEO_KHZ800);
    led_strip.begin();
    led_strip.clear();
}

void ClockStrip::showMenuLED(int num, bool secondChanged, int extra) {
    if (secondChanged) {
        num = num % 60;
        resetColorPerLED();
        
        _colorPerLED[num + 6] += _colors->minuteColor;
        
        if (extra != 61) {
            extra = extra % 60;
            _colorPerLED[extra + 6] += _colors->minuteColor;
        }
        
        setAllLEDs();
        led_strip.show();
    }
}

void ClockStrip::showAlarmTime(ComplexCounter alarmTime[3], int blink, int sec, int alarmLED) {
    resetColorPerLED();
    
    if (blink != 0 || sec % 2 == 0) {
        _colorPerLED[alarmTime[0].get() + 6] += _colors->minuteColor;
        _colorPerLED[(alarmTime[0] - 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[(alarmTime[0] + 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[1] = _colors->minuteColor / 8;
    }
    
    if (blink != 1 || sec % 2 == 0) {
        int twelveHourFormat = (alarmTime[1].get() > 11 ? alarmTime[1] - 12 : alarmTime[1].get()) * 5;
        _colorPerLED[twelveHourFormat + 6] += _colors->hourColor;
        _colorPerLED[twelveHourFormat + 5] += _colors->hourColor / 4;
        _colorPerLED[twelveHourFormat + 7] += _colors->hourColor / 4;
        _colorPerLED[0] = _colors->hourColor / 8;
    }
    
    if (blink != 2 || sec % 2 == 0) {
        for (int i = 0; i < alarmTime[2].get(); i++) {
            _colorPerLED[i + 7] += (_colors->secondColor + _colors->minuteColor + _colors->hourColor) / 12;
        }
        _colorPerLED[3] = (_colors->secondColor + _colors->minuteColor + _colors->hourColor) / 12;
    }
    
    _colorPerLED[alarmLED + 6] += _colors->secondColor;
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showAlarmTime(ComplexCounter alarmTime[3], int selection, int alarmLED) {
    resetColorPerLED();
    
    if (selection == 0) {
        _colorPerLED[alarmTime[0].get() + 6] += _colors->minuteColor;
        _colorPerLED[(alarmTime[0] - 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[(alarmTime[0] + 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[1] = _colors->minuteColor / 8;
    }
    
    if (selection == 1) {
        int twelveHourFormat = (alarmTime[1].get() > 11 ? alarmTime[1] - 12 : alarmTime[1].get()) * 5;
        _colorPerLED[twelveHourFormat + 6] += _colors->hourColor;
        _colorPerLED[twelveHourFormat + 5] += _colors->hourColor / 4;
        _colorPerLED[twelveHourFormat + 7] += _colors->hourColor / 4;
        _colorPerLED[0] = _colors->hourColor / 8;
    }
    
    if (selection == 2) {
        for (int i = 0; i < alarmTime[2].get(); i++) {
            _colorPerLED[i + 7] += (_colors->secondColor + _colors->minuteColor + _colors->hourColor) / 12;
        }
        _colorPerLED[3] = (_colors->secondColor + _colors->minuteColor + _colors->hourColor) / 12;
    }
    
    _colorPerLED[alarmLED + 6] += _colors->secondColor;
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showTimer(ComplexCounter timerTime[3], int blink, int sec) {
    resetColorPerLED();
    
    if (blink != 0 || sec % 2 == 0) {
        _colorPerLED[timerTime[0].get() + 6] += _colors->secondColor;
        _colorPerLED[(timerTime[0] - 1) + 6] += _colors->secondColor / 4;
        _colorPerLED[(timerTime[0] + 1) + 6] += _colors->secondColor / 4;
        _colorPerLED[2] = _colors->secondColor / 8;
    }
    
    if (blink != 1 || sec % 2 == 0) {
        _colorPerLED[timerTime[1].get() + 6] += _colors->minuteColor;
        _colorPerLED[(timerTime[1] - 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[(timerTime[1] + 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[1] = _colors->minuteColor / 8;
    }
    
    if (blink != 2 || sec % 2 == 0) {
        _colorPerLED[timerTime[2].get() + 6] += _colors->hourColor;
        _colorPerLED[(timerTime[2] - 1) + 6] += _colors->hourColor / 4;
        _colorPerLED[(timerTime[2] + 1) + 6] += _colors->hourColor / 4;
        _colorPerLED[0] = _colors->hourColor / 8;
    }
    
    if (blink != 3 || sec % 2 == 0) {
        _colorPerLED[23 + 6] += _colors->secondColor + _colors->minuteColor + _colors->hourColor;
        _colorPerLED[24 + 6] += _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    }
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showTimer(ComplexCounter timerTime[3], int selection) {
    resetColorPerLED();
    
    if (selection == 0) {
        _colorPerLED[timerTime[0].get() + 6] += _colors->secondColor;
        _colorPerLED[(timerTime[0] - 1) + 6] += _colors->secondColor / 4;
        _colorPerLED[(timerTime[0] + 1) + 6] += _colors->secondColor / 4;
        _colorPerLED[2] = _colors->secondColor / 8;
    }
    
    if (selection == 1) {
        _colorPerLED[timerTime[1].get() + 6] += _colors->minuteColor;
        _colorPerLED[(timerTime[1] - 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[(timerTime[1] + 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[1] = _colors->minuteColor / 8;
    }
    
    if (selection == 2) {
        _colorPerLED[timerTime[2].get() + 6] += _colors->hourColor;
        _colorPerLED[(timerTime[2] - 1) + 6] += _colors->hourColor / 4;
        _colorPerLED[(timerTime[2] + 1) + 6] += _colors->hourColor / 4;
        _colorPerLED[0] = _colors->hourColor / 8;
    }
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showTimer(ComplexCounter timerTime[3]) {
    resetColorPerLED();
    
    _colorPerLED[timerTime[0].get() + 6] += _colors->secondColor;
    _colorPerLED[(timerTime[0] - 1) + 6] += _colors->secondColor / 4;
    _colorPerLED[(timerTime[0] + 1) + 6] += _colors->secondColor / 4;
    _colorPerLED[2] = _colors->secondColor / 8;
    
    _colorPerLED[timerTime[1].get() + 6] += _colors->minuteColor;
    _colorPerLED[(timerTime[1] - 1) + 6] += _colors->minuteColor / 4;
    _colorPerLED[(timerTime[1] + 1) + 6] += _colors->minuteColor / 4;
    _colorPerLED[1] = _colors->minuteColor / 8;
    
    _colorPerLED[timerTime[2].get() + 6] += _colors->hourColor;
    _colorPerLED[(timerTime[2] - 1) + 6] += _colors->hourColor / 4;
    _colorPerLED[(timerTime[2] + 1) + 6] += _colors->hourColor / 4;
    _colorPerLED[0] = _colors->hourColor / 8;
    
    //_colorPerLED[23 + 6] += _colors->minuteColor + _colors->hourColor;
    //_colorPerLED[24 + 6] += _colors->minuteColor + _colors->hourColor;
    _colorPerLED[56 + 6] += _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showStoppwatch(ComplexCounter timerTime[3], ComplexCounter menuCounter) {
    resetColorPerLED();
    
    _colorPerLED[timerTime[0].get() + 6] += _colors->secondColor;
    _colorPerLED[(timerTime[0] - 1) + 6] += _colors->secondColor / 4;
    _colorPerLED[(timerTime[0] + 1) + 6] += _colors->secondColor / 4;
    _colorPerLED[2] = _colors->secondColor / 8;
    
    _colorPerLED[timerTime[1].get() + 6] += _colors->minuteColor;
    _colorPerLED[(timerTime[1] - 1) + 6] += _colors->minuteColor / 4;
    _colorPerLED[(timerTime[1] + 1) + 6] += _colors->minuteColor / 4;
    _colorPerLED[1] = _colors->minuteColor / 8;
    
    _colorPerLED[timerTime[2].get() + 6] += _colors->hourColor;
    _colorPerLED[(timerTime[2] - 1) + 6] += _colors->hourColor / 4;
    _colorPerLED[(timerTime[2] + 1) + 6] += _colors->hourColor / 4;
    _colorPerLED[0] = _colors->hourColor / 8;
    
    _colorPerLED[54 + 6] += _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    
    switch (menuCounter.getRelativePosition()) {
        case 0:
            _colorPerLED[23 + 6] += _colors->minuteColor + _colors->hourColor;
            _colorPerLED[24 + 6] += _colors->minuteColor + _colors->hourColor;
            break;
        case 1:
            _colorPerLED[24 + 6] += _colors->minuteColor + _colors->hourColor;
            break;
    }
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showColorSelectionMenu(ComplexCounter menuCounter, ComplexCounter themeCounter, int selection) {
    resetColorPerLED();
    
    // unter 45 muss aktuelle theme angezeigt werden = themecounter.get()
    // sonst das, was man auswählen könnte, also menuCounter.get()
    int currentTheme;
    if (menuCounter.get() <= 44)
        currentTheme = themeCounter.get();
    else
        currentTheme = menuCounter.get();
    
    switch (currentTheme) {
        case 45: // normal
            if (selection == 0 || selection == 3) {
                _colorPerLED[10 + 6] += _colors->secondColor;
                _colorPerLED[2] = _colors->secondColor / 8;
            }
            
            if (selection == 1 || selection == 3) {
                _colorPerLED[15 + 6] += _colors->minuteColor;
                _colorPerLED[1] = _colors->minuteColor / 8;
            }
            
            if (selection == 2 || selection == 3) {
                _colorPerLED[20 + 6] += _colors->hourColor;
                _colorPerLED[0] = _colors->hourColor / 8;
            }
            break;
        case 46:
            if (selection == 0 || selection == 3) {
                _colorPerLED[10 + 6] += _colors->secondColor + _colors->hourColor;
                _colorPerLED[2] = (_colors->secondColor + _colors->hourColor) / 8;
            }
            
            if (selection == 1 || selection == 3) {
                _colorPerLED[15 + 6] += _colors->minuteColor + _colors->secondColor;
                _colorPerLED[1] = (_colors->minuteColor + _colors->secondColor) / 8;
            }
            
            if (selection == 2 || selection == 3) {
                _colorPerLED[20 + 6] += _colors->hourColor + _colors->minuteColor;
                _colorPerLED[0] = (_colors->hourColor + _colors->minuteColor) / 8;
            }
            break;
        case 47:
            if (selection == 0 || selection == 3) {
                _colorPerLED[10 + 6] += _colors->secondColor + _colors->minuteColor;
                _colorPerLED[2] = (_colors->secondColor + _colors->minuteColor) / 8;
            }
            
            if (selection == 1 || selection == 3) {
                _colorPerLED[15 + 6] += _colors->minuteColor + _colors->hourColor;
                _colorPerLED[1] = (_colors->minuteColor + _colors->hourColor) / 8;
            }
            
            if (selection == 2 || selection == 3) {
                _colorPerLED[20 + 6] += _colors->hourColor + _colors->secondColor;
                _colorPerLED[0] = (_colors->hourColor + _colors->secondColor) / 8;
            }
            break;
    }
    
    if (selection == 0 || selection == 3) {
        _colorPerLED[(10 - 1) + 6] += _colors->secondColor / 4;
        _colorPerLED[(10 + 1) + 6] += _colors->secondColor / 4;
    }
    
    if (selection == 1 || selection == 3) {
        _colorPerLED[(15 - 1) + 6] += _colors->minuteColor / 4;
        _colorPerLED[(15 + 1) + 6] += _colors->minuteColor / 4;
    }
    
    if (selection == 2 || selection == 3) {
        _colorPerLED[(20 - 1) + 6] += _colors->hourColor / 4;
        _colorPerLED[(20 + 1) + 6] += _colors->hourColor / 4;
    }
    
    
    if (menuCounter.get() <= 44)
        _colorPerLED[menuCounter.get() + 6] += _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    else
        _colorPerLED[6 + 6] += _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showAll() {
    resetColorPerLED();
    
    for (int i = 0; i < 60; i++) {
        _colorPerLED[i + 6] = _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    }
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showRange(Counter range) {
    int start = range.get();
    resetColorPerLED();
    
    do {
        _colorPerLED[range++ % 60 + 6] = _colors->minuteColor;
    } while (range.get() != start);
    
    setAllLEDs();
    led_strip.show();
    
}

void ClockStrip::showNumber(int num) {
    resetColorPerLED();
    num = num % 60;
    
    for (int i = 0; i < num; i++) {
        _colorPerLED[i + 7] = _colors->secondColor + _colors->minuteColor + _colors->hourColor;
    }
    
    setAllLEDs();
    led_strip.show();
}

void ClockStrip::showTime(const struct time& currentTime, bool stoppwatch, bool timer, ComplexCounter theme) {
    resetColorPerLED();
    
    for (int i = 0; i < 7; i++) {
        if ((*_shouldDisplay)[i]) {
            switch (i)
            {
                case 0: // year
                    if (currentTime.second % 5 == 2) {
                        _colorPerLED[5] = { 20,20,20 };
                        _colorPerLED[12] += { 20, 20, 20 };
                    }
                    break;
                case 1: // month
                    if (currentTime.second % 5 == 1) {
                        _colorPerLED[4] = { 20,20,20 };
                        _colorPerLED[12] += { 20, 20, 20 };
                    }
                    break;
                case 2: // day
                    if (currentTime.second % 5 == 0) {
                        _colorPerLED[3] = { 20,20,20 };
                        _colorPerLED[12] += { 20, 20, 20 };
                    }
                    break;
                case 3: // weekday
                    if (currentTime.second % 5 == 4) {
                        _colorPerLED[52] += {50, 50, 50};
                        _colorPerLED[12] += { 20, 20, 20 };
                    }
                    break;
                case 4: // hour
                {
                    int twelveHourFormat = (currentTime.hour > 11 ? currentTime.hour - 12 : currentTime.hour) * 5;
                    int minuteOffset = currentTime.minute / 12;
                    Counter ledNum = Counter(0, 59);
                    ledNum.set(twelveHourFormat + minuteOffset);
                    
                    switch (theme.get()) {
                        case 45: // normal
                            _colorPerLED[ledNum.get() + 6] += _colors->hourColor;
                            _colorPerLED[0] = _colors->hourColor / 16;
                            break;
                        case 46:
                            _colorPerLED[ledNum.get() + 6] += _colors->hourColor + _colors->minuteColor;
                            _colorPerLED[0] = (_colors->hourColor + _colors->minuteColor) / 16;
                            break;
                        case 47:
                            _colorPerLED[ledNum.get() + 6] += _colors->hourColor + _colors->secondColor;
                            _colorPerLED[0] = (_colors->hourColor + _colors->secondColor) / 16;
                            break;
                    }
                    
                    _colorPerLED[(ledNum - 1) + 6] += _colors->hourColor / 8;
                    _colorPerLED[(ledNum + 1) + 6] += _colors->hourColor / 8;
                    
                    break;
                }
                case 5: // minute
                {
                    Counter ledNum = Counter(0, 59);
                    ledNum.set(currentTime.minute);
                    
                    switch (theme.get()) {
                        case 45: // normal
                            _colorPerLED[ledNum.get() + 6] += _colors->minuteColor;
                            _colorPerLED[1] = _colors->minuteColor / 16;
                            break;
                        case 46:
                            _colorPerLED[ledNum.get() + 6] += _colors->minuteColor + _colors->secondColor;
                            _colorPerLED[1] = (_colors->minuteColor + _colors->secondColor) / 16;
                            break;
                        case 47:
                            _colorPerLED[ledNum.get() + 6] += _colors->minuteColor + _colors->hourColor;
                            _colorPerLED[1] = (_colors->minuteColor + _colors->hourColor) / 16;
                            break;
                    }
                    
                    _colorPerLED[(ledNum - 1) + 6] += _colors->minuteColor / 8;
                    _colorPerLED[(ledNum + 1) + 6] += _colors->minuteColor / 8;
                    
                    break;
                }
                case 6: // second
                {
                    Counter ledNum = Counter(0, 59);
                    ledNum.set(currentTime.second);
                    
                    switch (theme.get()) {
                        case 45: // normal
                            _colorPerLED[ledNum.get() + 6] += _colors->secondColor;
                            _colorPerLED[2] = _colors->secondColor / 16;
                            break;
                        case 46:
                            _colorPerLED[ledNum.get() + 6] += _colors->secondColor + _colors->hourColor;
                            _colorPerLED[2] = (_colors->secondColor + _colors->hourColor) / 16;
                            break;
                        case 47:
                            _colorPerLED[ledNum.get() + 6] += _colors->secondColor + _colors->minuteColor;
                            _colorPerLED[2] = (_colors->secondColor + _colors->minuteColor) / 16;
                            break;
                    }
                    
                    _colorPerLED[(ledNum - 1) + 6] += _colors->secondColor / 8;
                    _colorPerLED[(ledNum + 1) + 6] += _colors->secondColor / 8;
                }
                    break;
            }
        }
    }
    
    
    if ((*_alarmState)[0])
        _colorPerLED[29 + 6] += _colors->minuteColor + _colors->hourColor;
    if ((*_alarmState)[1])
        _colorPerLED[28 + 6] += _colors->minuteColor + _colors->hourColor;
    
    if (stoppwatch)
        _colorPerLED[54 + 6] += _colors->minuteColor + _colors->hourColor;
    if (timer)
        _colorPerLED[56 + 6] += _colors->minuteColor + _colors->hourColor;
    
    setAllLEDs();
    
    led_strip.show();
}

void ClockStrip::clear() {
    led_strip.clear();
    led_strip.show();
}

void ClockStrip::show() {
    setAllLEDs();
    led_strip.show();
}


void ClockStrip::demo() {
    // Some example procedures showing how to display to the pixels:
    colorWipe(LEDcolor(255, 0, 0), 30); // Red
    colorWipe(LEDcolor(0, 255, 0), 30); // Green
    colorWipe(LEDcolor(0, 0, 255), 30); // Blue
    //colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    // Send a theater pixel chase in...
    
    theaterChase(LEDcolor(127, 127, 127), 10, 65); // White
    theaterChase(LEDcolor(255, 0, 0), 15, 50); // Red
    theaterChase(LEDcolor(0, 150, 0), 20, 35); // Green
    theaterChase(LEDcolor(0, 0, 100), 25, 20); // Blue
    theaterChase(LEDcolor(255, 255, 255), 40, 10); // White
    
    rainbowCycle(2, 20);
    rainbowCycle(5, 5);
    rainbowCycle(12, 1);
    theaterChaseRainbow(40);
}

void ClockStrip::setRunDemo(bool value) {
    runDemo = value;
}

