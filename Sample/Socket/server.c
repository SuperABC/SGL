#include "winsgl.h"

SOCKET server, connection;

void layoutWidget() {
	widgetObj *Output;

	Output = newWidget(SG_OUTPUT, "Output");
	Output->pos.x = 40;
	Output->pos.y = 40;
	Output->size.x = 560;
	Output->size.y = 400;
	strcpy(Output->content, "");
	registerWidget(Output);
}
void singleCommun(void) {
	SOCKET tmp = connection;
	char buf[64];

	while (1) {
		if (socketReceive(tmp, buf, 64) == SG_CONNECTION_FAILED)break;

		widgetObj *output = getWidgetByName("Output");
		strcat(output->content, buf);
	}
	closeSocket(tmp);
}
void resposeReq(void) {
	while (1) {
		connection = acceptOne(server);
		createThread(singleCommun);
	}
}

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();

	server = createServer(6000);
	createThread(resposeReq);
}
void sgLoop() {
	return;
}