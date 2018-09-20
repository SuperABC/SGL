#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL sample", BIT_MAP);
	initKey();
}
void sgLoop() {
	char *file = (char *)malloc(260);

	if (biosKey(1)) {
		biosKey(0);
		if(selectDir(file, NULL) == SG_NO_ERORR)
			alertInfo(file, "ѡ���ļ���", 0);
		if (selectFile(file, NULL, "All(*.*)\0*.*\0Text(.txt)\0*.txt\0", 0) == SG_NO_ERORR)
			alertInfo(file, "ѡ���ļ�", 0);
		if(selectSave(file, NULL, "All(*.*)\0*.*\0Text(.txt)\0*.txt\0",
			"*.txt", 0) == SG_NO_ERORR)
			alertInfo(file, "�����ļ�", 0);
	}
	clearKeyBuffer();
}