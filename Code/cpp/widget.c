#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"


extern int _sglMode;
extern struct _text text;

widgetObj *newWidget(int type, const char *name) {
	widgetObj *ret = (widgetObj *)malloc(sizeof(widgetObj));

	ret->type = type;

	ret->pos.x = ret->pos.y = 0;
	ret->size.x = ret->size.y = 0;

	ret->style = SG_DESIGN;
	ret->status = FALSE;
	ret->visible = TRUE;
	ret->priority = WIDGET_FRONT;
	ret->valid = 0;

	ret->hide = 0;
	ret->value = 0;

	ret->name = (unsigned char *)malloc(strlen(name) + 1);
	strcpy(ret->name, name);
	ret->content = (unsigned char *)malloc(1024 * sizeof(unsigned char *));
	memset(ret->content, 0, 1024 * sizeof(unsigned char *));

	ret->associate = NULL;
	ret->next = NULL;
	ret->child = NULL;

	ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 223;
	ret->passColor.r = ret->passColor.g = ret->passColor.b = 191;
	ret->pressColor.r = ret->pressColor.g = ret->pressColor.b = 159;
	ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
	ret->bgImg.sizeX = ret->bgImg.sizeY = 0;
	ret->bgImg.data = NULL;

	ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveDefault;
	ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickDefault;
	ret->mouseUser = (mouseClickUser)mouseUserDefault;
	ret->keyDown = ret->keyUp = (keyPressCall)keyDefault;
	ret->keyUser = (keyPressUser)keyUserDefault;
	switch (type) {
	case SG_BUTTON:
		break;
	case SG_INPUT:
		ret->passColor.r = ret->passColor.g = ret->passColor.b = 255;
		ret->pressColor.r = ret->pressColor.g = ret->pressColor.b = 255;
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 239;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickInput;
		ret->keyDown = ret->keyUp = (keyPressCall)keyInput;
		break;
	case SG_DIALOG:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 191;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
		ret->visible = FALSE;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickDialog;
		break;
	case SG_OUTPUT:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 255;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
		break;
	case SG_LIST:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 223;
		ret->passColor.r = ret->passColor.g = ret->passColor.b = 255;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
		ret->keyDown = ret->keyUp = (keyPressCall)keyList;
		ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveList;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickList;
		break;
	case SG_LABEL:
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
		break;
	case SG_PIC:
		break;
	case SG_CHECK:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 239;
		ret->passColor.r = ret->passColor.g = ret->passColor.b = 223;
		ret->pressColor.r = ret->pressColor.g = ret->pressColor.b = 191;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickCheck;
		break;
	case SG_PROCESS:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 191;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;
		ret->visible = FALSE;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickProcess;
		break;
	case SG_OPTION:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 255;
		ret->value = -1;
		ret->keyDown = ret->keyUp = (keyPressCall)keyOption;
		ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveOption;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickOption;
		break;
	case SG_DRAG:
		ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveDrag;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickDrag;
		break;
	case SG_SCROLLVERT:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 95;
		ret->passColor.r = ret->passColor.g = ret->passColor.b = 223;
		ret->pressColor.r = ret->pressColor.g = ret->pressColor.b = 255;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 191;
		if (getWidth(SG_SCREEN) < SCROLL_WIDTH) {
			ret->pos.x = 0;
			ret->size.x = getWidth(SG_SCREEN);
		}
		else {
			ret->pos.x = getWidth(SG_SCREEN) - SCROLL_WIDTH;
			ret->size.x = SCROLL_WIDTH;
		}
		ret->pos.y = 0;
		ret->size.y = getHeight(SG_SCREEN);

		ret->hide = 100;
		ret->keyDown = ret->keyUp = (keyPressCall)keyScrollVert;
		ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveScrollVert;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickScrollVert;
		break;
	case SG_SCROLLHORIZ:
		ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 95;
		ret->passColor.r = ret->passColor.g = ret->passColor.b = 223;
		ret->pressColor.r = ret->pressColor.g = ret->pressColor.b = 255;
		ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 191;
		if (getHeight(SG_SCREEN) < SCROLL_WIDTH) {
			ret->pos.y = 0;
			ret->size.y = getHeight(SG_SCREEN);
		}
		else {
			ret->pos.y = getHeight(SG_SCREEN) - SCROLL_WIDTH;
			ret->size.y = SCROLL_WIDTH;
		}
		ret->pos.x = 0;
		ret->size.x = getWidth(SG_SCREEN);

		ret->hide = 100;
		ret->keyDown = ret->keyUp = (keyPressCall)keyScrollHoriz;
		ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveScrollHoriz;
		ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickScrollHoriz;
		break;
	}
	return ret;
}
widgetObj *newCombinedWidget(int num, const char *name, ...) {
	widgetObj *ret = (widgetObj *)malloc(sizeof(widgetObj));
	widgetObj *iter;
	va_list ap;

	ret->type = SG_COMBINED;

	ret->pos.x = ret->pos.y = 0;
	ret->size.x = ret->size.y = 0;

	ret->style = SG_DESIGN;
	ret->status = FALSE;
	ret->visible = TRUE;
	ret->priority = WIDGET_FRONT;
	ret->valid = 0;

	ret->hide = 0;
	ret->value = 0;

	ret->name = (unsigned char *)malloc(strlen(name) + 1);
	strcpy(ret->name, name);
	ret->content = (unsigned char *)malloc(1024 * sizeof(unsigned char *));
	memset(ret->content, 0, 1024 * sizeof(unsigned char *));

	ret->associate = NULL;
	ret->next = NULL;

	ret->bgColor.r = ret->bgColor.g = ret->bgColor.b = 223;
	ret->passColor.r = ret->passColor.g = ret->passColor.b = 191;
	ret->pressColor.r = ret->pressColor.g = ret->pressColor.b = 159;
	ret->fgColor.r = ret->fgColor.g = ret->fgColor.b = 0;

	ret->mouseIn = ret->mouseOut = (mouseMoveCall)mouseMoveCombined;
	ret->mouseDown = ret->mouseUp = (mouseClickCall)mouseClickCombined;
	ret->keyDown = ret->keyUp = (keyPressCall)keyDefault;

	va_start(ap, name);
	iter = ret;
	iter->child = va_arg(ap, widgetObj*);
	iter = iter->child;
	while (num--) {
		iter->next = va_arg(ap, widgetObj*);
		iter = iter->next;
	}
	va_end(ap);

	return ret;
}
int registerWidget(widgetObj *obj) {
	widgetObj *tmp, *sub;
	struct _hash *end;
	int i, len, offx, offy, fit = 0, sum = 0;

	if (obj == NULL)return SG_NULL_POINTER;
	if (obj->name == NULL || obj->name[0] == 0)return SG_INCOMPLETE_STRUCT;
	if (Widget->count >= SG_QSIZE)return SG_OUT_OF_RANGE;

	tmp = (widgetObj *)malloc(sizeof(widgetObj));
	memcpy(tmp, obj, sizeof(widgetObj));
	tmp->name = (char *)malloc(strlen(obj->name) + 1);
	strcpy(tmp->name, obj->name);

	if (obj->type == SG_PIC) {
		void *pre = tmp->content;
		*((bitMap *)tmp->content) = loadBmp(tmp->content);
		free(pre);
	}
	if (obj->type == SG_COMBINED) {
		sub = tmp;
		offx = obj->pos.x;
		offy = obj->pos.y;
		if (obj->size.x == 0 || obj->size.y == 0)fit = 1;
		while (obj->next) {
			sub->next = (widgetObj *)malloc(sizeof(widgetObj));
			memcpy(sub->next, obj->next, sizeof(widgetObj));
			if (fit) {
				if (sub->next->pos.x + sub->next->size.x > tmp->size.x)
					tmp->size.x = sub->next->pos.x + sub->next->size.x;
				if (sub->next->pos.y + sub->next->size.y > tmp->size.y)
					tmp->size.y = sub->next->pos.y + sub->next->size.y;
			}
			sub->next->pos.x += offx;
			sub->next->pos.y += offy;
			sub = sub->next;
			obj = obj->next;
		}
		sub->next = NULL;
	}

	Widget->obj[Widget->count] = tmp;

	len = (int)strlen(tmp->name);
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


/*
* Inner widget callback functions.
* If the callback functions of some widget is changed, please put
* these default function in the beginning of the new function.
* Started in v2.1.0
*/
void mouseUserDefault(widgetObj *w) {

}
void keyUserDefault(widgetObj *w, int key) {

}

void mouseMoveDefault(widgetObj *w, int x, int y) {
	
}
void mouseMoveList(widgetObj *w, int x, int y) {

}
void mouseMoveOption(widgetObj *w, int x, int y) {

}
void mouseMoveDrag(widgetObj *w, int x, int y) {

}
void mouseMoveScrollVert(widgetObj *w, int x, int y) {

}
void mouseMoveScrollHoriz(widgetObj *w, int x, int y) {

}
void mouseMoveCombined(widgetObj *w, int x, int y) {

}

void mouseClickUserDefault(widgetObj *w, int x, int y, int status) {

}
void mouseClickDefault(widgetObj *w, int x, int y, int status) {

}
void mouseClickInput(widgetObj *w, int x, int y, int status) {

}
void mouseClickDialog(widgetObj *w, int x, int y, int status) {

}
void mouseClickList(widgetObj *w, int x, int y, int status) {

}
void mouseClickCheck(widgetObj *w, int x, int y, int status) {

}
void mouseClickProcess(widgetObj *w, int x, int y, int status) {

}
void mouseClickOption(widgetObj *w, int x, int y, int status) {

}
void mouseClickDrag(widgetObj *w, int x, int y, int status) {

}
void mouseClickScrollVert(widgetObj *w, int x, int y, int status) {

}
void mouseClickScrollHoriz(widgetObj *w, int x, int y, int status) {

}
void mouseClickCombined(widgetObj *w, int x, int y, int status) {

}

void keyDefault(widgetObj *w, int key) {

}
void keyInput(widgetObj *w, int key) {

}
void keyList(widgetObj *w, int key) {

}
void keyOption(widgetObj *w, int key) {

}
void keyScrollVert(widgetObj *w, int key) {

}
void keyScrollHoriz(widgetObj *w, int key) {

}
void keyCombined(widgetObj *w, int key) {

}


/*
* Main drawing.
* Start in v2.1.0
*/
void _drawWidget(int fb) {
	int i;
	widgetObj *current;

	if (_sglMode == BIT_MAP) {
		for (i = 0; i < Widget->count; i++) {
			current = Widget->obj[i];
			if (!current || !current->visible || current->priority != fb)continue;

			if (current->valid)continue;
			current->valid = 1;

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
			case SG_PIC:
				_drawPic(current);
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
			case SG_SCROLLVERT:
				_drawScrollVert(current);
				break;
			case SG_SCROLLHORIZ:
				_drawScrollHoriz(current);
				break;

			case SG_COMBINED:
				_drawCombined(current);
				break;
			}
		}
	}
}
void _drawSubWidget(int id, int fb) {
	int i;
	widgetObj *current;

	if (_sglMode == BIT_MAP) {
		startSubWindow(id);
		for (i = 0; i < Widget->count; i++) {
			current = Widget->obj[i];
			if (!current || !current->visible || current->priority != fb)continue;

			if (current->valid)continue;
			current->valid = 1;

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
			case SG_PIC:
				_drawPic(current);
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
			case SG_SCROLLVERT:
				_drawScrollVert(current);
				break;
			case SG_SCROLLHORIZ:
				_drawScrollHoriz(current);
				break;

			case SG_COMBINED:
				_drawCombined(current);
				break;
			}
		}
		endSubWindow();
	}
}
void _drawButton(widgetObj *w) {
	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_PRESSED)
			setColor(w->pressColor.r, w->pressColor.g, w->pressColor.b);
		else if (w->status&WIDGET_PASS)
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
		else setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(127, 127, 127);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
		if (w->status&WIDGET_SELECTED) {
			setColor(127, 127, 127);
			putQuad(w->pos.x + 2, w->pos.y + 2,
				w->pos.x + w->size.x - 3, w->pos.y + w->size.y - 3, EMPTY_FILL);
		}
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);

		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
		SGWINSTR _wd = NULL;
		GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), _strlenW(w->content), &text.strRect);
		free((void *)_wd);
		putString(w->content,
			w->pos.x + w->size.x / 2 - text.strRect.cx / 2,
			w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2 - 3);
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

}
void _drawDialog(widgetObj *w) {

}
void _drawOutput(widgetObj *w) {

}
void _drawList(widgetObj *w) {

}
void _drawLable(widgetObj *w) {

}
void _drawPic(widgetObj *w) {

}
void _drawCheck(widgetObj *w) {

}
void _drawProcess(widgetObj *w) {

}
void _drawOption(widgetObj *w) {

}
void _drawDrag(widgetObj *w) {

}
void _drawScrollVert(widgetObj *w) {

}
void _drawScrollHoriz(widgetObj *w) {

}
void _drawCombined(widgetObj *w) {

}

