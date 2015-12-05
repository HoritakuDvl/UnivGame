#include <netdb.h>

#include "../constants.h"
#include "../common.h"
#include "client_func.h"


static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask;
static CLIENT clients[MAX_NUM_CLIENTS];

//static void input_main_command(void); //送信（コマンド入力）
static int execute_command(void); //受信（行動実行）
//static void send_data(void *, int); //データを送る
static int receive_data(void *, int); //データを受け取る


static SDL_Surface *haikei1;
static SDL_Rect src_rect, dst_rect;

/******************************
void setup_client(char *server_name, u_short port)
引数：*server_name サーバー名(ホスト名)
      port クライアントが使用するポート番号
機能：クライアントの起動
********************************/
void setup_client(char *server_name, u_short port) {
  struct hostent *server;
  struct sockaddr_in sv_addr;

  fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
  if ((server = gethostbyname(server_name)) == NULL) { //ホスト名からIPアドレスを取得出来なかったら
    handle_error("gethostbyname()");
  }

  sock = socket(AF_INET, SOCK_STREAM, 0); //ソケットの識別番号を返す
  if (sock < 0) {
    handle_error("socket()");
  }

//サーバーのアドレス
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(port); //IPネットワークバイトオーダーの（16bit）形式で変換
  sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0];

//connect:クライアントからサーバー側へコネクションの接続確立要求を行う
//引数１：ソケットディスクリプタ　引数２：sockaddr構造体へのポインタ　引数３：サイズ
  if(connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) { 
    handle_error("connect()");
  }

  fprintf(stderr, "Input your name: ");
  char user_name[MAX_LEN_NAME];
  if(fgets(user_name, sizeof(user_name), stdin) == NULL) { //fgets:引数３ファイルから１行、または引数２バイトを引数１に書きこむ
    handle_error("fgets()");
  }
  user_name[strlen(user_name) - 1] = '\0';
  send_data(user_name, MAX_LEN_NAME, sock); //引数１をソケットに入れる

  fprintf(stderr, "Waiting for other clients...\n");
  receive_data(&num_clients, sizeof(int)); //クライアント数を読み込む
  fprintf(stderr, "Number of clients = %d.\n", num_clients);
  receive_data(&myid, sizeof(int)); //自分のIDを読み込む
  fprintf(stderr, "Your ID = %d.\n", myid);
  int i;
  for(i = 0; i < num_clients; i++) {
      receive_data(&clients[i], sizeof(CLIENT)); //クライアントのデータを読み込む
      fprintf(stderr, "Client %d is accepted (name=%s).\n", i, clients[i].name);
  }

  num_sock = sock + 1;
  FD_ZERO(&mask); //引数をゼロクリア
  FD_SET(0, &mask); //0番目のFDに対応する値を1にセット
  FD_SET(sock, &mask); //
  fprintf(stderr, "Input command (M=message, S=select, Q=quit): \n");

/*SDLの初期画面設定*/
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0) {
        printf("failed to initialize SDL.\n");
        return;
    }
    SDL_EnableKeyRepeat(0, 0);

    if((window = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, /*SDL_HWSURFACE | SDL_FULLSCREEN*/SDL_SWSURFACE)) == NULL) {
        printf("failed to initialize videomode.\n");
        exit(-1);
    }

    //SDL_WM_SetCaption("SDL Game", "Software Exp");	// ウィンドウ名などを指定

    for(i = 0; i < SDL_NumJoysticks(); i++){
        printf("%s\n", SDL_JoystickName(i));
    }
    joystick = SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);
    if(!joystick){
        fprintf(stderr, "failed to open joystick.\n");
        return;
    }

/*ロード*/
    haikei1 = IMG_Load("sozai/背景-砂漠.png");
    PlayerLoad();
    EnemyLoad();

/*初期化*/
    /*for(i = 0; i < MAX_CLIENTS; i++){
        en[i].x = 100;
        en[i].y = 100*(i+1);
        en[i].r = 20;
        }*/
    PlayerInit(num_clients);
    PlayerDataLoad();
    EnemyInit();
    EnemyDataLoad();
    
    src_rect = SrcRectInit(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    dst_rect = DstRectInit(0, 0);
}

/*************************************
int control_requests ()
引数：なし
機能：クライアント中の動作(main画面)
返値：1…メッセージの送信　0…チャット終了
***************************************/
int control_requests () {
  fd_set read_flag = mask;

  struct timeval timeout;
  timeout.tv_sec = 0; //クライアント側
  timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
  if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
    handle_error("select()");
  }

  int result = 1;
  //input_main_command(); //送信
  switch(player[myid].knd2){
  case 1:
      EventMainFighter(myid, sock);
      break;
  case 2:
      EventMainTank(myid, sock);
      break;
  }
      
  if (FD_ISSET(sock, &read_flag)) {
      result = execute_command(); //受信
  }

  SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 0, 0, 0, 255));
  SDL_BlitSurface(haikei1, &src_rect, window, &dst_rect);	// ウィンドウ（ここではSDL_GetVideoSurface関数で指定）に画像を貼り付ける  
  int i;

  EnemyDraw();

  for(i = 0; i < num_clients; i++){
      if(player[i].command.up == 1){
          PlayerUpMove(i);
      }
      if(player[i].command.down == 1){
          PlayerDownMove(i);
      }
      if(player[i].command.left == 1){
          PlayerLeftMove(i);
      }
      if(player[i].command.right == 1){
          PlayerRightMove(i);
      }
      if(player[i].command.b5 == 1){
          PlayerBulletEnter(i);
      }

      PlayerDraw(i);
  }

  PlayerEnter(num_clients);
  EnemyEnter();

  EnemyMove(num_clients);
  PlayerShotCalc();
  EnemyBulletMove(num_clients);

  return result;
}

/*************************************
static int input_command()
引数：なし
機能：クライアントからサーバーへの送信
返値：
*************************************/
/*static void input_main_command() {
  CONTAINER data;
  memset(&data, 0, sizeof(CONTAINER));
  SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_JOYBUTTONDOWN:
            switch(event.jbutton.button){
            case 3: //4ボタン
                data.command = MESSAGE_COMMAND; //server.cのcontrol_requests関数より
                data.cid = myid; //送信をしたIDの挿入
                send_data(&data, sizeof(CONTAINER)); //dataの書き込み
                break;
            }
            break;
        case SDL_JOYBUTTONUP:
            switch(event.jbutton.button){
            case 3: //4ボタン
                data.command = RIGHT_COMMAND; //server.cのcontrol_requests関数より
                data.cid = myid; //送信をしたIDの挿入
                send_data(&data, sizeof(CONTAINER)); //dataの書き込み
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
            case SDLK_RIGHT: //メッセージ送信
                data.command = MESSAGE_COMMAND; //server.cのcontrol_requests関数より
                data.cid = myid; //送信をしたIDの挿入
                send_data(&data, sizeof(CONTAINER)); //dataの書き込み
                break;

            case SDLK_ESCAPE: //チャットプログラムの終了
                data.command = QUIT_COMMAND; //server.cのcontrol_requests関数より
                data.cid = myid; //送信(退室)をしたIDの挿入
                send_data(&data, sizeof(CONTAINER)); //dataの書き込み
                break;
            default:
                fprintf(stderr, "not a valid command.\n");
                break;
            }
        }
    }
}*/

/**********************************
static int execute_command()
引数：なし
機能：サーバーからクライアントへの受信
返値：
***********************************/
static int execute_command() {
    CONTAINER data;
    int result = 1;
    memset(&data, 0, sizeof(CONTAINER));
    receive_data(&data, sizeof(data));
    switch (data.command) { //クライアントのチャットの行動
    case UP_COMMAND: //メッセージが送信されたら
        player[data.cid].command.up = 1;
        //player[data.cid].tx = data.tx;
        //player[data.cid].ty = data.ty;
        result = 1;
    break;
    case DOWN_COMMAND: //メッセージが送信されたら
        player[data.cid].command.down = 1;
        //player[data.cid].tx = data.tx;
        //player[data.cid].ty = data.ty;
        result = 1;
    break;
    case LEFT_COMMAND: //メッセージが送信されたら
        player[data.cid].command.left = 1;
        //player[data.cid].tx = data.tx;
        //player[data.cid].ty = data.ty;
        result = 1;
    break;
    case RIGHT_COMMAND: //メッセージが送信されたら
        player[data.cid].command.right = 1;
        //player[data.cid].tx = data.tx;
        //player[data.cid].ty = data.ty;
        result = 1;
    break;

    case SEPARATE_UPDO_COMMAND:
        player[data.cid].command.up = 0;
        player[data.cid].command.down = 0;
        if(myid != data.cid){
           player[data.cid].tx = data.tx;
           player[data.cid].ty = data.ty;
        }
        result = 1;
        break;
    case SEPARATE_LERI_COMMAND:
        player[data.cid].command.left = 0;
        player[data.cid].command.right = 0;
        if(myid != data.cid){
           player[data.cid].tx = data.tx;
           player[data.cid].ty = data.ty;
        }
        result = 1;
        break;

    case SHOT_COMMAND:
        player[data.cid].command.b5 = 1;
        //player[data.cid].tx = data.tx;
        //player[data.cid].ty = data.ty;
        result = 1;
        break;
    case SHOT_FINISH_COMMAND:
        player[data.cid].command.b5 = 0;
        //player[data.cid].tx = data.tx;
        //player[data.cid].ty = data.ty;
        result = 1;
        break;
  case END_COMMAND: //誰かが退室したら
    fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
    result = 0; //チャットを終了
    break;
  default:
    fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
    exit(1);
  }
  return result;
}

/*****************************************
static void send_data(void *data, int size)
引数：*data 書きこむデータ
       size データのサイズ
機能：データをソケット(sock)に入れる
 ****************************************/
/*static void send_data(void *data, int size) {
  if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "send_data(): data is illeagal.\n");
	exit(1);
  }

  if(write(sock, data, size) == -1) { //sockにdataのsizeバイト分書きこむ
      handle_error("write()"); //エラー
  }    
}*/

/*******************************************
static int receive_data(void *data, int size)
引数：*data 読み込みデータ
       size 読み込みデータのサイズ
機能：コネクションからバッファへのデータの読み込み
返値：読み込み結果
********************************************/
static int receive_data(void *data, int size) {
  if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "receive_data(): data is illeagal.\n");
	exit(1);
  }

  return(read(sock, data, size)); //引数１：接続ソケット　引数２：読み込み先のバッファ　引数３：読み込みバイト数
}

/**********************************************
static void handle_error(char *message)
引数：*message エラーメッセージ
機能：エラーメッセージの送信
***********************************************/
void handle_error(char *message) {
  perror(message);
  fprintf(stderr, "%d\n", errno);
  exit(1);
}
/**********************************************
void terminate_client()
引数：なし
機能：クライアントの終了
***********************************************/
void terminate_client() {
  fprintf(stderr, "Connection is closed.\n");
  close(sock);
  exit(0);
}


SDL_Rect SrcRectInit(int x, int y, int w, int h){
    SDL_Rect tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.w = w;
    tmp.h = h;

    return tmp;
}


SDL_Rect DstRectInit(int x, int y){
    SDL_Rect tmp;

    tmp.x = x;
    tmp.y = y;
    
    return tmp;
}




