#ifndef internalState_h
#define internalState_h	// verhindert Mehrfacheinbindung

#define strip_dataPin 10
#define strip_length 126

#define DS3234_SS_PIN 8 // DeadOn my_rtc Chip-select pin
#define DISPLAY_SS_PIN 7

#define DS3234_INTERRUPT_PIN 2 // DeadOn my_rtc SQW/interrupt pin (optional)

#define SPI_SCK_CLOCK 13     //
#define SPI_MOSI_DATA 11             // SPI PINS
#define SPI_MISO_DISABLE 12          // NICHT GENUTZT

#include <Arduino.h>
#include "ComplexCounter.h"
#include "ClockStrip.h"
#include "SegDisplay.h"
#include "Buttons.h"
#include "LEDcolor.h"
#include "types.h"
#include "SoundModule.h"
#include "ClockModule.h"
#include "TimeCounter.h"

class InternalState {
public:
    byte knobPins[9] = { 3,4,5,6,9,20,21,22,23 };
    /* MOSFET Arrangement
     Top RGB = 0, 1, 4
     Mid RGB = 8, 3, 2
     Bot RGB = 7, 5, 6
     */
    
    struct time clockTime;
    bool shouldDisplay[7] = { false, true, true, false, true, true, true }; // welche Sachen angezeigt werden aus currentDate, Reichenfolge: Year, Month, DoM, Weekday, Hour, Minute, Second
    
    bool alarmState[2] = { false, false };
    bool alarmRepeatState[2][3] = { {false, false, false}, {false, false, false} }; // Hour, weekday, everyDay (DoM)
    ComplexCounter alarmTime[2][3] = { {ComplexCounter(0,59), ComplexCounter(0,23), ComplexCounter(1,31) }, { ComplexCounter(0,59), ComplexCounter(0,23), ComplexCounter(1,31) } }; // minute, hour, DoM
    int alarmTone = 2;
    
    //struct colorTriple colors = { {255, 127, 0}, {0, 255, 130}, {255, 0, 200} }; // SECOND, MINUTE, HOUR
    struct colorTriple colors = { { 0, 0, 255 },{ 0, 255, 0 },{ 255, 0, 0 } }; // SECOND, MINUTE, HOUR
    
    bool on = true;
    Rotary_State rotary_state = not_changed;
    
    bool secondChanged = false;
    bool rotaryChanged = false;
    bool externalStateChanged = true;
    bool playingMusic = false;
    bool offInstrExecuted = false;
    bool onInstrExecuted = false;
    
    unsigned long lastSongCheck = 0;
    long lastRotaryCall = 0;
    int alarmFeedback = 0;
    
    int rotaryCounter = 100;
    double led_brightness = 1; // led_brightness from 0 to 1
    double prev_led_brightness = 1;
    ComplexCounter mainMenuCounter;
    ComplexCounter accuracySelectionCounter;
    ComplexCounter musicMenuCounter;
    ComplexCounter settingTimeMenuCounter;
    ComplexCounter alarmMenuCounter;
    ComplexCounter alarmCounter;
    ComplexCounter alarmRepeatCounter;
    ComplexCounter alarmSettingCounter;
    ComplexCounter timerSettingCounter;
    ComplexCounter stoppwatchMenuCounter;
    ComplexCounter colorSelectionMenuCounter;
    ComplexCounter colorTheme;
    ComplexCounter colorMenuCounter;
    ComplexCounter colorRange;
    int clockSettingOffset = 0;
    
    Timer my_Timer;
    ComplexCounter timerSelection[3] = { ComplexCounter(0,59), ComplexCounter(0,59), ComplexCounter(0,59) };
    Stoppwatch my_Stoppwatch;
    
    ClockStrip myStrip;
    SegDisplay myDisplay;
    Buttons myButtons;
    SoundModule music_player;
    ClockModule my_rtc;
    
    // Rotary Declarations
    volatile bool readingA = false;
    volatile bool readingB = false;
    volatile byte aFlag = 0;
    volatile byte bFlag = 0;
    volatile unsigned long lastMicrosA = 0;
    volatile unsigned long lastMicrosB = 0;
    volatile unsigned int debounceA = 1000;
    volatile unsigned int debounceB = 1000;
    
    InternalState();
    bool changed();
    void handleRotaryTurn(ComplexCounter &counter);
    void handleRotaryTurn(int &counter);
};

#endif
