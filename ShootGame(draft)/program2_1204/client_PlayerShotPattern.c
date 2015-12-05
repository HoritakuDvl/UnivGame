#include "common.h"
#include "client_func.h"

static int shot_search(int n){
    int j;
    for(j = 0; j < SHOT_BULLET_MAX; j++){
        if(pla_shot[n].bullet[j].flag == 0){
            return j;
        }
    }
    return -1;
}


void PlayerShotPattern0(int n){
    int k;
    if ((k = shot_search(n)) != -1) {
        pla_shot[n].bullet[k].flag = 1;
        pla_shot[n].bullet[k].ang = player[pla_shot[n].num].ang; //弾の角度
        pla_shot[n].bullet[k].rad = pla_shot[n].bullet[k].ang*180/PI;
        pla_shot[n].bullet[k].spd = 5; //速度

        switch (player[pla_shot[n].num].knd) { //機種ごとの初期位置
        case 1:
            pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + player[pla_shot[n].num].src.w / 2;
            pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty;
            break;
        case 2:
            pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + 5 + 50 * cos(pla_shot[n].bullet[k].ang);
            pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty - 15 + 50 * sin(pla_shot[n].bullet[k].ang);
            break;
        }
        //fprintf(stderr, "(tx, ty) = (%4d, %4d)\n", (int)pla_shot[n].bullet[k].tx, (int)pla_shot[n].bullet[k].ty);

        pla_shot[n].bullet[k].knd = 0;
        pla_shot[n].bullet[k].src = SrcRectInit(0, 0, 30, 30);
        pla_shot[n].bullet[k].dst = DstRectInit((Sint16)pla_shot[n].bullet[k].tx - pla_shot[n].bullet[k].src.w / 2, (Sint16)pla_shot[n].bullet[k].ty - pla_shot[n].bullet[k].src.h / 2);

        pla_shot[n].cnt = 30; //弾打ちの間隔

    }
}

void PlayerShotPattern1(int n){
    int k;
    if ((k = shot_search(n)) != -1) {
        pla_shot[n].bullet[k].flag = 1;
        pla_shot[n].bullet[k].ang = player[pla_shot[n].num].ang; //弾の角度
        pla_shot[n].bullet[k].rad = pla_shot[n].bullet[k].ang*180/PI;
        pla_shot[n].bullet[k].spd = 10; //速度

        switch (player[pla_shot[n].num].knd) { //機種ごとの初期位置
        case 1:
            pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + player[pla_shot[n].num].src.w / 2;
            pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty;
            break;
        case 2:
            pla_shot[n].bullet[k].tx = player[pla_shot[n].num].tx + 5 + 50 * cos(pla_shot[n].bullet[k].ang);
            pla_shot[n].bullet[k].ty = player[pla_shot[n].num].ty - 15 + 50 * sin(pla_shot[n].bullet[k].ang);
            break;
        }

        pla_shot[n].bullet[k].knd = 0;
        pla_shot[n].bullet[k].src = SrcRectInit(0, 0, 30, 30);
        pla_shot[n].bullet[k].dst = DstRectInit(pla_shot[n].bullet[k].tx - pla_shot[n].bullet[k].src.w / 2, pla_shot[n].bullet[k].ty - pla_shot[n].bullet[k].src.h / 2);

        pla_shot[n].cnt = 60; //弾打ちの間隔

    }
}
