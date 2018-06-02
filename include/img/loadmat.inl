
#include "cstd.h"
#include "cfile.h"
#include "imgopt.inl"
#define getarg(T, p)  (p += sizeof(T), (T*)(p-sizeof(T)))
#define getargs(T, p, n)  (p += n*sizeof(T), (T*)(p-n*sizeof(T)))

int loadmat_buf(buf_t* bf, int i, int c, img_t* im)
{
  if ((i + 8) < bf->len) {
    char* p = (char*)bf->data + i;
    int h = *getarg(int, p);
    int w = *getarg(int, p);
    int f = 1;
    int n = h * w * c * f;

    if ((i + n + 8) <= bf->len) {
      imsetsize(im, h, w, c, f);
      memcpy(im->tt.data, p, n);
      return i + 8 + n;
    }
  }

  return 0;
}
int loadmat_fromfile(const char* file, int c, img_t* im)
{
  buf_t bf[1] = {0};
  buf_load(file, bf);
  loadmat_buf(bf, 0, c, im);
  bffrees(bf);
  return 0;
}
int imzscore(const img_t* x, img_t* x1, img_t* mu, img_t* sigma)
{
  img_t im1[1] = {0};

  if (x == x1 || x == mu || x == sigma) {
    imclone2(x, im1);
    x = im1;
  }

  immeanstd_f8(x, 0, OptRow, mu, sigma);
  immaxs_f8(sigma, DBL_EPSILON, sigma);
  imminus_f8(x, mu, x1);
  imdivide_f8(x1, sigma, x1);
#if 0
  img_t im2[1] = {0};
  imsubref(x1, iRECT2(0, 0, 100, 100), im2);
  imshowmat_f8(im2);
  cvWaitKey(-1);
#endif
  //sigma=max(std(x),eps);
  //x=bsxfun(@minus,x,mu);
  //x=bsxfun(@rdivide,x,sigma);
  imfree(im1);
  return 0;
}

int imnormalize(const img_t* x, const img_t* mu, const img_t* sigma, img_t* x1)
{
  img_t im1[1] = {0};

  if (x1 == x || x1 == mu || x1 == sigma) {
    x1 = im1;
  }

  imminus_f8(x, mu, x1);
  imdivide_f8(x1, sigma, x1);

  //x=bsxfun(@minus,x,mu);
  //x=bsxfun(@rdivide,x,sigma);
  if (x1 == im1) {
    img_t t;
    CC_SWAP(*x1, *im1, t);
  }

  imfree(im1);
  return 0;
}