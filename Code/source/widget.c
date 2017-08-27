#define _CRT_SECURE_NO_WARNINGS
#include "inner.h"
#include "..\include\winsgl.h"

extern int _sglMode;
extern int _inLoop;

//Inner widget callback functions.
void mouseMoveDefault(widgetObj *w, int x, int y) {
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y>=w->pos.y&&y < w->pos.y + w->size.y) {
		w->status |= WIDGET_PASS;
	}
	else {
		w->status &= 0xFF ^ WIDGET_PASS;
		w->status &= 0xFF ^ WIDGET_PRESSED;
	}
}
void mouseMoveList(widgetObj *w, int x, int y) {
	if (w->status&WIDGET_SELECTED) {
		if (w->status&WIDGET_PRESSED&&
			x >= w->pos.x&&x < w->pos.x + w->size.x&&
			y >= w->pos.y + w->size.y&&y < w->pos.y + w->size.y + w->hide * SG_LINE_DELTA_DEFAULT) {
				w->status |= WIDGET_PRESSED;
				w->value = (y - w->pos.x - w->size.y) / SG_LINE_DELTA_DEFAULT;
		}
		else {
			if (x >= w->pos.x&&x < w->pos.x + w->size.x&&
				y >= w->pos.y&&y < w->pos.y + w->size.y + w->hide * SG_LINE_DELTA_DEFAULT) {
				w->status |= WIDGET_PASS;
			}
			else {
				w->status &= 0xFF ^ WIDGET_PASS;
				w->status &= 0xFF ^ WIDGET_PRESSED;
			}
		}
	}
	else {
		if (x >= w->pos.x&&x < w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
			w->status |= WIDGET_PASS;
		}
		else {
			w->status &= 0xFF ^ WIDGET_PASS;
			w->status &= 0xFF ^ WIDGET_PRESSED;
		}
	}
}
void mouseMoveOption(widgetObj *w, int x, int y) {
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
		w->status |= WIDGET_PASS;
		w->value = (y - w->pos.y) / SG_LINE_DELTA_DEFAULT;
	}
	else {
		w->status &= 0xFF ^ WIDGET_PASS;
		w->status &= 0xFF ^ WIDGET_PRESSED;
	}
}
void mouseMoveDrag(widgetObj *w, int x, int y) {
	if (w->status & WIDGET_PRESSED) {
		if (x < w->pos.x + 6)w->value = 0;
		else if (x >= w->pos.x + w->size.x - 6)w->value = 100;
		else {
			w->value = (x - w->pos.x - 6) * 100 / (w->size.x - 12);
		}
	}
	if (x >= w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 2 &&
		x < w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 10 &&
		y >= w->pos.y&&y < w->pos.y + w->size.y) {
		w->status |= WIDGET_PASS;
	}
	else {
		w->status &= 0xFF ^ WIDGET_PASS;
	}
}
void mouseClickDefault(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y>=w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON))w->status |= WIDGET_PRESSED;
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status |= WIDGET_SELECTED;
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active !=-1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name)==0)
			Widget->active = -1;
		w->status &= 0xFF ^ WIDGET_SELECTED;
	}
}
void mouseClickInput(widgetObj *w, int x, int y, int status) {
	int i = 0, len;
	len = strlen(w->content);

	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y>=w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_PRESSED;
			w->status |= WIDGET_FOCUSED;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			if (w->content[0] == '\0')w->value = 0;
			else if (x >= w->pos.x + 2&&x < w->pos.x + w->size.x - 2) {
				w->value = (x - w->pos.x + SG_CHAR_WIDTH / 2) / SG_CHAR_WIDTH - 1 + w->hide;
				if (w->value >= len)w->value = len;
				if (w->content[w->value] >= 128) {
					i = 0;
					do {
						i++;
					} while (w->content[w->value - i] >= 128);
					if ((i & 1) == 0)w->value--;
				}
			}
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		w->status &= 0xFF ^ WIDGET_SELECTED;
		w->status &= 0xFF ^ WIDGET_FOCUSED;
	}
	return;
}
void mouseClickDialog(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y>=w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (x >= w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1) &&
				x < w->pos.x + w->size.x - 2 &&
				y >= w->pos.y + 2 &&
				y < w->pos.y + SG_CHAR_HEIGHT + 2) {
				w->status |= WIDGET_PRESSED;
			}
			else {
				w->status |= WIDGET_FOCUSED;
			}
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			if (x >= w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1) &&
				x < w->pos.x + w->size.x - 2 &&
				y >= w->pos.y + 2 &&
				y < w->pos.y + SG_CHAR_HEIGHT + 2) {
				ceaseWidget(w->name);
				Widget->active = -1;
				return;
			}
			w->status |= WIDGET_SELECTED;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		w->status &= 0xFF ^ WIDGET_SELECTED;
		w->status &= 0xFF ^ WIDGET_FOCUSED;
	}
}
void mouseClickList(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (w->status&WIDGET_SELECTED) {
		if (x >= w->pos.x&&x < w->pos.x + w->size.x&&
			y >= w->pos.y + w->size.y - w->hide * SG_LINE_DELTA_DEFAULT&&y < w->pos.y + w->size.y) {
			if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				w->status |= WIDGET_PRESSED;
				w->value = (w->hide * SG_LINE_DELTA_DEFAULT + y - w->pos.y - w->size.y) / SG_LINE_DELTA_DEFAULT;
			}
			if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				w->status &= 0xFF ^ WIDGET_SELECTED;
				w->size.y -= w->hide*SG_LINE_DELTA_DEFAULT;
				putImage(w->pos.x, w->pos.y, w->cover, COPY_PUT);
			}
		}
		else if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->size.y -= w->hide*SG_LINE_DELTA_DEFAULT;
			putImage(w->pos.x, w->pos.y, w->cover, COPY_PUT);
		}
	}
	else {
		if (x >= w->pos.x&&x < w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
			if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
			if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				w->status |= WIDGET_PRESSED;
				w->status |= WIDGET_SELECTED;
				w->size.y += w->hide*SG_LINE_DELTA_DEFAULT;
				free(w->cover->data);
				getImage(w->pos.x, w->pos.y, w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, w->cover);
			}
		}
		else {
			if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
		}
	}
}
void mouseClickCheck(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_PRESSED;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			if (w->value)w->value--;
			else w->value = 3;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		w->status &= 0xFF ^ WIDGET_SELECTED;
	}
}
void mouseClickProcess(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (x >= w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1) &&
				x < w->pos.x + w->size.x - 2 &&
				y >= w->pos.y + 2 &&
				y < w->pos.y + SG_CHAR_HEIGHT + 2) {
				w->status |= WIDGET_PRESSED;
			}
			else {
				w->status |= WIDGET_FOCUSED;
			}
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			if (x >= w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1) &&
				x < w->pos.x + w->size.x - 2 &&
				y >= w->pos.y + 2 &&
				y < w->pos.y + SG_CHAR_HEIGHT + 2) {
				ceaseWidget(w->name);
				Widget->active = -1;
				return;
			}
			w->status |= WIDGET_SELECTED;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		w->status &= 0xFF ^ WIDGET_SELECTED;
		w->status &= 0xFF ^ WIDGET_FOCUSED;
	}
}
void mouseClickOption(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (w->status&WIDGET_SELECTED) {
		if (x >= w->pos.x&&x < w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
			if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				w->size.y = 0;
				putImage(w->pos.x, w->pos.y, w->cover, COPY_PUT);
				free(w->cover->data);
				Widget->active = -1;
				w->status &= 0xFF ^ WIDGET_SELECTED;
			}
		}
		else {
			if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
			w->size.y = 0;
			putImage(w->pos.x, w->pos.y, w->cover, COPY_PUT);
			free(w->cover->data);
			Widget->active = -1;
			w->status &= 0xFF ^ WIDGET_SELECTED;
		}
	}
	else {
		if (x >= w->associate->pos.x&&x < w->associate->pos.x + w->associate->size.x&&
			y >= w->associate->pos.y&&y < w->associate->pos.y + w->associate->size.y) {
			if (status & SG_RIGHT_BUTTON)Widget->active = getIndexByName(w->name);
			if (status == (SG_BUTTON_DOWN | SG_RIGHT_BUTTON))w->status |= WIDGET_PRESSED;
			if (status == (SG_BUTTON_UP | SG_RIGHT_BUTTON)) {
				w->status |= WIDGET_SELECTED;
				w->pos.x = x;
				w->pos.y = y;
				w->size.y = w->hide*SG_LINE_DELTA_DEFAULT;
				w->value = 0;
				getImage(w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y, w->cover);
			}
		}
		else {
			if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
		}
	}
}
void mouseClickDrag(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status &= 0xFF ^ WIDGET_PRESSED;
	if (x >= w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 2 &&
		x < w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 10 &&
		y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON))w->status |= WIDGET_PRESSED;
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON))w->status |= WIDGET_SELECTED;
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		w->status &= 0xFF ^ WIDGET_SELECTED;
	}
}
void keyDefault(widgetObj *w, int key) {
	if (w->associate) {
		if (key == SG_ENTER)
			w->associate->mouseDown(w->associate,
				w->associate->pos.x, w->associate->pos.y,
				(SG_BUTTON_DOWN | SG_LEFT_BUTTON));
		else if (key == (SG_ENTER | 0x8000)) {
			w->associate->mouseClick(w->associate,
				w->associate->pos.x, w->associate->pos.y,
				(SG_BUTTON_UP | SG_LEFT_BUTTON));
			if (w->associate->type == SG_INPUT) {
				w->mouseDown(w,
					w->associate->pos.x, w->associate->pos.y,
					(SG_BUTTON_DOWN | SG_LEFT_BUTTON));
				w->mouseClick(w,
					w->associate->pos.x, w->associate->pos.y,
					(SG_BUTTON_UP | SG_LEFT_BUTTON));
			}
		}
	}
	return;
}
void keyInput(widgetObj *w, int key) {
	int i, len;

	if (key == SG_LEFT) {
		if (w->value) {
			w->value--;
			if (w->content[w->value] >= 0x80)
				w->value--;
		}
	}
	if (key == SG_RIGHT) {
		if (w->value < (int)strlen(w->content)) {
			w->value++;
			if (w->content[w->value] >= 0x80)
				w->value++;
		}
	}

	len = strlen(w->content);

	/*if (key >= 0x100) {
		if (w->value == len) {
			w->content[w->value + 2] = '\0';
		}
		else {
			for (i = len + 1; i > w->value; i--) {
				w->content[i] = w->content[i - 1];
			}
		}
		w->content[w->value++] = key >> 8;
		w->content[w->value++] = key & 0xff;
		return;
	}*/
	if (key >= 0x80)return;

	if (key == SG_BACKS && w->value != 0) {
		if (w->content[w->value - 1] >= 0x80) {
			w->value -= 2;
			for (i = w->value; i < len; i++) {
				w->content[i] = w->content[i + 2];
			}
		}
		else {
			w->value--;
			for (i = w->value; i < len; i++) {
				w->content[i] = w->content[i + 1];
			}
		}
	}
	if (key >= 0x20) {
		if (w->value == len) {
			w->content[w->value + 1] = '\0';
		}
		else {
			for (i = len + 1; i > w->value; i--) {
				w->content[i] = w->content[i - 1];
			}
		}
		w->content[w->value++] = key;
	}
	return;
}
void keyList(widgetObj *w, int key) {
	if (key == SG_UP) {
		if (w->value)w->value--;
	}
	if (key == SG_DOWN) {
		if (w->value < w->hide - 1)w->value++;
	}
	if (key == (SG_ENTER | 0x80)) {
		mouseClickList(w, w->pos.x, w->pos.y, SG_BUTTON_DOWN);
	}
	if (key == SG_ENTER) {
		mouseClickList(w, w->pos.x, w->pos.y, SG_BUTTON_UP);
	}
}

//Main drawing.
void _drawWidget(int fb) {
	int i;
	widgetObj *current;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			current = Widget->obj[i];
			if (!current||!current->visible||current->priority!=fb)continue;
			switch (current->type) {
			case SG_BUTTON:
				_drawButton(current);
				break;
			case SG_INPUT:
				_drawInput(current);
				break;
			case SG_DIALOG:
				_drawDialog(current);
				break;
			case SG_OUTPUT:
				_drawOutput(current);
				break;
			case SG_LIST:
				_drawList(current);
				break;
			case SG_LABEL:
				_drawLable(current);
				break;
			case SG_CHECK:
				_drawCheck(current);
				break;
			case SG_PROCESS:
				_drawProcess(current);
				break;
			case SG_OPTION:
				_drawOption(current);
				break;
			case SG_DRAG:
				_drawDrag(current);
				break;
			}
		}
	}
}
void _drawButton(widgetObj *w) {
	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_PRESSED)setColor(127, 127, 127);
		else if (w->status&WIDGET_PASS)setColor(223, 223, 223);
		else setColor(191, 191, 191);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(127, 127, 255);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
		if (w->status&WIDGET_SELECTED) {
			setColor(63, 63, 63);
			putQuad(w->pos.x + 2, w->pos.y + 2,
				w->pos.x + w->size.x - 3, w->pos.y + w->size.y - 3, EMPTY_FILL);
		}
		setColor(0, 0, 0);
		putString(w->content,
			w->pos.x + w->size.x / 2 - SG_CHAR_WIDTH*strlen(w->content) / 2,
			w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2 - 2);
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawInput(widgetObj *w) {
	int i;

	switch (w->style) {
	case SG_DESIGN:
		if (w->status&(WIDGET_PRESSED | WIDGET_PASS | WIDGET_SELECTED))
			setColor(255, 255, 255);
		else setColor(239, 239, 239);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(0, 0, 0);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
		if (w->status&WIDGET_FOCUSED) {
			setColor(127, 127, 127);
			putQuad(w->pos.x + 1, w->pos.y + 1,
				w->pos.x + w->size.x - 2, w->pos.y + w->size.y - 2, EMPTY_FILL);
		}

		if (w->hide >= (int)strlen(w->content)) {
			w->value = strlen(w->content);
			w->hide = w->value - 1;
			if (w->hide < 0)w->hide = 0;
		}
		if (w->value <= w->hide && w->value) {
			w->hide = w->value - 1;
			if (w->content[w->hide] >= 0x80 && w->hide >= 1) w->hide--;
		}
		if (w->content[w->value]) {
			if (w->value > w->hide + w->size.x / SG_CHAR_WIDTH - 4) {
				w->hide = w->value - w->size.x / SG_CHAR_WIDTH + 4;
				if (w->content[w->value] < 0x80)w->hide--;
				if (w->content[w->hide] >= 0x80) {
					i = 0;
					do {
						i++;
					} while (w->hide - i >= 0 && w->content[w->hide - i] >= 0x80);
					if ((i & 1) == 0)w->hide++;
				}
			}
		}
		else{
			if (w->value > w->hide + w->size.x / SG_CHAR_WIDTH - 2) {
				w->hide = w->value - w->size.x / SG_CHAR_WIDTH + 2;
				if (w->content[w->hide] >= 0x80) {
					i = 0;
					do {
						i++;
					} while (w->hide - i >= 0 && w->content[w->hide - i] >= 0x80);
					if ((i & 1) == 0)w->hide++;
				}
			}
		}
		setColor(0, 0, 0);
		putStringConstraint(w->content + w->hide,
			w->pos.x + SG_CHAR_WIDTH,
			w->size.y > 2 * SG_CHAR_HEIGHT ?
			w->pos.y + SG_CHAR_HEIGHT / 2 - 2:
			w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2 - 2,
			w->size.x / SG_CHAR_WIDTH - 2);
		if ((w->status&WIDGET_FOCUSED)&&(clock() % 1000 > 500)) {
			setColor(0, 0, 0);
			putLine(w->pos.x + SG_CHAR_WIDTH*(1 + w->value - w->hide) - 1,
				w->size.y > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 - 1 :
				w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2 - 1,
				w->pos.x + SG_CHAR_WIDTH*(1 + w->value - w->hide) - 1,
				w->size.y > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 + SG_CHAR_HEIGHT :
				w->pos.y + w->size.y / 2 + SG_CHAR_HEIGHT / 2,
				SOLID_LINE);
		}
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawDialog(widgetObj *w) {
	int row, total, tmp;

	switch (w->style) {
	case SG_DESIGN:
		setColor(191, 191, 191);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(127, 223, 223);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + SG_CHAR_HEIGHT + 3, SOLID_FILL);
		if(w->status&WIDGET_PRESSED)setColor(191, 63, 63);
		else setColor(255, 63, 63);
		putQuad(w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1), w->pos.y + 2,
			w->pos.x + w->size.x - 3, w->pos.y + SG_CHAR_HEIGHT + 1, SOLID_FILL);
		setColor(255, 255, 255);
		putLine(w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1), w->pos.y + 2,
			w->pos.x + w->size.x - 2, w->pos.y + SG_CHAR_HEIGHT + 1, SOLID_LINE);
		putLine(w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1), w->pos.y + SG_CHAR_HEIGHT + 2,
			w->pos.x + w->size.x - 2, w->pos.y + 1, SOLID_LINE);
		setColor(127, 127, 255);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);

		if (w->status&WIDGET_FOCUSED) {
			setColor(127, 127, 127);
			putQuad(w->pos.x + 1, w->pos.y + 1,
				w->pos.x + w->size.x - 2, w->pos.y + w->size.y - 2, EMPTY_FILL);
		}

		row = 0;
		total = 0;
		tmp = 0;
		setColor(0, 0, 0);
		while (tmp = putStringConstraint(w->content + total,
			w->pos.x + SG_CHAR_WIDTH,
			w->pos.y + (row + 1) * SG_LINE_DELTA_DEFAULT + 8,
			w->size.x / SG_CHAR_WIDTH - 2)) {
			total += tmp;
			row++;
			if ((row + 2) * SG_LINE_DELTA_DEFAULT > w->size.y)break;
		}
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawOutput(widgetObj *w) {
	int row, total, tmp;

	switch (w->style) {
	case SG_DESIGN:
		setColor(255, 255, 255);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(191, 191, 191);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);

		row = 0;
		total = 0;
		tmp = 0;
		setColor(0, 0, 0);
		while (tmp = putStringConstraint(w->content + total,
			w->pos.x + SG_CHAR_WIDTH,
			row * SG_LINE_DELTA_DEFAULT + (w->size.y > 2 * SG_CHAR_HEIGHT ?
			w->pos.y + SG_CHAR_HEIGHT / 2 :
			w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2),
			w->size.x / SG_CHAR_WIDTH - 2)) {
			total += tmp;
			row++;
		}
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawList(widgetObj *w) {
	int i;
	SGstring tmp;

	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_SELECTED) {
			if (w->status&WIDGET_PASS)setColor(255, 255, 255);
			else setColor(239, 239, 239);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - w->hide*SG_LINE_DELTA_DEFAULT - 1, SOLID_FILL);
			setColor(0, 0, 0);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - w->hide*SG_LINE_DELTA_DEFAULT - 1, EMPTY_FILL);
			tmp = w->content;
			for (i = 0; i < w->value; i++) {
				tmp += strlen(tmp) + 1;
			}

			i = w->size.y - w->hide*SG_LINE_DELTA_DEFAULT > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 :
				w->pos.y + (w->size.y - w->hide*SG_LINE_DELTA_DEFAULT) / 2 - SG_CHAR_HEIGHT / 2;
			setColor(0, 0, 0);
			putStringConstraint(tmp,
				w->pos.x + SG_CHAR_WIDTH, i - 2,
				w->size.x / SG_CHAR_WIDTH - 2);
			setColor(0, 0, 0);
			putTriangle(w->pos.x + w->size.x - 12, i - 2 + SG_CHAR_HEIGHT / 2,
				w->pos.x + w->size.x - 6, i - 2 + SG_CHAR_HEIGHT / 2,
				w->pos.x + w->size.x - 9, i + 2 + SG_CHAR_HEIGHT / 2,
				SOLID_FILL);

			setColor(255, 255, 255);
			putQuad(w->pos.x, w->pos.y + w->size.y - w->hide * SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
			setColor(127, 127, 255);
			putQuad(w->pos.x, w->pos.y + w->size.y - (w->hide - w->value)*SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - (w->hide - w->value - 1)*SG_LINE_DELTA_DEFAULT - 1,
				SOLID_FILL);
			setColor(127, 127, 127);
			putQuad(w->pos.x, w->pos.y + w->size.y - w->hide*SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
			tmp = w->content;
			setColor(31, 31, 31);
			for (i = 0; i < w->hide; i++) {
				putStringConstraint(tmp, w->pos.x + SG_CHAR_WIDTH,
					w->pos.y + w->size.y - (w->hide - i)*SG_LINE_DELTA_DEFAULT,
					w->size.x / SG_CHAR_WIDTH - 2);
				tmp += strlen(tmp) + 1;
			}
		}
		else {
			if (w->status&WIDGET_PASS)setColor(255, 255, 255);
			else setColor(239, 239, 239);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
			setColor(127, 127, 127);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
			tmp = w->content;
			for (i = 0; i < w->value; i++) {
				tmp += strlen(tmp) + 1;
			}
			i = w->size.y > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 :
				w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2,
			setColor(0, 0, 0);
			putStringConstraint(tmp,
				w->pos.x + SG_CHAR_WIDTH, i - 2,
				w->size.x / SG_CHAR_WIDTH - 2);
			setColor(0, 0, 0);
			putTriangle(w->pos.x + w->size.x - 12, i - 2 + SG_CHAR_HEIGHT / 2,
				w->pos.x + w->size.x - 6, i - 2 + SG_CHAR_HEIGHT / 2,
				w->pos.x + w->size.x - 9, i + 2 + SG_CHAR_HEIGHT / 2,
				SOLID_FILL);
		}
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawLable(widgetObj *w) {
	switch (w->style) {
	case SG_DESIGN:
		setColor(w->value >> 16 & 0xff, w->value >> 8 & 0xff, w->value & 0xff);
		putString(w->content, w->pos.x, w->pos.y - 2);
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawCheck(widgetObj *w) {
	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_PRESSED)setColor(255, 255, 255);
		else if (w->status&WIDGET_PASS)setColor(255, 255, 255);
		else setColor(239, 239, 239);
		putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 6, SOLID_FILL);
		setColor(63, 63, 63);
		putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 6, EMPTY_FILL);
		if (w->status&WIDGET_SELECTED) {
			setColor(127, 127, 127);
			putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 5, EMPTY_FILL);
		}
		if (w->value) {
			setColor(63, 223, 127);
			putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 2, SOLID_FILL);
		}
		setColor(0, 0, 0);
		putString(w->content, w->pos.x + (int)(2.5f * SG_CHAR_WIDTH), w->pos.y - 2);
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawProcess(widgetObj *w) {
	int row, total, tmp;

	switch (w->style) {
	case SG_DESIGN:
		setColor(191, 191, 191);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(127, 223, 223);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + SG_CHAR_HEIGHT + 3, SOLID_FILL);
		if (w->status&WIDGET_PRESSED)setColor(191, 63, 63);
		else setColor(255, 63, 63);
		putQuad(w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1), w->pos.y + 2,
			w->pos.x + w->size.x - 3, w->pos.y + SG_CHAR_HEIGHT + 1, SOLID_FILL);
		setColor(255, 255, 255);
		putLine(w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1), w->pos.y + 2,
			w->pos.x + w->size.x - 2, w->pos.y + SG_CHAR_HEIGHT + 1, SOLID_LINE);
		putLine(w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1), w->pos.y + SG_CHAR_HEIGHT + 2,
			w->pos.x + w->size.x - 2, w->pos.y + 1, SOLID_LINE);
		setColor(127, 127, 255);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);

		if (w->status&WIDGET_FOCUSED) {
			setColor(127, 127, 127);
			putQuad(w->pos.x + 1, w->pos.y + 1,
				w->pos.x + w->size.x - 2, w->pos.y + w->size.y - 2, EMPTY_FILL);
		}

		row = 0;
		total = 0;
		tmp = 0;
		setColor(0, 0, 0);
		while (tmp = putStringConstraint(w->content + total,
			w->pos.x + SG_CHAR_WIDTH,
			w->pos.y + (row + 1) * SG_LINE_DELTA_DEFAULT + 8,
			w->size.x / SG_CHAR_WIDTH - 2)) {
			total += tmp;
			row++;
			if ((row + 4) * SG_LINE_DELTA_DEFAULT > w->size.y)break;
		}

		setColor(223, 223, 223);
		putQuad(w->pos.x + 2 * SG_CHAR_WIDTH,
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 8,
			w->pos.x + w->size.x - 2 * SG_CHAR_WIDTH,
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 1,
			SOLID_FILL);
		setColor(63, 223, 127);
		putQuad(w->pos.x + 2 * SG_CHAR_WIDTH,
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 8,
			w->pos.x+2*SG_CHAR_WIDTH + (int)(w->value*1.f/100*(w->size.x-4*SG_CHAR_WIDTH)),
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 1,
			SOLID_FILL);
		setColor(63, 63, 63);
		putQuad(w->pos.x + 2 * SG_CHAR_WIDTH,
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 8,
			w->pos.x + w->size.x - 2 * SG_CHAR_WIDTH,
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 1,
			EMPTY_FILL);
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawOption(widgetObj *w) {
	int i;
	SGstring tmp;

	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_SELECTED) {
			setColor(239, 239, 239);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
			
			setColor(127, 192, 255);
			putQuad(w->pos.x, w->pos.y + w->value * SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + (w->value + 1)*SG_LINE_DELTA_DEFAULT - 1,
				SOLID_FILL);
			tmp = w->content;
			setColor(31, 31, 31);
			for (i = 0; i < w->hide; i++) {
				putStringConstraint(tmp, w->pos.x + SG_CHAR_WIDTH,
					w->pos.y + w->size.y - (w->hide - i)*SG_LINE_DELTA_DEFAULT + 2,
					w->size.x / SG_CHAR_WIDTH - 2);
				tmp += strlen(tmp) + 1;
			}

			setColor(0, 0, 0);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
		}
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}
void _drawDrag(widgetObj *w) {
	RGB inverse = getPixel(w->pos.x + 1, w->pos.y + 1);
	inverse.r = 255 - inverse.r;
	inverse.g = 255 - inverse.g;
	inverse.b = 255 - inverse.b;
	putImage(w->pos.x, w->pos.y, w->cover, COPY_PUT);
	switch (w->style) {
	case SG_DESIGN:
		setColor(inverse.r, inverse.g, inverse.b);
		putQuad(w->pos.x, w->pos.y + w->size.y / 2 - 2,
			w->pos.x + w->size.x, w->pos.y + w->size.y / 2 + 2, EMPTY_FILL);
		if (w->status&WIDGET_PRESSED)setColor(127, 127, 127);
		else if (w->status&WIDGET_PASS)setColor(223, 223, 223);
		else setColor(191, 191, 191);
		putQuad(w->pos.x + w->value* (w->size.x - 12) / 100  + 2, w->pos.y,
			w->pos.x + w->value * (w->size.x - 12) / 100 + 10, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(0, 0, 0);
		putQuad(w->pos.x + w->value * (w->size.x - 12) / 100 + 2, w->pos.y,
			w->pos.x + w->value * (w->size.x - 12) / 100 + 10, w->pos.y + w->size.y - 1, EMPTY_FILL);
		break;
	case WIN_XP:
		break;
	case WIN_10:
		break;
	case LINUX:
		break;
	case WEBSITE:
		break;
	case ANDROID:
		break;
	}
}

//User interfaces.
widgetObj *newWidget(int type, SGstring name) {
	widgetObj *ret = (widgetObj *)malloc(sizeof(widgetObj));
	ret->type = type;
	ret->pos.x = ret->pos.y = 0;
	ret->size.x = ret->size.y = 0;
	ret->visible = TRUE;
	ret->priority = WIDGET_FRONT;
	ret->status = FALSE;
	ret->style = SG_DESIGN;
	ret->hide = 0;
	ret->value = 0;
	ret->name = (unsigned char *)malloc(strlen(name)+1);
	strcpy(ret->name, name);
	ret->content = (unsigned char *)malloc(1024 * sizeof(unsigned char *));
	memset(ret->content, 0, 1024 * sizeof(unsigned char *));
	ret->helpMessage = (unsigned char *)malloc(1024 * sizeof(unsigned char *));
	memset(ret->helpMessage, 0, 1024 * sizeof(unsigned char *));
	ret->cover = (bitMap *)malloc(sizeof(bitMap));
	ret->associate = NULL;
	ret->mouseIn = ret->mouseOut = mouseMoveDefault;
	ret->mouseClick = ret->mouseDown = ret->mouseUp = mouseClickDefault;
	ret->keyDown = ret->keyPress = ret->keyUp = keyDefault;
	switch (type) {
	case SG_BUTTON:
		break;
	case SG_INPUT:
		ret->mouseClick = ret->mouseDown = ret->mouseUp = mouseClickInput;
		ret->keyPress = keyInput;
		break;
	case SG_DIALOG:
		ret->visible = FALSE;
		ret->mouseClick = ret->mouseDown = ret->mouseUp = mouseClickDialog;
		break;
	case SG_OUTPUT:
		break;
	case SG_LIST:
		ret->keyUp = ret->keyDown = ret->keyPress = keyList;
		ret->mouseIn = ret->mouseOut = mouseMoveList;
		ret->mouseDown = ret->mouseUp = ret->mouseClick = mouseClickList;
		break;
	case SG_LABEL:
		break;
	case SG_CHECK:
		ret->mouseDown = ret->mouseUp = ret->mouseClick = mouseClickCheck;
		break;
	case SG_PROCESS:
		ret->visible = FALSE;
		ret->mouseDown = ret->mouseUp = ret->mouseClick = mouseClickProcess;
		break;
	case SG_OPTION:
		ret->mouseIn = ret->mouseOut = mouseMoveOption;
		ret->mouseDown = ret->mouseUp = ret->mouseClick = mouseClickOption;
		break;
	case SG_DRAG:
		ret->mouseIn = ret->mouseOut = mouseMoveDrag;
		ret->mouseDown = ret->mouseUp = ret->mouseClick = mouseClickDrag;
		break;
	}
	return ret;
}
int registerWidget(widgetObj *obj) {
	widgetObj *tmp;
	struct _hash *end;
	int i, len, sum = 0;

	if (obj == NULL)return SG_NULL_POINTER;
	if (obj->name == NULL)return SG_INCOMPLETE_STRUCT;
	if (Widget->count >= SG_QSIZE)return SG_OUT_OF_RANGE;

	tmp = (widgetObj *)malloc(sizeof(widgetObj));
	memcpy(tmp, obj, sizeof(widgetObj));
	if (tmp->visible&&_inLoop)
		getImage(tmp->pos.x, tmp->pos.y, tmp->pos.x + tmp->size.x, tmp->pos.y + tmp->size.y, tmp->cover);
	Widget->obj[Widget->count] = tmp;

	len = strlen(tmp->name);
	for (i = 0; i < len; i++)sum += tmp->name[i];
	sum %= 256;
	end = Widget->hash[sum];
	if (end == NULL) {
		Widget->hash[sum] = (struct _hash *)malloc(sizeof(struct _hash));
		Widget->hash[sum]->content = (char *)malloc(strlen(tmp->name) + 1);
		strcpy(Widget->hash[sum]->content, tmp->name);
		Widget->hash[sum]->value = Widget->count;
		Widget->hash[sum]->next = NULL;
	}
	else {
		while (end->next != NULL) {
			if (strcmp(end->content, obj->name) == 0)return SG_MULTY_VALUE;
			end = end->next;
		}
		end->next = (struct _hash *)malloc(sizeof(struct _hash));
		end->next->content = (char *)malloc(strlen(tmp->name) + 1);
		strcpy(end->next->content, tmp->name);
		end->next->value = Widget->count;
		end->next->next = NULL;
	}
	return Widget->count++;
}
int inWidget(widgetObj *obj, int x, int y) {
	if (obj == NULL)return 0;
	return (x >= obj->pos.x&&x < obj->pos.x + obj->size.x) &&
		(y >= obj->pos.y&&y < obj->pos.y + obj->size.y);
}
widgetObj *getWidgetByIndex(int index) {
	if (index < 0 || index >= Widget->count)return NULL;
	return Widget->obj[index];
}
widgetObj *getWidgetByName(char *name) {
	int i, len, sum = 0;
	struct _hash *end;
	len = strlen(name);
	for (i = 0; i < len; i++)sum += name[i];
	sum %= 256;
	end = Widget->hash[sum];
	while (end != NULL) {
		if (strcmp(end->content, name) == 0)return Widget->obj[end->value];
		end = end->next;
	}
	return NULL;
}
int getIndexByName(char *name) {
	int i, len, sum = 0;
	struct _hash *end;
	len = strlen(name);
	for (i = 0; i < len; i++)sum += name[i];
	sum %= 256;
	end = Widget->hash[sum];
	while (end != NULL) {
		if (strcmp(end->content, name) == 0)return end->value;
		end = end->next;
	}
	return SG_OBJECT_NOT_FOUND;
}
void showWidget(char *name) {
	widgetObj *obj = getWidgetByName(name);
	if (obj == NULL)return;
	if (obj->visible == TRUE) return;
	obj->visible = TRUE;
	getImage(obj->pos.x, obj->pos.y, obj->pos.x + obj->size.x, obj->pos.y + obj->size.y, obj->cover);
}
void ceaseWidget(char *name) {
	widgetObj *obj = getWidgetByName(name);
	if (obj == NULL)return;
	if (obj->visible == FALSE)return;
	obj->status = 0;
	obj->visible = FALSE;
	putImage(obj->pos.x, obj->pos.y, obj->cover, COPY_PUT);
	free(obj->cover->data);
}
int deleteWidgetByIndex(int index) {
	if (index >= Widget->count || Widget->obj[index] == NULL)return SG_OUT_OF_RANGE;
	if (index == --Widget->count) {
		free(Widget->obj[index]);
		Widget->obj[index] = NULL;
	}
	else {
		free(Widget->obj[index]);
		Widget->obj[index] = Widget->obj[Widget->count];
		Widget->obj[Widget->count] = NULL;
	}
	return SG_NO_ERORR;
}
int deleteWidgetByName(char *name) {
	int i, len, sum = 0;
	struct _hash *end, *tmp;
	len = strlen(name);
	for (i = 0; i < len; i++)sum += name[i];
	sum %= 256;
	end = Widget->hash[sum];
	if (end == NULL) {
		return SG_OBJECT_NOT_FOUND;
	}
	else {
		while (end->next != NULL) {
			if (strcmp(end->next->content, name) == 0)break;
			end = end->next;
		}
		if (end->next) {
			tmp = end->next->next;
			if (end->next->value == --Widget->count) {
				free(Widget->obj[end->next->value]);
				Widget->obj[end->next->value] = NULL;
			}
			else {
				free(Widget->obj[end->next->value]);
				Widget->obj[end->next->value] = Widget->obj[Widget->count];
				Widget->obj[Widget->count] = NULL;
			}
			free(end->next);
			end->next = tmp;
		}
		else return SG_OBJECT_NOT_FOUND;
	}
	return SG_NO_ERORR;
}
