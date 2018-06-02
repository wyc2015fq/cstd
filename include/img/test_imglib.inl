
//#include "colortable.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imglib.h"

#include "cstd.h"
#include "macro.h"
#include "findfile.h"
#include "filters.h"
#include "..\ml\matmul.h"
#include "xtypes.h"
//#include "cxcore.inl"
#include "cvcanny.inl"
#include "../fmtimg/fmtimg.inl"
#include "imdraw.inl"
#include "window.inl"

#if 0
int ImageDFT(int height, int width, unsigned char* data, int step, unsigned char* out, int out_step)
{
  int rimg_step = width * 2;
  // cvCanny(height, width, data, step, 1, out, step, 1, 60, 171);
  real* rimg = MALLOC(real, height * rimg_step);
  real aa[] = {1, 0, 2, 0, 3, 0, 4, 0}, bb[ 8 ] = {0};
  //cvDFT(1, 4, aa, 4, 2, 1, 4, bb, 4, 2, 0, 1);
  //cvDFT(1, 4, bb, 4, 2, 1, 4, aa, 4, 2, 1, 1);
  FILL(height * rimg_step, rimg, 0.f);
  SKIP_COPY(height, width, data, step, 1, rimg, rimg_step, 2);
  {
    real tim;
    utime_start(_start_time);
    //for (i=100; i--; )
    cvDFT(height, width, rimg, rimg_step, 2, height, width, rimg, rimg_step, 2, 0, height);
    tim = (real) utime_elapsed(_start_time);
    printf("time = %f\n", tim);
  }
#define LOGMAG(compx, _uch)  *(_uch) = (real)log(1+sqrt(*(compx) * *(compx) + *((compx)+1) * *((compx)+1)))

  IMTRANS1D(height * width, rimg, 2, rimg, 1, LOGMAG);
#undef LOGMAG

  {
    int minId, maxId;
    real a, b;
    MINMAX_ELEM(height * width, rimg, 1, minId, maxId);
    a = (255.f) / (rimg[ maxId ] - rimg[ minId ]);
    b = -a * rimg[ minId ];
    FOR_EACH_OP_1D2A(height * width, rimg, 1, rimg, 1, = b + a*);
    SKIP_COPY_CAST(height, width, rimg, width, 1, out, out_step, 1, (unsigned char));
  }
  {
    int height2 = height / 2, width2 = width / 2;
    unsigned char t, *out1 = out + width2, *out2 = out + height2 * out_step, *out3 = out + width2 + height2 * out_step;
    SKIP_SWAP(height2, width2, out, out_step, 1, out3, out_step, 1, t);
    SKIP_SWAP(height2, width2, out1, out_step, 1, out2, out_step, 1, t);
  }
  SAFEFREE(rimg);
  return 0;
}


int ImageCorner(int height, int width, unsigned char* data, int step, unsigned char* out, int out_step, int cn)
{
  void
  cvGoodFeaturesToTrack1(int height, int width, const unsigned char * img, int img_step,
      float * eig, int eig_step,
      float * tmp, int tmp_step,
      CvPoint2D32f * corners, int * corner_count,
      double quality_level, double min_distance,
      const unsigned char * mask, int mask_step,
      int block_size, int use_harris, double harris_k);
#define max_corners 100

  int i, cornerCount = max_corners;
  CvPoint2D32f corners[ max_corners ];
  float* corners1, *corners2;
  //create empty images of same size as the copied images
  corners1 = MALLOC(float, height * width);
  corners2 = MALLOC(float, height * width);

  cvGoodFeaturesToTrack1(height, width, data, step, corners1, width, corners2, width,
      corners, &cornerCount, 0.05, 5, 0, 0, 3, 0, 0.04);

  for (i = 0; i < cornerCount; ++i) {
    IPOINT pt = {(int)(corners[ i ].x), (int)(corners[ i ].y) };
    DrawCircle(height, width, out, out_step, cn, pt, 1, CC_RGB(255, 0, 0), 2, CC_AA, 0);
  }

  SAFEFREE(corners1);
  SAFEFREE(corners2);
  return cornerCount;
}
#endif

int test_imglib()
{
  char buf[ 256 ];
  char wndname[] = "Edge";
  char tbarname[] = "Threshold";
  _chdir("D:/pub/test");
  _mkdir("./out");
#ifdef _DEBUG

  cvNamedWindow(wndname, 1);
#define DODDD(height, width, out, step, cn)  cvShowImage(wndname, height, width, out, step, cn); imwrite(buf, height, width, out, step, 1);cvWaitKey(0)
#else
#define DODDD(height, width, out, step, cn)  imwrite(buf, height, width, out, step, 1)
#endif

  begin_find(info, "*.*");
  const char* picname = fileinfo_filename(info);
  int height, width, cn;
  struct fmtimgreader* reader = new_reader(picname, &height, &width, &cn);

  if (reader) {
    unsigned char* data, * out;
    int step;
    cn = 1;
    step = width * cn;
    data = (unsigned char*) pmalloc(height * step);
    out = (unsigned char*) pmalloc(height * step);
    memset(data, 128, height * step);
    imread(reader, data, step, cn > 1);
    //imwrite("2", height, width, out, step, cn);
    //imwrite("2.jpg", height, width, out, step, cn);
    //imwrite("2.png", height, width, out, step, cn);
    //imwrite("2.pgm", height, width, out, step, cn);

#define BEGIN_FUN(N) if (N) { utime_start(u); memcpy(out, data, height * step);
#define END_FUN(_STR)           \
  _snprintf(buf, 256, ".\\out\\%s_%s", _STR, picname); \
  printf("%13.9f %s\n", utime_elapsed(u), buf);        \
  DODDD(height, width, out, step, 1);   \
}

    BEGIN_FUN(1)
    cvCanny(height, width, data, step, 1, out, step, 1, 60, 171);
    END_FUN("cvCanny")
#if 0
    BEGIN_FUN(1)
    ImageDFT(height, width, data, step, out, step);
    END_FUN("ImageDFT")
    BEGIN_FUN(1)
    ImageCorner(height, width, data, step, out, step, 1);
    END_FUN("ImageCorner")
    BEGIN_FUN(1)
    SKIP_FILTER_RCEX(height, width, data, step, cn, out, step, cn, 3, 3, GAUSS7X7_FT, GAUSS7X7_FT, unsigned char);
    END_FUN("bilateral_filter")
    BEGIN_FUN(1)
    gauss_filter(height, width, data, step, cn, out, step, cn, 3);
    END_FUN("bilateral_filter")
    BEGIN_FUN(1)
    bilateral_filter(height, width, data, step, cn, out, step, cn, 3, 30);
    END_FUN("bilateral_filter")
    BEGIN_FUN(0)
    Canny(height, width, data, out, 1., 0.4, 0.8);
    END_FUN("Canny")
    BEGIN_FUN(1)
    shen_castan(height, width, out, step, out, step, 0.9f);
    END_FUN("shen_castan")
    BEGIN_FUN(1)
    WindowTrans(height, width, out, step, 30, 150);
    END_FUN("WindowTrans")
    BEGIN_FUN(1)
    OUTLINE(height, width, out, step);
    END_FUN("OUTLINE")
    BEGIN_FUN(1)
    STREETDIS(height, width, out, step);
    END_FUN("STREETDIS")
    BEGIN_FUN(0)
    FREAMEWORK(height, width, out, step);
    END_FUN("FREAMEWORK")
    BEGIN_FUN(1)
    HOLENUMBER(height, width, out, step);
    END_FUN("HOLENUMBER")
    BEGIN_FUN(1)
    LinearSharpen(height, width, out, step);
    END_FUN("LinearSharpen")
    BEGIN_FUN(1)
    MedianFilter(height, width, out, step, 3, 3, 1, 1);
    END_FUN("MedianFilter")
    BEGIN_FUN(1)
    HistogramEqualize(height, width, out, step);
    END_FUN("HistogramEqualize")
    BEGIN_FUN(1)
    DWTStep(height, width, out, step, 0);
    END_FUN("DWTStep")
    BEGIN_FUN(1)
    Prewitt(height, width, out, step);
    END_FUN("Prewitt")
    BEGIN_FUN(1)
    Kirsch(height, width, out, step);
    END_FUN("Kirsch")
    BEGIN_FUN(1)
    Gauss(height, width, out, step);
    END_FUN("Gauss")
    BEGIN_FUN(0)
    Hough(height, width, out, step);
    END_FUN("Hough")
    BEGIN_FUN(1)
    Robert(height, width, out, step);
    END_FUN("Robert")
    BEGIN_FUN(1)
    Sobel(height, width, out, step);
    END_FUN("Sobel")
    BEGIN_FUN(1)
    Walsh(height, width, out, step);
    END_FUN("Walsh")
    BEGIN_FUN(1)
    Dct(height, width, out, step);
    END_FUN("Dct")
    BEGIN_FUN(1)
    LimbPatternBayer(height, width, out, step);
    END_FUN("LimbPatternBayer");
    BEGIN_FUN(1)
    DitherFloydSteinberg(height, width, out, step);
    END_FUN("DitherFloydSteinberg");
    BEGIN_FUN(1)
    Fourier(height, width, out, step);
    END_FUN("Fourier");
    BEGIN_FUN(0)
    Hotelling(height, width, out, step);
    END_FUN("Hotelling");
    BEGIN_FUN(0)
    NoRestriction(height, width, out, step);
    END_FUN("NoRestriction");
    BEGIN_FUN(0)
    InverseFilter(height, width, out, step);
    END_FUN("InverseFilter");
    BEGIN_FUN(0)
    NoiseDegeneration(height, width, out, step);
    END_FUN("NoiseDegeneration");
    BEGIN_FUN(0)
    WinnerFilter(height, width, out, step);
    END_FUN("WinnerFilter");
    BEGIN_FUN(0)
    MotionDegeneration(height, width, out, step);
    END_FUN("MotionDegeneration");
    BEGIN_FUN(0)
    MotionRestore(height, width, out, step);
    END_FUN("MotionRestore");
#endif

    free_reader(reader);
    pfree(data);
    pfree(out);
  }

  end_find(info);
  return 0;
}

int main()
{
  test_imglib();
  return 0;
}
