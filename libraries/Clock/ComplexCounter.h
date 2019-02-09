#ifndef ComplexCounter_h
#define ComplexCounter_h
#include "Counter.h"

class ComplexCounter : public Counter {
private:
	int _endPoint = 0;
	static const int _arrSize = 60;
	int _jumpPoints[_arrSize];
	

	int getJumpsInRange(int lowerInt, int upperInt) {
		int counter = 0;
		for (int i = 0; i < _arrSize; i++) {
			if (_jumpPoints[i] >= lowerInt && _jumpPoints[i] <= upperInt)
				counter++;
		}
		return counter;
	}

public:
    ComplexCounter() : Counter(0,1) {}
    
	ComplexCounter(int lowerLimit, int upperLimit) : Counter(lowerLimit, upperLimit) {
		for (int i = 0; i < _arrSize; i++) {
			_jumpPoints[i] = -123456;
		}
	}
	ComplexCounter(int lowerLimit, int upperLimit, bool addStdJumps) : Counter(lowerLimit, upperLimit) {
		for (int i = 0; i < _arrSize; i++) {
			_jumpPoints[i] = -123456;
		}

		if (addStdJumps)
			addStandardJumps();
	}

	void addJumpAt(int value) {
		if (value >= _lowerLimit && value <= _upperLimit && _endPoint < 10) {
			_jumpPoints[_endPoint++] = value;
		}
	}

	bool isJumpPoint(int value) {
		for (int i = 0; i < _endPoint; i++) {
			if (value == _jumpPoints[i])
				return true; 
		}
		return false;
	}

	void addStandardJumps() {
		for (int i = _lowerLimit; i <= _upperLimit; i++) {
			if (i % 5 == 0)
				addJumpAt(i);
		}
	}

	virtual void set(int value) {
		if(!isJumpPoint(value)) {
			Counter::set(value);
		}
	}

	virtual int operator++(int) {
		int remember = _counter;

		int jump = 1;
		while (isJumpPoint(*this + jump))
			jump++;

		Counter::set(*this + jump);

		return remember;
	}

	virtual int operator--(int) {
		int remember = _counter;
		
		int jump = 1;
		while (isJumpPoint(*this - jump))
			jump++;

		Counter::set(*this - jump);

		return remember;
	}

	int getRelativePosition() {
		return _counter - _lowerLimit - getJumpsInRange(_lowerLimit, _counter); // minus every jump in that range
	}

};

#endif
