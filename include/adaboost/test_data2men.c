
//#define WIN32
// 可见光图片人脸和眼睛分类器
#include "./cas/vis_nesting_face0701.hpp"
#include "./cas/VisAllEye_nesting.hpp"
#include "./cas/NIR_face0615.hpp"

// 红外图片人脸和眼睛分类器
#include "./cas/NIRDoubleEYE.hpp"
#include "./cas/NIRAllEYE.hpp"
#include "./cas/NIREYELeftWithGlass.hpp"
#include "./cas/NIREYERightWithGlass.hpp"
#include "./cas/NIREYEWithoutGlass.hpp"
#include "../adaboost1/cascade.inl"

//#include "Integral.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if 1
#undef XROUND1
#define XROUND1 XROUND
#else
#define XROUND1(x) (XFLOOR(x))
#endif
static int caslen(const HAARCASCADE* ca)
{
  return sizeof(HAARCASCADE) + sizeof(fStage) * ca->stagelen + sizeof(fHaar) * ca->ihlen;
}
static int casleni(const HAARCASCADEI* ca)
{
  return sizeof(HAARCASCADE) + sizeof(iStage) * ca->stagelen + sizeof(iHaar) * ca->ihlen;
}
//#include "chalg.h"
HAARCASCADEI* convet_to_int(const HAARCASCADE* ca)
{
  int i, j;
  HAARCASCADEI* out = (HAARCASCADE*)malloc(sizeof(HAARCASCADEI) + sizeof(iStage) * ca->stagelen + sizeof(iHaar) * ca->ihlen);
  *out = *(HAARCASCADEI*)ca;
  out->stage = (iStage*)(((char*)out) + sizeof(HAARCASCADEI));
  out->ih = (iHaar*)(((char*)out) + sizeof(HAARCASCADEI) + sizeof(iStage) * ca->stagelen);

  if (CAISHAVEFLOAT(ca)) {
    ca->ih[ 0 ].ti = (unsigned char)(-1);
    {
      fStage* src = ca->stage;
      iStage* dst = out->stage;

      for (i = 0; i < ca->stagelen; ++i) {
        dst[ i ].len = src[ i ].len;
        dst[ i ].thd = XROUND1(SHIFT_TOINT(src[ i ].thd));
      }
    }
    {
      fHaar* src = ca->ih;
      iHaar* dst = out->ih;

      for (i = 0; i < ca->ihlen; ++i, ++src, ++dst) {
        dst->n = src->n;
        dst->ft0 = src->ft0;
        dst->ft1 = src->ft1;
        dst->ti = src->ti;
        dst->minval = XROUND1(SHIFT_TOINT(src->minval));
        dst->binval = XROUND(SHIFT_TOINT(src->binval));

        for (j = 0; j < LUT_BIN; ++j) {
          int t = XROUND1(SHIFT_TOINT(src->val[ j ]));

          if (t > SHRT_MAX) {
            printf("%d\n", t);
            t = SHRT_MAX;
          }

          if (t < SHRT_MIN) {
            printf("%d\n", t);
            t = SHRT_MIN;
          }

          dst->val[ j ] = t;
        }
      }
    }
  }

  return out;
}

int data2men(FILE* fpout, const void* ptr, int len, int addr, int isdat)
{
  int i;
  const char* HEXFOMAT = isdat ? "0x%02x%02x%02x%02x\r\n" : "0x%02x%02x%02x%02x,\r\n";

  if (isdat) {
    fprintf(fpout, "1651 1 %x 0 %x\r\n", addr, (len + 3) / 4);
  }

  for (i = 0; i < len - 3; i += 4) {
    unsigned char* r_data = (unsigned char*) ptr + i;
    fprintf(fpout, HEXFOMAT, r_data[ 3 ], r_data[ 2 ], r_data[ 1 ], r_data[ 0 ]);
  }

  if (i < len) {
    unsigned char buf[ 4 ] = { 0 };

    for (; i < len; ++i) {
      buf[ i ] = *((unsigned char*) ptr + i);
    }

    fprintf(fpout, HEXFOMAT, buf[ 3 ], buf[ 2 ], buf[ 1 ], buf[ 0 ]);
  }

  return 0;
}

HAARCASCADE* casdup(const HAARCASCADE* ca)
{
  unsigned char* mem = 0;
  HAARCASCADE* ca2;
  int len = caslen(ca);
  mem = malloc(len);
  assert(mem);
  ca2 = mem;
  memcpy(mem, ca, sizeof(HAARCASCADE));
  ca2->stage = (fStage*)(mem + sizeof(HAARCASCADE));
  ca2->ih = (fHaar*)(mem + sizeof(HAARCASCADE) + sizeof(fStage) * ca->stagelen);
  memcpy(ca2->stage, ca->stage, sizeof(fStage) * ca->stagelen);
  memcpy(ca2->ih, ca->ih, sizeof(fHaar) * ca->ihlen);
  return mem;
}

int cascade2mem(const HAARCASCADE* ca, const char* filename, int addr, int is_int)
{
  HAARCASCADE* ca2 = 0;
  int len = 0;
  int isdat = (NULL != strstr(filename, ".dat"));

  if (is_int) {
    ca2 = (HAARCASCADE*)convet_to_int(ca);
    len = casleni((HAARCASCADEI*)ca2);
  }
  else {
    ca2 = casdup(ca);
    len = caslen(ca2);
  }

  {
    unsigned char* mem = (unsigned char*)ca2;
    FILE* fout = fopen(filename, "wb");

    if (fout) {
      data2men(fout, mem, len, addr, isdat);
      fclose(fout);
    }
  }

  free(ca2);
  return 0;
}

unsigned char img_lena[ 160 * 120 ] = {
#include "lena.txt"
};

#define DATA2MEN2(_FNAME, _STRUCT, _ADDR) do { \
    FILE *  fout = fopen(_FNAME, "wb"); \
    if (fout) { \
      data2men(fout, _STRUCT, sizeof(_STRUCT), _ADDR); \
      fclose(fout); \
    } \
  } while (0)

unsigned char img[ 640 * 480 ];
int pos[] = { 105, 147, 210, 210 };
int test_dat2img(void)
{
  FILE* pf;
  int i, addr, len;
  char buf[ 256 ];
  pf = fopen("word_store(1).dat", "rb");
  fscanf(pf, "%s %d %x 0 %x\r\n", buf, &i, &addr, &len);
  assert(len == (640 * 480));

  for (i = 0; i < len; i += 4) {
#if 0
    unsigned char* r_data = (unsigned char*) img + i;

    //fscanf(pf, HEXFOMAT, r_data+3, r_data+2, r_data+1, r_data+0);
    fscanf(pf, HEXFOMAT, &r_data[ 0 ], &r_data[ 1 ], &r_data[ 2 ], &r_data[ 3 ]);
#else

    size_t* r_data = (size_t*)(img + i);

    //fscanf(pf, HEXFOMAT, r_data+3, r_data+2, r_data+1, r_data+0);
    fscanf(pf, "%x", r_data);
#endif

  }

  //imwrite( "a.bmp", 480, 640, img, 640, 1 );
  fclose(pf);
  return 0;
}

int test_mem2dat(void)
{
  {
    FILE* fout = fopen("vis_nesting_face0701_iHaar_mem.dat", "wb");

    if (fout) {
      data2men(fout, vis_nesting_face0701_iHaar, sizeof(vis_nesting_face0701_iHaar), BASE_ADDR, 1);
      fclose(fout);
    }
  }
  {
    FILE* fout = fopen("img_lena.dat", "wb");

    if (fout) {
      data2men(fout, img_lena, sizeof(img_lena), DDR_BASE_ADDR, 1);
      fclose(fout);
    }
  }

  return 0;
}


int haarcascade_txt2inl(const char* fname, const char* fnameout, int addr, int is_int)
{
  CASCADE* ca = cascade_read_txt(fname);
  HAARCASCADE* hca = cascade2haarcascade(ca);
  cascade2mem(hca, fnameout, addr, is_int);
  del_cascade(ca);
  free(hca);
  return 0;
}


int main(void)
{
  void* fff = &vis_nesting_face0701_cascade;
  int aaa = CASCADESIZE(fff);
#define DO_CASCADE2MEM(_N) \
  cascade2mem( &_N##_cascade, "./cas/"#_N"_f.inl", BASE_ADDR, 0 ), \
  cascade2mem( &_N##_cascade, "./cas/"#_N".inl", BASE_ADDR, 1 )

  DO_CASCADE2MEM(vis_nesting_face0701);
#if 1
  DO_CASCADE2MEM(VisAllEye_nesting);
  DO_CASCADE2MEM(NIR_face0615);
  DO_CASCADE2MEM(NIRDoubleEYE);
  DO_CASCADE2MEM(NIRAllEYE);
  DO_CASCADE2MEM(NIREYELeftWithGlass);
  DO_CASCADE2MEM(NIREYERightWithGlass);
  DO_CASCADE2MEM(NIREYEWithoutGlass);
#endif
#undef DO_CASCADE2MEM
#define DO_CASCADE2MEM(_N) \
  haarcascade_txt2inl( "./cas/"#_N".txt", "./cas/"#_N"_f.inl", BASE_ADDR, 0 ), \
  haarcascade_txt2inl( "./cas/"#_N".txt", "./cas/"#_N".inl", BASE_ADDR, 1 )

  DO_CASCADE2MEM(vis_nesting_face20110713);
  DO_CASCADE2MEM(NIR_RightEyeWithGlass20110714);
  DO_CASCADE2MEM(NIR_LeftEyeWithGlass20110717);
#undef DO_CASCADE2MEM
  //test_dat2img();
  return 0;
}

