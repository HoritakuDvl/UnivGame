#include "common.h"
#include <math.h>

static int shot_search(int n) {
	for (int j = 0; j < SHOT_BULLET_MAX; j++) {
		if (pla_shot[n].bullet[j].flag == 0) {
			return j;
		}
	}
	return -1;
}

void PlayerShotPattern0(int n) {
	int k;
	if ((k = shot_search(n)) != -1) {
		pla_shot[n].bullet[k].flag = 1;
		pla_shot[n].bullet[k].ang = player[pla_shot[n].num].ang; //’e‚ÌŠp“x
		pla_shot[n].bullet[k].spd = 5; //‘¬“x

		switch (player[pla_shot[n].num].knd) { //‹@Ží‚²‚Æ‚Ì‰ŠúˆÊ’u
		case 1:
			pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + player[pla_shot[n].num].w / 2;
			pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty;
			break;
		case 2:
			pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + 5 + 50 * cos(pla_shot[n].bullet[k].ang);
			pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty - 15 + 50 * sin(pla_shot[n].bullet[k].ang);
			break;
		}

		pla_shot[n].bullet[k].knd = 0;
		pla_shot[n].bullet[k].w = 20;
		pla_shot[n].bullet[k].h = 14;
		pla_shot[n].bullet[k].x = pla_shot[n].bullet[k].tx - pla_shot[n].bullet[k].w / 2;
		pla_shot[n].bullet[k].y = pla_shot[n].bullet[k].ty - pla_shot[n].bullet[k].h / 2;

		pla_shot[n].cnt = 30; //’e‘Å‚¿‚ÌŠÔŠu

	}
}

void PlayerShotPattern1(int n) {
	int k;
	if ((k = shot_search(n)) != -1) {
		pla_shot[n].bullet[k].flag = 1;
		pla_shot[n].bullet[k].ang = player[pla_shot[n].num].ang; //’e‚ÌŠp“x
		pla_shot[n].bullet[k].spd = 10; //‘¬“x

		switch (player[pla_shot[n].num].knd) { //‹@Ží‚²‚Æ‚Ì‰ŠúˆÊ’u
		case 1:
			pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + player[pla_shot[n].num].w / 2;
			pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty;
			break;
		case 2:
			pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + 5 + 50 * cos(pla_shot[n].bullet[k].ang);
			pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty - 15 + 50 * sin(pla_shot[n].bullet[k].ang);
			break;
		}

		pla_shot[n].bullet[k].knd = 0;
		pla_shot[n].bullet[k].w = 20;
		pla_shot[n].bullet[k].h = 14;
		pla_shot[n].bullet[k].x = pla_shot[n].bullet[k].tx - pla_shot[n].bullet[k].w / 2;
		pla_shot[n].bullet[k].y = pla_shot[n].bullet[k].ty - pla_shot[n].bullet[k].h / 2;

		pla_shot[n].cnt = 60; //’e‘Å‚¿‚ÌŠÔŠu

	}
}