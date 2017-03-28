#include "winsgl.h"

#define inRect(x, y, x1, y1, x2, y2) ((x>=x1&&x<=x2)&&(y>=y1&&y<=y2))
#define speedUp(e) (e+=e>0?2:-2)

void movePlayer(int x, int y);
void moveEnemy();
int hasCollision();

struct _player {
	vecTwo pos;
	bitMap *oldImage;
}player;
struct _enemy {
	vecTwo pos;
	vecTwo vel;
	bitMap *oldImage;
}enemy1, enemy2, enemy3, enemy4;
int liveTime = 0;

void sgSetup() {
	initWindow(640, 640, "Collision --SGL Sample Game", BIT_MAP);
	initMouse(SG_COORDINATE);
	player.oldImage = (bitMap*)malloc(sizeof(bitMap));
	enemy1.oldImage = (bitMap*)malloc(sizeof(bitMap));
	enemy2.oldImage = (bitMap*)malloc(sizeof(bitMap));
	enemy3.oldImage = (bitMap*)malloc(sizeof(bitMap));
	enemy4.oldImage = (bitMap*)malloc(sizeof(bitMap));
}
void sgLoop() {
	static int first = 1, cont = 0, drag = 0;
	vecTwo position;
	int press;

	if (first) {
		loadBmp(0, 0, "Source\\collision\\background.bmp");
		player.pos.x = player.pos.y = 290;
		enemy1.pos.x = 120;
		enemy1.pos.y = 100;
		enemy2.pos.x = 432;
		enemy2.pos.y = 120;
		enemy3.pos.x = 100;
		enemy3.pos.y = 432;
		enemy4.pos.x = 480;
		enemy4.pos.y = 480;
		enemy1.vel.x = enemy1.vel.y = enemy2.vel.y = enemy3.vel.x = 4;
		enemy4.vel.x = enemy4.vel.y = enemy3.vel.y = enemy2.vel.x = -4;
		getImage(player.pos.x, player.pos.y, player.pos.x + 59, player.pos.y + 59, player.oldImage);
		getImage(enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63, enemy1.oldImage);
		getImage(enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31, enemy2.oldImage);
		getImage(enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95, enemy3.oldImage);
		getImage(enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51, enemy4.oldImage);
		setColor(255, 0, 0);
		putQuad(player.pos.x, player.pos.y, player.pos.x + 59, player.pos.y + 59, SOLID_FILL);
		setColor(0, 0, 255);
		putQuad(enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63, SOLID_FILL);
		putQuad(enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31, SOLID_FILL);
		putQuad(enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95, SOLID_FILL);
		putQuad(enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51, SOLID_FILL);
		first = 0;
	}

	press = mouseStatus(SG_LEFT_BUTTON);
	position = mousePos();

	setColor(0, 0, 0);
	putQuad(0, 0, 639, 39, SOLID_FILL);
	setColor(255, 255, 0);
	putNumber(liveTime, 640, 0, 'r');

	if (hasCollision()) {
		first = 1;
		cont = 0;
		drag = 0;
		return;
	}
	if (press==SG_BUTTON_DOWN&&(inRect(position.x, position.y, player.pos.x, player.pos.y, player.pos.x+59, player.pos.y+59)||drag)) {
		if(!cont)liveTime = 0;
		cont = drag = 1;
		movePlayer(position.x - 29, position.y - 29);
	}
	if (press == SG_BUTTON_UP && inRect(position.x, position.y, player.pos.x, player.pos.y, player.pos.x + 59, player.pos.y + 59))drag = 0;
	if (!cont)return;
	else {
		moveEnemy();
		delayEnd(20);
		delayBegin();
		liveTime += 24;
	}
}

void movePlayer(int x, int y) {
	putImage(player.pos.x, player.pos.y, player.oldImage, COPY_PUT);
	setColor(255, 0, 0);
	player.pos.x = x;
	player.pos.y = y;
	free(player.oldImage->data);
	getImage(player.pos.x, player.pos.y, player.pos.x + 83, player.pos.y + 63, player.oldImage);
	putQuad(player.pos.x, player.pos.y, player.pos.x + 59, player.pos.y + 59, SOLID_FILL);
}
void moveEnemy() {
	putImage(enemy1.pos.x, enemy1.pos.y, enemy1.oldImage, COPY_PUT);
	putImage(enemy2.pos.x, enemy2.pos.y, enemy2.oldImage, COPY_PUT);
	putImage(enemy3.pos.x, enemy3.pos.y, enemy3.oldImage, COPY_PUT);
	putImage(enemy4.pos.x, enemy4.pos.y, enemy4.oldImage, COPY_PUT);

	if (liveTime % 10000 < 24) {
		speedUp(enemy1.vel.x);
		speedUp(enemy1.vel.y);
		speedUp(enemy2.vel.x);
		speedUp(enemy2.vel.y);
		speedUp(enemy3.vel.x);
		speedUp(enemy3.vel.y);
		speedUp(enemy4.vel.x);
		speedUp(enemy4.vel.y);
	}

	enemy1.pos.x += enemy1.vel.x;
	enemy1.pos.y += enemy1.vel.y;
	enemy2.pos.x += enemy2.vel.x;
	enemy2.pos.y += enemy2.vel.y;
	enemy3.pos.x += enemy3.vel.x;
	enemy3.pos.y += enemy3.vel.y;
	enemy4.pos.x += enemy4.vel.x;
	enemy4.pos.y += enemy4.vel.y;

	if (enemy1.pos.x < 0) {
		enemy1.pos.x = -enemy1.pos.x;
		enemy1.vel.x = -enemy1.vel.x;
	}
	if (enemy1.pos.x > 555) {
		enemy1.pos.x = 1110-enemy1.pos.x;
		enemy1.vel.x = -enemy1.vel.x;
	}
	if (enemy1.pos.y < 0) {
		enemy1.pos.y = -enemy1.pos.y;
		enemy1.vel.y = -enemy1.vel.y;
	}
	if (enemy1.pos.y > 575) {
		enemy1.pos.y = 1150 - enemy1.pos.y;
		enemy1.vel.y = -enemy1.vel.y;
	}

	if (enemy2.pos.x < 0) {
		enemy2.pos.x = -enemy2.pos.x;
		enemy2.vel.x = -enemy2.vel.x;
	}
	if (enemy2.pos.x > 503) {
		enemy2.pos.x = 1006 - enemy2.pos.x;
		enemy2.vel.x = -enemy2.vel.x;
	}
	if (enemy2.pos.y < 0) {
		enemy2.pos.y = -enemy2.pos.y;
		enemy2.vel.y = -enemy2.vel.y;
	}
	if (enemy2.pos.y > 607) {
		enemy2.pos.y = 1214 - enemy2.pos.y;
		enemy2.vel.y = -enemy2.vel.y;
	}

	if (enemy3.pos.x < 0) {
		enemy3.pos.x = -enemy3.pos.x;
		enemy3.vel.x = -enemy3.vel.x;
	}
	if (enemy3.pos.x > 579) {
		enemy3.pos.x = 1158 - enemy3.pos.x;
		enemy3.vel.x = -enemy3.vel.x;
	}
	if (enemy3.pos.y < 0) {
		enemy3.pos.y = -enemy3.pos.y;
		enemy3.vel.y = -enemy3.vel.y;
	}
	if (enemy3.pos.y > 543) {
		enemy3.pos.y = 1086 - enemy3.pos.y;
		enemy3.vel.y = -enemy3.vel.y;
	}

	if (enemy4.pos.x < 0) {
		enemy4.pos.x = -enemy4.pos.x;
		enemy4.vel.x = -enemy4.vel.x;
	}
	if (enemy4.pos.x > 575) {
		enemy4.pos.x = 1150 - enemy4.pos.x;
		enemy4.vel.x = -enemy4.vel.x;
	}
	if (enemy4.pos.y < 0) {
		enemy4.pos.y = -enemy4.pos.y;
		enemy4.vel.y = -enemy4.vel.y;
	}
	if (enemy4.pos.y > 588) {
		enemy4.pos.y = 1176 - enemy4.pos.y;
		enemy4.vel.y = -enemy4.vel.y;
	}

	free(enemy1.oldImage->data);
	free(enemy2.oldImage->data);
	free(enemy3.oldImage->data);
	free(enemy4.oldImage->data);
	getImage(enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63, enemy1.oldImage);
	getImage(enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31, enemy2.oldImage);
	getImage(enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95, enemy3.oldImage);
	getImage(enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51, enemy4.oldImage);
	setColor(0, 0, 255);
	putQuad(enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63, SOLID_FILL);
	putQuad(enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31, SOLID_FILL);
	putQuad(enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95, SOLID_FILL);
	putQuad(enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51, SOLID_FILL);
}
int hasCollision() {
	if (inRect(player.pos.x, player.pos.y, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x, player.pos.y + 29, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x, player.pos.y + 59, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x + 29, player.pos.y, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x + 29, player.pos.y + 59, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x + 59, player.pos.y, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 29, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 59, enemy1.pos.x, enemy1.pos.y, enemy1.pos.x + 83, enemy1.pos.y + 63))return 1;

	if (inRect(player.pos.x, player.pos.y, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x, player.pos.y + 29, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x, player.pos.y + 59, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x + 29, player.pos.y, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x + 29, player.pos.y + 59, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x + 59, player.pos.y, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 29, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 59, enemy2.pos.x, enemy2.pos.y, enemy2.pos.x + 135, enemy2.pos.y + 31))return 1;

	if (inRect(player.pos.x, player.pos.y, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x, player.pos.y + 29, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x, player.pos.y + 59, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x + 29, player.pos.y, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x + 29, player.pos.y + 59, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x + 59, player.pos.y, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 29, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 59, enemy3.pos.x, enemy3.pos.y, enemy3.pos.x + 59, enemy3.pos.y + 95))return 1;

	if (inRect(player.pos.x, player.pos.y, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x, player.pos.y + 29, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x, player.pos.y + 59, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x + 29, player.pos.y, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x + 29, player.pos.y + 59, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x + 59, player.pos.y, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 29, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;
	if (inRect(player.pos.x + 59, player.pos.y + 59, enemy4.pos.x, enemy4.pos.y, enemy4.pos.x + 63, enemy4.pos.y + 51))return 1;

	if (!inRect(player.pos.x, player.pos.y, 40, 40, 539, 539))return 1;
	return 0;
}
