/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

/*
バグ
・client_EnemyShotPattern.c  自機狙いができない 解決
・client_EnemyData.c  たまに敵が描画されない 解決
*/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

//#include <opencv/cv.h>
//#include <opencv/highgui.h>
/*#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>
*/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
//#include <SDL/SDL_mixer.h>

#define PORT			(u_short)8888	/* ポート番号 */

#define MAX_CLIENTS		4				/* クライアント数の最大値 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大値*/

#define MAX_DATA		200				/* 送受信するデータの最大値 */

#define END_COMMAND		'E'		  		/* プログラム終了コマンド */
#define LEFT_COMMAND	'L'				/* 左コマンド */
#define RIGHT_COMMAND	'R'				/* 右コマンド */
#define UP_COMMAND	'U'				/* 上コマンド */
#define DOWN_COMMAND	'D'				/* 下コマンド */
#define SEPARATE_UPDO_COMMAND	'P'				/* 左スティックを離すコマンド */
#define SEPARATE_LERI_COMMAND	'Q'				/* 左スティックを離すコマンド */
#define SHOT_COMMAND 'S'
#define SHOT_FINISH_COMMAND 'F'

#define PLAYER_ORDER_MAX 2
#define PLAYER_SHOT_MAX 4
#define PLAYER_SHOT_PATTERN_MAX 2
#define ENEMY_MAX 3
#define ENEMY_ORDER_MAX 5
#define ENEMY_PATTERN_MAX 2
#define ENEMY_SHOT_MAX 3
#define ENEMY_SHOT_PATTERN_MAX 2
#define SHOT_BULLET_MAX 100
#define PI 3.1415926535

SDL_Surface *window;

typedef struct{
    int up, down, left, right;
    int b5;
}Command;

typedef struct{
    SDL_Rect src, dst;
    int tx, ty;
    Command command;

    int flag, flag2; //生成されている状態か //ダメージを受けているか
    int knd, knd2, pattern2; //どの機種か //戦闘機か戦車か //初期の攻撃方法
    int power; //攻撃力
    int sp; //速度
    double ang; //砲台の角度
    int num; //○Pか
}PlayerData;

typedef struct{
    int w, h;
    int hp_max;
    int knd, knd2, sp;
    int pattern2, blknd, blCnt;
    int power;
    int w2, h2;
}PlayerOrder;

typedef struct {
    SDL_Rect src, dst; //画像
    int tx, ty;
    int flag; //生成されているか
    int flag2; //ダメージを受けているか
    int hp, hp_max; //体力
    int wait;
    int knd, pattern, sp, cnt; //種類、移動
    int pattern2, blknd, blW, blH, blCnt, bltime; //攻撃
    int item, item2;
}EnemyData;

typedef struct {
    int tx, ty, w, h; //画像の位置、大きさ
    int hp_max, flag;
    int mission, stage, wait; //ステージ
    int knd, pattern, sp; //種類、移動
    int pattern2, blknd, blW, blH, blCnt, bltime; //攻撃
    int item, item2;
}EnemyOrder;

typedef struct {
    SDL_Rect src, dst;
    double tx, ty;
    int ghandle;
    int flag, knd, cnt, state;
    double ang, rad, spd;
}Bullet;

typedef struct {
	int flag, cnt, knd, num; //フラグ、カウント、種類、敵の番号
	Bullet bullet[SHOT_BULLET_MAX];
}Shot;

PlayerData player[MAX_CLIENTS];
PlayerOrder playerOrder[PLAYER_ORDER_MAX];
Shot pla_shot[PLAYER_SHOT_MAX];
EnemyData enemy[ENEMY_MAX];
EnemyOrder enemyOrder[ENEMY_ORDER_MAX];
Shot ene_shot[ENEMY_SHOT_MAX];

extern int HP_Max;
extern int HP_Num;

#endif
