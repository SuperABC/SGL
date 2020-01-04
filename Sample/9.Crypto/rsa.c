#include "winsgl.h"

void sgSetup() {
	unsigned int cipher, plain;
	unsigned int p = 499, q = 479, e, d, n;
	ProduceKey(p, q, &e, &d, &n);
	RSAEncrypt(1234, e, n, &cipher);
	RSADecrypt(cipher, d, n, &plain);
	return;
}
void sgLoop() {

}