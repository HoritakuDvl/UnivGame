/*****************************************************************
�ե�����̾	: server_main.c
��ǽ		: �����С��Υᥤ��롼����
*****************************************************************/

#include<SDL/SDL.h>
#include"common.h"
#include"server_common.h"

int main(int argc,char *argv[])
{
	int	num = 1;
	int	endFlag = 1;
	
	u_short port = PORT; // �ݡ����ֹ�

	/* �����������å� */
	switch(argc) {
  	case 1:
    	break;
  	case 2:
    	num = atoi(argv[1]);
    	break;
  	case 3:
    	num = atoi(argv[1]);
    	port = atoi(argv[2]);
   		break;
  	default:
    	fprintf(stderr, "Usage: %s [number of clients] [port number]\n", argv[0]);
    	return 1;
  	}
  
	if((num = atoi(argv[1])) < 0 ||  num > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
	
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* ���饤����ȤȤ���³ */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
	
	fprintf(stderr, "Number of clients = %d\n", num);
	fprintf(stderr, "Port number = %d\n", port);
	
	SetUpServer(num, port); //server¦�ν������

	/* �ᥤ�󥤥٥�ȥ롼�� */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	/* ��λ���� */
	Ending();

	return 0;
}


