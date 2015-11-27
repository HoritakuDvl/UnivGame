/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

/*
バグ
・client_win.c : DrawLeftCircle関数 : 円が左に行かない
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
//#include <SDL/SDL_rotozoom.h>
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
#define SEPARATE_UPDO_COMMAND	'S'				/* 左スティックを離すコマンド */
#define SEPARATE_LERI_COMMAND	'T'				/* 左スティックを離すコマンド */

#define PLAYER_ORDER_MAX 2
#define PI 3.1415926535

SDL_Surface *window;

typedef struct{
    SDL_Rect src, dst;
    int tx, ty;
    int command;

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

PlayerData player[MAX_CLIENTS];
PlayerOrder playerOrder[PLAYER_ORDER_MAX];

extern int HP_Max;
extern int HP_Num;

#endif
