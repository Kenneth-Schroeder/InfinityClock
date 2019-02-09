#ifndef StateHandler_h
#define StateHandler_h	// verhindert Mehrfacheinbindung

#include "ExternalStates.h"
#include "InternalState.h"

#define strip_dataPin 10
#define strip_length 126

#define DS3234_SS_PIN 8 // DeadOn my_rtc Chip-select pin
#define DISPLAY_SS_PIN 7

#define DS3234_INTERRUPT_PIN 2 // DeadOn my_rtc SQW/interrupt pin (optional)

#define SPI_SCK_CLOCK 13     //
#define SPI_MOSI_DATA 11             // SPI PINS
#define SPI_MISO_DISABLE 12          // NICHT GENUTZT

extern Time_State myTimeState;
extern MainMenu_State myMainMenuState;
extern BrightnessSelection_State myBrightnessSelectionState;
extern VolumeSelection_State myVolumeSelectionState;
extern Demo_State myDemoState;
extern Accuracy_State myAccuracyState;
extern MusicMenu_State myMusicMenuState;
extern SettingTimeMenu_State mySettingTimeMenuState;
extern SettingTime_State mySettingTimeState;
extern AlarmMenu_State myAlarmMenuState;
extern AlarmEnabling_State myAlarmEnablingState;
extern AlarmRepetitionSelection_State myAlarmRepetitionSelectionState;
extern AlarmSettingMenu_State myAlarmSettingMenuState;
extern AlarmSetting_State myAlarmSettingState;
extern AlarmToneSelection_State myAlarmToneSelectionState;
extern TimerSettingMenu_State myTimerSettingMenuState;
extern TimerSetting_State myTimerSettingState;
extern Stoppwatch_State myStoppwatchState;
extern ColorSelectionMenu_State myColorSelectionMenuState;
extern ColorMenu_State myColorMenuState;
extern ColorValueSelection_State myColorValueSelectionState;

class StateHandler {
private:
    InternalState _iS;
    IState *externalState;

public:
    void setup();
    void loop();
    void display();
    void turnOn();
    void turnOff();
    void handleAlarmResets();
    void ISR_BotButton();
    void ISR_alarm();
    void ISR_RotaryPinA();
    void ISR_RotaryPinB();
    void ISR_TopButton();
    void ISR_MidButton();
    void changeState(IState* anIS);
};

#endif
