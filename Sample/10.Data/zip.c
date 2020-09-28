#include "winsgl.h"

char buf[65536];

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);

	HANDLE zip = createZip("test.zip");
	zipFolder(zip, "test");
	zipFile(zip, "test.exe", "test\\1");
	zipMemory(zip, "hello", 6, "2");
	zipFinish(zip);

	HANDLE zip1 = createZip("test1.zip");
	zipFile(zip1, "test.zip", "test.zip");
	zipFinish(zip1);

	HANDLE unzip1 = createUnzip("test1.zip");
	SGstring res = malloc(MAX_PATH);
	readUnzip(unzip1, 0, res);
	unzipFile(unzip1, res, "unzip.zip");
	zipFinish(unzip1);

	HANDLE unzip = createUnzip("unzip.zip");
	readUnzip(unzip, 0, res);
	debugf("%s\n", res);
	readUnzip(unzip, 1, res);
	debugf("%s\n", res);
	unzipFile(unzip, res, "unzip.exe");
	SGstring text = malloc(6);
	readUnzip(unzip, 2, res);
	unzipMemory(unzip, res, text, 6);
	debugf("%s\n", text);
	readUnzip(unzip, 3, res);
	debugf("%s\n", res);
	zipFinish(unzip);
}
void sgLoop() {

}