/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/

#include<SDL/SDL.h>
#include"common.h"
#include"server_common.h"

int main(int argc,char *argv[])
{
	int	num = 1;
	int	endFlag = 1;
	
	u_short port = PORT; // ポート番号

	/* 引き数チェック */
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
	
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	/* クライアントとの接続 */
	if(SetUpServer(num) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
	
	fprintf(stderr, "Number of clients = %d\n", num);
	fprintf(stderr, "Port number = %d\n", port);
	
	SetUpServer(num, port); //server側の初期設定

	/* メインイベントループ */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	/* 終了処理 */
	Ending();

	return 0;
}


