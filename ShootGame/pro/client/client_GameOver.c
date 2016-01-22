#include "../common.h"
#include "client_func.h"

static SDL_Surface *haikei;
static SDL_Surface *gameoverLabel;
static SDL_Surface *startBtn;
static SDL_Surface *gMark1, *gMark2, *gMark3, *gMark4;
static SDL_Rect src_rect, dst_rect;

static void MarkDraw(int n);

void GameoverLoad() {

    haikei = IMG_Load("sozai/main_resource/07_gameover/gameover_bg.png");
    gameoverLabel = IMG_Load("sozai/main_resource/07_gameover/gameover_label.png");
    startBtn = IMG_Load("sozai/main_resource/00_common/common_back_start_btn.jpg");
    
    gMark1 = IMG_Load("sozai/main_resource/00_common/common_frame1.png");
    gMark2 = IMG_Load("sozai/main_resource/00_common/common_frame2.png");
    gMark3 = IMG_Load("sozai/main_resource/00_common/common_frame3.png");
    gMark4 = IMG_Load("sozai/main_resource/00_common/common_frame4.png");

    int i;
    for(i = 0; i < MAX_CLIENTS; i++) {
        player[i].command.kndP = i;
    	pla_sele.kPflag = 0;
    }
}

/************************************
void PlayerSelect(int num)
引数：num クライアント数
機能：
返値：
***********************************/
void Gameover(int myid, int num) {

//背景
    src_rect = SrcRectInit(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    dst_rect = DstRectInit(0, 0);
    SDL_BlitSurface(haikei, &src_rect, window, &dst_rect);

//クリア
    src_rect = SrcRectInit(0, 0, 1200, 200);
    dst_rect = DstRectInit(WINDOW_WIDTH / 2 - 600, 100);
    SDL_BlitSurface(gameoverLabel, &src_rect, window, &dst_rect);

//スタートボタンへ
    src_rect = SrcRectInit(0, 0, 200, 60);
    dst_rect = DstRectInit(WINDOW_WIDTH / 2 - 100, 700);
    SDL_BlitSurface(startBtn, &src_rect, window, &dst_rect);

//マーク
    fprintf(stderr, "stageCount = %4d\n", stageCount);
    if(stageCount >= 200){
	int i;
        for(i = 0; i < num; i++) {
            if(i != myid) {
                MarkDraw(i);
            }
        }
        MarkDraw(myid);
    }

//スコア表示
    StringDraw(Total_Score, 4);

}

static void MarkDraw(int n) {

//マーク
    src_rect = SrcRectInit(0, 0, 214, 74);
    dst_rect = DstRectInit(WINDOW_WIDTH / 2 - 107, 693);
    switch (n) {
    case 0:
        SDL_BlitSurface(gMark1, &src_rect, window, &dst_rect);
        break;
    case 1:
        SDL_BlitSurface(gMark2, &src_rect, window, &dst_rect);
        break;
    case 2:
        SDL_BlitSurface(gMark3, &src_rect, window, &dst_rect);
        break;
    case 3:
        SDL_BlitSurface(gMark4, &src_rect, window, &dst_rect);
        break;
    }
}

void GameoverFree() {
    SDL_FreeSurface(haikei);
    SDL_FreeSurface(gameoverLabel);
    SDL_FreeSurface(startBtn);

    SDL_FreeSurface(gMark1);
    SDL_FreeSurface(gMark2);
    SDL_FreeSurface(gMark3);
    SDL_FreeSurface(gMark4);
}
