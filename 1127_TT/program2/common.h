/*****************************************************************
�ե�����̾	: common.h
��ǽ		: �����С��ȥ��饤����Ȥǻ��Ѥ�������������Ԥ�
*****************************************************************/

/*
�Х�
��client_win.c : DrawLeftCircle�ؿ� : �ߤ����˹Ԥ��ʤ�
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

#define PORT			(u_short)8888	/* �ݡ����ֹ� */

#define MAX_CLIENTS		4				/* ���饤����ȿ��κ����� */
#define MAX_NAME_SIZE	10 				/* �桼����̾�κ�����*/

#define MAX_DATA		200				/* ����������ǡ����κ����� */

#define END_COMMAND		'E'		  		/* �ץ���ཪλ���ޥ�� */
#define LEFT_COMMAND	'L'				/* �����ޥ�� */
#define RIGHT_COMMAND	'R'				/* �����ޥ�� */
#define UP_COMMAND	'U'				/* �女�ޥ�� */
#define DOWN_COMMAND	'D'				/* �����ޥ�� */
#define SEPARATE_UPDO_COMMAND	'S'				/* �����ƥ��å���Υ�����ޥ�� */
#define SEPARATE_LERI_COMMAND	'T'				/* �����ƥ��å���Υ�����ޥ�� */

#define PLAYER_ORDER_MAX 2
#define PI 3.1415926535

SDL_Surface *window;

typedef struct{
    SDL_Rect src, dst;
    int tx, ty;
    int command;

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

PlayerData player[MAX_CLIENTS];
PlayerOrder playerOrder[PLAYER_ORDER_MAX];

extern int HP_Max;
extern int HP_Num;

#endif
