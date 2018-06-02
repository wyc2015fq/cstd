
#include <direct.h>
//#include "debug.h"

#include "cstd.h"
#include "ui/window.inl"
#include "img/imgio.inl"
#include "luobo/luobo.inl"

int test_luobo()
{
  float feat[4000 * 4]={0};
  uchar img[80 * 80 * 3] = {0};

  if (0) {
    img_t im[10] = {0};
    imsetsize(im, 160, 160, 3, 1);
    imgetfeature_10(im, feat);
  }

  if (0) {
    MEM_CHECK_BEGIN();
    getfeature_10(img, feat);
    MEM_CHECK_END();
  }

  if (0) {
    int i = 0;
    {
      utime_start(_start_time);

      for (i = 0; i < 1; ++i) {
        getfeature_10(img, feat);
      }

      printf("%f\n", utime_elapsed(_start_time));
    }
  }
  
  if (0) {
		int n=20;
		float feat[4000]={0};
		uchar img[80*80*3]={0};
		MEM_CHECK_BEGIN();
		getfeature_10(img, feat);
		MEM_CHECK_END();
    cvShowMat("feat", "%I32f", 4000/n, n, feat, n*sizeof(float), sizeof(float), -1); cvWaitKey(-1);
		img[0]=0;
  }
  if (1) {
    img_t im[10] = {0};
    int i, n=1;
    double sum=0;
    static const double feat1[] = {
#include "D:/pub/bin/adaboost/lobo/1.txt"
    };
    _chdir("D:/pub/bin/adaboost/lobo");
    imread("1.bmp", 3, 1, im);
    imgetfeature_10(im, feat);
    for (i=0; i<4000; ++i) {
      sum += fabs(feat1[i]-feat[i]);
      ASSERT(sum<0.01);
    }
    ASSERT(sum<0.01);
    //cvShowMat("feat", "%I32f", 4000/n, n, feat, n*sizeof(float), sizeof(float), -1); cvWaitKey(-1);
    imfree(im);
  }
  
  if (0) {
    img_t im[10] = {0};
    int n=1;
    vstr_t sv[1] = {0};
    int i, buflen, fnn;
    char buf[40960];
    char fn[256];
    _chdir("D:/pub/bin/adaboost/lobo");
    vstr_load("poslist.ini", sv);
    for (i=0; i<sv->n; ++i) {
      imread(sv->v[i].s, 3, 1, im);
      imgetfeature_10(im, feat);
      buflen = str_nprintf_arr(buf, 40960, "%I32f\n", 4000, feat);
      fnn = STRpath_split(sv->v[i].s, sv->v[i].l, "dpn", fn, 256);
      memcpy(fn+fnn, ".txt", 4), fnn+=4;
      fn[fnn]=0;
      //_snprintf(fn, 256, "%s.txt", str);
      savedata_11(fn, buf, buflen, 0);
      fnn = STRpath_split(sv->v[i].s, sv->v[i].l, "nx", fn, 256);
      printf("%s\n", fn);
    }
    //cvShowMat("feat", "%I32f", 4000/n, n, feat, n*sizeof(float), sizeof(float), -1); cvWaitKey(-1);
    imfree(im);
    strv_free(sv);
  }

  //cvWaitKey(-1);
  //imshow(im); cvWaitKey(-1);
  return 0;
}
