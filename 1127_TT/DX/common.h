#include "DxLib.h"

/*
�̂��ɁA�@��I���������PlayerEnter�֐�����ꍞ��(���͏������ɗL)
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
	int flag; //��������Ă��邩
	int flag2; //�_���[�W���󂯂���Ԃ�(�󂯂Ă�����A1�ȏ�ɂ���)
	int knd, knd2, pattern2; //�ǂ̋@�킩 //�퓬�@(1)�����(2)�� //�����̍U�����@
	int power; //�U����
	int sp; //���x
	double ang; //�C��̊p�x(�퓬�@��0)
	int num; //�����́�P��
}PlayerData;

typedef struct {
	int w, h; //�v���C���[�摜
	int hp_max;
	int knd, knd2, sp;
	int pattern2, blknd, blCnt;
	int power;
	int w2, h2; //�C��摜
}PlayerOrder;

typedef struct {
	int x, y, w, h; //�摜
	int tx, ty;
	int flag; //��������Ă��邩
	int flag2; //�_���[�W���󂯂���Ԃ�(�󂯂Ă�����A1�ȏ�ɂ���)
	int hp, hp_max; //�̗�
	int wait;
	int knd, pattern, sp, cnt; //��ށA�ړ�
	int pattern2, blknd, blW, blH, blCnt, bltime; //�U��
	int item, item2;
}EnemyData;

typedef struct {
	int tx, ty, w, h; //�摜�̈ʒu�A�傫��
	int hp_max, flag;
	int mission, stage, wait; //�X�e�[�W
	int knd, pattern, sp; //��ށA�ړ�
	int pattern2, blknd, blW, blH, blCnt, bltime; //�U��
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
	int flag, cnt, knd, num; //�t���O�A�J�E���g�A��ށA�G�̔ԍ�
	Bullet bullet[SHOT_BULLET_MAX];
}Shot;

typedef struct {
	int x, y; //����̍��W
	int tx, ty, w, h; //���S���W�A���A����
	int flag; //�t���O
	int spd; //���x
	int knd, knd2; //���,knd=1�̂Ƃ��A�ύX����V���b�g�̔ԍ�
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
int MissionSelectMove();//�I������ς���

/*KyaraSelect.cpp*/
void KyaraSelectLoad();
void KyaraSelectDraw();
int KyaraSelectMove();//�I������ς���
void FighterSelectDraw();
void FighterSelectMove();
void TankSelectDraw();
void TankSelectMove();

/*PlayerData.cpp*/
void PlayerLoad();
void PlayerInit();
void PlayerDraw();
void PlayerDataLoad();
void PlayerEnter(); //�I�������@��̃f�[�^��ǂݍ���
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

