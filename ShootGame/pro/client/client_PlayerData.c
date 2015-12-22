#include "../common.h"
#include "client_func.h"

static SDL_Surface *gPlayer, *gPTama0;
static SDL_Surface *gPlayer2, *gPlayer2_2;
static SDL_Surface *gPlayer3, *gPTama1;
static SDL_Surface *gPlayer4;
static SDL_Surface *gPlayer5;
static SDL_Surface *gPlayer6;
static SDL_Surface *gPlayer7, *gPlayer7_2;
static SDL_Surface *gPlayer8, *gPlayer8_2;
static SDL_Surface *gPlayer9, *gPlayer9_2;


//static void PlayerShotEnter(int n, int num);

void(*PlayerShotPattern[PLAYER_SHOT_PATTERN_MAX])(int) = {
    PlayerShotPattern0, PlayerShotPattern1
};


void PlayerLoad(){
    gPlayer = IMG_Load("sozai/player1.png");

    gPlayer2 = IMG_Load("sozai/player2.png");
    gPlayer2_2 = IMG_Load("sozai/houdai2.png");

    gPlayer3 = IMG_Load("sozai/test_design/player/fighters/fighter_1.png");
    gPlayer4 = IMG_Load("sozai/test_design/player/fighters/fighter_2.png");
    gPlayer5 = IMG_Load("sozai/test_design/player/fighters/fighter_3.png");
    gPlayer6 = IMG_Load("sozai/test_design/player/fighters/fighter_4.png");

    gPlayer7 = IMG_Load("sozai/test_design/player/tanks/bodies/tank_body_1.png");
    gPlayer7_2 = IMG_Load("sozai/test_design/player/tanks/batteries/battery1.png");
    gPlayer8 = IMG_Load("sozai/test_design/player/tanks/bodies/tank_body_2.png");
    gPlayer8_2 = IMG_Load("sozai/test_design/player/tanks/batteries/battery2.png");
    gPlayer9 = IMG_Load("sozai/test_design/player/tanks/bodies/tank_body_4.png");
    gPlayer9_2 = IMG_Load("sozai/test_design/player/tanks/batteries/battery4.png");


    gPTama0 = IMG_Load("sozai/Ptama0.png");
    gPTama1 = IMG_Load("sozai/test_design/player/fighters/fighter_shots/fighter_shot_1.png");
}


void PlayerInit(int num){
    int i, j;
    for(i = 0; i < num; i++){
        player[i].flag = 0;
        player[i].knd = 0;
    }
    //HP_Max = 0;
    //HP_Num = HP_Max;

    for(i = 0; i < PLAYER_SHOT_MAX; i++){
        pla_shot[i].flag = 0;
        for(j = 0 ; j < SHOT_BULLET_MAX; j++){
            pla_shot[i].bullet[j].flag = 0;
        }
    }
}


void PlayerDraw(int pos){
    int i, j;
    SDL_Surface *Pl;
    Sint16 wid, hig;
    SDL_Rect pl_src, pl_dst;

//弾
    for(i = 0 ; i < PLAYER_SHOT_MAX; i++){
        if(pos == pla_shot[i].num){
            if(pla_shot[i].flag == 1){
                for(j = 0; j < SHOT_BULLET_MAX; j++){
                    if(pla_shot[i].bullet[j].flag > 0){
                        switch(pla_shot[i].bullet[j].knd){
                        case 0:

                            switch(player[pos].knd){
                            case 1:
                            case 2:
                                Pl = rotozoomSurface(gPTama0, -pla_shot[i].bullet[j].rad, 1, 0);
                                wid = (Sint16)((Pl->w-35)/2+0.5);
                                hig = (Sint16)((Pl->h-35)/2+0.5);
                                pl_dst = DstRectInit(pla_shot[i].bullet[j].dst.x-wid, pla_shot[i].bullet[j].dst.y-hig);

                                SDL_BlitSurface(Pl, &pla_shot[i].bullet[j].src, window, &pl_dst);

                                SDL_FreeSurface(Pl);
				break;

                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                //case 7:
                                //case 8:
                                case 9:
                                Pl = rotozoomSurface(gPTama1, -pla_shot[i].bullet[j].rad, 1, 0);
                                wid = (Sint16)((Pl->w-35)/2+0.5);
                                hig = (Sint16)((Pl->h-35)/2+0.5);
                                pl_dst = DstRectInit(pla_shot[i].bullet[j].dst.x-wid, pla_shot[i].bullet[j].dst.y-hig);

                                SDL_BlitSurface(Pl, &pla_shot[i].bullet[j].src, window, &pl_dst);

                                SDL_FreeSurface(Pl);
				break;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }


//プレイヤー(pos:各クライアントの番号)

    if(player[pos].flag > 0){
        int cnt = player[pos].flag2 % 14;
        if(cnt <= 4 || 11 <= cnt) {
            switch(player[pos].knd2){
            case 1://戦闘機のとき
                switch(player[pos].knd){
                case 1:
                    SDL_BlitSurface(gPlayer, &player[pos].src, window, &player[pos].dst);
                    break;
                case 3:
                    SDL_BlitSurface(gPlayer3, &player[pos].src, window, &player[pos].dst);
                    break;
                case 4:
                    SDL_BlitSurface(gPlayer4, &player[pos].src, window, &player[pos].dst);
                    break;
                case 5:
                    SDL_BlitSurface(gPlayer5, &player[pos].src, window, &player[pos].dst);
                    break;
                case 6:
                    SDL_BlitSurface(gPlayer6, &player[pos].src, window, &player[pos].dst);
                    break;
                }
                break;
            case 2://戦車のとき
                switch(player[pos].knd){
                case 2: //見本
                    //砲台
                    Pl = rotozoomSurface(gPlayer2_2, -player[pos].rad, 1, 0);
                    wid = (Sint16)((Pl->w-35)/2+0.5);
                    hig = (Sint16)((Pl->h-35)/2+0.5);

                    pl_src = SrcRectInit(0, 0, player[pos].src2.w + 50, player[pos].src2.h + 50);
                    pl_dst = DstRectInit(player[pos].dst2.x-wid, player[pos].dst2.y-hig);
                    SDL_BlitSurface(Pl, &pl_src, window, &pl_dst);

                    SDL_FreeSurface(Pl);   

                    //本体
                    SDL_BlitSurface(gPlayer2, &player[pos].src, window, &player[pos].dst);
                    circleColor(window, player[pos].tx, player[pos].ty, 75, 0xff0000ff);
                    break;

                case 7: //テスト用１
                    //砲台
                    Pl = rotozoomSurface(gPlayer7_2, -player[pos].rad, 1, 0);
                    wid = (Sint16)((Pl->w-35)/2+0.5);
                    hig = (Sint16)((Pl->h-35)/2+0.5);

                    pl_src = SrcRectInit(0, 0, player[pos].src2.w + 50, player[pos].src2.h + 50);
                    pl_dst = DstRectInit(player[pos].dst2.x-wid, player[pos].dst2.y-hig);
                    SDL_BlitSurface(Pl, &pl_src, window, &pl_dst);

                    SDL_FreeSurface(Pl);   

                    //本体
                    SDL_BlitSurface(gPlayer7, &player[pos].src, window, &player[pos].dst);
                    circleColor(window, player[pos].tx, player[pos].ty, 75, 0xff0000ff);
                    break;

                case 8: //テスト用１
                    //砲台
                    Pl = rotozoomSurface(gPlayer8_2, -player[pos].rad, 1, 0);
                    wid = (Sint16)((Pl->w-35)/2+0.5);
                    hig = (Sint16)((Pl->h-35)/2+0.5);

                    pl_src = SrcRectInit(0, 0, player[pos].src2.w + 50, player[pos].src2.h + 50);
                    pl_dst = DstRectInit(player[pos].dst2.x-wid, player[pos].dst2.y-hig);
                    SDL_BlitSurface(Pl, &pl_src, window, &pl_dst);

                    SDL_FreeSurface(Pl);   

                    //本体
                    SDL_BlitSurface(gPlayer8, &player[pos].src, window, &player[pos].dst);
                    circleColor(window, player[pos].tx, player[pos].ty, 75, 0xff0000ff);
                    break;

                case 9: //テスト用１
                    //砲台
                    Pl = rotozoomSurface(gPlayer9_2, -player[pos].rad, 1, 0);
                    wid = (Sint16)((Pl->w-35)/2+0.5);
                    hig = (Sint16)((Pl->h-35)/2+0.5);

                    pl_src = SrcRectInit(0, 0, player[pos].src2.w + 50, player[pos].src2.h + 50);
                    pl_dst = DstRectInit(player[pos].dst2.x-wid, player[pos].dst2.y-hig);
                    SDL_BlitSurface(Pl, &pl_src, window, &pl_dst);

                    SDL_FreeSurface(Pl);   

                    //本体
                    SDL_BlitSurface(gPlayer9, &player[pos].src, window, &player[pos].dst);
                    circleColor(window, player[pos].tx, player[pos].ty, 75, 0xff0000ff);
                    break;
                }
                break;
            }
        }
        if(player[pos].flag2 > 0)
            player[pos].flag2--;
    }

    //SDL_Flip(window);
}


void PlayerShotEnter(int n, int num){
    int i;

    fprintf(stderr, "n = %d, num = %d\n", n, num);

    for(i = 0; i < num; i++){ //1 <= num <= PLAYER_SHOT_MAX
        if(pla_shot[i].flag == 0){
            pla_shot[i].flag = 1;
            pla_shot[i].knd = player[n].pattern2;
            pla_shot[i].cnt = 0;
            pla_shot[i].num = n;
            return;
        }
    }
}


void PlayerAction(int pos){
      if(player[pos].command.up == 1){
          PlayerUpMove(pos);
      }
      if(player[pos].command.down == 1){
          PlayerDownMove(pos);
      }
      if(player[pos].command.left == 1){
          PlayerLeftMove(pos);
      }
      if(player[pos].command.right == 1){
          PlayerRightMove(pos);
      }
      if(player[pos].command.b5 == 1){
          PlayerBulletEnter(pos);
      }
      if(player[pos].command.rotaU == 1 || player[pos].command.rotaL == 1 || player[pos].command.rotaR == 1){ //旋回
          PlayerBatteryRota(pos);
      }
}


void PlayerUpMove(int pos){
    player[pos].ty-=player[pos].sp;
    if(player[pos].ty < player[pos].src.h/2)
        player[pos].ty = player[pos].src.h/2;

    player[pos].dst = DstRectInit(player[pos].tx - player[pos].src.w/2, player[pos].ty - player[pos].src.h/2);
}

void PlayerDownMove(int pos){
    player[pos].ty+=player[pos].sp;
    if(player[pos].ty > WINDOW_HEIGHT - player[pos].src.h/2)
        player[pos].ty = WINDOW_HEIGHT - player[pos].src.h/2;

    player[pos].dst = DstRectInit(player[pos].tx - player[pos].src.w/2, player[pos].ty - player[pos].src.h/2);
}

void PlayerLeftMove(int pos){
    player[pos].tx-=player[pos].sp;
    if(player[pos].tx < player[pos].src.w/2)
        player[pos].tx = player[pos].src.w/2;

    player[pos].dst = DstRectInit(player[pos].tx - player[pos].src.w/2, player[pos].ty - player[pos].src.h/2);

    if(player[pos].knd2 == 2){
        switch(player[pos].knd){
        case 2:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 30, player[pos].ty - player[pos].src.h/2-8);
            break;
        case 7:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 80, player[pos].ty - player[pos].src.h/2+5);
            break;
        case 8:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 100, player[pos].ty - player[pos].src.h/2+10);
            break;
        case 9:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 110, player[pos].ty - player[pos].src.h/2+5);
            break;
        }
    }
}

void PlayerRightMove(int pos){
    player[pos].tx+=player[pos].sp;
    if(player[pos].tx > WINDOW_WIDTH - player[pos].src.w/2)
        player[pos].tx = WINDOW_WIDTH - player[pos].src.w/2;

    player[pos].dst = DstRectInit(player[pos].tx - player[pos].src.w/2, player[pos].ty - player[pos].src.h/2);

    if(player[pos].knd2 == 2){
        switch(player[pos].knd){
        case 2:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 30, player[pos].ty - player[pos].src.h/2-8);
            break;
        case 7:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 80, player[pos].ty - player[pos].src.h/2+5);
            break;
        case 8:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 100, player[pos].ty - player[pos].src.h/2+10);
            break;
        case 9:
            player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 110, player[pos].ty - player[pos].src.h/2+5);
            break;
        }
    }
}

void PlayerBatteryRota(int pos){
    int num = player[pos].command.rotaR*1 + player[pos].command.rotaU*2 + player[pos].command.rotaL * 4;
    double ang = 0; //動かす目標の角度
    //double rad;

    fprintf(stderr, "%d : rota = %d\n", pos, num);

    switch(num){
    case 1:
        ang = 0;
        break;
    case 2:
        ang = -PI/2;
        break;
    case 3:
        ang = -PI/4;
        break;
    case 4:
    case 6:
        ang = -PI*3/4;
        break;
    }
    //rad = ang*180/PI;

    if(player[pos].ang < ang){
        player[pos].ang += PI/180*3;
    }
    else if(player[pos].ang > ang){
        player[pos].ang -= PI/180*3;
    }

    if(player[pos].ang < -PI*3/4)
        player[pos].ang = -PI*3/4;
    if(player[pos].ang > 0)
    player[pos].ang = 0;

    player[pos].rad = player[pos].ang*180/PI;

}


void PlayerBulletEnter(int pos){
    int i;

    for(i = 0; i < PLAYER_SHOT_MAX; i++){
        if(player[pla_shot[i].num].flag > 0 && pla_shot[i].cnt == 0 && pos == pla_shot[i].num){
            if(pla_shot[i].flag > 0 && 0 <= pla_shot[i].knd && pla_shot[i].knd < PLAYER_SHOT_PATTERN_MAX){
                PlayerShotPattern[pla_shot[i].knd](i);
            }
        }
    }
}


void PlayerShotCalc(int myid, int sock){
    int i, j;

    for(i = 0; i < PLAYER_SHOT_MAX; i++){
        if(pla_shot[i].cnt > 0)
            pla_shot[i].cnt--;

        if(pla_shot[i].flag > 0){
            for(j = 0; j < SHOT_BULLET_MAX; j++){
                if(pla_shot[i].bullet[j].flag > 0){
                    pla_shot[i].bullet[j].tx += pla_shot[i].bullet[j].spd * cos(pla_shot[i].bullet[j].ang);
                    pla_shot[i].bullet[j].ty += pla_shot[i].bullet[j].spd * sin(pla_shot[i].bullet[j].ang);
                    pla_shot[i].bullet[j].dst = DstRectInit((Sint16)pla_shot[i].bullet[j].tx - pla_shot[i].bullet[j].src.w / 2, (Sint16)pla_shot[i].bullet[j].ty - pla_shot[i].bullet[j].src.h / 2);
                    pla_shot[i].bullet[j].cnt++;

/*敵と弾の当たり判定*/
                    if(myid == pla_shot[i].num){
                        int k;
                        for(k = 0; k < ENEMY_MAX; k++){
                            if(enemy[k].flag == 1){
                                if(PTamaEnemyHitJudge(pla_shot[i].bullet[j], enemy[k])){
                                    EnemyHit(myid, i, j, k, sock);
                                    break;
                                }
                            }
                        }
                    }


/*画面外に出たら*/
                    if (pla_shot[i].bullet[j].tx < 0 || WINDOW_WIDTH < pla_shot[i].bullet[j].tx ||
                        pla_shot[i].bullet[j].ty < 0 || WINDOW_HEIGHT < pla_shot[i].bullet[j].ty)
                        pla_shot[i].bullet[j].flag = 0;
                }
            }
        }
    }
}


void PlayerBulletClean(){
    int i, j;
    for(i = 0; i < PLAYER_SHOT_MAX; i++){
        for(j = 0; j < SHOT_BULLET_MAX; j++){
            if(pla_shot[i].bullet[j].flag > 0)
                pla_shot[i].bullet[j].flag = 0;
        }
    }
}


void PlayerFree(){
    SDL_FreeSurface(gPlayer);
    SDL_FreeSurface(gPlayer2);
    SDL_FreeSurface(gPlayer2_2);
    SDL_FreeSurface(gPlayer3);
    SDL_FreeSurface(gPlayer4);
    SDL_FreeSurface(gPlayer5);
    SDL_FreeSurface(gPlayer6);
    SDL_FreeSurface(gPlayer7);
    SDL_FreeSurface(gPlayer7_2);
    SDL_FreeSurface(gPlayer8);
    SDL_FreeSurface(gPlayer8_2);
    SDL_FreeSurface(gPlayer9);
    SDL_FreeSurface(gPlayer9_2);

    SDL_FreeSurface(gPTama0);
    SDL_FreeSurface(gPTama1);
}
