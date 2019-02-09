#ifndef SegDisplay_h
#define SegDisplay_h	// verhindert Mehrfacheinbindung

#include <SPI.h>
#include "types.h"

class SegDisplay {
private:
	int _csPin;
	byte number[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111 }; // display_number for 7-Segment Display
	bool (*_shouldDisplay)[7];

// POINT, MIDDLE, TOPLEFT, BOTLEFT, BOT, BOTRIGHT, TOPRIGHT, TOP
// 00111111 O
// 01110001 F
// 01101110 Y
// 01111001 E
// 01010100 n
	void showON() {
		SPI.transfer(0b01010100);
		SPI.transfer(0b00111111);
		digitalWrite(_csPin, LOW);
		digitalWrite(_csPin, HIGH);
	}

	void showOF() {
		SPI.transfer(0b01110001);
		SPI.transfer(0b00111111);
		digitalWrite(_csPin, LOW);
		digitalWrite(_csPin, HIGH);
	}

public:
	SegDisplay(int csPin, bool (&shouldDisplay)[7]) : _csPin(csPin), _shouldDisplay(&shouldDisplay) {

		digitalWrite(_csPin, HIGH);
		SPI.setBitOrder(MSBFIRST);
		SPI.begin();						 // SPI INIT
	}
	
	void clear() {
		SPI.transfer(0b00000000);
		SPI.transfer(0b00000000);
		digitalWrite(_csPin, LOW);
		digitalWrite(_csPin, HIGH);
	}

	void showTime(const struct time& currentTime) {
		clear();

		for (int i = 0; i < 7; i++) {
			if ((*_shouldDisplay)[i]) {
				switch (i)
				{
				case 0: // year
					if (currentTime.second % 5 == 2) {
						showNumber(currentTime.year % 100);
					}
					break;
				case 1: // month
					if (currentTime.second % 5 == 1) {
						showNumber(currentTime.month);
					}
					break;
				case 2: // day
					if (currentTime.second % 5 == 0) {
						showNumber(currentTime.date);
					}
					break;
				case 3: // Weekday
					if (currentTime.second % 5 == 4) {
						showNumber(currentTime.weekday);
					}
					break;
				case 4: // hour
					break;
				case 5: // minute
					break;
				case 6: // second
					break;
				}
			}
		}
	}

	void showNumber(int num) {
		SPI.transfer(number[num % 10]); // DOT, G, F, E, D, C, B, A
		SPI.transfer(number[(num / 10) % 10]); // DOT, G, F, E, D, C, B, A
		digitalWrite(_csPin, LOW);
		digitalWrite(_csPin, HIGH);
	}

	void showAlarmTime(ComplexCounter alarmTime[3], int blink, int sec) {
		clear();

		if (blink == 0 && sec % 2 == 0) {
			showNumber(alarmTime[0].get());
		}

		if (blink == 1 && sec % 2 == 0) {
			showNumber(alarmTime[1].get());
		}

		if (blink == 2 && sec % 2 == 0) {
			showNumber(alarmTime[2].get());
		}
	}

	void showAlarmTime(ComplexCounter alarmTime[3], int selection) {
		clear();

		if (selection == 0) {
			showNumber(alarmTime[0].get());
		}

		if (selection == 1) {
			showNumber(alarmTime[1].get());
		}

		if (selection == 2) {
			showNumber(alarmTime[2].get());
		}
	}

	void showTimer(ComplexCounter timerTime[3], int blink, int sec) {
		showAlarmTime(timerTime, blink, sec);
	}

	void showTimer(ComplexCounter timerTime[3], int selection) {
		showAlarmTime(timerTime, selection);
	}

	void showColorSelectionMenu(ComplexCounter menuCounter) {
		clear();

		if(menuCounter.get() > 44)
			showNumber(menuCounter.get() - 45);
	}

	void showBool(bool value) {
		if (value)
			showON();
		else
			showOF();
	}

};

#endif