#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Init", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		setColor(0, 0, 0);
		setFontSize(20);
		setFontName("΢���ź�");
		putString("SGLv2.2.0��ʼ��\nSGL֧��ȫ�����������", 0, 0);
		putString("��������ֻҪ�����ַ������Ϳ������ı���ʾ����Ļ���ˡ�", 0, 48);

		putString(stringFormat("Ҳ���Ը�ʽ������ַ�����\nHello %s%c\nNext line(%d)\n.", "world", '!', 2), 0, 72);
		first = 0;
	}
}