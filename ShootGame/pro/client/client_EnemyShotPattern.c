#include "../common.h"
#include "client_func.h"

static int shot_search(int n);
static double shotatan2(int n);
static double shotatan2_2(int n, int num);


void EnemyShotPattern0(int n, int num) {
    int k;
    if (ene_shot[n].cnt % enemy[ene_shot[n].num].blCnt == 1) { //弾幕が始まってから0カウント目
        //敵が倒されていなくて、探した登録可能な弾番号が有効なら
        if (ene_shot[n].flag != 2 && (k = shot_search(n)) != -1) {
            ene_shot[n].bullet[k].knd = enemy[ene_shot[n].num].blknd;
            ene_shot[n].bullet[k].ang = -PI;//shotatan2_2(n, num);
            ene_shot[n].bullet[k].rad = ene_shot[n].bullet[k].ang*180/PI;
            ene_shot[n].bullet[k].flag = 1;
            ene_shot[n].bullet[k].tx = enemy[ene_shot[n].num].tx;
            ene_shot[n].bullet[k].ty = enemy[ene_shot[n].num].ty;
            if(enemy[ene_shot[n].num].blW > enemy[ene_shot[n].num].blH){
                ene_shot[n].bullet[k].src.w = enemy[ene_shot[n].num].blW + 20;
                ene_shot[n].bullet[k].src.h = enemy[ene_shot[n].num].blW + 20;
            }
            else{
                ene_shot[n].bullet[k].src.w = enemy[ene_shot[n].num].blH + 20;
                ene_shot[n].bullet[k].src.h = enemy[ene_shot[n].num].blH + 20;
            }
            ene_shot[n].bullet[k].cnt = 0;
            ene_shot[n].bullet[k].spd = 5;
        }
    }
}

void EnemyShotPattern1(int n, int num) {
    int k;
    if (ene_shot[n].cnt % enemy[ene_shot[n].num].blCnt == 1) { //弾幕が始まってから0カウント目
        //敵が倒されていなくて、探した登録可能な弾番号が有効なら
        if (ene_shot[n].flag != 2 && (k = shot_search(n)) != -1) {
            ene_shot[n].bullet[k].knd = enemy[ene_shot[n].num].blknd;
            ene_shot[n].bullet[k].ang = -PI;
            ene_shot[n].bullet[k].flag = 1;
            ene_shot[n].bullet[k].tx = enemy[ene_shot[n].num].tx;
            ene_shot[n].bullet[k].ty = enemy[ene_shot[n].num].ty;
            ene_shot[n].bullet[k].src.w = enemy[ene_shot[n].num].blW;
            ene_shot[n].bullet[k].src.h = enemy[ene_shot[n].num].blH;
            ene_shot[n].bullet[k].cnt = 0;
            ene_shot[n].bullet[k].spd = 3;
        }
    }
}


static int shot_search(int n) {
    int j;
    for (j = 0; j < SHOT_BULLET_MAX; j++) {
        if (ene_shot[n].bullet[j].flag == 0) {
            return j;
        }
    }
    return -1;
}


//自分に発射
static double shotatan2(int n) {
    return atan2(player[0].ty - enemy[ene_shot[n].num].ty, player[0].tx - enemy[ene_shot[n].num].tx);
}

//敵に一番近いプレイヤーに発射
static double shotatan2_2(int n, int num) {
    int x1 = player[0].tx - enemy[ene_shot[n].num].tx;
    int y1 = player[0].ty - enemy[ene_shot[n].num].ty;
    int r1 = sqrt(x1*x1 + y1*y1);

    int i;
    for (i = 1; i < num; i++) {
        int x2 = player[i].tx - enemy[ene_shot[n].num].tx;
        int y2 = player[i].ty - enemy[ene_shot[n].num].ty;
        int r2 = sqrt(x2*x2 + y2*y2);

        if (r2 < r1) {
            x1 = x2;
            y1 = y2;
            r1 = r2;
        }
    }

    return atan2(y1, x1);
}
