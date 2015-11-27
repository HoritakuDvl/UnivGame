#include "common.h"
#include <stdlib.h>
#include <time.h>

PlayerData player[PLAYER_MAX];
PlayerOrder playerOrder[PLAYER_ORDER_MAX];
Shot pla_shot[PLAYER_SHOT_MAX];
EnemyData enemy[ENEMY_MAX];
EnemyOrder enemyOrder[ENEMY_ORDER_MAX];
Shot ene_shot[ENEMY_SHOT_MAX];
ItemData item[ITEM_MAX];
int score = 0;
int HP_num = 0;
int HP_max = 0;
int stopCnt = 180;

GAME_STATE gstate = GAME_TITLE;
int white;

static int TitleHaikei;
static int haikei;
static int num = 0;

//プログラムはWinMainから始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetGraphMode(800, 600, 32); //画面モードの変更
	ChangeWindowMode(TRUE); //ウィンドウモードに設定
	if (DxLib_Init() == -1) { //DXライブラリ初期化処理
		return -1; //エラーが起きたら直ちに終了
	}
	//背景の読み込み
	TitleHaikei = LoadGraph("sozai\\title.png");
	haikei = LoadGraph("sozai\\haikei1.png");

	//画像の読み込み
	AllLoad();

	//初期化
	AllInit();
	srand((unsigned)time(NULL));

	//各設定
	white = GetColor(255, 255, 255);

	SetDrawScreen(DX_SCREEN_BACK); //描画先グラフィック領域の指定
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();
		switch (gstate) {
		case GAME_TITLE:
			DrawGameTitle();
			break;
		case GAME_SELECT:
			DrawGameSelect();
			break;
		case GAME_MACHINE:
			DrawGameMachine();
			break;
		case GAME_MACH_FIGHTER:
			DrawGameMachFighter();
			break;
		case GAME_MACH_TANK:
			DrawGameMachTank();
			break;
		case GAME_MAIN:
			DrawGameMain();
			break;
		case GAME_OVER:
			DrawGameOver();
			break;
		case GAME_CLEAR:
			DrawGameClear();
			break;
		}
		ScreenFlip();
	}

	WaitKey(); //キー入力待ち
	DxLib_End(); //DXライブラリ使用の終了処理
	return 0; //ソフトの終了
}

void DrawGameTitle() {
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, white, TRUE);
	DrawGraph(0, 0, TitleHaikei, TRUE);
	DrawString(200, 250, "Game Title", white);
	DrawString(300, 400, "Push Any Key", white);

	if (num == 0) {
		if (CheckHitKey(KEY_INPUT_Z)) {
			num = 30;
		}
	}
	else if (num > 0) {
		num--;
		if (num == 0)
			gstate = GAME_SELECT;
	}
}

void DrawGameSelect() {
	int flag;

	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, white, TRUE);
	MissionSelectDraw();
	flag = MissionSelectMove();

	if (flag == 3) { //missionを選択したら //3,4,5,6⇒mission1,2,3,4
		gstate = GAME_MACHINE;//GAME_MAIN;
		AllInit();
	}
}

void DrawGameMachine() {
	int flag;

	//DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, white, TRUE);
	KyaraSelectDraw();
	flag = KyaraSelectMove();

	switch (flag) {
	case 1:
		gstate = GAME_MACH_FIGHTER;
		break;
	case 2:
		gstate = GAME_MACH_TANK;
		break;
	}
}

void DrawGameMachFighter() {
	FighterSelectDraw();
	FighterSelectMove();
}

void DrawGameMachTank() {
	TankSelectDraw();
	TankSelectMove();
}

void DrawGameMain() {
	if (stopCnt > 0)
		stopCnt--;

	DrawGraph(-1, -1, haikei, TRUE);
	HPbox();
	EnemyDraw();
	ItemDraw();
	PlayerDraw();

	if (stopCnt == 0) {
		EnemyMove();
		PlayerMove();
		EnemyEnter();

		PlayerBulletMove();
		EnemyBulletMove();
		ItemMove();
	}
}

void DrawGameOver() {

}

void DrawGameClear() {

}


