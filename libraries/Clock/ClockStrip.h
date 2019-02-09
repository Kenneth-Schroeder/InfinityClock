#ifndef ClockStrip_h
#define ClockStrip_h	// verhindert Mehrfacheinbindung

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LEDcolor.h"

class ClockStrip {
private:
	const int _datapin;
	const int _length;
	const int _sck_clock;
	const int _mosi_data;
	Adafruit_NeoPixel led_strip;
	bool(*_shouldDisplay)[7];
	bool(*_alarmState)[2];
	struct colorTriple *_colors;
	bool on = true;
	double *_brightness;
	LEDcolor _colorPerLED[66];
	bool runDemo = true;

	// setLED setzt beliebige LED (0-65) und beachtet lediglich, dass Änderungen an den KreisLEDs, verdoppelt werden.----------------------------------
    void setLED(byte num, byte red, byte green, byte blue);
    void setLED(byte num, LEDcolor color);

	// setClockLED mapped jede Zahl auf Bereich von 0 bis 59 und beachtet bereits Orientierung der Uhr, wenn aufgehangen.
    void setClockLED(int num, byte red, byte green, byte blue);

	// setClockLED mit union
    void setClockLED(int num, LEDcolor color);
    int getPhysicalLEDnum(int num);
    void resetColorPerLED();
    void setAllLEDs();
    void myDelay(unsigned int duration);

	// Fill the dots one after the other with a color
    void colorWipe(LEDcolor c, uint8_t wait);

	// Slightly different, this makes the rainbow equally distributed throughout
    void rainbowCycle(int reps, uint8_t wait);

	//Theatre-style crawling lights.
    void theaterChase(LEDcolor c, int reps, uint8_t wait);
    
	//Theatre-style crawling lights with rainbow effect
    void theaterChaseRainbow(uint8_t wait);

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
    LEDcolor Wheel(byte WheelPos);

public:
    ClockStrip(int datapin, int length, int sck_clock, int mosi_data, struct colorTriple &colors, bool(&shouldDisplay)[7], bool(&alarmState)[2], double &brightness);
    void showMenuLED(int num, bool secondChanged = true, int extra = 61);
    void showAlarmTime(ComplexCounter alarmTime[3], int blink, int sec, int alarmLED);
    void showAlarmTime(ComplexCounter alarmTime[3], int selection, int alarmLED);
    void showTimer(ComplexCounter timerTime[3], int blink, int sec);
    void showTimer(ComplexCounter timerTime[3], int selection);
    void showTimer(ComplexCounter timerTime[3]);
    void showStoppwatch(ComplexCounter timerTime[3], ComplexCounter menuCounter);
    void showColorSelectionMenu(ComplexCounter menuCounter, ComplexCounter themeCounter, int selection = 3);
    void showAll();
    void showRange(Counter range);
    void showNumber(int num);
    void showTime(const struct time& currentTime, bool stoppwatch, bool timer, ComplexCounter theme);
    void clear();
    void show();
    void demo();
    void setRunDemo(bool value);
};

#endif
