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
	//プレイヤー
	for (int i = 0; i < PLAYER_MAX; i++) {
		player[i] = {0};
	}
	HP_max = 0;
	HP_num = HP_max;

	//弾
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
	tmp.knd2 = knd; //後で変える
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
	int fp;//ファイルを読み込む型
	int input[64];
	char inputc[64];

	fp = FileRead_open("PlayerData.csv");//ファイル読み込み
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}
	for (int i = 0; i < 2; i++)//最初の2行読み飛ばす
		while (FileRead_getc(fp) != '\n');

	int n = 0;//行
	int num = 0;//列

	while (1) {
		for (int i = 0; i < 64; i++) {
			inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する
													 //printfDx("%c",inputc[i]);
			if (inputc[i] == '/') {//スラッシュがあれば
				while (FileRead_getc(fp) != '\n');//改行までループ
				i = -1;//カウンタを最初に戻して
				continue;//forまで戻る
			}
			if (input[i] == ',' || input[i] == '\n') {//カンマか改行なら
				inputc[i] = '\0';//そこまでを文字列とし

				break;
			}
			if (input[i] == EOF) {//ファイルの終わりなら
				goto EXFILE;//終了(EXFILEまで飛ぶ)
			}
		}
		//printfDx("%d,",atoi(inputc));
		switch (num) {
		case 0: playerOrder[n].knd = atoi(inputc); break; //プレイヤーの種類  //atoi:文字列を数式に変える
		case 1: playerOrder[n].knd2 = atoi(inputc); break; //戦闘機か戦車か
		case 2: playerOrder[n].sp = atoi(inputc); break; //動く速度
		case 3: playerOrder[n].pattern2 = atoi(inputc); break; //攻撃パターン
		case 4: playerOrder[n].w = atoi(inputc); break; //(画像の)幅
		case 5: playerOrder[n].h = atoi(inputc); break; //(画像の)高さ
		case 6: playerOrder[n].hp_max = atoi(inputc); break; //体力
		case 7: playerOrder[n].power = atoi(inputc); break; //(画像の)幅
		case 8: playerOrder[n].w2 = atoi(inputc); break; //(画像の)幅
		case 9: playerOrder[n].h2 = atoi(inputc); break; //(画像の)高さ
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


/*機種をクライアント全員が設定したときのみ読み込む (0はなし)*/
void PlayerEnter() {
	player[0].knd = 1; //種類を変えるときに、この値を変える
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
					//player[i].hp_max = playerOrder[t].hp_max; //不要
					HP_max += playerOrder[t].hp_max;

					player[i].flag2 = 0;
					//player[i].hp = player[i].hp_max; //不要
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

					PlayerShotEnter(0);//後に"i"に変える

					break;
				}
			}
		}
	}
	HP_num = HP_max;
}


void PlayerMove() {
	/*プレーヤーの移動*/
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

	//画面外に出ないように
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


	/*砲台の角度*/
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

		//アイテムを取った時の切り替え(仮)
		if (CheckHitKey(KEY_INPUT_0))
			pla_shot[i].knd = 0;
		if (CheckHitKey(KEY_INPUT_1))
			pla_shot[i].knd = 1;

		if (player[0].flag > 0 && pla_shot[0].cnt == 0) {
			if (CheckHitKey(KEY_INPUT_S)) {
				if (pla_shot[i].flag > 0 && 0 <= pla_shot[i].knd && pla_shot[i].knd < PLAYER_SHOT_PATTERN_MAX) {

					//弾の生成
					PlayerShotPattern[pla_shot[i].knd](i); //弾の生成
				}
			}
		}

		if (pla_shot[i].cnt > 0)
			pla_shot[i].cnt--;
	}

	//弾を動かす
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

			/*敵と弾の当たり判定*/
			for (int k = 0; k < ENEMY_MAX; k++) {
				if (enemy[k].flag > 0) {
					if (PTamaEnemyHitJudge(pla_shot[n].bullet[j], enemy[k])) {
						enemy[k].hp -= player[pla_shot[n].num].power;
						enemy[k].flag2 = 60;
						if (enemy[k].hp <= 0) {
							enemy[k].flag = 2;

							//アイテムを出す
							if(enemy[k].item > 0)
								ItemEnter(k);
						}

						pla_shot[n].bullet[j].flag = 0;
						break;
					}
				}
			}

			/*画面外に出たら、*/
			if (pla_shot[n].bullet[j].tx < 0 || WINDOW_WIDTH < pla_shot[n].bullet[j].tx ||
				pla_shot[n].bullet[j].ty < 0 || WINDOW_HEIGHT < pla_shot[n].bullet[j].ty)
				pla_shot[n].bullet[j].flag = 0;
		}
	}

}