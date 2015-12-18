/*****************************************************************
�ե�����̾	: server_common.h
��ǽ		: �����С��ǻ��Ѥ�������������Ԥ�
*****************************************************************/

#ifndef _SERVER_COMMON_H_
#define _SERVER_COMMON_H_

#include"../common.h"

#define ALL_CLIENTS	-1   /* �����饤����Ȥ˥ǡ�����������˻��Ѥ��� */


typedef struct{
    int w, h;
    int hp_max;
    int knd, knd2, sp;
    int pattern2, blknd, blCnt;
    int power;
    int w2, h2;
}PlayerOrder;

/*typedef struct {
    int tx, ty, w, h; //�����ΰ��֡��礭��
    int hp_max, flag;
    int mission, stage, wait; //���ơ���
    int knd, pattern, sp; //���ࡢ��ư
    int pattern2, blknd, blW, blH, blCnt, bltime; //����
    int item, item2;
    int score;
}EnemyOrder;*/

PlayerOrder playerOrder[PLAYER_ORDER_MAX];
//EnemyOrder enemyOrder[ENEMY_ORDER_MAX];


int HP_Num; //�Ĥ�����
int HP_Max; //��������

#endif
