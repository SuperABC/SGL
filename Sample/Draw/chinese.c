#include "winsgl.h"

void sgSetup() {
	initWindow(640, 480, "SGL Sample", BIT_MAP);
}
void sgLoop() {
	static int first = 1;
	if (first) {
		char *ch = "Hey! 程序猿们！现在我们可以使用SGL库输出汉语了~\n"
			"像这样中英文混合输入也是OK的哟~快来Have a try吧！";
		setColor(0, 0, 0);
		putString(ch, 0, 0);
		first = 0;
	}
}
