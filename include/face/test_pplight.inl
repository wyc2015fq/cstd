
#include "cstd.h"
#include "img/filters.h"
#include "face/pplight_32f.inl"
#include "face/pplight_16s.inl"
uchar g_buf[1<<20];

#include "img/imgio.inl"
int test_pplight() {
	img_t im[2] = {0};
	img_t* im1 = im+1;
	imread("F:/pub/bin/face/00007_2008_03_04_20_45_50_671.bmp", 1, 1, im);
	savedata_inl("C:/stdface_100x100.inl", im->tt.data, 100*100);
	imsetsize(im1, 100, 100, 1, 1);
	pplight_16s(im->h, im->w, im->tt.data, im->s, 1, im1->tt.data, im1->s, 1, g_buf);
	imshow(im);
	imshow(im1);
	cvWaitKey(0);
	imfrees(im, 2);
	return 0;
}
