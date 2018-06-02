
//#include "face/face.inl"

typedef struct Iris_t {
  char name[20];
  int id;
  unsigned char data[IRIS_H* IRIS_W];
  unsigned char mask[IRIS_H* IRIS_W];
  unsigned char Juanl[IRIS_H* IRIS_W];
  //#define binum (1105)
#define binum (2145)
  unsigned char code[binum];
  unsigned char newMask[binum];
} Iris_t;


typedef struct IrisDist_t {
  int ida, idb; // ±ØÐë ida<idb
  double dist;
} IrisDist_t;

#include "iris/bianma4.inl"
#include "HDR.inl"
#include "meansure1.inl"
//#include "OMcode1.inl"
#include "Hamm.inl"


int getclass(char* name)
{
  char classstr[20];
  memcpy(classstr, name + 1, 4);
  classstr[4] = 0;
  return atoi(classstr) * 10 + (name[5] == 'R');
}

#if 0
// ¶àbinµÄlbpÂË²¨ lbp_ex_ft_all
int lbp_ex_ft_3(int srch, int srcw, const unsigned char* src, int srcstep, double* dst, int dststep, int fillborder)
{
  int it = 0;
  int i, yh = srch + 1, yw = srcw + 1;
  unsigned char* img2;
  int ysize = yh * yw;
  int exx = ((2 * FN - 1) * 3);
  int yl = yw + exx;
  int aah = srch + exx, aaw = srcw + exx;
  int aal = aaw;
  int* y1, * y;
  int exx2 = (exx / 2);
  int umap[ 16 ];
  int iw = 0;
  int retsize = 0;
  //utime_start(_start_time);
  y1 = MALLOC(int, (yh + exx) * (yl));
  MEMSET(y1, 0, (yh + exx) * (yl));
  y = y1 + exx2 * yl + exx2;

  {
    //FPRINT2D(".\\src.txt", "%d\n", srch, srcw, src, srcstep, 1);
    if (fillborder) {
      INT2* AA;
      AA = MALLOC(INT2, (aah) * (aal));
      MEMSET(AA, 0, (aah) * (aal));
      COPY_REPLICATE_BORDER(srch, srcw, src, srcstep, aah, aaw, AA, aal, 1, exx2, exx2);
      //PRINT(aah, aaw, Asrc, aal, 1, ".\\Asrc.txt", "%d\n");
      SKIP_INTEGRAL(aah, aaw, AA, aal, 1, y1, yl, 1);
      FREE(AA);
    }
    else {
      int* y2 = y1 + yl * exx2 + exx2;
      SKIP_INTEGRAL(srch, srcw, src, srcstep, 1, y2, yl, 1);
      COPY_REPLICATE_BORDER(srch + 1, srcw + 1, y2, yl, (yh + exx), yl, y1, yl, 1, exx2, exx2);
    }
  }
  img2 = MALLOC(unsigned char, ysize);

  //Mati& me = *this;
  for (i = MINFN; i < FN; ++i) {
    //int ex = ( 2 * i + 1 ) * 3 + 1;
    int j, bin = lbp_Feature_List[ i ].bin;
    int len = lbp_Feature_List[ i ].len;
    unsigned char* rect = lbp_Feature_List[ i ].ptr;
    //int eex = ex*1;
    const unsigned char* __lbpmap0 = lbpmap[ i ];
    unsigned char lbpmap256[256];

    for (j = 0; j < 256; ++j) {
      lbpmap256[j] = j;
    }

    lbp_ex_init(i, yl, 1, umap);
    MEMSET(img2, 0, ysize);
#if 0

    SKIP_FILTER(srch, srcw, y, yl, 1, img2, yw, 1, LBP_EX_FT);
#else

    skip_filter_lbp_ex_ft(srch, srcw, y, yl, img2, yw, umap, lbpmap256);
#endif
    //cvShowImage("asdf", srch, srcw, img2, yw, 1); cvWaitKey(-1);
    SKIP_COPY(srch, srcw, img2, yw, 1, dst, dststep, 1);
    break;
  }

  SAFEFREE(img2);
  SAFEFREE(y1);
  //printf("%f\n", utime_elapsed(_start_time));
  return retsize;
}
#endif


double getmean(int n, double* a)
{
  int i;
  double s = 0;

  for (i = 0; i < n; ++i) {
    if (fabs(a[i]) > 1) {
      int asdf = 0;
    }

    s += a[i];
  }

  s /= n;
  return s;
}

double getvar(int n, double* a)
{
  int i;
  double s = 0;
  double s2 = 0;

  for (i = 0; i < n; ++i) {
    s += a[i];
    s2 += a[i] * a[i];
  }

  s /= n;
  s2 /= n;
  return s2 - s * s;
}

typedef int (*bianma_t)(Iris_t* ir);
