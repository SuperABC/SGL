#define _CRT_SECURE_NO_WARNINGS

#include <shlwapi.h>
#include <vector>
#include "winsgl.h"
#include "inner.h"
#include "util.h"
#include "window.h"
#include "users.h"


using std::vector;
using std::string;


/*
* SG window management
* These variables and functions are used to control the window(s).
*/

vector<Window *> _windowList;
Window *_baseWindow;
int _currentWindow = 0;
long _windowSem = 1;
bitMap *canvasCache = NULL;

void _sgInit() {
	sgSetup();

	if (_windowList.size() == 0) {
		Window *base = new Window();
		_windowList.push_back(base);
	}
	_baseWindow = _windowList[0];
}
void _sgSubInit(vect setup) {
	setup();
}
void _sgSubInit(void(*setup)(void *), void *init) {
	setup(init);
}

void _startSubWindow(int id) {
	SEM_P();
	_currentWindow = id;
	canvasCache = _windowList[_currentWindow]->getCanvas();
}
void _endSubWindow() {
	SEM_V();
}

void initWindow(int width, int height, SGtext title, int mode) {
	if (mode != BIT_MAP && mode != TEXT_MAP)exit(SG_INVALID_MODE);

	if(_windowList.empty())
		_windowList.push_back(new Window(width, height, title, mode));
}
void initPolarWindow(int rx, int ry) {
	if (rx == 0) {
		rx = GetSystemMetrics(SM_CXFULLSCREEN)/2;
	}
	if (ry == 0) {
		ry = GetSystemMetrics(SM_CYFULLSCREEN)/2;
	}
	if (_windowList.empty()) {
		_windowList.push_back(new Window(rx * 2, ry * 2));
		_windowList.back()->sglCircle = 1;
		_windowList.back()->sglCircleCx = rx;
		_windowList.back()->sglCircleCy = ry;
		_windowList.back()->sglCircleRx = rx;
		_windowList.back()->sglCircleRy = ry;
		strcpy(_windowList.back()->winName, "");
	}
}
void maximizeWindow() {
	ShowWindow(_windowList[_currentWindow]->getHwnd(), SW_SHOWMAXIMIZED);
}
void minimizeWindow() {
	ShowWindow(_windowList[_currentWindow]->getHwnd(), SW_SHOWMINIMIZED);
}
int createWindow(int width, int height, SGtext title, int mode, vect setup, vect loop) {
	WNDCLASSEX wc;

	if (!_baseWindow)return SG_OBJECT_NOT_FOUND;
	Window *sub = new Window(width, height, title, mode);

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SubWndProc;
	wc.hInstance = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	String className = String((string("SubClass") + std::to_string(_windowList.size())).data());
#ifdef UNICODE
	wc.lpszClassName = className.widen();
#else
	wc.lpszClassName = className.shorten();
#endif
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		
		delete sub;
		return SG_IO_ERROR;
	}

	sub->loop = loop;
	sub->createWindow(NULL, wc, _baseWindow->getHwnd());

	ShowWindow(sub->getHwnd(), SW_SHOWDEFAULT);

	/*for (unsigned int i = 0; i < _windowList.size(); i++) {
		if (_windowList[i] == NULL) {
			_windowList[i] = sub;
			return i;
		}
	}*/
	_windowList.push_back(sub);

	int tmp = _currentWindow;
	_endSubWindow();
	_startSubWindow(_windowList.size() - 1);
	_sgSubInit(setup);
	_endSubWindow();
	_startSubWindow(tmp);
	return _windowList.size() - 1;
}
int createWindow(int width, int height, SGtext title, int mode, void(*setup)(void *), vect loop, void *init) {
	WNDCLASSEX wc;

	if (!_baseWindow)return SG_OBJECT_NOT_FOUND;
	Window *sub = new Window(width, height, title, mode);

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SubWndProc;
	wc.hInstance = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	String className = String((string("SubClass") + std::to_string(_windowList.size())).data());
#ifdef UNICODE
	wc.lpszClassName = className.widen();
#else
	wc.lpszClassName = className.shorten();
#endif
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);

		delete sub;
		return SG_IO_ERROR;
	}

	sub->loop = loop;
	sub->createWindow(NULL, wc, _baseWindow->getHwnd());

	ShowWindow(sub->getHwnd(), SW_SHOWDEFAULT);

	/*for (unsigned int i = 0; i < _windowList.size(); i++) {
	if (_windowList[i] == NULL) {
	_windowList[i] = sub;
	return i;
	}
	}*/
	_windowList.push_back(sub);

	int tmp = _currentWindow;
	_endSubWindow();
	_startSubWindow(_windowList.size() - 1);
	_sgSubInit(setup, init);
	_endSubWindow();
	_startSubWindow(tmp);
	return _windowList.size() - 1;
}
int createPolarWindow(int rx, int ry, vect setup, vect loop) {
	if (rx == 0) {
		rx = GetSystemMetrics(SM_CXFULLSCREEN) / 2;
	}
	if (ry == 0) {
		ry = GetSystemMetrics(SM_CYFULLSCREEN) / 2;
	}

	WNDCLASSEX wc;

	if (!_baseWindow)return SG_OBJECT_NOT_FOUND;
	Window *sub = new Window(rx * 2, ry * 2);
	sub->sglCircle = 1;
	sub->sglCircleCx = rx;
	sub->sglCircleCy = ry;
	sub->sglCircleRx = rx;
	sub->sglCircleRy = ry;
	strcpy(sub->winName, "");

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = SubWndProc;
	wc.hInstance = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	String className = String((string("SubClass") + std::to_string(_windowList.size())).data());
#ifdef UNICODE
	wc.lpszClassName = className.widen();
#else
	wc.lpszClassName = className.shorten();
#endif
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);

		delete sub;
		return SG_IO_ERROR;
	}

	sub->loop = loop;
	sub->createWindow(NULL, wc, _baseWindow->getHwnd());
	if (sub->sglCircle) {
		SetWindowRgn(sub->getHwnd(),
			CreateEllipticRgn(sub->sglCircleCx - sub->sglCircleRx,
				sub->sglCircleCy - sub->sglCircleRy,
				sub->sglCircleCx + sub->sglCircleRx,
				sub->sglCircleCy + sub->sglCircleRy), TRUE);
		//hideCaption();
	}

	ShowWindow(sub->getHwnd(), SW_SHOWDEFAULT);

	/*for (unsigned int i = 0; i < _windowList.size(); i++) {
	if (_windowList[i] == NULL) {
	_windowList[i] = sub;
	return i;
	}
	}*/
	_windowList.push_back(sub);

	int tmp = _currentWindow;
	_endSubWindow();
	_startSubWindow(_windowList.size() - 1);
	_sgSubInit(setup);
	_endSubWindow();
	_startSubWindow(tmp);
	return _windowList.size() - 1;
}
void windowFinish(vect finish) {
	_windowList[_currentWindow]->finish = finish;
}
void closeWindow(int id) {
	if (id >= (int)_windowList.size())return;
	if (id < 0)id = _currentWindow;

	PostMessage(_windowList[id]->getHwnd(), WM_CLOSE, 0, 0);
	if (_windowList[id]->finish)_windowList[id]->finish();
	delete _windowList[id];
	_windowList[id] = NULL;
}
void resizeFuntion(void(*func)(int x, int y)) {
	if (_windowList[_currentWindow]->resize == NULL) {
		_windowList[_currentWindow]->setResize();
	}
	if(func)_windowList[_currentWindow]->resize = func;
	else _windowList[_currentWindow]->resize = [](int x, int y) {};
}
void renameCaption(SGtext name) {
	SetWindowText(_windowList[_currentWindow]->getHwnd(), _widen(name));
}
void hideCaption() {
	LONG lStyle = GetWindowLong(_windowList[_currentWindow]->getHwnd(), GWL_STYLE);
	SetWindowLong(_windowList[_currentWindow]->getHwnd(), GWL_STYLE, lStyle & ~WS_CAPTION);
}
void showCaption() {
	LONG lStyle = GetWindowLong(_windowList[_currentWindow]->getHwnd(), GWL_STYLE);
	SetWindowLong(_windowList[_currentWindow]->getHwnd(), GWL_STYLE, lStyle | WS_CAPTION);
}
void setIcon(SGtext ico) {
	_windowList[_currentWindow]->setIcon(LoadImage(NULL, _widen(ico), IMAGE_ICON, 0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE));
}
bool safeAdd(SGuint a, SGuint b, SGuint *c) {
	*c = a + b;
	return *c < a;
}
bool safeMul(SGuint a, SGuint b, SGuint *c) {
	*c = a * b;
	return (a != 0 && *c / a != b);
}


/*
* Remained OpenGL IO functions.
* Directly send IO message to system.
*/

int _utf16GBK(int in_utf16) {
	int out_gbk;
	
	WCHAR in_buffer[2] = { 0 };
	unsigned char out_buffer[3] = { 0 };
	((char *)in_buffer)[1] = in_utf16 / 0x100;
	((char *)in_buffer)[0] = in_utf16 % 0x100;

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, setlocale(LC_ALL, ""));

	wcstombs((char *)out_buffer, in_buffer, 2);
	out_gbk = out_buffer[0] * 0x100 + out_buffer[1];

	setlocale(LC_CTYPE, old_locale);
	return out_gbk;
}

void sgKeyDown(int cAscii, int x, int y, int utf16) {
	if (utf16) {
		int gbk = _utf16GBK(cAscii);
		if (gbk == 0)return;

		_windowList[_currentWindow]->key->enqueue((gbk / 0x100) & 0x7fff);
		_windowList[_currentWindow]->key->enqueue((gbk % 0x100) & 0x7fff);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->keyPress(gbk, true);
		}
	}
	else {
		/*int ctrl, shift;
		ctrl = cAscii & SG_CTRLBIT;
		shift = cAscii & SG_SHIFTBIT;*/
		cAscii &= 0xff;
		if (cAscii >= 0x80 || cAscii < 0x20)return;

		_windowList[_currentWindow]->key->enqueue(cAscii & 0x7fff);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->keyPress(cAscii & 0x7fff);
		}
	}
}
void sgSpecialDown(int cAscii, int x, int y) {
	switch (cAscii) {
	case VK_UP:
		cAscii = SG_UP;
		break;
	case VK_DOWN:
		cAscii = SG_DOWN;
		break;
	case VK_LEFT:
		cAscii = SG_LEFT;
		break;
	case VK_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case 0x11:
		cAscii = SG_CTRL;
		break;
	case 0x10:
		cAscii = SG_SHIFT;
		break;
	case SG_ESC:
	case SG_BACKS:
	case SG_TAB:
	case SG_ENTER:
		break;
	case VK_INSERT:
		cAscii = SG_INSERT;
		break;
	case VK_DELETE:
		cAscii = SG_DELETE;
		break;
	case VK_HOME:
		cAscii = SG_HOME;
		break;
	case VK_END:
		cAscii = SG_END;
		break;
	case VK_NUMLOCK:
		cAscii = SG_NUMLOC;
		break;
	case VK_CAPITAL:
		cAscii = SG_CAPSLOC;
		break;
	case VK_PRIOR:
		cAscii = SG_PAGEUP;
		break;
	case VK_NEXT:
		cAscii = SG_PAGEDOWN;
		break;
	case VK_F1:
		cAscii = SG_F1;
		break;
	case VK_F2:
		cAscii = SG_F2;
		break;
	case VK_F3:
		cAscii = SG_F3;
		break;
	case VK_F4:
		cAscii = SG_F4;
		break;
	case VK_F5:
		cAscii = SG_F5;
		break;
	case VK_F6:
		cAscii = SG_F6;
		break;
	case VK_F7:
		cAscii = SG_F7;
		break;
	case VK_F8:
		cAscii = SG_F8;
		break;
	case VK_F9:
		cAscii = SG_F9;
		break;
	case VK_F10:
		cAscii = SG_F10;
		break;
	case VK_F11:
		cAscii = SG_F11;
		break;
	case VK_F12:
		cAscii = SG_F12;
		break;
	default:
		return;
	}

	_windowList[_currentWindow]->key->enqueue(cAscii & 0x7fff);

	for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
		if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
			_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
		_windowList[_currentWindow]->widgets[i]->keyPress(cAscii & 0x7fff);
	}
}
void sgKeyUp(int cAscii, int x, int y) {
	cAscii &= 0xff;
	if (cAscii >= 0x80 || cAscii < 0x20)return;

	_windowList[_currentWindow]->key->enqueue(cAscii | 0x8000);

	for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
		if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
			_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
		_windowList[_currentWindow]->widgets[i]->keyPress(cAscii | 0x8000);
	}
}
void sgSpecialUp(int cAscii, int x, int y) {
	switch (cAscii) {
	case VK_UP:
		cAscii = SG_UP;
		break;
	case VK_DOWN:
		cAscii = SG_DOWN;
		break;
	case VK_LEFT:
		cAscii = SG_LEFT;
		break;
	case VK_RIGHT:
		cAscii = SG_RIGHT;
		break;
	case 0x11:
		cAscii = SG_CTRL;
		break;
	case 0x10:
		cAscii = SG_SHIFT;
		break;
	case SG_ESC:
	case SG_BACKS:
	case SG_TAB:
	case SG_ENTER:
		break;
	case VK_INSERT:
		cAscii = SG_INSERT;
		break;
	case VK_DELETE:
		cAscii = SG_DELETE;
		break;
	case VK_HOME:
		cAscii = SG_HOME;
		break;
	case VK_END:
		cAscii = SG_END;
		break;
	case VK_NUMLOCK:
		cAscii = SG_NUMLOC;
		break;
	case VK_CAPITAL:
		cAscii = SG_CAPSLOC;
		break;
	case VK_PRIOR:
		cAscii = SG_PAGEUP;
		break;
	case VK_NEXT:
		cAscii = SG_PAGEDOWN;
		break;
	case VK_F1:
		cAscii = SG_F1;
		break;
	case VK_F2:
		cAscii = SG_F2;
		break;
	case VK_F3:
		cAscii = SG_F3;
		break;
	case VK_F4:
		cAscii = SG_F4;
		break;
	case VK_F5:
		cAscii = SG_F5;
		break;
	case VK_F6:
		cAscii = SG_F6;
		break;
	case VK_F7:
		cAscii = SG_F7;
		break;
	case VK_F8:
		cAscii = SG_F8;
		break;
	case VK_F9:
		cAscii = SG_F9;
		break;
	case VK_F10:
		cAscii = SG_F10;
		break;
	case VK_F11:
		cAscii = SG_F11;
		break;
	case VK_F12:
		cAscii = SG_F12;
		break;
	default:
		return;
	}

	_windowList[_currentWindow]->key->enqueue(cAscii | 0x8000);

	for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
		if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
			_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
		_windowList[_currentWindow]->widgets[i]->keyPress(cAscii | 0x8000);
	}
}
void sgMouse(int x, int y) {
	if (_windowList[_currentWindow]->resize) {
		_windowList[_currentWindow]->mouse->pos.x = x;
		_windowList[_currentWindow]->mouse->pos.y = y;
	}
	else {
		_windowList[_currentWindow]->mouse->pos.x =
			x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
		_windowList[_currentWindow]->mouse->pos.y =
			y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
	}

	for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
		if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
			_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
		_windowList[_currentWindow]->widgets[i]->mouseMove(x, y);
	}
}
void sgDrag(int x, int y) {
	if (_windowList[_currentWindow]->resize) {
		_windowList[_currentWindow]->mouse->pos.x = x;
		_windowList[_currentWindow]->mouse->pos.y = y;
	}
	else {
		_windowList[_currentWindow]->mouse->pos.x =
			x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
		_windowList[_currentWindow]->mouse->pos.y =
			y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
	}

	for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
		if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
			_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
		_windowList[_currentWindow]->widgets[i]->mouseMove(x, y);
	}
}
void sgClick(int button, int state, int x, int y) {
	vec3i m;
	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_DOWN) {
		_windowList[_currentWindow]->mouse->left = SG_BUTTON_DOWN;
		if (_windowList[_currentWindow]->resize) {
			m.x = x;
			m.y = y;
		}
		else {
			m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
			m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
		}
		m.z = SG_BUTTON_DOWN | SG_LEFT_BUTTON;
		_windowList[_currentWindow]->mouse->enqueue(m);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, SG_BUTTON_DOWN | SG_LEFT_BUTTON);
		}
	}
	if (button == SG_LEFT_BUTTON && state == SG_BUTTON_UP) {
		_windowList[_currentWindow]->mouse->left = SG_BUTTON_UP;
		if (_windowList[_currentWindow]->resize) {
			m.x = x;
			m.y = y;
		}
		else {
			m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
			m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
		}
		m.z = SG_BUTTON_UP | SG_LEFT_BUTTON;
		_windowList[_currentWindow]->mouse->enqueue(m);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, SG_BUTTON_UP | SG_LEFT_BUTTON);
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_DOWN) {
		_windowList[_currentWindow]->mouse->right = SG_BUTTON_DOWN;
		_windowList[_currentWindow]->panel->x = x;
		_windowList[_currentWindow]->panel->y = y;
		if (_windowList[_currentWindow]->resize) {
			m.x = x;
			m.y = y;
		}
		else {
			m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
			m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
		}
		m.z = SG_BUTTON_DOWN | SG_RIGHT_BUTTON;
		_windowList[_currentWindow]->mouse->enqueue(m);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, SG_BUTTON_DOWN | SG_RIGHT_BUTTON);
		}
	}
	if (button == SG_RIGHT_BUTTON && state == SG_BUTTON_UP) {
		_windowList[_currentWindow]->mouse->right = SG_BUTTON_UP;
		if (_windowList[_currentWindow]->resize) {
			m.x = x;
			m.y = y;
		}
		else {
			m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
			m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
		}
		m.z = SG_BUTTON_UP | SG_RIGHT_BUTTON;
		_windowList[_currentWindow]->mouse->enqueue(m);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, SG_BUTTON_UP | SG_RIGHT_BUTTON);
		}
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_DOWN) {
		_windowList[_currentWindow]->mouse->middle = SG_BUTTON_DOWN;
		if (_windowList[_currentWindow]->resize) {
			m.x = x;
			m.y = y;
		}
		else {
			m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
			m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
		}
		m.z = SG_BUTTON_DOWN | SG_MIDDLE_BUTTON;
		_windowList[_currentWindow]->mouse->enqueue(m);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, SG_BUTTON_DOWN | SG_MIDDLE_BUTTON);
		}
	}
	if (button == SG_MIDDLE_BUTTON && state == SG_BUTTON_UP) {
		_windowList[_currentWindow]->mouse->middle = SG_BUTTON_UP;
		if (_windowList[_currentWindow]->resize) {
			m.x = x;
			m.y = y;
		}
		else {
			m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
			m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
		}
		m.z = SG_BUTTON_UP | SG_MIDDLE_BUTTON;
		_windowList[_currentWindow]->mouse->enqueue(m);

		for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
			if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
				_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
			_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, SG_BUTTON_UP | SG_MIDDLE_BUTTON);
		}
	}
}
void sgWheel(int dir) {
	int x = _windowList[_currentWindow]->mouse->pos.x, y = _windowList[_currentWindow]->mouse->pos.y;

	vec3i m;
	if (_windowList[_currentWindow]->resize) {
		m.x = x;
		m.y = y;
	}
	else {
		m.x = x * _windowList[_currentWindow]->getBufferSize().x / _windowList[_currentWindow]->getWindowSize().x;
		m.y = y * _windowList[_currentWindow]->getBufferSize().y / _windowList[_currentWindow]->getWindowSize().y;
	}
	m.z = dir > 0 ? SG_MIDDLE_BUTTON_UP : SG_MIDDLE_BUTTON_DOWN;
	_windowList[_currentWindow]->mouse->enqueue(m);

	for (unsigned int i = 0; i < _windowList[_currentWindow]->widgets.size(); i++) {
		if (!crossWidget(_windowList[_currentWindow]->widgets[i]->obj, 0, 0,
			_windowList[_currentWindow]->getWindowSize().x, _windowList[_currentWindow]->getWindowSize().y))continue;
		_windowList[_currentWindow]->widgets[i]->mouseClick(x, y, m.z);
	}
}
void sgIdle(HWND hwnd) {

}


/*
* SG IO interfaces
* Basic IO functions including keyboard and mouse are to make
* the program executive.
*/

int biosKey(int cmd) {
	word ret;

	if (cmd == 1)return !_windowList[_currentWindow]->key->empty();
	if (cmd != 0)return 0;

	ret = _windowList[_currentWindow]->key->dequeue();

	return ret;
}
void clearKeyBuffer() {
	_windowList[_currentWindow]->key->clear();
}
int mouseStatus(int b) {
	switch (b) {
	case SG_LEFT_BUTTON:
		return _windowList[_currentWindow]->mouse->left;
	case SG_RIGHT_BUTTON:
		return _windowList[_currentWindow]->mouse->right;
	case SG_MIDDLE_BUTTON:
		return _windowList[_currentWindow]->mouse->middle;
	default:
		return 0;
	}
}
vec2i mousePos() {
	vec2i ret;

	ret.x = _windowList[_currentWindow]->mouse->pos.x;
	ret.y = _windowList[_currentWindow]->mouse->pos.y;
	return ret;
}
vec3i biosMouse(int cmd) {
	vec3i ret;

	if (cmd == 1) {
		ret.z = !_windowList[_currentWindow]->mouse->empty();
		return ret;
	}
	if (cmd != 0) {
		ret.z = 0;
		return ret;
	}

	ret = _windowList[_currentWindow]->mouse->dequeue();

	return ret;
}
void clearMouseBuffer() {
	_windowList[_currentWindow]->mouse->clear();
}
void showMouse() {
	while (ShowCursor(TRUE) < 0)
		ShowCursor(TRUE);
}
void hideMouse() {
	while (ShowCursor(FALSE) >= 0)
		ShowCursor(FALSE);
}
void setMousePos(int x, int y) {
	LPPOINT point;

	point = (LPPOINT)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;

	ClientToScreen(_windowList[_currentWindow]->getHwnd(), point);
	SetCursorPos(point->x, point->y);
	free(point);
}
void setMouseIcon(SGWINSTR icon) {
	HCURSOR cur = LoadCursor(NULL, icon);
	SetCursor(cur);
}


/*
* SG tool interfaces
* These functions can be used not only in graphic modes but also in
* console mode.
*/

int _delayBegin, _delayEnd, _delayInter = 0;
struct _mci mci;
vector<HANDLE> threads;

int _mciSend(const char *cmd) {
	return !mciSendString(_widen(cmd), mci.buffer, SG_MCI_BUFFER_SIZE, 0);
}

int getWidth(int obj) {
	switch (obj) {
	case SG_WINDOW:
		return _windowList[_currentWindow]->getWindowSize().x;
	case SG_CANVAS:
		return _windowList[_currentWindow]->getBufferSize().x;
	}
	return 0;
}
int getHeight(int obj) {
	switch (obj) {
	case SG_WINDOW:
		return _windowList[_currentWindow]->getWindowSize().y;
	case SG_CANVAS:
		return _windowList[_currentWindow]->getBufferSize().y;
	}
	return 0;
}
vec2i getSize(int obj) {
	switch (obj) {
	case SG_WINDOW:
		return _windowList[_currentWindow]->getWindowSize();
	case SG_CANVAS:
		return _windowList[_currentWindow]->getBufferSize();
	}
	return Vec2i(0, 0);
}
void debugf(SGtext format, ...) {
	va_list ap;
	string buffer;
	va_start(ap, format);
	while (*format) {
		if (*format == '%') {
			format++;
			switch (*format) {
			case 'c': {
				char valch = va_arg(ap, int);
				buffer += valch;
				format++;
				break;
			}
			case 'd': {
				int valint = va_arg(ap, int);
				buffer += std::to_string(valint);
				format++;
				break;
			}
			case 's': {
				char *valstr = va_arg(ap, char *);
				buffer += valstr;
				format++;
				break;
			}
			case 'f': {
				float valfloat = va_arg(ap, double);
				buffer += std::to_string(valfloat);
				format++;
				break;
			}
			case '%': {
				buffer += *format++;
			}
			}
		}
		else {
			buffer += *format++;
		}
	}

	OutputDebugString(_widen(buffer.data()));
	va_end(ap);
}
void delay(int t) {
	Sleep(t);
}
void delayBegin() {
	_delayBegin = clock();
	_delayInter = 1;
}
int delayEnd(int t) {
	if (!_delayInter)return 0;

	_delayEnd = clock();
	if (t > _delayEnd - _delayBegin)
		Sleep(t - _delayEnd + _delayBegin);

	_delayInter = 0;
	return 1;
}
int random(int n) {
	return rand() % n;
}
int selectFile(char name[], SGWINSTR start, SGWINSTR format, int idx) {
	OPENFILENAME ofn = { 0 };
	SGWINSTR strFilename = (SGWINSTR)malloc(MAX_PATH);
	memset(strFilename, 0, MAX_PATH);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = format;
	ofn.nFilterIndex = idx;
	ofn.lpstrFile = strFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = start;
	ofn.lpstrTitle = TEXT("请选择一个文件");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	if (strFilename[0]) {
		strcpy(name, _shorten(strFilename));
		free(strFilename);
		return SG_NO_ERROR;
	}
	else {
		free(strFilename);
		return SG_OBJECT_NOT_FOUND;
	}
}
int selectSave(char name[], SGWINSTR start, SGWINSTR format, SGWINSTR def, int idx) {
	OPENFILENAME ofn = { 0 };
	SGWINSTR strFilename = (SGWINSTR)malloc(MAX_PATH);
	memset(strFilename, 0, MAX_PATH);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = format;
	ofn.nFilterIndex = idx;
	ofn.lpstrFile = strFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = start;
	ofn.lpstrTitle = TEXT("请选择一个文件");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("保存到");
	ofn.lpstrDefExt = def;
	GetSaveFileName(&ofn);
	if (strFilename[0]) {
		strcpy(name, _shorten(strFilename));
		free(strFilename);
		return SG_NO_ERROR;
	}
	else {
		free(strFilename);
		return SG_OBJECT_NOT_FOUND;
	}
}
int makePath(SGtext path) {
	if (!PathFileExists(_widen(path))) {
		_wmkdir(String(path).widen());
	}
	return SG_NO_ERROR;
}
int fileExist(SGtext path) {
	return PathFileExists(_widen(path));
}
int selectDir(char name[], char start[]) {
	SGWINSTR szBuffer = (SGWINSTR)malloc(MAX_PATH);
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = TEXT("选择一个文件夹");
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(idl, szBuffer);
	if (szBuffer[0]) {
		strcpy(name, _shorten(szBuffer));
		free(szBuffer);
		return SG_NO_ERROR;
	}
	else {
		free(szBuffer);
		return SG_OBJECT_NOT_FOUND;
	}
}
void initMci() {
	mci.buffer = (SGWINSTR)malloc(sizeof(SG_MCI_BUFFER_SIZE));
	mci.num = 0;
}
int loadMciSrc(SGtext filename) {
	unsigned int i;
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(mci.num, randstr, 10);

	strcpy(cmd, "open ");
	strcat(cmd, filename);
	for (i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == '/')cmd[i] = '\\';
	}
	strcat(cmd, " alias ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	strcpy(cmd, "set ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " time format milliseconds");
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	strcpy(cmd, "status ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " length");
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	mci.length[mci.num] = atoi(_shorten(mci.buffer));
	free(cmd);
	return mci.num++;
}
int playMci(int id) {
	char startTime[16], endTime[16];
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(0, startTime, 10);
	_itoa(mci.length[id], endTime, 10);
	_itoa(id, randstr, 10);
	strcpy(cmd, "play ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " from ");
	strcat(cmd, startTime);
	strcat(cmd, " to ");
	strcat(cmd, endTime);
	if (_mciSend(cmd)) {
		free(cmd);
		return SG_NO_ERROR;
	}
	free(cmd);
	return SG_MEDIA_ERROR;
}
int stopMci(int id) {
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(id, randstr, 10);
	strcpy(cmd, "stop ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	strcpy(cmd, "seek ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	strcat(cmd, " to start");
	if (!_mciSend(cmd)) {
		free(cmd);
		return SG_MEDIA_ERROR;
	}
	free(cmd);
	return SG_NO_ERROR;
}
int pauseMci(int id) {
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(id, randstr, 10);
	strcpy(cmd, "pause ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (_mciSend(cmd)) {
		free(cmd);
		return SG_NO_ERROR;
	}
	free(cmd);
	return SG_MEDIA_ERROR;
}
int resumeMci(int id) {
	char randstr[6];
	char *cmd = (char *)malloc(256);

	_itoa(id, randstr, 10);
	strcpy(cmd, "resume ");
	strcat(cmd, "mp3_");
	strcat(cmd, randstr);
	if (_mciSend(cmd)) {
		free(cmd);
		return SG_NO_ERROR;
	}
	free(cmd);
	return SG_MEDIA_ERROR;
}
void createThread(thread func, void *param) {
	threads.push_back(CreateThread(NULL, 0, func, param, 0, NULL));
}
int copyText(SGtext src) {
	HGLOBAL hg;
	char *pt;

	hg = GlobalAlloc(GHND | GMEM_SHARE,
		(strlen(src) + 1) * sizeof(TCHAR));
	pt = (char *)GlobalLock(hg);
	strcpy(pt, src);
	GlobalUnlock(hg);

	OpenClipboard(_windowList[_currentWindow]->getHwnd());
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();

	return SG_NO_ERROR;
}
char *pasteText() {
	HGLOBAL hg;
	char *pt, *ret;

	OpenClipboard(_windowList[_currentWindow]->getHwnd());

	if (hg = GetClipboardData(CF_TEXT)) {
		pt = (char *)GlobalLock(hg);

		ret = (char *)malloc(GlobalSize(hg));
		strcpy(ret, pt);
	}
	else return NULL;
	CloseClipboard();
	return ret;
}
SOCKET createServer(int port) {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return 0;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	listen(sockSrv, SG_MAX_CONNECTION);
	return sockSrv;
}
SOCKET createClient(SGtext server, int port) {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return 0;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(server);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	return sockClient;
}
SOCKET acceptOne(SOCKET server) {
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	return accept(server, (SOCKADDR*)&addrClient, &len);
}
int socketSend(SOCKET s, SGtext buffer) {
	if (send(s, buffer, (int)strlen(buffer) + 1, 0) == SOCKET_ERROR)return SG_CONNECTION_FAILED;
	return SG_NO_ERROR;
}
int socketReceive(SOCKET s, SGstring buffer, int len) {
	if (recv(s, buffer, len, 0) == SOCKET_ERROR)return SG_CONNECTION_FAILED;
	return SG_NO_ERROR;
}
void closeSocket(SOCKET s) {
	closesocket(s);
}
void alertInfo(SGtext caption, SGtext text, int mode, void(*result)(int)) {
	int line = 1;
	int idx = 0;

	while (text[idx++]) {
		if (text[idx] == '\n') {
			line++;
		}
	}

	createWindow(400, 20 * line + 80, caption, BIT_MAP, [](void *param) {
		registerWidget(easyWidget(SG_LABEL, "__alert_text", 20, 20,
			400, 20 * ((std::pair<string, int>*)param)->second + 80,
			((std::pair<string, int>*)param)->first.data(), NULL));
	}, []() {}, new std::pair<string, int>(text, line));
}


/*
* SG drawing interfaces
* Funtions below are used to draw something on the screen.
* The image after drawing will be shown after the loop function.
*/

int _stringPrintf(SGtext format, va_list ap, char *buffer) {
	string buf;
	while (*format) {
		if (*format == '\t') {
			buf +=  "    ";
			format++;
		}
		else if (*format == '%') {
			format++;
			switch (*format) {
			case 'c': {
				char valch = va_arg(ap, int);
				buf += valch;
				format++;
				break;
			}
			case 'd': {
				int valint = va_arg(ap, int);
				buf += std::to_string(valint);
				format++;
				break;
			}
			case 's': {
				char *valstr = va_arg(ap, char *);
				buf += valstr;
				format++;
				break;
			}
			case 'f': {
				float valfloat = va_arg(ap, double);
				buf += std::to_string(valfloat);
				format++;
				break;
			}
			case '%': {
				buf += *format++;
			}
			}
		}
		else {
			buf += *format++;
		}
	}

	strcpy(buffer, buf.data());
	return SG_NO_ERROR;
}
int _stringLength(SGtext format, va_list ap) {
	int len = 0;
	while (*format) {
		if (*format == '\t') {
			len += 4;
			format++;
		}
		else if (*format == '%') {
			format++;
			if (*format >= '0' && *format <= '9' || *format == '.' || *format == '-') {
				if (*format == '-')format++;
				len += std::to_string(atoi(format)).length();
				bool pointed = false;
				while (*format >= '0' && *format <= '9' || *format == '.') {
					if (pointed&&*format == '.')break;
					else if (*format == '.')pointed = true;
					format++;
				}
			}
			switch (*format) {
			case 'c': {
				len++;
				format++;
				break;
			}
			case 'd': {
				int valint = va_arg(ap, int);
				len += (int)log10(valint) + 1;
				format++;
				break;
			}
			case 'o': {
				int valint = va_arg(ap, int);
				len += (int)(log(valint) / log(8)) + 1;
				format++;
				break;
			}
			case 'x': {
				int valint = va_arg(ap, int);
				len += (int)(log(valint) / log(16)) + 1;
				format++;
				break;
			}
			case 's': {
				char *valstr = va_arg(ap, char *);
				len += strlen(valstr);
				format++;
				break;
			}
			case 'f': {
				float valfloat = va_arg(ap, double);
				len += 10;
				format++;
				break;
			}
			case '%': {
				len++;
				format++;
			}
			}
		}
		else {
			len++;
			format++;
		}
	}

	return len;
}

void setColor(int r, int g, int b) {
	_windowList[_currentWindow]->setColor(r, g, b);
}
void setFontSize(int height) {
	_windowList[_currentWindow]->setFontSize(height);
}
void setFontName(SGtext name) {
	_windowList[_currentWindow]->setFontName(name);
}
void setFontStyle(int coeff) {
	_windowList[_currentWindow]->setFontStyle(coeff);
}
void setAlpha(float alpha) {
	_windowList[_currentWindow]->setAlpha(alpha);
}
float getAlpha() {
	return _windowList[_currentWindow]->getAlpha();
}
void clearScreen() {
	_windowList[_currentWindow]->clearScreen();
}
int putPixel(int x, int y) {
	return _windowList[_currentWindow]->putPixel(x, y);
}
rgb getPixel(int x, int y) {
	return _windowList[_currentWindow]->getPixel(x, y);
}
void putLine(int x1, int y1, int x2, int y2, int mode) {
	_windowList[_currentWindow]->putLine(x1, y1, x2, y2, mode);
}
int putQuad(int x1, int y1, int x2, int y2, int mode) {
	return _windowList[_currentWindow]->putQuad(x1, y1, x2, y2, mode);
}
void putTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int mode) {
	return _windowList[_currentWindow]->putTriangle(x1, y1, x2, y2, x3, y3, mode);
}
void putCircle(int xc, int yc, int r, int mode) {
	return _windowList[_currentWindow]->putCircle(xc, yc, r, mode);
}
void putEllipse(int xc, int yc, int a, int b, int mode) {
	return _windowList[_currentWindow]->putEllipse(xc, yc, a, b, mode);
}
void putBitmap(int x, int y, bitMap bmp) {
	return _windowList[_currentWindow]->putBitmap(x, y, bmp);
}
void putChar(char ch, int x, int y) {
	return _windowList[_currentWindow]->putChar(ch, x, y);
}
void putNumber(int n, int x, int y, char lr) {
	return _windowList[_currentWindow]->putNumber(n, x, y, lr);
}
int putString(SGtext str, int x, int y) {
	return _windowList[_currentWindow]->putString(str, x, y);
}
int putStringConstraint(SGtext str, int x, int y, int start, int constraint) {
	return _windowList[_currentWindow]->putStringConstraint(str, x, y, start, constraint);
}
int stringWidth(SGtext str, int x) {
	return _windowList[_currentWindow]->stringWidth(str, x);
}
char *stringFormat(SGtext str, ...) {
	int cnt = 0;
	va_list ap;
	char *result;

	va_start(ap, str);
	result = (char *)malloc(_stringLength(str, ap));
	cnt = _stringPrintf(str, ap, result);
	va_end(ap);
	return result;
}
int getImage(int left, int top, int right, int bottom, bitMap *bitmap) {
	return _windowList[_currentWindow]->getImage(left, top, right, bottom, bitmap);
}
void putImage(int left, int top, bitMap *bitmap, int op) {
	return _windowList[_currentWindow]->putImage(left, top, bitmap, op);
}
int maskImage(int left, int top, bitMap *mask, bitMap *bitmap) {
	return _windowList[_currentWindow]->maskImage(left, top, mask, bitmap);
}
void funcMap(int x1, int x2, int y1, int y2, float(*vect)(float x)) {
	return _windowList[_currentWindow]->funcMap(x1, x2, y1, y2, vect);
}
void floodFill(int x, int y, rgb c) {
	return _windowList[_currentWindow]->floodFill(x, y, c);
}
void setBfc(int bgc, int fgc) {
	return _windowList[_currentWindow]->setBfc(bgc, fgc);
}
void clearText() {
	return _windowList[_currentWindow]->clearText();
}
void setCharColor(short color, int x, int y) {
	return _windowList[_currentWindow]->setCharColor(color, x, y);
}
void setCharBgc(char color, int x, int y) {
	return _windowList[_currentWindow]->setCharBgc(color, x, y);
}
void setCharFgc(char color, int x, int y) {
	return _windowList[_currentWindow]->setCharFgc(color, x, y);
}
int getShort(int x, int y) {
	return _windowList[_currentWindow]->getShort(x, y);
}
void writeChar(char c, int x, int y) {
	return _windowList[_currentWindow]->writeChar(c, x, y);
}
void writeString(SGtext s, int x, int y) {
	return _windowList[_currentWindow]->writeString(s, x, y);
}
int getText(int left, int top, int right, int bottom, textMap *text) {
	return _windowList[_currentWindow]->getText(left, top, right, bottom , text);
}
void putText(int left, int top, textMap *text) {
	return _windowList[_currentWindow]->putText(left, top, text);
}
int showFps() {
	return _windowList[_currentWindow]->showFps();
}


/*
* SG menu interfaces
* SGL provide several methods to create and edit the menu.
*/

void enablePanel() {
	if (_windowList[_currentWindow]->panel->en)return;
	_windowList[_currentWindow]->panel->en = 1;
	_windowList[_currentWindow]->panel->cover = (bitMap *)malloc(sizeof(bitMap));
	_windowList[_currentWindow]->panel->cover->data = NULL;
}
void disablePanel() {
	if (!_windowList[_currentWindow]->panel->en)return;
	_windowList[_currentWindow]->panel->en = 0;
	if (_windowList[_currentWindow]->panel->cover->data)free(_windowList[_currentWindow]->panel->cover->data);
	free(_windowList[_currentWindow]->panel->cover);
}
int addPanelItem(SGtext name, vect function, int shift, int ctrl) {
	int i = 0;
	Panel *panel = _windowList[_currentWindow]->panel;
	if (shift == 0 && ctrl == 0) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel->normalPanel[i] == NULL) {
				panel->normalPanel[i] = (struct Function *)malloc(sizeof(struct Function));
				panel->normalPanel[i]->id = panel->maxId++;
				panel->normalPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel->normalPanel[i]->name, name);
				panel->normalPanel[i]->function = function;
				return panel->normalPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	if (shift == 1 && ctrl == 0) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel->shiftPanel[i] == NULL) {
				panel->shiftPanel[i] = (struct Function *)malloc(sizeof(struct Function));
				panel->shiftPanel[i]->id = panel->maxId++;
				panel->shiftPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel->shiftPanel[i]->name, name);
				panel->shiftPanel[i]->function = function;
				return panel->shiftPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	if (shift == 0 && ctrl == 1) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel->ctrlPanel[i] == NULL) {
				panel->ctrlPanel[i] = (struct Function *)malloc(sizeof(struct Function));
				panel->ctrlPanel[i]->id = panel->maxId++;
				panel->ctrlPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel->ctrlPanel[i]->name, name);
				panel->ctrlPanel[i]->function = function;
				return panel->ctrlPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	if (shift == 1 && ctrl == 1) {
		for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
			if (panel->shiftctrlPanel[i] == NULL) {
				panel->shiftctrlPanel[i] = (struct Function *)malloc(sizeof(struct Function));
				panel->shiftctrlPanel[i]->id = panel->maxId++;
				panel->shiftctrlPanel[i]->name = (char *)malloc(strlen(name) + 1);
				strcpy(panel->shiftctrlPanel[i]->name, name);
				panel->shiftctrlPanel[i]->function = function;
				return panel->shiftctrlPanel[i]->id;
			}
		}
		if (i == SG_MAX_PANEL_FUNCTION)return SG_OUT_OF_RANGE;
	}
	return SG_OUT_OF_RANGE;
}
int changePanelItem(int id, SGtext name, vect function) {
	int i;
	Panel *panel = _windowList[_currentWindow]->panel;
	for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
		if (panel->normalPanel[i]->id == id) {
			free(panel->normalPanel[i]->name);
			panel->normalPanel[i]->name = (char *)malloc(strlen(name) + 1);
			strcpy(panel->normalPanel[i]->name, name);
			panel->normalPanel[i]->function = function;
			return panel->normalPanel[i]->id;
		}
		if (panel->shiftPanel[i]->id == id) {
			free(panel->shiftPanel[i]->name);
			panel->shiftPanel[i]->name = (char *)malloc(strlen(name) + 1);
			strcpy(panel->shiftPanel[i]->name, name);
			panel->shiftPanel[i]->function = function;
			return panel->shiftPanel[i]->id;
		}
		if (panel->ctrlPanel[i]->id == id) {
			free(panel->ctrlPanel[i]->name);
			panel->ctrlPanel[i]->name = (char *)malloc(strlen(name) + 1);
			strcpy(panel->ctrlPanel[i]->name, name);
			panel->ctrlPanel[i]->function = function;
			return panel->ctrlPanel[i]->id;
		}
		if (panel->shiftctrlPanel[i]->id == id) {
			free(panel->shiftctrlPanel[i]->name);
			panel->shiftctrlPanel[i]->name = (char *)malloc(strlen(name) + 1);
			strcpy(panel->shiftctrlPanel[i]->name, name);
			panel->shiftctrlPanel[i]->function = function;
			return panel->shiftctrlPanel[i]->id;
		}
	}
	return SG_OBJECT_NOT_FOUND;
}
int deletePanelItem(int id) {
	int i;
	Panel *panel = _windowList[_currentWindow]->panel;
	for (i = 0; i < SG_MAX_PANEL_FUNCTION; i++) {
		if (panel->normalPanel[i]->id == id) {
			free(panel->normalPanel[i]);
			panel->normalPanel[i] = NULL;
			return SG_NO_ERROR;
		}
		if (panel->shiftPanel[i]->id == id) {
			free(panel->shiftPanel[i]);
			panel->shiftPanel[i] = NULL;
			return SG_NO_ERROR;
		}
		if (panel->ctrlPanel[i]->id == id) {
			free(panel->ctrlPanel[i]);
			panel->ctrlPanel[i] = NULL;
			return SG_NO_ERROR;
		}
		if (panel->shiftctrlPanel[i]->id == id) {
			free(panel->shiftctrlPanel[i]);
			panel->shiftctrlPanel[i] = NULL;
			return SG_NO_ERROR;
		}
	}
	return SG_OBJECT_NOT_FOUND;
}


/*
* SG widget interfaces.
* SGL provide several types of widgets to use.
*/

void _clickDefault(widget *obj) {

}
void _moveDefault(widget *obj, int x, int y) {

}
void _dragDefault(widget *obj, int x, int y) {

}
void _pressDefault(widget *obj, int k) {

}
Widget *_getSub(Widget *root, SGtext name) {
	Widget *iter = root->child;
	while (iter) {
		if (iter->type != SG_COMBINED) {
			if (iter->name != name) {
				iter = iter->next;
				continue;
			}
			else return iter;
		}
		else {
			Widget *tmp;
			if (iter->name == name) {
				return iter;
			}
			else if (tmp = _getSub(iter, name)) {
				return tmp;
			}
			else {
				iter = iter->next;
				continue;
			}
		}
	}
	return NULL;
}
Widget *_getByName(SGtext name, bool refresh = false) {
	string path(name);

	Widget *parent = NULL;
	while (true) {
		int sp1 = path.find('/');
		int sp2 = path.find('\\');
		if (sp1 < 0 && sp2 < 0) {
			if (parent == NULL) {
				for (auto wnd : _windowList) {
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path)return w;
					}
				}
				return NULL;
			}
			else {
				if (refresh)parent->valid = false;
				return _getSub(parent, path.data());
			}
		}
		else if (sp1 < 0) {
			if (parent == NULL) {
				int found = 0;
				for (auto wnd : _windowList) {
					if (found)break;
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path.substr(0, sp2)) {
							parent = w;
							found = 1;
							break;
						}
					}
				}
			}
			else {
				if (refresh)parent->valid = false;
				parent = _getSub(parent, path.substr(0, sp2).data());
			}
			path = path.substr(sp2 + 1);
		}
		else if (sp2 < 0) {
			if (parent == NULL) {
				int found = 0;
				for (auto wnd : _windowList) {
					if (found)break;
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path.substr(0, sp1)) {
							parent = w;
							found = 1;
							break;
						}
					}
				}
			}
			else {
				if (refresh)parent->valid = false;
				parent = _getSub(parent, path.substr(0, sp1).data());
			}
			path = path.substr(sp1 + 1);
		}
		else {
			int sp = min(sp1, sp2);
			if (parent == NULL) {
				int found = 0;
				for (auto wnd : _windowList) {
					if (found)break;
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path.substr(0, sp)) {
							parent = w;
							found = 1;
							break;
						}
					}
				}
			}
			else {
				if (refresh)parent->valid = false;
				parent = _getSub(parent, path.substr(0, sp).data());
			}
			path = path.substr(sp + 1);
		}
	}
	return NULL;
}
int _getIndex(const char *name) {
	string path(name);
	int sp1 = path.find('/');
	int sp2 = path.find('\\');
	if (sp1 < 0 && sp2 < 0) {
		for (auto wnd : _windowList) {
			if (!wnd)continue;
			for (unsigned int i = 0; i < wnd->widgets.size(); i++) {
				if (wnd->widgets[i]->name == path)return i;
			}
		}
	}
	else if (sp1 < 0) {
		for (auto wnd : _windowList) {
			if (!wnd)continue;
			for (unsigned int i = 0; i < wnd->widgets.size(); i++) {
				if (wnd->widgets[i]->name == path.substr(0, sp2))return i;
			}
		}
	}
	else if (sp2 < 0) {
		for (auto wnd : _windowList) {
			if (!wnd)continue;
			for (unsigned int i = 0; i < wnd->widgets.size(); i++) {
				if (wnd->widgets[i]->name == path.substr(0, sp1))return i;
			}
		}
	}
	else {
		int sp = min(sp1, sp2);
		for (auto wnd : _windowList) {
			if (!wnd)continue;
			for (unsigned int i = 0; i < wnd->widgets.size(); i++) {
				if (wnd->widgets[i]->name == path.substr(0, sp))return i;
			}
		}
	}
	return SG_OBJECT_NOT_FOUND;
}
void _deleteSub(Widget *root, SGtext name) {
	Widget *iter = root->child;
	Widget *prev = iter;
	if (iter->name == name) {
		iter->cease();
		if (_windowList[iter->window]->backgroundRefresh)
			_windowList[iter->window]->backgroundRefresh(
				iter->pos.x, iter->pos.y, iter->pos.x + iter->size.x, iter->pos.y + iter->size.y);
		root->child = iter->next;
		delete iter;
	}
	else {
		iter = iter->next;
		while (iter) {
			if (iter->name == name) {
				iter->cease();
				if (_windowList[iter->window]->backgroundRefresh)
					_windowList[iter->window]->backgroundRefresh(
						iter->pos.x, iter->pos.y, iter->pos.x + iter->size.x, iter->pos.y + iter->size.y);
				prev->next = iter->next;
				delete iter;
				break;
			}
			iter = iter->next;
			prev = prev->next;
		}
	}
}
void _deleteByName(SGtext name) {
	string path(name);

	Widget *parent = NULL;
	while (true) {
		int sp1 = path.find('/');
		int sp2 = path.find('\\');
		if (sp1 < 0 && sp2 < 0) {
			if (parent == NULL) {
				for (auto wnd : _windowList) {
					if (!wnd)continue;
					for (auto &w : wnd->widgets) {
						if (w->name == path) {
							w->cease();
							if (_windowList[w->window]->backgroundRefresh)
								_windowList[w->window]->backgroundRefresh(
									w->pos.x, w->pos.y, w->pos.x + w->size.x, w->pos.y + w->size.y);
							delete w;
							w = wnd->widgets.back();
							wnd->widgets.pop_back();
							return;
						}
					}
				}
				break;
			}
			else {
				return _deleteSub(parent, path.data());
			}
		}
		else if (sp1 < 0) {
			if (parent == NULL) {
				int found = 0;
				for (auto wnd : _windowList) {
					if (found)break;
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path.substr(0, sp2)) {
							parent = w;
							found = 1;
							break;
						}
					}
				}
				if (!found)break;
			}
			else {
				parent = _getSub(parent, path.substr(0, sp2).data());
			}
			path = path.substr(sp2 + 1);
		}
		else if (sp2 < 0) {
			if (parent == NULL) {
				int found = 0;
				for (auto wnd : _windowList) {
					if (found)break;
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path.substr(0, sp1)) {
							parent = w;
							found = 1;
							break;
						}
					}
				}
				if (!found)break;
			}
			else {
				parent = _getSub(parent, path.substr(0, sp1).data());
			}
			path = path.substr(sp1 + 1);
		}
		else {
			int sp = min(sp1, sp2);
			if (parent == NULL) {
				int found = 0;
				for (auto wnd : _windowList) {
					if (found)break;
					if (!wnd)continue;
					for (auto w : wnd->widgets) {
						if (w->name == path.substr(0, sp)) {
							parent = w;
							found = 1;
							break;
						}
					}
				}
				if (!found)break;
			}
			else {
				parent = _getSub(parent, path.substr(0, sp).data());
			}
			path = path.substr(sp + 1);
		}
	}
}
void _moveSub(Widget *root, int x, int y) {
	Widget *iter = root->child;
	while (iter) {
		iter->obj->pos.x = iter->pos.x = iter->pos.x + x;
		iter->obj->pos.y = iter->pos.y = iter->pos.y + y;
		iter->valid = false;
		if (iter->type == SG_COMBINED) {
			_moveSub(iter, x, y);
		}
		iter = iter->next;
	}
}

void setBackgroundRefresh(void(*refresh)(int left, int top, int right, int bottom)) {
	_windowList[_currentWindow]->setBgRefresh(refresh);
}
void useBackgroundRefresh(int left, int top, int right, int bottom) {
	_windowList[_currentWindow]->useBgRefresh(left, top, right, bottom);
}
widget newWidget(enum _control type, SGtext name) {
	widget ret;
	ret.type = type;

	ret.pos.x = ret.pos.y = 0;
	ret.size.x = ret.size.y = 0;

	ret.style = SG_DESIGN;
	ret.value = 0;
	ret.extra = 0;

	ret.name = (SGstring)malloc(strlen(name)+1);
	strcpy(ret.name, name);
	ret.tf.color.r = 0;
	ret.tf.color.g = 0;
	ret.tf.color.b = 0;
	ret.tf.name = (SGWINSTR)malloc((_strlen(_widen("微软雅黑")) + 1) * sizeof(wchar_t));
	_strcpy(ret.tf.name, _widen("微软雅黑"));
	ret.tf.size = 20;
	ret.tf.coeff = 0;
	ret.content = NULL;

	ret.next = NULL;
	ret.child = NULL;

	ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 223;
	ret.passColor.r = ret.passColor.g = ret.passColor.b = 191;
	ret.pressColor.r = ret.pressColor.g = ret.pressColor.b = 159;
	ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
	ret.bgImg.sizeX = ret.bgImg.sizeY = 0;
	ret.bgImg.data = NULL;

	ret.click = _clickDefault;
	ret.move = _moveDefault;
	ret.drag = _dragDefault;
	ret.press = _pressDefault;

	switch (type) {
	case SG_BUTTON:
		break;
	case SG_INPUT:
		ret.passColor.r = ret.passColor.g = ret.passColor.b = 255;
		ret.pressColor.r = ret.pressColor.g = ret.pressColor.b = 255;
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 239;
		break;
	case SG_DIALOG:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 191;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
		break;
	case SG_OUTPUT:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 255;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
		break;
	case SG_LIST:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 223;
		ret.passColor.r = ret.passColor.g = ret.passColor.b = 255;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
		break;
	case SG_LABEL:
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
		break;
	case SG_PIC:
		break;
	case SG_CHECK:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 239;
		ret.passColor.r = ret.passColor.g = ret.passColor.b = 223;
		ret.pressColor.r = ret.pressColor.g = ret.pressColor.b = 191;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
		break;
	case SG_PROCESS:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 191;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 0;
		break;
	case SG_DRAG:
		break;
	case SG_SCROLLVERT:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 95;
		ret.passColor.r = ret.passColor.g = ret.passColor.b = 223;
		ret.pressColor.r = ret.pressColor.g = ret.pressColor.b = 255;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 191;
		if (getWidth(SG_CANVAS) < SCROLL_WIDTH) {
			ret.pos.x = 0;
			ret.size.x = getWidth(SG_CANVAS);
		}
		else {
			ret.pos.x = getWidth(SG_CANVAS) - SCROLL_WIDTH;
			ret.size.x = SCROLL_WIDTH;
		}
		ret.pos.y = 0;
		ret.size.y = getHeight(SG_CANVAS);

		ret.extra = 100;
		break;
	case SG_SCROLLHORIZ:
		ret.bgColor.r = ret.bgColor.g = ret.bgColor.b = 95;
		ret.passColor.r = ret.passColor.g = ret.passColor.b = 223;
		ret.pressColor.r = ret.pressColor.g = ret.pressColor.b = 255;
		ret.fgColor.r = ret.fgColor.g = ret.fgColor.b = 191;
		if (getHeight(SG_CANVAS) < SCROLL_WIDTH) {
			ret.pos.y = 0;
			ret.size.y = getHeight(SG_CANVAS);
		}
		else {
			ret.pos.y = getHeight(SG_CANVAS) - SCROLL_WIDTH;
			ret.size.y = SCROLL_WIDTH;
		}
		ret.pos.x = 0;
		ret.size.x = getWidth(SG_CANVAS);

		ret.extra = 100;
		break;
	case SG_COMBINED:
		break;
	}

	return ret;
}
void registerWidget(widget obj) {
	switch (obj.type) {
	case SG_BUTTON:
		_windowList[_currentWindow]->widgets.push_back(new Button(obj, _currentWindow));
		break;
	case SG_INPUT:
		_windowList[_currentWindow]->widgets.push_back(new Input(obj, _currentWindow));
		break;
	case SG_DIALOG:
		_windowList[_currentWindow]->widgets.push_back(new Dialog(obj, _currentWindow));
		break;
	case SG_OUTPUT:
		_windowList[_currentWindow]->widgets.push_back(new Output(obj, _currentWindow));
		break;
	case SG_LIST:
		_windowList[_currentWindow]->widgets.push_back(new List(obj, _currentWindow));
		break;
	case SG_LABEL:
		_windowList[_currentWindow]->widgets.push_back(new Label(obj, _currentWindow));
		break;
	case SG_PIC:
		_windowList[_currentWindow]->widgets.push_back(new Pic(obj, _currentWindow));
		break;
	case SG_CHECK:
		_windowList[_currentWindow]->widgets.push_back(new Check(obj, _currentWindow));
		break;
	case SG_DRAG:
		_windowList[_currentWindow]->widgets.push_back(new Drag(obj, _currentWindow));
		break;
	case SG_PROCESS:
		_windowList[_currentWindow]->widgets.push_back(new Process(obj, _currentWindow));
		break;
	case SG_SCROLLVERT:
		_windowList[_currentWindow]->widgets.push_back(new ScrollVert(obj, _currentWindow));
		break;
	case SG_SCROLLHORIZ:
		_windowList[_currentWindow]->widgets.push_back(new ScrollHoriz(obj, _currentWindow));
		break;

	case SG_COMBINED:
		_windowList[_currentWindow]->widgets.push_back(new CombinedWidget(obj, _currentWindow));
		break;
	}
}
widget easyWidget(int type, SGtext name, int x, int y, int width, int height, SGtext content, void(*click)(widget *obj)) {
	widget tmp = newWidget((enum _control)type, name);
	tmp.pos.x = x;
	tmp.pos.y = y;
	tmp.size.x = width;
	tmp.size.y = height;

	tmp.content = (char *)malloc(strlen((char *)content) < SG_MIN_WIDGET_CONTENT ?
		SG_MIN_WIDGET_CONTENT : (strlen((char *)content) + 1));
	strcpy((char *)tmp.content, content);
	if (click)tmp.click = click;

	return tmp;
}
widget easyCombinedWidget(SGtext name, int x, int y, int width, int height, int num, ...) {
	widget parent = newWidget(SG_COMBINED, name);
	parent.pos.x = x;
	parent.pos.y = y;
	parent.size.x = width;
	parent.size.y = height;

	widget *iter;
	va_list ap;

	va_start(ap, num);
	iter = &parent;
	if (num > 0) {
		iter->child = new widget(va_arg(ap, widget));
		iter = iter->child;
		while (--num) {
			iter->next = new widget(va_arg(ap, widget));
			iter = iter->next;
		}
	}
	else {
		iter->child = NULL;
	}
	va_end(ap);

	return parent;
}
widget *getWidgetByName(SGtext name) {
	Widget *tmp = _getByName(name);
	if (tmp)return tmp->obj;
	else return NULL;
}
SGvoid refreshWidget(SGtext name) {
	Widget *tmp = _getByName(name, true);
	tmp->valid = false;

	tmp->style = tmp->obj->style;
	tmp->content = tmp->obj->content;
	tmp->value = tmp->obj->value;
	tmp->extra = tmp->obj->extra;
	tmp->bgImg = tmp->obj->bgImg;

	tmp->tf.color = tmp->obj->tf.color;
	tmp->tf.size = tmp->obj->tf.size;
	tmp->tf.coeff = tmp->obj->tf.coeff;
	tmp->bgColor = tmp->obj->bgColor;
	tmp->fgColor = tmp->obj->fgColor;
	tmp->passColor = tmp->obj->passColor;
	tmp->pressColor = tmp->obj->pressColor;

	tmp->click = tmp->obj->click;
	tmp->move = tmp->obj->move;
	tmp->drag = tmp->obj->drag;
	tmp->press = tmp->obj->press;

	tmp->update();
}
int inWidget(widget *obj, int x, int y) {
	if (obj == NULL)return 0;
	return (x >= obj->pos.x&&x < obj->pos.x + obj->size.x) &&
		(y >= obj->pos.y&&y < obj->pos.y + obj->size.y);
}
int crossWidget(widget *obj, int left, int top, int right, int bottom) {
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
int showWidget(SGtext name) {
	Widget *tmp = _getByName(name);
	if (tmp) {
		tmp->show();
		return SG_NO_ERROR;
	}
	else return SG_OBJECT_NOT_FOUND;
}
int ceaseWidget(SGtext name) {
	Widget *tmp = _getByName(name);
	if (tmp) {
		tmp->cease();
		if (_windowList[tmp->window]->backgroundRefresh)
			_windowList[tmp->window]->backgroundRefresh(
				tmp->pos.x, tmp->pos.y, tmp->pos.x + tmp->size.x, tmp->pos.y + tmp->size.y);
		return SG_NO_ERROR;
	}
	else return SG_OBJECT_NOT_FOUND;
}
int deleteWidget(SGtext name) {
	_deleteByName(name);
	return SG_NO_ERROR;
}
widget *getSubWidget(SGtext parent, SGtext sub) {
	Widget *root = _getByName(parent);
	Widget *tmp = _getSub(root, sub);
	if (tmp)return tmp->obj;
	else return NULL;
}
void insertSubWidget(SGtext parent, widget sub, int index) {
	Widget *root = _getByName(parent);
	if (root->size.x < sub.pos.x + sub.size.x)root->size.x = root->obj->size.x = sub.pos.x + sub.size.x;
	if (root->size.y < sub.pos.y + sub.size.y)root->size.y = root->obj->size.y = sub.pos.y + sub.size.y;
	Widget *child = NULL;
	switch (sub.type) {
	case SG_BUTTON:
		child = new Button(sub, _currentWindow);
		break;
	case SG_INPUT:
		child = new Input(sub, _currentWindow);
		break;
	case SG_DIALOG:
		child = new Dialog(sub, _currentWindow);
		break;
	case SG_OUTPUT:
		child = new Output(sub, _currentWindow);
		break;
	case SG_LIST:
		child = new List(sub, _currentWindow);
		break;
	case SG_LABEL:
		child = new Label(sub, _currentWindow);
		break;
	case SG_PIC:
		child = new Pic(sub, _currentWindow);
		break;
	case SG_CHECK:
		child = new Check(sub, _currentWindow);
		break;
	case SG_PROCESS:
		child = new Process(sub, _currentWindow);
		break;
	case SG_DRAG:
		child = new Drag(sub, _currentWindow);
		break;
	case SG_SCROLLVERT:
		child = new ScrollVert(sub, _currentWindow);
		break;
	case SG_SCROLLHORIZ:
		child = new ScrollHoriz(sub, _currentWindow);
		break;
	case SG_COMBINED:
		child = new CombinedWidget(sub, _currentWindow);
		break;
	default:
		break;
	}
	if (index > 0) {
		Widget *iter = root->child;
		while (iter) {
			index--;
			if (index == 0) {
				child->next = iter->next;
				iter->next = child;
				return;
			}
			if(iter->next)
				iter = iter->next;
			else {
				iter->next = child;
				return;
			}
		}
	}
	child->next = root->child;
	root->child = child;
}
int moveWidget(SGtext name, int xDelta, int yDelta) {
	Widget *tmp = _getByName(name);
	if (tmp) {
		tmp->cease();
		if (_windowList[tmp->window]->backgroundRefresh) {
			int prev = _windowList[tmp->window]->drawingWidget;
			_windowList[tmp->window]->drawingWidget = _getIndex(name);
			_windowList[tmp->window]->backgroundRefresh(
				tmp->pos.x, tmp->pos.y, tmp->pos.x + tmp->size.x, tmp->pos.y + tmp->size.y);
			_windowList[tmp->window]->drawingWidget = prev;
		}

		tmp->obj->pos.x = tmp->pos.x = tmp->pos.x + xDelta;
		tmp->obj->pos.y = tmp->pos.y = tmp->pos.y + yDelta;
		tmp->valid = false;
		if (tmp->type == SG_COMBINED) {
			_moveSub(tmp, xDelta, yDelta);
		}
		tmp->show();
		return SG_NO_ERROR;
	}
	else return SG_OBJECT_NOT_FOUND;
}
void setWidgetTop(SGtext name) {
	_windowList[_currentWindow]->setWidgetTop(name);
}
void setWidgetBottom(SGtext name) {
	_windowList[_currentWindow]->setWidgetBottom(name);
}
void setSubWidgetTop(SGtext parent, SGtext name) {

}
void setSubWidgetBottom(SGtext parent, SGtext name) {

}
void widgetCover(int window, int id, int left, int top, int right, int bottom) {
	_windowList[window]->widgetCover(id, left, top, right, bottom);
}

/*
* Windows menus
* Manage main menu, tray menu and popup menu.
*/

struct _menu _popupMenuInfo[SG_MAX_POPUP_NUM];
int _popupNum = 0, _tmpItem = 0;

int _addList(SGtext name, struct _menu *super, int id);
int _addItem(SGtext name, struct _menu *super, int id, void(*func)());
int _addSeparator(struct _menu *super, int id);
void _checkItem(struct _menu *m, int id, int check, int disable);
void _createMenu(enum _menuType type, struct _item *it, HMENU super);
void _callMenu(struct _menu *m, int id);
int _addList(SGtext name, struct _menu *super, int id) {
	int i, j, ret;

	if (super == NULL)return SG_OBJECT_NOT_FOUND;
	if (super->id == id) {
		i = 0;
		while (super->sub[i++]);
		i--;
		super->sub[i] = (struct _item *)malloc(sizeof(struct _item));
		super->sub[i]->name = (char *)malloc(strlen(name) + 1);
		strcpy(super->sub[i]->name, name);
		super->sub[i]->id = _tmpItem;
		super->sub[i]->sub = (struct _menu *)malloc(sizeof(struct _menu));
		super->sub[i]->sub->name = (char *)malloc(strlen(name) + 1);
		strcpy(super->sub[i]->sub->name, name);
		super->sub[i]->sub->id = _tmpItem;
		super->sub[i]->checked = 0;
		super->sub[i]->func = NULL;
		for (j = 0; j < SG_MAX_MENU_ITEM_NUM; j++) {
			super->sub[i]->sub->sub[j] = NULL;
		}
		return _tmpItem++;
	}
	else {
		for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
			if (super->sub[i] == NULL)return SG_OBJECT_NOT_FOUND;
			if (super->sub[i] == (struct _item *) - 1)continue;
			if ((ret = _addList(name, super->sub[i]->sub, id)) !=
				SG_OBJECT_NOT_FOUND)return ret;
		}
		return SG_OBJECT_NOT_FOUND;
	}
}
int _addItem(SGtext name, struct _menu *super, int id, void(*func)()) {
	int i, ret;

	if (super == NULL)return SG_OBJECT_NOT_FOUND;
	if (super->id == id) {
		i = 0;
		while (super->sub[i++]);
		i--;
		super->sub[i] = (struct _item *)malloc(sizeof(struct _item));
		super->sub[i]->name = (char *)malloc(strlen(name) + 1);
		strcpy(super->sub[i]->name, name);
		super->sub[i]->id = _tmpItem;
		super->sub[i]->checked = 0;
		super->sub[i]->func = func;
		super->sub[i]->sub = NULL;
		return _tmpItem++;
	}
	else {
		for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
			if (super->sub[i] == NULL)return SG_OBJECT_NOT_FOUND;
			if (super->sub[i] == (struct _item *) - 1)continue;
			if ((ret = _addItem(name, super->sub[i]->sub, id, func)) !=
				SG_OBJECT_NOT_FOUND)return ret;
		}
		return SG_OBJECT_NOT_FOUND;
	}
}
int _addSeparator(struct _menu *super, int id) {
	int i, ret;

	if (super == NULL)return SG_OBJECT_NOT_FOUND;
	if (super->id == id) {
		i = 0;
		while (super->sub[i++]);
		i--;
		super->sub[i] = (struct _item *) - 1;
		return SG_NO_ERROR;
	}
	else {
		for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
			if (super->sub[i] == NULL)return SG_OBJECT_NOT_FOUND;
			if (super->sub[i] == (struct _item *) - 1)continue;
			if ((ret = _addSeparator(super->sub[i]->sub, id)) !=
				SG_OBJECT_NOT_FOUND)return ret;
		}
		return SG_OBJECT_NOT_FOUND;
	}
}
void _checkItem(struct _menu *m, int id, int check, int disable) {
	int i, s;
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		if (m->sub[i]) {
			if (m->sub[i] == (struct _item *) - 1)continue;
			if (m->sub[i]->id != id) {
				if (m->sub[i]->sub != NULL)_checkItem(m->sub[i]->sub, id, check, disable);
			}
			else {
				s = GetMenuState(m->hm, id, MF_BYCOMMAND | MF_CHECKED | MF_GRAYED);
				if (check == 1)s |= MF_CHECKED;
				if (check == 0)s &= ~MF_CHECKED;
				if (disable == 1)s |= MF_GRAYED;
				if (disable == 0)s &= ~MF_GRAYED;
				CheckMenuItem(m->hm, id, s);
				EnableMenuItem(m->hm, id, s);
			}
		}
		else break;
	}

}
void _createMenu(enum _menuType type, struct _item *it, HMENU super) {
	int i;
	HMENU tmp;

	if (it == NULL) {
		if (type == MT_MAIN) {
			for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
				if (_windowList[_currentWindow]->_mainMenuInfo.sub[i]) {
					if (_windowList[_currentWindow]->_mainMenuInfo.sub[i] == (struct _item *)(-1)) {
						AppendMenu(super, MF_SEPARATOR, 0, NULL);
					}
					else
						_createMenu(MT_SUB, _windowList[_currentWindow]->_mainMenuInfo.sub[i], _windowList[_currentWindow]->_mainMenuInfo.hm);
				}
				else break;
			}
		}
		if (type == MT_TRAY) {
			for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
				if (_windowList[_currentWindow]->_trayMenuInfo.sub[i]) {
					if (_windowList[_currentWindow]->_trayMenuInfo.sub[i] == (struct _item *)(-1)) {
						AppendMenu(super, MF_SEPARATOR, 0, NULL);
					}
					else
						_createMenu(MT_SUB, _windowList[_currentWindow]->_trayMenuInfo.sub[i],
							_windowList[_currentWindow]->_trayMenuInfo.hm);
				}
				else break;
			}
		}
		if (type >= MT_POPUP) {
			int t = type - MT_POPUP;
			for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
				if (_popupMenuInfo[t].sub[i]) {
					if (_popupMenuInfo[t].sub[i] == (struct _item *)(-1)) {
						AppendMenu(super, MF_SEPARATOR, 0, NULL);
					}
					else
						_createMenu(MT_SUB, _popupMenuInfo[t].sub[i], _popupMenuInfo[t].hm);
				}
				else break;
			}
		}
		return;
	}

	if (it->sub == NULL) {
		AppendMenu(super, MF_STRING, it->id, _widen(it->name));
		return;
	}
	tmp = CreateMenu();
	it->sub->hm = it->hm = tmp;
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		if (it->sub->sub[i]) {
			if (it->sub->sub[i] == (struct _item *)(-1)) {
				AppendMenu(it->hm, MF_SEPARATOR, 0, NULL);
			}
			else
				_createMenu(MT_SUB, it->sub->sub[i], tmp);
		}
		else break;
	}
	AppendMenu(super, MF_STRING | MF_POPUP, (UINT_PTR)tmp, _widen(it->name));
}
void _callMenu(struct _menu *m, int id) {
	int i;
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		if (m->sub[i]) {
			if (m->sub[i] == (struct _item *) - 1)continue;
			if (m->sub[i]->id != id) {
				if (m->sub[i]->sub != NULL)_callMenu(m->sub[i]->sub, id);
			}
			else {
				m->sub[i]->func();
			}
		}
		else break;
	}
}

int initMainMenu() {
	int i;

	_windowList[_currentWindow]->_mainMenu = 1;
	_windowList[_currentWindow]->_mainMenuInfo.id = _tmpItem++;
	_windowList[_currentWindow]->_mainMenuInfo.name = (char *)malloc(strlen("main") + 1);
	strcpy(_windowList[_currentWindow]->_mainMenuInfo.name, "main");
	_windowList[_currentWindow]->_mainMenuInfo.hm = CreateMenu();
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		_windowList[_currentWindow]->_mainMenuInfo.sub[i] = NULL;
	}
	return _windowList[_currentWindow]->_mainMenuInfo.id;
}
int addMainMenuList(SGtext title, int id) {
	if (_windowList[_currentWindow]->_mainMenu == 0)return SG_INVALID_MODE;
	return _addList(title, &_windowList[_currentWindow]->_mainMenuInfo, id);
}
int addMainMenuItem(SGtext title, int id, void(*func)()) {
	if (_windowList[_currentWindow]->_mainMenu == 0)return SG_INVALID_MODE;
	return _addItem(title, &_windowList[_currentWindow]->_mainMenuInfo, id, func);
}
int addMainMenuSeparator(int id) {
	if (_windowList[_currentWindow]->_mainMenu == 0)return SG_INVALID_MODE;
	return _addSeparator(&_windowList[_currentWindow]->_mainMenuInfo, id);
}
int enableMainItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, -1, 0);
	return 0;
}
int disableMainItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, -1, 1);
	return 0;
}
void checkMainItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, 1, -1);
}
void uncheckMainItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, 0, -1);
}
void addTray() {
	NOTIFYICONDATA nid;

	if (_windowList[_currentWindow]->_useTray)return;
	_windowList[_currentWindow]->_useTray = 1;

	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = _windowList[_currentWindow]->getHwnd();
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAY;
	if (_windowList[_currentWindow]->getIcon() == NULL)
		nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	else
		nid.hIcon = _windowList[_currentWindow]->getIcon();
	_strcpy(nid.szTip, _widen(_windowList[_currentWindow]->getTitle()));
	Shell_NotifyIcon(NIM_ADD, &nid);

	if(_windowList[_currentWindow]->_trayMenu)
		_createMenu(MT_TRAY, NULL, _windowList[_currentWindow]->_trayMenuInfo.hm);
}
void hideToTray() {
	addTray();
	ShowWindow(_windowList[_currentWindow]->getHwnd(), SW_HIDE);
}
void restoreFromTray() {
	ShowWindow(_windowList[_currentWindow]->getHwnd(), SW_RESTORE);
}
int initTrayMenu() {
	int i;

	_windowList[_currentWindow]->_trayMenu = 1;
	_windowList[_currentWindow]->_trayMenuInfo.id = _tmpItem++;
	_windowList[_currentWindow]->_trayMenuInfo.name = (char *)malloc(strlen("tray")+1);
	strcpy(_windowList[_currentWindow]->_trayMenuInfo.name, "tray");
	_windowList[_currentWindow]->_trayMenuInfo.hm = CreatePopupMenu();
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		_windowList[_currentWindow]->_trayMenuInfo.sub[i] = NULL;
	}
	return _windowList[_currentWindow]->_trayMenuInfo.id;
}
int addTrayMenuList(SGtext title, int id) {
	if (_windowList[_currentWindow]->_trayMenu == 0)return SG_INVALID_MODE;
	return _addList(title, &_windowList[_currentWindow]->_trayMenuInfo, id);
}
int addTrayMenuItem(SGtext title, int id, void(*func)()) {
	if (_windowList[_currentWindow]->_trayMenu == 0)return SG_INVALID_MODE;
	return _addItem(title, &_windowList[_currentWindow]->_trayMenuInfo, id, func);
}
int addTrayMenuSeparator(int id) {
	if (_windowList[_currentWindow]->_trayMenu == 0)return SG_INVALID_MODE;
	return _addSeparator(&_windowList[_currentWindow]->_trayMenuInfo, id);
}
int enableTrayItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_trayMenuInfo, id, -1, 0);
	return 0;
}
int disableTrayItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_trayMenuInfo, id, -1, 1);
	return 0;
}
void checkTrayItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_trayMenuInfo, id, 1, -1);
}
void uncheckTrayItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_trayMenuInfo, id, 0, -1);
}
int createPopupMenu() {
	int i;

	_popupMenuInfo[_popupNum].id = _tmpItem++;
	_popupMenuInfo[_popupNum].name = (char *)malloc(strlen("popup") + 1);
	strcpy(_popupMenuInfo[_popupNum].name, "popup");
	_popupMenuInfo[_popupNum].hm = CreatePopupMenu();
	for (i = 0; i < SG_MAX_MENU_ITEM_NUM; i++) {
		_popupMenuInfo[_popupNum].sub[i] = NULL;
	}
	return _popupMenuInfo[_popupNum++].id;
}
int addPopupMenuList(SGtext title, int id) {
	int i, ret;
	for (i = 0; i < _popupNum; i++) {
		if ((ret = _addList(title, &_popupMenuInfo[i], id)) < 0)continue;
		break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	return ret;
}
int addPopupMenuItem(SGtext title, int id, void(*func)()) {
	int i, ret;
	for (i = 0; i < _popupNum; i++) {
		if ((ret = _addItem(title, &_popupMenuInfo[i], id, func)) < 0)continue;
		break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	return ret;
}
int addPopupMenuSeparator(int id) {
	int i, ret;
	for (i = 0; i < _popupNum; i++) {
		if ((ret = _addSeparator(&_popupMenuInfo[i], id)) < 0)continue;
		break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	return ret;
}
int enablePopupItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, -1, 0);
	return 0;
}
int disablePopupItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, -1, 1);
	return 0;
}
void checkPopupItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, 1, -1);
}
void uncheckPopupItem(int id) {
	_checkItem(&_windowList[_currentWindow]->_mainMenuInfo, id, 0, -1);
}
int finishPopupMenu(int id) {
	int i;
	for (i = 0; i < _popupNum; i++) {
		if (_popupMenuInfo[i].id == id)break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	_createMenu(MT_POPUP, NULL, _popupMenuInfo[i].hm);
	return SG_NO_ERROR;
}
int showPopupMenu(int menu, int x, int y) {
	int i;
	struct tagPOINT p;

	for (i = 0; i < _popupNum; i++) {
		if (_popupMenuInfo[i].id == menu)break;
	}
	if (i == _popupNum)return SG_OBJECT_NOT_FOUND;
	if (_windowList[_currentWindow]->resize) {
		p.x = x * _windowList[_currentWindow]->getWindowSize().x /
			_windowList[_currentWindow]->getBufferSize().x;
		p.y = y * _windowList[_currentWindow]->getWindowSize().y /
			_windowList[_currentWindow]->getBufferSize().y;
	}
	else {
		p.x = x;
		p.y = y;
	}
	ClientToScreen(_windowList[_currentWindow]->getHwnd(), &p);
	TrackPopupMenu(_popupMenuInfo[i].hm, TPM_LEFTALIGN,
		p.x, p.y, 0, _windowList[_currentWindow]->getHwnd(), NULL);

	return SG_NO_ERROR;
}


/*
* Win main functions
* The main functions for windows application.
*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (!_baseWindow->inLoop) {
		_baseWindow->inLoop = 1;
		_baseWindow->setThread();
		SetTimer(hwnd, TIMER_DELTA_HANDLE, 10, NULL);
	}

	LPPOINT lPoint;
	switch (message) {
	case WM_CREATE:
		if (_baseWindow->_mainMenu) {
			_createMenu(MT_MAIN, NULL, _baseWindow->_mainMenuInfo.hm);
			SetMenu(hwnd, _baseWindow->_mainMenuInfo.hm);
		}
		break;

	case WM_COMMAND:
		if(_baseWindow->_mainMenu)
			_callMenu(&_baseWindow->_mainMenuInfo, LOWORD(wParam));
		if(_baseWindow->_trayMenu)
			_callMenu(&_baseWindow->_trayMenuInfo, LOWORD(wParam));
		break;

	case WM_SIZE:
		_baseWindow->makeResize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		_baseWindow->drawWidgets(WIDGET_FRONT);
		_baseWindow->drawPanel();
		_baseWindow->fillRect(hwnd);
		_baseWindow->clearPanel();
		_baseWindow->drawWidgets(WIDGET_BACK);
		_endSubWindow();
		break;

	case WM_TIMER:
		sgIdle(hwnd);
		if (wParam == TIMER_DELTA_HANDLE) {
			if (_windowSem <= 0)break;
			_startSubWindow(0);
			sgLoop();
			_endSubWindow();
		}
		if (_baseWindow->sglMode == TEXT_MAP)_baseWindow->textLoop();
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_TRAY:
		switch (lParam) {
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
			break;
		case WM_LBUTTONDBLCLK:
			ShowWindow(hwnd, SW_RESTORE);
			SetForegroundWindow(hwnd);
			break;
		case WM_RBUTTONDOWN:
			lPoint = (LPPOINT)malloc(sizeof(LPPOINT));
			GetCursorPos(lPoint);
			TrackPopupMenu(_baseWindow->_trayMenuInfo.hm, TPM_LEFTALIGN, lPoint->x, lPoint->y, 0, hwnd, NULL);
			free(lPoint);
			break;
		}
		break;

	case	WM_SETCURSOR:
		//return 0;
		break;

	case WM_KEYDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgSpecialDown((int)wParam, 0, 0);
		_endSubWindow();
		break;

	case WM_CHAR:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		if (wParam >= 0x100) {
			sgKeyDown((int)wParam, 0, 0, true);
		}
		else {
			sgKeyDown((int)wParam |
				((GetKeyState(VK_CONTROL) & 0x8000) >> 1) |
				((GetKeyState(VK_SHIFT) & 0x8000) >> 2), 0, 0, false);
		}
		_endSubWindow();
		break;

	case WM_KEYUP:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgKeyUp((int)wParam, 0, 0);
		sgSpecialUp((int)wParam, 0, 0);
		_endSubWindow();
		break;

	case WM_MOUSEMOVE:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		if (!wParam)sgMouse(LOWORD(lParam), HIWORD(lParam));
		else sgDrag(LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_MOUSEWHEEL:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgWheel((short)HIWORD(wParam));
		_endSubWindow();
		break;

	case WM_LBUTTONDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgClick(SG_LEFT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_MBUTTONDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgClick(SG_MIDDLE_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_RBUTTONDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgClick(SG_RIGHT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_LBUTTONUP:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgClick(SG_LEFT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_MBUTTONUP:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgClick(SG_MIDDLE_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_RBUTTONUP:
		if (_windowSem <= 0)break;
		_startSubWindow(0);
		sgClick(SG_RIGHT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
LRESULT CALLBACK SubWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	unsigned int index;
	for (index = 0; index < _windowList.size(); index++) {
		if (_windowList[index] && hwnd == _windowList[index]->getHwnd())break;
	}
	if (index == _windowList.size() || !_windowList[index])
		return DefWindowProc(hwnd, message, wParam, lParam);

	if (!_windowList[index]->inLoop) {
		_windowList[index]->inLoop = 1;
		_windowList[index]->setThread();
		SetTimer(hwnd, TIMER_DELTA_HANDLE, 10, NULL);
	}

	LPPOINT lPoint;
	switch (message) {
	case WM_CREATE:
		if (_windowList[index]->_mainMenu) {
			_createMenu(MT_MAIN, NULL, _windowList[index]->_mainMenuInfo.hm);
			SetMenu(hwnd, _windowList[index]->_mainMenuInfo.hm);
		}

		if (_windowList[index]->sglCircle) {
			SetWindowRgn(_windowList[index]->getHwnd(),
				CreateEllipticRgn(_windowList[index]->sglCircleCx - _windowList[index]->sglCircleRx + 8,
					_windowList[index]->sglCircleCy - _windowList[index]->sglCircleRy + 8,
					_windowList[index]->sglCircleCx + _windowList[index]->sglCircleRx + 8,
					_windowList[index]->sglCircleCy + _windowList[index]->sglCircleRy + 8), TRUE);
			hideCaption();
		}
		break;

	case WM_COMMAND:
		if (_windowList[index]->_mainMenu)
			_callMenu(&_windowList[index]->_mainMenuInfo, LOWORD(wParam));
		if (_windowList[index]->_trayMenu)
			_callMenu(&_windowList[index]->_trayMenuInfo, LOWORD(wParam));
		return 0;

	case WM_SIZE:
		_windowList[index]->makeResize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		_windowList[index]->drawWidgets(WIDGET_FRONT);
		_windowList[index]->drawPanel();
		_windowList[index]->fillRect(hwnd);
		_windowList[index]->clearPanel();
		_windowList[index]->drawWidgets(WIDGET_BACK);
		_endSubWindow();
		break;

	case WM_TIMER:
		if (wParam == TIMER_DELTA_HANDLE) {
			if (_windowSem <= 0)break;
			_startSubWindow(index);
			_windowList[index]->loop();
			_endSubWindow();
		}
		if (_windowList[index] && _windowList[index]->sglMode == TEXT_MAP)_windowList[index]->textLoop();
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_TRAY:
		switch (lParam) {
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
			break;
		case WM_LBUTTONDBLCLK:
			ShowWindow(hwnd, SW_RESTORE);
			SetForegroundWindow(hwnd);
			break;
		case WM_RBUTTONDOWN:
			lPoint = (LPPOINT)malloc(sizeof(LPPOINT));
			GetCursorPos(lPoint);
			TrackPopupMenu(_windowList[index]->_trayMenuInfo.hm, TPM_LEFTALIGN, lPoint->x, lPoint->y, 0, hwnd, NULL);
			free(lPoint);
			break;
		}
		break;

	case	WM_SETCURSOR:
		//return 0;
		break;

	case WM_KEYDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgSpecialDown((int)wParam, 0, 0);
		_endSubWindow();
		break;

	case WM_CHAR:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		if (wParam >= 0x100) {
			sgKeyDown((int)wParam, 0, 0, true);
		}
		else {
			sgKeyDown((int)wParam |
				((GetKeyState(VK_CONTROL) & 0x8000) >> 1) |
				((GetKeyState(VK_SHIFT) & 0x8000) >> 2), 0, 0, false);
		}
		_endSubWindow();
		break;

	case WM_KEYUP:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgKeyUp((int)wParam, 0, 0);
		sgSpecialUp((int)wParam, 0, 0);
		_endSubWindow();
		break;

	case WM_MOUSEMOVE:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		if (!wParam)sgMouse(LOWORD(lParam), HIWORD(lParam));
		else sgDrag(LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_MOUSEWHEEL:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgWheel((short)HIWORD(wParam));
		_endSubWindow();
		break;

	case WM_LBUTTONDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgClick(SG_LEFT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_MBUTTONDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgClick(SG_MIDDLE_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_RBUTTONDOWN:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgClick(SG_RIGHT_BUTTON, SG_BUTTON_DOWN, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_LBUTTONUP:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgClick(SG_LEFT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_MBUTTONUP:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgClick(SG_MIDDLE_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_RBUTTONUP:
		if (_windowSem <= 0)break;
		_startSubWindow(index);
		sgClick(SG_RIGHT_BUTTON, SG_BUTTON_UP, LOWORD(lParam), HIWORD(lParam));
		_endSubWindow();
		break;

	case WM_DESTROY:
		closeWindow(index);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	MSG msg;

	_sgInit();

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = TEXT("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	_baseWindow->createWindow(hInstance, wc);
	if (_baseWindow->sglCircle) {
		SetWindowRgn(_baseWindow->getHwnd(),
			CreateEllipticRgn(_baseWindow->sglCircleCx - _baseWindow->sglCircleRx,
				_baseWindow->sglCircleCy - _baseWindow->sglCircleRy,
				_baseWindow->sglCircleCx + _baseWindow->sglCircleRx,
				_baseWindow->sglCircleCy + _baseWindow->sglCircleRy), TRUE);
		//hideCaption();
	}

	ShowWindow(_baseWindow->getHwnd(), nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
