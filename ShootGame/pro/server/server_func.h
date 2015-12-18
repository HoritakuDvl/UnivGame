/*****************************************************************
�ե�����̾	: server_func.h
��ǽ		: �����С��γ����ؿ������
*****************************************************************/

#ifndef _SERVER_FUNC_H_
#define _SERVER_FUNC_H_

#include"server_common.h"

/* server_net.c */
extern int SetUpServer(int num);
extern void Ending(void);
extern int RecvIntData(int pos,int *intData);
extern void SendData(int pos,void *data,int dataSize);
extern int SendRecvManager(void);

/*server.c*/
//extern void EnemyDataLoad();
//extern void EnemyEnter();


/* server_command.c */
extern int ExecuteCommand(char command,int pos);

#endif
