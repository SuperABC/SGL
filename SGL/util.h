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



#ifndef SGL_UTIL_H
#define SGL_UTIL_H


#include "winsgl.h"
#include <windows.h>


class String {
private:
	LPWSTR ws;
	LPSTR ss;
public:
	String(LPWSTR str) {
		ws = (LPWSTR)malloc((wcslen(str) + 1) * sizeof(wchar_t));
		wcscpy(ws, str);

		int rt = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		ss = (char*)malloc((rt + 1));
		WideCharToMultiByte(CP_ACP, 0, str, -1, ss, rt, NULL, NULL);
	}
	String(LPCSTR str) {
		ss = (LPSTR)malloc(strlen(str) + 1);
		strcpy(ss, str);

		int rt = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
		ws = (LPWSTR)malloc((rt + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, str, -1, ws, rt * sizeof(wchar_t));
	}
	~String() {
		free(ws);
		free(ss);
	}

	LPWSTR widen() {
		return ws;
	}
	LPSTR shorten() {
		return ss;
	}

	static void copy(LPWSTR dst, LPWSTR src) {
		wcscpy(dst, src);
	}
	static void copy(LPSTR dst, LPSTR src) {
		strcpy(dst, src);
	}
	static int len(LPWSTR str) {
		return wcslen(str);
	}
	static int len(LPSTR str) {
		return strlen(str);
	}
	static void connect(LPWSTR str, const wchar_t *suffix) {
		wcscat(str, suffix);
	}
	static void connect(LPSTR str, const char *suffix) {
		strcat(str, suffix);
	}
	static void connect(LPWSTR str, const char *suffix) {
		wcscat(str, _widen((LPSTR)suffix));
	}
	static void connect(LPSTR str, const wchar_t *suffix) {
		strcat(str, _shorten((LPWSTR)suffix));
	}
};

#endif