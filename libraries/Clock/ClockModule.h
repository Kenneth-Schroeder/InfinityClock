#ifndef ClockModule_h
#define ClockModule_h

#include "SparkFunDS3234RTC.h" // DS3234 header

class ClockModule {
private:
	int _csPin;
	int _interruptPin;
	int _lastSecond = 0;
	struct time *_clockTime;

	int modulo(int x, int m) {
		return (x%m + m) % m;
	}	

public:
	ClockModule(int csPin, int interruptPin, struct time &clockTime) : _csPin(csPin), _interruptPin(interruptPin), _clockTime(&clockTime) { }

	void init() {
		rtc.begin(_csPin);
		//rtc.set24Hour();

		//rtc.autoTime();
		//rtc.setTime(0, 42, 12, 6, 8, 4, 17);  // sec, min, hour, weekday, day, month, year
		//rtc.setHour(13);

		// Update time/date values, so we can set alarms
		rtc.update();
		// Configure Alarm(s):
		// (Optional: enable SQW pin as an intterupt)
		rtc.enableAlarmInterrupt();
        enableAlarm(0, false);
        enableAlarm(1, false);
		// Set alarm1 to alert when seconds hits 30
		//rtc.setAlarm1(30);
		// Set alarm2 to alert when minute increments by 1
		//rtc.setAlarm2(rtc.minute() + 1);
	}

	bool secondChanged() {
		if (_lastSecond != rtc.second()) {
			_lastSecond = rtc.second();
			updateTime();
			return true;
		}
		else
			return false;
	}

	void updateTime() {
		_clockTime->year = rtc.year();
		_clockTime->month = rtc.month();
		_clockTime->date = rtc.date();
		_clockTime->weekday = modulo(rtc.day() - 2, 7) + 1; //  1 bis 7, Montag bis Sonntag, 1-7 von Sonntag bis Samstag kommt von rtc.day()
		_clockTime->hour = rtc.hour();
		_clockTime->minute = rtc.minute();
		_clockTime->second = rtc.second();
	}

	int second() {
		return rtc.second();
	}

	int minute() {
		return rtc.minute();
	}

	int hour() {
		return rtc.hour();
	}

	int weekDay() {
		return rtc.day();
	}

	int date() {
		return rtc.date();
	}

	int month() {
		return rtc.month();
	}

	int year() {
		return rtc.year();
	}

	void setSecond(int second) {
		rtc.setSecond(second);
	}

	void setMinute(int minute) {
		rtc.setMinute(minute);
	}

	void setHour(int hour) {
		rtc.setHour(hour);
	}

	void setWeekday(int weekday) {
		rtc.setDay(weekday);
	}

	void setDate(int date) {
		rtc.setDate(date);
	}

	void setMonth(int month) {
		rtc.setMonth(month);
	}

	void setYear(int year) {
		rtc.setYear(year);
	}

	void setSecondByOffset(int offset) {
		setSecond(modulo(offset + _clockTime->second, 60));
	}

	void setMinuteByOffset(int offset) {
		setMinute(modulo(offset + _clockTime->minute, 60));
	}

	void setHourByOffset(int offset) {
		setHour(modulo(offset + _clockTime->hour, 24));
	}

	void setWeekdayByOffset(int offset) {
		setWeekday(modulo(offset + _clockTime->weekday, 7) + 1); // 1-7 Mo bis So, %7 wird 0-6 So bis Sa
	}

	void setDateByOffset(int offset) {
		setDate(modulo(offset + _clockTime->date, 31) + 1);
	}

	void setMonthByOffset(int offset) {
		setMonth(modulo(offset + _clockTime->month, 12) + 1);
	}

	void setYearByOffset(int offset) {
		setYear(modulo(offset + _clockTime->year, 100) + 2000);
	}

	// GET BY OFFSET --------------------------------------------------

	int getSecondWithOffset(int offset) {
		return modulo(offset + _clockTime->second, 60);
	}

	int getMinuteWithOffset(int offset) {
		return modulo(offset + _clockTime->minute, 60);
	}

	int getHourWithOffset(int offset) {
		return modulo(offset + _clockTime->hour, 24);
	}

	int getWeekdayWithOffset(int offset) {
		return modulo(offset + _clockTime->weekday - 1, 7) + 1;
	}

	int getDateWithOffset(int offset) {
		return modulo(offset + _clockTime->date - 1, 31) + 1;
	}

	int getMonthWithOffset(int offset) {
		return modulo(offset + _clockTime->month - 1, 12) + 1;
	}

	int getYearWithOffset(int offset) {
		return modulo(offset + _clockTime->year, 100);
	}

	void setAlarm(int alarmNr, int minute, int hour, int DoM) {
		if (alarmNr == 0) {
			rtc.setAlarm1(0, minute, hour, DoM, false);
		}
		else {
			rtc.setAlarm2(minute, hour, DoM, false);
		}
	}

	void setAlarm(int alarmNr, ComplexCounter alarmTime[3]) {
		int minute = alarmTime[0].get();
		int hour = alarmTime[1].get();
		int DoM = alarmTime[2].get();

		if (alarmNr == 0) {
			rtc.setAlarm1(0, minute, hour, DoM, false);
		}
		else {
			rtc.setAlarm2(minute, hour, DoM, false);
		}
	}

	void enableAlarm(int alarmNr, bool enable) {
		rtc.enableDisableAlarm(alarmNr, enable);
	}

	int alarmThings() {
		if (rtc.alarm1())
			return 0;
		if (rtc.alarm2())
			return 1;
		return 2;
	}

	void update() {
		rtc.update();
	}
};

#endif
