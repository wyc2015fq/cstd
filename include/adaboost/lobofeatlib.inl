
#include "img/imgio.inl"
#include "img/imgopt.inl"

typedef float base_feat_type;
// h_feat
typedef struct LOBO_FEAT_T {
  char postxtfile[256];
  char negtxtfile[256];
  base_feat_type* samp; // featnum * sampnum 的矩阵
  int* allfeat;        // 所有的特征数据
  double zoom;
  int stepxy;
  int ipic;
}
LOBO_FEAT_T;

typedef int LOBOFEAT;

#define LOBO_FEAT_ELEMSIZE  sizeof(int)

#define LOBO_FEAT_VAL(A1, pw, IV) (*((A1) + (pw)))

STATIC double LOBO_feat_val(double* A, int al, int* chi, int i, double IV)
{
  return LOBO_FEAT_VAL(A, chi[ i ], IV);
}

#define LOBO_CN 0
#define LOBO_setoffset 0
#define LOBO_integral 0

STATIC int LOBO_print(FILE* pf, const int* chi)
{
  fprintf(pf, " %6d ", chi[ 0 ]);
  return 0;
}
STATIC int LOBO_scanf(FILE* pf, int* chi)
{
  fscanf(pf, "%d", chi);
  return 0;
}

STATIC int LOBO_uninit(BOOST* hh)
{
  LOBO_FEAT_T* fs = (LOBO_FEAT_T*)(hh->h_feat);
  free(fs->allfeat);
  free(fs->samp);
  free(hh->h_feat);
  hh->h_feat = 0;
  return 0;
}

STATIC int LOBO_samp_data(BOOST* hh, char** buf, int* size)
{
  LOBO_FEAT_T* fs = (LOBO_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->allfeat);
  *size = hh->feat_num * LOBO_FEAT_ELEMSIZE;
  return 0;
}

STATIC int LOBO_save_feat(BOOST* hh, int id)
{
  LOBO_FEAT_T* fs = (LOBO_FEAT_T*)(hh->h_feat);
  int es = LOBO_FEAT_ELEMSIZE;
  char* dst = (char*)(hh->ca->featptr) + hh->ca->weaklen * es;

  if (id >= 0) {
    char* src = (char*)(fs->allfeat) + id * es;
    memcpy(dst, src, es);
  }
  else {
    memset(dst, 0, es);
  }

  hh->ca->weaklen++;
  return 0;
}
STATIC double* LOBO_feat(BOOST* hh, int i)
{
  LOBO_FEAT_T* fs = (LOBO_FEAT_T*)(hh->h_feat);
  const base_feat_type* samp = (fs->samp) + hh->samp_num * i;
  COPY(hh->samp_num, samp, hh->fi);
  return hh->fi;
}

#define SAMP_H 80
#define SAMP_W 80

#include "luobo/luobo.inl"
//#include "src/adaboost/luobo/luobo.h"

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
STATIC int LOBO_find_samp(BOOST* hh, int type)
{
  LOBO_FEAT_T* fs = (LOBO_FEAT_T*)(hh->h_feat);
  // 由特征计算样本
  int i, j, ispass;
  int offset = (type > 0 ? 0 : GetPosNum(hh));
  int Num = (type > 0) ? GetPosNum(hh) : hh->samp_num;
  double* wi = hh->wi;
  double* vi = hh->vi;
  int id = type;
  double score;
  vstr_t sv[1] = {0};
  const char* piclist = type > 0 ? fs->postxtfile : fs->negtxtfile;
  float* feat = NULL;
  double* featd = NULL;
  img_t im[1] = {0};
  img_t im1[1] = {0};
  img_t im2[1] = {0};
  float* Samp = 0;
  int picnum = 0, allsampnum = 0, sampnum = 0;
  utime_start(time);
  vstr_load(piclist, sv);
  featd = MALLOC(double, 4000);
  feat = MALLOC(float, 4000);

  j = offset;
  i = fs->ipic;

#define STR111 "找了%5d张图片，共%9d个%s样本，找到%6d个，耗时%3d秒！"

  for (; j < Num && i < sv->n; ++i) {
    imread((char*)sv->v[i].s, 3, 1, im);

    if (im->h < SAMP_H || im->w < SAMP_W) {
      continue;
    }

    ++picnum;

    if (type > 0) {
      imresize_1(im, SAMP_H, SAMP_W, im2);
      memset(feat, 0, sizeof(*feat) * 4000);
      getfeature_10(im2->tt.data, feat);
      COPY(4000, feat, featd);
      cascade_pass(hh->ca, featd, 1, 0, &ispass, &score);

      if (ispass) {
        for (; j < Num && wi[j] > 0.; ++j);

        if (j >= Num) {
          break;
        }

        Samp  = fs->samp + j;
        COPY1D(4000, feat, 1, Samp, hh->samp_num);
        wi[ j ] = 1.f;
        vi[ j ] = score;
        ++j;
        ++sampnum;
      }

      ++allsampnum;
      printf(STR111"\r", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time));
    }
    else {
      double tt = 3;
      double zo = MAX(SAMP_H * 1. / im->h, SAMP_W * 1. / im->w);
      double zomax = MAX(SAMP_H * tt / im->h, SAMP_W * tt / im->w);
      int x, y, st = 10;
      uchar img[SAMP_H * SAMP_W * 3];
      int cnt = sampnum;
      zomax = MIN(1, zomax);

      for (; j < Num && zo <= zomax; zo *= 1.1) {
        imresize1(im, zo, im2);
        ASSERT(3 == im2->c);

        for (y = 0; j < Num && y <= im2->h - SAMP_H; y += st) {
          for (x = 0; x <= im2->w - SAMP_W; x += st) {
            memcpy2d(img, SAMP_W * 3, im2->tt.data + y * im2->s + x * im2->c, im2->s, SAMP_H, SAMP_W * 3);
            //cvShowImage("adsfaf", 80, 80, img, 240, 3); cvWaitKey(-1);
            memset(feat, 0, sizeof(*feat) * 4000);
            getfeature_10(img, feat);
            COPY(4000, feat, featd);
            cascade_pass(hh->ca, featd, 1, 0, &ispass, &score);

            if (ispass) {
              for (; j < Num && wi[j] > 0.; ++j);

              if (j >= Num) {
                break;
              }

              Samp  = fs->samp + j;
              COPY1D(4000, feat, 1, Samp, hh->samp_num);
              wi[ j ] = 1.f;
              vi[ j ] = score;
              ++j;
              ++sampnum;
            }

            ++allsampnum;
            printf(STR111"\r", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time));

          }
        }

        if (sampnum - cnt > 30) {
          break;
        }
      }
    }
  }

  if (type < 0) {
    fs->ipic = i % sv->n;
    //cvShowMat("samp", "%I32f", hh->feat_num, hh->samp_num, fs->samp, hh->samp_num*sizeof(float), sizeof(float), -1); cvWaitKey(-1);
  }

  logprintf(STR111" %d \n", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time), fs->ipic);
  strv_free(sv);
  imfree(im);
  imfree(im1);
  imfree(im2);
  FREE(feat);
  FREE(featd);
  return 0;
}

STATIC int LOBO_init(BOOST* hh)
{
  int i, samp_size;
  base_feat_type* samp;
  LOBO_FEAT_T* fs = MALLOC(LOBO_FEAT_T, 1);
  memset(fs, 0, sizeof(LOBO_FEAT_T));
  strcpy(fs->postxtfile, inigetstr("正样本txt文件"));
  strcpy(fs->negtxtfile, inigetstr("负样本txt文件"));
  fs->zoom = inigetfloat("缩放比");
  fs->stepxy = inigetint("样本检测步进");
  hh->feat_num = getfeature_10(0, 0);
  samp_size = hh->feat_num * hh->samp_num;
  fs->samp = samp = MALLOC(base_feat_type, samp_size);
  hh->h_feat = fs;
  fs->allfeat = MALLOC(int, hh->feat_num);
  fs->ipic = 0;

  for (i = 0; i < hh->feat_num; ++i) {
    fs->allfeat[ i ] = i;
  }

  return 1;
}

int lobo_copyfeat(const float* feat, int rows, int sz, double* featd) {
  int i, j, k;
  for (k=0; k<10; ++k) {
    for (i=0; i<20; ++i) {
      for (j=0; j<20; ++j) {//列
        featd[k*400 + i*20 + j] = feat[k*sz + i*rows + j];
      }
    }
  }
  return 0;
}

int lobo_detect(cascade_t* ca, img_t* im, XRECT* out, int maxoutlen)
{
  double tt = 3;
  double zo = MAX(SAMP_H * 1. / im->h, SAMP_W * 1. / im->w);
  double zomax = MAX(SAMP_H * tt / im->h, SAMP_W * tt / im->w);
  int j = 0, allsampnum = 0, x, y, st = 1, ispass = 0;
  img_t im2[1] = {0};
  double* featd = MALLOC(double, 4000);
  float* feat;
  double score;
  double zz = SAMP_H * 3. / im->h;
  feat = MALLOC(float, 10*(im->h/4)*(im->w/4));
  zomax = MIN(zz, zomax);
  zomax = MIN(1, zomax);
  ASSERT(3 == im->c);

  //zo = 1., zomax = 10;
  for (; j < maxoutlen && zo <= zomax; zo *= 1.1) {
    img_t im3[1] = {0};
    int rows, cols;
    imresize1(im, zo, im2);
    //imshow(im2);cvWaitKey(-1);
    ASSERT(3 == im2->c);
    rows = im2->h/4;
    cols = im2->w/4;
    imgetfeature_10(im2, feat);
    
    for (x = 0; j < maxoutlen && x <= im2->w/4 - 20; x += st) {
      for (y = 0; j < maxoutlen && y <= im2->h/4 - 20; y += st) {
        //COPY(4000, feat, featd);
        lobo_copyfeat(feat + x*rows+y, rows, rows*cols, featd);
        cascade_pass(ca, featd, 1, 0, &ispass, &score);

        if (ispass) {
          out[j].x = (int)(x / zo)*4;
          out[j].y = (int)(y / zo)*4;
          out[j].w = (int)(SAMP_W / zo);
          out[j].h = (int)(SAMP_H / zo);
          out[j].score = (int)(score * (1 << 16));
          ++j;
          //printf("%d %d\n", x, y);
        }

        ++allsampnum;
      }
    }
  }

  FREE(featd);
  FREE(feat);
  imfree(im2);
  return j;
}

#undef STR111
