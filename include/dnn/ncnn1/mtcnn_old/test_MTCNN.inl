
#include "cstd.h"
#include "network.h"
#include "mtcnn.h"
#include <time.h>

#include "network.inl"
#include "pBox.inl"
#include "mtcnn.inl"


#ifdef _WIN64
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib,"opencv_ts320.lib")
#pragma comment(lib,"strmiids.lib")
#endif

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib,"opencv_world300d.lib")
#else
#pragma comment(lib,"opencv_world300.lib")
#endif
// _cblas_sgemm
// _cblas_sgemv
#pragma comment(lib,"libopenblas.lib")
#endif

#include "cap.h"
#undef ACCESS_MASK

int test_MTCNN() {
	capdev cap[1] = { 0 };
	int h = 720, w = 1280;
#ifdef _WIN32
	sys_chdir("E:/code/cstd/include/facelib/MTCNN");
#endif
	if (capdshow_open(cap, 0, w, h) > 0) {
		int fram = 0;
		char ch = 0;
		img_t im[1] = { 0 };
		cap_getframe(cap, im, CAP_RGB);
		Mat image;
		h = im->h, w = im->w;
		mtcnn find(h, w);
		for (; 'q' != ch && 'Q' != ch; ++fram) {
			cap_getframe(cap, im, CAP_RGB);
			image = Mat(h, w, CV_8UC3, im->tt.data, im->s);
			{utime_start(_start_time);
			find.findFace(image);
			printf("%f\n", utime_elapsed(_start_time)); }
			imshow_(im);
			ch = WaitKey(10);
		}
		cap_close(cap);
		imfree(im);
	}
	return 0;
}

int test_MTCNN1()
{
	sys_chdir("E:/code/cstd/include/facelib/MTCNN");
	if (1) {
		Mat image = imread("4.jpg");
		mtcnn find(image.rows, image.cols);
		clock_t start;
		start = clock();
		find.findFace(image);
		imshow("result", image);
		imwrite("result.jpg", image);
		start = clock() - start;
		cout << "time is  " << start / 10e3 << endl;
	}

	if (1) {
		Mat image;
		VideoCapture cap(0);
		int h = 720, w = 1280;
		cap.set(CAP_PROP_FRAME_WIDTH, w);
		cap.set(CAP_PROP_FRAME_HEIGHT, h);
		mtcnn find(h, w);
		if (!cap.isOpened())
			cout << "fail to open!" << endl;
		for (;;) {
		  cap >> image;
		  {utime_start(_start_time);
		  find.findFace(image);
		  printf("%f\n", utime_elapsed(_start_time)); }
		  imshow("result", image);
		  waitKey(10);
		}
		if (!image.data) {
			cout << "读取视频失败" << endl;
			return -1;
		}
	}

    // mtcnn find(image.rows, image.cols);
    // clock_t start;
    // int stop = 1200;
    // while(stop--){
    //     start = clock();
    //     cap>>image;
    //     find.findFace(image);
    //     imshow("result", image);
    //     if( waitKey(1)>=0 ) break;
    //     start = clock() -start;
    //     cout<<"time is  "<<start/10e3<<endl;
    // }

    waitKey(0);
    //image.release();
    return 0;
}