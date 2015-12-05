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

typedef struct {
    int cid; //クライアントのID
    char command;
    int tx, ty;
} CONTAINER;
