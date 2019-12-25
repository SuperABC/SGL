#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "Init", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		setColor(0, 0, 0);
		setFontSize(20);
		setFontName("微软雅黑");
		putString("SGLv2.2.0开始，\nSGL支持全部字体输出。", 0, 0);
		putString("像这样，只要传入字符串，就可以让文本显示在屏幕上了。", 0, 48);

		putString(stringFormat("也可以格式化输出字符串：\nHello %s%c\nNext line(%d)\n.", "world", '!', 2), 0, 72);
		first = 0;
	}
}