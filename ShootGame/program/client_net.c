/*****************************************************************
ファイル名	: client_net.c
機能		: クライアントのネットワーク処理
*****************************************************************/

#include"common.h"
#include"client_func.h"
#include<errno.h>

#define	BUF_SIZE	100

static int num_clients;
static int myid;
static fd_set mask; //ファイル識別値を管理する構造体
static Client clients[MAX_CLIENTS];

static int	gSocket;	/* ソケット */
static fd_set	gMask;	/* select()用のマスク */
static int	gWidth;		/* gMask中ののチェックすべきビット数 */

static void GetAllName(int *clientID,int *num,char clientNames[][MAX_NAME_SIZE]);
static void SetMask(void);
static int RecvData(void *data,int dataSize);

/*****************************************************************
関数名	: SetUpClient
機能	: サーバーとのコネクションを設立し，
		  ユーザーの名前の送受信を行う
引数	: char	*hostName		: ホスト
		  int	*num			: 全クライアント数
出力	: コネクションに失敗した時-1,成功した時0
*****************************************************************/
int SetUpClient(char *serverName, u_short port)
{
    struct hostent *servHost;
    struct sockaddr_in sv_addr;
    
    fprintf(stderr, "Trying to connect server %s (port = %d).\n", serverName, port);
  	if ((servHost = gethostbyname(serverName)) == NULL) { // IPアドレスを取得する
    	//handle_error("gethostbyname()"); // アドレスが無い時
  	}
  	
  	// Socketシステムコール
  	// Socketの生成
  	gSocket = socket(AF_INET, SOCK_STREAM, 0); //ファイル識別値を管理する構造体
  	if (gSocket < 0) {
    	//handle_error("socket()"); //割り当てが出来なかった時
  	}
  	
  	// connectシステムコール
  	sv_addr.sin_family = AF_INET; // 種類がInternet
  	sv_addr.sin_port = htons(port); // ポート番号
  	sv_addr.sin_addr.s_addr = *(u_int *)servHost->h_addr_list[0];

  	if(connect(gSocket, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) { // サーバーに接続要求
    	//handle_error("connect()"); // sockに接続要求出来なかった時
  	}

  	fprintf(stderr, "Input your name: "); // nameの入力
  	char user_name[MAX_NAME_SIZE];
  	if(fgets(user_name, sizeof(user_name), stdin) == NULL) {
    	//handle_error("fgets()"); // クライアントの名前が無い時
  	}
  	user_name[strlen(user_name) - 1] = '\0';
  	SendData(user_name, MAX_NAME_SIZE);

  	fprintf(stderr, "Waiting for other clients...\n");
  	RecvData(&num_clients, sizeof(int)); // Clientの人数読み込み
  	fprintf(stderr, "Number of clients = %d.\n", num_clients);
  	RecvData(&myid, sizeof(int));
  	fprintf(stderr, "Your ID = %d.\n", myid); // ClientのID読み込み
  	int i;
  	for(i = 0; i < num_clients; i++) {
    	RecvData(&clients[i], sizeof(Client));
  	}

    /* select()のためのマスク値を設定する */
    SetMask();
    
    return 0;
}

/*****************************************************************
関数名	: SendRecvManager
機能	: サーバーから送られてきたデータを処理する
引数	: なし
出力	: プログラム終了コマンドが送られてきた時0を返す．
		  それ以外は1を返す
*****************************************************************/
int SendRecvManager(void)
{
    fd_set	readOK;
    char	command;
    int		i;
    int		endFlag = 1;
    struct timeval	timeout;

    /* select()の待ち時間を設定する */
    timeout.tv_sec = 0;
    timeout.tv_usec = 20;

    readOK = gMask;
    /* サーバーからデータが届いているか調べる */
    select(gWidth,&readOK,NULL,NULL,&timeout);
    if(FD_ISSET(gSocket,&readOK)){
		/* サーバーからデータが届いていた */
    	/* コマンドを読み込む */
		RecvData(&command,sizeof(char));
    	/* コマンドに対する処理を行う */
		endFlag = ExecuteCommand(command);
    }
    return endFlag;
}

/*****************************************************************
関数名	: RecvIntData
機能	: サーバーからint型のデータを受け取る
引数	: int		*intData	: 受信したデータ
出力	: 受け取ったバイト数
*****************************************************************/
int RecvIntData(int *intData)
{
    int n,tmp;
    
    /* 引き数チェック */
    assert(intData!=NULL);

    n = RecvData(&tmp,sizeof(int));
    (*intData) = ntohl(tmp);
    
    return n;
}

/*****************************************************************
関数名	: SendData
機能	: サーバーにデータを送る
引数	: void		*data		: 送るデータ
		  int		dataSize	: 送るデータのサイズ
出力	: なし
*****************************************************************/
void SendData(void *data,int dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 < dataSize);

    write(gSocket,data,dataSize);
}

/*****************************************************************
関数名	: CloseSoc
機能	: サーバーとのコネクションを切断する
引数	: なし
出力	: なし
*****************************************************************/
void CloseSoc(void)
{
    printf("...Connection closed\n");
    close(gSocket);
}

/*****
static
*****/
/*****************************************************************
関数名	: GetAllName
機能	: サーバーから全クライアントのユーザー名を受信する
引数	: int		*num			: クライアント数
		  char		clientNames[][]	: 全クライアントのユーザー名
出力	: なし
*****************************************************************/
static void GetAllName(int *clientID,int *num,char clientNames[][MAX_NAME_SIZE])
{
    int	i;

    /* クライアント番号の読み込み */
    RecvIntData(clientID);
    /* クライアント数の読み込み */
    RecvIntData(num);

    /* 全クライアントのユーザー名を読み込む */
    for(i=0;i<(*num);i++){
		RecvData(clientNames[i],MAX_NAME_SIZE);
    }
#ifndef NDEBUG
    printf("#####\n");
    printf("client number = %d\n",(*num));
    for(i=0;i<(*num);i++){
		printf("%d:%s\n",i,clientNames[i]);
    }
#endif
}

/*****************************************************************
関数名	: SetMask
機能	: select()のためのマスク値を設定する
引数	: なし
出力	: なし
*****************************************************************/
static void SetMask(void)
{
    int	i;

    FD_ZERO(&gMask);
    FD_SET(gSocket,&gMask);

    gWidth = gSocket+1;
}

/*****************************************************************
関数名	: RecvData
機能	: サーバーからデータを受け取る
引数	: void		*data		: 受信したデータ
		  int		dataSize	: 受信するデータのサイズ
出力	: 受け取ったバイト数
*****************************************************************/
int RecvData(void *data,int dataSize)
{
    /* 引き数チェック */
    assert(data != NULL);
    assert(0 < dataSize);

    return read(gSocket,data,dataSize);
}
