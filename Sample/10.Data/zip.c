#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);

	HANDLE zip = createZip("test.zip");
	zipFolder(zip, "test");
	zipFile(zip, "test.exe", "test\\1");
	zipMemory(zip, "hello", 6, "2");
	zipFinish(zip);

	HANDLE unzip = createUnzip("test.zip");
	SGstring res = malloc(MAX_PATH);
	readUnzip(unzip, 0, res);
	debugf("%s\n", res);
	readUnzip(unzip, 1, res);
	debugf("%s\n", res);
	unzipFile(unzip, res, "uzip.exe");
	SGstring text = malloc(6);
	readUnzip(unzip, 2, res);
	unzipMemory(unzip, res, text, 6);
	debugf("%s\n", text);
}
void sgLoop() {

}