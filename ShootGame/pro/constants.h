#include <netinet/in.h>

#define DEFAULT_PORT 51000
#define MAX_LEN_NAME 10
#define MAX_NUM_CLIENTS 5
#define MAX_LEN_BUFFER 256
#define MAX_LEN_ADDR 32
#define BROADCAST -1

typedef struct {
  int cid;
  int sock;
  struct sockaddr_in addr;
  char name[MAX_LEN_NAME];
} CLIENT;

/*typedef struct {
    int cid; //クライアントのID
    char command;
    int tx, ty; //プレイヤーの中心座標を送るときに使用
    int m, n; //敵の打った弾の番号を送るときに使用(ene_shot[m].bullet[n]),(pla_shot[m].bullet[n])
    int ene_num; //敵に弾が当たった時の敵の番号
} CONTAINER;*/
