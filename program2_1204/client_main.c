/*****************************************************************
ファイル名	: client_main.c
機能		: クライアントのメインルーチン
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void TimeFrames();

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;

    /* 引き数チェック */
    if(argc == 1){
    	serverName = localHostName;
    }
    else if(argc == 2){
    	serverName = argv[1];
    }
    else{
        fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
        return -1;
    }

    /* サーバーとの接続 */
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
        fprintf(stderr,"setup failed : SetUpClient\n");
        return -1;
    }
    fprintf(stderr, "clientID = %d\n", clientID);
    /* ウインドウの初期化 */
    if(InitWindows(clientID,num,name)==-1){
        fprintf(stderr,"setup failed : InitWindows\n");
        return -1;
    }

/*画像・データのロード*/
    PlayerLoad();
    PlayerDataLoad();
    EnemyLoad();
    EnemyDataLoad();

/*各構造体の初期化*/
    PlayerInit(num);
    EnemyInit();

 
    /* メインイベントループ */
    while(endFlag){
        DrawGameMain(clientID, num);
        endFlag = SendRecvManager();
        //fprintf(stderr, "endFlag = %d\n", endFlag);

        TimeFrames();
        SDL_Flip(window);
    };

    /* 終了処理 */
    DestroyWindow();
    CloseSoc();

    return 0;
}


static void TimeFrames(){
    timer.now = SDL_GetTicks();
    timer.wit = timer.now - timer.lev;

    if(timer.wit < 10)
        SDL_Delay(10-timer.wit);

    timer.lev = SDL_GetTicks();
    //SDL_Flip(window);
}
