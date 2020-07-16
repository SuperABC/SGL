#include "winsgl.h"

int color = 32;

void sgSetup() {
	initWindow(640, 480, "File", BIT_MAP);
}
void sgLoop() {
	char *file = (char *)malloc(260);

	if (biosKey(1)) {
		biosKey(0);

		makePath("C:/test");

		if (selectDir(file, "") == SG_NO_ERORR)
			alertInfo("Dir", file, 0, NULL);

		if (selectFile(file, L"C:/test", L"All files\0*.*\0C/C++file\0 * .h; *.c; *.cpp\0\0", 1) == SG_NO_ERORR)
			alertInfo("File", file, 0, NULL);

		if (selectSave(file, L"C:/test", L"All files\0*.*\0C/C++file\0 * .h; *.c; *.cpp\0\0", L"c", 1) == SG_NO_ERORR)
		if(fileExist(file)) alertInfo("Exist", file, 0, NULL);
		else alertInfo("Save", file, 0, NULL);
	}
	clearKeyBuffer();
}