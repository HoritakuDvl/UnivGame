//#include "../constants.h"
#include "../common.h"
#include "client_func.h"

//static Uint32 SignalHandler(Uint32 interval, void *param);
static void TimeFrames();

int main(int argc, char *argv[]) {
  u_short port = DEFAULT_PORT;
  char server_name[MAX_LEN_NAME];

  sprintf(server_name, "localhost");

  switch(argc) {
  case 1:
    break;
  case 2:
    sprintf(server_name, "%s", argv[1]);
    break;
  case 3:
    sprintf(server_name, "%s", argv[1]);
    port = (u_short)atoi(argv[2]); //16bitに変換
    break;
  default:
    fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
    return 1;
  }

  setup_client(server_name, port); //クライアントのセットアップ

  //SDL_AddTimer(5000,SignalHandler,NULL);
  int cond = 1;
  while(cond) {
      cond = control_requests(); //クライアント中の動作

      TimeFrames();
      SDL_Flip(window);
  }

  SDL_Quit();
  HaikeiFree();
  PlayerFree();
  EnemyFree();
  GameTitleFree();
  PlSeFree();
  terminate_client(); //クライアント
  
  return 0;
}

/*static Uint32 SignalHandler(Uint32 interval, void *param)
{
	SendDiamondCommand();

	return interval;
}*/

//現在100フレーム
static void TimeFrames(){
    timer.now = SDL_GetTicks();
    timer.wit = timer.now - timer.lev;

    if(timer.wit < 10)
        SDL_Delay(10-timer.wit);

    timer.lev = SDL_GetTicks();
}




