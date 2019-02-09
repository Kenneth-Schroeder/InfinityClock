#ifndef clock_types_h
#define clock_types_h	// verhindert Mehrfacheinbindung
#include "LEDcolor.h"

struct time {
	int second;
	int minute;
	int hour;
	int weekday;
	int date;
	int month;
	int year;
};

struct colorTriple {
	LEDcolor secondColor;
	LEDcolor minuteColor;
	LEDcolor hourColor;
	colorTriple(LEDcolor st, LEDcolor nd, LEDcolor rd) : secondColor(st), minuteColor(nd), hourColor(rd) {}
	LEDcolor& operator[](int index) {
		switch (index) {
			case 0:
				return secondColor;
				break;
			case 1:
				return minuteColor;
				break;
			default:
				return hourColor;
				break;
		}
	}
};

enum Rotary_State { turned_up, turned_down, not_changed };


#endif
