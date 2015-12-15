#include "../common.h"
#include "client_func.h"

static SDL_Surface *gPlayer, *gPTama0;
static SDL_Surface *gPlayer2, *gPlayer2_2;
static SDL_Surface *gPlayer3;
static SDL_Surface *gPlayer4;
static SDL_Surface *gPlayer5;
static SDL_Surface *gPlayer6;


static void PlayerShotEnter(int n, int num);

void(*PlayerShotPattern[PLAYER_SHOT_PATTERN_MAX])(int) = {
    PlayerShotPattern0, PlayerShotPattern1
};


void PlayerLoad(){
    gPlayer = IMG_Load("sozai/player1.png");

    gPlayer2 = IMG_Load("sozai/player2.png");
    gPlayer2_2 = IMG_Load("sozai/houdai2.png");

    gPlayer3 = IMG_Load("sozai/test_design/player/fighters/fighter1(130*62).png");
    gPlayer4 = IMG_Load("sozai/test_design/player/fighters/fighter2(150*59).png");
    gPlayer5 = IMG_Load("sozai/test_design/player/fighters/fighter3(160*82).png");
    gPlayer6 = IMG_Load("sozai/test_design/player/fighters/fighter4(150*71).png");

    gPTama0 = IMG_Load("sozai/Ptama0.png");
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

                            Pl = rotozoomSurface(gPTama0, -pla_shot[i].bullet[j].rad, 1, 0);
                            wid = (Sint16)((Pl->w-35)/2+0.5);
                            hig = (Sint16)((Pl->h-35)/2+0.5);
                            pl_dst = DstRectInit(pla_shot[i].bullet[j].dst.x-wid, pla_shot[i].bullet[j].dst.y-hig);

                            SDL_BlitSurface(Pl, &pla_shot[i].bullet[j].src, window, &pl_dst);

                            SDL_FreeSurface(Pl);
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
                SDL_BlitSurface(gPlayer, &player[pos].src, window, &player[pos].dst);
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
                    break;

                case 7: //テスト用１

                    break;
                }
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
        }
        if(player[pos].flag2 > 0)
            player[pos].flag2--;
    }

    //SDL_Flip(window);
}


void PlayerDataLoad(){
    FILE *fp;//ファイルを読み込む型
    int input[64];
    char inputc[64];
    int i;

    if((fp = fopen("PlayerData.csv", "r")) == NULL){//ファイル読み込み
        
        exit(-1);
    }
    for (i = 0; i < 2; i++)//   2      
        while (getc(fp) != '\n');

    int n = 0;//行
    int num = 0;//列

    while (1) {
        for (i = 0; i < 64; i++) {
            input[i] = getc(fp);//1文字取得
            inputc[i] = input[i];
            if (inputc[i] == '/') {//スラッシュがあれば
                while (getc(fp) != '\n');
                i = -1;//カウンタを最初に戻す
                continue;
            }
            if (input[i] == ',' || input[i] == '\n') {//カンマか改行なら
                inputc[i] = '\0';

                break;
            }
            if (input[i] == EOF) {//ファイルの終わりなら
                goto EXFILE;
            }
        }

        switch (num) {
        case 0: playerOrder[n].knd = atoi(inputc); break; //プレイヤーの種類
        case 1: playerOrder[n].knd2 = atoi(inputc); break; //戦闘機か戦車か
        case 2: playerOrder[n].sp = atoi(inputc); break; //動く速度
        case 3: playerOrder[n].pattern2 = atoi(inputc); break; //攻撃パターン
        case 4: playerOrder[n].w = atoi(inputc); break; //（画像の）幅
        case 5: playerOrder[n].h = atoi(inputc); break; //（画像の）高さ
        case 6: playerOrder[n].hp_max = atoi(inputc); break; //体力
        case 7: playerOrder[n].power = atoi(inputc); break; //初期攻撃力
        case 8: playerOrder[n].w2 = atoi(inputc); break; //（砲台画像の）幅
        case 9: playerOrder[n].h2 = atoi(inputc); break; //（砲台画像の）高さ
        }
        num++;
        if (num == 10) {
            num = 0;
            n++;
        }
    }
EXFILE:
    fclose(fp);
}


void PlayerEnter(int num){
    int i, t;
    for(i = 0; i < num; i++){
        player[i].knd = 1;
    }
    for(i = num; i < MAX_CLIENTS; i++){
        player[i].knd = 0;
    }
    /*player[0].knd = 1;
    player[1].knd = 1;
    player[2].knd = 1;
    player[3].knd = 1;*/

    for(i = 0; i < num; i++){
        if(player[i].flag == 0){
            for(t = 0; t < PLAYER_ORDER_MAX; t++){
                if(player[i].knd == playerOrder[t].knd){
                    player[i].flag = 1;
                    player[i].knd2 = playerOrder[t].knd2;
                    player[i].sp = playerOrder[t].sp+5;
                    player[i].power = playerOrder[t].power;
                    player[i].pattern2 = playerOrder[t].pattern2;

                    HP_Max += playerOrder[t].hp_max;

                    player[i].flag2 = 0;
                    player[i].num = i;

                    switch (player[i].knd2) {
                    case 1:
                        player[i].tx = 100;
                        player[i].ty = 100*(player[i].num+1);
                        player[i].ang = 0;
                        break;
                    case 2:
                        player[i].src2 = SrcRectInit(0, 0, playerOrder[t].w2, playerOrder[t].h2);
                        player[i].tx = 50*(player[i].num+1);
                        player[i].ty = 850+10*(player[i].num+1);
                        player[i].ang = -PI / 6;
                        player[i].rad = player[i].ang*180/PI;
                        break;
                    }
                    player[i].src = SrcRectInit(0, 0, playerOrder[t].w, playerOrder[t].h);
                    player[i].dst = DstRectInit(player[i].tx - player[i].src.w / 2, player[i].ty - player[i].src.h / 2);
                    player[i].dst2 = DstRectInit(player[i].tx - player[i].src.w / 2 + 30, player[i].ty - player[i].src.h / 2 - 8);

                    PlayerShotEnter(i, num);

                    fprintf(stderr, "knd = %d, w = %d, h = %d\n", player[i].knd, player[i].src.w, player[i].src.h);

                    break;
                }
            }
        }
    }
    HP_Num = HP_Max;
}



static void PlayerShotEnter(int n, int num){
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

    if(player[pos].knd2 == 2)
        player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 30, player[pos].ty - player[pos].src.h/2-8);
}

void PlayerRightMove(int pos){
    player[pos].tx+=player[pos].sp;
    if(player[pos].tx > WINDOW_WIDTH - player[pos].src.w/2)
        player[pos].tx = WINDOW_WIDTH - player[pos].src.w/2;

    player[pos].dst = DstRectInit(player[pos].tx - player[pos].src.w/2, player[pos].ty - player[pos].src.h/2);

    if(player[pos].knd2 == 2)
        player[pos].dst2 = DstRectInit(player[pos].tx - player[pos].src.w/2 + 30, player[pos].ty - player[pos].src.h/2-8);
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
    SDL_FreeSurface(gPTama0);
}
