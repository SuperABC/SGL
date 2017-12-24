#include "main.h"
#include "Music/phone.h"

Phone phone;

void freePlay() {
	int key;
	if (biosKey(1)) {
		key = biosKey(0);
		if(key&0x8000)
			phone.stop(key);
		else
			phone.play(key);
	}
}