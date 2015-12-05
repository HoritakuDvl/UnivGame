/*****************************************************************
¥Õ¥¡¥¤¥ëÌ¾	: client_win.c
µ¡Ç½		: ¥¯¥é¥¤¥¢¥ó¥È¤Î¥æ¡¼¥¶¡¼¥¤¥ó¥¿¡¼¥Õ¥§¡¼¥¹½èÍý
*****************************************************************/

#include"common.h"
#include"client_func.h"

static SDL_Joystick *joystick;
static IplImage *gPlayer, *gPlayerMask, *gPlayerMask2, *gPlayerZero;

static void WindowMainEvent(int num, int c);
static void PlayerKuromaki();
static void PlayerMoveMask(int x, int y);

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

    for(i = 0; i < SDL_NumJoysticks(); i++){
        printf("%s\n", SDL_JoystickName(i));
    }
    joystick = SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);
    if(!joystick){
        fprintf(stderr, "failed to open joystick.\n");
        return -1;
    }

    img = cvLoadImage("sozai/背景-砂漠.png", CV_LOAD_IMAGE_COLOR);//cvLoadImage  : 画像を読み込む関数
    if(img == NULL){
        fprintf(stderr, "not picture.\n");
        return -1;
    }
    img2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cvSet(img2, cvScalar(0, 0, 0, 0), NULL);
    cvCopy(img, img2, NULL);

    gPlayer = cvLoadImage("sozai/player.png", CV_LOAD_IMAGE_COLOR);

    gPlayerZero = cvCreateImage(cvGetSize(gPlayer), IPL_DEPTH_8U, 3);
    cvSet(gPlayerZero, cvScalar(0, 0, 0, 0), NULL);

    gPlayerMask = cvLoadImage("sozai/player_1.png", CV_LOAD_IMAGE_COLOR);

    gPlayerMask2 = cvCreateImage(cvGetSize(gPlayerMask), IPL_DEPTH_8U, 3);
    cvSet(gPlayerMask2, cvScalar(0, 0, 0, 0), NULL);


    for(i = 0; i < 2; i++){
        player[i].center.x = 100;
        player[i].center.y = 100*(i+1);
        player[i].x = 100;
        player[i].y = 100*(i+1);
        player[i].r = 5;
        player[i].command = 0;
    }

    if(cvNamedWindow("a", CV_WINDOW_NORMAL | CV_WINDOW_FREERATIO) == 0){
        fprintf(stderr, "a\n");
        exit(1);
    }
    cvResizeWindow("a", 1600, 900);
    cvShowImage("a", img2);

    cvNamedWindow("b", CV_WINDOW_NORMAL);
    cvNamedWindow("c", CV_WINDOW_NORMAL);
	
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
    SDL_Quit();
    cvDestroyWindow("a");
    cvDestroyWindow("b");
    cvDestroyWindow("c");

    cvReleaseImage(&img);
    cvReleaseImage(&img2);
    cvReleaseImage(&gPlayer);

    cvReleaseImage(&gPlayerMask);
    cvReleaseImage(&gPlayerMask2);
    cvReleaseImage(&gPlayerZero);
}


void DrawGameMain(int num){
    int i;
    cvCopy(img, img2, NULL); //画面を戻す
    PlayerKuromaki();

    for(i = 0; i < 2; i++){
        if(player[i].command > 0){
            if(player[i].command%2 == 1)
                DrawUpCircle(i);
            if((player[i].command/2)%2 == 1)
                DrawDownCircle(i);
            if((player[i].command/4)%2 == 1)
                DrawLeftCircle(i);
            if((player[i].command/8)%2 == 1)
                DrawRightCircle(i);
        }
    }
    int c = cvWaitKey(1); //キー入力
    WindowMainEvent(num, c);

    //cvAdd(img, gPlayer, img2, NULL); //サイズが合わない
    for(i = 0; i < 2; i++){
        cvCircle(img2, player[i].center, player[i].r, CV_RGB(255, 255, 255), -1, 8, 0);
    }
    cvShowImage("a", img2);

}


static void PlayerKuromaki(){
    CvScalar lower = cvScalar(0, 0, 0, 0);
    CvScalar upper = cvScalar(255, 255, 255, 0);
    IplImage *gImage;
    gImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cvSet(gImage, cvScalar(0, 0, 0, 0), NULL);
    cvSetZero(gImage);

    cvCopy(img, gImage, gPlayerMask); //範囲外は黒くなる

    cvNot(gPlayerMask, gPlayerMask2);
    cvSetZero(gPlayerZero);
    cvCopy(gPlayer, gPlayerZero, gPlayerMask2);

    cvAdd(gImage, gPlayerZero, img2, NULL);

    cvShowImage("b", gImage);
    cvShowImage("c", gPlayerZero);

    cvReleaseImage(&gImage);
}


/*****************************************************************
´Ø¿ôÌ¾	: WindowEvent
µ¡Ç½	: ¥á¥¤¥ó¥¦¥¤¥ó¥É¥¦¤ËÂÐ¤¹¤ë¥¤¥Ù¥ó¥È½èÍý¤ò¹Ô¤¦
°ú¿ô	: int		num		: Á´¥¯¥é¥¤¥¢¥ó¥È¿ô
½ÐÎÏ	: ¤Ê¤·
*****************************************************************/
static void WindowMainEvent(int num, int c)
{
    SDL_Event event;
    int buttonNO;

    /* °ú¤­¿ô¥Á¥§¥Ã¥¯ */
    assert(0<num && num<=MAX_CLIENTS);

//キーボードから
    //fprintf(stderr, "c = %x\n", c);
    if(c == 0x10001B)
        SendEndCommand();
    else if(c == 0x10ff52)
        SendUpCommand(1);
    else if(c == 0x10ff54)
        SendDownCommand(1);
    else if(c == 0x10ff51)
        SendLeftCommand(1);
    else if(c == 0x10ff53)
        SendRightCommand(1);

//コントローラーから
    else if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
            SendEndCommand();
            break;

	case SDL_JOYAXISMOTION:
            printf("The axis ID of the operated key is %d.\n",event.jaxis.axis);	// 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
            if(event.jaxis.axis==0){
                if(event.jaxis.value == -32768){
                    SendLeftCommand(1);
                    printf("左\n");
                }
                if(event.jaxis.value == 32767){
                    SendRightCommand(1);
                    printf("右\n");
                }
                //printf("--- Analog-Direction Key: ?? Axis\n");
            }
            else if(event.jaxis.axis==1){
                if(event.jaxis.value == -32768){
                    SendUpCommand(1);
                    //cvShowImage("a", gPlayer); //画面チェンジができる
                    printf("上\n");
                }
                if(event.jaxis.value == 32767){
                    SendDownCommand(1);
                    printf("下\n");
                }
                //printf("--- Anolag-Direction Key: ?? Axis\n");
            }
            /*else if(event.jaxis.axis==2){
                printf("--- Four-Direction Key: Horizontal Axis\n");
            }
            else if(event.jaxis.axis==3){
                printf("--- Four-Direction Key: Vertical Axis\n");
                }*/						
            printf("--- The axis value of the operated key is %d.\n",event.jaxis.value);	// ジョイスティックの操作された方向キーの値を表示（-32767（真左，真上）～32767（真右，真下））
            if(event.jaxis.value == 0){
                SendSeparateCommand(1, event.jaxis.axis);
                printf("%d : 左スティックを離した\n", event.jaxis.axis);
            }
            break;
            // ジョイスティックのボタンが押された時
        case SDL_JOYBUTTONDOWN:
            printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            if(event.jbutton.button==0){
                printf("--- You pressed a button on the joystick.\n");
            }
            break;
            // ボタンが離された時
        case SDL_JOYBUTTONUP:
            printf("The ID of the released button is %d.\n", event.jbutton.button);	// 離されたボタンのIDを表示（0から）
            // ボタンIDに応じた処理
            if(event.jbutton.button==0){
                printf("--- You released a button on the joystick.\n");
            }
            break;
        }
    }
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

    if(player[pos].y > 100){
        //circleColor(gMainWindow,x,y,r,0x000000ff);
        player[pos].center.y-=5;
        player[pos].y-=5;
        //SDL_Flip(gMainWindow);

        PlayerMoveMask(0, -5);
    }
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

    if(player[pos].y < WINDOW_HEIGHT){
        player[pos].center.y += 5;
        player[pos].y+=5;

        PlayerMoveMask(0, 5);    
    }
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

    if(player[pos].x > 100){
        player[pos].center.x-=5;
        player[pos].x-=5;
    

        PlayerMoveMask(-5, 0);
    }
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

    if(player[pos].x < WINDOW_WIDTH-100){
        player[pos].center.x+=5;
        player[pos].x+=5;

        PlayerMoveMask(5,0);
    }
}


static void PlayerMoveMask(int x, int y){
    IplImage *gPlayer2, *gPlayerMask2_2; //座標移動時に使用
    CvPoint2D32f original[3], translate[3];

    gPlayer2 = cvCreateImage(cvGetSize(gPlayer), IPL_DEPTH_8U, 3);
    cvSet(gPlayer2, cvScalar(0, 0, 0, 0), NULL);
    gPlayerMask2_2 = cvCreateImage(cvGetSize(gPlayerMask), IPL_DEPTH_8U, 3);
    cvSet(gPlayerMask2_2, cvScalar(0, 0, 0, 0), NULL);


    original[0] = cvPoint2D32f(0,0);
    original[1] = cvPoint2D32f(gPlayer->width, 0);
    original[2] = cvPoint2D32f(0,gPlayer->height);

    translate[0] = cvPoint2D32f(x, y);
    translate[1] = cvPoint2D32f(gPlayer->width + x, y);
    translate[2] = cvPoint2D32f(x, gPlayer->height + y);

    CvMat* affineMatrix = cvCreateMat(2, 3, CV_32FC1);
    cvGetAffineTransform(original, translate, affineMatrix);

    cvWarpAffine(gPlayer, gPlayer2, affineMatrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
    cvCopy(gPlayer2, gPlayer, NULL);
    cvWarpAffine(gPlayerMask, gPlayerMask2_2, affineMatrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
    cvCopy(gPlayerMask2_2, gPlayerMask, NULL);


    cvReleaseImage(&gPlayer2);
    cvReleaseImage(&gPlayerMask2_2);
}
