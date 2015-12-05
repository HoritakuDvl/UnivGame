#include "common.h"

/*
���͉~�ő�p
item[i].knd > 0 (�K���I)

*/

void ItemLoad() {

}


void ItemInit() {
	for (int i = 0; i < ITEM_MAX; i++) {
		item[i] = { 0 };
	}
}


void ItemDraw() {
	for (int i = 0; i < ITEM_MAX; i++) {
		if (item[i].flag > 0) {
			switch (item[i].knd) {
			case 1:
				DrawCircle(item[i].tx, item[i].ty, item[i].w/2, GetColor(255, 255, 0));
				break;
			case 2:
				DrawCircle(item[i].tx, item[i].ty, item[i].w / 2, GetColor(255, 0, 255));
				break;
			case 3:
				DrawCircle(item[i].tx, item[i].ty, item[i].w / 2, GetColor(0, 255, 255));
				break;
			case 4:
				DrawCircle(item[i].tx, item[i].ty, item[i].w / 2, GetColor(255, 255, 255));
				break;
			}
		}
	}
}


void ItemMove() {
	for (int i = 0; i < ITEM_MAX; i++) {
		if (item[i].flag > 0) {
			item[i].tx -= item[i].spd;

			//�v���C���[�ƃA�C�e���̂����蔻��
			for (int k = 0; k < PLAYER_SHOT_MAX; k++) {
				if(player[pla_shot[k].num].flag > 0)
				if (PlayerItemHitJudge(player[pla_shot[k].num], item[i])) {
					switch (item[i].knd) { //�A�C�e���̎�ނɂ���āA�@�킪�ω�
					case 1: //�V���b�g�ύX
						pla_shot[k].knd = item[i].knd2;
						break;
					case 2: //���x�A�b�v
						player[pla_shot[k].num].sp += 2;
						break;
					case 3: //�p���[�A�b�v
						player[pla_shot[k].num].power++;
						break;
					case 4: //�̗͉�
						HP_num += 10;
						if (HP_num > HP_max)
							HP_num = HP_max;
						break;
					}
					item[i].flag = 0;
					break;
				}
			}

			if (item[i].tx < 0)
				item[i].flag = 0;
		}
	}
}

/*

�A�C�e���̎��
1: �e�v���C���[�̃V���b�g�̕ύX
2: �e�v���C���[�̈ړ����x�̕ύX
3: �e�v���C���[�̍U���̓A�b�v
4: �S�̗͉̂̑�
*/
void ItemEnter(int n) {
	for (int i = 0; i < ITEM_MAX; i++) {
		if (item[i].flag == 0 && 0 < enemy[n].item && enemy[i].item <= ITEM_KND_MAX) {
			item[i].flag = 1;
			item[i].knd = enemy[n].item;
			item[i].knd2 = enemy[n].item2;
			item[i].spd = 2;

			item[i].tx = enemy[n].tx;
			item[i].ty = enemy[n].ty;

			item[i].w = 20;
			item[i].h = 20;

			item[i].x = item[i].tx - item[i].w / 2;
			item[i].y = item[i].ty - item[i].h / 2;

			enemy[n].item = -1;
			return;
		}
	}
}

