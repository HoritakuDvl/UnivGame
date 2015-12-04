/*****************************************************************
ファイル名	: client_func.h
機能		: クライアントの外部関数の定義
*****************************************************************/

#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"common.h"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void DrawGameMain(int pos, int num);
extern void DrawUpCircle(int pos);
extern void DrawDownCircle(int pos);
extern void DrawLeftCircle(int pos);
extern void DrawRightCircle(int pos);
extern SDL_Rect SrcRectInit(int x, int y, int w, int h);
extern SDL_Rect DstRectInit(int x, int y);

/* client_command.c */
extern int ExecuteCommand(char command);
extern void SendUpCommand(int pos);
extern void SendDownCommand(int pos);
extern void SendLeftCommand(int pos);
extern void SendRightCommand(int pos);
extern void SendSeparateCommand(int pos, int muki); //向きは送っても判断できないので、改善する
extern void SendShotCommand(int pos);
extern void SendShotFinishCommand(int pos);
extern void SendEndCommand(void);

/* client_Event.c */
extern void EventMainFighter(int num);
extern void EventMainTank(int num);

/* client_PlayerData.c */
extern void PlayerLoad();
extern void PlayerInit(int num);
extern void PlayerDraw(int pos);
extern void PlayerDataLoad();
extern void PlayerBulletEnter(int pos);
extern void PlayerShotCalc();
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
extern void EnemyMove();
extern void EnemyBulletMove(int num);
extern void EnemyFree();

/* client_EnemyPattern.c */
extern void EnemyPattern0(int n);
extern void EnemyPattern1(int n);

/* client_EnemyShotPattern.c */
extern void EnemyShotPattern0(int n, int num);
extern void EnemyShotPattern1(int n, int num);

#endif
