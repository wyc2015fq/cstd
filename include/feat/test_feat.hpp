
#define C_DBG

#include "feat.hpp"
#include "objectdetect.hpp"
#include "str.h"
#include "cfile.h"
#include "img/imgio.inl"
//#include "objectdetect.frontalface.hpp"

float frontalface[] = {
#include "objectdetect.frontalface.txt"
};


int test_feat() {
	int aaa = CC_32S;
	auto bb = 1;
	auto aa = jsfeat.C1_t;
	const char* fn = ("E:/data/fddb/FDDB-folds/FDDB-fold-01.txt");
	const char* path = ("E:/data/fddb/images");
	vstr_t sv[1] = { 0 };
	vstr_load(fn, sv);
	int i;
	char buf[256];
	//img_t im[10] = {0};
	matrix_t im;
	int rects[1000];
	_objectdetect facedetect;

	for (i = 0; i < sv->n; ++i) {
		_snprintf(buf, 256, "%s/%s.jpg", path, sv->v[i].s);
		if (imread(buf, 3, 1, &im)) {
			printf("%s ", sv->v[i].s);
			int len=0;
			//{utime_start(_start_time);
			tic;
			len  = facedetect.detect(im, 1, 1, 1.1, frontalface, countof(frontalface), rects, countof(rects) / 4);
			toc;
			tictoc;
			//printf("%f ", utime_elapsed(_start_time)); }
			//facedetect.detect(im, );
			//objectdetect.detect();
			printf("\n");
			imshow_(&im);
			WaitKey(10);
		}
	}
	vstr_free(sv);
	//imfrees2(im);
	return 0;
}
