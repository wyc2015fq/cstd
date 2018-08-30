

// #include "cv1/filter.inl"
//#include "face/pplight_16s.inl"

int img_mean_var(int h, int w, const uchar* src, int step, int cn, const uchar* mask, int maskstep, double* m1, double* v1) {
  int i, j, k, n=0;
  for (k=0; k<cn; ++k) {
    m1[k] = v1[k] = 0;
  }
  for (i=0; i<h; ++i) {
    const uchar* src1 = src + i*step;
    const uchar* mask1 = mask + i*maskstep;
    for(j=0; j<w; ++j) {
      if (mask1[j]>20) {
        const uchar* src2 = src1 + j*cn;
        for (k=0; k<cn; ++k) {
          double t = src2[k];
          m1[k] += t;
        }
        ++n;
      }
    }
  }
  for (k=0; k<cn; ++k) {
    m1[k]/=n;
  }
  if (v1) {
    for (i=0; i<h; ++i) {
      const uchar* src1 = src + i*step;
      const uchar* mask1 = mask + i*maskstep;
      for(j=0; j<w; ++j) {
        if (mask1[j]>20) {
          const uchar* src2 = src1 + j*cn;
          for (k=0; k<cn; ++k) {
            double t = src2[k] - m1[k];
            v1[k] += t*t;
          }
        }
      }
    }
    for (k=0; k<cn; ++k) {
      v1[k]/=n;
    }
  }
  return 0;
}
double pdf_u8(int n, const uchar* x, const double* m1, const double* v1) {
  int i, cc=0;
  double y;
  for (i=0; i<n; ++i) {
    double c = x[i];
    c = c - m1[i];
    c = c * c/v1[i];
    cc += c;
  }
  y = exp(-0.5*cc);
  return y;
}

//#include "cv1/smooth.inl"

int img_pdf_mask(int h, int w, const uchar* src, int step, int cn, uchar* mask, int maskstep, const double* m1, const double* v1) {
  //float* fmask = MALLOC(float, h*w);
  int i, j, k;
  for (i=0; i<h; ++i) {
    const uchar* src1 = src + i*step;
    uchar* mask1 = mask + i*maskstep;
    //float* fmask1 = fmask + i*w;
    for(j=0; j<w; ++j) {
      double t;
      t = pdf_u8(cn, src1, m1, v1)*mask1[j];
      mask1[j] = BOUND(t, 0, 255);
      src1 += cn;
    }
  }
  //FREE(fmask);
  return 0;
}
int img_mask_blender(int h, int w, uchar* src, int step, int cn, const uchar* mask, int maskstep, COLOR clr) {
  uchar* pclr = (uchar*)&clr;
  int i, j, k;
  for (i=0; i<h; ++i) {
    uchar* src1 = src + i*step;
    const uchar* mask1 = mask + i*maskstep;
    for(j=0; j<w; ++j) {
      uchar a = mask1[j];
      for (k=0; k<cn; ++k) {
        src1[k] += (a * (pclr[k]-src1[k]))>>8;
      }
      src1 += cn;
    }
  }
  return 0;
}