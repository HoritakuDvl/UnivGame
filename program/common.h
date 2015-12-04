/*****************************************************************
�ե�����̾	: common.h
��ǽ		: �����С��ȥ��饤����Ȥǻ��Ѥ�������������Ԥ�
*****************************************************************/

/*
�Х�
��client_EnemyShotPattern.c  �����������Ǥ��ʤ� ���
��client_EnemyData.c  ���ޤ�Ũ�����褵��ʤ� ���
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

#define PORT			(u_short)8888	/* �ݡ����ֹ� */

#define MAX_CLIENTS		4				/* ���饤����ȿ��κ����� */
#define MAX_NAME_SIZE	10 				/* �桼����̾�κ�����*/

#define MAX_DATA		200				/* ����������ǡ����κ����� */

#define END_COMMAND		'E'		  		/* �ץ���ཪλ���ޥ�� */
#define LEFT_COMMAND	'L'				/* �����ޥ�� */
#define RIGHT_COMMAND	'R'				/* �����ޥ�� */
#define UP_COMMAND	'U'				/* �女�ޥ�� */
#define DOWN_COMMAND	'D'				/* �����ޥ�� */
#define SEPARATE_UPDO_COMMAND	'P'				/* �����ƥ��å���Υ�����ޥ�� */
#define SEPARATE_LERI_COMMAND	'Q'				/* �����ƥ��å���Υ�����ޥ�� */
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

    int flag, flag2; //��������Ƥ�����֤� //���᡼��������Ƥ��뤫
    int knd, knd2, pattern2; //�ɤε��狼 //��Ʈ������֤� //����ι�����ˡ
    int power; //������
    int sp; //®��
    double ang; //ˤ��γ���
    int num; //��P��
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
    SDL_Rect src, dst; //����
    int tx, ty;
    int flag; //��������Ƥ��뤫
    int flag2; //���᡼��������Ƥ��뤫
    int hp, hp_max; //����
    int wait;
    int knd, pattern, sp, cnt; //���ࡢ��ư
    int pattern2, blknd, blW, blH, blCnt, bltime; //����
    int item, item2;
}EnemyData;

typedef struct {
    int tx, ty, w, h; //�����ΰ��֡��礭��
    int hp_max, flag;
    int mission, stage, wait; //���ơ���
    int knd, pattern, sp; //���ࡢ��ư
    int pattern2, blknd, blW, blH, blCnt, bltime; //����
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
	int flag, cnt, knd, num; //�ե饰��������ȡ����ࡢŨ���ֹ�
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
