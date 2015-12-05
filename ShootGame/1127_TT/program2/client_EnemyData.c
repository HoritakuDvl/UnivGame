#include "common.h"
#include "client_func.h"

static SDL_Surface *gEnemy1, *gEnemy2;
static SDL_Surface *gTama0;

static void EnemyShotEnter(int n);
static void EnemyShotCalc(int n);

void (*EnemyPattern[ENEMY_PATTERN_MAX])(int) = {
    EnemyPattern0, EnemyPattern1
};

void(*EnemyShotPattern[ENEMY_SHOT_PATTERN_MAX])(int, int) = {
	EnemyShotPattern0, EnemyShotPattern1
};

void EnemyLoad(){
    gEnemy1 = IMG_Load("sozai/enemy1.png");
    gEnemy2 = IMG_Load("sozai/enemy2.png");

    gTama0 = IMG_Load("sozai/Etama0.png");
}


void EnemyInit(){
    int i, j;

    for(i = 0; i < ENEMY_SHOT_MAX; i++){
        ene_shot[i].flag = 0;
        for(j = 0; j < SHOT_BULLET_MAX; j++){
            ene_shot[i].bullet[j].flag = 0;
        }
    }

    for(i = 0; i < ENEMY_MAX; i++){
        enemy[i].flag = 0;
    }
}


void EnemyDraw(){
    int i, j;
//弾
    for (i = 0; i < ENEMY_SHOT_MAX; i++) {
        for (j = 0; j < SHOT_BULLET_MAX; j++) {
            if (ene_shot[i].bullet[j].flag > 0) {
                switch (ene_shot[i].bullet[j].knd) { //後に変更する
                case 0:
                    SDL_BlitSurface(gTama0, &ene_shot[i].bullet[j].src, window, &ene_shot[i].bullet[j].dst);
                    break;
                }
            }
        }
    }

//敵
    for(i = 0; i < ENEMY_MAX; i++){
        if(enemy[i].flag == 1){
            switch(enemy[i].knd){
            case 0:
                SDL_BlitSurface(gEnemy1, &enemy[i].src, window, &enemy[i].dst);
                break;
            case 1:
                SDL_BlitSurface(gEnemy2, &enemy[i].src, window, &enemy[i].dst);
                break;
            }
        }
    }
    SDL_Flip(window);
}


void EnemyDataLoad(){
    FILE *fp;//ファイルを読み込む型
    int input[64];
    char inputc[64];
    int i, j;

    if((fp = fopen("EnemyData.csv", "r")) == NULL){//ファイル読み込み
        SendEndCommand();
        return;
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
        case 0: enemyOrder[n].mission = atoi(inputc); break; //ミッション  //atoi:文字列を数式に変える
        case 1: enemyOrder[n].stage = atoi(inputc); break; //ステージ
        case 2: enemyOrder[n].wait = atoi(inputc); break; //待機時間
        case 3: enemyOrder[n].knd = atoi(inputc); break; //敵の種類
        case 4: enemyOrder[n].pattern = atoi(inputc); break; //敵の動くパターン
        case 5: enemyOrder[n].sp = atoi(inputc); break; //敵の動く速度
        case 6: enemyOrder[n].pattern2 = atoi(inputc); break; //敵の攻撃するパターン
        case 7: enemyOrder[n].blknd = atoi(inputc); break; //敵が打つ弾の種類
        case 8: enemyOrder[n].blW = atoi(inputc); break; //弾画像の幅
        case 9: enemyOrder[n].blH = atoi(inputc); break; //弾画像の高さ
        case 10: enemyOrder[n].blCnt = atoi(inputc); break; //弾打ちの間隔
        case 11: enemyOrder[n].bltime = atoi(inputc); break; //敵が出現してから打つまでの時間
        case 12: enemyOrder[n].tx = atoi(inputc); break; //x座標
        case 13: enemyOrder[n].ty = atoi(inputc); break; //y座標
        case 14: enemyOrder[n].w = atoi(inputc); break; //幅
        case 15: enemyOrder[n].h = atoi(inputc); break; //高さ
        case 16: enemyOrder[n].hp_max = atoi(inputc); break; //最大HP
        case 17: enemyOrder[n].item = atoi(inputc); break; //落とすアイテムの種類
        case 18: enemyOrder[n].item2 = atoi(inputc); break; //item=1のとき、変更するショットの番号
        }
        num++;
        if (num == 19) {
            num = 0;
            enemyOrder[n].flag = 1;
            n++;
        }
    }
EXFILE:
    fclose(fp);
}


void EnemyEnter(){
    int i, t;
    for(t = 0; t < ENEMY_ORDER_MAX; t++){
        if (enemyOrder[t].flag == 1 && enemyOrder[t].mission == 1 && enemyOrder[t].stage == 1) { //ミッション１、ステージ１のとき
            for(i = 0; i < ENEMY_MAX; i++){
                if(enemy[i].flag == 0){
                    enemy[i].flag = 1;
                    enemy[i].wait = enemyOrder[t].wait;
                    enemy[i].knd = enemyOrder[t].knd;
                    enemy[i].pattern = enemyOrder[t].pattern;
                    enemy[i].sp = enemyOrder[t].sp;
                    enemy[i].cnt = 0;
                    enemy[i].pattern2 = enemyOrder[t].pattern2;
                    enemy[i].blknd = enemyOrder[t].blknd;
                    enemy[i].blW = enemyOrder[t].blW;
                    enemy[i].blH = enemyOrder[t].blH;
                    enemy[i].blCnt = enemyOrder[t].blCnt;
                    enemy[i].bltime = enemyOrder[t].bltime; //弾打ちまでの時間
                    enemy[i].tx = enemyOrder[t].tx;
                    enemy[i].ty = enemyOrder[t].ty;
                    enemy[i].hp_max = enemyOrder[t].hp_max;
                    enemy[i].item = enemyOrder[t].item;
                    enemy[i].item2 = enemyOrder[t].item2;

                    enemy[i].src = SrcRectInit(0, 0, enemyOrder[t].w, enemyOrder[t].h);
                    enemy[i].dst = DstRectInit(enemy[i].tx - enemy[i].src.w / 2, enemy[i].ty - enemy[i].src.h / 2);
                    enemy[i].hp = enemy[i].hp_max;

                    enemyOrder[t].flag = 0;
                    break;
                }
            }
        }
    }
}


void EnemyMove(){
    int i;
    for(i = 0; i < ENEMY_MAX; i++) {
        if(enemy[i].flag > 0) {
            if(enemy[i].wait == 0){
                EnemyPattern[enemy[i].pattern](i);
                enemy[i].dst = DstRectInit(enemy[i].tx - enemy[i].src.w / 2, enemy[i].ty - enemy[i].src.h / 2);

                if(enemy[i].bltime == enemy[i].cnt){
                    EnemyShotEnter(i);
                }
                enemy[i].cnt++;

//プレイヤーと敵の当たり判定
                int k;

            }
            else if(enemy[i].wait > 0)
                enemy[i].wait--;
        }
    }
}


void EnemyBulletMove(int num){
    int i;
    for (i = 0; i < ENEMY_SHOT_MAX; i++) {
        if (ene_shot[i].flag != 0 && 0 <= ene_shot[i].knd && ene_shot[i].knd < ENEMY_SHOT_PATTERN_MAX) {
            EnemyShotPattern[ene_shot[i].knd](i, num); //弾の生成
            EnemyShotCalc(i); //弾を動かす
            ene_shot[i].cnt++;
        }
    }
}


void EnemyFree(){
    SDL_FreeSurface(gEnemy1);
    SDL_FreeSurface(gEnemy2);

    SDL_FreeSurface(gTama0);
}


/**************************
static
 *************************/
static void EnemyShotEnter(int n){
    int i;
    for (i = 0; i < ENEMY_SHOT_MAX; i++) {
        if (ene_shot[i].flag == 0) {
            ene_shot[i].flag = 1;
            ene_shot[i].knd = enemy[n].pattern2;
            ene_shot[i].num = n;
            ene_shot[i].cnt = 0;
            return;
        }
    }
}


static void EnemyShotCalc(int n){
    int max = 0;
    int j;
    if (enemy[ene_shot[n].num].flag != 1) //敵が倒されたら
        ene_shot[n].flag = 2; //それ以上弾を生成しないフラグに変える
    for (j = 0; j < SHOT_BULLET_MAX; j++) {
        if (ene_shot[n].bullet[j].flag > 0) { //生成していたら
            ene_shot[n].bullet[j].tx += cos(ene_shot[n].bullet[j].ang)*ene_shot[n].bullet[j].spd;
            ene_shot[n].bullet[j].ty += sin(ene_shot[n].bullet[j].ang)*ene_shot[n].bullet[j].spd;
            ene_shot[n].bullet[j].dst = DstRectInit(ene_shot[n].bullet[j].tx - ene_shot[n].bullet[j].src.w/2, ene_shot[n].bullet[j].ty - ene_shot[n].bullet[j].src.h/2);
            ene_shot[n].bullet[j].cnt++;

            //プレイヤーと弾のあたり判定
            int k;
            /*for (k = 0; k < PLAYER_MAX; k++) {
                if (player[k].flag > 0) {
                    if (player[k].flag2 == 0 && ETamaPlayerHitJjudge(ene_shot[n].bullet[j], player[k])) {
                        //player[k].hp--; //不要
                        HP_num--;
                        player[k].flag2 = 180;

                        ene_shot[n].bullet[j].flag = 0;
                        break;
                    }
                }
                }*/

            //画面外に出たら
            if (ene_shot[n].bullet[j].tx < 0 || WINDOW_WIDTH < ene_shot[n].bullet[j].tx ||
                ene_shot[n].bullet[j].ty < 0 || WINDOW_HEIGHT < ene_shot[n].bullet[j].ty) {
                ene_shot[n].bullet[j].flag = 0; //弾を消す
            }
        }
    }

    //現在表示中の弾が一つでもあるかどうか調べる
    for (j = 0; j < SHOT_BULLET_MAX; j++) {
        if (ene_shot[n].bullet[j].flag > 0)
            return;
    }

    //敵が倒されている、もしくはまだ出現していないとき
    if (enemy[ene_shot[n].num].flag != 1) {
        ene_shot[n].flag = 0;
        enemy[ene_shot[n].num].flag = 0;
    }
}

