#include "InternalState.h"

InternalState::InternalState() :
    mainMenuCounter(51, 63, true),
    accuracySelectionCounter(42, 49, true),
    musicMenuCounter(23, 25, false),
    settingTimeMenuCounter(42, 49, true),
    alarmMenuCounter(28, 33, true),
    alarmCounter(28, 29, true),
    alarmRepeatCounter(43, 47, true),
    alarmSettingCounter(0, 2, false),
    timerSettingCounter(0, 3, false),
    stoppwatchMenuCounter(0, 1, false),
    colorSelectionMenuCounter(42, 47, false),
    colorTheme(45, 47, false),
    colorMenuCounter(36, 38, false),
    colorRange(0, 51, false),
    my_Timer(10, 20, 30),
    myStrip(strip_dataPin, strip_length, SPI_SCK_CLOCK, SPI_MOSI_DATA, colors, shouldDisplay, alarmState, led_brightness),
    myDisplay(DISPLAY_SS_PIN, shouldDisplay),
    myButtons(colors, knobPins, led_brightness, colorTheme),
    my_rtc(DS3234_SS_PIN, DS3234_INTERRUPT_PIN, clockTime)
{
    
}

bool InternalState::changed(){
    return rotaryChanged || secondChanged || externalStateChanged;
}

void InternalState::handleRotaryTurn(ComplexCounter &counter) {
    rotaryChanged = false;
    externalStateChanged = false;
    if (rotary_state != not_changed) {
        if (rotary_state == turned_up)
            counter++;
        else
            counter--;
        
        rotary_state = not_changed;
        rotaryChanged = true; // until next call of this method, for the menumenagement-strip-updates
    }
}

void InternalState::handleRotaryTurn(int &counter) {
    rotaryChanged = false;
    externalStateChanged = false;
    if (rotary_state != not_changed) {
        if (rotary_state == turned_up)
            counter++;
        else
            counter--;
        
        rotary_state = not_changed;
        rotaryChanged = true; // see above
    }
}
