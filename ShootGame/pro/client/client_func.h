/*****************************************************************
ファイル名	: client_func.h
機能		: クライアントの外部関数の定義
*****************************************************************/

#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include "../common.h"
#include "../constants.h"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

int HP_Num; //残り体力
int HP_Max; //最大体力
int Total_Score; //合計スコア
int Score_Plus; //足されるスコア

/* client.c */
extern void setup_client(char *, u_short);
extern int control_requests();
extern void terminate_client();
extern SDL_Rect SrcRectInit(int x, int y, int w, int h);
extern SDL_Rect DstRectInit(int x, int y);
extern void handle_error(char *);
extern void HaikeiFree();

/* client_Event.c */
extern void EventMainFighter(int myid, int sock);
extern void EventMainTank(int myid, int sock);
extern void send_data(void *, int, int);

/* client_PlayerData.c */
extern void PlayerLoad();
extern void PlayerInit(int num);
extern void PlayerDraw(int pos);
extern void PlayerDataLoad();
extern void PlayerEnter(int num);
extern void PlayerAction(int pos);
extern void PlayerUpMove(int pos);
extern void PlayerDownMove(int pos);
extern void PlayerLeftMove(int pos);
extern void PlayerRightMove(int pos);
extern void PlayerBatteryRota(int pos);
extern void PlayerBulletEnter(int pos);
extern void PlayerShotCalc(int myid, int sock);
extern void PlayerBulletClean();
extern void PlayerFree();

/* client_PlayerShotPattern.c */
extern void PlayerShotPattern0(int n);
extern void PlayerShotPattern1(int n);

/* client_EnemyData.c */
extern void EnemyLoad();
extern void EnemyInit();
extern void EnemyDraw();
extern void EnemyDataLoad();
extern void EnemyEnter();
extern void EnemyMove(int num, int myid, int sock);
extern void EnemyBulletMove(int num, int myid, int sock);
extern void EnemyBulletClean();
extern void EnemyFree();

/* client_EnemyPattern.c */
extern void EnemyPattern0(int n);
extern void EnemyPattern1(int n);

/* client_EnemyShotPattern.c */
extern void EnemyShotPattern0(int n, int num);
extern void EnemyShotPattern1(int n, int num);

/* client_System.c */
extern int PlayerEnemyHitJudge(PlayerData a, EnemyData b);
extern int PTamaEnemyHitJudge(Bullet a, EnemyData x);
extern int ETamaPlayerHitJudge(Bullet a, PlayerData x);
extern void PlayerHit(int myid, int m, int n, int sock);
extern void PlayerDamage(CONTAINER data);
extern void PlayerHit2(int myid, int sock);
extern void PlayerDamage2(CONTAINER data);
extern void EnemyHit(int myid, int m, int n, int ene_num, int sock);
extern int EnemyDamage(CONTAINER data);
extern void StringDraw(int num, int knd);



#endif
