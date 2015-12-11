#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../constants.h"
#include "../common.h"

static CLIENT clients[MAX_NUM_CLIENTS]; 
static int num_clients;
static int num_socks;
static fd_set mask; //fd_set:FD(ファイルディスクリプタ)の集合
static CONTAINER data;

void setup_server(int, u_short);
int control_requests();
void terminate_server();

static void send_data(int, void *, int);
static int receive_data(int, void *, int);
static void handle_error(char *);

/**************************
void setup_server(int num_cl, u_short port)
引数：num_client 利用するクライアント数
      port サーバーが使用するポート番号
機能：サーバーのセットアップ
 **************************/
void setup_server(int num_cl, u_short port) {
  int rsock, sock = 0;
  struct sockaddr_in sv_addr, cl_addr;

  fprintf(stderr, "Server setup is started.\n");

  num_clients = num_cl;

  rsock = socket(AF_INET, SOCK_STREAM, 0); //ソケット生成
  if (rsock < 0) handle_error("socket()");
  fprintf(stderr, "sock() for request socket is done successfully.\n"); //成功
    
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(port); //IPネットワークバイトオーダーの（16bit）形式で変換
  sv_addr.sin_addr.s_addr = INADDR_ANY; //任意アドレスから受付可

  int opt = 1;
//ソケットのオプションの設定と取得を行う
  setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(rsock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) handle_error("bind()"); //bind:ソケットに設定を結びつける
  fprintf(stderr, "bind() is done successfully.\n");

  if (listen(rsock, num_clients) != 0) handle_error("listen()"); //接続ソケットの準備
  fprintf(stderr, "listen() is started.\n");

//クライアントの入室
  int i, max_sock = 0;
  socklen_t len;
  char src[MAX_LEN_ADDR];
  for (i = 0; i < num_clients; i++) { //クライアントが全員入ったら抜ける
    len = sizeof(cl_addr);
    sock = accept(rsock, (struct sockaddr *)&cl_addr, &len); //クライアントアドレスと接続の確立
    if (sock < 0) handle_error("accept()");

    if (max_sock < sock) max_sock = sock;

    if(read(sock, clients[i].name, MAX_LEN_NAME) == -1) handle_error("read()"); //read:データの読み込み(引数１：接続ソケット　引数２：読み込み先のバッファ　引数３：読み込みバイト数)
    clients[i].cid = i;
    clients[i].sock = sock;
    clients[i].addr = cl_addr;
    memset(src, 0, sizeof(src));
//inet_ntop:引数２のアドレスをバイナリ形式→テキスト形式に変換し、引数３に返す
    inet_ntop(AF_INET, (struct sockaddr *)&cl_addr.sin_addr, src, sizeof(src));
    fprintf(stderr, "Client %d is accepted (name=%s, address=%s, port=%d).\n", i, clients[i].name, src, ntohs(cl_addr.sin_port)); //ntohs:IPネットワークバイトオーダー→ホストバイトオーダーに変換
  }
  
  close(rsock);

  int j;
  for (i = 0; i < num_clients; i++) {
      send_data(i, &num_clients, sizeof(int));
    send_data(i, &i, sizeof(int));
    for (j = 0; j < num_clients; j++) {
      send_data(i, &clients[j], sizeof(CLIENT));
    }
  }

  num_socks = max_sock + 1;
  FD_ZERO(&mask); //引数をゼロクリア
  FD_SET(0, &mask); //0番目のFDに対応する値を1にセット

  for(i = 0; i < num_clients; i++) {
    FD_SET(clients[i].sock, &mask);
  }
  fprintf(stderr, "Server setup is done.\n"); //サーバーのセットアップ完了
}

/************************
int control_requests()
引数：なし
機能：サーバー中の動作
返値：1…メッセージ送信　0…チャットプログラム終了
************************/
int control_requests() {
  fd_set read_flag = mask;
  memset(&data, 0, sizeof(CONTAINER)); //dataの先頭から"sizeof(CONTAINER)"バイト分'0'をセット(全て0)
  
  fprintf(stderr, "select() is started.\n");
//select:FDの集合から読み込み可(引数２)/書き込み可(引数３)/例外発生(引数４)のFDを発見
  if(select(num_socks, (fd_set *)&read_flag, NULL, NULL, NULL) == -1) handle_error("select()");

  int i, result = 1;
  for (i = 0; i < num_clients; i++) {
      if(FD_ISSET(clients[i].sock, &read_flag)) {//FD_ISSET:[引数１]番目のFDが1かどうかの確認
	receive_data(i, &data, sizeof(data)); //読み込み先から引数２へのデータの読み込み
	switch(data.command) {
        case LEFT_COMMAND:
	case RIGHT_COMMAND:
        case UP_COMMAND:
	case DOWN_COMMAND:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
	  break;
	case SEPARATE_UPDO_COMMAND:
	case SEPARATE_LERI_COMMAND:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
	  break;
        case UP_ROTA:
        case RIGHT_ROTA:
        case LEFT_ROTA:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
            break;
        case UP_SEPA_ROTA:
        case RIGHT_SEPA_ROTA:
        case LEFT_SEPA_ROTA:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
            break;
	case SHOT_COMMAND:
	case SHOT_FINISH_COMMAND:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
	  break;
        case PLAYER_HIT:
        case PLAYER_HIT2:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
          break;
        case ENEMY_HIT:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 1; //メッセージの送信
          break;
	case END_COMMAND:
	  send_data(BROADCAST, &data, sizeof(data));
	  result = 0; //チャットプログラムの終了
	  break;
	default:
	  fprintf(stderr, "control_requests(): %c is not a valid command.\n", data.command);
	  exit(1);
	}
    }
  }
  return result;   
}

/***********************
static void send_data(int cid, void *data, int size)
引数：cid クライアントの番号(サーバー側：BROADCAST)
      *data 書きこむデータ
      size データの大きさ
機能：データをクライアントソケットに入れる
***********************/
static void send_data(int cid, void *data, int size) {
    if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) { //クライアントのエラー
    fprintf(stderr, "send_data(): client id is illeagal.\n");
    exit(1);
  }
    if ((data == NULL) || (size <= 0)) { //サーバーのエラー
        fprintf(stderr, "send_data(): data is illeagal.\n");
	exit(1);
  }

    if (cid == BROADCAST) { //サーバー
    int i;
    for (i = 0; i < num_clients; i++) {
//write:引数１のファイルディスクリプタに引数２バッファの引数３バイト分書きこむ
      if (write(clients[i].sock, data, size) < 0) handle_error("write()");
    }
    } else { //クライアント
    if (write(clients[cid].sock, data, size) < 0) handle_error("write()");
  }  
}

/**********************
static int receive_data(int cid, void *data, int size)
引数：cid クライアント番号
      *data 読み込みデータ
      size 読み込みデータのサイズ
機能：コネクションからバッファへのデータの読み込み
返値：読み込み結果
***********************/
static int receive_data(int cid, void *data, int size) {
  if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) {
    fprintf(stderr, "receive_data(): client id is illeagal.\n");
    exit(1);
  }
  if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "receive_data(): data is illeagal.\n");
	exit(1);
  }

  return read(clients[cid].sock, data, size); //引数１：接続ソケット　引数２：読み込み先のバッファ　引数３：読み込みバイト数
}

/***************************************
static void handle_error(char *message)
引数：*message 送るメッセージ
機能：エラーメッセージの送信
****************************************/
static void handle_error(char *message) {
  perror(message);
  fprintf(stderr, "%d\n", errno);
  exit(1);
}

/***************************************
void terminate_server(void)
引数：なし
機能：サーバーの終了
****************************************/
void terminate_server(void) {
  int i;
  for (i = 0; i < num_clients; i++) {
    close(clients[i].sock);
  }
  fprintf(stderr, "All connections are closed.\n");
  exit(0);
}
