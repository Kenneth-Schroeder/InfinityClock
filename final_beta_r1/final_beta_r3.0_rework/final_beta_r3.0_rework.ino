#include "StateHandler.h"

#define BUTTON_UPPER_INPUT 18
#define BUTTON_MID_INPUT 17
#define BUTTON_LOWER_INPUT 16 // Button Pins

StateHandler cState;

/*
 * SETUP *****************************************************************
 */
 
void setup() {
	cState.setup();
 
  pinMode(BUTTON_UPPER_INPUT, INPUT_PULLDOWN);
  pinMode(BUTTON_MID_INPUT, INPUT_PULLDOWN);
  pinMode(BUTTON_LOWER_INPUT, INPUT_PULLDOWN);
  pinMode(DS3234_INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  
  attachInterrupt(BUTTON_UPPER_INPUT, ISR_TopButton, RISING);
  attachInterrupt(BUTTON_MID_INPUT, ISR_MidButton, RISING);
  attachInterrupt(BUTTON_LOWER_INPUT, ISR_BotButton, RISING); // Button Inits
  attachInterrupt(DS3234_INTERRUPT_PIN, ISR_alarm, FALLING);
  attachInterrupt(14, ISR_RotaryPinA, CHANGE);
  attachInterrupt(15, ISR_RotaryPinB, CHANGE);
}

void loop() {
	cState.loop();
}

void ISR_TopButton(){
  cState.ISR_TopButton();
}

void ISR_MidButton(){
  cState.ISR_MidButton();
}

void ISR_BotButton(){
  cState.ISR_BotButton();
}

void ISR_RotaryPinA(){
  cState.ISR_RotaryPinA();
}

void ISR_RotaryPinB(){
  cState.ISR_RotaryPinB();
}

void ISR_alarm(){
  cState.ISR_alarm();
}
