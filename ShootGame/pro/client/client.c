#include <netdb.h>

//#include "../constants.h"
#include "../common.h"
#include "client_func.h"

static GAME_STATE gstate = GAME_TITLE;

static int num_clients;
static int myid;
static int sock;
static int num_sock;
static fd_set mask;
static CLIENT clients[MAX_NUM_CLIENTS];

static int DrawGameTitle();
static int DrawGameSelect();
static int DrawGameLoad();
static int DrawGameMain();
static int DrawGameOver();
static int DrawGameClear();
static void PlayerAllInit(int knd); //初期データの送信・受信
//static void input_main_command(void); //送信（コマンド入力）
static int execute_command(void); //受信（行動実行）
//static void send_data(void *, int); //データを送る
static int receive_data(void *, int); //データを受け取る

static void SpeedChange(int sp);


static SDL_Surface *haikei1, *haikei2, *haikei3, *haikei4, *haikei5, *haikei6, *haikei7, *haikei8, *haikei9;
static SDL_Rect src_rect, dst_rect, src_rect2, dst_rect2;


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
  //fprintf(stderr, "Input command (M=message, S=select, Q=quit): \n");

/*SDLの初期画面設定*/
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0) {
        printf("failed to initialize SDL.\n");
        return;
    }
    SDL_EnableKeyRepeat(0, 0);

    TTF_Init();
    font = TTF_OpenFont("sozai/kochi-gothic-subst.ttf", 36);

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
    //haikei1 = IMG_Load("sozai/haikei1.png");
    haikei1 = IMG_Load("sozai/main_resource/05_main_game/background/bg_1.png");
    haikei2 = IMG_Load("sozai/main_resource/05_main_game/background/bg_2.png");
    haikei3 = IMG_Load("sozai/main_resource/05_main_game/background/bg_3.png");
    haikei4 = IMG_Load("sozai/main_resource/05_main_game/background/bg_4.png");
    haikei5 = IMG_Load("sozai/main_resource/05_main_game/background/bg_5.png");
    haikei6 = IMG_Load("sozai/main_resource/05_main_game/background/bg_6.png");
    haikei7 = IMG_Load("sozai/main_resource/05_main_game/background/bg_7.png");
    haikei8 = IMG_Load("sozai/main_resource/05_main_game/background/bg_8.png");
    haikei9 = IMG_Load("sozai/main_resource/05_main_game/background/bg_9.png");
    PlayerLoad();
    EnemyLoad();
    PlSeLoad();
    ItemLoad();

/*初期化*/
    stageFlag = 1;
    stageCount = 0;
    ItemInit();
    power = 1;
    speed = 1;
    //PlayerAllInit(3); //一斉スタートするとき
}

/*************************************
int control_requests ()
引数：なし
機能：クライアント中の動作(main画面)
返値：1…メッセージの送信　0…チャット終了
***************************************/
int control_requests () {
  int result = 1;
  switch(gstate){
  case GAME_TITLE:
      result = DrawGameTitle();
      break;
  case GAME_SELECT:
      result = DrawGameSelect();
      break;
  case GAME_LOAD:
      result = DrawGameLoad();
      break;
  case GAME_MAIN:
      result = DrawGameMain();
      break;
  case GAME_OVER:
      result = DrawGameOver();
      break;
  case GAME_CLEAR:
      result = DrawGameClear();
      break;
  }
  return result;
}


/************************************
static int DrawGameTitle()
引数：
機能：
返値：result
*************************************/
static int DrawGameTitle(){

    fd_set read_flag = mask;

    struct timeval timeout;
    timeout.tv_sec = 0; //クライアント側
    timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
    if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }

    int result = 1;
    if(stageFlag == 1){
        EventTitle(myid, sock);
    }
    if (FD_ISSET(sock, &read_flag)) {
        result = execute_command(); //受信
    }

    SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 255, 255, 255, 255));

    return result;
}

/************************************
static int DrawGameSelect()
引数：
機能：
返値：result
*************************************/
static int DrawGameSelect(){

    fd_set read_flag = mask;

    struct timeval timeout;
    timeout.tv_sec = 0; //クライアント側
    timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
    if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }

    int result = 1;
    if(myid == pla_sele.kPflag){
        EventSelect(myid, sock);
    }

    SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 255, 255, 255, 255));

    PlayerSelect(myid, num_clients);

    if (FD_ISSET(sock, &read_flag)) {
        result = execute_command(); //受信
    }

    return result;
}

/************************************
static int DrawGameLoad()
引数：
機能：
返値：result
*************************************/
static int DrawGameLoad(){

    fd_set read_flag = mask;

    struct timeval timeout;
    timeout.tv_sec = 0; //クライアント側
    timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
    if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }

    int result = 1;
    if(stageFlag == 1){
        EventLoad(myid, sock);
    }
    if (FD_ISSET(sock, &read_flag)) {
        result = execute_command(); //受信
    }

    SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 255, 255, 255, 255));

    return result;
}

/************************************
static void DrawGameMain()
引数：
機能：
返値：result
*************************************/
static int DrawGameMain(){
    fd_set read_flag = mask;

    struct timeval timeout;
    timeout.tv_sec = 0; //クライアント側
    timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
    if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }

    int result = 1;
    if(stageFlag == 1){
        stageCount++;
        //fprintf(stderr, "stageCount = %5d\n", stageCount);

        switch(player[myid].knd2){
        case 1:
            EventMainFighter(myid, sock);
            break;
        case 2:
            EventMainTank(myid, sock);
            break;
        }

//１秒に１回プレイヤーのデータ送信
        if(stageCount%10==1){
            CONTAINER data;
            memset(&data, 0, sizeof(CONTAINER));
            data.command = DATA_PULL;
            data.cid = myid;
            data.state = GAME_MAIN;
            data.num = 4;
            data.player = player[myid];
            send_data(&data, sizeof(CONTAINER), sock);
            //fprintf(stderr, "プレイヤーデータ送信!\n");
        }
      
        if (FD_ISSET(sock, &read_flag)) {
            result = execute_command(); //受信
        }
    }

    int i;
    if(HP == 0){
        for(i = 0; i < num_clients; i++){
            if(player[i].flag == 0){ //読み込み中
                result = 1;
                return result;
            }
        }
    }
  

    SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 0, 0, 0, 255));
    if(stageFlag != 1){
        ItemInit(); //アイテムの初期化

        src_rect.x += 10;
        if(src_rect.x + src_rect.w > WINDOW_WIDTH+200)
            src_rect.w = WINDOW_WIDTH + 200 - src_rect.x;
        if(src_rect.w < 0){
            src_rect.w = 0;
            src_rect2.x+=10;
        }

        src_rect2.w += 10;
        dst_rect2 = DstRectInit(src_rect.w, 0);

        switch(stageFlag){
        case 2:
            SDL_BlitSurface(haikei1, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei2, &src_rect2, window, &dst_rect2);
            break;
        case 3:
            SDL_BlitSurface(haikei2, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei3, &src_rect2, window, &dst_rect2);
            break;
        case 4:
            SDL_BlitSurface(haikei3, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei4, &src_rect2, window, &dst_rect2);
            break;
        case 5:
            SDL_BlitSurface(haikei4, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei5, &src_rect2, window, &dst_rect2);
            break;
        case 6:
            SDL_BlitSurface(haikei5, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei6, &src_rect2, window, &dst_rect2);
            break;
        case 7:
            SDL_BlitSurface(haikei6, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei7, &src_rect2, window, &dst_rect2);
            break;
        case 8:
            SDL_BlitSurface(haikei7, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei8, &src_rect2, window, &dst_rect2);
            break;
        case 9:
            SDL_BlitSurface(haikei8, &src_rect, window, &dst_rect);
            SDL_BlitSurface(haikei9, &src_rect2, window, &dst_rect2);
            break;
        }

        if(src_rect.w <= 0){
            stageFlag = 1;
            src_rect = SrcRectInit(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            dst_rect = DstRectInit(0, 0);
            src_rect2 = SrcRectInit(0, 0, 0, WINDOW_HEIGHT);
            dst_rect2 = DstRectInit(0, 0);
        }

        for(i = 0; i < ENEMY_MAX; i++){
            //fprintf(stderr, "[MOVE]enemy[%d].flag = %d\n", i, enemy[i].flag);
            if(enemy[i].flag != 1){
                enemy[i].flag = 1;
                fprintf(stderr, "[MOVE]enemy[%d].flag = %d!\n\n", i, enemy[i].flag);
            }
        }
        for(i = 0; i < ENEMY_SHOT_MAX; i++){
            if(ene_shot[i].flag == 2)
                ene_shot[i].flag = 0;
        }
   
        result = 1;
    }
    else{ //遊んでいるとき
        switch(stage){
        case 1:
            SDL_BlitSurface(haikei1, &src_rect, window, &dst_rect);
            break;
        case 2:
            SDL_BlitSurface(haikei2, &src_rect, window, &dst_rect);
            break;
        case 3:
            SDL_BlitSurface(haikei3, &src_rect, window, &dst_rect);
            break;
        case 4:
            SDL_BlitSurface(haikei4, &src_rect, window, &dst_rect);
            break;
        case 5:
            SDL_BlitSurface(haikei5, &src_rect, window, &dst_rect);
            break;
        case 6:
            SDL_BlitSurface(haikei6, &src_rect, window, &dst_rect);
            break;
        case 7:
            SDL_BlitSurface(haikei7, &src_rect, window, &dst_rect);
            break;
        case 8:
            SDL_BlitSurface(haikei8, &src_rect, window, &dst_rect);
            break;
        case 9:
            SDL_BlitSurface(haikei9, &src_rect, window, &dst_rect);
            break;
        }
        result = 1;
    }

    boxColor(window, 30/*1280*/, 30, WINDOW_WIDTH-30, 120, 0xffffffaa);
//体力ゲージの描画
    if(HP != 0){
        boxColor(window, 50, 50, (WINDOW_WIDTH - 400)*HP / HP_M, 100, 0x00ff00ff);

        StringDraw(HP, 0);
    }

//スコアの設定
    if(Score_Plus < Total_Score){
        Score_Plus+=10;
        if(Total_Score>Score_Plus)
            Score_Plus = Total_Score;
    }
    /*if(Score_Plus > 0){
        if(Score_Plus >= 10){
            Total_Score += 10;
            Score_Plus -= 10;
        }
        else{
            Total_Score += Score_Plus;
            Score_Plus = 0;
        }
        }*/
    StringDraw(Score_Plus, 1);

//攻撃レベル
    //fprintf(stderr, "power = %d\n", power);
    StringDraw(power,2);

//速度レベル
    StringDraw(speed, 3);

    if(stageFlag == 1){
//敵の描画
        EnemyDraw();
//アイテムの描画
        ItemDraw();
    }

//プレイヤーの動作
    for(i = 0; i < num_clients; i++){
        if(stageFlag == 1)
            PlayerAction(i);
        PlayerDraw(i);
    }

    if(stageFlag == 1){
//その他の動作
        /*for(i = 0; i < ENEMY_MAX; i++)
            if(enemy[i].flag == 0)
                fprintf(stderr, "enemy[%d] = 0\n", i);
            else if(enemy[i].flag == 1)
                fprintf(stderr, "enemy[%d] = 1\n", i);
            else if(enemy[i].flag == 2)
                fprintf(stderr, "enemy[%d] = 2\n", i);
        */

        EnemyMove(num_clients, myid, sock);
        PlayerShotCalc(myid, sock);
        EnemyBulletMove(num_clients, myid, sock);
        ItemMove(myid, sock);
    }
    return result;
}

/************************************
static int DrawGameOver()
引数：
機能：
返値：result
*************************************/
static int DrawGameOver(){

    fd_set read_flag = mask;

    struct timeval timeout;
    timeout.tv_sec = 0; //クライアント側
    timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
    if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }

    int result = 1;
    if(stageFlag == 1){
        EventOver(myid, sock);
    }
    if (FD_ISSET(sock, &read_flag)) {
        result = execute_command(); //受信
    }

    SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 255, 255, 255, 255));

    return result;
}

/************************************
static int DrawGameClear()
引数：
機能：
返値：result
*************************************/
static int DrawGameClear(){

    fd_set read_flag = mask;

    struct timeval timeout;
    timeout.tv_sec = 0; //クライアント側
    timeout.tv_usec = 30; //ユーザー側

//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
    if(select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }

    int result = 1;
    if(stageFlag == 1){
        EventClear(myid, sock);
    }
    if (FD_ISSET(sock, &read_flag)) {
        result = execute_command(); //受信
    }

    SDL_FillRect(window, NULL, SDL_MapRGBA(window->format, 255, 255, 255, 255));

    return result;
}

/********************************************
static void PlayerAllInit()
引数：
機能：自分の選んだ機種をサーバーからもらう
返値：
*********************************************/
static void PlayerAllInit(int knd) {
    stage = 1;
    stageFlag = 1;

    PlayerInit(num_clients);
    //PlayerDataLoad();
    EnemyInit();
    //EnemyDataLoad();
    
    src_rect = SrcRectInit(100, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    dst_rect = DstRectInit(0, 0);
    src_rect2 = SrcRectInit(0, 0, 0, WINDOW_HEIGHT);
    dst_rect2 = DstRectInit(0, 0);

    player[myid].knd = knd; //ここは機体セレクトで決める
    //fprintf(stderr, "player[%d].knd = %d\n", myid, player[myid].knd);
    CONTAINER data;
    memset(&data, 0, sizeof(CONTAINER));

    data.command = DATA_PULL;
    data.cid = myid;
    data.state = gstate;
    data.player = player[myid];
    data.flag = 2;
    send_data(&data, sizeof(CONTAINER), sock);

    //PlayerEnter(num_clients, sock); //ロードするとき（ゲーム開始前）に取り入れる
}


/**********************************
static int execute_command()
引数：なし
機能：サーバーからクライアントへの受信
返値：
***********************************/
static int execute_command() {
    CONTAINER data;
    int result = 1;
    int k;
    memset(&data, 0, sizeof(CONTAINER));
    receive_data(&data, sizeof(data));
    switch(gstate) {
//GAME_TITLE
    case GAME_TITLE:
        switch(data.command) {
        case FOUR_COMMAND:
            fprintf(stderr, "Go to Game!\n");
            gstate = data.state;
            stageFlag = 1;
            //PlayerAllInit(3);
            result = 1;
            break;
        case END_COMMAND:
            fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
            result = 0; //チャットを終了
            break;
        default:
            fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
            exit(1);
            break;
        }
        break;

//GAME_SELECT
    case GAME_SELECT:
        switch(data.command) {
        case LEFT_COMMAND:
        case RIGHT_COMMAND:
        case UP_COMMAND:
        case DOWN_COMMAND:
            player[data.cid].command.kndP = data.kndP;
            fprintf(stderr, "player[%d].command.kndP = %d\n", data.cid, data.kndP);
            result = 1;
            break;
        case FOUR_COMMAND:
            switch(data.flag) {
            case 3:
                pla_sele.kPflag = data.kPflag;
                player[data.cid].command.kndP = data.kndP;
                result = 1;
                fprintf(stderr, "check %d\n", pla_sele.kPflag);
                break;
            case 4:
                gstate = data.state;
                stageFlag = 1;
                PlayerAllInit(player[myid].command.kndP);
                pla_sele.kPflag = 0;
                result = 1;
                break;
            }
            break;
        case THREE_COMMAND:

            break;
        case END_COMMAND:
            fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
            result = 0; //チャットを終了
            break;
        default:
            fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
            exit(1);
            break;
        }
        break;

//GAME_LOAD
    case GAME_LOAD:
        switch(data.command) {
        case END_COMMAND:
            fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
            result = 0; //チャットを終了
            break;
        default:
            fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
            exit(1);
            break;
        }
        break;

//GAME_MAIN
    case GAME_MAIN:
        switch (data.command) { //クライアントのチャットの行動
        case UP_COMMAND: //メッセージが送信されたら
            player[data.cid].command.up = 1;
            result = 1;
            break;
        case DOWN_COMMAND: //メッセージが送信されたら
            player[data.cid].command.down = 1;
            result = 1;
            break;
        case LEFT_COMMAND: //メッセージが送信されたら
            player[data.cid].command.left = 1;
            result = 1;
            break;
        case RIGHT_COMMAND: //メッセージが送信されたら
            player[data.cid].command.right = 1;
            result = 1;
            break;

        case SEPARATE_UPDO_COMMAND:
            player[data.cid].command.up = 0;
            player[data.cid].command.down = 0;
            /*if(myid != data.cid){
                player[data.cid].tx = data.tx;
                player[data.cid].ty = data.ty;
                }*/
            result = 1;
            break;
        case SEPARATE_LERI_COMMAND:
            player[data.cid].command.left = 0;
            player[data.cid].command.right = 0;
            /*if(myid != data.cid){
                player[data.cid].tx = data.tx;
                player[data.cid].ty = data.ty;
                }*/
            result = 1;
            break;

        case UP_ROTA:
            player[data.cid].command.rotaU = 1;
            result = 1;
            break;
        case RIGHT_ROTA:
            player[data.cid].command.rotaR = 1;
            result = 1;
            break;
        case LEFT_ROTA:
            player[data.cid].command.rotaL = 1;
            result = 1;
            break;

        case UP_SEPA_ROTA:
            player[data.cid].command.rotaU = 0;
            result = 1;
            break;
        case RIGHT_SEPA_ROTA:
            player[data.cid].command.rotaR = 0;
            result = 1;
            break;
        case LEFT_SEPA_ROTA:
            player[data.cid].command.rotaL = 0;
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
            result = 1;
            break;

        case BARRIER_COMMAND:
            player[data.cid].command.b4 = 1;
            fprintf(stderr, "%d:バリアON\n", data.cid);
            result = 1;
            break;
        case BARRIER_FINISH_COMMAND:
            player[data.cid].command.b4 = 0;
            fprintf(stderr, "%d:バリアOFF\n", data.cid);
            result = 1;
            break;

        case PLAYER_HIT:
            PlayerDamage(data, myid, sock);
            result = 1;
            break;
        case PLAYER_HIT2:
            PlayerDamage2(data, myid, sock);
            result = 1;
            break;
        case ENEMY_HIT:
            enemy[data.ene_num] = data.enemy;
            k = data.num;//EnemyDamage(data);
            Total_Score = data.scoreM; //スコアの最大値
            pla_shot[data.m].bullet[data.n].flag = 0;
            if(k > 0 && data.item.knd > 0){ //アイテムの出現
                ItemEnter(data.item);
            }
            switch(stage){
            case 1:
                if(k == 6){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 2:
                if(k == 12){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 3:
                if(k == 18){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 4:
                if(k == 25){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 5:
                if(k == 32){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 6:
                if(k == 39){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 7:
                if(k == 47){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 8:
                if(k == 56){
                    stage++;
                    stageFlag = stage;
                    PlayerBulletClean();
                    EnemyBulletClean();
                }
                break;
            case 9:
                if(k == 63){
                    memset(&data, 0, sizeof(CONTAINER));
                    data.command = END_COMMAND;
                    fprintf(stderr, "%d\n", data.command);
                    data.cid = myid;
                    data.state = gstate;
                    send_data(&data, sizeof(CONTAINER), sock);

                    fprintf(stderr, "////////////////////\n");
                    fprintf(stderr, "//     Clear!     //\n");
                    fprintf(stderr, "////////////////////\n");
                }
                break;
            }
            result = 1;
            break;

        case BARRIER_HIT:
            ene_shot[data.m].bullet[data.n].flag = 0;
            result = 1;
            break;

        case DATA_PULL:
            switch(data.flag){
            case 2://プレイヤーの読み込み(初期)
                player[data.cid] = data.player;
                fprintf(stderr, "%d:Pin! %d\n", data.cid, player[data.cid].knd);
                PlayerShotEnter(data.cid, num_clients);
                if(data.hp > 0){
                    HP_M = data.hp;
                    HP = HP_M;
                }
                break;
            case 3://敵の読み込み
                enemy[data.ene_num] = data.enemy;
                fprintf(stderr, "%d:Ein! knd:%d ene_shot[%d]\n", data.ene_num, enemy[data.ene_num].knd, data.ene_num);
                break;
            case 4:
                player[data.cid] = data.player;
                HP = data.hp;
                break;
            case 5://アイテムゲット・効果
                item[data.m].flag = data.item.flag;//0
                switch(item[data.m].knd){
                case 1:
                    power = data.power;
                    break;
                case 2:
                    HP = data.hp;
                    break;
                case 3:
                    speed = data.speed;
                    SpeedChange(speed);
                    break;
                }
                break;
            }
            break;

        case END_COMMAND: //誰かが退室したら
            fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
            result = 0; //チャットを終了
            break;
        default:
            fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
            exit(1);
            break;
        }
        break;

//GAME_OVER
    case GAME_OVER:
        switch(data.command) {
        case FOUR_COMMAND:

            break;
        case END_COMMAND: //誰かが退室したら
            fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
            result = 0; //チャットを終了
            break;
        default:
            fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
            exit(1);
            break;
        }
        break;

//GAME_CLEAR
    case GAME_CLEAR:
        switch(data.command) {
        case FOUR_COMMAND:

            break;
        case END_COMMAND: //誰かが退室したら
            fprintf(stderr, "client[%d] %s sent quit command.\n", data.cid, clients[data.cid].name);
            result = 0; //チャットを終了
            break;
        default:
            fprintf(stderr, "execute_command(): %c is not a valid command.\n", data.command);    
            exit(1);
            break;
        }
        break;
    }
    return result;
}

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


void HaikeiFree(){
    SDL_FreeSurface(haikei1);
    SDL_FreeSurface(haikei2);
    SDL_FreeSurface(haikei3);
    SDL_FreeSurface(haikei4);
    SDL_FreeSurface(haikei5);
    SDL_FreeSurface(haikei6);
    SDL_FreeSurface(haikei7);
    SDL_FreeSurface(haikei8);
    SDL_FreeSurface(haikei9);
}


void SpeedChange(int sp) {
    int i;
    switch(sp) {
    case 1:
        for(i = 0; i < num_clients; i++){
            player[i].sp = 8;
        }
        break;
    case 2:
        for(i = 0; i < num_clients; i++){
            player[i].sp = 10;
        }
        break;
    case 3:

        break;
    }
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




