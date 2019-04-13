#ifndef _TEST_CAM_INL_
#define _TEST_CAM_INL_

#include "cap/cap.inl"
#include "img/imgio.inl"
#include "utime.h"
#include "std/gui_c.h"
#include "std/drawing_c.h"
//#include "cap/capdshow.inl"

#ifdef _WIN32
#define cap_open capdshow_open
#else
#define cap_open capv4l_open
#endif

#if 0
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
int test_cam() {
	VideoCapture capture;
  if (capture.open(0)) {
    int frame = 0;
    Mat cap;
    for (; frame<10; ++frame) {
		  capture.read(cap);
      printf(" %d %d\n", cap.cols, cap.rows);
    }
  }
  return 0;
}
#endif

#define __CRECTF_DEFINED__
#define __FPOINT_DEFINED__
typedef FRect CRectf;
#include "facedetect.inl"

double stringDistance(const char* stringA, const char* stringB) {
	int lengthA = strlen(stringA);
	int lengthB = strlen(stringB);
	int step = (lengthB + 1);
	float* distanceMatrix = (float*)malloc(sizeof(float)*(lengthA + 1)*step);

	for (int i = 0; i <= lengthA; i++) {
		distanceMatrix[i*step+0] = i;
	}

	for (int j = 1; j <= lengthB; j++) {
		distanceMatrix[0 * step + j] = j;
	}

	for (int i = 1; i <= lengthA; i++) {
		for (int j = 1; j <= lengthB; j++) {
			if (stringA[i - 1] == stringB[j - 1]) {
				distanceMatrix[i*step + j] = distanceMatrix[(i - 1)*step + j - 1];
			}
			else {
				int t11 = distanceMatrix[(i - 1)*step + j - 1] + 1;
				distanceMatrix[i*step + j] = 1 + MIN(distanceMatrix[(i - 1)*step + j],
					MIN(distanceMatrix[i*step + j - 1], t11));
			}
		}
	}
	double ret = distanceMatrix[lengthA*step + lengthB];
	return ret;
}

int struniq(char* s) {
	int pre = 0;
	char* p = s;
	char* s0 = s;
	for (; *s;) {
		char c = *s++;
		if (c != pre) {
			pre = c;
			*p++ = c;
		}
	}
	*p = 0;
	return p - s0;
}


double strDist(const char* a, const char* b) {
	int lengthA = strlen(a);
	int lengthB = strlen(b);
	double d = stringDistance(a, b);
	return d/(lengthA+ lengthB);
}


int test_facedetect_cam() {
  capdev cap[1] = {0};
  int ret;
  double t = stringDistance("123", "abc");
  //ret = cap_open(cap, 0, 720, 576);
  //ret = cap_open(cap, 0, 640, 480);
  ret = cap_open(cap, 0, 1280, 960);
  
  if (ret > 0) {
    int frame = 0;
    char ch = 0;
    img_t im[1] = {0};
    

	int i, buflen = 100 * (1 << 20);
	unsigned char *buf = NULL;
	buf = (unsigned char*)malloc(buflen);

    //imsetsize(im, 100, 100, 3, 1);
    //imwrite("/storage/emulated/pic/pic_aa.jpg", im);
	char code[] = "udlr";
	char maxbuf[100] = "";
	double dd = 0;
	for (; 'q' != ch && 'Q' != ch && frame < 100000; ++frame) {
		cap_getframe(cap, im, CAP_RGB);
		struct FaceSet* faceset = NULL;
	  if (1) {
		  {utime_start(_start_time);
		  faceset = facedetect(buf, buflen, im->h, im->w, im->data, im->s, im->c, 60);
		  printf("%f ", utime_elapsed(_start_time)); }
		  if (faceset) {
			  for (i = 0; i<faceset->nfaces; ++i) {
				  struct Face* face = faceset->faces + i;
				  IRect rect = { (int)face->rect.x, (int)face->rect.y, (int)face->rect.width, (int)face->rect.height };
				  if (1) {
					  char buf[64];
					  //_snprintf(buf, 64, "%f %f %f\n", face->headpose.pitch_angle, face->headpose.yaw_angle, face->headpose.roll_angle);
					  //printf("%f %f %f ", face->headpose.pitch_angle, face->headpose.yaw_angle, face->headpose.roll_angle);
				  }
				  if (1) {
					  char t = '0';
					  double p = face->headpose.pitch_angle-4, y = face->headpose.yaw_angle;
					  p *= 2;
					  if (fabs(p) > 10 || fabs(y) > 10) {
						  if (fabs(p) > fabs(y)) {
							  t = p < 0 ? 'u' : 'd';
						  }
						  else {
							  t = y < 0 ? 'l' : 'r';
						  }
					  }
					  if ('0' != t) {
						  if (strlen(maxbuf) < sizeof(maxbuf)) {
							  char tmp[] = { t, '\0' };
							  strcat(maxbuf, tmp);
							  struniq(maxbuf);
							  dd = strDist(maxbuf, code);
						  }
						  else {
							  maxbuf[0] = 0;
						  }
					  }
					  printf("%c %f %f %f ", t, p, y, dd);
				  }
				  rectangle(im, rect, RGB(255, 0, 0), 1, 8);
				  if (0)
					  printf("%2d (%3d %3d %3d %3d)(%6.2f %6.2f %6.2f)\n", i,
					  (int)face->rect.x, (int)face->rect.y, (int)face->rect.width, (int)face->rect.height,
					  face->headpose.pitch_angle, face->headpose.yaw_angle, face->headpose.roll_angle);
				  
			  }
		  }
	  }
	  if (1) {
		  imshow_(im);
		  ch = waitkey(10);
		  char buf[256];
		  _snprintf(buf, 256, "/storage/emulated/pic/pic_%d.jpg", frame);
		  //imwrite(buf, im);
		  //_snprintf(buf, 256, "/storage/emulated/pic/pic_%d.dat", frame);
		  //savefile(buf, im->tt.data, im->h*im->s);
	  }
      printf("pic %d w=%d h=%d\n", frame, im->w, im->h);
    }
    cap_close(cap);
    imfree(im);
  }
  return 0;
}



#endif // _TEST_CAM_INL_
