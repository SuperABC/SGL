#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		char *ch = "Hey! ����Գ�ǣ��������ǿ���ʹ��SGL�����������~\n"
			"��������Ӣ�Ļ������Ҳ��OK��Ӵ~����Have a try�ɣ�";
		setColor(0, 0, 0);
		putString(ch, 0, 0);
		first = 0;
	}
}
