#include "DxLib.h"

/*
のちに、機種選択した後にPlayerEnter関数を入れ込む(今は初期化に有)
*/

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ENEMY_MAX 3 //21
#define ENEMY_ORDER_MAX 5
#define ENEMY_PATTERN_MAX 2
#define ENEMY_SHOT_PATTERN_MAX 2
#define PLAYER_MAX 4
#define PLAYER_ORDER_MAX 2
#define PLAYER_SHOT_PATTERN_MAX 2
#define PLAYER_SHOT_MAX 1
#define ENEMY_SHOT_MAX 3
#define SHOT_BULLET_MAX 100
#define ITEM_MAX 3 //21
#define ITEM_KND_MAX 4
#define PI 3.1415926535

typedef enum {
	GAME_TITLE,
	GAME_SELECT,
	GAME_MACHINE,
	GAME_MACH_FIGHTER,
	GAME_MACH_TANK,
	GAME_MAIN,
	GAME_OVER,
	GAME_CLEAR
}GAME_STATE;

typedef struct {
	int x, y, w, h;
	int tx, ty;
	int flag; //生成されているか
	int flag2; //ダメージを受けた状態か(受けていたら、1以上にする)
	int knd, knd2, pattern2; //どの機種か //戦闘機(1)か戦車(2)か //初期の攻撃方法
	int power; //攻撃力
	int sp; //速度
	double ang; //砲台の角度(戦闘機は0)
	int num; //自分は○Pか
}PlayerData;

typedef struct {
	int w, h; //プレイヤー画像
	int hp_max;
	int knd, knd2, sp;
	int pattern2, blknd, blCnt;
	int power;
	int w2, h2; //砲台画像
}PlayerOrder;

typedef struct {
	int x, y, w, h; //画像
	int tx, ty;
	int flag; //生成されているか
	int flag2; //ダメージを受けた状態か(受けていたら、1以上にする)
	int hp, hp_max; //体力
	int wait;
	int knd, pattern, sp, cnt; //種類、移動
	int pattern2, blknd, blW, blH, blCnt, bltime; //攻撃
	int item, item2;
}EnemyData;

typedef struct {
	int tx, ty, w, h; //画像の位置、大きさ
	int hp_max, flag;
	int mission, stage, wait; //ステージ
	int knd, pattern, sp; //種類、移動
	int pattern2, blknd, blW, blH, blCnt, bltime; //攻撃
	int item, item2;
}EnemyOrder;

typedef struct {
	int w, h;
	double x, y, tx, ty;
	int ghandle;
	int flag, knd, cnt, state;
	double ang, spd;
}Bullet;

typedef struct {
	int flag, cnt, knd, num; //フラグ、カウント、種類、敵の番号
	Bullet bullet[SHOT_BULLET_MAX];
}Shot;

typedef struct {
	int x, y; //左上の座標
	int tx, ty, w, h; //中心座標、幅、高さ
	int flag; //フラグ
	int spd; //速度
	int knd, knd2; //種類,knd=1のとき、変更するショットの番号
}ItemData;

extern PlayerData player[];
extern PlayerOrder playerOrder[];
extern Shot pla_shot[];
extern EnemyData enemy[];
extern EnemyOrder enemyOrder[];
extern Shot ene_shot[];
extern ItemData item[];
extern int score;
extern int HP_num;
extern int HP_max;
extern int stopCnt;

/*main.cpp*/
void DrawGameTitle();
void DrawGameSelect();
void DrawGameMachine();
void DrawGameMachFighter();
void DrawGameMachTank();
void DrawGameMain();
void DrawGameOver();
void DrawGameClear();

/*MissionSelect.cpp*/
void MissionSelectLoad();
void MissionSelectDraw();
int MissionSelectMove();//選択肢を変える

/*KyaraSelect.cpp*/
void KyaraSelectLoad();
void KyaraSelectDraw();
int KyaraSelectMove();//選択肢を変える
void FighterSelectDraw();
void FighterSelectMove();
void TankSelectDraw();
void TankSelectMove();

/*PlayerData.cpp*/
void PlayerLoad();
void PlayerInit();
void PlayerDraw();
void PlayerDataLoad();
void PlayerEnter(); //選択した機種のデータを読み込む
void PlayerMove();
void PlayerBulletMove();

/*PlayerShotPattern.cpp*/
void PlayerShotPattern0(int);
void PlayerShotPattern1(int);

/*System.cpp*/
int PlayerEnemyHitJudge(PlayerData a, EnemyData b);
int PTamaEnemyHitJudge(Bullet a, EnemyData x);
int ETamaPlayerHitJjudge(Bullet a, PlayerData x);
int PlayerItemHitJudge(PlayerData a, ItemData x);
void HPbox();
void AllLoad();
void AllInit();

/*EnemyData.cpp*/
void EnemyLoad();
void EnemyInit();
void EnemyDraw();
void EnemyDataLoad();
void EnemyEnter();
void EnemyMove();
void EnemyBulletMove();

/*EnemyPattern.cpp*/
void EnemyPattern0(int);
void EnemyPattern1(int);

/*EnemyShotPattern.cpp*/
void EnemyShotPattern0(int);
void EnemyShotPattern1(int);

/*ItemData.cpp*/
void ItemLoad();
void ItemInit();
void ItemDraw();
void ItemMove();
void ItemEnter(int n);

