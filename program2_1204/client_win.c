/*****************************************************************
¥Õ¥¡¥¤¥ëÌ¾	: client_win.c
µ¡Ç½		: ¥¯¥é¥¤¥¢¥ó¥È¤Î¥æ¡¼¥¶¡¼¥¤¥ó¥¿¡¼¥Õ¥§¡¼¥¹½èÍý
*****************************************************************/

#include"common.h"
#include"client_func.h"

static SDL_Joystick *joystick;
static SDL_Surface *haikei;
static SDL_Rect src_rect, dst_rect;

/*****************************************************************
´Ø¿ôÌ¾	: InitWindows
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤ÎÉ½¼¨¡¤ÀßÄê¤ò¹Ô¤¦
°ú¿ô	: int	clientID		: ¥¯¥é¥¤¥¢¥ó¥ÈÈÖ¹æ
		  int	num				: Á´¥¯¥é¥¤¥¢¥ó¥È¿ô
½ÐÎÏ	: Àµ¾ï¤ËÀßÄê¤Ç¤­¤¿¤È¤­0¡¤¼ºÇÔ¤·¤¿¤È¤­-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
    int i;
    char *s,title[10];

    /* °ú¤­¿ô¥Á¥§¥Ã¥¯ */
    assert(0<num && num<=MAX_CLIENTS);
	
    /* SDL¤Î½é´ü²½ */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("failed to initialize SDL.\n");
        return -1;
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
        return -1;
    }

    src_rect = SrcRectInit(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    dst_rect = DstRectInit(0, 0);
    haikei = IMG_Load("sozai/背景-砂漠.png");
    SDL_BlitSurface(haikei, &src_rect, window, &dst_rect);	// ウィンドウ（ここではSDL_GetVideoSurface関数で指定）に画像を貼り付ける    
	
    return 0;
}

/*****************************************************************
´Ø¿ôÌ¾	: DestroyWindow
µ¡Ç½	: SDL¤ò½ªÎ»¤¹¤ë
°ú¿ô	: ¤Ê¤·
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void DestroyWindow(void)
{
    SDL_FreeSurface(window);
    SDL_FreeSurface(haikei);
    PlayerFree();
    EnemyFree();

    SDL_Quit();
}


void DrawGameMain(int pos, int num){
    int i;
    //SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 0,0,128));	// ウィンドウの色を変更
    SDL_BlitSurface(haikei, NULL, window, NULL);	// ウィンドウ（ここではSDL_GetVideoSurface関数で指定）に画像を貼り付ける

    EnemyDraw();

    for(i = 0; i < num; i++){
        if(player[i].command.up == 1)
            DrawUpCircle(i);
        if(player[i].command.down == 1)
            DrawDownCircle(i);
        if(player[i].command.left == 1)
            DrawLeftCircle(i);
        if(player[i].command.right == 1)
            DrawRightCircle(i);
        if(player[i].command.b5 == 1){
            PlayerBulletEnter(i);
        }
        PlayerDraw(i);
    }

    switch(player[pos].knd){ //pos
    case 1:
        EventMainFighter(num);
        break;
    case 2:
        EventMainTank(num);
        break;
    }

    PlayerEnter(num);
    EnemyEnter();

    EnemyMove(num);
    PlayerShotCalc();
    EnemyBulletMove(num);
}


/*****************************************************************
´Ø¿ôÌ¾	: DrawUpCircle
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤Ë±ß¤òÉ½¼¨¤¹¤ë
°ú¿ô	: int		x		: ±ß¤Î x ºÂÉ¸
		  int		y		: ±ß¤Î y ºÂÉ¸
		  int		r		: ±ß¤ÎÈ¾·Â
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void DrawUpCircle(int pos)
{
#ifndef NDEBUG
    printf("#####\n");
    printf("DrawCircle()\n");
    printf("x=%d,y=%d,tyokkei=%d\n",x,y,r);
#endif

    player[pos].dst.y-=player[pos].sp;
    if(player[pos].dst.y < 0)
        player[pos].dst.y = 0;

    player[pos].ty = player[pos].dst.y + player[pos].src.h/2;
}

/*****************************************************************
´Ø¿ôÌ¾	: DrawDownCircle
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤Ë±ß¤òÉ½¼¨¤¹¤ë
°ú¿ô	: int		x		: ±ß¤Î x ºÂÉ¸
		  int		y		: ±ß¤Î y ºÂÉ¸
		  int		r		: ±ß¤ÎÈ¾·Â
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void DrawDownCircle(int pos)
{
#ifndef NDEBUG
    printf("#####\n");
    printf("DrawCircle()\n");
    printf("x=%d,y=%d,tyokkei=%d\n",x,y,r);
#endif

    player[pos].dst.y += player[pos].sp;
    if(player[pos].dst.y + player[pos].src.h > WINDOW_HEIGHT)
        player[pos].dst.y = WINDOW_HEIGHT - player[pos].src.h;

    player[pos].ty = player[pos].dst.y + player[pos].src.h/2;
}

/*****************************************************************
´Ø¿ôÌ¾	: DrawLeftCircle
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤Ë±ß¤òÉ½¼¨¤¹¤ë
°ú¿ô	: int		x		: ±ß¤Î x ºÂÉ¸
		  int		y		: ±ß¤Î y ºÂÉ¸
		  int		r		: ±ß¤ÎÈ¾·Â
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void DrawLeftCircle(int pos)
{
#ifndef NDEBUG
    printf("#####\n");
    printf("DrawCircle()\n");
    printf("x=%d,y=%d,tyokkei=%d\n",x,y,r);
#endif

    player[pos].dst.x-=player[pos].sp;
    if(player[pos].dst.x < 0)
        player[pos].dst.x = 0;

    player[pos].tx = player[pos].dst.x + player[pos].src.w/2;
}

/*****************************************************************
´Ø¿ôÌ¾	: DrawRightCircle
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤Ë±ß¤òÉ½¼¨¤¹¤ë
°ú¿ô	: int		x		: ±ß¤Î x ºÂÉ¸
		  int		y		: ±ß¤Î y ºÂÉ¸
		  int		r		: ±ß¤ÎÈ¾·Â
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
void DrawRightCircle(int pos)
{
#ifndef NDEBUG
    printf("#####\n");
    printf("DrawCircle()\n");
    printf("x=%d,y=%d,tyokkei=%d\n",x,y,r);
#endif

    player[pos].dst.x+=player[pos].sp;
    if(player[pos].dst.x + player[pos].src.w > WINDOW_WIDTH)
        player[pos].dst.x = WINDOW_WIDTH - player[pos].src.w; 

    player[pos].tx = player[pos].dst.x + player[pos].src.w/2;
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




