#include "common.h"

static int gSubTitle;
static int gButton[5];
static int gSelect[2];

static int sele_num = 2; //mission1を選択
static int num = 0; //すぐに選択できないようにする

void MissionSelectLoad() {
	gSubTitle = LoadGraph("sozai\\MissionSelect\\subTitle.png");
	gButton[0] = LoadGraph("sozai\\MissionSelect\\ShopButton.png");
	gButton[1] = LoadGraph("sozai\\MissionSelect\\missionButton1.png");
	gButton[2] = LoadGraph("sozai\\MissionSelect\\missionButton2.png");
	gButton[3] = LoadGraph("sozai\\MissionSelect\\missionButton3.png");
	gButton[4] = LoadGraph("sozai\\MissionSelect\\missionButton4.png");

	gSelect[0] = LoadGraph("sozai\\MissionSelect\\Mark1.png");
	gSelect[1] = LoadGraph("sozai\\MissionSelect\\Mark2.png");

}

void MissionSelectDraw() {
	DrawGraph(-22, 1, gSubTitle, TRUE);


	if (sele_num == 3)
		DrawGraph(800 - 200, 5, gButton[0], TRUE);
	for (int i = 1; i < 5; i++) {
		DrawGraph(-10 + 400 * ((i-1) % 2), 140 + 210 * ((i-1) / 2), gButton[i], TRUE);
	}
	if(sele_num != 3)
		DrawGraph(800 - 200, 5, gButton[0], TRUE);

	if(sele_num <= 1)
		DrawGraph(800 - 205, 0, gSelect[0], TRUE);
	else
		DrawGraph(5 + 400 * ((sele_num - 2) % 2), 165 + 210 * ((sele_num - 2) / 2), gSelect[1], TRUE);

}

int MissionSelectMove() {//選択肢を変える
	if (num == 0) {
		if (CheckHitKey(KEY_INPUT_UP)) {
			if (sele_num >= 2) { //Shop以外
				sele_num -= 2;
				num = 30;
			}
		}
		if (CheckHitKey(KEY_INPUT_DOWN)) {
			if (sele_num <= 3) { //mission3,4以外
				sele_num += 2;
				num = 30;
			}
		}
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			if (sele_num == 3 || sele_num == 5) { //mission2,4のみ
				sele_num--;
				num = 30;
			}
		}
		if (CheckHitKey(KEY_INPUT_RIGHT)) {
			if (sele_num == 2 || sele_num == 4) { //mission1,3のみ
				sele_num++;
				num = 30;
			}
		}

		if (CheckHitKey(KEY_INPUT_Z)) {
			switch (sele_num) {
			case 0:
			case 1:
				return 2;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				return sele_num+1;
				break;
			}
		}
	}
	else if (num > 0)
		num--;

	return 1;
}


