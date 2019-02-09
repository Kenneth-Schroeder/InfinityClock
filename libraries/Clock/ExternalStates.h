#ifndef states_h
#define states_h	// verhindert Mehrfacheinbindung

class InternalState;
class StateHandler;

class IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler) = 0;
    virtual void loopAction(InternalState& clock) = 0;
};

class Time_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class MainMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class BrightnessSelection_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class VolumeSelection_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class Demo_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class Accuracy_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class MusicMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class SettingTimeMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class SettingTime_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class AlarmMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class AlarmEnabling_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class AlarmRepetitionSelection_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class AlarmSettingMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class AlarmSetting_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class AlarmToneSelection_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class TimerSettingMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class TimerSetting_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class Stoppwatch_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class ColorSelectionMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class ColorMenu_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

class ColorValueSelection_State : public IState {
public:
    virtual void midButtonPress(InternalState& clock, StateHandler *stateHandler);
    virtual void loopAction(InternalState& clock);
};

#endif
