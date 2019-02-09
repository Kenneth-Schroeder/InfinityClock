#ifndef LEDcolor_h
#define LEDcolor_h	// verhindert Mehrfacheinbindung

class LEDcolor {
private:
	int _red;
	int _green;
	int _blue;
	int _mashCounter;

public:
	LEDcolor(int red, int green, int blue) : _red(red), _green(green), _blue(blue) { _mashCounter = 1; }
	
	LEDcolor(int red, int green, int blue, int counter) : _red(red), _green(green), _blue(blue), _mashCounter(counter) {}

	LEDcolor() {
		_red = 0;
		_green = 0;
		_blue = 0;
		_mashCounter = 1;
	}
	
	LEDcolor operator+(LEDcolor ndColor) {
		return LEDcolor(_red + ndColor._red, _green + ndColor._green, _blue + ndColor._blue, _mashCounter + ndColor._mashCounter);
	}

	LEDcolor operator+=(LEDcolor ndColor) {
		_red += ndColor._red;
		_green += ndColor._green;
		_blue += ndColor._blue;
		_mashCounter++;
		return *this;
	}

	LEDcolor operator/(double divisor) {
		return LEDcolor(_red / divisor, _green / divisor, _blue / divisor, _mashCounter);
	}

	LEDcolor operator*(double factor) {
		return LEDcolor(_red * factor, _green * factor, _blue * factor, _mashCounter);
	}

	int operator[](int index) {
		switch (index) {
			case 0:
				return red();
				break;
			case 1:
				return green();
				break;
			default:
				return blue();
				break;
		}
	}
	
	int red() {
		return (_red / _mashCounter) % 256;
	}

	int green() {
		return (_green / _mashCounter) % 256;
	}

	int blue() {
		return (_blue / _mashCounter) % 256;
	}

	void setColor(int index, int value) {
		switch(index) {
			case 0:
				_red = value * _mashCounter;
				break;
			case 1:
				_green = value * _mashCounter;
				break;
			default:
				_blue = value * _mashCounter;
				break;
		}
	}

};

#endif