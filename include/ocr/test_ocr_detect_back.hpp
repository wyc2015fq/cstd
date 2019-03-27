
#include "ocr_detect_back.hpp"
//#include "ocr/ocr_caffe.hpp"

int test_ocr_detect_back() {
	char* fn = NULL;
	vstr_t sv[1] = { 0 };
	int i, n;
	//buf_t bf[1] = {0};
	sys_chdir("D:/pub/bin/adaboost/lobo");
	sys_chdir("D:/pub/faceplusplus/gender/out");
	sys_chdir("E:/OCR_Line/adaboost");
	sys_chdir("E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com");
	vstr_load("../backend_2.txt", sv);
	IdCardBackDetecter idcard_back_detecter;
	if (1) {
		fn = "haar_LUT_nesting_cas.dat";
		fn = "cas.dat";
		fn = "haar_gen_cas.dat";
		fn = "gender_cas.dat";
		fn = "idcard_cas.dat";
		fn = "idcard_cas.dat";
		fn = "cas.txt";
		//ca->w = 100, ca->h = 100;
		//ca->w = 30, ca->h = 20;
		// idcard_back_20190326
		idcard_back_detecter.init("cas.txt");
	}
	i = 0;
	i = 55;
	for (; i<sv->n; ++i) {
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
		for (int i = 0; i < 4; ++i) {
			Mat im1;
			rotate90(im, im1, i);
			n += idcard_back_detecter.run(im1, rr, i);
			//n = cvFaceDetect(ca, gry, NULL, 0.5, NULL, out, countof(out));
		}
		drawRotatedRects(im, rr, 1);
		printf("%d, %d\n", i, n);
		int j;
		for (j = 0; j<n; ++j) {
			//imdraw_xrects(im, out, n);
			//imdraw_rect(im, 0, NULL, rc, 0, _RGB(255, 0, 0), 1);
			//Rect rc = idcard_back_detecter.rect[j];
			//rectangle(im, rc, CV_RGB(255, 0, 0), 1, 8, 0);
		}
		imshow("im", im);
		waitKey(-1);
	}
	//bffree(bf);
	vstr_free(sv);
	return 0;
}
#if 0
int test_ocr_detect_back() {
	char* fn = NULL;
	vstr_t sv[1] = { 0 };
	img_t im[2] = { 0 };
	img_t* gry = im + 1;
	int i, n;
	XRECT out[10000];
	//buf_t bf[1] = {0};
	sys_chdir("D:/pub/bin/adaboost/lobo");
	sys_chdir("D:/pub/faceplusplus/gender/out");
	sys_chdir("E:/OCR_Line/adaboost");
	sys_chdir("E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com");
	vstr_load("../backend_2.txt", sv);
	IdCardBackDetecter idcard_back_detecter;
	if (1) {
		fn = "haar_LUT_nesting_cas.dat";
		fn = "cas.dat";
		fn = "haar_gen_cas.dat";
		fn = "gender_cas.dat";
		fn = "idcard_cas.dat";
		fn = "idcard_cas.dat";
		fn = "cas.txt";
		//ca->w = 100, ca->h = 100;
		//ca->w = 30, ca->h = 20;
		// idcard_back_20190326
		idcard_back_detecter.init("cas.txt");
	}

	for (i = 0; i<sv->n; ++i) {
		if (!imread((char*)sv->v[i].s, 3, 1, im)) continue;
		if (im->h < 400) {
			continue;
		}
		//imshow2("im1", im);
		if (im->h>400) {
			imresize1(im, 400. / im->h, im);
		}
		imcolorcvt(im, gry, T_BGR, T_GRAY);
		n = 0;
		memset(out, 0, sizeof(out));

		if (1) {
			n = objdetect(ca, gry->h, gry->w, gry->data, gry->s, NULL, 0, 0.5, 1, 100, 1.1, 1, 0.8, 5, );
			//n = cvFaceDetect(ca, gry, NULL, 0.5, NULL, out, countof(out));
		}
		printf("%d\n", n);
		int j;
		for (j = 0; j<n; ++j) {
			//imdraw_xrects(im, out, n);
			if (out[j].count>5) {
				IRect rc = iRect(out[j].x, out[j].y, out[j].w, out[j].h);
				//imdraw_rect(im, 0, NULL, rc, 0, _RGB(255, 0, 0), 1);
				rectangle(im, rc, _RGB(255, 0, 0), 1, 8, 0);
			}
		}
		imshow_(im);
		waitkey(-1);
	}
	//bffree(bf);
	freeims(im, 2);
	vstr_free(sv);
	return 0;
}
#endif

