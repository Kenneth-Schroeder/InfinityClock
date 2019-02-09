#ifndef TimeCounter_h
#define TimeCounter_h
#include "Counter.h"

class TimeCounter {
protected:
	ComplexCounter _seconds;
	ComplexCounter _minutes;
	ComplexCounter _hours;
	ComplexCounter _myArr[3];
	bool activated = false;

public:
	TimeCounter() : _seconds(0, 59), _minutes(0, 59), _hours(0, 59), _myArr{ ComplexCounter(0,59), ComplexCounter(0,59), ComplexCounter(0,59) } {}
	TimeCounter(int seconds, int minutes, int hours) : _seconds(0, 59), _minutes(0, 59), _hours(0, 59), _myArr{ComplexCounter(0,59), ComplexCounter(0,59), ComplexCounter(0,59)} {
		_seconds.set(seconds);
		_minutes.set(minutes);
		_hours.set(hours);
	}

	int getSeconds() {
		return _seconds.get();
	}

	int getMinutes() {
		return _minutes.get();
	}

	int getHours() {
		return _hours.get();
	}

	ComplexCounter* getTime() {		
		_myArr[0].set(_seconds.get());
		_myArr[1].set(_minutes.get());
		_myArr[2].set(_hours.get());

		return _myArr;
	}

	void set(int seconds, int minutes, int hours) {
		_seconds.set(seconds);
		_minutes.set(minutes);
		_hours.set(hours);
	}

	void set(ComplexCounter time[3]) {
		_seconds.set(time[0].get());
		_minutes.set(time[1].get());
		_hours.set(time[2].get());
	}

	bool isActive() {
		return activated;
	}

	void setOnOff(bool enable) {
		activated = enable;
	}
};

class Timer : public TimeCounter {
private:
	
public:
	Timer(int seconds, int minutes, int hours) : TimeCounter(seconds, minutes, hours) { }

	bool operator--(int) {
		if (_seconds.get() == 1 && _minutes.get() == 0 && _hours.get() == 0) { // second springt auf 0, falls minute und hour auch 0, return true zum Alarm
			_seconds--;
			activated = false;
			return true;
		}
		else if (_seconds.get() == 0) { // seconds war 0, jetzt bei 59
			if (_minutes-- == 0) {
				_hours--;
			}
		}

		_seconds--;

		return false;
	}
};

class Stoppwatch : public TimeCounter {
public:
	Stoppwatch() : TimeCounter() { }

	void operator++(int) {
		if (_seconds++ == 59) { // eine minute ist vorbei
			if (_minutes++ == 59) { // eine stunde ist vorbei
				_hours++;
			}
		}
	}

	void reset() {
		_seconds.set(0);
		_minutes.set(0);
		_hours.set(0);
	}
};

#endif // !timeCounter_h
