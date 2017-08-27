#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		setColor(0, 0, 0);
		setFontSize(20);
		setFontName("楷体");
		putString("SGLv2.2.0开始，SGL支持全部字体输出。", 0, 0);
		putString("像这样，只要传入字符串，就可以让文本显示在屏幕上了。", 0, 24);
		first = 0;
	}
}
