/*
**
*****************************************************************************
**
** Project     : OpenCV sample project
** Module      : image_cv.c
** Description : OpenCVによる画像の読み込み(カラープレーンの置き換え)
**
** Version : Date:          Author:       Comment:
**     1.0   2011/09/16(金) Akinori Tsuji Creation
**
** Version : Date:          Author:       Comment:
**     2.0   2014/05/28(水) Momoyo Ito    Modified
*****************************************************************************
**
*/
/*________ INCLUDES _______________________________________________________*/
#include <stdio.h>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

/*________ MACROS _________________________________________________________*/
/*________ DEFINITIONS ____________________________________________________*/
#define CONVERT_RGB

/*________ VARIABLES ______________________________________________________*/
/*________ LOCAL-FUNCTION PROTOTYPES ______________________________________*/
/*________ LOCAL-FUNCTIONS ________________________________________________*/

/*________ MAIN-FUNCTION __________________________________________________*/

int main(int argc, char **argv)
{
    int x, y;
    uchar p[3];//画素値を格納する配列
    uchar q[3];//画素値を格納する配列
    IplImage *img, *img2, *img3;
    IplImage *imgMask, *imgMask1; //
    IplImage *imgMask_dst;

    img = cvLoadImage("test.jpg", CV_LOAD_IMAGE_COLOR);//cvLoadImage  : 画像を読み込む関数
    //imgMask = cvLoadImage("mask.png", CV_LOAD_IMAGE_GRAYSCALE);//cvLoadImage  : 画像を読み込む関数
    imgMask = cvLoadImage("mask.png", CV_LOAD_IMAGE_COLOR);
    imgMask1 = cvLoadImage("mask1.png", CV_LOAD_IMAGE_COLOR); //反応なし

    if (img == NULL) {
        fprintf(stderr, "*Error* cannot open test.jpg\n");
        return;
    }

    img2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cvSet(img2, cvScalar(0, 0, 0, 0), NULL);
    img3 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    cvSet(img3, cvScalar(0, 0, 0, 0), NULL);
    //imgMask_dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    //cvSet(imgMask_dst, cvScalar(0, 0, 0, 0), NULL);

    //cvCopy(imgMask, imgMask_dst, NULL);
    //cvCopy(imgMask1, imgMask_dst, NULL);

    //cvCopy(img, img2, imgMask_dst);
    //cvCopy(imgMask, img2, imgMask1);
    //cvCopy(img, img3, img2);


/*imgの色を変換*/
#ifdef CONVERT_RGB
    for (y = 0; y < img2->height; y++) {
	for (x = 0; x < img2->width; x++) {
           /*img->widthStep  : 画像横１行のバイト幅（画像を読み込んだ時点で格納済）*/
	    p[0] = imgMask->imageData[imgMask->widthStep * y + x * 3];	        // B
	    p[1] = imgMask->imageData[imgMask->widthStep * y + x * 3 + 1];	// G
	    p[2] = imgMask->imageData[imgMask->widthStep * y + x * 3 + 2];	// R

	    q[0] = imgMask1->imageData[imgMask1->widthStep * y + x * 3];	        // B
	    q[1] = imgMask1->imageData[imgMask1->widthStep * y + x * 3 + 1];	// G
	    q[2] = imgMask1->imageData[imgMask1->widthStep * y + x * 3 + 2];	// R

// Image Processing
           /*中身を変えないまま、値を戻す(１の場合)。０の場合は、画素値を０にして戻す*/
            /*if(x > img2->width/2){
                img2->imageData[img2->widthStep * y + x * 3] =
                    cvRound(p[0] * 0 + q[0] * 1);
                img2->imageData[img2->widthStep * y + x * 3 + 1] =
                    cvRound(p[1] * 0 + q[1]*1);
                img2->imageData[img2->widthStep * y + x * 3 + 2] =
                    cvRound(p[2] * 1 + q[2]*1);
            }
            else{*/
                img2->imageData[img2->widthStep * y + x * 3] =
                    cvRound(p[0] * 1 + q[0]*1);
                img2->imageData[img2->widthStep * y + x * 3 + 1] =
                    cvRound(p[1] * 1 + q[1]*1);
                img2->imageData[img2->widthStep * y + x * 3 + 2] =
                    cvRound(p[2] * 1 + q[2]*1);
                //}
        }
    }
#endif

    //cvCopy(imgMask, img2, imgMask1);
    cvCopy(img, img3, img2);

    cvNamedWindow("after", CV_WINDOW_AUTOSIZE);//ウィンドウの生成
    cvShowImage("after", img);
    /*cvNamedWindow("before", CV_WINDOW_AUTOSIZE);//ウィンドウの生成
      cvShowImage("before", img2);*/
    cvNamedWindow("img3", CV_WINDOW_AUTOSIZE);
    cvShowImage("img3", img3);
    cvNamedWindow("imgMask", CV_WINDOW_AUTOSIZE);//ウィンドウの生成
    cvShowImage("imgMask", imgMask);
    cvNamedWindow("imgMask1", CV_WINDOW_AUTOSIZE);//ウィンドウの生成
    cvShowImage("imgMask1", imgMask1);
    //cvNamedWindow("imgMask_dst", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);//ウィンドウの生成
    //cvShowImage("imgMask_dst", imgMask_dst);

    cvWaitKey(0);//何かキーが押された時に終了させる

    cvDestroyWindow("after");//imageウィンドウの破棄
    //cvDestroyWindow("before");//image2ウィンドウの破棄
    cvDestroyWindow("img3");//image2ウィンドウの破棄
    cvDestroyWindow("imgMask");//imgMaskウィンドウの破棄
    cvDestroyWindow("imgMask1");//imgMask1ウィンドウの破棄
    //cvDestroyWindow("imgMask_dst");//imgMask_dstウィンドウの破棄

    cvReleaseImage(&img);//imgのメモリの解放
    cvReleaseImage(&img2);//img2のメモリの解放
    cvReleaseImage(&img3);//img3のメモリの解放
    cvReleaseImage(&imgMask);//imgMaskのメモリの解放
    cvReleaseImage(&imgMask1);//imgMaskのメモリの解放
    //cvReleaseImage(&imgMask_dst);//imgMaskのメモリの解放

    return 0;
}

