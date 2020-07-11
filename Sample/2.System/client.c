#include "winsgl.h"

SGL_CONSOLE_FRAME

SOCKET client;

NEW_THREAD_FUNC(msgRecv) {
	char buffer[256] = { 0 };
	char *buf;
	while (socketReceive(client, buffer, 256) != SG_CONNECTION_FAILED) {
		buf = buffer;
		while (buf[0]) {
			printf("%s\n", buf);
			buf += strlen(buf) + 1;
		}

		memset(buffer, 0, 256);
	}
	return 0;
}

int main() {
	client = createClient("127.0.0.1", 4497);
	createThread(msgRecv, NULL);
	socketSend(client, "hello");

	while (1);
}