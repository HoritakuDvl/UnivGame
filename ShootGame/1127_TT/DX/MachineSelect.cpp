#include "common.h"

static int gSubTitle;
static int gMachine[2];
static int gMark[1];

static int sele_num = 0;
static int num = 0;

void KyaraSelectLoad() {
	gSubTitle = LoadGraph("sozai\\MachineSelect\\subTitle.png");
	gMachine[0] = LoadGraph("sozai\\MachineSelect\\Fighter.png");
	gMachine[1] = LoadGraph("sozai\\MachineSelect\\Tank.png");
	gMark[0] = LoadGraph("sozai\\MachineSelect\\Mark.png");


}

void KyaraSelectDraw() {
	DrawGraph(10, 1, gSubTitle, TRUE);

	for (int i = 0; i < 2; i++) {
		DrawGraph(40+400*i, 200, gMachine[i], TRUE);
	}

	DrawGraph(35+400*sele_num, 195, gMark[0], TRUE);
}

int KyaraSelectMove() {//‘I‘ðŽˆ‚ð•Ï‚¦‚é
	if (num == 0) {
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			if (sele_num == 1) {
				sele_num--;
				//num = 10;
			}
		}
		if (CheckHitKey(KEY_INPUT_RIGHT)) {
			if (sele_num == 0) {
				sele_num++;
				//num = 10;
			}
		}

		/*if (CheckHitKey(KEY_INPUT_Z)) {
			return sele_num+1;
		}*/
	}
	else if (num > 0)
		num--;

	return 0;
}

void FighterSelectDraw() {
	DrawGraph(10, 1, gSubTitle, TRUE);

}

void FighterSelectMove() {

}

void TankSelectDraw() {
	DrawGraph(10, 1, gSubTitle, TRUE);

}

void TankSelectMove() {

}



