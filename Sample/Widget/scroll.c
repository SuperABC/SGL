#include "winsgl.h"

int baseX = 800, baseY = 600;

void moveVert(widgetObj *w, int x, int y) {
	mouseMoveScrollVert(w, x, y);

	if (w->status & WIDGET_PRESSED) {
		if (baseY != 600 - w->value * 40) {
			setColor(255, 255, 255);
			putCircle(baseX, baseY, 10, SOLID_FILL);

			baseY = 600 - w->value * 40;
		}
	}
}
void moveHoriz(widgetObj *w, int x, int y) {
	mouseMoveScrollHoriz(w, x, y);

	if (w->status & WIDGET_PRESSED) {
		if (baseX != 800 - w->value) {
			setColor(255, 255, 255);
			putCircle(baseX, baseY, 10, SOLID_FILL);

			baseX = 800 - w->value;
		}
	}
}

void layoutWidget() {
	widgetObj *vertical;
	widgetObj *horizontal;

	vertical = newWidget(SG_SCROLLVERT, "vertical");
	horizontal = newWidget(SG_SCROLLHORIZ, "horizontal");
	vertical->size.y = 640;
	horizontal->size.x = 1200;
	vertical->hide = 10;
	horizontal->hide = 400;

	vertical->mouseIn = vertical->mouseOut = moveVert;
	horizontal->mouseIn = horizontal->mouseOut = moveHoriz;
	registerWidget(vertical);
	registerWidget(horizontal);
	free(vertical);
	free(horizontal);
}

void sgSetup() {
	initWindow(0, 0, "SGL Sample", BIT_MAP);
	initMouse(SG_COORDINATE);
	initKey();
	layoutWidget();
}
void sgLoop() {
	setColor(0, 0, 0);
	putCircle(baseX, baseY, 10, SOLID_FILL);
	return;
}
