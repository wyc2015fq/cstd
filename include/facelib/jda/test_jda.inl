
#include "cstd.h"
#include "jda.inl"
#include "str.h"
#include "cfile.h"
#include "img/imgio.inl"
#include "img/color.inl"
#include "draw/imdraw.inl"
#include "cap.h"

int test_jda() {
	const char* fn = ("E:/data/fddb/FDDB-folds/FDDB-fold-01.txt");
	const char* path = ("E:/data/fddb/images");
	vstr_t sv[1] = { 0 };
	vstr_load(fn, sv);
	int i, j;
	char buf[256];
	//img_t im[10] = {0};

	sys_chdir("C:/local/JDA/model");
	void* cascador = NULL;
#if 0
	cascador = jdaCascadorCreateDouble("jda.model");
	jdaCascadorSerializeTo(cascador, "jda_float32.model");
	jdaCascadorRelease(cascador);
#endif
	cascador = jdaCascadorCreateFloat("jda_float32.model");
	img_t im[10] = { 0 };
	img_t* img = im;
	img_t* gray = im + 1;

	for (i = 0; i < sv->n; ++i) {
		_snprintf(buf, 256, "%s/%s.jpg", path, sv->v[i].s);
		if (imread(buf, 3, 1, im)) {
			printf("%s\n", sv->v[i].s);
			imcolorcvt(img, gray, T_BGR, T_GRAY);
			//facedetect.detect(im, );
			//objectdetect.detect();
			enum { N = 1 };
			jdaResult res[N] = {0};
			{utime_start(_start_time);
			for (j = 0; j < N; j++) {
				res[j] = jdaDetect(cascador, gray->data, img->cols, img->rows, 1.25, 0.1, 40, -1, -0.5);
			}
			printf("%f\n", utime_elapsed(_start_time)); }
#if 1
			jdaResult result = res[0];
			for (j = 0; j < result.n; j++) {
				CRect r = cRect(result.bboxes[3 * j + 0], result.bboxes[3 * j + 1], result.bboxes[3 * j + 2], result.bboxes[3 * j + 2]);
				float* shape = &result.shapes[2 * result.landmark_n*j];
				rectangle(img, r, cScalar(0, 0, 255), 2);
				char buff[200];
				sprintf(buff, "%.4lf", result.scores[j]);
				putText(img, buff, cPoint(r.x, r.y), CC_FONT_HERSHEY_PLAIN, 1, cScalar(0, 255, 0), 2);
				for (int j = 0; j < result.landmark_n; j++) {
					circle(img, cPoint(shape[2 * j], shape[2 * j + 1]), 2, cScalar(0, 255, 0), -1);
				}
			}
#endif

			for (int j = 0; j < N; j++) {
				jdaResultRelease(res[j]);
			}


			imshow_(im);
			WaitKey(-1);
		}
	}
	vstr_free(sv);
	jdaCascadorRelease(cascador);
	imfrees2(im);
	return 0;
}

#ifdef _CAP_H_
int test_jda_cam() {
	int i, j;
	//img_t im[10] = {0};

	int ch = 0, fram = 0;
	capdev cap[1] = { 0 };
	int h = 720, w = 1280;
	h = 1080, w = 1920;

	//h = 480, w = 640, minsize=60;
	if (capdshow_open(cap, 0, w, h) > 0) {
		img_t im[10] = { 0 };
		img_t* img = im;
		img_t* gray = im + 1;
		sys_chdir("C:/local/JDA/model");
		void* cascador = NULL;
		cascador = jdaCascadorCreateFloat("jda_float32.model");

		for (; 'q' != ch && 'Q' != ch; ++fram) {
			cap_getframe(cap, im, CAP_RGB);

			imcolorcvt(img, gray, T_BGR, T_GRAY);
			//facedetect.detect(im, );
			//objectdetect.detect();
			enum { N = 1 };
			jdaResult res[N] = { 0 };
			{utime_start(_start_time);
			for (j = 0; j < N; j++) {
				res[j] = jdaDetect(cascador, gray->data, img->cols, img->rows, 1.25, 0.1, 150, -1, -0.5);
			}
			printf("%f\n", utime_elapsed(_start_time)); }
#if 1
			jdaResult result = res[0];
			for (j = 0; j < result.n; j++) {
				CRect r = cRect(result.bboxes[3 * j + 0], result.bboxes[3 * j + 1], result.bboxes[3 * j + 2], result.bboxes[3 * j + 2]);
				float* shape = &result.shapes[2 * result.landmark_n*j];
				rectangle(img, r, cScalar(0, 0, 255), 2);
				char buff[200];
				sprintf(buff, "%.4lf", result.scores[j]);
				putText(img, buff, cPoint(r.x, r.y), CC_FONT_HERSHEY_PLAIN, 1, cScalar(0, 255, 0), 2);
				for (int j = 0; j < result.landmark_n; j++) {
					circle(img, cPoint(shape[2 * j], shape[2 * j + 1]), 2, cScalar(0, 255, 0), -1);
				}
			}
#endif

			for (int j = 0; j < N; j++) {
				jdaResultRelease(res[j]);
			}

			imshow_(im);
			ch = WaitKey(10);
		}
		jdaCascadorRelease(cascador);
		imfrees2(im);
	}

	cap_close(cap);
	return 0;
}
#endif
