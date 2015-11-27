#include "common.h"
#include <math.h>

static int gEnemy, gEnemy2;
static int gTama;

static void EnemyShotEnter(int n);
static void EnemyShotCalc(int n);

void(*EnemyPattern[ENEMY_PATTERN_MAX])(int) = {
	EnemyPattern0, EnemyPattern1
};

void(*EnemyShotPattern[ENEMY_SHOT_PATTERN_MAX])(int) = {
	EnemyShotPattern0, EnemyShotPattern1
};

void EnemyLoad() {
	gEnemy = LoadGraph("sozai\\enemy1.png");
	gEnemy2 = LoadGraph("sozai\\enemy2.png");
	gTama = LoadGraph("sozai\\Etama0.png");
}

void EnemyInit() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i] = { 0 };
	}

	for (int i = 0; i < ENEMY_SHOT_MAX; i++){
		ene_shot[i] = { 0 };
		for (int j = 0; j < SHOT_BULLET_MAX; j++) {
			ene_shot[i].bullet[j] = { 0 };
		}
	}
}

void EnemyDraw() {
	//�e
	for (int i = 0; i < ENEMY_SHOT_MAX; i++) {
		for (int j = 0; j < SHOT_BULLET_MAX; j++) {
			if (ene_shot[i].bullet[j].flag > 0) {
				switch (ene_shot[i].bullet[j].knd) {
				case 0:
					DrawRotaGraph(ene_shot[i].bullet[j].tx, ene_shot[i].bullet[j].ty, 1, ene_shot[i].bullet[j].ang, gTama, TRUE, FALSE);
					break;
				}
			}
		}
	}

	//�G
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].flag == 1) {
			int cnt = enemy[i].flag2 % 14;
			if (cnt <= 4 || 11 <= cnt) {
				switch (enemy[i].knd) {
				case 0:
					DrawGraph(enemy[i].x, enemy[i].y, gEnemy, TRUE);
					break;
				case 1:
					DrawGraph(enemy[i].x, enemy[i].y, gEnemy2, TRUE);
					break;
				}
			}

			if (enemy[i].flag2 > 0)
				enemy[i].flag2--;
		}
	}
}

void EnemyDataLoad() {
	int fp;//�t�@�C����ǂݍ��ތ^
	int input[64];
	char inputc[64];

	fp = FileRead_open("EnemyData.csv");//�t�@�C���ǂݍ���
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
		case 0: enemyOrder[n].mission = atoi(inputc); break; //�~�b�V����  //atoi:������𐔎��ɕς���
		case 1: enemyOrder[n].stage = atoi(inputc); break; //�X�e�[�W
		case 2: enemyOrder[n].wait = atoi(inputc); break; //�ҋ@����
		case 3: enemyOrder[n].knd = atoi(inputc); break; //�G�̎��
		case 4: enemyOrder[n].pattern = atoi(inputc); break; //�G�̓����p�^�[��
		case 5: enemyOrder[n].sp = atoi(inputc); break; //�G�̓������x
		case 6: enemyOrder[n].pattern2 = atoi(inputc); break; //�G�̍U������p�^�[��
		case 7: enemyOrder[n].blknd = atoi(inputc); break; //�G���łe�̎��
		case 8: enemyOrder[n].blW = atoi(inputc); break; //�e�摜�̕�
		case 9: enemyOrder[n].blH = atoi(inputc); break; //�e�摜�̍���
		case 10: enemyOrder[n].blCnt = atoi(inputc); break; //�e�ł��̊Ԋu
		case 11: enemyOrder[n].bltime = atoi(inputc); break; //�G���o�����Ă���ł܂ł̎���
		case 12: enemyOrder[n].tx = atoi(inputc); break; //x���W
		case 13: enemyOrder[n].ty = atoi(inputc); break; //y���W
		case 14: enemyOrder[n].w = atoi(inputc); break; //��
		case 15: enemyOrder[n].h = atoi(inputc); break; //����
		case 16: enemyOrder[n].hp_max = atoi(inputc); break; //�ő�HP
		case 17: enemyOrder[n].item = atoi(inputc); break; //���Ƃ��A�C�e���̎��
		case 18: enemyOrder[n].item2 = atoi(inputc); break; //item=1�̂Ƃ��A�ύX����V���b�g�̔ԍ�
		}
		num++;
		if (num == 19) {
			num = 0;
			enemyOrder[n].flag = 1;
			n++;
		}
	}
EXFILE:
	FileRead_close(fp);
}

void EnemyEnter() {
	for (int t = 0; t < ENEMY_ORDER_MAX; t++) {
		if (enemyOrder[t].flag == 1 && enemyOrder[t].mission == 1 && enemyOrder[t].stage == 1) {
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (enemy[i].flag == 0) {
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
					enemy[i].bltime = enemyOrder[t].bltime; //�e�ł��܂ł̎���
					enemy[i].tx = enemyOrder[t].tx;
					enemy[i].ty = enemyOrder[t].ty;
					enemy[i].w = enemyOrder[t].w;
					enemy[i].h = enemyOrder[t].h;
					enemy[i].hp_max = enemyOrder[t].hp_max;
					enemy[i].item = enemyOrder[t].item;
					enemy[i].item2 = enemyOrder[t].item2;

					enemy[i].x = enemy[i].tx - enemy[i].w / 2;
					enemy[i].y = enemy[i].ty - enemy[i].h / 2;
					enemy[i].hp = enemy[i].hp_max;

					enemyOrder[t].flag = 0;
					break;
				}
			}
		}
	}
}

void EnemyMove() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].flag > 0) {
			if (enemy[i].wait == 0) {
				EnemyPattern[enemy[i].pattern](i);
				enemy[i].x = enemy[i].tx - enemy[i].w / 2;
				enemy[i].y = enemy[i].ty - enemy[i].h / 2;

				if (enemy[i].bltime == enemy[i].cnt)
					EnemyShotEnter(i);
				enemy[i].cnt++;

				//�v���C���[�ƓG�̂����蔻��
				for (int k = 0; k < PLAYER_MAX; k++) {
					if (player[k].flag > 0) {
						if (player[k].flag2 == 0 && PlayerEnemyHitJudge(player[k], enemy[i])) {
							HP_num--;
							player[k].flag2 = 180;
						}
					}
				}

			}
			else if (enemy[i].wait > 0)
				enemy[i].wait--;
		}
	}
}

static void EnemyShotEnter(int n) {//�V���b�g��łĂ�悤�ɂ���
	for (int i = 0; i < ENEMY_SHOT_MAX; i++) {
		if (ene_shot[i].flag == 0) {
			ene_shot[i].flag = 1;
			ene_shot[i].knd = enemy[n].pattern2;
			ene_shot[i].num = n;
			ene_shot[i].cnt = 0;
			return;
		}
	}
}

void EnemyBulletMove() {
	for (int i = 0; i < ENEMY_SHOT_MAX; i++) {
		if (ene_shot[i].flag != 0 && 0 <= ene_shot[i].knd && ene_shot[i].knd < ENEMY_SHOT_PATTERN_MAX) {
			EnemyShotPattern[ene_shot[i].knd](i); //�e�̐���
			EnemyShotCalc(i); //�e�𓮂���
			ene_shot[i].cnt++;
		}
	}
}

static void EnemyShotCalc(int n) {
	int max = 0;
	if (enemy[ene_shot[n].num].flag != 1) //�G���|���ꂽ��
		ene_shot[n].flag = 2; //����ȏ�e�𐶐����Ȃ��t���O�ɕς���
	for (int j = 0; j < SHOT_BULLET_MAX; j++) {
		if (ene_shot[n].bullet[j].flag > 0) { //�������Ă�����
			ene_shot[n].bullet[j].tx += cos(ene_shot[n].bullet[j].ang)*ene_shot[n].bullet[j].spd;
			ene_shot[n].bullet[j].ty += sin(ene_shot[n].bullet[j].ang)*ene_shot[n].bullet[j].spd;
			ene_shot[n].bullet[j].cnt++;

			//�v���C���[�ƒe�̂����蔻��
			for (int k = 0; k < PLAYER_MAX; k++) {
				if (player[k].flag > 0) {
					if (player[k].flag2 == 0 && ETamaPlayerHitJjudge(ene_shot[n].bullet[j], player[k])) {
						//player[k].hp--; //�s�v
						HP_num--;
						player[k].flag2 = 180;

						ene_shot[n].bullet[j].flag = 0;
						break;
					}
				}
			}

			//��ʊO�ɏo����
			if (ene_shot[n].bullet[j].tx < 0 || WINDOW_WIDTH < ene_shot[n].bullet[j].tx ||
				ene_shot[n].bullet[j].ty < 0 || WINDOW_HEIGHT < ene_shot[n].bullet[j].ty) {
				ene_shot[n].bullet[j].flag = 0; //�e������
			}
		}
	}

	//���ݕ\�����̒e����ł����邩�ǂ������ׂ�
	for (int j = 0; j < SHOT_BULLET_MAX; j++) {
		if (ene_shot[n].bullet[j].flag > 0)
			return;
	}

	//�G���|����Ă���A�������͂܂��o�����Ă��Ȃ��Ƃ�
	if (enemy[ene_shot[n].num].flag != 1) {
		ene_shot[n].flag = 0;
		enemy[ene_shot[n].num].flag = 0;
	}
}