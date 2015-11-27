#include "common.h"
#include <math.h>

static int rad = 0;

void EnemyPattern0(int n) {
	if(enemy[n].cnt <= 60)
		enemy[n].tx -= 5;

	/*if (enemy[n].cnt >= 300) {
		enemy[n].y += 5 * cos(rad*PI/180);
		rad++;
	}*/

}

void EnemyPattern1(int n) {
	if (enemy[n].cnt <= 60)
		enemy[n].tx -= 5;

	else {
		int t = enemy[n].cnt % 180;

		if (0 <= t && t < 90)
			enemy[n].ty -= 3;
		else if (90 <= t && t < 180)
			enemy[n].ty += 3;
	}
}