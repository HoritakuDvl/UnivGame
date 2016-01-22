#include "../common.h"
#include "client_func.h"

static int shot_search(int n);

void EnemyShotPattern0(int n, int num) {
    int k;
    if (ene_shot[n].cnt % (enemy[ene_shot[n].num].blCnt/2) == 1) { //弾幕が始まってから0カウント目
        //敵が倒されていなくて、探した登録可能な弾番号が有効なら
        if (ene_shot[n].flag != 2 && (k = shot_search(n)) != -1) {
            ene_shot[n].bullet[k].knd = enemy[ene_shot[n].num].blknd;
            ene_shot[n].bullet[k].ang = -PI;//shotatan2_2(n, num);
            ene_shot[n].bullet[k].rad = ene_shot[n].bullet[k].ang*180/PI;
            ene_shot[n].bullet[k].flag = 1;
            ene_shot[n].bullet[k].tx = enemy[ene_shot[n].num].dst.x+50;
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
            ene_shot[n].bullet[k].spd = 10;
        }
    }
}

void EnemyShotPattern1(int n, int num) {
    int k;
    if (ene_shot[n].cnt % (enemy[ene_shot[n].num].blCnt/2) == 1) { //弾幕が始まってから0カウント目
        //敵が倒されていなくて、探した登録可能な弾番号が有効なら
        if (ene_shot[n].flag != 2 && (k = shot_search(n)) != -1) {
            ene_shot[n].bullet[k].knd = enemy[ene_shot[n].num].blknd;
            ene_shot[n].bullet[k].ang = -PI;
            ene_shot[n].bullet[k].flag = 1;
            ene_shot[n].bullet[k].tx = enemy[ene_shot[n].num].dst.x+50;
            ene_shot[n].bullet[k].ty = enemy[ene_shot[n].num].ty;
            ene_shot[n].bullet[k].src.w = enemy[ene_shot[n].num].blW;
            ene_shot[n].bullet[k].src.h = enemy[ene_shot[n].num].blH;
            ene_shot[n].bullet[k].cnt = 0;
            ene_shot[n].bullet[k].spd = 6;
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
