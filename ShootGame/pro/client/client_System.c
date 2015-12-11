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


int ETamaPlayerHitJudge(Bullet a, PlayerData x) {
	int ra = (a.src.w < a.src.h) ? a.src.w / 2 : a.src.h / 2;
	int rx = (x.src.w < x.src.h) ? x.src.w / 2 - 5 : x.src.h / 2 - 5;
	int xx = a.tx - x.tx;
	int yy = a.ty - x.ty;

	if (xx*xx + yy*yy <= (ra+rx) * (ra+rx))
		return 1;
	return 0;
}


void PlayerHit(int myid, int m, int n, int sock){ //プレイヤーと弾
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = PLAYER_HIT;
    data.cid = myid;
    data.m = m;
    data.n = n;

    send_data(&data, sizeof(CONTAINER), sock);
}

void PlayerDamage(CONTAINER data){
    HP_Num--;
    player[data.cid].flag2 = 180;

    ene_shot[data.m].bullet[data.n].flag = 0;
}


void PlayerHit2(int myid, int sock){ //プレイヤーと敵
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = PLAYER_HIT2;
    data.cid = myid;

    send_data(&data, sizeof(CONTAINER), sock);
}

void PlayerDamage2(CONTAINER data){
    HP_Num--;
    player[data.cid].flag2 = 180;
}


void EnemyHit(int myid, int m, int n, int ene_num, int sock){ //プレイヤーと弾
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = ENEMY_HIT;
    data.cid = myid;
    data.m = m;
    data.n = n;
    data.ene_num = ene_num;

    send_data(&data, sizeof(CONTAINER), sock);
}

void EnemyDamage(CONTAINER data){
    enemy[data.ene_num].hp-=player[pla_shot[data.m].num].power;
    enemy[data.ene_num].flag2 = 60;
    if(enemy[data.ene_num].hp <= 0){
        enemy[data.ene_num].flag = 2;

        Score_Plus += enemy[data.ene_num].score; //ここをサーバーで行う
//アイテムを出す
        if(enemy[data.ene_num].item > 0){

        }
    }

    pla_shot[data.m].bullet[data.n].flag = 0;
}


/******************

knd : 0...体力の数字
******************/
void StringDraw(int num, int knd){
    char str[100];
    SDL_Surface *stringA;
    SDL_Color red = {0xFF, 0x00, 0x00};

    SDL_Rect stA_src, stA_dst;
    switch(knd){
    case 0:
        sprintf(str, "%d", num);
        stringA = TTF_RenderUTF8_Blended(font, str, red);
        stA_src = SrcRectInit(0, 0, 500, 40);
        stA_dst = DstRectInit((WINDOW_WIDTH - 380)*HP_Num/HP_Max, 80);
        break;

    case 1:
        sprintf(str, "Score:%8d", num);
        stringA = TTF_RenderUTF8_Blended(font, str, red);
        stA_src = SrcRectInit(0, 0, 500, 40);
        stA_dst = DstRectInit(WINDOW_WIDTH - 300, 80);
        break;
    }

    SDL_BlitSurface(stringA, &stA_src, window, &stA_dst);
    SDL_FreeSurface(stringA);
}
