#ifndef SoundModule_h
#define SoundModule_h
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include "Counter.h"
#include <Arduino.h>

class SoundModule {
private:
	SoftwareSerial _mySoftwareSerial;
	DFRobotDFPlayerMini _music_player;
	Counter _currentVolume;

public:
	SoundModule() : _mySoftwareSerial(0, 1) /*RX, TX*/ , _currentVolume(0, 25) { }

	void init() {
		_mySoftwareSerial.begin(9600);
		_music_player.begin(_mySoftwareSerial);
		delay(10);
		_currentVolume.set(20);
		_music_player.volume(_currentVolume.get());
		setLoop(true);
	}

	int getVolume() {
		return _currentVolume.get();
	}

	void setVolume(int value) {
		_currentVolume.set(value);
		_music_player.volume(_currentVolume.get());
	}

	void increaseVolume() {
		_currentVolume++;
		_music_player.volume(_currentVolume.get());
	}

	void decreaseVolume() {
		_currentVolume--;
		_music_player.volume(_currentVolume.get());
	}

	void play(int num) {
		_music_player.play(num);
	}

	void stop() {
		_music_player.stop();
	}

	void setLoop(bool on) {
		if(on)
			_music_player.enableLoop();
		else
			_music_player.disableLoop();
	}

	void playRandom() {
		_music_player.randomAll();
	}

	void next() {
		if(isPlaying())
			_music_player.next();
	}

	void previous() {
		if (isPlaying())
			_music_player.previous();
	}

	bool isPlaying() {
		return _music_player.readState() & 1;
	}

	int currentSong() {
		return _music_player.readCurrentFileNumber();
	}
};

#endif // !soundModule_h
