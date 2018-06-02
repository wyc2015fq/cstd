
// 广义矩阵
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif // ASSERT
typedef struct matvtable matvtable;
typedef struct matbase {
  matvtable* vtable;
  void* userdata;
  unsigned char* rowbuf;
  int rows, cols, step, off, rowpos, elemsize;
} matbase;
#define mat_setrow(m, i)  ((matbase*)(m))->vtable->f_setrow((matbase*)(m), i)
#define mat_loadrow(m)     ((matbase*)(m))->vtable->f_loadrow((matbase*)(m))
#define mat_saverow(m)     ((matbase*)(m))->vtable->f_saverow((matbase*)(m))
#define mat_delete(m)     (((matbase*)(m))->vtable->f_delete((matbase*)(m)), m=0)
typedef struct matvtable {
  void* (*f_setrow)(matbase* mf, int i);
  void* (*f_loadrow)(matbase* mf);
  void* (*f_saverow)(matbase* mf);
  int (*f_delete)(matbase* mf);
} matvtable;

#include <stdio.h>
#define cast(T, _p)  (T)(_p)
#define NEW(T, _n)    ((T*)malloc(sizeof(T)*(_n)))
#define ZERO(_p, _n)  memset(_p, 0, sizeof(*(_p))*(_n))
#define HEADSIZE  sizeof(matbase)
#define MAGICSIZE (int)(&(((matbase*)0)->rows))
#define MAGICSTR "matfile v1"
static void* matbase_setrow(matbase* mf, int i)
{
  int  result;

  if (i != mf->rowpos) {
    result = fseek(cast(FILE*, mf->userdata), HEADSIZE + mf->step * i + mf->elemsize * mf->off, SEEK_SET);

    if (result) {
      perror("Fseek failed");
      return 0;
    }
    else {
      mf->rowpos = i;
    }
  }

  return mf->rowbuf;
}
static void* matbase_loadrow(matbase* mf)
{
  int bytes;
  ASSERT(mf->rowpos >= 0 && mf->rowpos < mf->rows);
  bytes = fread(mf->rowbuf + mf->off * mf->elemsize, mf->elemsize, mf->cols, cast(FILE*, mf->userdata));
  return mf->rowbuf;
}
static void* matbase_saverow(matbase* mf)
{
  int bytes;
  ASSERT(mf->rowpos >= 0 && mf->rowpos < mf->rows);
  bytes = fwrite(mf->rowbuf + mf->off * mf->elemsize, mf->elemsize, mf->cols, cast(FILE*, mf->userdata));
  return mf->rowbuf;
}
int delete_matbase(matbase* mf)
{
  if (cast(FILE*, mf->userdata)) {
    fclose(cast(FILE*, mf->userdata));
  }

  free(mf->rowbuf);
  free(mf);
  return 0;
}
static int saveheader(matbase* mf, FILE* pFile)
{
  matbase mm = *mf;
  char magicstr[MAGICSIZE] = MAGICSTR;
  memset(&mm, 0, MAGICSIZE);
  memcpy(&mm, magicstr, MAGICSIZE);
  //fseek( cast(FILE*, mf->userdata), 0, SEEK_SET);
  fwrite(&mm, sizeof(matbase), 1, cast(FILE*, pFile));
  return 0;
}
#if 0
#include "../codec/huff.h"
#include "..\..\fmtimg\lib\libzlib.inl"
int compress_matfile(matbase* mf, const char* fname)
{
  FILE* pFile = fopen(fname, "wb");

  if (NULL != pFile) {
    int i, len = 0;
    int* rowseek = NEW(int, mf->rows);
    char* pDes0 = NEW(char, mf->step * 2);
    ZERO(rowseek, mf->rows);
    ZERO(pDes0, mf->step * 2);
    saveheader(mf, pFile);
    fseek(pFile, HEADSIZE + sizeof(int)*mf->rows, SEEK_SET);

    for (i = 0; i < mf->rows; ++i) {
      char* pSrc0 = (char*)mat_setrow(mf, i);
      int l;
      mat_loadrow(mf);
      //l = HUFF_Encode( pSrc0, mf->step, pDes0, mf->step*2 );
      {
        int res;
        int compressionLevel = 2;
        res = compress2(pDes0, &l, pSrc0, mf->step, compressionLevel);
      }
      len += l;
      rowseek[i] = len;
      fwrite(pDes0, l, 1, pFile);
    }

    fseek(pFile, HEADSIZE, SEEK_SET);
    fwrite(rowseek, sizeof(int), mf->rows, pFile);
    fclose(pFile);
    free(rowseek);
    free(pDes0);
  }

  return 0;
}

int compress_matfile2(matbase* mf, const char* fname)
{
  FILE* pFile = fopen(fname, "wb");

  if (NULL != pFile) {
    int i, len;
    char* pSrc0 = NEW(char, sizeof(matbase) + mf->step * mf->rows);
    char* pDes0 = NEW(char, sizeof(matbase) + mf->step * mf->rows * 2);
    ZERO(pDes0, mf->step * 2);
    memcpy(pDes0, mf, sizeof(matbase));

    for (i = 0; i < mf->rows; ++i) {
      char* row = (char*)mat_setrow(mf, i);
      mat_loadrow(mf);
      memcpy(pSrc0 + i * mf->step + sizeof(matbase), row, mf->step);
    }

    len = HUFF_Encode(pSrc0, sizeof(matbase) + mf->step * mf->rows, pDes0, sizeof(matbase) + mf->step * mf->rows * 2);
    {
      int res;
      int compressionLevel = Z_BEST_COMPRESSION;
      res = compress2(pDes0, &len, pSrc0, sizeof(matbase) + mf->step * mf->rows, compressionLevel);
    }
    //compress(pDes0, &len, pSrc0, sizeof(matbase)+mf->step*mf->rows);
    fwrite(pDes0, len, 1, pFile);
    fclose(pFile);
    free(pDes0);
    free(pSrc0);
  }

  return 0;
}
#endif
static int loadheader(matbase* mf, FILE* pFile)
{
  matbase mm = *mf;
  char magicstr[MAGICSIZE] = MAGICSTR;
  fread(&mm, sizeof(matbase), 1, pFile);

  //fseek( cast(FILE*, mf->userdata), 0, SEEK_SET);
  if (0 == strcmp((char*)&mm, MAGICSTR)) {
    *mf = mm;
    return 1;
  }

  printf("不是%s文件", MAGICSTR);
  return 0;
}
matbase* new_matfile(int rows, int cols, int elemsize)
{
  static matvtable _matvtable = {matbase_setrow, matbase_loadrow, matbase_saverow, delete_matbase};
  matbase* mf = NEW(matbase, 1);

  if (NULL != mf) {
    ZERO(mf, 1);
    mf->rows = rows, mf->cols = cols, mf->off = 0;
    mf->step = cols * elemsize, mf->elemsize = elemsize;
    mf->rowpos = -1;
    mf->vtable = &_matvtable;
    mf->rowbuf = NEW(char, cols * elemsize);
  }

  return mf;
}
#define open_matfile(fname) create_matfile(0, 0, 0, fname, "rb")
matbase* create_matfile(int rows, int cols, int elemsize, const char* fname, const char* mode)
{
  matbase* mf = NULL;

  if (rows <= 0 || cols <= 0 || elemsize <= 0) {
    // open
    FILE* pFile = fopen(fname, "rb");

    if (NULL != pFile) {
      matbase mm;

      if (loadheader(&mm, pFile)) {
        mf = new_matfile(mm.rows, mm.cols, mm.elemsize);
        cast(FILE*, mf->userdata) = pFile;
      }
      else {
        fclose(pFile);
      }
    }
  }
  else {
    FILE* pFile = fopen(fname, mode);

    if (NULL != pFile) {
      mf = new_matfile(rows, cols, elemsize);
      cast(FILE*, mf->userdata) = pFile;
      saveheader(mf, pFile);
    }
  }

  return (matbase*)mf;
}
#include <math.h>
static void* matrand_setrow(matbase* mf, int i)
{
  mf->rowpos = i;
  return mf->rowbuf;
}
static void* matrand_loadrow(matbase* mf)
{
  int i;
  ASSERT(mf->rowpos >= 0 && mf->rowpos < mf->rows);

  for (i = 0; i < mf->cols; ++i) {
    mf->rowbuf[i] = rand() & 0xff;
  }

  return mf->rowbuf;
}
static void* matrand_saverow(matbase* mf)
{
  ASSERT(0);
  return mf->rowbuf;
}
#define delete_matrand delete_matbase
matbase* create_matrand(int rows, int cols)
{
  static matvtable _matvtable = {matrand_setrow, matrand_loadrow, matrand_saverow, delete_matrand};
  matbase* mf = NULL;
  mf = NEW(matbase, 1);
  ZERO(mf, 1);
  mf->rows = rows, mf->cols = cols;
  cast(FILE*, mf->userdata) = 0;
  mf->rowpos = -1;
  mf->vtable = &_matvtable;
  mf->rowbuf = NEW(char, cols);
  srand(10);
  return (matbase*)mf;
}

// 矩阵转置
int mat_transport(matbase* src, matbase* dst)
{
  int i, j = 0, k, off = 0, elemsize = dst->elemsize, n = 1024;
  char* buf = (char*)malloc(n * src->step);
  ASSERT(src->rows == dst->cols && src->cols == dst->rows);
  ASSERT(src->elemsize == dst->elemsize && src->elemsize > 0);

  for (i = 0; i < src->rows;) {
    char* srcrow;
    srcrow = mat_setrow(src, i);
    mat_loadrow(src);
    memcpy(buf + (i - off)*src->step, srcrow, src->step);
    ++i;

    if (i >= MIN((off + n), src->rows)) {
      dst->cols = MIN(n, (dst->step / elemsize - off));
      dst->off = off;

      if (sizeof(char) == elemsize) {
        for (j = 0; j < dst->rows; ++j) {
          char* srcbuf = (char*)buf;
          char* dstrow = (char*)((char*)mat_setrow(dst, j) + off);

          for (k = 0; k < (int)(dst->cols); ++k) {
            dstrow[k] = srcbuf[k * src->step / elemsize + j];
            //printf("%d\n", dstrow[k]);
          }

          mat_saverow(dst);
        }

#if 1
      }
      else if (sizeof(short) == elemsize) {
        for (j = 0; j < dst->rows; ++j) {
          short* srcbuf = (short*)buf;
          short* dstrow = (short*)((char*)mat_setrow(dst, j) + off);

          for (k = 0; k < (int)(dst->cols / elemsize); ++k) {
            dstrow[k] = srcbuf[k * src->step / elemsize + j];
            //printf("%d\n", dstrow[k]);
          }

          mat_saverow(dst);
        }
      }
      else if (sizeof(int) == elemsize) {
        for (j = 0; j < dst->rows; ++j) {
          int* srcbuf = (int*)buf;
          int* dstrow = (int*)((char*)mat_setrow(dst, j) + off);

          for (k = 0; k < (int)(dst->cols / elemsize); ++k) {
            dstrow[k] = srcbuf[k * src->step / elemsize + j];
            //printf("%d\n", dstrow[k]);
          }

          mat_saverow(dst);
        }
      }
      else if (sizeof(double) == elemsize) {
        for (j = 0; j < dst->rows; ++j) {
          double* srcbuf = (double*)buf;
          double* dstrow = (double*)((char*)mat_setrow(dst, j) + off);

          for (k = 0; k < (int)(dst->cols / elemsize); ++k) {
            dstrow[k] = srcbuf[k * src->step / elemsize + j];
            //printf("%d\n", dstrow[k]);
          }

          mat_saverow(dst);
        }

#endif
      }
      else {
        for (j = 0; j < dst->rows; ++j) {
          char* srcbuf = (char*)buf;
          char* dstrow = (char*)((char*)mat_setrow(dst, j) + off);

          for (k = 0; k < (int)(dst->cols); ++k) {
            memcpy(dstrow + k * elemsize, srcbuf + k * src->step + j * elemsize, elemsize);
            printf("%d\n", dstrow[k]);
          }

          mat_saverow(dst);
        }
      }

      off += dst->cols;
    }
  }

  return 0;
}
