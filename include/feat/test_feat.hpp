

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
	_objectdetect facedetect;

	for (i = 0; i < sv->n; ++i) {
		_snprintf(buf, 256, "%s/%s.jpg", path, sv->v[i].s);
		if (imread(buf, 3, 1, &im)) {
			printf("%s\n", sv->v[i].s);
			facedetect.init(im.w, im.h, 1.1, frontalface, countof(frontalface));
			//facedetect.detect(im, );
			//objectdetect.detect();
			imshow_(&im);
			WaitKey(-1);
		}
	}
	vstr_free(sv);
	//imfrees2(im);
	return 0;
}
