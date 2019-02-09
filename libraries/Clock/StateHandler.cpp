#include <Arduino.h>
#include "StateHandler.h"
#include "clockStrip.h"
#include "SegDisplay.h"
#include "Buttons.h"
#include "LEDcolor.h"
#include "ComplexCounter.h"
#include "soundModule.h"
#include "clockModule.h"
#include "timeCounter.h"
#include "InternalState.h"
#include "ExternalStates.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

Time_State myTimeState;
MainMenu_State myMainMenuState;
BrightnessSelection_State myBrightnessSelectionState;
VolumeSelection_State myVolumeSelectionState;
Demo_State myDemoState;
Accuracy_State myAccuracyState;
MusicMenu_State myMusicMenuState;
SettingTimeMenu_State mySettingTimeMenuState;
SettingTime_State mySettingTimeState;
AlarmMenu_State myAlarmMenuState;
AlarmEnabling_State myAlarmEnablingState;
AlarmRepetitionSelection_State myAlarmRepetitionSelectionState;
AlarmSettingMenu_State myAlarmSettingMenuState;
AlarmSetting_State myAlarmSettingState;
AlarmToneSelection_State myAlarmToneSelectionState;
TimerSettingMenu_State myTimerSettingMenuState;
TimerSetting_State myTimerSettingState;
Stoppwatch_State myStoppwatchState;
ColorSelectionMenu_State myColorSelectionMenuState;
ColorMenu_State myColorMenuState;
ColorValueSelection_State myColorValueSelectionState;

void StateHandler::setup(){
    _iS.music_player.init();
    _iS.musicMenuCounter.set(25);
    
    _iS.alarmTime[0][0].set(0);
    _iS.alarmTime[0][1].set(1);
    _iS.alarmTime[0][2].set(10);
    
    _iS.alarmTime[1][0].set(0);
    _iS.alarmTime[1][1].set(1);
    _iS.alarmTime[1][2].set(10);
    
    _iS.timerSelection[0].set(10);
    _iS.timerSelection[1].set(5);
    _iS.timerSelection[2].set(0);
    _iS.my_Timer.set(_iS.timerSelection);
    
    for (int i = 0; i < 9; i++) {
        pinMode(_iS.knobPins[i], OUTPUT);
    }
    for (int i = 0; i < 9; i++) {
        analogWrite(_iS.knobPins[i], 0);
    } // Knob Init
    
    pinMode(SPI_MOSI_DATA, OUTPUT);
    pinMode(SPI_SCK_CLOCK, OUTPUT);
    pinMode(DISPLAY_SS_PIN, OUTPUT); // SPI Pin Init
    
    _iS.my_rtc.init();
    
    myTimeState = Time_State();
    myMainMenuState = MainMenu_State();
    myBrightnessSelectionState = BrightnessSelection_State();
    myVolumeSelectionState = VolumeSelection_State();
    myDemoState = Demo_State();
    myAccuracyState = Accuracy_State();
    myMusicMenuState = MusicMenu_State();
    mySettingTimeMenuState = SettingTimeMenu_State();
    mySettingTimeState = SettingTime_State();
    myAlarmMenuState = AlarmMenu_State();
    myAlarmEnablingState = AlarmEnabling_State();
    myAlarmRepetitionSelectionState = AlarmRepetitionSelection_State();
    myAlarmSettingMenuState = AlarmSettingMenu_State();
    myAlarmSettingState = AlarmSetting_State();
    myAlarmToneSelectionState = AlarmToneSelection_State();
    myTimerSettingMenuState = TimerSettingMenu_State();
    myTimerSettingState = TimerSetting_State();
    myStoppwatchState = Stoppwatch_State();
    myColorSelectionMenuState = ColorSelectionMenu_State();
    myColorMenuState = ColorMenu_State();
    myColorValueSelectionState = ColorValueSelection_State();
    
    externalState = &myTimeState;
}

void StateHandler::loop(){
    _iS.readingA = digitalReadFast(14);
    _iS.readingB = digitalReadFast(15); // read for interrupts
    _iS.alarmFeedback = _iS.my_rtc.alarmThings();
    
    handleAlarmResets();
    
    delay(1); // for bottomKnob
    
    if (analogRead(A5) < 800 && _iS.on == true) { // brightness threshold
        turnOn();
        display();
    }
    else if(analogRead(A5) > 900 || _iS.on == false) {
        turnOff();
    }
    else if (_iS.onInstrExecuted) { // ensures, that the clock doesn't do nothing, when in between the main modes
        display();
    }
}

void StateHandler::handleAlarmResets() {
    if (_iS.alarmFeedback != 2) { // alarm 1 or 2 has been triggered, so lets reset it according to alarmRepeatState
        if (_iS.alarmRepeatState[_iS.alarmFeedback][0]) // repeat every hour
            _iS.alarmTime[_iS.alarmFeedback][1]++;
        else if (_iS.alarmRepeatState[_iS.alarmFeedback][1]) // repeat every Weekday
            if (_iS.my_rtc.weekDay() < 5)
                _iS.alarmTime[_iS.alarmFeedback][2]++;
            else
                _iS.alarmTime[_iS.alarmFeedback][2].set(_iS.alarmTime[_iS.alarmFeedback][2] + (_iS.my_rtc.date() + (8 - _iS.my_rtc.weekDay())));
            else if (_iS.alarmRepeatState[_iS.alarmFeedback][2]) // repeat every day
                _iS.alarmTime[_iS.alarmFeedback][2]++;
            else // dont repeat
                _iS.alarmState[_iS.alarmFeedback] = false;
        
        _iS.my_rtc.setAlarm(_iS.alarmFeedback, _iS.alarmTime[_iS.alarmFeedback]);
    }
}

void StateHandler::turnOn(){
    if (!_iS.onInstrExecuted) { // executed ones
        _iS.myButtons.show(_iS.colorTheme);
        _iS.onInstrExecuted = true;
        _iS.offInstrExecuted = false;
    }
}

void StateHandler::turnOff(){
    if (!_iS.offInstrExecuted) { // executed ones
        _iS.myStrip.clear();
        _iS.myDisplay.clear();
        _iS.myButtons.clear();
        _iS.offInstrExecuted = true;
        _iS.onInstrExecuted = false;
    }
}

void StateHandler::display(){
    _iS.my_rtc.update();
    
    _iS.secondChanged = false;
    if (_iS.my_rtc.secondChanged()) {
        if (_iS.my_Timer.isActive()) {
            if (_iS.my_Timer--) {
                _iS.music_player.play(_iS.alarmTone);
            }
        }
        
        if (_iS.my_Stoppwatch.isActive())
            _iS.my_Stoppwatch++;
        
        _iS.secondChanged = true; // hinweise für menuManagement
    }
    
    externalState->loopAction(_iS);
}

void StateHandler::ISR_BotButton() {
    _iS.on = !_iS.on;
}

void StateHandler::ISR_alarm() {
    _iS.music_player.play(_iS.alarmTone);
}

void StateHandler::ISR_RotaryPinA() {
    /*
     *  http://www.instructables.com/id/Improved-Arduino-Rotary-Encoder-Reading/#ampshare=http://www.instructables.com/id/Improved-Arduino-Rotary-Encoder-Reading/ inspired, not copied
     */
    if (micros() - _iS.lastMicrosA > _iS.debounceA) {
        cli(); //stop interrupts happening before we read pin values
        _iS.lastMicrosA = micros();
        
        // wenn readingA 1 ist, ist es wegen Change gerade gefallen
        if (_iS.readingA && !_iS.readingB && _iS.aFlag) { // aFlag heißt, B ist schon unten und "wartet" auf A, also Drehung hoch
            _iS.rotary_state = turned_up;
            _iS.bFlag = 0;
            _iS.aFlag = 0;
        }
        else if (_iS.readingA) _iS.bFlag = 1; // A ist gerade gefallen und wartet auf B
        // Flags stellen sicher, dass Interrupts beider Pins sofort hintereinander folgen können, auch wenn readings im std Code noch nicht aktualisiert wurden
        sei(); //restart interrupts
    }
}

void StateHandler::ISR_RotaryPinB() {
    if (micros() - _iS.lastMicrosB > _iS.debounceB) {
        cli();
        _iS.lastMicrosB = micros();
        
        if (_iS.readingB && !_iS.readingA && _iS.bFlag) { // A ist vor B gefallen, damit Drehung runter
            _iS.rotary_state = turned_down;
            _iS.bFlag = 0;
            _iS.aFlag = 0;
        }
        else if (_iS.readingB) _iS.aFlag = 1; // B ist gerade gefallen, erwarten also, dass A gleich fällt
        sei();
    }
}

void StateHandler::ISR_TopButton() { // Back-Button
    if(externalState == &myMainMenuState)
            externalState = &myTimeState;
    else if(externalState == &myBrightnessSelectionState)
            externalState = &myMainMenuState;
    else if(externalState == &myVolumeSelectionState)
            externalState = &myMainMenuState;
    else if(externalState == &myDemoState)
            externalState = &myMainMenuState;
    else if(externalState == &myAccuracyState)
            externalState = &myMainMenuState;
    else if(externalState == &myMusicMenuState)
            externalState = &myMainMenuState;
    else if(externalState == &mySettingTimeMenuState)
            externalState = &myMainMenuState;
    else if(externalState == &mySettingTimeState)
            externalState = &mySettingTimeMenuState;
    else if(externalState == &myAlarmMenuState)
            externalState = &myMainMenuState;
    else if(externalState == &myAlarmEnablingState)
            externalState = &myAlarmMenuState;
    else if(externalState == &myAlarmRepetitionSelectionState)
            externalState = &myAlarmMenuState;
    else if(externalState == &myAlarmSettingMenuState)
            externalState = &myAlarmMenuState;
    else if(externalState == &myAlarmSettingState)
            externalState = &myAlarmSettingMenuState;
    else if(externalState == &myAlarmToneSelectionState)
            externalState = &myAlarmMenuState;
    else if(externalState == &myTimerSettingMenuState)
            externalState = &myMainMenuState;
    else if(externalState == &myTimerSettingState)
            externalState = &myTimerSettingMenuState;
    else if(externalState == &myStoppwatchState)
            externalState = &myMainMenuState;
    else if(externalState == &myColorSelectionMenuState)
            externalState = &myMainMenuState;
    else if(externalState == &myColorMenuState)
            externalState = &myColorSelectionMenuState;
    else if(externalState == &myColorValueSelectionState)
            externalState = &myColorMenuState;
    
    _iS.externalStateChanged = true;
}

void StateHandler::ISR_MidButton() { // "Steuermethode"
    externalState->midButtonPress(_iS, this);
    _iS.rotaryChanged = true; // display menu led when menu changed
}

void StateHandler::changeState(IState* aState) {
    externalState = aState;
    _iS.externalStateChanged = true;
}
