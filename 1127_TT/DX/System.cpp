#include "common.h"


int PlayerEnemyHitJudge(PlayerData a, EnemyData b) {
	int ra = (a.w < a.h) ? a.w / 2 : a.h / 2;
	int rb = (b.w < b.h) ? b.w / 2 : b.h / 2;
	int xx = a.tx - b.tx;
	int yy = a.ty - b.ty;

	if (xx*xx + yy*yy <= (ra + rb) * (ra + rb))
		return 1;
	return 0;
}


int PTamaEnemyHitJudge(Bullet a, EnemyData x) {
	int ra = (a.w < a.h) ? a.w / 2 : a.h / 2;
	int rx = (x.w < x.h) ? x.w / 2-10 : x.h / 2-10;
	int xx = a.tx-x.tx;
	int yy = a.ty-x.ty;

	//DrawCircle(a.tx, a.ty, ra, GetColor(0, 255, 0), FALSE);
	//DrawCircle(x.tx, x.ty, rx, GetColor(0, 255, 0), FALSE);

	if (xx*xx + yy*yy <= (ra+rx) * (ra+rx))
		return 1;
	return 0;
}


int ETamaPlayerHitJjudge(Bullet a, PlayerData x) {
	int ra = (a.w < a.h) ? a.w / 2 : a.h / 2;
	int rx = (x.w < x.h) ? x.w / 2 - 5 : x.h / 2 - 5;
	int xx = a.tx - x.tx;
	int yy = a.ty - x.ty;

	//DrawCircle(a.tx, a.ty, ra, GetColor(0, 255, 0), FALSE);
	//DrawCircle(x.tx, x.ty, rx, GetColor(0, 255, 0), FALSE);

	if (xx*xx + yy*yy <= (ra+rx) * (ra+rx))
		return 1;
	return 0;
}


int PlayerItemHitJudge(PlayerData a, ItemData x) {
	int ra = (a.w < a.h) ? a.w / 2 : a.h / 2;
	int rx = (x.w < x.h) ? x.w / 2 : x.h / 2;
	int xx = a.tx - x.tx;
	int yy = a.ty - x.ty;

	//DrawCircle(a.tx, a.ty, ra, GetColor(0, 255, 0), FALSE);
	//DrawCircle(x.tx, x.ty, rx, GetColor(0, 255, 0), FALSE);

	if (xx*xx + yy*yy <= (ra + rx) * (ra + rx))
		return 1;
	return 0;
}


void HPbox() {
	DrawBox(50, 50, (WINDOW_WIDTH - 50)*HP_num / HP_max, 80, GetColor(255, 0, 0), TRUE);
	DrawFormatString((WINDOW_WIDTH - 50)*HP_num / HP_max + 5, 60, GetColor(0, 0, 0), "%d", HP_num);

	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].flag == 1)
			DrawFormatString(enemy[i].tx + enemy[i].w / 2, enemy[i].ty, GetColor(0, 0, 0), "%d", enemy[i].hp);
	}
}

void AllLoad() {
	MissionSelectLoad();
	KyaraSelectLoad();
	PlayerLoad();
	PlayerDataLoad();
	EnemyLoad();
	EnemyDataLoad();
	ItemLoad();
}

void AllInit() {
	PlayerInit();
	PlayerEnter();
	EnemyInit();
	ItemInit();
}

