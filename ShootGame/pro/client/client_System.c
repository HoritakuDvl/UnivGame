#include "../common.h"
#include "client_func.h"

int PlayerEnemyHitJudge(PlayerData a, EnemyData b) {
	int ra = (a.src.w < a.src.h) ? a.src.w / 2 : a.src.h / 2;
	int rb = (b.src.w < b.src.h) ? b.src.w / 2 : b.src.h / 2;
	int xx = a.tx - b.tx;
	int yy = a.ty - b.ty;

	if (xx*xx + yy*yy <= (ra + rb) * (ra + rb))
		return 1;
	return 0;
}


int PTamaEnemyHitJudge(Bullet a, EnemyData x) {
	int ra = (a.src.w < a.src.h) ? a.src.w / 2 : a.src.h / 2;
	int rx = (x.src.w < x.src.h) ? x.src.w / 2-10 : x.src.h / 2-10;
	int xx = a.tx-x.tx;
	int yy = a.ty-x.ty;

	if (xx*xx + yy*yy <= (ra+rx) * (ra+rx))
		return 1;
	return 0;
}


int ETamaPlayerHitJjudge(Bullet a, PlayerData x) {
	int ra = (a.src.w < a.src.h) ? a.src.w / 2 : a.src.h / 2;
	int rx = (x.src.w < x.src.h) ? x.src.w / 2 - 5 : x.src.h / 2 - 5;
	int xx = a.tx - x.tx;
	int yy = a.ty - x.ty;

	if (xx*xx + yy*yy <= (ra+rx) * (ra+rx))
		return 1;
	return 0;
}
