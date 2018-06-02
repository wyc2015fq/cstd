
#ifndef _OMKERNEL_OK_INL_
#define _OMKERNEL_OK_INL_

#include "../img/filter_rect.inl"

static int bf_makeom_i1(buf_t* bf, const uchar* A, uchar* ft, double da, int db)
{
  //double buf1f[IRIS_W*10] = {0};
  //double ikf[100] = {0};
  enum {ftlen = IRIS_H * IRIS_S};

  if (ft && A) {
    uchar* A3 = ft;
    int nk, shift = 8;
    int* ik = 0;
    int* buf1 = 0;
    int* A2_i4 = 0;
    BFMALLOC(bf, buf1, IRIS_W);
    BFMALLOC(bf, ik, 100);
    BFMALLOC(bf, A2_i4, IRIS_H * IRIS_W);
    nk = make_guass_kernel_i(da, ik, shift);
    filter_rect_symmetric_i4(IRIS_H, IRIS_W, A, IRIS_W, 1, A2_i4, IRIS_W, 1, buf1, nk, ik, nk, ik, shift);
    //filter_binary_i4(IRIS_H, IRIS_W, A2_i4, IRIS_W, 1, A3, IRIS_W, 1, -db, 0, db, 0, 0);
    filter_binary_bit_i4(IRIS_H, IRIS_W, A2_i4, IRIS_W, 1, A3, IRIS_S, 1, -db, 0, db, 0, 0);
    //{img_t im[1]={0}; IMINIT(im, IRIS_H, IRIS_W/8, A3, IRIS_W/8, 1, 1), imshow(im); cvWaitKey(-1);}
    BFFREE(bf, A2_i4, IRIS_H * IRIS_W);
    BFFREE(bf, ik, 100);
    BFFREE(bf, buf1, IRIS_W);
  }

  return ftlen;
}

static const unsigned char poptable[256] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

static int count_xor(int n, const unsigned char* A, const unsigned char* B)
{
  int i = 0, k = 0;

  for (; i < n - 3; i += 4, A+=4, B+=4) {
    k +=
        poptable[A[0] ^ B[0]] +
        poptable[A[1] ^ B[1]] +
        poptable[A[2] ^ B[2]] +
        poptable[A[3] ^ B[3]];
  }

  for (; i < n; ++i, ++A, ++B) {
    k += poptable[A[0] ^ B[0]];
  }

  return k;
}

static int calc_sum(int n, const unsigned char* A) {
  int i = 0, k = 0;
  for (; i < n; ++i) {
    k += A[i];
  }

  return k;
}

#endif // _OMKERNEL_OK_INL_
