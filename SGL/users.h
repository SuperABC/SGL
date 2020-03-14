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



#ifndef SGL_USERS_H
#define SGL_USERS_H


#include "winsgl.h"
#include <windows.h>


class Key {
private:
	word keyBuf[1024];
	int front = -1, rear = 0;

public:
	void enqueue(word key) {
		front = (front + 1) % 1024;
		keyBuf[front] = (word)key;
	}
	word dequeue() {
		word ret;
		ret = keyBuf[rear++];
		rear %= 1024;
		return ret;
	}
	int empty() {
		return (front + 1) % 1024 == rear;
	}
	void clear() {
		rear = (front + 1) % 1024;
	}
};

class Mouse {
private:
	vec3i mouseBuf[1024];
	int front = -1, rear = 0;

public:
	vec2i pos;
	int left = SG_BUTTON_UP, middle = SG_BUTTON_UP, right = SG_BUTTON_UP;

	void enqueue(vec3i mouse) {
		front = (front + 1) % 1024;
		mouseBuf[front] = mouse;
	}
	vec3i dequeue() {
		vec3i ret;
		ret = mouseBuf[rear++];
		rear %= 1024;
		return ret;
	}
	int empty() {
		return (front + 1) % 1024 == rear;
	}
	void clear() {
		rear = (front + 1) % 1024;
	}
};


#endif