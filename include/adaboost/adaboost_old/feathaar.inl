
#include "imresize.h"
#include "img\imgopt.inl"

#define FMT_D5  _D(2d)_D(2d)_D(2d)_D(2d)_D(2d)
#define HAAR_FMT _D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)

#define FPRINTF_EOH(_F, _E)    fprintf(_F, FMT_D5, (_E).x, (_E).y, (_E).w, (_E).h, (_E).bi)
#define FSCANF_EOH(_F, _E)     fscanf(_F, FMT_D5, &(_E).x, &(_E).y, &(_E).w, &(_E).h, &(_E).bi)
#define FPRINTF_IRECTW(_F, _E) fprintf(_F, FMT_D5, (_E).x, (_E).y, (_E).w, (_E).h, (_E).d)
#define FSCANF_IRECTW(_F, _E)  fscanf(_F, FMT_D5, &(_E).x, &(_E).y, &(_E).w, &(_E).h, &(_E).d)
#define FPRINTF_HAAR(_F, _H)   FPRINTF_IRECTW(_F, (_H).ft0), FPRINTF_IRECTW(_F, (_H).ft1)
#define FSCANF_HAAR(_F, _H)    FSCANF_IRECTW(_F, (_H).ft0), FSCANF_IRECTW(_F, (_H).ft1)
#define FPRINTF_HAAR3(_F, _H)   FPRINTF_IRECTW(_F, (_H).ft0), FPRINTF_IRECTW(_F, (_H).ft1), FPRINTF_IRECTW(_F, (_H).ft2)
#define FSCANF_HAAR3(_F, _H)    FSCANF_IRECTW(_F, (_H).ft0), FSCANF_IRECTW(_F, (_H).ft1), FSCANF_IRECTW(_F, (_H).ft2)


double HAAR_feat_val(double* A, int al, HAARFEAT* haar, int i, double IV)
{
  double aa = HAAR_RECT_VAL(A, haar[ i ].ft0);
  double bb = HAAR_RECT_VAL(A, haar[ i ].ft1);
  return HAAR_FEAT_VAL(A, haar[ i ], IV);
}
double HAAR3_feat_val(double* A, int al, HAAR3FEAT* haar, int i, double IV)
{
  return HAAR3_FEAT_VAL(A, haar[ i ], IV);
}
double EOHEX_feat_val(double* A, int al, EOHFEAT* eoh, int i, double IV)
{
  return EOHEX_FEAT_VAL(A, eoh[ i ], IV);
}

int HAAR_setoffset(HAARFEAT* haar, int al, int ai, double cs)
{
  HAAR_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}
int HAAR3_setoffset(HAAR3FEAT* haar, int al, int ai, double cs)
{
  HAAR3_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}

int integral_sq(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi)
{
  SKIP_INTEGRAL_SQ(h, w, img, al, 1, mag, ml, mi);
  return 0;
}

typedef struct HAAR_FEAT_T {
  char postxtfile[256];
  char negtxtfile[256];
  HAARFEAT* haar;
  int allhaarlen;
  double* samp;
  int samp_size;
  int samp_step;
  int w, h;
  int stepxy;
  double zoom;
}
HAAR_FEAT_T;

int HAAR_samp_data(BOOST* hh, char** buf, int* size)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->samp);
  *size = fs->samp_size;
  return 0;
}

int HAAR_save_feat(BOOST* hh, int id)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);

  if (id < 0) {
    hh->ca->weaklen++;
  }
  else {
    hh->ca->haar[ hh->ca->weaklen++ ] = fs->haar[ id ];
  }

  return 0;
}

int HAAR_uninit(BOOST* hh)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  FREE(fs->haar);
  return 0;
}

double guiyi(int n, double* f, double* w)
{
  int i;
  double mimval = *f, maxval = *f;

  for (i = 0; i < n; ++i) {
    if (w[i] < 0.f) {
      continue;
    }

    if (f[i] > maxval) {
      maxval = f[i];
    }
    else if (f[i] < mimval) {
      mimval = f[i];
    }
  }

  for (i = 0; i < n; ++i) {
    //if (w[i]<0.f) continue;
    f[i] = (f[i] - mimval) / (maxval - mimval);
  }

  return 0;
}

double* HAAR_feat(BOOST* hh, int i)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  int j, cx = fs->w, stp = (cx + 1);
  const double* samp = fs->samp;
  HAARFEAT haar;
  //i = 2310;
  haar = fs->haar[ i ];
  HAAR_SETOFFSET(haar, stp, 1, 1.f);

  for (j = 0; j < hh->samp_num; ++j, samp += fs->samp_step) {
    if (hh->wi[ j ] < 0.f) {
      hh->fi[ j ] = 0;
    }
    else {
      hh->fi[ j ] = HAAR_FEAT_VAL(samp, haar, 1);  ///hh->vvv[j];
    }
  }

  return hh->fi;
}

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
int HAAR_find_samp(BOOST* hh, int type)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  int j = 0, n = 0, cx = hh->ca->w, cy = hh->ca->h;
  int cy1 = cy + 1, cx1 = cx + 1;
  int _step = (cx + 1);
  int _size = (cy + 1) * _step;
  int SampNum = (type > 0) ? (hh->pos_num) : (hh->samp_num - hh->pos_num);
  double* wi = hh->wi + (type > 0 ? 0 : hh->pos_num);
  //double * vvv = hh->vvv + ( type > 0 ? 0 : hh->pos_num );
  double* vi = hh->vi + (type > 0 ? 0 : hh->pos_num);
  double zoom = fs->zoom;
  int stepxy = fs->stepxy;
  const char* piclist = type > 0 ? fs->postxtfile : fs->negtxtfile;
  int picnum = 0, sampnum = 0, allsampnum = 0;
  char buf[ 256 ];
  img_t im[1] = {0};
  FILE* pf = fopen(piclist, "r");

  if (!pf) {
    printf("%s文件打不开", piclist);
    return 0;
  }

  fs->samp_step = _size;

  //stepxy = IniGetInt( INIFILENAME, "3", "样本检测步进", 3 );
  //IniGetFloat( INIFILENAME, "3", "缩放比", zoom );
  //IniGetFloat( INIFILENAME, ADABOOSTSECT, "最小负样本比例", hh->min_neg_rate );
  for (; sampnum < SampNum * hh->min_neg_rate && stepxy >= 1; stepxy = stepxy * 2 / 3, zoom = pow(zoom, 0.7)) {
    utime_start(time);

    if (zoom < 1.1) {
      zoom = 1.1;
    }

    if (1 == type) {
    }
    else {
      FILL(SampNum, wi, -1);
    }

    for (picnum = 0, sampnum = 0, j = 0; j < SampNum && fscanf(pf, "%s", buf) > 0;) {
      if (wi[ j ] >= 0.f) {
        ++j;
        ++sampnum;
        continue;
      }

      if (!imread(buf, 1, 1, im)) {
        continue;
      }

      if (1 == type) {
        imresize_1(im, fs->h, fs->w, im);
      }

      {
        int step = im->s;
        unsigned char* data = im->tt.data;
        int w = im->w, h = im->h;
        double* inter = MALLOC(double, (h + 1) * (w + 1) * HAAR_CN);
        double* inter0 = inter;
        double* inter1 = inter + (h + 1) * (w + 1);
        ++picnum;

        if (1 == type) {
          int x, y, ispass = 0;
          unsigned char* tmpimg = data;
          int stp = (w + 1);
          int siz = (h + 1) * stp;
          double score;
          double invarea = 1.f / (cx * cy);

          //int _al = stp * cy, _ai = cx;
          if (!(0 == h % cy && 0 == w % cx)) {
            printf("%s\n", buf);
          }

          assert(0 == h % cy && 0 == w % cx);
          FILL(siz * 2, inter, 0);
          //SKIP_HISTEQ(height, width, data, step, 1, data, step, 1);
          SKIP_INTEGRAL(h, w, data, step, 1, inter0, stp, 1);
          SKIP_INTEGRAL_GQ(h, w, data, step, 1, inter1, stp, 1);
          FEAT_SETOFFSET(hh->ca->haar, hh->ca->weaklen, cx1, 1, 1.f, HAAR);

          for (y = 0; y < h && j < SampNum; y += cy) {
            for (x = 0; x < w && j < SampNum; x += cx) {
              double* tmpSamp0 = inter0 + y * stp + x;
              double* tmpSamp1 = inter1 + y * stp + x;
              double* Samp = fs->samp + j * _size;
              double var;
              double invvar;
              ++allsampnum;
              //FPRINT2D("a1.txt", "%8.f, ", height, width, tmpSamp, stp, HAAR_CN);
              //FILL(cx1*cy1, inter0, 0);
              //FILL(cx1*cy1, inter1, 0);
              //SKIP_INTEGRAL( hh->h, hh->w, tmpimg + y * w + x, w, 1, inter0, hh->w+1, 1 );
              //SKIP_INTEGRAL_GQ( hh->h, hh->w, tmpimg + y * w + x, w, 1, inter1, hh->w+1, 1 );
              SKIP_INTEGRAL_VAR(inter0, cx1 * cy, cx, inter1, cx1 * cy, cx, invarea, var);
              //SKIP_INTEGRAL_VAR( tmpSamp0, _al, _ai, tmpSamp1, _al, _ai, invarea, var );
              score = 0.f;
              ispass = 1;
              invvar = (var <= 0.) ? 1. : (1. / sqrt(var));
              //if (invvar>1.f) invvar=1.f;
              //CASCADE_PASS(hh->ca, tmpSamp0, hh->ca->haar, HAAR_FEAT_VAL, invvar, ispass, score);
              SKIP_COPY(cy1, cx1, tmpSamp0, stp, 1, Samp, _step, 1);
              cascade_pass(hh->ca, Samp, _step, invvar, &ispass, &score);

              if (ispass) {
                int k;

                //FPRINT2D("a1.txt", "%8.f, ", cy1, cx1, Samp, _step, 1);
                for (k = 0; k < _size; ++k) {
                  Samp[ k ] *= invvar;
                }

                //vvv[ j ] = invvar;
                wi[ j ] = 1.f;
                vi[ j ] = score;
                ++j;
                ++sampnum;
              }
              else {
                //printf("%f %d ", invvar, hh->ca->is_nesting);
                //imwrite("a.bmp", height, width, data, step, 1);exit(0);
              }
            }
          }
        }

        if (-1 == type) {
          double ss = MIN(fs->h * 1. / im->h, fs->w * 1. / im->w);
          img_t im2[1] = {0};

          for (; j < SampNum && ss <= 1; ss *= zoom) {
            int tt = imresize2(im, ss, im2);
            unsigned char* tmpimg = im2->tt.data;
            int h = im2->h, w = im2->w;
            int ranx = w - cx + 1;
            int rany = h - cy + 1;
            int stp = (w + 1);
            int siz = (h + 1) * stp;
            int x, y, k = 0;
            int stepx = stepxy, stepy = stepxy;
            double invarea = 1.f / (cx * cy);
            //fs->zoom = zoom;
            //fs->stepxy = stepxy;
            FILL(siz * 2, inter, 0);
            //IMRESIZE( im->h, im->w, im->data, im->s, 1, h, w, tmpimg, w, 1, INTER_BILINEARITY );
            //SKIP_HISTEQ(h, w, tmpimg, w, 1, tmpimg, w, 1);
            //SKIP_INTEGRAL_SQ( h, w, tmpimg, w, 1, inter, stp, 1 );
            SKIP_INTEGRAL(h, w, tmpimg, w, 1, inter0, stp, 1);
            SKIP_INTEGRAL_GQ(h, w, tmpimg, w, 1, inter1, stp, 1);

            //PRINT2D("a1.txt", "%8.f, ", 25, 25, inter, stp, HAAR_CN);
            //FPRINT2D("a0.txt", "%8.f, ", "\n", 25, 25, inter, stp, HAAR_CN);
            //FPRINT2D("a1.txt", "%8.f, ", "\n", 25, 25, inter+1, stp, HAAR_CN);
            FEAT_SETOFFSET(hh->ca->haar, hh->ca->weaklen, cx1, 1, 1.f, HAAR);

            //for (i=0; i<times; ++i)
            for (x = 0; j < SampNum && x < ranx; x += stepx) {
              for (y = 0; j < SampNum && y < rany; y += stepy) {
                double* tmpSamp0 = inter0 + y * stp + x;
                double* tmpSamp1 = inter1 + y * stp + x;
                unsigned char* tmpimg0 = tmpimg + y * w + x;
                double* Samp = fs->samp + (j + hh->pos_num) * _size;
                int ispass = 0;
                double score, var, invvar;
#if 0
                FILL(cx1 * cy1, inter0, 0.f);
                FILL(cx1 * cy1, inter1, 0.f);
                SKIP_HISTEQ(hh->h, hh->w, tmpimg0, w, 1, tmpimg0, w, 1);
                SKIP_INTEGRAL(hh->h, hh->w, tmpimg0, w, 1, inter0, hh->w + 1, 1);
                SKIP_INTEGRAL_GQ(hh->h, hh->w, tmpimg0, w, 1, inter1, hh->w + 1, 1);
                SKIP_INTEGRAL_VAR(inter0, cx1 * cy, cx, inter1, cx1 * cy, cx, invarea, var);
                tmpSamp0 = inter0, tmpSamp1 = inter1;
                SKIP_COPY(cy1, cx1, tmpSamp0, cx1, 1, Samp, _step, 1);
#else
                SKIP_INTEGRAL_VAR(tmpSamp0, stp * cy, cx, tmpSamp1, stp * cy, cx, invarea, var);

                if (var < 10.) {
                  continue;
                }

                FILL(cx1 * cy1, Samp, 0.f);
                //SKIP_COPY( cy1, cx1, tmpSamp0, stp, 1, Samp, _step, 1 );
                SKIP_INTEGRAL(fs->h, fs->w, tmpimg0, w, 1, Samp, _step, 1);
                //if (var<1.) var=1.;
#endif
                ++allsampnum;
                invvar = (var <= 0.) ? 1. : 1. / sqrt(var);
                //invvar=((int)(invvar*(1<<13)))*1./(1<<13);
#if 0

                if (sqrt(var) < 0.01) {
                  double int0[500] = {0};
                  double int1[500] = {0};
                  SKIP_INTEGRAL(hh->h, hh->w, tmpimg0, w, 1, int0, hh->w + 1, 1);
                  SKIP_INTEGRAL_GQ(hh->h, hh->w, tmpimg0, w, 1, int1, hh->w + 1, 1);
                  FPRINT2D("int0.txt", "%8.8f, ", "\r\n", hh->h + 1, hh->w + 1, int0, hh->w + 1, 1);
                  FPRINT2D("int1.txt", "%8.8f, ", "\r\n", hh->h + 1, hh->w + 1, int1, hh->w + 1, 1);
                  FPRINT2D("86433.txt", "%8d, ", "\r\n", cy, cx, tmpimg0, w, 1);
                  SKIP_INTEGRAL_VAR(int0, cx1 * cy, cx, int1, cx1 * cy, cx, invarea, var);
                  imwrite("86433.bmp", cy, cx, tmpimg0, w, 1);
                }

#endif

                if (invvar == (int)invvar) {
                  int asdf = 0;
                }

                //if (j + hh->pos_num==9031) { imwrite("86433.bmp", cy, cx, tmpimg0, w, 1); }
                //CASCADE_PASS(hh->ca, tmpSamp, hh->ca->haar, HAAR_FEAT_VAL, invvar, ispass, score);
                cascade_pass(hh->ca, Samp, _step, invvar, &ispass, &score);

                if (j < SampNum && ispass) {
                  int k;

                  //unsigned char* tmpimg0 =  tmpimg + y * w + x;
                  //SKIP_INTEGRAL( cy, cx, tmpimg0, w, 1, Samp, _step, 1 );
                  for (k = 0; k < _size; ++k) {
                    Samp[ k ] *= invvar;
                  }

                  //vvv[ j ] = invvar;
                  wi[ j ] = 1.f;
                  vi[ j ] = score;
                  ++j;
                  ++sampnum;
                }
              }
            }
          }

          imfree(im2);
        }

        FREE(inter);
#define STR111 "找了%5d张图片，共%9d个%s样本，找到%6d个，耗时%3d秒！%2d %3.3f"
        printf(STR111"\r", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time), stepxy, zoom);
      }
    }

    fclose(pf);
    logprintf(STR111"\n", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time), stepxy, zoom);
  }

  imfree(im);

  return n;
}

int HAAR_init(BOOST* hh)
{
  int mincx, mincy, rectxy;
  HAAR_FEAT_T* fs = MALLOC(HAAR_FEAT_T, 1);
  memset(fs, 0, sizeof(HAAR_FEAT_T));
  strcpy(fs->postxtfile, inigetstr("正样本txt文件"));
  strcpy(fs->negtxtfile, inigetstr("负样本txt文件"));
  fs->zoom = inigetfloat("缩放比");
  fs->w = hh->ca->w = inigetint("样本宽");
  fs->h = hh->ca->h = inigetint("样本高");
  mincx = inigetint("最小宽");
  mincy = inigetint("最小高");
  rectxy = inigetint("特征穷举步进");
  fs->stepxy = inigetint("样本检测步进");
  fs->samp_step = (hh->ca->h + 1) * (hh->ca->w + 1);

  hh->feat_num = get_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, NULL);
  fs->haar = MALLOC(HAARFEAT, hh->feat_num);
  get_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, fs->haar);

  hh->h_feat = fs;

  if (!fs->samp) {
    fs->samp_size = hh->samp_num * fs->samp_step;
    fs->samp = MALLOCSET(double, fs->samp_size, 0);
  }

  return 1;
}
