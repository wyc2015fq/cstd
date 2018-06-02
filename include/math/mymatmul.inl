
int mat_transpose0(const void* a, int al, void* b, int bl, int h, int w, int elem_size) {
  int i, j;
  switch (elem_size) {
  case 8:
    {
      typedef struct {uchar v[8]; } arr_type;
      const arr_type* _a = (const arr_type*)a;
      arr_type* _b = (arr_type*)b;
      al/=elem_size, bl/=elem_size;
      for (i=0; i<h; ++i) {
        for (j=0; j<w; ++j) {
          _b[j*bl+i] = _a[i*al+j];
        }
      }
    }
    break;
  default:
    break;
  }
  return 0;
}

#define transpose_2x2(hh)  hh(0, 0); hh(0, 1); hh(1, 0); hh(1, 1);
#define transpose_4x4(hh)  hh(0, 0); hh(0, 1); hh(0, 2); hh(0, 3); \
  hh(1, 0); hh(1, 1); hh(1, 2); hh(1, 3); \
  hh(2, 0); hh(2, 1); hh(2, 2); hh(2, 3); \
  hh(3, 0); hh(3, 1); hh(3, 2); hh(3, 3); \
  
#define hh(ii, jj)  _b[(j+jj)*_bl+(i+ii)] = _a[(i+ii)*_al+(j+jj)];

#define MAT_TRANSPOSE1(a, al, b, bl, h, w, arr_type)  { int i, j; \
  const arr_type* _a = (const arr_type*)(a); \
  arr_type* _b = (arr_type*)(b); \
  int _al = al, _bl = bl; \
  for (i=0; i<h-3; i+=4) { \
    for (j=0; j<w-3; j+=4) { transpose_4x4(hh); } \
    for (; j<w; ++j) { hh(0, 0); }\
  } \
  for (; i<h; ++i) { for (j=0; j<w; ++j) { hh(0, 0); } }\
}

#define MAT_TRANSPOSE_CASE(n)  case n: { typedef struct {uchar v[n]; } arr_type; MAT_TRANSPOSE1(a, al, b, bl, h, w, arr_type); } break;

int mat_transpose(const void* a, int al, void* b, int bl, int h, int w, int elem_size) {
  enum {ttn=2};
  al/=elem_size, bl/=elem_size;
  switch (elem_size) {
    MAT_TRANSPOSE_CASE(1);
    MAT_TRANSPOSE_CASE(2);
    MAT_TRANSPOSE_CASE(3);
    MAT_TRANSPOSE_CASE(4);
    MAT_TRANSPOSE_CASE(5);
    MAT_TRANSPOSE_CASE(6);
    MAT_TRANSPOSE_CASE(7);
    MAT_TRANSPOSE_CASE(8);
  default:
    break;
  }
  return 0;
}

#define MAT_TRANSPOSE(a, al, b, bl, h, w, arr_type)  { int i, j; \
  const arr_type* _a = (const arr_type*)(a); \
  arr_type* _b = (arr_type*)(b); \
  int _al = al, _bl = bl; \
  for (i=0; i<h; ++i) { \
    for (j=0; j<w; ++j) { \
      _b[(j)*_bl+(i)] = _a[(i)*_al+(j)]; \
    } \
  } \
}

void cmul(const double* a, const double* b, double* c) {
  double p, q, s;
  p = a[0]*b[0]; q = a[1]*b[1]; s = (a[0]+a[1])*(b[0]+b[1]);
  c[0] += p-q; c[1] += s-p-q;
  return ;
}

int matmul0(const double* a, int al, const double* b, int bl, double* d, int dl, int h, int w, int n, int flag) {
  int i, j, k;
  for (i=0; i<h; ++i) {
    double* d1 = d + i*dl;
    for (j=0; j<w; ++j) {
      const double* aa = a + i*al;
      const double* bb = b + j*bl;
      double dd[2] = {0};
      double p, q, s;
      for (k=0; k<n; ++k, aa+=2, bb+=2) {
        p = aa[0]*bb[0]; q = aa[1]*bb[1]; s = (aa[0]+aa[1])*(bb[0]+bb[1]);
        dd[0] += p-q;
        dd[1] += s-p-q;
      }
      d1[j*2+0] = dd[0];
      d1[j*2+1] = dd[1];
    }
  }
  return 0;
}

int mymatmul(const double* a, int al, const double* b, int bl, double* d, int dl, int h, int w, int n, int flag) {
  int i, j, k, t;
  double* a_buf = NULL;
  double* b_buf = NULL;
  int isat = !!(flag&CC_GEMM_A_T);
  int isbt = !(flag&CC_GEMM_B_T);
  int iscomplex = !!(flag&CC_GEMM_COMPLEX);
  int cn = iscomplex ? 2 : 1;
  int di = cn;
  if (flag&CC_GEMM_D_T) {
    CC_SWAP(dl, di, t);
  }
  if (isat && isbt) {
    double* d_buf, *dd;
    a_buf = MALLOC(double, n*cn+w*cn);
    d_buf = a_buf+n*cn;
    for (i=0; i<h; ++i) {
      const double* a1 = a + i*al;
      if (isat) {
        a1 = a+i*cn;
        if (iscomplex) {
          for (k=0; k<n; ++k) {
            a_buf[k*2+0] = a1[k*al+0];
            a_buf[k*2+1] = a1[k*al+1];
          }
        } else {
          for (k=0; k<n; ++k) {
            a_buf[k] = a1[k*al];
          }
        }
        a1 = a_buf;
      }
      for (j=0; j<w*cn; ++j) {
        d_buf[j] = 0;
      }
      dd = d_buf;
      if (iscomplex) {
        for (k=0; k<n; ++k) {
          const double* aa = a1+k*2;
          const double* bb = b + k*bl;
          for (j=0; j<w; ++j, bb+=2, dd+=2) {
            double p, q, s;
            p = aa[0]*bb[0]; q = aa[1]*bb[1]; s = (aa[0]+aa[1])*(bb[0]+bb[1]);
            dd[0] += p-q;
            dd[1] += s-p-q;
          }
        }
      } else {
        for (k=0; k<n; ++k) {
          double aa = a1[k];
          const double* b1 = b + k*bl;
          for (j=0; j<w-3; j+=4) {
            dd[j] += b1[j]*aa;
            dd[j+1] += b1[j+1]*aa;
            dd[j+2] += b1[j+2]*aa;
            dd[j+3] += b1[j+3]*aa;
          }
          for (; j<w; +j) {
            dd[j] += b1[j]*aa;
          }
        }
      }
      
      dd = d+i*di;
      if (iscomplex) {
        for (k=0; k<w; ++k) {
          dd[k*al+0] = d_buf[k*2+0];
          dd[k*al+1] = d_buf[k*2+1];
        }
      } else {
        for (k=0; k<w; ++k) {
          dd[k*dl] = d_buf[k];
        }
      }
    }
  } else {
    if (isat||isbt) {
      if (isbt) {
        const double* pt;
        CC_SWAP(isat, isbt, t);
        CC_SWAP(al, bl, t);
        CC_SWAP(h, w, t);
        CC_SWAP(dl, di, t);
        CC_SWAP(a, b, pt);
      }
      a_buf = MALLOC(double, n*cn);
    }
    for (i=0; i<h; ++i) {
      const double* a1 = a + i*al;
      if (isat) {
        a1 = a+i*cn;
        if (iscomplex) {
          for (k=0; k<n; ++k) {
            a_buf[k*2+0] = a1[k*al+0];
            a_buf[k*2+1] = a1[k*al+1];
          }
        } else {
          for (k=0; k<n; ++k) {
            a_buf[k] = a1[k*al];
          }
        }
        a1 = a_buf;
      }
      for (j=0; j<w; ++j) {
        const double* b1 = b + j*bl;
        if (iscomplex) {
          double p, q, s;
          const double* bb = b1;
          const double* aa = a1;
          double dd[2] = {0};
          for (k=0; k<n; ++k, aa+=2, bb+=2) {
            p = aa[0]*bb[0];
            q = aa[1]*bb[1];
            s = (aa[0]+aa[1])*(bb[0]+bb[1]);
            dd[0] += p-q;
            dd[1] += s-p-q;
          }
          d[i*dl + j*di + 0] = dd[0];
          d[i*dl + j*di + 1] = dd[1];
        } else {
          double t0=0, t1=0, t2=0, t3=0;
          for (k=0; k<n-3; k+=4) {
            t0 += a1[k]*b1[k];
            t1 += a1[k+1]*b1[k+1];
            t2 += a1[k+2]*b1[k+2];
            t3 += a1[k+3]*b1[k+3];
          }
          t0 += t1 + t2 + t3;
          for (; k<n; ++k) {
            t0 += a1[k]*b1[k];
          }
          d[i*dl + j*di] = t0;
        }
      }
    }
  }
  FREE(a_buf);
  return 0;
}

#if 0
#include "cv/matmul.inl"
int test_mymatmul() {
  int n = 1024, i;
  double* a = MALLOC(double, n*n*5);
  double* b = a+n*n*1;
  double* c = a+n*n*2;
  double* d = a+n*n*3;
  double err=0;
  for (i=0; i<n*n*5; ++i) {
    a[i] = rand()*1./RAND_MAX;
  }
  for (i=0; i<n*n; ++i) {
    c[i]=d[i]=0;
  }
  if (0) {
    {utime_start(_start_time);
    mat_transpose0(a, n*sizeof(double), c, n*sizeof(double), n, n, sizeof(double));
    printf("%f\n", utime_elapsed(_start_time));}
    {utime_start(_start_time);
    mat_transpose(a, n*sizeof(double), d, n*sizeof(double), n, n, sizeof(double));
    //MAT_TRANSPOSE(a, n, d, n, n, n, double);
    printf("%f\n", utime_elapsed(_start_time));}
    {utime_start(_start_time);
    MAT_TRANSPOSE(a, n, d, n, n, n, double);
    printf("%f\n", utime_elapsed(_start_time));}
    {utime_start(_start_time);
    icvGEMM_TransposeBlock(a, n*sizeof(double), d, n*sizeof(double), cvSize(n, n), sizeof(double));
    printf("%f\n", utime_elapsed(_start_time));}
    for (i=0; i<n*n; ++i) {
      err += fabs(c[i]-d[i]);
    }
    printf("%f\n", err);
  }
  if (1) {
    int tt = 1;
    {utime_start(_start_time);
    for (i=0; i<tt; ++i) {
      cvGEMM_64f(a, n, b, n, 1, 0, 0, 0, d, n, CC_GEMM_A_T, n, n, n);
    }
    printf("%f\n", utime_elapsed(_start_time));}
    {utime_start(_start_time);
    for (i=0; i<tt; ++i) {
      mymatmul(a, n, b, n, c, n, n, n, n, CC_GEMM_A_T|CC_GEMM_B_T);
    }
    printf("%f\n", utime_elapsed(_start_time));}
    for (i=0; i<n*n; ++i) {
      err += fabs(c[i]-d[i]);
    }
    printf("%f\n", err);
    if (n==2) {
      cvShowMat("a", "%I64f", n, n, a, n*sizeof(double), sizeof(double), -1);
      cvShowMat("b", "%I64f", n, n, b, n*sizeof(double), sizeof(double), -1);
      cvShowMat("c", "%I64f", n, n, c, n*sizeof(double), sizeof(double), -1);
      cvShowMat("d", "%I64f", n, n, d, n*sizeof(double), sizeof(double), -1);
      cvWaitKey(-1);
    }
  }
  free(a);
  return 0;
}

#endif

