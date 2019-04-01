
#include "ocr_detect_back.hpp"
//#include "ocr/ocr_caffe.hpp"

#ifdef _DEBUG
#define MODELPATH "E:/OCR_Line/bin/model/"
#else
#define MODELPATH "./model/"
#endif

int test_ocr_detect_back() {
	vstr_t sv[1] = { 0 };
	int i, n;
	//buf_t bf[1] = {0}; // 228251_190044
	char* fn = NULL;
	fn = "E:/data/ew_id/listjpgb.txt";
	vstr_load(fn, sv);
	IdCardBackDetecter idcard_back_detecter;
	if (1) {
		char* fn = NULL;
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
		idcard_back_detecter.init(MODELPATH"idcard_back_cas_20190326.txt");
	}
	i = 0;
	//i = 11;
	for (; i<sv->n; ++i) {
		string ss = (char*)sv->v[i].s;
		if (!strstr(ss.c_str(), "223428_186282")) { continue; }
		Mat im = imread(ss.c_str(), cv::IMREAD_COLOR);
		if (im.rows < 400) {
			continue;
		}
		//imshow2("im1", im);
		if (im.rows>800) {
			im = resize(im, 800. / im.rows);
		}
		n = 0;
		std::vector<vector<RotatedRect>> rr;
		for (int i = 0; i < 4; ++i) {
			Mat im1;
			rotate90(im, im1, i);
			n += idcard_back_detecter.run(im1, rr, i);
			//n = cvFaceDetect(ca, gry, NULL, 0.5, NULL, out, countof(out));
		}
		drawRotatedRects(im, rr, 2);
		const char* fnext = GetFileNameExt(ss.c_str());
		printf("%d, %d %s\n", i, n, fnext);
		int j;
		for (j = 0; j<n; ++j) {
			//imdraw_xrects(im, out, n);
			//imdraw_rect(im, 0, NULL, rc, 0, _RGB(255, 0, 0), 1);
			//Rect rc = idcard_back_detecter.rect[j];
			//rectangle(im, rc, CV_RGB(255, 0, 0), 1, 8, 0);
		}
		if (0) {
			imshow("im", im);
			waitKey(-1);
		}
		if (1) {
			char buf[256];
			_snprintf(buf, 256, "E:/OCR_Line/back/out/%d_%s", i, fnext);
			imwrite(buf, im);
		}

	}
	//bffree(bf);
	vstr_free(sv);
	return 0;
}

