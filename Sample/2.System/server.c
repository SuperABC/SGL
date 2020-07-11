#include "winsgl.h"

SGL_CONSOLE_FRAME

SOCKET server, connection;

NEW_THREAD_FUNC(mainHandler) {
	char *str = (char *)param;
	SOCKET socket;
	memcpy(&socket, (char *)param + strlen((char *)param) + 1, sizeof(socket));

	printf("%s\n", str);
	socketSend(socket, "received");
	return 0;
}
NEW_THREAD_FUNC(singleMsg) {
	SOCKET tmp = *(SOCKET *)param;
	char buffer[256] = { 0 };
	char *buf;

	while (socketReceive(tmp, buffer, 256) != SG_CONNECTION_FAILED) {
		buf = buffer;
		while (buf[0]) {
			char *param = (char *)malloc(256);
			memcpy(param, buf, strlen(buf) + 1);
			memcpy(param + strlen(buf) + 1, &tmp, sizeof(tmp));
			createThread(mainHandler, param);
			buf += strlen(buf) + 1;
		}
		memset(buffer, 0, 256);
	}
	closeSocket(tmp);
	free(param);
	return 0;
}
NEW_THREAD_FUNC(socketResponse) {
	int nodelay = TRUE;
	while (1) {
		connection = acceptOne(server);
		setsockopt(connection, SOL_SOCKET, TCP_NODELAY, (const char *)&nodelay, sizeof(int));

		SOCKET *param = (SOCKET *)malloc(sizeof(SOCKET));
		*param = connection;
		createThread(singleMsg, param);
	}
	return 0;
}

int main() {
	server = createServer(4497);
	createThread(socketResponse, NULL);
	printf("Server is on.");

	while (1);
}
