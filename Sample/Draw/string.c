#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		setColor(0, 0, 0);
		setFontSize(20);
		setFontName("΢���ź�");
		putString("SGLv2.2.0��ʼ��\nSGL֧��ȫ�����������", 0, 0);
		putString("��������ֻҪ�����ַ������Ϳ������ı���ʾ����Ļ���ˡ�", 0, 24);

		putStringFormat("Hello %s%c\nNext line(%d)\n.", 0, 48, "world", '!', 2);
		first = 0;
	}
}
