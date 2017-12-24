#ifndef KPP_MAIN
#define KPP_MAIN
#include <winsgl.h>

enum kppState {
	KS_LAUNCH,
	KS_FREE
};

void nullFunc();
void setVolume(widgetObj *w, int x, int y);
void setMenu();
void setPanel();

void refreshLayout();

void freePlay();

#endif

