#ifndef Buttons_h
#define Buttons_h	// verhindert Mehrfacheinbindung
#include "types.h"
#include "ComplexCounter.h"

class Buttons {
private:
	struct colorTriple *_colors;
	byte _knobPins[9];
	double *_brightness;

	void setKnobLamp(byte knobNum, int rVal, int gVal, int bVal) {
		rVal *= *_brightness;
		gVal *= *_brightness;
		bVal *= *_brightness;

		switch (knobNum) {
		case 0:
			analogWrite(_knobPins[0], map(rVal, 0, 255, 0, 95));
			analogWrite(_knobPins[1], map(gVal, 0, 255, 0, 220));
			analogWrite(_knobPins[4], bVal);
			break;
		case 1:
			analogWrite(_knobPins[8], map(rVal, 0, 255, 0, 230));
			analogWrite(_knobPins[3], gVal);
			analogWrite(_knobPins[2], map(bVal, 0, 255, 0, 128));
			break;
		default:
			analogWrite(_knobPins[7], map(rVal, 0, 255, 0, 95));
			analogWrite(_knobPins[5], map(gVal, 0, 255, 0, 200)); // zum farbtuning
			analogWrite(_knobPins[6], bVal);
			break;
		}
	}

	void setKnobLamp(byte knobNum, LEDcolor color) {
		setKnobLamp(knobNum, color.red(), color.green(), color.blue());
	}

public:
	Buttons(struct colorTriple &colors, byte knobPins[9], double &brightness, ComplexCounter theme) : _colors(&colors), _brightness(&brightness) {
		for (int i = 0; i < 9; i++) {
			_knobPins[i] = knobPins[i];
		}

		show(theme);
	}

	void show(ComplexCounter theme) {
		switch (theme.get()) {
			case 45: // normal
				setKnobLamp(0, _colors->hourColor);
				setKnobLamp(1, _colors->minuteColor);
				setKnobLamp(2, _colors->secondColor);
				break;
			case 46:
				setKnobLamp(0, _colors->hourColor + _colors->minuteColor);
				setKnobLamp(1, _colors->minuteColor + _colors->secondColor);
				setKnobLamp(2, _colors->secondColor + _colors->hourColor);
				break;
			case 47:
				setKnobLamp(0, _colors->hourColor + _colors->secondColor);
				setKnobLamp(1, _colors->minuteColor + _colors->hourColor);
				setKnobLamp(2, _colors->secondColor + _colors->minuteColor);
				break;
		}
	}

	void clear() {
		setKnobLamp(0, 0, 0, 0);
		setKnobLamp(1, 0, 0, 0);
		setKnobLamp(2, 0, 0, 0);
	}
};

#endif