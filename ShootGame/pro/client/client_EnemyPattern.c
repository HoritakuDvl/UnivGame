#include "../common.h"
#include "client_func.h"

void EnemyPattern0(int n){
    if(enemy[n].cnt <= 60)
        enemy[n].tx -= enemy[n].sp;

    else if(enemy[n].tx+enemy[n].src.w/2 > WINDOW_WIDTH)
        enemy[n].tx -= enemy[n].sp;

}


void EnemyPattern1(int n){
    if (enemy[n].cnt <= 60)
        enemy[n].tx -= enemy[n].sp;

    else if(enemy[n].tx+enemy[n].src.w/2 > WINDOW_WIDTH)
        enemy[n].tx -= enemy[n].sp;

    else {
        int t = enemy[n].cnt % 180;
        int sp = enemy[n].sp-2;
        if(stage == 3)
            sp*=3;

        if (0 <= t && t < 90)
            enemy[n].ty -= sp;
        else if (90 <= t && t < 180)
            enemy[n].ty += sp;
    }
}
