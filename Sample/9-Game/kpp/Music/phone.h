#ifndef KPP_PHONE
#define KPP_PHONE
#include <winsgl.h>

enum kppKey {
	KK_1 = 0x48,
	KK_2 = 0x4A,
	KK_3 = 0x4C,
	KK_4 = 0x4D,
	KK_5 = 0x4F,
	KK_6 = 0x51,
	KK_7 = 0x53,
	KK_8 = 0x54
};

class Phone {
private:
	int tune, instrument, volume;
	bool state[0x100] = { 0 };
public:
	Phone();
	~Phone();

	void play();
	void play(char c);
	void stop(char c);
	void tuneUp();
	void tuneDown();
	void setVolume(int v);
	void changeInstrument(int inst);
};

#endif
