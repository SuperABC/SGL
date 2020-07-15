/*
* Copyright (c) 2016-2020, Super GP Individual.
* All Rights Reserved.
*
* Permission to use, copy, modify, and distribute this library for any
* purpose and without fee is hereby granted, provided that the above
* copyright notice appear in all copies and that both the copyright
* notice and this permission notice appear in supporting documentation,
* and that the name of SGL not be used in advertising or publicity
* pertaining to distribution of the software without specific,
* written prior permission.
*/



#ifndef SGL_WIDGET_H
#define SGL_WIDGET_H


#define SCROLL_WIDTH 20
#define MINIMAL_BAR 24


#include "winsgl.h"
#include "inner.h"
#include "util.h"
#include <windows.h>

#include <string>

using std::string;

int _wPos2sPos(const char *src, int pos) {
	int ret = 0;
	while (pos-- > 0) {
		if (src[ret] == 0)return ret;
		if (src[ret] < 0)ret++;
		ret++;
	}
	return ret;
}
int _sPos2wPos(const char *src, int pos) {
	int ret = 0, i = 0;
	if (pos >= 0) {
		while (i < pos) {
			if (src[i] == 0)return ret;
			if (src[i] < 0)i++;
			ret++;
			i++;
		}
	}
	else {
		while (src[i]) {
			if (src[i] < 0)i++;
			ret++;
			i++;
		}
	}
	return ret;
}
char *_stringConvert(const char *str) {
	int i, j, len = 0;
	char *ret;

	for (i = 0; i < (int)strlen(str); i++) {
		if (str[i] == '\t')len += 3;
		if (str[i] == '\b')len -= 2;
		len++;
	}

	ret = (char *)malloc(len + 1);

	for (i = 0, j = 0; i < len; ) {
		if (str[j] == '\t') {
			j++;
			ret[i++] = ' ';
			ret[i++] = ' ';
			ret[i++] = ' ';
			ret[i++] = ' ';
			continue;
		}
		if (str[j] == '\b') {
			j++;
			i--;
			continue;
		}
		ret[i++] = str[j++];
	}
	ret[len] = 0;
	return ret;
}

void mouseUserDefault() {

}
void keyUserDefault(int key) {

}

class Widget {
public:
	int window;

	enum _control type;

	vec2i pos, size;
	int style, status, visible, priority, valid;
	
	string name;
	string path;
	void *content;
	int value, extra;

	font tf;
	Widget *child, *next;

	rgb bgColor, passColor, pressColor, fgColor;
	bitMap bgImg;

	void(*click)(widget *obj);
	void(*move)(widget *obj, int x, int y);
	void(*drag)(widget *obj, int x, int y);
	void(*press)(widget *obj, int key);

	widget *obj;

	virtual void draw(int id) {

	}
	virtual void mouseMove(int x, int y) {
		if (inWidget(obj, x, y)) {
			status |= WIDGET_PASS;
			valid = 0;
			move(obj, x, y);
		}
		else {
			if (status & WIDGET_PASS) {
				status &= 0xFF ^ WIDGET_PASS;
				valid = 0;
			}
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (inWidget(obj, x, y)) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				status |= WIDGET_PRESSED;
				valid = 0;
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status |= WIDGET_SELECTED;
				status &= 0xFF ^ WIDGET_PRESSED;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				status &= 0xFF ^ WIDGET_PRESSED;
				valid = 0;
			}
		}

		if (this->status & WIDGET_PASS && this->status & WIDGET_SELECTED &&
			button == (SG_BUTTON_UP | SG_LEFT_BUTTON))
			click(obj);

	}
	virtual void keyPress(int key, bool utf16 = false) {
		if (status != WIDGET_SELECTED)return;
	}

	Widget(widget w, int window, string p = "");
	virtual ~Widget() {
		free(content);
		free(obj);
		free((void *)tf.name);
		free(bgImg.data);
	}
	widget *getObj() { return obj; }
	void show() {
		if (visible == TRUE) return;
		visible = TRUE;
		valid = 0;
	}
	void cease() {
		if (visible == FALSE)return;
		status = 0;
		visible = FALSE;
	}

	virtual void update() {}
};

class Button : public Widget {
public:
	Button(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		switch (style) {
		case SG_DESIGN: {
			if (bgImg.data == NULL) {
				if (status&WIDGET_PRESSED)
					setColor(pressColor.r, pressColor.g, pressColor.b);
				else if (status&WIDGET_PASS)
					setColor(passColor.r, passColor.g, passColor.b);
				else setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y,
					pos.x + size.x, pos.y + size.y, SOLID_FILL);
				setColor(127, 127, 127);
				putQuad(pos.x, pos.y,
					pos.x + size.x, pos.y + size.y, EMPTY_FILL);

				if (status&WIDGET_SELECTED) {
					setColor(127, 127, 127);
					putQuad(pos.x + 2, pos.y + 2,
						pos.x + size.x - 2, pos.y + size.y - 2, EMPTY_FILL);
				}
			}
			else {
				putBitmap(pos.x, pos.y, bgImg);
			}

			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			int line = 1;
			char *linestr = (char *)content;
			int total = strlen((char *)content);
			for (int i = 0; i < total; i++) {
				if (((char *)content)[i] == '\n') {
					((char *)content)[i] = '\0';
					line++;
				}
			}
			for (int i = 0; i < line; i++) {
				int strWidth = stringWidth(linestr, _sPos2wPos(linestr, -1));
				putString(linestr,
					pos.x + size.x / 2 - (strWidth >= size.x ? size.x : strWidth) / 2,
					pos.y + size.y / 2 - (tf.size * line) / 2 + tf.size * i);
				linestr += strlen(linestr) + 1;
				linestr[-1] = '\n';
			}
			linestr[-1] = '\0';
			break;
		}
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
};
class Input : public Widget {
private:
	int start = -1;
	int end = -1;

	bool drag = false;

	void constraint() {
		if (value < 0)value=0;
		if (value > _strlen(_widen((char *)content)))value = _strlen(_widen((char *)content));
		int width1 = stringWidth((char *)content, value);
		int width2 = stringWidth((char *)content, extra);
		while (width1 > width2 + size.x - 2 * SG_CHAR_WIDTH) {
			extra++;
			width2 = stringWidth((char *)content, extra);
		}
		if (value <= extra && value)extra = value - 1;
	}
public:
	Input(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		valid = 0;
		switch (style) {
		case SG_DESIGN:
			if (bgImg.data == NULL) {
				if (status&(WIDGET_PRESSED | WIDGET_SELECTED))
					setColor(pressColor.r, pressColor.g, pressColor.b);
				else if (status&WIDGET_PASS)
					setColor(passColor.r, passColor.g, passColor.b);
				else setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y,
					pos.x + size.x, pos.y + size.y, SOLID_FILL);
			}
			else {
				putBitmap(pos.x, pos.y, bgImg);
			}
			setColor(0, 0, 0);
			putQuad(pos.x, pos.y,
				pos.x + size.x, pos.y + size.y, EMPTY_FILL);
			if (status&WIDGET_SELECTED) {
				setColor(127, 127, 127);
				putQuad(pos.x + 1, pos.y + 1,
					pos.x + size.x - 1, pos.y + size.y - 1, EMPTY_FILL);
			}

			int width1, width2;
			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			width1 = stringWidth((char *)content, value);
			width2 = stringWidth((char *)content, extra);
			putStringConstraint((char *)content,
				pos.x + SG_CHAR_WIDTH,
				size.y > 2 * SG_CHAR_HEIGHT ?
				pos.y + SG_CHAR_HEIGHT / 2 - 2 :
				pos.y + size.y / 2 - SG_CHAR_HEIGHT / 2 - 2,
				extra, size.x - 2 * SG_CHAR_WIDTH);
			if ((status&WIDGET_SELECTED) && (clock() % 1000 > 400)) {
				setColor(fgColor.r, fgColor.g, fgColor.b);
				putLine(pos.x + SG_CHAR_WIDTH + width1 - width2,
					size.y > 2 * SG_CHAR_HEIGHT ?
					pos.y + SG_CHAR_HEIGHT / 2 - 1 :
					pos.y + size.y / 2 - SG_CHAR_HEIGHT / 2 - 1,
					pos.x + SG_CHAR_WIDTH + width1 - width2,
					size.y > 2 * SG_CHAR_HEIGHT ?
					pos.y + SG_CHAR_HEIGHT / 2 + SG_CHAR_HEIGHT :
					pos.y + size.y / 2 + SG_CHAR_HEIGHT / 2,
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseMove(int x, int y) {
		Widget::mouseMove(x, y);
		if (inWidget(obj, x, y)) {
			setMouseIcon(IDC_IBEAM);

			if (drag) {
				int i = 0, len, tmp;
				len = _strlen(_widen((char *)content));
				valid = 0;
				if (((char *)content)[0] == '\0')value = 0;
				else if (x >= pos.x + 2 && x < pos.x + size.x - 2) {
					tmp = stringWidth((char *)content, extra);
					value = extra;
					x += tmp - pos.x;
					do {
						value++;
						tmp = stringWidth((char *)content, value);
					} while (tmp < x - 4 && value <= len);
					value--;
				}
			}
		}
		else {
			if (drag) {
				if (x >= pos.x + size.x) {
					value++;
					constraint();
				}
				if (x < pos.x) {
					value--;
					constraint();
				}
			}
			setMouseIcon(IDC_ARROW);
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		int i = 0, len, tmp;
		len = _strlen(_widen((char *)content));

		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
			drag = false;
		}
		if (inWidget(obj, x, y)) {
			setMouseIcon(IDC_IBEAM);
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				status |= WIDGET_PRESSED;
				status |= WIDGET_SELECTED;
				drag = true;
				mouseMove(x, y);
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status |= WIDGET_SELECTED;
				valid = 0;
				if (((char *)content)[0] == '\0')value = 0;
				else if (x >= pos.x + 2 && x < pos.x + size.x - 2) {
					tmp = stringWidth((char *)content, extra);
					value = extra;
					x += tmp - pos.x;
					do {
						value++;
						tmp = stringWidth((char *)content, value);
					} while (tmp < x - 4 && value <= len);
					value--;
				}
			}
		}
		else {
			setMouseIcon(IDC_ARROW);
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
	}
	virtual void keyPress(int key, bool utf16 = false) {
		if (!(status & WIDGET_SELECTED))return;

		if (utf16) {
			int len = _strlen(_widen((char *)content));
			int lenW = _wPos2sPos((char *)content, len), i;
			if (value == len) {
				((char *)content)[_wPos2sPos((char *)content, value) + 2] = '\0';
			}
			else {
				for (i = lenW + 2; i > _wPos2sPos((char *)content, value); i--) {
					((char *)content)[i] = ((char *)content)[i - 2];
				}
			}
			((char *)content)[_wPos2sPos((char *)content, value)] = key / 0x100;
			((char *)content)[_wPos2sPos((char *)content, value) + 1] = key % 0x100;
			value++;
		}
		else {
			if (key == SG_LEFT) {
				value--;
				constraint();
			}
			if (key == SG_RIGHT) {
				value++;
				constraint();
			}

			if (key >= 0x80)return;

			int len = _strlen(_widen((char *)content));
			int lenW = _wPos2sPos((char *)content, len), i;
			if (key == SG_BACKS && value != 0) {
				value--;
				if (((unsigned char *)content)[i = _wPos2sPos((char *)content, value)] >= 0x80) {
					for (; i < lenW; i++) {
						((char *)content)[i] = ((char *)content)[i + 2];
					}
				}
				else {
					for (; i < lenW; i++) {
						((char *)content)[i] = ((char *)content)[i + 1];
					}
				}
			}
			if (key >= 0x20) {
				if (value == len) {
					((char *)content)[_wPos2sPos((char *)content, value) + 1] = '\0';
				}
				else {
					for (i = lenW + 1; i > _wPos2sPos((char *)content, value); i--) {
						((char *)content)[i] = ((char *)content)[i - 1];
					}
				}
				((char *)content)[_wPos2sPos((char *)content, value++)] = key;
			}
			constraint();
		}
	}
};
class Dialog : public Widget {
public:
	Dialog(widget w, int window, string p = "") :Widget(w, window, p) {
		visible = false;
	}

	virtual void draw(int id) {
		int row, total, tmp;

		switch (style) {
		case SG_DESIGN: {
			if (bgImg.data == NULL) {
				setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y,
					pos.x + size.x, pos.y + size.y, SOLID_FILL);
				setColor(223, 223, 223);
				putQuad(pos.x, pos.y,
					pos.x + size.x, pos.y + SG_CHAR_HEIGHT + 4, SOLID_FILL);
				setColor(127, 127, 127);
				putQuad(pos.x, pos.y,
					pos.x + size.x, pos.y + size.y, EMPTY_FILL);
			}
			else {
				putBitmap(pos.x, pos.y, bgImg);
			}
			if (status&WIDGET_PRESSED)setColor(191, 63, 63);
			else setColor(255, 63, 63);
			putQuad(pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1), pos.y + 2,
				pos.x + size.x - 2, pos.y + SG_CHAR_HEIGHT + 2, SOLID_FILL);
			setColor(255, 255, 255);
			putLine(pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1), pos.y + 2,
				pos.x + size.x - 2, pos.y + SG_CHAR_HEIGHT + 1, SOLID_LINE);
			putLine(pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1), pos.y + SG_CHAR_HEIGHT + 2,
				pos.x + size.x - 2, pos.y + 1, SOLID_LINE);

			if (status&WIDGET_SELECTED) {
				setColor(127, 127, 127);
				putQuad(pos.x + 1, pos.y + 1,
					pos.x + size.x - 1, pos.y + size.y - 1, EMPTY_FILL);
			}

			row = 0;
			total = 0;
			tmp = 0;
			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			char *conv = _stringConvert((char *)content);
			while (tmp = putStringConstraint(conv + total,
				pos.x + SG_CHAR_WIDTH,
				pos.y + (row + 1) * tf.size + SG_CHAR_HEIGHT / 2, 0,
				size.x - 2 * SG_CHAR_WIDTH)) {
				total += tmp;
				row++;
				if ((row + 2) * tf.size > size.y)break;
			}
			free(conv);
			break;
		}
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (inWidget(obj, x, y)) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				if (x >= pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1) && x < pos.x + size.x - 2 &&
					y >= pos.y + 2 && y < pos.y + SG_CHAR_HEIGHT + 2) {
					status |= WIDGET_PRESSED;
					valid = 0;
				}
				else {
					status |= WIDGET_SELECTED;
					valid = 0;
				}
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				if (x >= pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1) && x < pos.x + size.x - 2 &&
					y >= pos.y + 2 && y < pos.y + SG_CHAR_HEIGHT + 2) {
					ceaseWidget(((path == ""?"" : path  + '/') + name).data());
					return;
				}
				status |= WIDGET_SELECTED;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
	}
};
class Output : public Widget {
public:
	Output(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		int start, row, total, tmp;

		switch (style) {
		case SG_DESIGN: {
			if (bgImg.data == NULL) {
				setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y, pos.x + size.x, pos.y + size.y, SOLID_FILL);
				setColor(191, 191, 191);
				putQuad(pos.x, pos.y, pos.x + size.x, pos.y + size.y, EMPTY_FILL);
			}
			else {
				putBitmap(pos.x, pos.y, bgImg);
			}

			start = 0;
			row = 0;
			total = 0;
			tmp = 0;
			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			char *conv = _stringConvert((char *)content);
			while (tmp = putStringConstraint(conv + total,
				pos.x + SG_CHAR_WIDTH,
				row * tf.size + (size.y > 2 * SG_CHAR_HEIGHT ?
					pos.y + SG_CHAR_HEIGHT / 2 :
					pos.y + size.y / 2 - SG_CHAR_HEIGHT / 2), 0,
				size.x - 2 * SG_CHAR_WIDTH)) {
				total += tmp;
				row++;
				if ((row + 2) * tf.size > size.y)break;
			}
			free(conv);
			break;
		}
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
};
class List : public Widget {
public:
	List(widget w, int window, string p = "") :Widget(w, window, p) {
		int length = strlen((char *)w.content);
		for (int i = 0; i < length; i++) {
			if (((char *)w.content)[i] == '\n')
				((char *)w.content)[i] = '\0';
		}
	}

	virtual void draw(int id) {
		int i;
		SGstring tmp;

		switch (style) {
		case SG_DESIGN:
			if (status&WIDGET_SELECTED) {
				setColor(passColor.r, passColor.g, passColor.b);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + size.y - extra*SG_LINE_DELTA_DEFAULT - 1, SOLID_FILL);
				setColor(0, 0, 0);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + size.y - extra *SG_LINE_DELTA_DEFAULT - 1, EMPTY_FILL);
				tmp = (char *)content;
				for (i = 0; i < value; i++) {
					tmp += strlen(tmp) + 1;
				}

				setColor(tf.color.r, tf.color.g, tf.color.b);
				setFontSize(tf.size);
				setFontName(_shorten(tf.name));
				i = size.y - extra*SG_LINE_DELTA_DEFAULT > 2 * SG_CHAR_HEIGHT ?
					pos.y + SG_CHAR_HEIGHT / 2 :
					pos.y + (size.y - extra*SG_LINE_DELTA_DEFAULT) / 2 - SG_CHAR_HEIGHT / 2;
				putStringConstraint(tmp,
					pos.x + SG_CHAR_WIDTH, i - 2, 0,
					size.x - 2 * SG_CHAR_WIDTH);
				setColor(0, 0, 0);
				putTriangle(pos.x + size.x - 12, i - 2 + SG_CHAR_HEIGHT / 2,
					pos.x + size.x - 6, i - 2 + SG_CHAR_HEIGHT / 2,
					pos.x + size.x - 9, i + 2 + SG_CHAR_HEIGHT / 2,
					SOLID_FILL);

				setColor(passColor.r, passColor.g, passColor.b);
				putQuad(pos.x, pos.y + size.y - extra * SG_LINE_DELTA_DEFAULT,
					pos.x + size.x - 1, pos.y + size.y - 1, SOLID_FILL);
				setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y + size.y - (extra - value)*SG_LINE_DELTA_DEFAULT,
					pos.x + size.x - 1, pos.y + size.y - (extra - value - 1)*SG_LINE_DELTA_DEFAULT - 1,
					SOLID_FILL);
				setColor(127, 127, 127);
				putQuad(pos.x, pos.y + size.y - extra*SG_LINE_DELTA_DEFAULT,
					pos.x + size.x - 1, pos.y + size.y - 1, EMPTY_FILL);
				tmp = (char *)content;
				setColor(tf.color.r, tf.color.g, tf.color.b);
				for (i = 0; i < extra; i++) {
					putStringConstraint(tmp, pos.x + SG_CHAR_WIDTH,
						pos.y + size.y - (extra - i)*SG_LINE_DELTA_DEFAULT, 0,
						size.x - 2 * SG_CHAR_WIDTH);
					tmp += strlen(tmp) + 1;
				}
			}
			else {
				if (status&WIDGET_PASS)
					setColor(passColor.r, passColor.g, passColor.b);
				else setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + size.y - 1, SOLID_FILL);
				setColor(127, 127, 127);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + size.y - 1, EMPTY_FILL);
				tmp = (char *)content;
				for (i = 0; i < value; i++) {
					tmp += strlen(tmp) + 1;
				}

				setColor(tf.color.r, tf.color.g, tf.color.b);
				setFontSize(tf.size);
				setFontName(_shorten(tf.name));
				i = size.y > 2 * SG_CHAR_HEIGHT ?
					pos.y + SG_CHAR_HEIGHT / 2 :
					pos.y + size.y / 2 - SG_CHAR_HEIGHT / 2;
				putStringConstraint(tmp,
					pos.x + SG_CHAR_WIDTH, i - 2, 0,
					size.x - 2 * SG_CHAR_WIDTH);
				setColor(0, 0, 0);
				putTriangle(pos.x + size.x - 12, i - 2 + SG_CHAR_HEIGHT / 2,
					pos.x + size.x - 6, i - 2 + SG_CHAR_HEIGHT / 2,
					pos.x + size.x - 9, i + 2 + SG_CHAR_HEIGHT / 2,
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseMove(int x, int y) {
		if (status&WIDGET_SELECTED) {
			if (status&WIDGET_PRESSED&&
				x >= pos.x&&x < pos.x + size.x&&
				y >= pos.y + size.y - extra * SG_LINE_DELTA_DEFAULT&&y < pos.y + size.y) {
				status |= WIDGET_PRESSED;
				value = (y - pos.y - size.y) / SG_LINE_DELTA_DEFAULT + extra - 1;
				if (value < 0)value = 0;
				valid = 0;
			}
			else {
				if (inWidget(obj, x, y)) {
					status |= WIDGET_PASS;
					value = (y - pos.y - size.y) / SG_LINE_DELTA_DEFAULT + extra - 1;
					if (value < 0)value = 0;
					valid = 0;
				}
				else {
					status &= 0xFF ^ WIDGET_PASS;
					status &= 0xFF ^ WIDGET_PRESSED;
				}
			}
		}
		else {
			if (inWidget(obj, x, y)) {
				if (!(status & WIDGET_PASS)) {
					status |= WIDGET_PASS;
					valid = 0;
				}
			}
			else {
				if (status & WIDGET_PASS) {
					status &= 0xFF ^ WIDGET_PASS;
					status &= 0xFF ^ WIDGET_PRESSED;
					valid = 0;
				}
			}
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (status&WIDGET_SELECTED) {
			if (button == SG_MIDDLE_BUTTON_UP) {
				if (value > 0) {
					value--;
					valid = 0;
				}
			}
			else if (button == SG_MIDDLE_BUTTON_DOWN) {
				if (value < extra - 1) {
					value++;
					valid = 0;
				}
			}
			else if (x >= pos.x&&x < pos.x + size.x&&
				y >= pos.y + size.y - extra * SG_LINE_DELTA_DEFAULT&&y < pos.y + size.y) {
				if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
					status &= 0xFF ^ WIDGET_SELECTED;
					status &= 0xFF ^ WIDGET_PASS;
					valid = 0;

					size.y -= extra *SG_LINE_DELTA_DEFAULT;
					obj->size.y -= extra * SG_LINE_DELTA_DEFAULT;
					useBackgroundRefresh(
						pos.x, pos.y, pos.x + size.x, pos.y + size.y + extra * SG_LINE_DELTA_DEFAULT);
				}
			}
			else if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				status &= 0xFF ^ WIDGET_PASS;
				valid = 0;

				size.y -= extra *SG_LINE_DELTA_DEFAULT;
				obj->size.y -= extra * SG_LINE_DELTA_DEFAULT;
				useBackgroundRefresh(
					pos.x, pos.y, pos.x + size.x, pos.y + size.y + extra * SG_LINE_DELTA_DEFAULT);
			}
		}
		else {
			if (inWidget(obj, x, y)) {
				if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
					status |= WIDGET_PRESSED;
					status |= WIDGET_SELECTED;
					size.y += extra *SG_LINE_DELTA_DEFAULT;
					obj->size.y += extra * SG_LINE_DELTA_DEFAULT;
					valid = 0;
				}
			}
		}
	}
	virtual void keyPress(int key) {
		if ((status & WIDGET_SELECTED) == 0)return;

		if (key == SG_UP) {
			if (value) {
				value--;
				valid = 0;
			}
		}
		if (key == SG_DOWN) {
			if (value < extra - 1) {
				value++;
				valid = 0;
			}
		}
		if (key == SG_ENTER) {
			if (status&WIDGET_SELECTED) {
				status &= 0xFF ^ WIDGET_SELECTED;
				status &= 0xFF ^ WIDGET_PASS;
				valid = 0;

				useBackgroundRefresh(
					pos.x, pos.y, pos.x + size.x, pos.y + size.y);
				size.y -= extra*SG_LINE_DELTA_DEFAULT;
				obj->size.y -= extra * SG_LINE_DELTA_DEFAULT;
			}
		}
	}
};
class Label :public Widget {
public:
	Label(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		switch (style) {
		case SG_DESIGN:
			useBackgroundRefresh(
				pos.x, pos.y, pos.x + size.x - 1, pos.y + size.y - 1);
			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			putString((char *)content, pos.x, pos.y);
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
};
class Pic :public Widget {
public:
	Pic(widget w, int window, string p = "") :Widget(w, window, p) {
		obj->bgImg = bgImg = loadBmp((SGtext)w.content);
	}

	virtual void draw(int id) {
		putBitmap(pos.x, pos.y, bgImg);
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
};
class Check :public Widget {
public:
	Check(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		switch (style) {
		case SG_DESIGN:
			useBackgroundRefresh(
				pos.x, pos.y, pos.x + size.x - 1, pos.y + size.y - 1);
			if (status&WIDGET_PRESSED)
				setColor(pressColor.r, pressColor.g, pressColor.b);
			else if (status&WIDGET_PASS)
				setColor(passColor.r, passColor.g, passColor.b);
			else setColor(bgColor.r, bgColor.g, bgColor.b);
			putCircle(pos.x + SG_CHAR_WIDTH, pos.y + SG_CHAR_HEIGHT / 2, 6, SOLID_FILL);
			setColor(63, 63, 63);
			putCircle(pos.x + SG_CHAR_WIDTH, pos.y + SG_CHAR_HEIGHT / 2, 6, EMPTY_FILL);
			if (status&WIDGET_SELECTED) {
				setColor(127, 127, 127);
				putCircle(pos.x + SG_CHAR_WIDTH, pos.y + SG_CHAR_HEIGHT / 2, 5, EMPTY_FILL);
			}
			if (value) {
				setColor(fgColor.r, fgColor.g, fgColor.b);
				putCircle(pos.x + SG_CHAR_WIDTH, pos.y + SG_CHAR_HEIGHT / 2, 2, SOLID_FILL);
			}
			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			putString((char *)content, pos.x + (int)(2.5f * SG_CHAR_WIDTH), pos.y - 2);
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (x >= pos.x&&x<pos.x + size.x&&y >= pos.y&&y < pos.y + size.y) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				status |= WIDGET_PRESSED;
				valid = 0;
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status |= WIDGET_SELECTED;
				if (value)value--;
				else value = 1;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
		if (status & WIDGET_PASS && status & WIDGET_SELECTED &&
			button == (SG_BUTTON_UP | SG_LEFT_BUTTON))
			click(obj);

	}
};
class Drag :public Widget {
public:
	Drag(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		useBackgroundRefresh(
			pos.x, pos.y, pos.x + size.x - 1, pos.y + size.y - 1);
		switch (style) {
		case SG_DESIGN:
			setColor(passColor.r, passColor.g, passColor.b);
			putQuad(pos.x, pos.y + size.y / 2 - 2,
				pos.x + size.x, pos.y + size.y / 2 + 2, EMPTY_FILL);
			if (status&WIDGET_PRESSED)
				setColor(pressColor.r, pressColor.g, pressColor.b);
			else if (status&WIDGET_PASS)
				setColor(passColor.r, passColor.g, passColor.b);
			else setColor(bgColor.r, bgColor.g, bgColor.b);
			putQuad(pos.x + value* (size.x - 12) / 100 + 2, pos.y,
				pos.x + value * (size.x - 12) / 100 + 10, pos.y + size.y - 1, SOLID_FILL);
			setColor(fgColor.r, fgColor.g, fgColor.b);
			putQuad(pos.x + value * (size.x - 12) / 100 + 2, pos.y,
				pos.x + value * (size.x - 12) / 100 + 10, pos.y + size.y - 1, EMPTY_FILL);
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseMove(int x, int y) {
		if (status & WIDGET_PRESSED) {
			if (x < pos.x + 6)obj->value = value = 0;
			else if (x >= pos.x + size.x - 6)obj->value = value = 100;
			else {
				obj->value = value = (x - pos.x - 6) * 100 / (size.x - 12);
			}
			valid = 0;
		}
		if (x >= pos.x + (float)value / 100 * (size.x - 12) + 2 &&
			x < pos.x + (float)value / 100 * (size.x - 12) + 10 &&
			y >= pos.y&&y < pos.y + size.y) {
			if (!(status & WIDGET_PASS)) {
				status |= WIDGET_PASS;
				valid = 0;
			}
		}
		else {
			if (status & WIDGET_PASS) {
				status &= 0xFF ^ WIDGET_PASS;
				valid = 0;
			}
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (x >= pos.x + (float)value / 100 * (size.x - 12) + 2 &&
			x < pos.x + (float)value / 100 * (size.x - 12) + 10 &&
			y >= pos.y&&y < pos.y + size.y) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				status |= WIDGET_PRESSED;
				valid = 0;
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status |= WIDGET_SELECTED;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
	}
};
class Process : public Widget {
public:
	Process(widget w, int window, string p = "") :Widget(w, window, p) {
		visible = false;
	}

	virtual void draw(int id) {
		int row, total, tmp;

		valid = 0;
		switch (style) {
		case SG_DESIGN:{
			if (bgImg.data == NULL) {
				setColor(bgColor.r, bgColor.g, bgColor.b);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + size.y - 1, SOLID_FILL);
				setColor(223, 223, 223);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + SG_CHAR_HEIGHT + 3, SOLID_FILL);
				setColor(127, 127, 127);
				putQuad(pos.x, pos.y,
					pos.x + size.x - 1, pos.y + size.y - 1, EMPTY_FILL);
			}
			else {
				putBitmap(pos.x, pos.y, bgImg);
			}
			if (status&WIDGET_PRESSED)setColor(191, 63, 63);
			else setColor(255, 63, 63);
			putQuad(pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1), pos.y + 2,
				pos.x + size.x - 3, pos.y + SG_CHAR_HEIGHT + 1, SOLID_FILL);
			setColor(255, 255, 255);
			putLine(pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1), pos.y + 2,
				pos.x + size.x - 2, pos.y + SG_CHAR_HEIGHT + 1, SOLID_LINE);
			putLine(pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1), pos.y + SG_CHAR_HEIGHT + 2,
				pos.x + size.x - 2, pos.y + 1, SOLID_LINE);

			if (status&WIDGET_SELECTED) {
				setColor(127, 127, 127);
				putQuad(pos.x + 1, pos.y + 1,
					pos.x + size.x - 2, pos.y + size.y - 2, EMPTY_FILL);
			}

			row = 0;
			total = 0;
			tmp = 0;
			setColor(tf.color.r, tf.color.g, tf.color.b);
			setFontSize(tf.size);
			setFontName(_shorten(tf.name));
			char *conv = _stringConvert((char *)content);
			while (tmp = putStringConstraint(conv + total,
				pos.x + SG_CHAR_WIDTH,
				pos.y + (row + 1) * tf.size + 8, 0,
				size.x - 2 * SG_CHAR_WIDTH)) {
				total += tmp;
				row++;
				if ((row + 4) * tf.size > size.y)break;
			}
			free(conv);

			setColor(223, 223, 223);
			putQuad(pos.x + 2 * SG_CHAR_WIDTH,
				pos.y + size.y - SG_CHAR_HEIGHT - 8,
				pos.x + size.x - 2 * SG_CHAR_WIDTH,
				pos.y + size.y - SG_CHAR_HEIGHT - 1,
				SOLID_FILL);
			setColor(63, 63, 63);
			putQuad(pos.x + 2 * SG_CHAR_WIDTH,
				pos.y + size.y - SG_CHAR_HEIGHT - 8,
				pos.x + 2 * SG_CHAR_WIDTH + (int)(value*1.f / 100 * (size.x - 4 * SG_CHAR_WIDTH)),
				pos.y + size.y - SG_CHAR_HEIGHT - 1,
				SOLID_FILL);
			setColor(63, 63, 63);
			putQuad(pos.x + 2 * SG_CHAR_WIDTH,
				pos.y + size.y - SG_CHAR_HEIGHT - 8,
				pos.x + size.x - 2 * SG_CHAR_WIDTH,
				pos.y + size.y - SG_CHAR_HEIGHT - 1,
				EMPTY_FILL);
			break;
		}
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (x >= pos.x&&x<pos.x + size.x&&y >= pos.y&&y < pos.y + size.y) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				if (x >= pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1) &&
					x < pos.x + size.x - 2 &&
					y >= pos.y + 2 &&
					y < pos.y + SG_CHAR_HEIGHT + 2) {
					status |= WIDGET_PRESSED;
					valid = 0;
				}
				else {
					status |= WIDGET_SELECTED;
					valid = 0;
				}
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				if (x >= pos.x + size.x - 2 * (SG_CHAR_WIDTH + 1) &&
					x < pos.x + size.x - 2 &&
					y >= pos.y + 2 &&
					y < pos.y + SG_CHAR_HEIGHT + 2) {
					ceaseWidget(name.data());
					return;
				}
				status |= WIDGET_SELECTED;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
	}
};
class ScrollVert : public Widget {
private:
	int previous = 0;

public:
	ScrollVert(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		double barHeight;
		double moveDelta;
		if (extra - 1 <= 0) {
			barHeight = size.y;
			moveDelta = 0;
		}
		else {
			barHeight = MINIMAL_BAR + (size.y - MINIMAL_BAR * 3) / sqrt(extra - 1);
			moveDelta = (size.y - barHeight) / (extra - 1);
		}
		switch (style) {
		case SG_DESIGN:
			setColor(bgColor.r, bgColor.g, bgColor.b);
			putQuad(pos.x, pos.y, pos.x + size.x - 1, pos.y + size.y - 1, SOLID_FILL);

			if (status&WIDGET_PRESSED)
				setColor(pressColor.r, pressColor.g, pressColor.b);
			else if (status&WIDGET_PASS)
				setColor(passColor.r, passColor.g, passColor.b);
			else setColor(fgColor.r, fgColor.g, fgColor.b);
			putQuad(pos.x + 2, pos.y + (int)(value * moveDelta + 2),
				pos.x + size.x - 3, pos.y + (int)(value * moveDelta + barHeight - 3), SOLID_FILL);
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseMove(int x, int y) {
		if (status & WIDGET_PRESSED) {
			double barHeight;
			double moveDelta;
			if (extra - 1 <= 0) {
				barHeight = size.y;
				moveDelta = 0;
			}
			else {
				barHeight = MINIMAL_BAR + (size.y - MINIMAL_BAR * 3) / sqrt(extra - 1);
				moveDelta = (size.y - barHeight) / (extra - 1);
			}

			value = obj->value = (int)((y - pos.y - barHeight / 2) / moveDelta);
			if (value < 0)value = obj->value = 0;
			if (value >= extra)value = obj->value = extra - 1;

			valid = 0;
		}
		else {
			if (inWidget(obj, x, y)) {
				if (!(status & WIDGET_PASS)) {
					status |= WIDGET_PASS;
					valid = 0;
				}
			}
			else {
				if (status & WIDGET_PASS) {
					status &= 0xFF ^ WIDGET_PASS;
					valid = 0;
				}
			}
		}
		if (value >= 0 && value < extra && value != previous) {
			drag(obj, 0, value - previous);
			previous = value;
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (inWidget(obj, x, y)) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				double barHeight;
				double moveDelta;
				if (extra - 1 <= 0) {
					barHeight = size.y;
					moveDelta = 0;
				}
				else {
					barHeight = MINIMAL_BAR + (size.y - MINIMAL_BAR * 3) / sqrt(extra - 1);
					moveDelta = (size.y - barHeight) / (extra - 1);
				}

				if (y < pos.y + value * moveDelta) {
					if (value > 0) {
						value--;
						obj->value--;
					}
				}
				else if (y >= pos.y + value * moveDelta + barHeight) {
					if (value < extra - 1) {
						value++;
						obj->value++;
					}
				}
				status |= WIDGET_PRESSED;
				valid = 0;
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status |= WIDGET_SELECTED;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
	}

	virtual void update() {
		previous = value;
	}
};
class ScrollHoriz : public Widget {
private:
	int previous = 0;

public:
	ScrollHoriz(widget w, int window, string p = "") :Widget(w, window, p) {}

	virtual void draw(int id) {
		double barWidth;
		double moveDelta;
		if (extra - 1 <= 0) {
			barWidth = size.x;
			moveDelta = 0;
		}
		else {
			barWidth = MINIMAL_BAR + (size.x - MINIMAL_BAR * 3) / sqrt(extra - 1);
			moveDelta = (size.x - barWidth) / (extra - 1);
		}
		switch (style) {
		case SG_DESIGN:
			setColor(bgColor.r, bgColor.g, bgColor.b);
			putQuad(pos.x, pos.y, pos.x + size.x - 1, pos.y + size.y - 1, SOLID_FILL);

			if (status&WIDGET_PRESSED)
				setColor(pressColor.r, pressColor.g, pressColor.b);
			else if (status&WIDGET_PASS)
				setColor(passColor.r, passColor.g, passColor.b);
			else setColor(fgColor.r, fgColor.g, fgColor.b);
			putQuad(pos.x + (int)(value * moveDelta + 2), pos.y + 2,
				pos.x + (int)(value * moveDelta + barWidth - 3), pos.y + size.y - 3, SOLID_FILL);
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
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseMove(int x, int y) {
		if (status & WIDGET_PRESSED) {
			double barWidth;
			double moveDelta;
			if (extra - 1 <= 0) {
				barWidth = size.x;
				moveDelta = 0;
			}
			else {
				barWidth = MINIMAL_BAR + (size.x - MINIMAL_BAR * 3) / sqrt(extra - 1);
				moveDelta = (size.x - barWidth) / (extra - 1);
			}

			value = obj->value = (int)((x - pos.x - barWidth / 2) / moveDelta);
			if (value < 0)value = obj->value = 0;
			if (value >= extra)value = obj->value = extra - 1;

			valid = 0;
		}
		else {
			if (inWidget(obj, x, y)) {
				if (!(status & WIDGET_PASS)) {
					status |= WIDGET_PASS;
					valid = 0;
				}
			}
			else {
				if (status & WIDGET_PASS) {
					status &= 0xFF ^ WIDGET_PASS;
					valid = 0;
				}
			}
		}
		if (value >= 0 && value < extra && value != previous) {
			drag(obj, value - previous, 0);
			previous = value;
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
			(status & WIDGET_PRESSED)) {
			status &= 0xFF ^ WIDGET_PRESSED;
			valid = 0;
		}
		if (inWidget(obj, x, y)) {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				double barWidth;
				double moveDelta;
				if (extra - 1 <= 0) {
					barWidth = size.x;
					moveDelta = 0;
				}
				else {
					barWidth = MINIMAL_BAR + (size.x - MINIMAL_BAR * 3) / sqrt(extra - 1);
					moveDelta = (size.x - barWidth) / (extra - 1);
				}

				if (x < pos.x + value * moveDelta) {
					if (value > 0) {
						value--;
						obj->value--;
					}
				}
				else if (x >= pos.x + value * moveDelta + barWidth) {
					if (value < extra - 1) {
						value++;
						obj->value++;
					}
				}
				status |= WIDGET_PRESSED;
				valid = 0;
			}
			if (button == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				status |= WIDGET_SELECTED;
				valid = 0;
			}
		}
		else {
			if (button == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
				(status & WIDGET_SELECTED)) {
				status &= 0xFF ^ WIDGET_SELECTED;
				valid = 0;
			}
		}
	}

	virtual void update() {
		previous = value;
	}
};

class CombinedWidget : public Widget {
public:
	CombinedWidget(widget w, int window, string p = "") : Widget(w, window, p) {
		if (w.child == NULL) {
			this->child = NULL;
			this->obj->child = NULL;
		}
		else {
			widget *iter = w.child;
			while (iter) {
				iter->pos.x += w.pos.x;
				iter->pos.y += w.pos.y;
				iter = iter->next;
			}
			switch (w.child->type) {
			case SG_BUTTON:
				this->child = new Button(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_INPUT:
				this->child = new Input(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_DIALOG:
				this->child = new Dialog(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_OUTPUT:
				this->child = new Output(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_LIST:
				this->child = new List(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_LABEL:
				this->child = new Label(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_PIC:
				this->child = new Pic(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_CHECK:
				this->child = new Check(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_PROCESS:
				this->child = new Process(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_DRAG:
				this->child = new Drag(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_SCROLLVERT:
				this->child = new ScrollVert(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_SCROLLHORIZ:
				this->child = new ScrollHoriz(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			case SG_COMBINED:
				this->child = new CombinedWidget(*w.child, window, (path == "" ? "" : path + '/') + name);
				break;
			default:
				break;
			}
			this->obj->child = this->child->getObj();
			delete w.child;
		}
	}
	virtual ~CombinedWidget() {

	}

	virtual void draw(int id) {
		Widget *iter = child;
		while (iter) {
			if (!iter->visible || iter->valid) {
				iter = iter->next;
				continue;
			}
			iter->valid = 1;

			iter->draw(-1);
			if (!iter->valid)valid = 0;
			iter = iter->next;
		}
		if (id >= 0) {
			widgetCover(window, id, pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		}
	}
	virtual void mouseMove(int x, int y) {
		Widget::mouseMove(x, y);
		Widget *iter = child;
		while (iter) {
			iter->mouseMove(x, y);
			iter = iter->next;
		}
	}
	virtual void mouseClick(int x, int y, int button) {
		Widget::mouseClick(x, y, button);
		Widget *iter = child;
		while (iter) {
			if (!crossWidget(iter->obj, pos.x, pos.y, pos.x + size.x, pos.y + size.y)) {
				iter = iter->next;
				continue;
			}
			iter->mouseClick(x, y, button);
			iter = iter->next;
		}
	}
	virtual void keyPress(int key, bool utf16 = false) {
		Widget::keyPress(key, utf16);
		Widget *iter = child;
		while (iter) {
			if (!crossWidget(iter->obj, pos.x, pos.y, pos.x + size.x, pos.y + size.y)) {
				iter = iter->next;
				continue;
			}
			iter->keyPress(key, utf16);
			iter = iter->next;
		}
	}

	virtual void update() {
		Widget *iter = child;
		while (iter) {
			if (!iter->visible || iter->valid) {
				iter = iter->next;
				continue;
			}
			iter->valid = 1;

			iter->update();
			iter = iter->next;
		}
	}
};

Widget::Widget(widget w, int window, string p) {
	this->window = window;
	this->obj = (widget *)malloc(sizeof(widget));
	this->type = this->obj->type = w.type;

	this->pos = this->obj->pos = w.pos;
	this->size = this->obj->size = w.size;

	this->style = this->obj->style = w.style;
	this->status = FALSE;
	this->visible = TRUE;
	this->priority = WIDGET_FRONT;
	this->valid = 0;

	this->value = this->obj->value = w.value;
	this->extra = this->obj->extra = w.extra;

	this->name = this->obj->name = w.name;
	this->path = p;
	this->tf.color = this->obj->tf.color = w.tf.color;
	this->tf.name = (SGWINSTR)malloc((_strlen(w.tf.name) + 1) * sizeof(wchar_t));
	_strcpy(this->tf.name, w.tf.name);
	this->obj->tf.name = w.tf.name;
	this->tf.size = this->obj->tf.size = w.tf.size;
	this->tf.coeff = this->obj->tf.coeff = w.tf.coeff;
	this->content = this->obj->content = w.content;

	this->child = NULL;
	this->obj->child = NULL;
	this->next = NULL;
	this->obj->next = NULL;

	this->bgColor = this->obj->bgColor = w.bgColor;
	this->passColor = this->obj->passColor = w.passColor;
	this->pressColor = this->obj->pressColor = w.pressColor;
	this->fgColor = this->obj->fgColor = w.fgColor;
	this->bgImg = this->obj->bgImg = w.bgImg;

	this->click = this->obj->click = w.click;
	this->move = this->obj->move = w.move;
	this->drag = this->obj->drag = w.drag;
	this->press = this->obj->press = w.press;

	if (w.next == NULL) {
		this->next = NULL;
		this->obj->next = NULL;
	}
	else {
		switch (w.next->type) {
		case SG_BUTTON:
			this->next = new Button(*w.next, window, path);
			break;
		case SG_INPUT:
			this->next = new Input(*w.next, window, path);
			break;
		case SG_DIALOG:
			this->next = new Dialog(*w.next, window, path);
			break;
		case SG_OUTPUT:
			this->next = new Output(*w.next, window, path);
			break;
		case SG_LIST:
			this->next = new List(*w.next, window, path);
			break;
		case SG_LABEL:
			this->next = new Label(*w.next, window, path);
			break;
		case SG_PIC:
			this->next = new Pic(*w.next, window, path);
			break;
		case SG_CHECK:
			this->next = new Check(*w.next, window, path);
			break;
		case SG_PROCESS:
			this->next = new Process(*w.next, window, path);
			break;
		case SG_DRAG:
			this->next = new Drag(*w.next, window, path);
			break;
		case SG_SCROLLVERT:
			this->next = new ScrollVert(*w.next, window, path);
			break;
		case SG_SCROLLHORIZ:
			this->next = new ScrollHoriz(*w.next, window, path);
			break;
		case SG_COMBINED:
			this->next = new CombinedWidget(*w.next, window, path);
			break;
		default:
			break;
		}
		this->obj->next = this->next->getObj();
		delete w.next;
	}
}

#endif
