#include "common.h"
#include <math.h>

static int gPlayer, gPlayer2;
static int gHoudai2;
static int gTama2;

//static PlayerData PlayerDataInit(int x, int y, int w, int h, int f, int hp_max, int knd, double a);
static void PlayerShotEnter(int n);
static void PlayerShotCalc(int n);

void(*PlayerShotPattern[PLAYER_SHOT_PATTERN_MAX])(int) = {
	PlayerShotPattern0, PlayerShotPattern1
};

void PlayerLoad() {
	gPlayer = LoadGraph("sozai\\player1.png");
	gPlayer2 = LoadGraph("sozai\\player2.png");

	gHoudai2 = LoadGraph("sozai\\houdai2.png");

	gTama2 = LoadGraph("sozai\\Ptama0.png");
}


void PlayerInit() {
	//�v���C���[
	for (int i = 0; i < PLAYER_MAX; i++) {
		player[i] = {0};
	}
	HP_max = 0;
	HP_num = HP_max;

	//�e
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < SHOT_BULLET_MAX; j++) {
			pla_shot[i].bullet[j] = { 0 };
			pla_shot[i].bullet[j].ghandle = gTama2;
		}
	}
}


/*static PlayerData PlayerDataInit(int x, int y, int w, int h, int f, int hp_max, int knd, double a) {
	PlayerData tmp;

	tmp.x = x;
	tmp.y = y;
	tmp.w = w;
	tmp.h = h;
	tmp.flag = f;
	tmp.flag2 = 0;
	tmp.hp_max = hp_max;
	tmp.hp = tmp.hp_max;
	tmp.knd = knd;
	tmp.knd2 = knd; //��ŕς���
	tmp.ang = a;
	tmp.sp = 5;

	tmp.tx = tmp.x + tmp.w / 2;
	tmp.ty = tmp.y + tmp.h / 2;

	return tmp;
}*/


void PlayerDraw() {
	for (int i = 0; i < PLAYER_SHOT_MAX; i++){
		if (pla_shot[i].flag > 0) {
			for (int j = 0; j < SHOT_BULLET_MAX; j++) {
				if (pla_shot[i].bullet[j].flag > 0) {
					switch (pla_shot[i].bullet[j].knd) {
					case 0:
						DrawRotaGraph(pla_shot[i].bullet[j].tx, pla_shot[i].bullet[j].ty, 1, pla_shot[i].bullet[j].ang, gTama2, TRUE, FALSE);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < PLAYER_MAX; i++) {
		if (player[i].flag > 0) {
			int cnt = player[i].flag2 % 14;
			if (cnt <= 4 || 11 <= cnt) {
				switch (player[i].knd) {
				case 1:
					DrawGraph(player[i].x, player[i].y, gPlayer, TRUE);
					break;
				case 2:
					DrawRotaGraph(player[i].tx + 5, player[i].ty - 15, 1, player[i].ang, gHoudai2, TRUE, FALSE);
					DrawGraph(player[i].x, player[i].y, gPlayer2, TRUE);
					break;
				}
			}
			if (player[i].flag2 > 0)
				player[i].flag2--;
		}
	}
}


void PlayerDataLoad() {
	int fp;//�t�@�C����ǂݍ��ތ^
	int input[64];
	char inputc[64];

	fp = FileRead_open("PlayerData.csv");//�t�@�C���ǂݍ���
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}
	for (int i = 0; i < 2; i++)//�ŏ���2�s�ǂݔ�΂�
		while (FileRead_getc(fp) != '\n');

	int n = 0;//�s
	int num = 0;//��

	while (1) {
		for (int i = 0; i < 64; i++) {
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����
													 //printfDx("%c",inputc[i]);
			if (inputc[i] == '/') {//�X���b�V���������
				while (FileRead_getc(fp) != '\n');//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂���
				continue;//for�܂Ŗ߂�
			}
			if (input[i] == ',' || input[i] == '\n') {//�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ�

				break;
			}
			if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��(EXFILE�܂Ŕ��)
			}
		}
		//printfDx("%d,",atoi(inputc));
		switch (num) {
		case 0: playerOrder[n].knd = atoi(inputc); break; //�v���C���[�̎��  //atoi:������𐔎��ɕς���
		case 1: playerOrder[n].knd2 = atoi(inputc); break; //�퓬�@����Ԃ�
		case 2: playerOrder[n].sp = atoi(inputc); break; //�������x
		case 3: playerOrder[n].pattern2 = atoi(inputc); break; //�U���p�^�[��
		case 4: playerOrder[n].w = atoi(inputc); break; //(�摜��)��
		case 5: playerOrder[n].h = atoi(inputc); break; //(�摜��)����
		case 6: playerOrder[n].hp_max = atoi(inputc); break; //�̗�
		case 7: playerOrder[n].power = atoi(inputc); break; //(�摜��)��
		case 8: playerOrder[n].w2 = atoi(inputc); break; //(�摜��)��
		case 9: playerOrder[n].h2 = atoi(inputc); break; //(�摜��)����
		}
		num++;
		if (num == 10) {
			num = 0;
			enemyOrder[n].flag = 1;
			n++;
		}
	}
EXFILE:
	FileRead_close(fp);
}


/*�@����N���C�A���g�S�����ݒ肵���Ƃ��̂ݓǂݍ��� (0�͂Ȃ�)*/
void PlayerEnter() {
	player[0].knd = 1; //��ނ�ς���Ƃ��ɁA���̒l��ς���
	player[1].knd = 2;
	player[2].knd = 1;
	player[3].knd = 0;

	for (int i = 0; i < PLAYER_MAX; i++) {
		if (player[i].flag == 0) {
			for (int t = 0; t < PLAYER_ORDER_MAX; t++) {
				if (player[i].knd == playerOrder[t].knd) {
					player[i].flag = 1;
					player[i].knd2 = playerOrder[t].knd2;
					player[i].sp = playerOrder[t].sp;
					player[i].power = playerOrder[t].power;
					player[i].pattern2 = playerOrder[t].pattern2;
					player[i].w = playerOrder[t].w;
					player[i].h = playerOrder[t].h;
					//player[i].hp_max = playerOrder[t].hp_max; //�s�v
					HP_max += playerOrder[t].hp_max;

					player[i].flag2 = 0;
					//player[i].hp = player[i].hp_max; //�s�v
					player[i].num = i;

					switch (player[i].knd2) {
					case 1:
						player[i].tx = 100;
						player[i].ty = 100*(player[i].num+1);
						player[i].ang = 0;
						break;
					case 2:
						//player[i].w2 = playerOrder[t].w2;
						//player[i].h2 = playerOrder[t].h2;
						player[i].tx = 50*(player[i].num+1);
						player[i].ty = 520+10*(player[i].num+1);
						player[i].ang = -PI / 6;
						break;
					}
					player[i].x = player[i].tx - player[i].w / 2;
					player[i].y = player[i].ty - player[i].h / 2;

					PlayerShotEnter(0);//���"i"�ɕς���

					break;
				}
			}
		}
	}
	HP_num = HP_max;
}


void PlayerMove() {
	/*�v���[���[�̈ړ�*/
	if (player[0].knd == 1) {
		if (CheckHitKey(KEY_INPUT_UP)) {
			player[0].ty -= player[0].sp;
		}
		if (CheckHitKey(KEY_INPUT_DOWN)) {
			player[0].ty += player[0].sp;
		}
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		player[0].tx -= player[0].sp;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		player[0].tx += player[0].sp;
	}

	//��ʊO�ɏo�Ȃ��悤��
	if (player[0].tx < player[0].w/2)
		player[0].tx = player[0].w / 2;
	if (player[0].tx + player[0].w/2 >= WINDOW_WIDTH)
		player[0].tx = WINDOW_WIDTH - player[0].w/2;

	switch (player[0].knd) {
	case 1:
		if (player[0].ty < player[0].h/2)
			player[0].ty = player[0].h/2;
		if (player[0].ty + player[0].h/2 >= WINDOW_HEIGHT - 90)
			player[0].ty = WINDOW_HEIGHT - 90 - player[0].h/2;
		break;
	}

	player[0].x = player[0].tx - player[0].w / 2;
	player[0].y = player[0].ty - player[0].h / 2;


	/*�C��̊p�x*/
	if (player[0].knd == 2) {
		if (CheckHitKey(KEY_INPUT_D))
			player[0].ang += PI / 180;
		if (CheckHitKey(KEY_INPUT_A))
			player[0].ang -= PI / 180;

		if (player[0].ang > 0)
			player[0].ang = 0;
		if (player[0].ang < -PI * 2 / 3)
			player[0].ang = -PI * 2 / 3;
	}

}


static void PlayerShotEnter(int n) {
	for (int i = 0; i < PLAYER_SHOT_MAX; i++) {
		if (pla_shot[i].flag == 0) {
			pla_shot[i].flag = 1;
			pla_shot[i].knd = player[n].pattern2;
			pla_shot[i].cnt = 0;
			pla_shot[i].num = n;
			return;
		}
	}
}


void PlayerBulletMove() {
	for (int i = 0; i < PLAYER_SHOT_MAX; i++) {

		//�A�C�e������������̐؂�ւ�(��)
		if (CheckHitKey(KEY_INPUT_0))
			pla_shot[i].knd = 0;
		if (CheckHitKey(KEY_INPUT_1))
			pla_shot[i].knd = 1;

		if (player[0].flag > 0 && pla_shot[0].cnt == 0) {
			if (CheckHitKey(KEY_INPUT_S)) {
				if (pla_shot[i].flag > 0 && 0 <= pla_shot[i].knd && pla_shot[i].knd < PLAYER_SHOT_PATTERN_MAX) {

					//�e�̐���
					PlayerShotPattern[pla_shot[i].knd](i); //�e�̐���
				}
			}
		}

		if (pla_shot[i].cnt > 0)
			pla_shot[i].cnt--;
	}

	//�e�𓮂���
	for (int i = 0; i < PLAYER_SHOT_MAX; i++) {
		if (pla_shot[i].flag > 0) {
			PlayerShotCalc(i);
		}
	}
}

static void PlayerShotCalc(int n) {
	for (int j = 0; j < SHOT_BULLET_MAX; j++) {
		if (pla_shot[n].bullet[j].flag > 0) {

			pla_shot[n].bullet[j].tx += pla_shot[n].bullet[j].spd * cos(pla_shot[n].bullet[j].ang);
			pla_shot[n].bullet[j].ty += pla_shot[n].bullet[j].spd * sin(pla_shot[n].bullet[j].ang);
			pla_shot[n].bullet[j].x = pla_shot[n].bullet[j].tx - pla_shot[n].bullet[j].w / 2;
			pla_shot[n].bullet[j].y = pla_shot[n].bullet[j].ty - pla_shot[n].bullet[j].h / 2;
			pla_shot[n].bullet[j].cnt++;

			/*�G�ƒe�̓����蔻��*/
			for (int k = 0; k < ENEMY_MAX; k++) {
				if (enemy[k].flag > 0) {
					if (PTamaEnemyHitJudge(pla_shot[n].bullet[j], enemy[k])) {
						enemy[k].hp -= player[pla_shot[n].num].power;
						enemy[k].flag2 = 60;
						if (enemy[k].hp <= 0) {
							enemy[k].flag = 2;

							//�A�C�e�����o��
							if(enemy[k].item > 0)
								ItemEnter(k);
						}

						pla_shot[n].bullet[j].flag = 0;
						break;
					}
				}
			}

			/*��ʊO�ɏo����A*/
			if (pla_shot[n].bullet[j].tx < 0 || WINDOW_WIDTH < pla_shot[n].bullet[j].tx ||
				pla_shot[n].bullet[j].ty < 0 || WINDOW_HEIGHT < pla_shot[n].bullet[j].ty)
				pla_shot[n].bullet[j].flag = 0;
		}
	}

}