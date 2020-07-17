#include <windows.h>
#include <tchar.h>

#define IDC_LABEL     1000
#define IDC_COPY_IMG  1001
#define IDC_COPY_TXT  1002

static TCHAR szAppName[] = TEXT("Clipboard Demo");
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND     hWnd;
	MSG      msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindow(szAppName,            // window class name
		szAppName,           // window caption
		WS_OVERLAPPEDWINDOW, // window style
		CW_USEDEFAULT,       // initial x position
		CW_USEDEFAULT,       // initial y position
		400,              // initial x size
		300,              // initial y size
		NULL,             // parent window handle
		NULL,             // window menu handle
		hInstance,        // program instance handle
		NULL);            // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

static int DrawBmp(HDC hDC, int xDst, int yDst, int width, int height, int BytesPerPixel, unsigned char *pPixels)
{
	int ret = -1;
	HDC hdcMem;
	BITMAPINFO bmi;
	BYTE *pBits = NULL;

	memset(&bmi, 0x00, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = BytesPerPixel * 8;
	bmi.bmiHeader.biCompression = BI_RGB;

	hdcMem = CreateCompatibleDC(hDC);
	if (hdcMem)
	{
		HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void **)&pBits, NULL, 0);
		if (hBitmap)
		{
			HGDIOBJ hOldBmp = SelectObject(hdcMem, hBitmap);
			memcpy(pBits, pPixels, width * height * BytesPerPixel);
			BitBlt(hDC, xDst, yDst, width, height, hdcMem, 0, 0, SRCCOPY);
			SelectObject(hdcMem, hOldBmp);
			DeleteObject(hBitmap);
			ret = 0;
		}
		DeleteDC(hdcMem);
	}

	return ret;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static HBITMAP hBmp;
	static BITMAP  bm;

	switch (message)
	{
	case WM_CREATE:
		CreateWindow(TEXT("STATIC"), TEXT("ÄãºÃ£¬World!"), WS_CHILD | WS_VISIBLE, 10, 160, 100, 20, hWnd, (HMENU)IDC_LABEL, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("copy image"), WS_CHILD | WS_VISIBLE, 10, 190, 100, 20, hWnd, (HMENU)IDC_COPY_IMG, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("copy text"), WS_CHILD | WS_VISIBLE, 10, 220, 100, 20, hWnd, (HMENU)IDC_COPY_TXT, NULL, NULL);
		hBmp = (HBITMAP)LoadImage(NULL, TEXT("Source/pic.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetObject(hBmp, sizeof(BITMAP), &bm);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBmp);
		BitBlt(hDC, 10, 10, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_COPY_IMG:
		{
			BOOL ret;
			BYTE *pData = NULL;
			BITMAPINFO bmpInfo;

			bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmpInfo.bmiHeader.biWidth = bm.bmWidth;
			bmpInfo.bmiHeader.biHeight = bm.bmHeight;
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biBitCount = 32;
			bmpInfo.bmiHeader.biCompression = BI_RGB;

			HDC hClientDC = GetDC(hWnd);
			HDC hMemDC = CreateCompatibleDC(hClientDC);
			HBITMAP hBitmap = CreateDIBSection(hMemDC, &bmpInfo, DIB_RGB_COLORS, (void **)&pData, NULL, 0);
			SelectObject(hMemDC, hBitmap);
			ret = BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hClientDC, 10, 10, SRCCOPY);
			DeleteDC(hMemDC);

			LONG len = bm.bmWidth * bm.bmHeight * 4;
			HGLOBAL hClipData = GlobalAlloc(GHND, len);
			BYTE *pClipData = (BYTE *)GlobalLock(hClipData);
			memcpy(pClipData, pData, len);
			ret = GlobalUnlock(hClipData);

			ret = OpenClipboard(hWnd);
			ret = EmptyClipboard();
			SetClipboardData(CF_BITMAP, hClipData);
			ret = CloseClipboard();

			DeleteObject(hBitmap);
			ReleaseDC(hWnd, hClientDC);

			MessageBox(hWnd, TEXT("image has been copy into clipboard"), TEXT("info"), MB_OK);
		}
		break;

		case IDC_COPY_TXT:
		{
			BOOL ret;
			TCHAR buf[256];

			GetWindowText(GetDlgItem(hWnd, IDC_LABEL), buf, sizeof(buf));
			int len = _tcslen(buf) + 1;

			HGLOBAL hClipData = GlobalAlloc(GHND, len * sizeof(TCHAR));
			TCHAR *pClipData = (TCHAR *)GlobalLock(hClipData);
			memcpy(pClipData, buf, len * sizeof(TCHAR));
			pClipData[len - 1] = (TCHAR)0;
			ret = GlobalUnlock(hClipData);

			ret = OpenClipboard(hWnd);
			ret = EmptyClipboard();
			SetClipboardData(CF_TEXT, hClipData);
			ret = CloseClipboard();

			MessageBox(hWnd, TEXT("text has been copy into clipboard"), TEXT("info"), MB_OK);
		}
		break;

		default:
			break;
		}
	}
	return 0;

	case WM_LBUTTONUP:
	{
		BOOL ret;
		WORD xPos = LOWORD(lParam);
		WORD yPos = HIWORD(lParam);

		ret = IsClipboardFormatAvailable(CF_BITMAP);
		if (ret)
		{
			ret = OpenClipboard(hWnd);
			HGLOBAL hglb = GetClipboardData(CF_BITMAP);
			//len = GlobalSize(hglb);
			BYTE *pClipData = (BYTE *)GlobalLock(hglb);
			HDC hClientDC = GetDC(hWnd);
			DrawBmp(hClientDC, xPos, yPos, bm.bmWidth, bm.bmHeight, 4, pClipData);
			GlobalUnlock(hglb);
			CloseClipboard();
			ReleaseDC(hWnd, hClientDC);
		}

		ret = IsClipboardFormatAvailable(CF_TEXT);
		if (ret)
		{
			ret = OpenClipboard(hWnd);
			HGLOBAL hglb = GetClipboardData(CF_TEXT);
			TCHAR *pClipData = (TCHAR *)GlobalLock(hglb);
			HDC hClientDC = GetDC(hWnd);
			int len = _tcslen(pClipData);
			//len = GlobalSize(hglb);
			TextOut(hClientDC, xPos, yPos, pClipData, len);
			GlobalUnlock(hglb);
			CloseClipboard();
			ReleaseDC(hWnd, hClientDC);
		}
	}
	return 0;

	case WM_DESTROY:
		DeleteObject(hBmp);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}