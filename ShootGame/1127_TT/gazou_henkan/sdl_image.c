/*************************************************
sdl_image.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lSDL_image -L/usr/lib -g -lopencv_core -lopencv_highgui -lopencv_imgproc
備考：画像は適当に用意してください（白背景、アルファチャンネルで背景透過した画像等）．
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード

//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window; // ウィンドウ（画像）データへのポインタ
	SDL_Surface *image, *image2;  // 画像データへのポインタ

	// 画像の座標とサイズ
	SDL_Rect src_rect = { 0, 0, 800, 600 }; // コピー元画像のコピーする一領域の座標とサイズ（x, y, w, h）
	SDL_Rect dst_rect = { 0, 10 }; // 画像のコピー先の座標（x, y）

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	// ウィンドウ生成（800*600、1677万色）
	if((window = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	// BMP画像以外
	image=IMG_Load("sozai/背景-砂漠.png");	//
	SDL_BlitSurface(image, &src_rect, window, &dst_rect);	// ウィンドウ（ここではSDL_GetVideoSurface関数で指定）に画像を貼り付ける

	image=IMG_Load("sozai/player.png");	//
	SDL_BlitSurface(image, &src_rect, window, &dst_rect);	// ウィンドウ（ここではSDL_GetVideoSurface関数で指定）に画像を貼り付ける

SDL_SaveBMP(window, "sozai/Gousei.bmp");

IplImage *imgA = cvLoadImage("sozai/Gousei.bmp", CV_LOAD_IMAGE_COLOR);



	SDL_Flip(window);	// 画面にアニメーションを表示（反映）

	SDL_Delay(5000);	// 5秒間スリープ

	SDL_FreeSurface(window);	// ウィンドウ（サーフェイス）の解放
	SDL_FreeSurface(image);	// 画像（サーフェイス）の解放

	SDL_Quit();	// SDLの利用終了
	return 0;
}

