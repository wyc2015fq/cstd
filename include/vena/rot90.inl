
#include "img/imgopt.inl"
#include "iris/Imfilter.inl"
#include "iris/canny_fix.inl"

int bf_rot90(buf_t* bf, const img_t* im, int k, img_t* im2) {
  int rotflag[4] = {0, ISROT1, ISROT2, ISROT3};
  int tt = ((k%4)+4)%4;
  int flag = rotflag[tt];
  ASSERT(im!=im2);
  bf_im_trans(bf, im, im2, flag);
  return 0;
}

int imdouble(const img_t* im, img_t* im2) {
  int i, j;
  imsetsize(im2, im->h, im->w, sizeof(double), 1);
  for (i=0; i<im->h; ++i) {
    double* b = (double*)(im2->tt.data + i*im2->s);
    const uchar* a = (const uchar*)(im->tt.data + i*im->s);
    for (j=0; j<im->w; ++j) {
      b[j] = a[j];
    }
  }
  return 0;
}

int bf_imdouble(buf_t* bf, const img_t* im, img_t* im2) {
  int i, j;
  bf_imsetsize(bf, im2, im->h, im->w, sizeof(double), 1);
  for (i=0; i<im->h; ++i) {
    double* b = (double*)(im2->tt.data + i*im2->s);
    const uchar* a = (const uchar*)(im->tt.data + i*im->s);
    for (j=0; j<im->w; ++j) {
      b[j] = a[j];
    }
  }
  return 0;
}

int convn_f64(int ah, int aw, const double* a, int al, int kh, int kw, const double* ker, double* c, int cl) {
  int i, j, k, kn = kh*kw, ch=ah-kh+1, cw=aw-kw+1;
  int stoff[100];
  ASSERT(kn<100);
  for (i=0; i<kh; ++i) {
    for (j=0; j<kw; ++j) {
      stoff[i*kw+j] = (kh-1-i)*al+(kw-1-j);
    }
  }
  for (i=0; i<ch; ++i) {
    const double* a0 = a + i*al;
    double* c0 = c + i*cl;
    for (j=0; j<cw; ++j, ++a0) {
      double z = 0;
      for (k=0; k<kn; ++k) {
        z += a0[stoff[k]] * ker[k];
      }
      c0[j] += z;
    }
  }
  return 0;
}
int imboderex(const img_t* im, IRECT rc, img_t* im2) {
  imsetsize(im2, im->h+rc.t+rc.b, im->w+rc.l+rc.r, im->c, 1);
  COPY_REPLICATE_BORDER(im->h, im->w, im->tt.data, im->s, im2->h, im2->w, im2->tt.data, im2->s, im->c, rc.l, rc.t);
  return 0;
}
int bf_imboderex(buf_t* bf, const img_t* im, IRECT rc, img_t* im2) {
  bf_imsetsize(bf, im2, im->h+rc.t+rc.b, im->w+rc.l+rc.r, im->c, 1);
  COPY_REPLICATE_BORDER(im->h, im->w, im->tt.data, im->s, im2->h, im2->w, im2->tt.data, im2->s, im->c, rc.l, rc.t);
  return 0;
}
int imfilter2(const img_t* im, const double* ker, int kh, int kw, img_t* im2) {
  img_t im1[1] = {0};
  imsetsize(im2, im->h, im->w, sizeof(double), 1);
  imboderex(im, iRECT(kw/2, kh/2, kw-kw/2-1, kh-kh/2-1), im1);
  im = im1;
  convn_f64(im->h, im->w, im->tt.f8, im->w, kh, kw, ker, im2->tt.f8, im2->w);
  imfree(im1);
  return 0;
}
int bf_imfilter2(buf_t* bf, const img_t* im, const double* ker, int kh, int kw, img_t* im2) {
  img_t im1[1] = {0};
  bf_imsetsize(bf, im2, im->h, im->w, sizeof(double), 1);
  bf_imboderex(bf, im, iRECT(kw/2, kh/2, kw-kw/2-1, kh-kh/2-1), im1);
  im = im1;
  convn_f64(im->h, im->w, im->tt.f8, im->w, kh, kw, ker, im2->tt.f8, im2->w);
  bf_imfree(bf, im1);
  return 0;
}
int sobel_v(buf_t* bf, const img_t* im, img_t* im2) {
  int i, j;
  double scaling = 1.0,    sigma = 2,    vert = 1.0,    horz = 0.0;
  // generate the edge image
  canny_fix(bf, im, sigma, scaling, vert, horz, 1.9, 1.5, im2);    // edge detection 4-19
  //return 0;
  //imsetsamesize(im2, im);
  for (i=1; i<im->h-1; ++i) {
    //const uchar* a = im->tt.data + i*im->s;
    uchar* b = im2->tt.data + i*im2->s;
    for (j=1; j<im->w-1; ++j) {
      //int t1 = a[j+1], t2 = a[j-1];
      b[j] = b[j]>100 ? 255:0;
    }
  }
  return 0;
}
int im_minmax_u8(int h, int w, const uchar* A, int al, int* pmin, int* pmax) {
  int i, j, vmin=100000, vmax=0;
  for (i=0; i<h; ++i) {
    for (j=0; j<w; ++j) {
      int t = A[al * i + j];
      if (t<vmin) {
        vmin = t;
      } else if (t>vmax) {
        vmax = t;
      }
    }
  }
  *pmin = vmin;
  *pmax = vmax;
  return 0;
}
int imminmax_u8(const img_t* im, int* pmin, int* pmax) {
  im_minmax_u8(im->h, im->w, im->tt.data, im->s, pmin, pmax);
  return 0;
}

