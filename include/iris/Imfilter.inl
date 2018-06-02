
//K=krow*kcol'
#if 0
int dekernel(int kh, int kw, const double* K, int kl, double* krow, double kcol)
{
  int i;
  krow[0] = 1;
  kcol[0] = K[0];

  for (i = 0; i < kh; ++i) {
    double d =
  }

      return 0;
}
#endif

int Imfilter(int h, int w, const unsigned char* A, int al,
    int kh, int kw, const double* K, unsigned char* B, int bl)
{
  int i, j, k, kn, kh2 = kh / 2, kw2 = kw / 2;
  int pos[256];
  //int ker[256];
  unsigned char* AA;
  AA = (unsigned char*)malloc((h + kh - 1) * (w + kw - 1));
  COPY_REPLICATE_BORDER(h, w, A, al, (h + kh - 1), (w + kw - 1), AA, (w + kw - 1), 1, kh2, kw2);
  al = (w + kw - 1), A = AA + kh2 * (w + kw - 1) + kw2;
  //double krow[10];
  //double kcol[10];
  //dekernel(kh, kw, K, kw, krow, kcol);
  kn = 0;

  for (i = 0; i < kh; ++i) {
    for (j = 0; j < kw; ++j) {
      if (K[i * kw + j] != 0) {
        //K[kn] = K[i*kw+j];
        //ker[kn] = (int)(K[i * kw + j] * (1 << 13) + 0.5);
        pos[kn] = (i - kh2) * al + (j - kw2);
        kn++;
      }
    }
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      const unsigned char* AA = A + i * al + j;
#if 1
      double b = 0;

      for (k = 0; k < kn; ++k) {
        b += AA[pos[k]] * K[k];
      }

#else
      int b = 0;

      for (k = 0; k < kn; ++k) {
        b += AA[pos[k]] * ker[k];
      }

      b >>= 13;
#endif
      b = MIN(b, 255);
      b = MAX(b, 0);
      B[i * bl + j] = (unsigned char)b;
    }
  }

  free(AA);
  return 0;
}

int Imfilter_f64(int h, int w, const unsigned char* A, int al,
    int kh, int kw, const double* K, double* B, int bl)
{
  int i, j, k, kn, kh2 = kh / 2, kw2 = kw / 2;
  int pos[256];
  unsigned char* AA;
  //lbp_ex_ft_3(h, w, A, al, B, bl, 1);
  //return 0;
  AA = (unsigned char*)malloc((h + kh - 1) * (w + kw - 1));
  COPY_REPLICATE_BORDER(h, w, A, al, (h + kh - 1), (w + kw - 1), AA, (w + kw - 1), 1, kh2, kw2);
  al = (w + kw - 1), A = AA + kh2 * (w + kw - 1) + kw2;
  //double krow[10];
  //double kcol[10];
  //dekernel(kh, kw, K, kw, krow, kcol);
  kn = 0;

  for (i = 0; i < kh; ++i) {
    for (j = 0; j < kw; ++j) {
      if (K[i * kw + j] != 0) {
        //K[kn] = K[i*kw+j];
        pos[kn] = (i - kh2) * al + (j - kw2);
        kn++;
      }
    }
  }

  //{utime_start(_start_time);
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      const unsigned char* AA = A + i * al + j;
      double b = 0;

      for (k = 0; k < kn; ++k) {
        b += AA[pos[k]] * K[k];
      }

      B[i * bl + j] = b;
    }
  }

  //printf(" %.4f", utime_elapsed(_start_time));}

  free(AA);
  return 0;
}
