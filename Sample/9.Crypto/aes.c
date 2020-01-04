#include "winsgl.h"

void sgSetup() {
	unsigned char plain[2560] = { 0 }, cipher[2560] = { 0 };
	DESEncrypt("ÄãºÃ123456789", 13, "1997092019970920", cipher);
	DESDecrypt(cipher, 13, "1997092019970920", plain);
	return;
}
void sgLoop() {

}