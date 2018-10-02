#define _CRT_SECURE_NO_WARNINGS
#include "..\include\inner.h"
#include "..\include\winsgl.h"


extern int _sglMode;
extern int _drawingWidget;
extern struct _text text;
extern struct _Sub _wndList[SG_MAX_WINDOW_NUM];

void(*backgroundRefresh)(int left, int top, int right, int bottom);	


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
	while (--num) {
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
	if (currentWindow == -1) {
		if (Widget->count >= SG_QSIZE)return SG_OUT_OF_RANGE;
	}
	else
		if (_wndList[currentWindow].widget->count >= SG_QSIZE)return SG_OUT_OF_RANGE;
	

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
		if (obj->child) {
			sub->child = (widgetObj *)malloc(sizeof(widgetObj));
			memcpy(sub->child, obj->child, sizeof(widgetObj));
			if (fit) {
				if (sub->child->pos.x + sub->child->size.x > tmp->size.x)
					tmp->size.x = sub->child->pos.x + sub->child->size.x;
				if (sub->child->pos.y + sub->child->size.y > tmp->size.y)
					tmp->size.y = sub->child->pos.y + sub->child->size.y;
			}
			sub->child->pos.x += offx;
			sub->child->pos.y += offy;
			sub = sub->child;
			obj = obj->child;

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
		else sub->child = NULL;
	}

	if (currentWindow == -1) {
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
	else {
		_wndList[currentWindow].widget->obj[_wndList[currentWindow].widget->count] = tmp;

		len = (int)strlen(tmp->name);
		for (i = 0; i < len; i++)sum += tmp->name[i];
		sum %= 256;
		end = _wndList[currentWindow].widget->hash[sum];
		if (end == NULL) {
			_wndList[currentWindow].widget->hash[sum] = (struct _hash *)malloc(sizeof(struct _hash));
			_wndList[currentWindow].widget->hash[sum]->content = (char *)malloc(strlen(tmp->name) + 1);
			strcpy(_wndList[currentWindow].widget->hash[sum]->content, tmp->name);
			_wndList[currentWindow].widget->hash[sum]->value = _wndList[currentWindow].widget->count;
			_wndList[currentWindow].widget->hash[sum]->next = NULL;
		}
		else {
			while (end->next != NULL) {
				if (strcmp(end->content, obj->name) == 0)return SG_MULTY_VALUE;
				end = end->next;
			}
			end->next = (struct _hash *)malloc(sizeof(struct _hash));
			end->next->content = (char *)malloc(strlen(tmp->name) + 1);
			strcpy(end->next->content, tmp->name);
			end->next->value = _wndList[currentWindow].widget->count;
			end->next->next = NULL;
		}
		return _wndList[currentWindow].widget->count++;
	}
}
int inWidget(widgetObj *obj, int x, int y) {
	if (obj == NULL)return 0;
	return (x >= obj->pos.x&&x < obj->pos.x + obj->size.x) &&
		(y >= obj->pos.y&&y < obj->pos.y + obj->size.y);
}
int crossWidget(widgetObj *obj, int left, int top, int right, int bottom) {
	int horizontal = 0, vertical = 0;
	if (obj == NULL)return 0;

	if (left >= obj->pos.x && left < obj->pos.x + obj->size.x)horizontal = 1;
	if (right >= obj->pos.x && right < obj->pos.x + obj->size.x)horizontal = 1;
	if (obj->pos.x >= left && obj->pos.x < right)horizontal = 1;
	if (obj->pos.x + obj->size.x >= left && obj->pos.x + obj->size.x < right)horizontal = 1;

	if (top >= obj->pos.y && top < obj->pos.y + obj->size.y)vertical = 1;
	if (bottom >= obj->pos.y && bottom < obj->pos.y + obj->size.y)vertical = 1;
	if (obj->pos.y >= top && obj->pos.y < bottom)vertical = 1;
	if (obj->pos.y + obj->size.y >= top && obj->pos.y + obj->size.y < bottom)vertical = 1;

	return horizontal && vertical;
}
int easyWidget(int type, const char *name,
	int x, int y, int width, int height, const char *content, mouseClickUser click) {
	widgetObj *tmp;

	tmp = newWidget(type, name);
	tmp->pos.x = x;
	tmp->pos.y = y;
	tmp->size.x = width;
	tmp->size.y = height;

	strcpy(tmp->content, content);
	if (click)tmp->mouseUser = click;

	switch (type) {
	case SG_BUTTON:
	case SG_INPUT:
	case SG_DIALOG:
	case SG_OUTPUT:
	case SG_LIST:
	case SG_LABEL:
	case SG_PIC:
	case SG_CHECK:
	case SG_PROCESS:
	case SG_OPTION:
	case SG_DRAG:
		break;
	default:
		return SG_INVALID_MODE;
	}
	return registerWidget(tmp);
}
void setBackgroundRefresh(void(*refresh)(int left, int top, int right, int bottom)) {
	if (currentWindow == -1)
		backgroundRefresh = refresh;
	else
		_wndList[currentWindow].background = refresh;
}
widgetObj *getWidgetByIndex(int index) {
	if (index < 0 || index >= Widget->count)return NULL;
	return Widget->obj[index];
}
widgetObj *getWidgetByName(const char *name) {
	int i, len, sum = 0;
	struct _hash *end;
	len = (int)strlen(name);
	for (i = 0; i < len; i++)sum += name[i];
	sum %= 256;
	end = Widget->hash[sum];
	while (end != NULL) {
		if (strcmp(end->content, name) == 0)return Widget->obj[end->value];
		end = end->next;
	}
	return NULL;
}
int getIndexByName(const char *name) {
	int i, len, sum = 0;
	struct _hash *end;
	len = (int)strlen(name);
	for (i = 0; i < len; i++)sum += name[i];
	sum %= 256;
	end = Widget->hash[sum];
	while (end != NULL) {
		if (strcmp(end->content, name) == 0)return end->value;
		end = end->next;
	}
	return SG_OBJECT_NOT_FOUND;
}
void showWidget(const char *name) {
	widgetObj *obj = getWidgetByName(name);
	if (obj == NULL)return;
	if (obj->visible == TRUE) return;
	obj->visible = TRUE;
	obj->valid = 0;
}
void ceaseWidget(const char *name) {
	widgetObj *obj = getWidgetByName(name);
	if (obj == NULL)return;
	if (obj->visible == FALSE)return;
	obj->status = 0;
	obj->visible = FALSE;

	backgroundRefresh(
		obj->pos.x, obj->pos.y, obj->pos.x + obj->size.x, obj->pos.y + obj->size.y);
}
int deleteWidgetByIndex(int index) {
	if (index >= Widget->count || Widget->obj[index] == NULL)return SG_OUT_OF_RANGE;

	deleteWidgetByName(Widget->obj[index]->name);
	return SG_NO_ERORR;
}
int deleteWidgetByName(const char *name) {
	int i, len, sum1 = 0, sum2 = 0;
	struct _hash *prev = NULL, *end, *tmp, *move;
	len = (int)strlen(name);
	for (i = 0; i < len; i++)sum1 += name[i];
	sum1 %= 256;
	end = Widget->hash[sum1];
	if (end == NULL) {
		return SG_OBJECT_NOT_FOUND;
	}
	while (end->next != NULL) {
		if (strcmp(end->content, name) == 0)break;
		end = end->next;
		prev = end;
	}

	backgroundRefresh(
		Widget->obj[end->value]->pos.x, Widget->obj[end->value]->pos.y,
		Widget->obj[end->value]->pos.x + Widget->obj[end->value]->size.x,
		Widget->obj[end->value]->pos.y + Widget->obj[end->value]->size.y);
	if (end->next) {
		tmp = end->next;
		if (end->value == --Widget->count) {
			deleteSubWidget(Widget->obj[end->value]);
			free(Widget->obj[end->value]->name);
			free(Widget->obj[end->value]);
			Widget->obj[end->value] = NULL;
			if (Widget->active == end->value)Widget->active = -1;
		}
		else {
			deleteSubWidget(Widget->obj[end->value]);
			free(Widget->obj[end->value]->name);
			free(Widget->obj[end->value]);
			len = (int)strlen(Widget->obj[Widget->count]->name);
			for (i = 0; i < len; i++)sum2 += Widget->obj[Widget->count]->name[i];
			sum2 %= 256;
			move = Widget->hash[sum2];
			while (move->next != NULL) {
				if (strcmp(move->content, Widget->obj[Widget->count]->name) == 0)break;
				move = move->next;
			}
			if (move == NULL) {
				return SG_OBJECT_NOT_FOUND;
			}
			move->value = end->value;
			if (Widget->active == end->next->value)Widget->active = -1;
			if (Widget->active == Widget->count)Widget->active = end->next->value;
			Widget->obj[end->value] = Widget->obj[Widget->count];
			Widget->obj[Widget->count] = NULL;
		}
		free(end->content);
		free(end);
		if (prev)prev->next = tmp;
		else Widget->hash[sum1] = tmp;
	}
	else {
		if (end->value == --Widget->count) {
			deleteSubWidget(Widget->obj[end->value]);
			free(Widget->obj[end->value]->name);
			free(Widget->obj[end->value]);
			Widget->obj[end->value] = NULL;
			if (Widget->active == end->value)Widget->active = -1;
		}
		else {
			deleteSubWidget(Widget->obj[end->value]);
			free(Widget->obj[end->value]->name);
			free(Widget->obj[end->value]);
			len = (int)strlen(Widget->obj[Widget->count]->name);
			for (i = 0; i < len; i++)sum2 += Widget->obj[Widget->count]->name[i];
			sum2 %= 256;
			move = Widget->hash[sum2];
			while (move->next != NULL) {
				if (strcmp(move->content, Widget->obj[Widget->count]->name) == 0)break;
				move = move->next;
			}
			if (move == NULL) {
				return SG_OBJECT_NOT_FOUND;
			}
			move->value = end->value;
			if (Widget->active == end->value)Widget->active = -1;
			if (Widget->active == Widget->count)Widget->active = end->value;
			Widget->obj[end->value] = Widget->obj[Widget->count];
			Widget->obj[Widget->count] = NULL;
		}
		free(end->content);
		free(end);
		if (prev)prev->next = NULL;
		else Widget->hash[sum1] = NULL;
	}
	return SG_NO_ERORR;
}
void deleteSubWidget(widgetObj *w) {

}
void moveWidgetByIndex(int index, int xDelta, int yDelta) {
	widgetObj *tmp = getWidgetByIndex(index);

	backgroundRefresh(
		tmp->pos.x, tmp->pos.y, tmp->pos.x + tmp->size.x, tmp->pos.y + tmp->size.y);

	tmp->pos.x += xDelta;
	tmp->pos.y += yDelta;
}
void moveWidgetByName(const char *name, int xDelta, int yDelta) {
	widgetObj *tmp = getWidgetByName(name);

	backgroundRefresh(
		tmp->pos.x, tmp->pos.y, tmp->pos.x + tmp->size.x, tmp->pos.y + tmp->size.y);

	tmp->pos.x += xDelta;
	tmp->pos.y += yDelta;
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
	if (inWidget(w, x, y)) {
		if (!(w->status & WIDGET_PASS)) {
			w->status |= WIDGET_PASS;
			w->valid = 0;
		}
	}
	else {
		if (w->status & WIDGET_PASS) {
			w->status &= 0xFF ^ WIDGET_PASS;
			w->status &= 0xFF ^ WIDGET_PRESSED;
			w->valid = 0;
		}
	}
}
void mouseMoveList(widgetObj *w, int x, int y) {
	if (w->status&WIDGET_SELECTED) {
		if (w->status&WIDGET_PRESSED&&
			x >= w->pos.x&&x < w->pos.x + w->size.x&&
			y >= w->pos.y + w->size.y - w->hide * SG_LINE_DELTA_DEFAULT&&y < w->pos.y + w->size.y) {
			w->status |= WIDGET_PRESSED;
			w->value = (y - w->pos.y - w->size.y) / SG_LINE_DELTA_DEFAULT + w->hide - 1;
			if (w->value < 0)w->value = 0;
			w->valid = 0;
		}
		else {
			if (inWidget(w, x, y)) {
				w->status |= WIDGET_PASS;
				w->value = (y - w->pos.y - w->size.y) / SG_LINE_DELTA_DEFAULT + w->hide - 1;
				if (w->value < 0)w->value = 0;
				w->valid = 0;
			}
			else {
				w->status &= 0xFF ^ WIDGET_PASS;
				w->status &= 0xFF ^ WIDGET_PRESSED;
			}
		}
	}
	else {
		if (inWidget(w, x, y)) {
			if (!(w->status & WIDGET_PASS)) {
				w->status |= WIDGET_PASS;
				w->valid = 0;
			}
		}
		else {
			if (w->status & WIDGET_PASS) {
				w->status &= 0xFF ^ WIDGET_PASS;
				w->status &= 0xFF ^ WIDGET_PRESSED;
				w->valid = 0;
			}
		}
	}
}
void mouseMoveOption(widgetObj *w, int x, int y) {
	if (inWidget(w, x, y)) {
		w->status |= WIDGET_PASS;
		if (w->status & WIDGET_SELECTED)
			w->value = (y - w->pos.y) / SG_LINE_DELTA_DEFAULT;
		w->valid = 0;
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
		w->valid = 0;
	}
	if (x >= w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 2 &&
		x < w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 10 &&
		y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (!(w->status & WIDGET_PASS)) {
			w->status |= WIDGET_PASS;
			w->valid = 0;
		}
	}
	else {
		if (w->status & WIDGET_PASS) {
			w->status &= 0xFF ^ WIDGET_PASS;
			w->valid = 0;
		}
	}
}
void mouseMoveScrollVert(widgetObj *w, int x, int y) {
	if (w->status & WIDGET_PRESSED) {
		double barHeight;
		double moveDelta;
		if (w->hide - 1 <= 0) {
			barHeight = w->size.y;
			moveDelta = 0;
		}
		else {
			barHeight = MINIMAL_BAR + (w->size.y - MINIMAL_BAR * 3) / sqrt(w->hide - 1);
			moveDelta = (w->size.y - barHeight) / (w->hide - 1);
		}

		w->value = (int)((y - w->pos.y - barHeight / 2) / moveDelta);
		if (w->value < 0)w->value = 0;
		if (w->value >= w->hide)w->value = w->hide - 1;

		w->valid = 0;
	}
	else {
		if (inWidget(w, x, y)) {
			if (!(w->status & WIDGET_PASS)) {
				w->status |= WIDGET_PASS;
				w->valid = 0;
			}
		}
		else {
			if (w->status & WIDGET_PASS) {
				w->status &= 0xFF ^ WIDGET_PASS;
				w->valid = 0;
			}
		}
	}
}
void mouseMoveScrollHoriz(widgetObj *w, int x, int y) {
	if (w->status & WIDGET_PRESSED) {
		double barWidth;
		double moveDelta;
		if (w->hide - 1 <= 0) {
			barWidth = w->size.x;
			moveDelta = 0;
		}
		else {
			barWidth = MINIMAL_BAR + (w->size.x - MINIMAL_BAR * 3) / sqrt(w->hide - 1);
			moveDelta = (w->size.x - barWidth) / (w->hide - 1);
		}

		w->value = (int)((x - w->pos.x - barWidth / 2) / moveDelta);
		if (w->value < 0)w->value = 0;
		if (w->value >= w->hide)w->value = w->hide - 1;

		w->valid = 0;
	}
	else {
		if (inWidget(w, x, y)) {
			if (!(w->status & WIDGET_PASS)) {
				w->status |= WIDGET_PASS;
				w->valid = 0;
			}
		}
		else {
			if (w->status & WIDGET_PASS) {
				w->status &= 0xFF ^ WIDGET_PASS;
				w->valid = 0;
			}
		}
	}
}
void mouseMoveCombined(widgetObj *w, int x, int y) {
	widgetObj *S[SG_QSIZE], *tmp;
	int top = 0;

#define PUSH(x) S[top++] = x
#define POP() S[--top]
#define ISEMPTY() (top == 0)

	if (inWidget(w, x, y)) {
		w->status |= WIDGET_PASS;
		tmp = w->child;
		while (tmp) {
			PUSH(tmp);
			tmp = tmp->next;
		}
		while (!ISEMPTY()) {
			tmp = POP();
			if (inWidget(tmp, x, y)) {
				tmp->mouseIn(tmp, x, y);
				break;
			}
			else {
				tmp->mouseOut(tmp, x, y);
			}
		}
	}
	else {
		w->status &= 0xFF ^ WIDGET_PASS;
		w->status &= 0xFF ^ WIDGET_PRESSED;
		tmp = w->child;
		while (tmp) {
			tmp->mouseOut(tmp, x, y);
			tmp = tmp->next;
		}
	}

#undef PUSH
#undef POP
#undef ISEMPTY

}

void mouseClickUserDefault(widgetObj *w, int x, int y, int status) {

}
void mouseClickDefault(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (inWidget(w, x, y)) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_PRESSED;
			w->valid = 0;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->valid = 0;
		}
	}
	switch (w->type) {
	case SG_BUTTON:
		if (w->status & WIDGET_PASS &&
			status == (SG_BUTTON_UP | SG_LEFT_BUTTON))
			w->mouseUser(w);
		break;
	default:
		break;
	}
}
void mouseClickInput(widgetObj *w, int x, int y, int status) {
	int i = 0, len;
	SIZE tmp;
	SGWINSTR _wd = NULL;
	len = _strlenW(w->content);

	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (inWidget(w, x, y)) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_PRESSED;
			w->status |= WIDGET_FOCUSED;
			w->valid = 0;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			w->valid = 0;
			if (((char *)w->content)[0] == '\0')w->value = 0;
			else if (x >= w->pos.x + 2 && x < w->pos.x + w->size.x - 2) {
				GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), w->hide, &tmp);
				free((void *)_wd);
				w->value = (x - w->pos.x) / 20 + w->hide;
				x += tmp.cx - w->pos.x;
				do {
					w->value++;
					GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), w->value, &tmp);
					free((void *)_wd);
				} while (tmp.cx < x - 4);
				w->value--;
				if (w->value >= len)w->value = len;
			}
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_FOCUSED;
			w->valid = 0;
		}
	}
}
void mouseClickDialog(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (inWidget(w, x, y)) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (x >= w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1) &&
				x < w->pos.x + w->size.x - 2 &&
				y >= w->pos.y + 2 &&
				y < w->pos.y + SG_CHAR_HEIGHT + 2) {
				w->status |= WIDGET_PRESSED;
				w->valid = 0;
			}
			else {
				w->status |= WIDGET_FOCUSED;
				w->valid = 0;
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
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_FOCUSED;
			w->valid = 0;
		}
	}
}
void mouseClickList(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (w->status&WIDGET_SELECTED) {
		if (status == SG_MIDDLE_BUTTON_UP) {
			if (w->value > 0) {
				w->value--;
				w->valid = 0;
			}
		}
		else if (status == SG_MIDDLE_BUTTON_DOWN) {
			if (w->value < w->hide - 1) {
				w->value++;
				w->valid = 0;
			}
		}
		else if (x >= w->pos.x&&x < w->pos.x + w->size.x&&
			y >= w->pos.y + w->size.y - w->hide * SG_LINE_DELTA_DEFAULT&&y < w->pos.y + w->size.y) {
			if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				w->status &= 0xFF ^ WIDGET_SELECTED;
				w->status &= 0xFF ^ WIDGET_PASS;
				w->valid = 0;

				backgroundRefresh(
					w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
				w->size.y -= w->hide*SG_LINE_DELTA_DEFAULT;
			}
		}
		else if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_PASS;
			w->valid = 0;

			backgroundRefresh(
				w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
			w->size.y -= w->hide*SG_LINE_DELTA_DEFAULT;
		}
	}
	else {
		if (inWidget(w, x, y)) {
			if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
			if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
				w->status |= WIDGET_PRESSED;
				w->status |= WIDGET_SELECTED;
				w->size.y += w->hide*SG_LINE_DELTA_DEFAULT;
				w->valid = 0;
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
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_PRESSED;
			w->valid = 0;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			if (w->value)w->value--;
			else w->value = 1;
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->valid = 0;
		}
	}
}
void mouseClickProcess(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (x >= w->pos.x + w->size.x - 2 * (SG_CHAR_WIDTH + 1) &&
				x < w->pos.x + w->size.x - 2 &&
				y >= w->pos.y + 2 &&
				y < w->pos.y + SG_CHAR_HEIGHT + 2) {
				w->status |= WIDGET_PRESSED;
				w->valid = 0;
			}
			else {
				w->status |= WIDGET_FOCUSED;
				w->valid = 0;
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
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_FOCUSED;
			w->valid = 0;
		}
	}
}
void mouseClickOption(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (w->status&WIDGET_SELECTED) {
		if (status == SG_MIDDLE_BUTTON_UP) {
			if (w->value > 0) {
				w->value--;
				w->valid = 0;
			}
		}
		else if (status == SG_MIDDLE_BUTTON_DOWN) {
			if (w->value < w->hide - 1) {
				w->value++;
				w->valid = 0;
			}
		}
		else if (inWidget(w, x, y)) {
			if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
				Widget->active = -1;
				w->status &= 0xFF ^ WIDGET_SELECTED;
				w->status &= 0xFF ^ WIDGET_PASS;
				w->valid = 0;

				backgroundRefresh(
					w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
				w->size.y = 0;
			}
		}
		else  if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
			Widget->active = -1;
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_PASS;
			w->valid = 0;

			backgroundRefresh(
				w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
			w->size.y = 0;
		}
	}
	else {
		w->value = -1;
		if (w->hide > 0 && (w->associate == NULL ||
			(x >= w->associate->pos.x&&x < w->associate->pos.x + w->associate->size.x&&
				y >= w->associate->pos.y&&y < w->associate->pos.y + w->associate->size.y))) {
			if (status & SG_RIGHT_BUTTON)Widget->active = getIndexByName(w->name);
			if (status == (SG_BUTTON_DOWN | SG_RIGHT_BUTTON))w->status |= WIDGET_PRESSED;
			if (status == (SG_BUTTON_UP | SG_RIGHT_BUTTON)) {
				w->status |= WIDGET_SELECTED;
				w->pos.x = x;
				w->pos.y = y;
				w->size.y = w->hide*SG_LINE_DELTA_DEFAULT;
				w->value = 0;
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
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (x >= w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 2 &&
		x < w->pos.x + (float)w->value / 100 * (w->size.x - 12) + 10 &&
		y >= w->pos.y&&y < w->pos.y + w->size.y) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_PRESSED;
			w->valid = 0;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->valid = 0;
		}
	}
}
void mouseClickScrollVert(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (inWidget(w, x, y)) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			double barHeight;
			double moveDelta;
			if (w->hide - 1 <= 0) {
				barHeight = w->size.y;
				moveDelta = 0;
			}
			else {
				barHeight = MINIMAL_BAR + (w->size.y - MINIMAL_BAR * 3) / sqrt(w->hide - 1);
				moveDelta = (w->size.y - barHeight) / (w->hide - 1);
			}

			if (y < w->pos.y + w->value * moveDelta) {
				if (w->value > 0)w->value--;
			}
			else if (y >= w->pos.y + w->value * moveDelta + barHeight) {
				if (w->value < w->hide - 1)w->value++;
			}
			w->status |= WIDGET_PRESSED;
			w->valid = 0;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->valid = 0;
		}
	}
}
void mouseClickScrollHoriz(widgetObj *w, int x, int y, int status) {
	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON) &&
		(w->status & WIDGET_PRESSED)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
		w->valid = 0;
	}
	if (inWidget(w, x, y)) {
		if (status & SG_LEFT_BUTTON)Widget->active = getIndexByName(w->name);
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON)) {
			double barWidth;
			double moveDelta;
			if (w->hide - 1 <= 0) {
				barWidth = w->size.x;
				moveDelta = 0;
			}
			else {
				barWidth = MINIMAL_BAR + (w->size.x - MINIMAL_BAR * 3) / sqrt(w->hide - 1);
				moveDelta = (w->size.x - barWidth) / (w->hide - 1);
			}

			if (x < w->pos.x + w->value * moveDelta) {
				if (w->value > 0)w->value--;
			}
			else if (x >= w->pos.x + w->value * moveDelta + barWidth) {
				if (w->value < w->hide - 1)w->value++;
			}
			w->status |= WIDGET_PRESSED;
			w->valid = 0;
		}
		if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
			w->status |= WIDGET_SELECTED;
			w->valid = 0;
		}
	}
	else {
		if (status & SG_LEFT_BUTTON && Widget->active != -1 &&
			strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
			Widget->active = -1;
		if (status == (SG_BUTTON_DOWN | SG_LEFT_BUTTON) &&
			(w->status & WIDGET_SELECTED)) {
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->valid = 0;
		}
	}
}
void mouseClickCombined(widgetObj *w, int x, int y, int status) {
	widgetObj *S[SG_QSIZE], *tmp;
	int top = 0;

#define PUSH(x) S[top++] = x
#define POP() S[--top]
#define ISEMPTY() (top == 0)

	if (status & SG_BUTTON_UP) {
		tmp = w->child;
		while (tmp) {
			PUSH(tmp);
			tmp = tmp->next;
		}
		while (!ISEMPTY()) {
			tmp = POP();
			tmp->mouseUp(tmp, x, y, status);
			if (inWidget(tmp, x, y))break;
		}
	}
	else {
		tmp = w->child;
		while (tmp) {
			PUSH(tmp);
			tmp = tmp->next;
		}
		while (!ISEMPTY()) {
			tmp = POP();
			tmp->mouseDown(tmp, x, y, status);
			if (inWidget(tmp, x, y))break;
		}
	}

#undef PUSH
#undef POP
#undef ISEMPTY

	if (status == (SG_BUTTON_UP | SG_LEFT_BUTTON)) {
		w->status &= 0xFF ^ WIDGET_PRESSED;
	}
	if (x >= w->pos.x&&x<w->pos.x + w->size.x&&y >= w->pos.y&&y < w->pos.y + w->size.y) {
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
		if (key == (SG_ENTER | 0x8000)) {
			w->associate->mouseUser(w->associate);
		}
	}
}
void keyInput(widgetObj *w, int key) {
	int i, len;

	if (w->associate) {
		if (key == (SG_ENTER | 0x8000)) {
			w->associate->mouseUser(w->associate);
		}
	}

	if (key == SG_LEFT)
		if (w->value)w->value--;
	if (key == SG_RIGHT)
		if (w->value < _strlenW(w->content))
			w->value++;

	len = _strlenW(w->content);

	if (key >= 0x80)return;

	int lenW = _wcharAt(w->content, len);
	if (key == SG_BACKS && w->value != 0) {
		w->value--;
		if (((char *)w->content)[i = _wcharAt(w->content, w->value)] >= 0x80) {
			for (; i < lenW; i++) {
				((char *)w->content)[i] = ((char *)w->content)[i + 2];
			}
		}
		else {
			for (; i < lenW; i++) {
				((char *)w->content)[i] = ((char *)w->content)[i + 1];
			}
		}
	}
	if (key >= 0x20) {
		if (w->value == len) {
			((char *)w->content)[_wcharAt(w->content, w->value) + 1] = '\0';
		}
		else {
			for (i = lenW + 1; i > _wcharAt(w->content, w->value); i--) {
				((char *)w->content)[i] = ((char *)w->content)[i - 1];
			}
		}
		((char *)w->content)[_wcharAt(w->content, w->value++)] = key;
	}

}
void keyList(widgetObj *w, int key) {
	if (key == SG_UP) {
		if (w->value) {
			w->value--;
			w->valid = 0;
		}
	}
	if (key == SG_DOWN) {
		if (w->value < w->hide - 1) {
			w->value++;
			w->valid = 0;
		}
	}
	if (key == SG_ENTER) {
		if (w->status&WIDGET_SELECTED) {
			if (Widget->active != -1 &&
				strcmp(w->name, getWidgetByIndex(Widget->active)->name) == 0)
				Widget->active = -1;
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_PASS;
			w->valid = 0;

			backgroundRefresh(
				w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
			w->size.y -= w->hide*SG_LINE_DELTA_DEFAULT;
		}
	}
}
void keyOption(widgetObj *w, int key) {

	if (key == SG_UP) {
		if (w->value) {
			w->value--;
			w->valid = 0;
		}
	}
	if (key == SG_DOWN) {
		if (w->value < w->hide - 1) {
			w->value++;
			w->valid = 0;
		}
	}
	if (key == SG_ENTER) {
		if (w->status&WIDGET_SELECTED) {
			Widget->active = -1;
			w->status &= 0xFF ^ WIDGET_SELECTED;
			w->status &= 0xFF ^ WIDGET_PASS;
			w->valid = 0;

			backgroundRefresh(
				w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
			w->size.y = 0;
		}
	}
}
void keyScrollVert(widgetObj *w, int key) {

}
void keyScrollHoriz(widgetObj *w, int key) {

}
void keyCombined(widgetObj *w, int key) {
	widgetObj *tmp;

	tmp = w->child;
	while (tmp) {
		if (tmp->status&WIDGET_SELECTED) {
			if (key & 0x8000) {
				tmp->keyUp(tmp, key);
			}
			else {
				tmp->keyDown(tmp, key);
			}
		}
		tmp = tmp->next;
	}
}


/*
* Main drawing.
* Start in v2.1.0
*/
void _drawWidget(int fb) {
	int i;
	widgetObj *current;

	if (_sglMode == BIT_MAP) {
		startSubWindow(-1);
		for (i = 0; i < Widget->count; i++) {
			current = Widget->obj[i];
			if (!current || !current->visible || current->priority != fb)continue;

			if (current->valid)continue;
			current->valid = 1;

			_drawingWidget = i;
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
			_drawingWidget = -1;
		}
		endSubWindow();
	}
}
void _drawSubWidget(int id, int fb) {
	int i;
	widgetObj *current;

	if (_sglMode == BIT_MAP) {
		startSubWindow(id);
		for (i = 0; i < _wndList[currentWindow].widget->count; i++) {
			current = _wndList[currentWindow].widget->obj[i];
			if (!current || !current->visible || current->priority != fb)continue;

			if (current->valid)continue;
			current->valid = 1;

			_wndList[currentWindow].drawingWidget = i;
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
			_wndList[currentWindow].drawingWidget = -1;
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
	w->valid = 0;
	switch (w->style) {
	case SG_DESIGN:
		if (w->status&(WIDGET_PRESSED | WIDGET_SELECTED))
			setColor(w->pressColor.r, w->pressColor.g, w->pressColor.b);
		else if (w->status&WIDGET_PASS)
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
		else setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
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

		SIZE tmp1, tmp2;
		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
		if (w->hide >= _strlenW(w->content)) {
			w->value = _strlenW(w->content);
			w->hide = w->value - 1;
			if (w->hide < 0)w->hide = 0;
		}
		if (w->value <= w->hide && w->value)w->hide = w->value - 1;
		SGWINSTR _wd = NULL;
		GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), w->value, &tmp1);
		free((void *)_wd);
		GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), w->hide, &tmp2);
		free((void *)_wd);
		if (tmp1.cx > tmp2.cx + w->size.x - 2 * SG_CHAR_WIDTH) {
			do {
				w->hide++;
				GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), w->value, &tmp1);
				free((void *)_wd);
				GetTextExtentPoint32(text.memDC, _wd = _widen(w->content), w->hide, &tmp2);
				free((void *)_wd);
			} while (tmp1.cx > tmp2.cx + w->size.x - 2 * SG_CHAR_WIDTH);
		}
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		putStringConstraint(w->content,
			w->pos.x + SG_CHAR_WIDTH,
			w->size.y > 2 * SG_CHAR_HEIGHT ?
			w->pos.y + SG_CHAR_HEIGHT / 2 - 2 :
			w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2 - 2,
			w->hide, w->size.x - 2 * SG_CHAR_WIDTH);
		if ((w->status&WIDGET_FOCUSED) && (clock() % 1000 > 500)) {
			setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
			putLine(w->pos.x + SG_CHAR_WIDTH + tmp1.cx - tmp2.cx,
				w->size.y > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 - 1 :
				w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2 - 1,
				w->pos.x + SG_CHAR_WIDTH + tmp1.cx - tmp2.cx,
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
		setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(223, 223, 223);
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
		setColor(127, 127, 127);
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
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
		while (tmp = putStringConstraint((char *)w->content + total,
			w->pos.x + SG_CHAR_WIDTH,
			w->pos.y + (row + 1) * SG_LINE_DELTA_DEFAULT + 8, 0,
			w->size.x - 2 * SG_CHAR_WIDTH)) {
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
	int start, row, total, tmp;

	switch (w->style) {
	case SG_DESIGN:
		setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(191, 191, 191);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);

		start = 0;
		row = 0;
		total = 0;
		tmp = 0;
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
		while (tmp = putStringConstraint((char *)w->content + total,
			w->pos.x + SG_CHAR_WIDTH,
			row * SG_LINE_DELTA_DEFAULT + (w->size.y > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 :
				w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2), 0,
			w->size.x - 2 * SG_CHAR_WIDTH)) {
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
void _drawList(widgetObj *w) {
	int i;
	SGstring tmp;

	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_SELECTED) {
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - w->hide*SG_LINE_DELTA_DEFAULT - 1, SOLID_FILL);
			setColor(0, 0, 0);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - w->hide*SG_LINE_DELTA_DEFAULT - 1, EMPTY_FILL);
			tmp = w->content;
			for (i = 0; i < w->value; i++) {
				tmp += strlen(tmp) + 1;
			}

			setFontSize(20);
			setFontName("Î¢ÈíÑÅºÚ");
			i = w->size.y - w->hide*SG_LINE_DELTA_DEFAULT > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 :
				w->pos.y + (w->size.y - w->hide*SG_LINE_DELTA_DEFAULT) / 2 - SG_CHAR_HEIGHT / 2;
			setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
			putStringConstraint(tmp,
				w->pos.x + SG_CHAR_WIDTH, i - 2, 0,
				w->size.x - 2 * SG_CHAR_WIDTH);
			setColor(0, 0, 0);
			putTriangle(w->pos.x + w->size.x - 12, i - 2 + SG_CHAR_HEIGHT / 2,
				w->pos.x + w->size.x - 6, i - 2 + SG_CHAR_HEIGHT / 2,
				w->pos.x + w->size.x - 9, i + 2 + SG_CHAR_HEIGHT / 2,
				SOLID_FILL);

			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
			putQuad(w->pos.x, w->pos.y + w->size.y - w->hide * SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
			setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
			putQuad(w->pos.x, w->pos.y + w->size.y - (w->hide - w->value)*SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - (w->hide - w->value - 1)*SG_LINE_DELTA_DEFAULT - 1,
				SOLID_FILL);
			setColor(127, 127, 127);
			putQuad(w->pos.x, w->pos.y + w->size.y - w->hide*SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
			tmp = w->content;
			setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
			for (i = 0; i < w->hide; i++) {
				putStringConstraint(tmp, w->pos.x + SG_CHAR_WIDTH,
					w->pos.y + w->size.y - (w->hide - i)*SG_LINE_DELTA_DEFAULT, 0,
					w->size.x - 2 * SG_CHAR_WIDTH);
				tmp += strlen(tmp) + 1;
			}
		}
		else {
			if (w->status&WIDGET_PASS)
				setColor(w->passColor.r, w->passColor.g, w->passColor.b);
			else setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
			setColor(127, 127, 127);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, EMPTY_FILL);
			tmp = w->content;
			for (i = 0; i < w->value; i++) {
				tmp += strlen(tmp) + 1;
			}

			setFontSize(20);
			setFontName("Î¢ÈíÑÅºÚ");
			i = w->size.y > 2 * SG_CHAR_HEIGHT ?
				w->pos.y + SG_CHAR_HEIGHT / 2 :
				w->pos.y + w->size.y / 2 - SG_CHAR_HEIGHT / 2;
			setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
			putStringConstraint(tmp,
				w->pos.x + SG_CHAR_WIDTH, i - 2, 0,
				w->size.x - 2 * SG_CHAR_WIDTH);
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
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
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
void _drawPic(widgetObj *w) {
	putBitmap(w->pos.x, w->pos.y, *((bitMap *)w->content));
}
void _drawCheck(widgetObj *w) {
	switch (w->style) {
	case SG_DESIGN:
		if (w->status&WIDGET_PRESSED)
			setColor(w->pressColor.r, w->pressColor.g, w->pressColor.b);
		else if (w->status&WIDGET_PASS)
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
		else setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 6, SOLID_FILL);
		setColor(63, 63, 63);
		putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 6, EMPTY_FILL);
		if (w->status&WIDGET_SELECTED) {
			setColor(127, 127, 127);
			putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 5, EMPTY_FILL);
		}
		if (w->value) {
			setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
			putCircle(w->pos.x + SG_CHAR_WIDTH, w->pos.y + SG_CHAR_HEIGHT / 2, 2, SOLID_FILL);
		}
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
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

	w->valid = 0;
	switch (w->style) {
	case SG_DESIGN:
		setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x, w->pos.y,
			w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(223, 223, 223);
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
		setColor(127, 127, 127);
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
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		setFontSize(20);
		setFontName("Î¢ÈíÑÅºÚ");
		while (tmp = putStringConstraint((char *)w->content + total,
			w->pos.x + SG_CHAR_WIDTH,
			w->pos.y + (row + 1) * SG_LINE_DELTA_DEFAULT + 8, 0,
			w->size.x - 2 * SG_CHAR_WIDTH)) {
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
		setColor(63, 63, 63);
		putQuad(w->pos.x + 2 * SG_CHAR_WIDTH,
			w->pos.y + w->size.y - SG_CHAR_HEIGHT - 8,
			w->pos.x + 2 * SG_CHAR_WIDTH + (int)(w->value*1.f / 100 * (w->size.x - 4 * SG_CHAR_WIDTH)),
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
			setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
			putQuad(w->pos.x, w->pos.y,
				w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);

			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
			putQuad(w->pos.x, w->pos.y + w->value * SG_LINE_DELTA_DEFAULT,
				w->pos.x + w->size.x - 1, w->pos.y + (w->value + 1)*SG_LINE_DELTA_DEFAULT - 1,
				SOLID_FILL);
			tmp = w->content;
			setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
			setFontSize(20);
			setFontName("Î¢ÈíÑÅºÚ");
			for (i = 0; i < w->hide; i++) {
				putStringConstraint(tmp, w->pos.x + SG_CHAR_WIDTH,
					w->pos.y + w->size.y - (w->hide - i)*SG_LINE_DELTA_DEFAULT, 0,
					w->size.x - 2 * SG_CHAR_WIDTH);
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
	RGB inverse = getPixel(w->pos.x, w->pos.y);
	inverse.r = 255 - inverse.r;
	inverse.g = 255 - inverse.g;
	inverse.b = 255 - inverse.b;
	backgroundRefresh(
		w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
	switch (w->style) {
	case SG_DESIGN:
		setColor(inverse.r, inverse.g, inverse.b);
		putQuad(w->pos.x, w->pos.y + w->size.y / 2 - 2,
			w->pos.x + w->size.x, w->pos.y + w->size.y / 2 + 2, EMPTY_FILL);
		if (w->status&WIDGET_PRESSED)
			setColor(w->pressColor.r, w->pressColor.g, w->pressColor.b);
		else if (w->status&WIDGET_PASS)
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
		else setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x + w->value* (w->size.x - 12) / 100 + 2, w->pos.y,
			w->pos.x + w->value * (w->size.x - 12) / 100 + 10, w->pos.y + w->size.y - 1, SOLID_FILL);
		setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
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
void _drawScrollVert(widgetObj *w) {
	double barHeight;
	double moveDelta;
	if (w->hide - 1 <= 0) {
		barHeight = w->size.y;
		moveDelta = 0;
	}
	else {
		barHeight = MINIMAL_BAR + (w->size.y - MINIMAL_BAR * 3) / sqrt(w->hide - 1);
		moveDelta = (w->size.y - barHeight) / (w->hide - 1);
	}
	switch (w->style) {
	case SG_DESIGN:
		setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x, w->pos.y, w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);

		if (w->status&WIDGET_PRESSED)
			setColor(w->pressColor.r, w->pressColor.g, w->pressColor.b);
		else if (w->status&WIDGET_PASS)
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
		else setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		putQuad(w->pos.x + 2, w->pos.y + (int)(w->value * moveDelta + 2),
			w->pos.x + w->size.x - 3, w->pos.y + (int)(w->value * moveDelta + barHeight - 3), SOLID_FILL);
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
void _drawScrollHoriz(widgetObj *w) {
	double barWidth;
	double moveDelta;
	if (w->hide - 1 <= 0) {
		barWidth = w->size.x;
		moveDelta = 0;
	}
	else {
		barWidth = MINIMAL_BAR + (w->size.x - MINIMAL_BAR * 3) / sqrt(w->hide - 1);
		moveDelta = (w->size.x - barWidth) / (w->hide - 1);
	}
	switch (w->style) {
	case SG_DESIGN:
		setColor(w->bgColor.r, w->bgColor.g, w->bgColor.b);
		putQuad(w->pos.x, w->pos.y, w->pos.x + w->size.x - 1, w->pos.y + w->size.y - 1, SOLID_FILL);

		if (w->status&WIDGET_PRESSED)
			setColor(w->pressColor.r, w->pressColor.g, w->pressColor.b);
		else if (w->status&WIDGET_PASS)
			setColor(w->passColor.r, w->passColor.g, w->passColor.b);
		else setColor(w->fgColor.r, w->fgColor.g, w->fgColor.b);
		putQuad(w->pos.x + (int)(w->value * moveDelta + 2), w->pos.y + 2,
			w->pos.x + (int)(w->value * moveDelta + barWidth - 3), w->pos.y + w->size.y - 3, SOLID_FILL);
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
void _drawCombined(widgetObj *w) {
	widgetObj *current = w->child;
	w->valid = 0;
	while (current) {
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

		current = current->next;
	}
}

