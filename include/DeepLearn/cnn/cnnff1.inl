
int convn_valid_f64(int ah, int aw, const double* a, int al, int kh, int kw, const double* ker, double* c, int cl, int f) {
  int i, j, k, kn = kh*kw, ch=ah-kh+1, cw=aw-kw+1;
  int stoff[100];
  ASSERT(kn<100);
  for (i=0; i<kh; ++i) {
    for (j=0; j<kw; ++j) {
      stoff[i*kw+j] = (kh-1-i)*al+(kw-1-j);
    }
  }
  for (;f-->0;a+=ah*al, c+=ch*cl) {
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
  }
  return 0;
}
double sigm(double x) {
  double y = 1./(1+exp(-x));
  return y;
}
int subsampling_2x2_f64(int ah, int aw, const double* a, int al, double* c, int cl, int f) {
  int i, j, ch = ah/2, cw = aw/2, kh=2, kw=2, kn=kh*kw;
  int stoff[20];
  ASSERT(kn<20);
  for (i=0; i<kh; ++i) {
    for (j=0; j<kw; ++j) {
      stoff[i*kw+j] = (kh-1-i)*al+(kw-1-j);
    }
  }
  for (;f-->0;a+=ah*al, c+=ch*cl) {
    for (i=0; i<ch; ++i) {
      const double* a0 = a + (i*2)*al;
      double* c0 = c + i*cl;
      for (j=0; j<cw; ++j, a0+=2) {
        double z = a0[stoff[0]]+a0[stoff[1]]+a0[stoff[2]]+a0[stoff[3]];
        c0[j] = z*(1./4);
      }
    }
  }
  return 0;
}

int cnnff1(const void* cnn, int h, int w, const uchar* img, int* y, buf_t* bf) {
  double* o = NULL;
  double* a = NULL;
  char* p = (char*)cnn;
  int i, j, l, n, inputmaps=1;
  int nclass = *getarg(int, p);
  int tt = *getarg(int, p);
  const double* ffb = getargs(double, p, nclass);
  const double* ffW = getargs(double, p, nclass*tt);
  //cvShowMat("ffW", "%I64f", nclass, tt, ffW, tt*sizeof(double), sizeof(double), -1);cvWaitKey(-1);
  n = *getarg(int, p);
  BFMALLOC(bf, a, h*w);
  for (i=0; i<h*w; ++i) {
    a[i] = img[i]*(1./255);
  }
  //cvShowMat("au", "%I8u", h, w, img, w*sizeof(uchar), sizeof(uchar), -1);
  //cvShowMat("a", "%I64f", h, w, a, w*sizeof(double), sizeof(double), -1);cvWaitKey(-1);
  for (l=1; l<n; ++l) {
    char type = *getarg(char, p);
    double* a1 = NULL;
    if ('c'==type) {
      int outputmaps = *getarg(int, p);
      int kernelsize = *getarg(int, p);
      const double* k = getargs(double, p, inputmaps*outputmaps*kernelsize*kernelsize);
      const double* b = getargs(double, p, outputmaps);
      int h2 = (h-kernelsize+1), w2=(w-kernelsize+1);
      int nsz = h2*w2*outputmaps;
      BFMALLOC(bf, a1, nsz);
      FILL(nsz, a1, 0);
      for (j=0; j<outputmaps; ++j) {
        double* z = a1 + j*h2*w2;
        const double* k0 = k + j*kernelsize*kernelsize * inputmaps;
        for (i=0; i<inputmaps; ++i) {
          const double* k1 = k0 + i*kernelsize*kernelsize;
          convn_valid_f64(h, w, a + i*h*w, w, kernelsize, kernelsize, k1, z, w2, 1);
          //cvShowMat("ker", "%I64f", kernelsize, kernelsize, k1, kernelsize*sizeof(double), sizeof(double), -1);
          //cvShowMat("z", "%I64f", h2, w2, z, w2*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
        }
        for (i=0; i<h2*w2; ++i) {
          z[i] = sigm(z[i]+b[j]);
        }
        //cvShowMat("z", "%I64f", h2, w2, z, w2*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
      }
      inputmaps = outputmaps;
      h = h2, w = w2;
    } else if ('s'==type) {
      int h2 = h/2, w2=w/2;
      int nsz = h2*w2*inputmaps;
      int scale = *getarg(int, p);
      //cvShowMat("a", "%I64f", h, w, a, w*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
      BFMALLOC(bf, a1, nsz);
      for (j=0; j<inputmaps; ++j) {
        subsampling_2x2_f64(h, w, a + j*h*w, w, a1 + j*h2*w2, w2, 1);
      }
      h = h2, w = w2;
      //cvShowMat("a", "%I64f", h, w, a, w*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
    } else if ('f'==type) {
      int nrow = *getarg(int, p);
      int ncol = *getarg(int, p);
      double* b = getargs(double, p, nrow);
      double* weights = getargs(double, p, nrow*ncol);
      BFMALLOC(bf, a1, nrow);
      //cvShowMat("weights", "%I64f", nrow, ncol, weights, ncol*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
      //cvShowMat("a", "%I64f", h*w*inputmaps, 1, a, 1*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
      for (i=0; i<nrow; ++i) {
        double z=b[i];
        double* w = weights + i*ncol;
        for (j=0; j<ncol; ++j) {
          z += a[j]*w[j];
        }
        a1[i] = sigm(z);
      }
      h = 1, w = nrow;
      inputmaps = 1;
      //cvShowMat("a", "%I64f", h*w, 1, a1, 1*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
    }

    a = a1;
  }
  //cvShowMat("a", "%I64f", h*w*inputmaps, 1, a, 1*sizeof(double), sizeof(double), -1); cvWaitKey(-1);
  {
    double zz=-100000000;
    *y = 0;
    ASSERT(tt==h*w*inputmaps);
    for (i=0; i<nclass; ++i) {
      double z=ffb[i];
      const double* ffw = ffW + i*tt;
      for (j=0; j<tt; ++j) {
        z += ffw[j] * a[j];
      }
      z = sigm(z);
      if (z>zz) {
        *y = i+1;
        zz = z;
      }
    }
  }
  return 0;
}

