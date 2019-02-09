#include "types.h"
#include "ExternalStates.h"
#include "InternalState.h"
#include "StateHandler.h"

void Time_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    stateHandler->changeState(&myMainMenuState);
}

void Time_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showTime(clock.clockTime,
                              clock.my_Stoppwatch.isActive(),
                              clock.my_Timer.isActive(),
                              clock.colorTheme);
        clock.myDisplay.showTime(clock.clockTime);
    }
}


void MainMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    switch (clock.mainMenuCounter.get()) {
        case 51:
            stateHandler->changeState(&myBrightnessSelectionState);
            break;
        case 52:
            stateHandler->changeState(&myVolumeSelectionState);
            clock.music_player.play(2);
            break;
        case 53:
            stateHandler->changeState(&myColorSelectionMenuState);
            break;
        case 54:
            stateHandler->changeState(&myStoppwatchState);
            break;
        case 56:
            stateHandler->changeState(&myTimerSettingMenuState);
            break;
        case 57:
            stateHandler->changeState(&myAlarmMenuState);
            break;
        case 58:
            stateHandler->changeState(&mySettingTimeMenuState);
            break;
        case 59:
            stateHandler->changeState(&myTimeState);
            clock.myStrip.showTime(clock.clockTime, clock.my_Stoppwatch.isActive(), clock.my_Timer.isActive(), clock.colorTheme);
            clock.myDisplay.showTime(clock.clockTime);
            break;
        case 61:
            clock.playingMusic = clock.music_player.isPlaying();
            stateHandler->changeState(&myMusicMenuState);
            break;
        case 62:
            clock.myStrip.setRunDemo(true);
            stateHandler->changeState(&myDemoState);
            break;
        case 63:
            stateHandler->changeState(&myAccuracyState);
            break;
    }
}

void MainMenu_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showMenuLED(clock.mainMenuCounter.get());
        clock.myDisplay.clear();
    }
    
    clock.handleRotaryTurn(clock.mainMenuCounter);
}


void BrightnessSelection_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    stateHandler->changeState(&myTimeState);
}

void BrightnessSelection_State::loopAction(InternalState& clock) {
    if (clock.changed())
        clock.myStrip.showAll();
    
    clock.rotaryChanged = false;
    if (clock.rotary_state != not_changed) {
        if (clock.rotary_state == turned_up)
            clock.rotaryCounter += 1;
        else
            clock.rotaryCounter -= 1;
        
        clock.led_brightness = (double)((clock.rotaryCounter % 9) + 1) / 10;
        
        if (clock.led_brightness != clock.prev_led_brightness) {
            clock.myStrip.show();
            clock.myButtons.show(clock.colorTheme);
            clock.prev_led_brightness = clock.led_brightness;
        }
        clock.rotaryChanged = true;
        
        clock.rotary_state = not_changed;
    }
}


void VolumeSelection_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.music_player.stop();
    clock.myDisplay.clear();
    stateHandler->changeState(&myMainMenuState);
}

void VolumeSelection_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showNumber(clock.music_player.getVolume());
        clock.myDisplay.showNumber(clock.music_player.getVolume());
    }
    
    clock.rotaryChanged = false;
    if (clock.rotary_state != not_changed) {
        if (clock.rotary_state == turned_up)
            clock.music_player.increaseVolume();
        else
            clock.music_player.decreaseVolume();
        
        clock.rotary_state = not_changed;
        clock.rotaryChanged = true;
    }
}


void Demo_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.myStrip.setRunDemo(false);
    
    stateHandler->changeState(&myMainMenuState);
}

void Demo_State::loopAction(InternalState& clock) {
    clock.myStrip.demo();
}


void Accuracy_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.shouldDisplay[6 - clock.accuracySelectionCounter.getRelativePosition()] = !clock.shouldDisplay[6 - clock.accuracySelectionCounter.getRelativePosition()];
}

void Accuracy_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showMenuLED(clock.accuracySelectionCounter.get());
        clock.myDisplay.showBool(clock.shouldDisplay[6 - clock.accuracySelectionCounter.getRelativePosition()]);
    }
    
    clock.handleRotaryTurn(clock.accuracySelectionCounter);
}


void MusicMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    switch (clock.musicMenuCounter.get()) {
        case 23:
            clock.music_player.next();
            break;
        case 24:
            clock.music_player.previous();
            break;
        case 25:
            if (!clock.music_player.isPlaying()) {
                clock.music_player.play(4 - random(1, 4));
                clock.playingMusic = true;
            }
            else {
                clock.music_player.stop();
                clock.playingMusic = false;
            }
            break;
    }
}

void MusicMenu_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        if (clock.musicMenuCounter.get() == 25) {
            clock.myStrip.showRange(Counter(23, 24));
            clock.myDisplay.showBool(clock.playingMusic);
        }
        else {
            clock.myStrip.showMenuLED(clock.musicMenuCounter.get());
            clock.myDisplay.clear();
        }
    }
    
    clock.handleRotaryTurn(clock.musicMenuCounter);
}


void SettingTimeMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    stateHandler->changeState(&mySettingTimeState);
    if (clock.settingTimeMenuCounter.get() == 47 || clock.settingTimeMenuCounter.get() == 48)
        clock.clockSettingOffset = -1;
    else
        clock.clockSettingOffset = 0;
}

void SettingTimeMenu_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showMenuLED(clock.settingTimeMenuCounter.get());
        clock.myDisplay.clear();
    }
    
    clock.handleRotaryTurn(clock.settingTimeMenuCounter);
}


void SettingTime_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    switch (clock.settingTimeMenuCounter.get()) {
        case 42:
            clock.my_rtc.setSecondByOffset(clock.clockSettingOffset); // 0 bis 59
            break;
        case 43:
            clock.my_rtc.setMinuteByOffset(clock.clockSettingOffset); // 0 bis 59
            break;
        case 44:
            clock.my_rtc.setHourByOffset(clock.clockSettingOffset); // 0 bis 23
            break;
        case 46:
            clock.my_rtc.setWeekdayByOffset(clock.clockSettingOffset); // 1 bis 7
            break;
        case 47:
            clock.my_rtc.setDateByOffset(clock.clockSettingOffset); // 1 bis 31
            break;
        case 48:
            clock.my_rtc.setMonthByOffset(clock.clockSettingOffset); // 1 bis 12
            break;
        case 49:
            clock.my_rtc.setYearByOffset(clock.clockSettingOffset);
            break;
    }
    stateHandler->changeState(&mySettingTimeMenuState);
    // TODO
}

void SettingTime_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        switch (clock.settingTimeMenuCounter.get()) {
            case 42:
                clock.myStrip.showNumber(clock.my_rtc.getSecondWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getSecondWithOffset(clock.clockSettingOffset));
                break;
            case 43:
                clock.myStrip.showNumber(clock.my_rtc.getMinuteWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getMinuteWithOffset(clock.clockSettingOffset));
                break;
            case 44:
                clock.myStrip.showNumber(clock.my_rtc.getHourWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getHourWithOffset(clock.clockSettingOffset));
                break;
            case 46:
                clock.myStrip.showNumber(clock.my_rtc.getWeekdayWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getWeekdayWithOffset(clock.clockSettingOffset));
                break;
            case 47:
                clock.myStrip.showNumber(clock.my_rtc.getDateWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getDateWithOffset(clock.clockSettingOffset));
                break;
            case 48:
                clock.myStrip.showNumber(clock.my_rtc.getMonthWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getMonthWithOffset(clock.clockSettingOffset));
                break;
            case 49:
                clock.myStrip.showNumber(clock.my_rtc.getYearWithOffset(clock.clockSettingOffset));
                clock.myDisplay.showNumber(clock.my_rtc.getYearWithOffset(clock.clockSettingOffset));
                break;
        }
    }
    
    clock.handleRotaryTurn(clock.clockSettingOffset);
}


void AlarmMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    // TODO
    switch (clock.alarmMenuCounter.get()) {
        case 28:
        case 29:
            stateHandler->changeState(&myAlarmSettingMenuState);
            break;
        case 31:
            stateHandler->changeState(&myAlarmToneSelectionState);
            clock.music_player.play(clock.alarmTone);
            break;
        case 32:
            clock.alarmCounter.set(29);
            stateHandler->changeState(&myAlarmRepetitionSelectionState);
            break;
        case 33:
            stateHandler->changeState(&myAlarmEnablingState);
            break;
    }
}

void AlarmMenu_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showMenuLED(clock.alarmMenuCounter.get());
        clock.myDisplay.clear();
    }
    
    clock.handleRotaryTurn(clock.alarmMenuCounter);
}


void AlarmEnabling_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    switch (clock.alarmCounter.get()) {
        case 28:
            clock.alarmState[1] = !clock.alarmState[1];
            if (clock.alarmState[1]) // enable Alarm
                clock.my_rtc.enableAlarm(1, true);
            else // disable Alarm
                clock.my_rtc.enableAlarm(1, false);
            break;
        case 29:
            clock.alarmState[0] = !clock.alarmState[0];
            if (clock.alarmState[0])
                clock.my_rtc.enableAlarm(0, true);
            else
                clock.my_rtc.enableAlarm(0, false);
            break;
    }
}

void AlarmEnabling_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showMenuLED(clock.alarmCounter.get());
        clock.myDisplay.showBool(clock.alarmState[1 - (clock.alarmCounter.get() - 28)]);
    }
    
    clock.handleRotaryTurn(clock.alarmCounter);
}


void AlarmRepetitionSelection_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    if (clock.alarmRepeatCounter.get() != 43) {
        clock.alarmRepeatState[1 - clock.alarmCounter.getRelativePosition()]
        [clock.alarmRepeatCounter.getRelativePosition() - 1] = !clock.alarmRepeatState[1 - clock.alarmCounter.getRelativePosition()]
        [clock.alarmRepeatCounter.getRelativePosition() - 1];
        for (int i = 0; i < 3; i++) {
            clock.alarmRepeatCounter++;
            if (clock.alarmRepeatCounter.get() != 23)
                clock.alarmRepeatState[1 - clock.alarmCounter.getRelativePosition()][clock.alarmRepeatCounter.getRelativePosition() - 1] = false;
        }
        clock.alarmRepeatCounter++;
    }
    else
        clock.alarmCounter++;
}

void AlarmRepetitionSelection_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        if (clock.alarmRepeatCounter.get() != 43) {
            clock.myStrip.showMenuLED(clock.alarmRepeatCounter.get(), true, clock.alarmCounter.get());
            clock.myDisplay.showBool(clock.alarmRepeatState[1 - clock.alarmCounter.getRelativePosition()][clock.alarmRepeatCounter.getRelativePosition() - 1]);
        }
        else {
            clock.myStrip.showMenuLED(23, true, clock.alarmCounter.get());
            clock.myDisplay.clear();
        }
    }
    
    clock.handleRotaryTurn(clock.alarmRepeatCounter);
}


void AlarmSettingMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    stateHandler->changeState(&myAlarmSettingState);
}

void AlarmSettingMenu_State::loopAction(InternalState& clock) {
    clock.myStrip.showAlarmTime(clock.alarmTime[1 - clock.alarmMenuCounter.getRelativePosition()], clock.alarmSettingCounter.getRelativePosition(), clock.my_rtc.second(), clock.alarmMenuCounter.get());
    clock.myDisplay.showAlarmTime(clock.alarmTime[1 - clock.alarmMenuCounter.getRelativePosition()], clock.alarmSettingCounter.getRelativePosition(), clock.my_rtc.second());
    
    clock.handleRotaryTurn(clock.alarmSettingCounter);
}


void AlarmSetting_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.my_rtc.setAlarm(0, clock.alarmTime[0]);
    clock.my_rtc.setAlarm(1, clock.alarmTime[1]);
    stateHandler->changeState(&myAlarmSettingMenuState);
}

void AlarmSetting_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showAlarmTime(clock.alarmTime[1 - clock.alarmMenuCounter.getRelativePosition()], clock.alarmSettingCounter.getRelativePosition(), clock.alarmMenuCounter.get());
        clock.myDisplay.showAlarmTime(clock.alarmTime[1 - clock.alarmMenuCounter.getRelativePosition()], clock.alarmSettingCounter.getRelativePosition());
    }
    
    clock.handleRotaryTurn(clock.alarmTime[1 - clock.alarmMenuCounter.getRelativePosition()][clock.alarmSettingCounter.getRelativePosition()]);
}


void AlarmToneSelection_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.alarmTone = clock.music_player.currentSong();
    clock.music_player.stop();
    stateHandler->changeState(&myAlarmMenuState);
}

void AlarmToneSelection_State::loopAction(InternalState& clock) {
    if (millis() - clock.lastSongCheck > 5000) {
        int currentSong = clock.music_player.currentSong();
        clock.myStrip.showNumber(currentSong);
        clock.myDisplay.showNumber(currentSong);
        clock.lastSongCheck = millis();
    }
    
    if (clock.rotary_state != not_changed) {
        if (clock.rotary_state == turned_up)
            clock.music_player.next();
        else
            clock.music_player.previous();
        
        int currentSong = clock.music_player.currentSong();
        clock.myStrip.showNumber(currentSong);
        clock.myDisplay.showNumber(currentSong);
        
        clock.rotary_state = not_changed;
    }
}


void TimerSettingMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    if (clock.timerSettingCounter.getRelativePosition() != 3) {
        stateHandler->changeState(&myTimerSettingState);
    }
    else {
        if (clock.music_player.isPlaying())
            clock.music_player.stop();
        else {
            clock.my_Timer.setOnOff(!clock.my_Timer.isActive());
        }
    }
}

void TimerSettingMenu_State::loopAction(InternalState& clock) {
    if (clock.my_Timer.isActive()) {
        if (clock.changed())
            clock. myStrip.showTimer(clock.my_Timer.getTime());
    }
    else {
        clock.myStrip.showTimer(clock.timerSelection, clock.timerSettingCounter.getRelativePosition(), millis() / 300);
    }
    
    if (clock.timerSettingCounter.getRelativePosition() != 3)
        clock.myDisplay.showTimer(clock.timerSelection, clock.timerSettingCounter.getRelativePosition(), millis() / 300);
    else
        clock.myDisplay.showBool(clock.my_Timer.isActive());
    
    if (!clock.my_Timer.isActive())
        clock.handleRotaryTurn(clock.timerSettingCounter);
}


void TimerSetting_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.my_Timer.set(clock.timerSelection);
    stateHandler->changeState(&myTimerSettingMenuState);
}

void TimerSetting_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showTimer(clock.timerSelection, clock.timerSettingCounter.getRelativePosition());
        clock.myDisplay.showTimer(clock.timerSelection, clock.timerSettingCounter.getRelativePosition());
    }
    
    clock.handleRotaryTurn(clock.timerSelection[clock.timerSettingCounter.getRelativePosition()]);
}


void Stoppwatch_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    switch (clock.stoppwatchMenuCounter.getRelativePosition()) {
        case 0: // start/stop
            clock.my_Stoppwatch.setOnOff(!clock.my_Stoppwatch.isActive());
            break;
        case 1: // reset
            clock.my_Stoppwatch.reset();
            break;
    }
}

void Stoppwatch_State::loopAction(InternalState& clock) {
    if (clock.changed() && clock.my_Stoppwatch.isActive()) {
        clock.myStrip.showStoppwatch(clock.my_Stoppwatch.getTime(), clock.stoppwatchMenuCounter); // uhr entlasten wenn stoppuhr an
        clock.myDisplay.showBool(clock.my_Stoppwatch.isActive());
    }
    else {
        clock.myStrip.showStoppwatch(clock.my_Stoppwatch.getTime(), clock.stoppwatchMenuCounter);
        clock.myDisplay.showBool(clock.my_Stoppwatch.isActive());
    }
    
    clock.handleRotaryTurn(clock.stoppwatchMenuCounter);
}


void ColorSelectionMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    if (clock.colorSelectionMenuCounter.get() > 44)
        clock.colorTheme.set(clock.colorSelectionMenuCounter.get());
    else {
        stateHandler->changeState(&myColorMenuState);
    }
}

void ColorSelectionMenu_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showColorSelectionMenu(clock.colorSelectionMenuCounter, clock.colorTheme);
        clock.myDisplay.showColorSelectionMenu(clock.colorSelectionMenuCounter);
        clock.myButtons.show(clock.colorTheme);
    }
    
    clock.handleRotaryTurn(clock.colorSelectionMenuCounter);
}


void ColorMenu_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    clock.colorRange.set(clock.colors[clock.colorSelectionMenuCounter.getRelativePosition()][2 - clock.colorMenuCounter.getRelativePosition()] / 5);
    stateHandler->changeState(&myColorValueSelectionState);
}

void ColorMenu_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showColorSelectionMenu(clock.colorMenuCounter, clock.colorTheme, clock.colorSelectionMenuCounter.getRelativePosition());
        clock.myDisplay.clear();
    }
    
    clock.handleRotaryTurn(clock.colorMenuCounter);
}


void ColorValueSelection_State::midButtonPress(InternalState& clock, StateHandler *stateHandler) {
    stateHandler->changeState(&myColorMenuState);
}

void ColorValueSelection_State::loopAction(InternalState& clock) {
    if (clock.changed()) {
        clock.myStrip.showColorSelectionMenu(clock.colorMenuCounter, clock.colorTheme, clock.colorSelectionMenuCounter.getRelativePosition());
        clock.myDisplay.showNumber(clock.colorRange.get());
    }
    
    clock.colors[clock.colorSelectionMenuCounter.getRelativePosition()].setColor(2 - clock.colorMenuCounter.getRelativePosition(), clock.colorRange.get() * 5);
    clock.myButtons.show(clock.colorTheme);
    
    clock.handleRotaryTurn(clock.colorRange);
}
