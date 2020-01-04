#include "winsgl.h"

void sgSetup() {
	unsigned char plain[2560] = { 0 }, cipher[2560] = { 0 };
	DESEncrypt("ÄãºÃ", 4, "19970920", cipher);
	DESDecrypt(cipher, 4, "19970920", plain);
	return;
}
void sgLoop() {

}