#include "../common.h"
#include "client_func.h"

static SDL_Surface *gItem1, *gItem2, *gItem3;

void ItemLoad(){
    gItem1 = IMG_Load("sozai/main_resource/05_main_game/main_items/atk_ptn_up.png");
    gItem2 = IMG_Load("sozai/main_resource/05_main_game/main_items/life_up.png");
    gItem3 = IMG_Load("sozai/main_resource/05_main_game/main_items/speed_up.png");
}


void ItemInit(){
    int i;
    for(i = 0; i < ITEM_MAX; i++) {
        item[i].tx = 0;
        item[i].ty = 0;
        item[i].src = SrcRectInit(0, 0, 60, 60);
        item[i].dst = DstRectInit(item[i].tx-item[i].src.w, item[i].ty-item[i].src.h);
        item[i].flag = 0;
        item[i].knd = 0;
    }
}


void ItemDraw(){
    int i = 0;
    for(i = 0; i < ITEM_MAX; i++){
        if(item[i].flag == 1){
            switch(item[i].knd){
            case 1:
                SDL_BlitSurface(gItem1, &item[i].src, window, &item[i].dst);
                break;
            case 2:
                SDL_BlitSurface(gItem2, &item[i].src, window, &item[i].dst);
                break;
            case 3:
                SDL_BlitSurface(gItem3, &item[i].src, window, &item[i].dst);
                break;
            }
        }
    }
}


void ItemMove(int myid, int sock){
    int i;
    for(i = 0; i < ITEM_MAX; i++){
        if(item[i].flag == 1) {
            item[i].tx -= 5;
            item[i].dst = DstRectInit(item[i].tx-item[i].src.w, item[i].ty-item[i].src.h);

//アイテムとプレイヤーの当たり判定
            if(PlayerItemHitJudge(player[myid], item[i])){
                ItemHit(myid, i, sock);
            }
            
//画面外に出たら
            if(item[i].dst.x < -item[i].src.w){
                item[i].flag = 0;
            }
        }
    }
}


void ItemEnter(ItemData a){
    int i = a.num;
    item[i].tx = a.tx;
    item[i].ty = a.ty;
    item[i].src = SrcRectInit(0, 0, 60, 60);
    item[i].dst = DstRectInit(item[i].tx-item[i].src.w, item[i].ty-item[i].src.h);
    item[i].flag = a.flag;
    item[i].knd = a.knd;
}


void ItemFree(){
    SDL_FreeSurface(gItem1);
    SDL_FreeSurface(gItem2);
    SDL_FreeSurface(gItem3);
}



