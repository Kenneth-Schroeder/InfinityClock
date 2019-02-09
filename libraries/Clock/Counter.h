#ifndef Counter_h
#define Counter_h

class Counter {
protected:
	int _lowerLimit;
	int _upperLimit;
	int _counter;

public:
	Counter(int lowerLimit, int upperLimit) : _lowerLimit(lowerLimit), _upperLimit(upperLimit) {
		if (lowerLimit > upperLimit) {
			_lowerLimit = upperLimit;
			_upperLimit = lowerLimit;
		}
		_counter = _lowerLimit;
	}

	virtual void set(int value) {
		if (value >= _lowerLimit && value <= _upperLimit) {
			_counter = value;
		}
	}

	int get() {
		return _counter;
	}

	int operator+(int value) {
		if (value > 0 && value < _upperLimit - _lowerLimit + 1) {
			int result = _counter + value;
			if (result > _upperLimit)
				result = _lowerLimit + (result - _upperLimit - 1); // bsp: 6, 58 ; _c = 50, val = 24 => res = 74 > uL => res = 6 + (74 - 58 - 1) = 21

			return result;
		}
		else
			return _counter;
	}

	int operator-(int value) {
		if (value > 0 && value < _upperLimit - _lowerLimit + 1) {
			int result = _counter - value;
			if (result < _lowerLimit)
				result = _upperLimit - (_lowerLimit - result - 1);

			return result;
		}
		else
			return _counter;
	}

	virtual int operator++(int) {
		int remember = _counter;
		if (_counter == _upperLimit)
			_counter = _lowerLimit;
		else
			_counter++;
		return remember;
	}

	virtual int operator--(int) {
		int remember = _counter;
		if (_counter == _lowerLimit)
			_counter = _upperLimit;
		else
			_counter--;
		return remember;
	}
};

#endif // !Counter_h