/*****************************************************************
�ե�����̾	: client_net.c
��ǽ		: ���饤����ȤΥͥåȥ������
*****************************************************************/

#include"common.h"
#include"client_func.h"
#include<errno.h>

#define	BUF_SIZE	100

static int num_clients;
static int myid;
static fd_set mask; //�ե����뼱���ͤ�������빽¤��
static Client clients[MAX_CLIENTS];

static int	gSocket;	/* �����å� */
static fd_set	gMask;	/* select()�ѤΥޥ��� */
static int	gWidth;		/* gMask��ΤΥ����å����٤��ӥåȿ� */

static void GetAllName(int *clientID,int *num,char clientNames[][MAX_NAME_SIZE]);
static void SetMask(void);
static int RecvData(void *data,int dataSize);

/*****************************************************************
�ؿ�̾	: SetUpClient
��ǽ	: �����С��ȤΥ��ͥ���������Ω����
		  �桼������̾������������Ԥ�
����	: char	*hostName		: �ۥ���
		  int	*num			: �����饤����ȿ�
����	: ���ͥ������˼��Ԥ�����-1,����������0
*****************************************************************/
int SetUpClient(char *serverName, u_short port)
{
    struct hostent *servHost;
    struct sockaddr_in sv_addr;
    
    fprintf(stderr, "Trying to connect server %s (port = %d).\n", serverName, port);
  	if ((servHost = gethostbyname(serverName)) == NULL) { // IP���ɥ쥹���������
    	//handle_error("gethostbyname()"); // ���ɥ쥹��̵����
  	}
  	
  	// Socket�����ƥॳ����
  	// Socket������
  	gSocket = socket(AF_INET, SOCK_STREAM, 0); //�ե����뼱���ͤ�������빽¤��
  	if (gSocket < 0) {
    	//handle_error("socket()"); //������Ƥ�����ʤ��ä���
  	}
  	
  	// connect�����ƥॳ����
  	sv_addr.sin_family = AF_INET; // ���बInternet
  	sv_addr.sin_port = htons(port); // �ݡ����ֹ�
  	sv_addr.sin_addr.s_addr = *(u_int *)servHost->h_addr_list[0];

  	if(connect(gSocket, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) { // �����С�����³�׵�
    	//handle_error("connect()"); // sock����³�׵����ʤ��ä���
  	}

  	fprintf(stderr, "Input your name: "); // name������
  	char user_name[MAX_NAME_SIZE];
  	if(fgets(user_name, sizeof(user_name), stdin) == NULL) {
    	//handle_error("fgets()"); // ���饤����Ȥ�̾����̵����
  	}
  	user_name[strlen(user_name) - 1] = '\0';
  	SendData(user_name, MAX_NAME_SIZE);

  	fprintf(stderr, "Waiting for other clients...\n");
  	RecvData(&num_clients, sizeof(int)); // Client�οͿ��ɤ߹���
  	fprintf(stderr, "Number of clients = %d.\n", num_clients);
  	RecvData(&myid, sizeof(int));
  	fprintf(stderr, "Your ID = %d.\n", myid); // Client��ID�ɤ߹���
  	int i;
  	for(i = 0; i < num_clients; i++) {
    	RecvData(&clients[i], sizeof(Client));
  	}

    /* select()�Τ���Υޥ����ͤ����ꤹ�� */
    SetMask();
    
    return 0;
}

/*****************************************************************
�ؿ�̾	: SendRecvManager
��ǽ	: �����С����������Ƥ����ǡ������������
����	: �ʤ�
����	: �ץ���ཪλ���ޥ�ɤ������Ƥ�����0���֤���
		  ����ʳ���1���֤�
*****************************************************************/
int SendRecvManager(void)
{
    fd_set	readOK;
    char	command;
    int		i;
    int		endFlag = 1;
    struct timeval	timeout;

    /* select()���Ԥ����֤����ꤹ�� */
    timeout.tv_sec = 0;
    timeout.tv_usec = 20;

    readOK = gMask;
    /* �����С�����ǡ������Ϥ��Ƥ��뤫Ĵ�٤� */
    select(gWidth,&readOK,NULL,NULL,&timeout);
    if(FD_ISSET(gSocket,&readOK)){
		/* �����С�����ǡ������Ϥ��Ƥ��� */
    	/* ���ޥ�ɤ��ɤ߹��� */
		RecvData(&command,sizeof(char));
    	/* ���ޥ�ɤ��Ф��������Ԥ� */
		endFlag = ExecuteCommand(command);
    }
    return endFlag;
}

/*****************************************************************
�ؿ�̾	: RecvIntData
��ǽ	: �����С�����int���Υǡ�����������
����	: int		*intData	: ���������ǡ���
����	: ������ä��Х��ȿ�
*****************************************************************/
int RecvIntData(int *intData)
{
    int n,tmp;
    
    /* �����������å� */
    assert(intData!=NULL);

    n = RecvData(&tmp,sizeof(int));
    (*intData) = ntohl(tmp);
    
    return n;
}

/*****************************************************************
�ؿ�̾	: SendData
��ǽ	: �����С��˥ǡ���������
����	: void		*data		: ����ǡ���
		  int		dataSize	: ����ǡ����Υ�����
����	: �ʤ�
*****************************************************************/
void SendData(void *data,int dataSize)
{
    /* �����������å� */
    assert(data != NULL);
    assert(0 < dataSize);

    write(gSocket,data,dataSize);
}

/*****************************************************************
�ؿ�̾	: CloseSoc
��ǽ	: �����С��ȤΥ��ͥ����������Ǥ���
����	: �ʤ�
����	: �ʤ�
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
�ؿ�̾	: GetAllName
��ǽ	: �����С����������饤����ȤΥ桼����̾���������
����	: int		*num			: ���饤����ȿ�
		  char		clientNames[][]	: �����饤����ȤΥ桼����̾
����	: �ʤ�
*****************************************************************/
static void GetAllName(int *clientID,int *num,char clientNames[][MAX_NAME_SIZE])
{
    int	i;

    /* ���饤������ֹ���ɤ߹��� */
    RecvIntData(clientID);
    /* ���饤����ȿ����ɤ߹��� */
    RecvIntData(num);

    /* �����饤����ȤΥ桼����̾���ɤ߹��� */
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
�ؿ�̾	: SetMask
��ǽ	: select()�Τ���Υޥ����ͤ����ꤹ��
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
static void SetMask(void)
{
    int	i;

    FD_ZERO(&gMask);
    FD_SET(gSocket,&gMask);

    gWidth = gSocket+1;
}

/*****************************************************************
�ؿ�̾	: RecvData
��ǽ	: �����С�����ǡ�����������
����	: void		*data		: ���������ǡ���
		  int		dataSize	: ��������ǡ����Υ�����
����	: ������ä��Х��ȿ�
*****************************************************************/
int RecvData(void *data,int dataSize)
{
    /* �����������å� */
    assert(data != NULL);
    assert(0 < dataSize);

    return read(gSocket,data,dataSize);
}
