/*****************************************************************
ファイル名	: server_common.h
機能		: サーバーで使用する定数の宣言を行う
*****************************************************************/

#ifndef _SERVER_COMMON_H_
#define _SERVER_COMMON_H_

#include"../common.h"

#define ALL_CLIENTS	-1   /* 全クライアントにデータを送る時に使用する */


typedef struct{
    int w, h;
    int hp_max;
    int knd, knd2, sp;
    int pattern2, blknd, blCnt;
    int power;
    int w2, h2;
}PlayerOrder;

/*typedef struct {
    int tx, ty, w, h; //画像の位置、大きさ
    int hp_max, flag;
    int mission, stage, wait; //ステージ
    int knd, pattern, sp; //種類、移動
    int pattern2, blknd, blW, blH, blCnt, bltime; //攻撃
    int item, item2;
    int score;
}EnemyOrder;*/

PlayerOrder playerOrder[PLAYER_ORDER_MAX];
//EnemyOrder enemyOrder[ENEMY_ORDER_MAX];


int HP_Num; //残り体力
int HP_Max; //最大体力

#endif
