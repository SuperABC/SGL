#include "main.h"
extern knight *player;
extern tower *wall[81];
extern opponent *mons[41];
extern int pro, obj, maxFloor;

void start() {
	static int first = 1;
	vecThree mouse;
	if (first) {
		loadBmp(0, 0, "Source\\tower\\start.bmp");
		first = 0;
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (mouse.x < 240 || mouse.x>439)return;
		if (mouse.y >= 160 && mouse.y <= 199) {
			pro = ARCHIVE;
			first = 1;
		}
		if (mouse.y >= 240 && mouse.y <= 279) {
			pro = INSTRUCT;
			first = 1;
		}
		if (mouse.y >= 320 && mouse.y <= 359) {
			exit(0);
		}
	}
}
void instruct() {
	static int first = 1;
	static int t = 0;
	bitMap *oldImage = (bitMap*)malloc(sizeof(bitMap));
	if (first) {
		setColor(0, 0, 0);
		clearScreen();
		first = 0;
	}
	if (++t >= 840) {
		t = 0;
		first = 1;
		pro = START;
	}
	if (t <= 400) {
		delayBegin();
		loadBmp(0, 400 - t, "Source\\tower\\instruct.bmp");
		putQuad(0, 480, 679, 519, SOLID_FILL);
		delayEnd(20);
	}
	if (t > 400) {
		delayBegin();
		getImage(0, 41, 679, 519, oldImage);
		putImage(0, 40, oldImage, 0);
		delayEnd(20);
	}
	if (biosKey(1)) {
		free(oldImage);
		pro = START;
		first = 1;
	}
}
void archive() {
	static int first = 1;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\tower\\save.bmp");
		showArchive();
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = START;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (inRect(mouse.x, mouse.y, 240, 60, 440, 140)) {
			loadFile(0);
			pro = FIGURE;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 120, 200, 280, 240)) {
			loadFile(1);
			pro = PLAYING;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 120, 280, 280, 320)) {
			loadFile(2);
			pro = PLAYING;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 120, 360, 280, 400)) {
			loadFile(3);
			pro = PLAYING;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 400, 200, 560, 240)) {
			loadFile(4);
			pro = PLAYING;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 400, 280, 560, 320)) {
			loadFile(5);
			pro = PLAYING;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 400, 360, 560, 400)) {
			loadFile(6);
			pro = PLAYING;
			first = 1;
			return;
		}
	}
}
void figure() {
	static int first = 1;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\tower\\clothes.bmp");
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = ARCHIVE;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (inRect(mouse.x, mouse.y, 200, 120, 239, 159)) {
			player->outlook = 0;
			pro = DIFFICULTY;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 280, 120, 319, 159)) {
			player->outlook = 1;
			pro = DIFFICULTY;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 360, 120, 399, 159)) {
			player->outlook = 2;
			pro = DIFFICULTY;
			first = 1;
			return;
		}
		if (inRect(mouse.x, mouse.y, 440, 120, 479, 159)) {
			player->outlook = 3;
			pro = DIFFICULTY;
			first = 1;
			return;
		}
	}
}
void difficulty() {
	static int first = 1;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\tower\\dif.bmp");
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = FIGURE;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (mouse.x < 240 || mouse.x>439)return;
		if (mouse.y >= 160 && mouse.y <= 199) {
			player->blood = 3000;
			player->deffence = 30;
			player->offence = 30;
			player->box[0] = 3;
			player->box[1] = 3;
			player->box[2] = 3;
			pro = NAME;
			first = 1;
		}
		if (mouse.y >= 240 && mouse.y <= 279) {
			player->blood = 2000;
			player->deffence = 20;
			player->offence = 20;
			player->box[0] = 2;
			player->box[1] = 2;
			player->box[2] = 2;
			pro = NAME;
			first = 1;
		}
		if (mouse.y >= 320 && mouse.y <= 359) {
			player->blood = 1000;
			player->deffence = 10;
			player->offence = 10;
			player->box[0] = 1;
			player->box[1] = 1;
			player->box[2] = 1;
			pro = NAME;
			first = 1;
		}
	}
}
void name() {
	static int first = 1;
	static int n = 0;
	static char *name = NULL;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\tower\\name.bmp");
		name = (char *)malloc(16 * sizeof(char));
		name[0] = '\0';
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
			pro = FIGURE;
			first = 1;
			return;
		case SG_ENTER:
			if (n) {
				strcpy(player->name, name);
				pro = PLAYING;
				first = 1;
				n = 0;
			}
			break;
		case SG_BACKS:
			if (n) name[--n] = '\0';
			break;
		default:
			if (n > 12)return;
			if ((key & 0x8000) == 0&&(key & 0x7FFF) > 31 && (key & 0x7FFF) < 128) {
				name[n++] = key;
				name[n] = '\0';
			}
			break;
		}
		setColor(99, 99, 99);
		putQuad(240, 240, 439, 259, SOLID_FILL);
		setColor(255, 255, 0);
		putString(name, 240, 240);
	}
}
void playing() {
	static int first = 1;
	int key;
	if (first) {
		loadBmp(0, 0, "Source\\tower\\tower.bmp");
		first = 0;
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				putBlock(j + 1, i + 1, wall[player->floor]->pos[i][j]);
			}
		}
		getImage(160 + player->x*SIZE, player->y*SIZE, 160 + player->x*SIZE + 39, player->y*SIZE + 39, player->oldImage);
		showPlayer(player->x, player->y, player->face);
		showMsg();
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC:
		case 'p':
			pro = PAUSE;
			first = 1;
			return;
		case SG_UP:
			if (player->y != 1) {
				if (up())first = 1;
			}
			else {
				player->face = 0;
				showPlayer(player->x, player->y, player->face);
			}
			break;
		case SG_DOWN:
			if (player->y != 11) {
				if (down())first = 1;
			}
			else {
				player->face = 1;
				showPlayer(player->x, player->y, player->face);
			}
			break;
		case SG_LEFT:
			if (player->x != 1) {
				if (left())first = 1;
			}
			else {
				player->face = 2;
				showPlayer(player->x, player->y, player->face);
			}
			break;
		case SG_RIGHT:
			if (player->x != 11) {
				if (right())first = 1;
			}
			else {
				player->face = 3;
				showPlayer(player->x, player->y, player->face);
			}
			break;
		case 's':
			pro = SAVE;
			first = 1;
			break;
		case 'u':
			if (player->box[4]&&player->floor < maxFloor) {
				player->floor++;
				player->x = wall[player->floor]->upx;
				player->y = wall[player->floor]->upy;
				first = 1;
			}
			break;
		case 'd':
			if (player->box[4] && player->floor) {
				player->floor--;
				player->x = wall[player->floor]->downx;
				player->y = wall[player->floor]->downy;
				first = 1;
			}
			break;
		case 'f':
			if (player->box[3]) {
				pro = DICTION;
				first = 1;
			}
		default:
			break;
		}
	}
}
void fight() {
	static int first = 1, fast = 0, monsb;
	if (first) {
		if (mons[obj - 11]->deffence >= player->offence) {
			pro = PLAYING;
			return;
		}
		monsb = mons[obj - 11]->blood;
		loadBmp(40, 40, "Source\\tower\\fighting.bmp");
		switch (obj) {
		case 11:
			loadBmp(100, 90, "Source\\tower\\block\\bblock11.bmp");
			break;
		case 12:
			loadBmp(100, 90, "Source\\tower\\block\\bblock12.bmp");
			break;
		case 13:
			loadBmp(100, 90, "Source\\tower\\block\\bblock13.bmp");
			break;
		case 14:
			loadBmp(100, 90, "Source\\tower\\block\\bblock14.bmp");
			break;
		case 15:
			loadBmp(100, 90, "Source\\tower\\block\\bblock15.bmp");
			break;
		case 16:
			loadBmp(100, 90, "Source\\tower\\block\\bblock16.bmp");
			break;
		case 17:
			loadBmp(100, 90, "Source\\tower\\block\\bblock17.bmp");
			break;
		case 18:
			loadBmp(100, 90, "Source\\tower\\block\\bblock18.bmp");
			break;
		case 19:
			loadBmp(100, 90, "Source\\tower\\block\\bblock19.bmp");
			break;
		case 20:
			loadBmp(100, 90, "Source\\tower\\block\\bblock20.bmp");
			break;
		case 21:
			loadBmp(100, 90, "Source\\tower\\block\\bblock21.bmp");
			break;
		case 22:
			loadBmp(100, 90, "Source\\tower\\block\\bblock22.bmp");
			break;
		case 23:
			loadBmp(100, 90, "Source\\tower\\block\\bblock23.bmp");
			break;
		case 24:
			loadBmp(100, 90, "Source\\tower\\block\\bblock24.bmp");
			break;
		case 25:
			loadBmp(100, 90, "Source\\tower\\block\\bblock25.bmp");
			break;
		case 26:
			loadBmp(100, 90, "Source\\tower\\block\\bblock26.bmp");
			break;
		case 27:
			loadBmp(100, 90, "Source\\tower\\block\\bblock27.bmp");
			break;
		case 28:
			loadBmp(100, 90, "Source\\tower\\block\\bblock28.bmp");
			break;
		case 29:
			loadBmp(100, 90, "Source\\tower\\block\\bblock29.bmp");
			break;
		case 30:
			loadBmp(100, 90, "Source\\tower\\block\\bblock30.bmp");
			break;
		case 31:
			loadBmp(100, 90, "Source\\tower\\block\\bblock31.bmp");
			break;
		case 32:
			loadBmp(100, 90, "Source\\tower\\block\\bblock32.bmp");
			break;
		case 33:
			loadBmp(100, 90, "Source\\tower\\block\\bblock33.bmp");
			break;
		case 34:
			loadBmp(100, 90, "Source\\tower\\block\\bblock34.bmp");
			break;
		case 35:
			loadBmp(100, 90, "Source\\tower\\block\\bblock35.bmp");
			break;
		case 36:
			loadBmp(100, 90, "Source\\tower\\block\\bblock36.bmp");
			break;
		case 37:
			loadBmp(100, 90, "Source\\tower\\block\\bblock37.bmp");
			break;
		case 38:
			loadBmp(100, 90, "Source\\tower\\block\\bblock38.bmp");
			break;
		case 39:
			loadBmp(100, 90, "Source\\tower\\block\\bblock39.bmp");
			break;
		case 40:
			loadBmp(100, 90, "Source\\tower\\block\\bblock40.bmp");
			break;
		case 41:
			loadBmp(100, 90, "Source\\tower\\block\\bblock41.bmp");
			break;
		case 42:
			loadBmp(100, 90, "Source\\tower\\block\\bblock42.bmp");
			break;
		case 43:
			loadBmp(100, 90, "Source\\tower\\block\\bblock43.bmp");
			break;
		case 44:
			loadBmp(100, 90, "Source\\tower\\block\\bblock44.bmp");
			break;
		case 45:
			loadBmp(100, 90, "Source\\tower\\block\\bblock45.bmp");
			break;
		case 46:
			loadBmp(100, 90, "Source\\tower\\block\\bblock46.bmp");
			break;
		case 47:
			loadBmp(100, 90, "Source\\tower\\block\\bblock47.bmp");
			break;
		case 48:
			loadBmp(100, 90, "Source\\tower\\block\\bblock48.bmp");
			break;
		case 49:
			loadBmp(100, 90, "Source\\tower\\block\\bblock49.bmp");
			break;
		case 50:
			loadBmp(100, 90, "Source\\tower\\block\\bblock50.bmp");
			break;
		case 51:
			loadBmp(100, 90, "Source\\tower\\block\\bblock51.bmp");
			break;
		case 52:
			loadBmp(100, 90, "Source\\tower\\block\\bblock52.bmp");
			break;
		}
		switch (player->outlook) {
		case 0:
			loadBmp(500, 90, "Source\\tower\\face\\bigface1.bmp");
			break;
		case 1:
			loadBmp(500, 90, "Source\\tower\\face\\bigface2.bmp");
			break;
		case 2:
			loadBmp(500, 90, "Source\\tower\\face\\bigface3.bmp");
			break;
		case 3:
			loadBmp(500, 90, "Source\\tower\\face\\bigface4.bmp");
			break;
		}
	}
	if (biosKey(1))biosKey(0);
	if (biosKey(1)) {
		biosKey(0);
		fast = 1;
	}
	delayBegin();
	drawNumbers(260, 200, monsb, 4, 1);
	drawNumbers(260, 240, mons[obj - 11]->deffence, 4, 1);
	drawNumbers(260, 280, mons[obj - 11]->offence, 4, 1);
	drawNumbers(480, 200, player->blood, 4, 1);
	drawNumbers(480, 240, player->deffence, 4, 1);
	drawNumbers(480, 280, player->offence, 4, 1);
	if (player->offence - mons[obj - 11]->deffence>0)
		monsb -= (player->offence - mons[obj - 11]->deffence);
	if (monsb<0) {
		monsb = 0;
	}
	if (mons[obj - 11]->offence - player->deffence>0)
		player->blood -= mons[obj - 11]->offence - player->deffence;
	if (player->blood<0) {
		player->blood = 0;
	}
	if (first) {
		first = 0;
		return;
	}
	if (fast)delayEnd(50);
	else delayEnd(400);
	if (player->blood == 0 || monsb == 0) {
		drawNumbers(260, 200, monsb, 4, 1);
		drawNumbers(480, 200, player->blood, 4, 1);
		delay(200);
		if (monsb == 0) {
			player->money += mons[obj - 11]->money;
			player->exp += mons[obj - 11]->exp;
		}
		if (player->blood == 0) {
			pro = DIE;
			first = 1;
			return;
		}
		pro = PLAYING;
		first = 1;
		fast = 0;
	}
}
void pause() {
	static int first = 1;
	int key;
	if (first) {
		loadBmp(80, 60, "Source\\tower\\pause.bmp");
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC | 0x8000:
			pro = PLAYING;
			first = 1;
			return;
		case SG_ENTER | 0x8000:
			pro = START;
			first = 1;
			return;
		default:
			break;
		}
	}
}
void buy() {
	static int first = 1;
	vecThree mouse;
	int key;
	if (first) {
		switch (obj) {
		case 61:
			loadBmp(240, 80, "Source\\tower\\shop1.bmp");
			break;
		case 62:
			loadBmp(240, 80, "Source\\tower\\shop2.bmp");
			break;
		case 63:
			loadBmp(240, 80, "Source\\tower\\shop3.bmp");
			break;
		case 64:
			loadBmp(240, 80, "Source\\tower\\shop4.bmp");
			break;
		case 65:
			loadBmp(240, 80, "Source\\tower\\shop5.bmp");
			break;
		case 66:
			loadBmp(240, 80, "Source\\tower\\shop6.bmp");
			break;
		case 67:
			loadBmp(240, 80, "Source\\tower\\shop7.bmp");
			break;
		}
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC | 0x8000:
			pro = PLAYING;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (mouse.x < 280 || mouse.x>399)return;
		if (mouse.y >= 200 && mouse.y <= 239) {
			switch (obj) {
			case 61:
				if (player->money >= 25) {
					player->money -= 25;
					player->blood += 800;
				}
				break;
			case 62:
				if (player->exp >= 40) {
					player->exp -= 40;
					player->level += 1;
					player->offence += 2;
					player->deffence += 2;
					player->blood += 200;
				}
				break;
			case 63:
				if (player->money >= 60) {
					player->money -= 60;
					player->box[0] += 3;
				}
				break;
			}
			showMsg();
		}
		if (mouse.y >= 260 && mouse.y <= 299) {
			switch (obj) {
			case 61:
				if (player->money >= 25) {
					player->money -= 25;
					player->offence += 4;
				}
				break;
			case 62:
				if (player->exp >= 40) {
					player->exp -= 40;
					player->offence += 5;
				}
				break;
			case 63:
				if (player->money >= 40) {
					player->money -= 40;
					player->box[1] += 1;
				}
				break;
			}
			showMsg();
		}
		if (mouse.y >= 320 && mouse.y <= 359) {
			switch (obj) {
			case 61:
				if (player->money >= 25) {
					player->money -= 25;
					player->deffence += 4;
				}
				break;
			case 62:
				if (player->exp >= 40) {
					player->exp -= 40;
					player->deffence += 5;
				}
				break;
			case 63:
				if (player->money >= 100) {
					player->money -= 100;
					player->deffence += 1;
				}
				break;
			}
			showMsg();
		}
	}
}
void die() {
	pro = START;
}
void save() {
	static int first = 1;
	vecThree mouse;
	int key;
	if (first) {
		loadBmp(100, 180, "Source\\tower\\cards.bmp");
		showArchive();
		first = 0;
	}
	if (biosKey(1)) {
		key = biosKey(0);
		switch (key) {
		case SG_ESC | 0x8000:
			pro = PLAYING;
			first = 1;
			return;
		default:
			break;
		}
	}
	if (biosMouse(1).m) {
		mouse = biosMouse(0);
		if (inRect(mouse.x, mouse.y, 120, 200, 280, 240)) {
			saveFile(1);
		}
		if (inRect(mouse.x, mouse.y, 120, 280, 280, 320)) {
			saveFile(2);
		}
		if (inRect(mouse.x, mouse.y, 120, 360, 280, 400)) {
			saveFile(3);
		}
		if (inRect(mouse.x, mouse.y, 400, 200, 560, 240)) {
			saveFile(4);
		}
		if (inRect(mouse.x, mouse.y, 400, 280, 560, 320)) {
			saveFile(5);
		}
		if (inRect(mouse.x, mouse.y, 400, 360, 560, 400)) {
			saveFile(6);
		}
		pro = PLAYING;
		first = 1;
	}
}
void diction() {
	static int first = 1;
	int stat[100] = { 0 }, count = 1;
	if (first) {
		loadBmp(200, 40, "Source\\tower\\allm.bmp");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				stat[wall[player->floor]->pos[i][j]] = 1;
			}
		}
		for (int i = 11; i <= 51; i++) {
			if (stat[i]) {
				putInfo(count, i);
				count++;
			}
		}
		first = 0;
	}
	if (biosKey(1)) {
		if (biosKey(0) < 0x8000)return;
		pro = PLAYING;
		first = 1;
	}
}

void initPro() {
	int i;
	for (i = 0; i <= 80; i++)wall[i] = (tower *)malloc(2 * sizeof(tower));
	for (i = 0; i <= 41; i++)mons[i] = (opponent *)malloc(2 * sizeof(opponent));
	player = (knight *)malloc(2 * sizeof(knight));
	player->oldImage = malloc(40 * 40 * 3 * sizeof(char));
	player->name = (char*)malloc(20 * sizeof(char));
	player->time = 0;
	player->exp = 0;
	player->floor = 0;
	player->level = 0;
	player->money = 0;
	player->outlook = 0;
	player->x = 0;
	player->y = 0;

	mons[0]->blood = 100;
	mons[0]->deffence = 5;
	mons[0]->offence = 15;
	mons[0]->money = 3;
	mons[0]->exp = 2;

	mons[1]->blood = 80;
	mons[1]->deffence = 8;
	mons[1]->offence = 20;
	mons[1]->money = 4;
	mons[1]->exp = 2;

	mons[2]->blood = 180;
	mons[2]->deffence = 14;
	mons[2]->offence = 28;
	mons[2]->money = 5;
	mons[2]->exp = 3;

	mons[3]->blood = 160;
	mons[3]->deffence = 14;
	mons[3]->offence = 40;
	mons[3]->money = 8;
	mons[3]->exp = 4;

	mons[4]->blood = 200;
	mons[4]->deffence = 24;
	mons[4]->offence = 36;
	mons[4]->money = 12;
	mons[4]->exp = 7;

	mons[5]->blood = 240;
	mons[5]->deffence = 28;
	mons[5]->offence = 48;
	mons[5]->money = 14;
	mons[5]->exp = 8;

	mons[6]->blood = 120;
	mons[6]->deffence = 44;
	mons[6]->offence = 80;
	mons[6]->money = 14;
	mons[6]->exp = 12;

	mons[7]->blood = 300;
	mons[7]->deffence = 32;
	mons[7]->offence = 96;
	mons[7]->money = 20;
	mons[7]->exp = 10;

	mons[8]->blood = 200;
	mons[8]->deffence = 42;
	mons[8]->offence = 120;
	mons[8]->money = 22;
	mons[8]->exp = 16;

	mons[9]->blood = 540;
	mons[9]->deffence = 72;
	mons[9]->offence = 108;
	mons[9]->money = 25;
	mons[9]->exp = 22;

	mons[10]->blood = 600;
	mons[10]->deffence = 80;
	mons[10]->offence = 120;
	mons[10]->money = 30;
	mons[10]->exp = 20;

	mons[11]->blood = 400;
	mons[11]->deffence = 80;
	mons[11]->offence = 160;
	mons[11]->money = 32;
	mons[11]->exp = 22;

	mons[12]->blood = 400;
	mons[12]->deffence = 96;
	mons[12]->offence = 180;
	mons[12]->money = 42;
	mons[12]->exp = 24;

	mons[13]->blood = 360;
	mons[13]->deffence = 120;
	mons[13]->offence = 176;
	mons[13]->money = 45;
	mons[13]->exp = 32;

	mons[14]->blood = 600;
	mons[14]->deffence = 144;
	mons[14]->offence = 192;
	mons[14]->money = 45;
	mons[14]->exp = 42;

	mons[15]->blood = 560;
	mons[15]->deffence = 288;
	mons[15]->offence = 216;
	mons[15]->money = 52;
	mons[15]->exp = 44;

	mons[16]->blood = 200;
	mons[16]->deffence = 128;
	mons[16]->offence = 360;
	mons[16]->money = 60;
	mons[16]->exp = 50;

	mons[17]->blood = 800;
	mons[17]->deffence = 192;
	mons[17]->offence = 360;
	mons[17]->money = 66;
	mons[17]->exp = 52;

	mons[18]->blood = 720;
	mons[18]->deffence = 396;
	mons[18]->offence = 320;
	mons[18]->money = 64;
	mons[18]->exp = 62;

	mons[19]->blood = 1000;
	mons[19]->deffence = 320;
	mons[19]->offence = 420;
	mons[19]->money = 80;
	mons[19]->exp = 72;

	mons[20]->blood = 1500;
	mons[20]->deffence = 300;
	mons[20]->offence = 600;
	mons[20]->money = 100;
	mons[20]->exp = 80;

	mons[21]->blood = 1000;
	mons[21]->deffence = 480;
	mons[21]->offence = 640;
	mons[21]->money = 108;
	mons[21]->exp = 96;

	mons[22]->blood = 800;
	mons[22]->deffence = 600;
	mons[22]->offence = 680;
	mons[22]->money = 136;
	mons[22]->exp = 90;

	mons[23]->blood = 2000;
	mons[23]->deffence = 720;
	mons[23]->offence = 800;
	mons[23]->money = 150;
	mons[23]->exp = 120;

	mons[24]->blood = 2800;
	mons[24]->deffence = 680;
	mons[24]->offence = 1080;
	mons[24]->money = 240;
	mons[24]->exp = 144;

	mons[25]->blood = 1800;
	mons[25]->deffence = 800;
	mons[25]->offence = 1200;
	mons[25]->money = 240;
	mons[25]->exp = 144;

	mons[26]->blood = 2000;
	mons[26]->deffence = 1000;
	mons[26]->offence = 1440;
	mons[26]->money = 240;
	mons[26]->exp = 144;

	mons[27]->blood = 2400;
	mons[27]->deffence = 1080;
	mons[27]->offence = 1600;
	mons[27]->money = 240;
	mons[27]->exp = 144;

	mons[28]->blood = 3600;
	mons[28]->deffence = 1440;
	mons[28]->offence = 1680;
	mons[28]->money = 240;
	mons[28]->exp = 144;

	mons[29]->blood = 3200;
	mons[29]->deffence = 1400;
	mons[29]->offence = 1920;
	mons[29]->money = 240;
	mons[29]->exp = 144;

	mons[30]->blood = 3000;
	mons[30]->deffence = 1800;
	mons[30]->offence = 2160;
	mons[30]->money = 240;
	mons[30]->exp = 144;

	mons[31]->blood = 2000;
	mons[31]->deffence = 2400;
	mons[31]->offence = 2000;
	mons[31]->money = 240;
	mons[31]->exp = 144;

	mons[32]->blood = 5000;
	mons[32]->deffence = 2000;
	mons[32]->offence = 2880;
	mons[32]->money = 240;
	mons[32]->exp = 144;

	mons[33]->blood = 4800;
	mons[33]->deffence = 2800;
	mons[33]->offence = 2600;
	mons[33]->money = 240;
	mons[33]->exp = 144;

	mons[34]->blood = 6000;
	mons[34]->deffence = 2400;
	mons[34]->offence = 3400;
	mons[34]->money = 240;
	mons[34]->exp = 144;

	mons[35]->blood = 4800;
	mons[35]->deffence = 3200;
	mons[35]->offence = 3200;
	mons[35]->money = 240;
	mons[35]->exp = 144;

	mons[36]->blood = 8000;
	mons[36]->deffence = 3000;
	mons[36]->offence = 4200;
	mons[36]->money = 240;
	mons[36]->exp = 144;

	mons[37]->blood = 7200;
	mons[37]->deffence = 3200;
	mons[37]->offence = 4840;
	mons[37]->money = 240;
	mons[37]->exp = 144;

	mons[38]->blood = 10000;
	mons[38]->deffence = 4000;
	mons[38]->offence = 4500;
	mons[38]->money = 240;
	mons[38]->exp = 144;

	mons[39]->blood = 12000;
	mons[39]->deffence = 3600;
	mons[39]->offence = 6000;
	mons[39]->money = 240;
	mons[39]->exp = 144;

	mons[40]->blood = 3600;
	mons[40]->deffence = 6000;
	mons[40]->offence = 5200;
	mons[40]->money = 240;
	mons[40]->exp = 144;

	mons[41]->blood = 24000;
	mons[41]->deffence = 8000;
	mons[41]->offence = 10000;
	mons[41]->money = 240;
	mons[41]->exp = 144;
}
void drawNumber(int num, int x, int y, int bs) {
	switch (num) {
	case -1:
		if(bs)loadBmp(x, y, "Source\\tower\\number\\empty.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\sempty.bmp");
		break;
	case 0:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\00.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s00.bmp");
		break;
	case 1:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\01.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s01.bmp");
		break;
	case 2:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\02.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s02.bmp");
		break;
	case 3:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\03.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s03.bmp");
		break;
	case 4:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\04.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s04.bmp");
		break;
	case 5:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\05.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s05.bmp");
		break;
	case 6:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\06.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s06.bmp");
		break;
	case 7:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\07.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s07.bmp");
		break;
	case 8:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\08.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s08.bmp");
		break;
	case 9:
		if (bs)loadBmp(x, y, "Source\\tower\\number\\09.bmp");
		else loadBmp(x, y, "Source\\tower\\number\\s09.bmp");
		break;
	}
}
void drawNumbers(int rx, int ry, int temp, int digit, int bs) {
	int i, d, max = 1;
	for (i = 0; i < digit; i++)max *= 10;
	temp %= max;
	for (i = 0; i < digit; i++) {
		d = temp % 10;
		temp /= 10;
		drawNumber(d, rx - (i + 1)*(bs ? 20 : 10), ry, bs);
		if (temp == 0)break;
	}
	while (++i < digit) {
		drawNumber(-1, rx - (i + 1)*(bs ? 20 : 10), ry, bs);
	}
}
void showArchive() {
	FILE *fp;
	char *name;
	int time, floor;
	name = (char *)malloc(20 * sizeof(char));
	setColor(255, 255, 255);
	fp = fopen("Source\\tower\\archive\\save1.tow", "r");
	if (fp != NULL) {
		fgets(name, 20, fp);
		name[strlen(name) - 1] = '\0';
		fscanf(fp, "%d", &time);
		fscanf(fp, "%d", &floor);
		fclose(fp);
		putString(name, 160 + 10, 204);
		putString("time:", 160 + 10, 220);
		putString("floor:", 160 + 10, 236);
		time = (time - 1451620770) % 86400;
		putNumber(time / 3600, 242, 220, 'r');
		putChar(':', 242, 220);
		putNumber((time % 3600) / 60, 252, 220, 'l');
		putNumber(floor, 268, 236, 'r');
	}
	fp = fopen("Source\\tower\\archive\\save2.tow", "r");
	if (fp != NULL) {
		fgets(name, 20, fp);
		name[strlen(name) - 1] = '\0';
		fscanf(fp, "%d", &time);
		fscanf(fp, "%d", &floor);
		fclose(fp);
		putString(name, 160 + 10, 284);
		putString("time:", 160 + 10, 300);
		putString("floor:", 160 + 10, 316);
		time = (time - 1451620770) % 86400;
		putNumber(time / 3600, 242, 300, 'r');
		putChar(':', 242, 300);
		putNumber((time % 3600) / 60, 252, 300, 'l');
		putNumber(floor, 268, 316, 'r');
	}
	fp = fopen("Source\\tower\\archive\\save3.tow", "r");
	if (fp != NULL) {
		fgets(name, 20, fp);
		name[strlen(name) - 1] = '\0';
		fscanf(fp, "%d", &time);
		fscanf(fp, "%d", &floor);
		fclose(fp);
		putString(name, 160 + 10, 364);
		putString("time:", 160 + 10, 380);
		putString("floor:", 160 + 10, 396);
		time = (time - 1451620770) % 86400;
		putNumber(time / 3600, 242, 380, 'r');
		putChar(':', 242, 380);
		putNumber((time % 3600) / 60, 252, 380, 'l');
		putNumber(floor, 268, 396, 'r');
	}
	fp = fopen("Source\\tower\\archive\\save4.tow", "r");
	if (fp != NULL) {
		fgets(name, 20, fp);
		name[strlen(name) - 1] = '\0';
		fscanf(fp, "%d", &time);
		fscanf(fp, "%d", &floor);
		fclose(fp);
		putString(name, 440 + 10, 204);
		putString("time:", 440 + 10, 220);
		putString("floor:", 440 + 10, 236);
		time = (time - 1451620770) % 86400;
		putNumber(time / 3600, 522, 220, 'r');
		putChar(':', 522, 220);
		putNumber((time % 3600) / 60, 532, 220, 'l');
		putNumber(floor, 546, 236, 'r');
	}
	fp = fopen("Source\\tower\\archive\\save5.tow", "r");
	if (fp != NULL) {
		fgets(name, 20, fp);
		name[strlen(name) - 1] = '\0';
		fscanf(fp, "%d", &time);
		fscanf(fp, "%d", &floor);
		fclose(fp);
		putString(name, 440 + 10, 284);
		putString("time:", 440 + 10, 300);
		putString("floor:", 440 + 10, 316);
		time = (time - 1451620770) % 86400;
		putNumber(time / 3600, 522, 300, 'r');
		putChar(':', 522, 300);
		putNumber((time % 3600) / 60, 532, 300, 'l');
		putNumber(floor, 546, 316, 'r');
	}
	fp = fopen("Source\\tower\\archive\\save6.tow", "r");
	if (fp != NULL) {
		fgets(name, 20, fp);
		name[strlen(name) - 1] = '\0';
		fscanf(fp, "%d", &time);
		fscanf(fp, "%d", &floor);
		fclose(fp);
		putString(name, 440 + 10, 364);
		putString("time:", 440 + 10, 380);
		putString("floor:", 440 + 10, 396);
		time = (time - 1451620770) % 86400;
		putNumber(time / 3600, 522, 380, 'r');
		putChar(':', 522, 380);
		putNumber((time % 3600) / 60, 532, 380, 'l');
		putNumber(floor, 546, 396, 'r');
	}
}
int loadFile(int card) {
	FILE *fp = NULL;
	int i, j, k;
	switch (card) {
	case 0:
		fp = fopen("Source\\tower\\archive\\save0.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 1:
		fp = fopen("Source\\tower\\archive\\save1.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 2:
		fp = fopen("Source\\tower\\archive\\save2.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 3:
		fp = fopen("Source\\tower\\archive\\save3.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 4:
		fp = fopen("Source\\tower\\archive\\save4.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 5:
		fp = fopen("Source\\tower\\archive\\save5.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 6:
		fp = fopen("Source\\tower\\archive\\save6.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	}
	fgets(player->name, 20, fp);
	player->name[strlen(player->name) - 1] = '\0';
	fscanf(fp, "%d", &player->time);
	fscanf(fp, "%d", &player->floor);
	fscanf(fp, "%d", &player->deffence);
	fscanf(fp, "%d", &player->exp);
	fscanf(fp, "%d", &player->blood);
	fscanf(fp, "%d", &player->level);
	fscanf(fp, "%d", &player->money);
	fscanf(fp, "%d", &player->offence);
	fscanf(fp, "%d", &player->outlook);
	fscanf(fp, "%d", &player->x);
	fscanf(fp, "%d", &player->y);
	fscanf(fp, "%d", &player->face);
	for (i = 0; i < 100; i++) {
		fscanf(fp, "%d", &player->box[i]);
	}
	fclose(fp);

	switch (card) {
	case 0:
		fp = fopen("Source\\tower\\archive\\build0.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 1:
		fp = fopen("Source\\tower\\archive\\build1.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 2:
		fp = fopen("Source\\tower\\archive\\build2.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 3:
		fp = fopen("Source\\tower\\archive\\build3.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 4:
		fp = fopen("Source\\tower\\archive\\build4.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 5:
		fp = fopen("Source\\tower\\archive\\build5.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	case 6:
		fp = fopen("Source\\tower\\archive\\build6.tow", "r");
		if (fp == NULL)
			return 0;
		break;
	}
	for (i = 0; i <= 80; i++) {
		for (j = 0; j < 11; j++) {
			for (k = 0; k < 11; k++) {
				fscanf(fp, "%d", &wall[i]->pos[j][k]);
			}
		}
		fscanf(fp, "%d", &wall[i]->upstairx);
		fscanf(fp, "%d", &wall[i]->upstairy);
		fscanf(fp, "%d", &wall[i]->downstairx);
		fscanf(fp, "%d", &wall[i]->downstairy);
		fscanf(fp, "%d", &wall[i]->upx);
		fscanf(fp, "%d", &wall[i]->upy);
		fscanf(fp, "%d", &wall[i]->downx);
		fscanf(fp, "%d", &wall[i]->downy);
	}
	fclose(fp);
	return card;
}
void saveFile(int card) {
	FILE *fp = NULL;
	int i, j, k;
	time_t t;
	switch (card) {
	case 1:
		fp = fopen("Source\\tower\\archive\\save1.tow", "w+");
		break;
	case 2:
		fp = fopen("Source\\tower\\archive\\save2.tow", "w+");
		break;
	case 3:
		fp = fopen("Source\\tower\\archive\\save3.tow", "w+");
		break;
	case 4:
		fp = fopen("Source\\tower\\archive\\save4.tow", "w+");
		break;
	case 5:
		fp = fopen("Source\\tower\\archive\\save5.tow", "w+");
		break;
	case 6:
		fp = fopen("Source\\tower\\archive\\save6.tow", "w+");
		break;
	}
	fputs(player->name, fp);
	fprintf(fp, "\n");
	time(&t);
	fprintf(fp, "%d\n", (int)t);
	fprintf(fp, "%d\n", player->floor);
	fprintf(fp, "%d\n", player->deffence);
	fprintf(fp, "%d\n", player->exp);
	fprintf(fp, "%d\n", player->blood);
	fprintf(fp, "%d\n", player->level);
	fprintf(fp, "%d\n", player->money);
	fprintf(fp, "%d\n", player->offence);
	fprintf(fp, "%d\n", player->outlook);
	fprintf(fp, "%d\n", player->x);
	fprintf(fp, "%d\n", player->y);
	fprintf(fp, "%d\n", player->face);
	for (i = 0; i < 100; i++) {
		fprintf(fp, "%d\n", player->box[i]);
	}
	fclose(fp);
	
	switch (card) {
	case 1:
		fp = fopen("Source\\tower\\archive\\build1.tow", "w+");
		break;
	case 2:
		fp = fopen("Source\\tower\\archive\\build2.tow", "w+");
		break;
	case 3:
		fp = fopen("Source\\tower\\archive\\build3.tow", "w+");
		break;
	case 4:
		fp = fopen("Source\\tower\\archive\\build4.tow", "w+");
		break;
	case 5:
		fp = fopen("Source\\tower\\archive\\build5.tow", "w+");
		break;
	case 6:
		fp = fopen("Source\\tower\\archive\\build6.tow", "w+");
		break;
	}
	for (i = 0; i <= 80; i++) {
		for (j = 0; j < 11; j++) {
			for (k = 0; k < 11; k++) {
				fprintf(fp, "%d\n", wall[i]->pos[j][k]);
			}
		}
		fprintf(fp, "%d\n", wall[i]->upstairx);
		fprintf(fp, "%d\n", wall[i]->upstairy);
		fprintf(fp, "%d\n", wall[i]->downstairx);
		fprintf(fp, "%d\n", wall[i]->downstairy);
		fprintf(fp, "%d\n", wall[i]->upx);
		fprintf(fp, "%d\n", wall[i]->upy);
		fprintf(fp, "%d\n", wall[i]->downx);
		fprintf(fp, "%d\n", wall[i]->downy);
	}
	fclose(fp);
}
int up() {
	switch (wall[player->floor]->pos[player->y - 2][player->x - 1]) {
	case 0:
		putBlock(player->x, player->y--, 0);
		break;
	case 1:
		break;
	case 2:
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 3:
		break;
	case 4:
		if (player->box[0]) {
			wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
			player->box[0]--;
		}
		break;
	case 5:
		if (player->box[1]) {
			wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
			player->box[1]--;
		}
		break;
	case 6:
		if (player->box[2]) {
			wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
			player->box[2]--;
		}
		break;
	case 7:
	case 8:
		break;
	case 9:
		if (player->floor == maxFloor)
			maxFloor++;
		player->floor++;
		player->x = wall[player->floor]->upx;
		player->y = wall[player->floor]->upy;
		return 1;
	case 10:
		player->floor--;
		player->x = wall[player->floor]->downx;
		player->y = wall[player->floor]->downy;
		return 1;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		pro = FIGHT;
		obj = wall[player->floor]->pos[player->y - 2][player->x - 1];
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		clearKeyBuffer();
		return 1;
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
		pro = BUY;
		obj = wall[player->floor]->pos[player->y - 2][player->x - 1];
		return 1;
	case 68:
	case 69:
	case 70:
		break;
	case 71:
		player->offence += 3;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 72:
		player->offence += 5;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 73:
		player->offence += 10;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 74:
		player->deffence += 3;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 75:
		player->deffence += 5;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 76:
		player->deffence += 10;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 77:
		player->blood += 200;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 78:
		player->blood += 500;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 79:
		player->blood += 1000;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 80:
		break;
	case 81:
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		putBlock(player->x, player->y - 1, 0);
		//response();
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 82;
		putBlock(player->x - 1, player->y, 81);
		break;
	case 82:
		break;
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
		break;
	case 91:
		player->box[0]++;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 92:
		player->box[1]++;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 93:
		player->box[2]++;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 94:
		player->box[3]++;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 95:
		player->box[4]++;
		wall[player->floor]->pos[player->y - 2][player->x - 1] = 0;
		break;
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
		break;
	}
	if (!wall[player->floor]->pos[player->y - 2][player->x - 1] && player->y > 1) {
		putBlock(player->x, player->y - 1, 0);
		showMsg();
	}
	showPlayer(player->x, player->y, 0);
	return 0;
}
int down() {
	switch (wall[player->floor]->pos[player->y][player->x - 1]) {
	case 0:
		putBlock(player->x, player->y++, 0);
		break;
	case 1:
		break;
	case 2:
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 3:
		break;
	case 4:
		if (player->box[0]) {
			wall[player->floor]->pos[player->y][player->x - 1] = 0;
			player->box[0]--;
		}
		break;
	case 5:
		if (player->box[1]) {
			wall[player->floor]->pos[player->y][player->x - 1] = 0;
			player->box[1]--;
		}
		break;
	case 6:
		if (player->box[2]) {
			wall[player->floor]->pos[player->y][player->x - 1] = 0;
			player->box[2]--;
		}
		break;
	case 7:
	case 8:
		break;
	case 9:
		if (player->floor == maxFloor)
			maxFloor++;
		player->floor++;
		player->x = wall[player->floor]->upx;
		player->y = wall[player->floor]->upy;
		return 1;
	case 10:
		player->floor--;
		player->x = wall[player->floor]->downx;
		player->y = wall[player->floor]->downy;
		return 1;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		pro = FIGHT;
		obj = wall[player->floor]->pos[player->y][player->x - 1];
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		clearKeyBuffer();
		return 1;
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
		pro = BUY;
		obj = wall[player->floor]->pos[player->y][player->x - 1];
		return 1;
	case 68:
	case 69:
	case 70:
		break;
	case 71:
		player->offence += 3;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 72:
		player->offence += 5;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 73:
		player->offence += 10;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 74:
		player->deffence += 3;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 75:
		player->deffence += 5;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 76:
		player->deffence += 10;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 77:
		player->blood += 200;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 78:
		player->blood += 500;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 79:
		player->blood += 1000;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
		break;
	case 91:
		player->box[0]++;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 92:
		player->box[1]++;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 93:
		player->box[2]++;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 94:
		player->box[3]++;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 95:
		player->box[4]++;
		wall[player->floor]->pos[player->y][player->x - 1] = 0;
		break;
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
		break;
	}
	if (!wall[player->floor]->pos[player->y][player->x - 1] && player->y < 11) {
		putBlock(player->x, player->y + 1, 0);
		showMsg();
	}
	showPlayer(player->x, player->y, 1);
	return 0;
}
int left() {
	switch (wall[player->floor]->pos[player->y - 1][player->x - 2]) {
	case 0:
		putBlock(player->x--, player->y, 0);
		break;
	case 1:
		break;
	case 2:
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 3:
		break;
	case 4:
		if (player->box[0]) {
			wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
			player->box[0]--;
		}
		break;
	case 5:
		if (player->box[1]) {
			wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
			player->box[1]--;
		}
		break;
	case 6:
		if (player->box[2]) {
			wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
			player->box[2]--;
		}
		break;
	case 7:
	case 8:
		break;
	case 9:
		if (player->floor == maxFloor)
			maxFloor++;
		player->floor++;
		player->x = wall[player->floor]->upx;
		player->y = wall[player->floor]->upy;
		return 1;
	case 10:
		player->floor--;
		player->x = wall[player->floor]->downx;
		player->y = wall[player->floor]->downy;
		return 1;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		pro = FIGHT;
		obj = wall[player->floor]->pos[player->y - 1][player->x - 2];
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		clearKeyBuffer();
		return 1;
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
		pro = BUY;
		obj = wall[player->floor]->pos[player->y - 1][player->x - 2];
		return 1;
	case 68:
	case 69:
	case 70:
		break;
	case 71:
		player->offence += 3;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 72:
		player->offence += 5;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 73:
		player->offence += 10;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 74:
		player->deffence += 3;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 75:
		player->deffence += 5;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 76:
		player->deffence += 10;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 77:
		player->blood += 200;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 78:
		player->blood += 500;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 79:
		player->blood += 1000;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
		break;
	case 91:
		player->box[0]++;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 92:
		player->box[1]++;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 93:
		player->box[2]++;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 94:
		player->box[3]++;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 95:
		player->box[4]++;
		wall[player->floor]->pos[player->y - 1][player->x - 2] = 0;
		break;
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
		break;
	}
	if (!wall[player->floor]->pos[player->y - 1][player->x - 2] && player->x > 1) {
		putBlock(player->x - 1, player->y, 0);
		showMsg();
	}
	showPlayer(player->x, player->y, 2);
	return 0;
}
int right() {
	switch (wall[player->floor]->pos[player->y - 1][player->x]) {
	case 0:
		putBlock(player->x++, player->y, 0);
		break;
	case 1:
		break;
	case 2:
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 3:
		break;
	case 4:
		if (player->box[0]) {
			wall[player->floor]->pos[player->y - 1][player->x] = 0;
			player->box[0]--;
		}
		break;
	case 5:
		if (player->box[1]) {
			wall[player->floor]->pos[player->y - 1][player->x] = 0;
			player->box[1]--;
		}
		break;
	case 6:
		if (player->box[2]) {
			wall[player->floor]->pos[player->y - 1][player->x] = 0;
			player->box[2]--;
		}
		break;
	case 7:
	case 8:
		break;
	case 9:
		if (player->floor == maxFloor)
			maxFloor++;
		player->floor++;
		player->x = wall[player->floor]->upx;
		player->y = wall[player->floor]->upy;
		return 1;
	case 10:
		player->floor--;
		player->x = wall[player->floor]->downx;
		player->y = wall[player->floor]->downy;
		return 1;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		pro = FIGHT;
		obj = wall[player->floor]->pos[player->y - 1][player->x];
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		clearKeyBuffer();
		return 1;
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		break;
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
		pro = BUY;
		obj = wall[player->floor]->pos[player->y - 1][player->x];
		return 1;
	case 68:
	case 69:
	case 70:
		break;
	case 71:
		player->offence += 3;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 72:
		player->offence += 5;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 73:
		player->offence += 10;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 74:
		player->deffence += 3;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 75:
		player->deffence += 5;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 76:
		player->deffence += 10;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 77:
		player->blood += 200;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 78:
		player->blood += 500;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 79:
		player->blood += 1000;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
		break;
	case 91:
		player->box[0]++;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 92:
		player->box[1]++;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 93:
		player->box[2]++;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 94:
		player->box[3]++;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 95:
		player->box[4]++;
		wall[player->floor]->pos[player->y - 1][player->x] = 0;
		break;
	case 96:
	case 97:
	case 98:
	case 99:
	case 100:
		break;
	}
	if (!wall[player->floor]->pos[player->y - 1][player->x] && player->x < 11) {
		putBlock(player->x + 1, player->y, 0);
		showMsg();
	}
	showPlayer(player->x, player->y, 3);
	return 0;
}
void putBlock(int x, int y, int style) {
	switch (style) {
	case 0:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block0.bmp");
		break;
	case 1:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block1.bmp");
		break;
	case 2:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block2.bmp");
		break;
	case 3:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block3.bmp");
		break;
	case 4:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block4.bmp");
		break;
	case 5:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block5.bmp");
		break;
	case 6:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block6.bmp");
		break;
	case 7:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block7.bmp");
		break;
	case 8:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block8.bmp");
		break;
	case 9:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block9.bmp");
		break;
	case 10:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block10.bmp");
		break;
	case 11:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block11.bmp");
		break;
	case 12:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block12.bmp");
		break;
	case 13:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block13.bmp");
		break;
	case 14:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block14.bmp");
		break;
	case 15:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block15.bmp");
		break;
	case 16:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block16.bmp");
		break;
	case 17:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block17.bmp");
		break;
	case 18:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block18.bmp");
		break;
	case 19:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block19.bmp");
		break;
	case 20:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block20.bmp");
		break;
	case 21:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block21.bmp");
		break;
	case 22:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block22.bmp");
		break;
	case 23:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block23.bmp");
		break;
	case 24:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block24.bmp");
		break;
	case 25:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block25.bmp");
		break;
	case 26:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block26.bmp");
		break;
	case 27:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block27.bmp");
		break;
	case 28:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block28.bmp");
		break;
	case 29:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block29.bmp");
		break;
	case 30:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block30.bmp");
		break;
	case 31:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block31.bmp");
		break;
	case 32:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block32.bmp");
		break;
	case 33:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block33.bmp");
		break;
	case 34:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block34.bmp");
		break;
	case 35:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block35.bmp");
		break;
	case 36:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block36.bmp");
		break;
	case 37:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block37.bmp");
		break;
	case 38:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block38.bmp");
		break;
	case 39:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block39.bmp");
		break;
	case 40:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block40.bmp");
		break;
	case 41:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block41.bmp");
		break;
	case 42:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block42.bmp");
		break;
	case 43:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block43.bmp");
		break;
	case 44:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block44.bmp");
		break;
	case 45:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block45.bmp");
		break;
	case 46:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block46.bmp");
		break;
	case 47:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block47.bmp");
		break;
	case 48:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block48.bmp");
		break;
	case 49:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block49.bmp");
		break;
	case 50:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block50.bmp");
		break;
	case 51:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block51.bmp");
		break;
	case 52:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block52.bmp");
		break;
	case 53:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block53.bmp");
		break;
	case 54:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block54.bmp");
		break;
	case 55:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block55.bmp");
		break;
	case 56:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block56.bmp");
		break;
	case 57:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block57.bmp");
		break;
	case 58:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block58.bmp");
		break;
	case 59:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block59.bmp");
		break;
	case 60:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block60.bmp");
		break;
	case 61:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block61.bmp");
		break;
	case 62:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block62.bmp");
		break;
	case 63:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block63.bmp");
		break;
	case 64:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block64.bmp");
		break;
	case 65:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block65.bmp");
		break;
	case 66:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block66.bmp");
		break;
	case 67:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block67.bmp");
		break;
	case 68:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block68.bmp");
		break;
	case 69:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block69.bmp");
		break;
	case 70:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block70.bmp");
		break;
	case 71:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block71.bmp");
		break;
	case 72:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block72.bmp");
		break;
	case 73:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block73.bmp");
		break;
	case 74:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block74.bmp");
		break;
	case 75:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block75.bmp");
		break;
	case 76:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block76.bmp");
		break;
	case 77:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block77.bmp");
		break;
	case 78:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block78.bmp");
		break;
	case 79:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block79.bmp");
		break;
	case 80:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block80.bmp");
		break;
	case 81:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block81.bmp");
		break;
	case 82:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block81.bmp");
		break;
	case 83:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block83.bmp");
		break;
	case 84:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block84.bmp");
		break;
	case 85:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block85.bmp");
		break;
	case 86:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block86.bmp");
		break;
	case 87:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block87.bmp");
		break;
	case 88:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block88.bmp");
		break;
	case 89:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block89.bmp");
		break;
	case 90:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block90.bmp");
		break;
	case 91:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block91.bmp");
		break;
	case 92:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block92.bmp");
		break;
	case 93:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block93.bmp");
		break;
	case 94:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block94.bmp");
		break;
	case 95:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block95.bmp");
		break;
	case 96:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block96.bmp");
		break;
	case 97:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block97.bmp");
		break;
	case 98:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block98.bmp");
		break;
	case 99:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block99.bmp");
		break;
	case 100:
		loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\block\\block100.bmp");
		break;

	}
}
void showPlayer(int x, int y, int face) {
	switch (player->outlook) {
	case 0:
		switch (face) {
		case 0:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c1f1.bmp");
			break;
		case 1:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c1f2.bmp");
			break;
		case 2:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c1f3.bmp");
			break;
		case 3:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c1f4.bmp");
			break;
		}
		break;
	case 1:
		switch (face) {
		case 0:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c2f1.bmp");
			break;
		case 1:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c2f2.bmp");
			break;
		case 2:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c2f3.bmp");
			break;
		case 3:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c2f4.bmp");
			break;
		}
		break;
	case 2:
		switch (face) {
		case 0:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c3f1.bmp");
			break;
		case 1:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c3f2.bmp");
			break;
		case 2:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c3f3.bmp");
			break;
		case 3:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c3f4.bmp");
			break;
		}
		break;
	case 3:
		switch (face) {
		case 0:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c4f1.bmp");
			break;
		case 1:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c4f2.bmp");
			break;
		case 2:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c4f3.bmp");
			break;
		case 3:
			loadBmp(160 + x*SIZE, y*SIZE, "Source\\tower\\face\\c4f4.bmp");
			break;
		}
		break;
	}

}
void showMsg() {
	putString(player->name, 98 - strlen(player->name) * 5, 24);
	drawNumbers(156, 80, player->blood, 5, 1);
	drawNumbers(156, 120, player->money, 4, 1);
	drawNumbers(156, 160, player->level, 4, 1);
	drawNumbers(156, 200, player->exp, 4, 1);
	drawNumbers(156, 240, player->deffence, 4, 1);
	drawNumbers(156, 280, player->offence, 4, 1);
	drawNumbers(156, 370, player->box[0], 3, 1);
	drawNumbers(156, 405, player->box[1], 3, 1);
	drawNumbers(156, 440, player->box[2], 3, 1);
	drawNumbers(140, 342, player->floor, 2, 0);
}
int response() {
	bitMap *oldImage = (bitMap*)malloc(280 * 200 * 3 * sizeof(char));
	getImage(200, 160, 479, 359, oldImage);
	loadBmp(200, 160, "Source\\tower\\response.bmp");
	delay(5000);
	putImage(200, 160, oldImage, 0);
	free(oldImage);
	return 0;
}
void putInfo(int count, int style) {
	int de = 0;
	int de1, de2;
	switch (style) {
	case 11:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block11.bmp");
		break;
	case 12:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block12.bmp");
		break;
	case 13:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block13.bmp");
		break;
	case 14:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block14.bmp");
		break;
	case 15:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block15.bmp");
		break;
	case 16:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block16.bmp");
		break;
	case 17:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block17.bmp");
		break;
	case 18:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block18.bmp");
		break;
	case 19:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block19.bmp");
		break;
	case 20:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block20.bmp");
		break;
	case 21:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block21.bmp");
		break;
	case 22:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block22.bmp");
		break;
	case 23:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block23.bmp");
		break;
	case 24:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block24.bmp");
		break;
	case 25:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block25.bmp");
		break;
	case 26:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block26.bmp");
		break;
	case 27:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block27.bmp");
		break;
	case 28:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block28.bmp");
		break;
	case 29:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block29.bmp");
		break;
	case 30:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block30.bmp");
		break;
	case 31:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block31.bmp");
		break;
	case 32:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block32.bmp");
		break;
	case 33:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block33.bmp");
		break;
	case 34:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block34.bmp");
		break;
	case 35:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block35.bmp");
		break;
	case 36:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block36.bmp");
		break;
	case 37:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block37.bmp");
		break;
	case 38:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block38.bmp");
		break;
	case 39:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block39.bmp");
		break;
	case 40:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block40.bmp");
		break;
	case 41:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block41.bmp");
		break;
	case 42:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block42.bmp");
		break;
	case 43:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block43.bmp");
		break;
	case 44:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block44.bmp");
		break;
	case 45:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block45.bmp");
		break;
	case 46:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block46.bmp");
		break;
	case 47:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block47.bmp");
		break;
	case 48:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block48.bmp");
		break;
	case 49:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block49.bmp");
		break;
	case 50:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block50.bmp");
		break;
	case 51:
		loadBmp(240, 40 + count*SIZE, "Source\\tower\\block\\block51.bmp");
		break;

	}
	drawNumbers(340, 50 + count*SIZE, mons[style - 11]->blood, 6, 0);
	drawNumbers(390, 50 + count*SIZE, mons[style - 11]->offence, 6, 0);
	drawNumbers(440, 50 + count*SIZE, mons[style - 11]->deffence, 6, 0);
	drawNumbers(490, 50 + count*SIZE, mons[style - 11]->money, 6, 0);
	drawNumbers(540, 50 + count*SIZE, mons[style - 11]->exp, 6, 0);

	de1 = mons[style - 11]->offence - player->deffence;
	de2 = player->offence - mons[style - 11]->deffence;
	if (de2>0) {
		if (de1<0)
			de1 = 0;
		if (mons[style - 11]->blood%de2)
			de = de1*(mons[style - 11]->blood / de2 + 1);
		else
			de = de1*(mons[style - 11]->blood / de2);

		drawNumbers(610, 50 + count*SIZE, de, 6, 0);
	}
	else
		drawNumbers(610, 50 + count*SIZE, 999999, 6, 0);
}

