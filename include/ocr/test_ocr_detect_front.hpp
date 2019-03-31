


#include "utime.h"
#include "ocr/ocr_detect_front.hpp"


#define CAS_PATH  "E:/OCR_Line/bin/model/"

int test_ocr_detect_front() {
	char* fn = NULL;
	vstr_t sv[1] = { 0 };
	int i, n;
	//buf_t bf[1] = {0};
	sys_chdir("D:/pub/bin/adaboost/lobo");
	sys_chdir("D:/pub/faceplusplus/gender/out");
	sys_chdir("E:/OCR_Line/adaboost");
	sys_chdir("");
	//vstr_load("../backend_2.txt", sv);
	char* txtlist = "E:/data/ew_id/backend_2.txt";
	txtlist = "E:/OCR_Line/bin/front/list.txt";
	txtlist = "E:/data/ew_id/listjpgf.txt";
	vstr_load(txtlist, sv);
	IdCardFrontDetecter idcard_front_detecter;
	if (1) {
		fn = "haar_LUT_nesting_cas.dat";
		fn = "cas.dat";
		fn = "haar_gen_cas.dat";
		fn = "gender_cas.dat";
		fn = "idcard_cas.dat";
		fn = "idcard_cas.dat";
		fn = CAS_PATH"idcard_front_cas_20180830.txt";
		fn = CAS_PATH"idcard_front_cas_20190330.txt";
		//ca->w = 100, ca->h = 100;
		//ca->w = 30, ca->h = 20;
		// idcard_back_20190326
		idcard_front_detecter.init(fn);
	}
	i = 0;
	//i = 88;
	for (; i<sv->n; ++i) {
		if (SHOW) {
			if (!strstr((char*)sv->v[i].s, "1276400_1152736.jpg")) { continue; }
		}
		const char* fnext = GetFileNameExt((char*)sv->v[i].s);
		Mat im = imread((char*)sv->v[i].s, cv::IMREAD_COLOR);
		if (im.rows < 400) {
			continue;
		}
		//imshow2("im1", im);
		if (im.rows>800) {
			im = resize(im, 800. / im.rows);
		}
		n = 0;
		std::vector<RotatedRect> rr;
		utime_start(_start_time);
		for (int i = 0; i < 4; ++i) {
			Mat im1;
			rotate90(im, im1, i);
			n += idcard_front_detecter.run(im1, rr, i);
			//n = cvFaceDetect(ca, gry, NULL, 0.5, NULL, out, countof(out));
		}
		double time1 = utime_elapsed(_start_time);
		//printf("utime  %lf\n", };
		//imshow("im1", im);
		//drawRotatedRects(im, idcard_front_detecter.lines_ok, 1);
		drawRotatedRects(im, rr, 2);
		printf("%d, %d   %lf %s\n", i, n, time1, fnext);
		int j;
		for (j = 0; j<n; ++j) {
			//imdraw_xrects(im, out, n);
			//imdraw_rect(im, 0, NULL, rc, 0, _RGB(255, 0, 0), 1);
			//Rect rc = idcard_front_detecter.rect[j];
			//rectangle(im, rc, CV_RGB(255, 0, 0), 1, 8, 0);
		}
		if (SHOW) {
			imshow("im", im);
			waitKey(-1);
		}
		if (1) {
			char buf[256];
			_snprintf(buf, 256, "E:/OCR_Line/front/out/%s", fnext);
			imwrite(buf, im);
		}
	}
	//bffree(bf);
	vstr_free(sv);
	return 0;
}



