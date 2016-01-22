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


int PBarriETamaHitJudge(PlayerData a, Bullet x){
    int ra = 220;
    int rb = 150;
    int rx = (a.src.w < a.src.h) ? a.src.w / 2 : a.src.h / 2;
    int xx = a.tx - x.tx;
    int yy = a.ty - x.ty;

    if (xx*xx + yy*yy <= (ra+rx) * (ra+rx)){
        if (xx*xx + yy*yy > (rb+rx) * (rb+rx))
            return 1;
    }
    return 0;
}


int PlayerItemHitJudge(PlayerData a, ItemData b) {
    int ra = (a.src.w < a.src.h) ? a.src.w / 2 : a.src.h / 2;
    int rb = 25;
    int xx = a.tx - b.tx;
    int yy = a.ty - b.ty;

    if (xx*xx + yy*yy <= (ra+rb) * (ra+rb))
            return 1;
    return 0;
}


void PlayerHit(int myid, int m, int n, int sock){ //プレイヤーと弾
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = PLAYER_HIT;
    data.cid = myid;
    data.state = GAME_MAIN;
    data.m = m;
    data.n = n;
    data.player = player[myid];

    send_data(&data, sizeof(CONTAINER), sock);
}

void PlayerDamage(CONTAINER data, int myid, int sock){
    //HP--;
    //player[data.cid].flag2 = 180;

    HP = data.hp;
    player[data.cid] = data.player;

    /*if(HP <= 0){ //ゲームオーバー
        memset(&data, 0, sizeof(CONTAINER));
        data.command = END_COMMAND;
        fprintf(stderr, "%d\n", data.command);
        data.cid = myid;
        data.state = GAME_MAIN;
        send_data(&data, sizeof(CONTAINER), sock);

        fprintf(stderr, "/////////////////////\n");
        fprintf(stderr, "//   GameOver...   //\n");
        fprintf(stderr, "/////////////////////\n");
        }*/

    ene_shot[data.m].bullet[data.n].flag = 0;
}


void PlayerHit2(int myid, int sock){ //プレイヤーと敵
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = PLAYER_HIT2;
    data.cid = myid;
    data.state = GAME_MAIN;
    data.player = player[myid];

    send_data(&data, sizeof(CONTAINER), sock);
}

void PlayerDamage2(CONTAINER data, int myid, int sock){
    //HP_Num-=5;
    //player[data.cid].flag2 = 180;

    HP = data.hp;
    player[data.cid] = data.player;

    /*if(HP <= 0){ //ゲームオーバー
        memset(&data, 0, sizeof(CONTAINER));
        data.command = END_COMMAND;
        fprintf(stderr, "%d\n", data.command);
        data.cid = myid;
        send_data(&data, sizeof(CONTAINER), sock);

        fprintf(stderr, "/////////////////////\n");
        fprintf(stderr, "//   GameOver...   //\n");
        fprintf(stderr, "/////////////////////\n");
        }*/
}


void EnemyHit(int myid, int m, int n, int ene_num, int sock){ //プレイヤーと弾
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = ENEMY_HIT;
    data.cid = myid;
    data.state = GAME_MAIN;
    data.m = m; //pla_shot[m]
    data.n = n; //pla_shot[m].bullet[n]
    data.ene_num = ene_num;
    data.enemy = enemy[ene_num];
    data.stage = stage;

    send_data(&data, sizeof(CONTAINER), sock);
}


void BarrierHit(int myid, int m, int n, int sock){ //バリアと弾
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = BARRIER_HIT;
    data.cid = myid;
    data.state = GAME_MAIN;
    data.m = m; //ene_shot[m]
    data.n = n; //ene_shot[m].bullet[n]
    data.stage = stage;

    send_data(&data, sizeof(CONTAINER), sock);
}


void ItemHit(int myid, int m, int sock) {
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = DATA_PULL;
    data.cid = myid;
    data.state = GAME_MAIN;
    data.flag = 5;
    data.m = m; //item[m]
    data.stage = stage;

    send_data(&data, sizeof(CONTAINER), sock);
    //fprintf(stderr, "%dアイテムゲット！\n", data.m);
}


/*int num = 0; //倒した敵の数
int EnemyDamage(CONTAINER data){
    enemy[data.ene_num] = data.enemy;
    if(enemy[data.ene_num].flag == 1){
        enemy[data.ene_num].hp-=player[pla_shot[data.m].num].power;
        if(enemy[data.ene_num].hp <= 0){
            enemy[data.ene_num].flag = 2;
            enemy[data.ene_num].flag2 = 0;
            num++;

            fprintf(stderr, "%2d:Enemy kill.\n", num);

            Score_Plus += enemy[data.ene_num].score; //ここをサーバーで行う
//アイテムを出す
            if(enemy[data.ene_num].item > 0){

            }
            pla_shot[data.m].bullet[data.n].flag = 0;
            return num;
        }
        enemy[data.ene_num].flag2 = 60;
        pla_shot[data.m].bullet[data.n].flag = 0;
    }
    return -1;
    }*/


/******************

knd : 0...体力の数字
******************/
void StringDraw(int num, int knd){
    char str[100];
    SDL_Surface *stringA;
    SDL_Color red = {0xFF, 0x00, 0x00};
    SDL_Color green = {0x00, 0xFF, 0x00};
    SDL_Color blue = {0x00, 0x00, 0xFF};

    SDL_Rect stA_src, stA_dst;
    switch(knd){
    case 0://体力
        sprintf(str, "Player HP");
        stringA = TTF_RenderUTF8_Blended(font, str, red);
        stA_src = SrcRectInit(0, 0, 500, 40);
        stA_dst = DstRectInit(50, 60);
        SDL_BlitSurface(stringA, &stA_src, window, &stA_dst);
        SDL_FreeSurface(stringA);

        sprintf(str, "%d", num);
        stringA = TTF_RenderUTF8_Blended(font, str, green);
        stA_src = SrcRectInit(0, 0, 500, 40);
        stA_dst = DstRectInit((WINDOW_WIDTH - 380)*HP/HP_M, 80);
        break;

    case 1://スコア
        sprintf(str, "Score:%8d", num);
        stringA = TTF_RenderUTF8_Blended(font, str, red);
        stA_src = SrcRectInit(0, 0, 500, 60);
        stA_dst = DstRectInit(WINDOW_WIDTH - 300, 40);
        break;

    case 2://パワー
        sprintf(str, "P : %d", num);
        stringA = TTF_RenderUTF8_Blended(font, str, red);
        stA_src = SrcRectInit(0, 0, 500, 40);
        stA_dst = DstRectInit(WINDOW_WIDTH - 300, 80);
        break;

    case 3://スピード
        sprintf(str, "S : %d", num);
        stringA = TTF_RenderUTF8_Blended(font, str, blue);
        stA_src = SrcRectInit(0, 0, 500, 40);
        stA_dst = DstRectInit(WINDOW_WIDTH - 150, 80);
        break;

    case 4://結果画面のスコア表示
        fprintf(stderr, "スコアin\n");
        sprintf(str, "スコア：%6d点", num);
        stringA = TTF_RenderUTF8_Blended(font, str, red);
        stA_src = SrcRectInit(0, 0, 500, 400);
        stA_dst = DstRectInit(WINDOW_WIDTH/2-100, WINDOW_HEIGHT/2-40);
        break;
    }

    SDL_BlitSurface(stringA, &stA_src, window, &stA_dst);
    SDL_FreeSurface(stringA);
}
