#include "phone.h"

Phone::Phone() {
	tune = 0x3C;
	instrument = 0;
	volume = 100;
}
Phone::~Phone() {}

void Phone::play(char c) {
	switch (c) {
	case '1':
		tune = KK_1;
		break;
	case '2':
		tune = KK_2;
		break;
	case '3':
		tune = KK_3;
		break;
	case '4':
		tune = KK_4;
		break;
	case '5':
		tune = KK_5;
		break;
	case '6':
		tune = KK_6;
		break;
	case '7':
		tune = KK_7;
		break;
	case '8':
		tune = KK_8;
		break;
	}
	if(state[tune]==false)
		playMidi(tune, volume, 1);
	state[tune] = true;
}
void Phone::stop(char c) {
	switch (c) {
	case '1':
		playMidi(KK_1, volume, 0);
		break;
	case '2':
		playMidi(KK_2, volume, 0);
		break;
	case '3':
		playMidi(KK_3, volume, 0);
		break;
	case '4':
		playMidi(KK_4, volume, 0);
		break;
	case '5':
		playMidi(KK_5, volume, 0);
		break;
	case '6':
		playMidi(KK_6, volume, 0);
		break;
	case '7':
		playMidi(KK_7, volume, 0);
		break;
	case '8':
		playMidi(KK_8, volume, 0);
		break;
	}
	state[tune] = false;
}
void Phone::setVolume(int v) {
	if (v < 0)v = 0;
	if (v > 100)v = 100;
	this->volume = v;
}