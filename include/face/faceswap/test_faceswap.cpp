
#include "cv/cv.inl"
#include "face/facelandmark.inl"
#include "img/imgio.inl"
//#include "cv/drawing.inl"
#include "cv/imgproc/drawing.inl"
#include "geo/geo.inl"
//#include "cv/meansdv.inl"
//#include "cv/arithm.inl"


static void blas_saxpy2(enum CBLAS_TRANSPOSE TransB, int M, int N, float alpha, const float* A, float beta, const float* B, float* Y) {
  int i, j;
  if (CblasNoTrans==TransB) {
    for (i=0; i<M; ++i) {
      const float* a = A + i*N;
      float* y = Y + i*N;
      for (j=0; j<N; ++j) {
        y[j] = alpha*a[j] + beta*B[j];
      }
    }
  } else if (CblasTrans==TransB) {
    for (i=0; i<M; ++i) {
      const float* a = A + i*N;
      float b = beta * B[i];
      for (j=0; j<N; ++j) {
        Y[j] = alpha*a[j] + b;
      }
    }
  } else {
    ASSERT(0);
  }
}
static void blas_ssum(enum CBLAS_TRANSPOSE TransY, int M, int N, const float* A, float alpha, const float* B, float* Y) {
  int i, j;
  if (CblasNoTrans==TransY) {
    if (1.f==alpha) {
      for (i=0; i<M; ++i) {
        float sum = B ? B[i] : 0;
        const float* a = A + i*N;
        for (j=0; j<N; ++j) {
          sum += a[j];
        }
        Y[i] = sum;
      }
    } else if (2==alpha) {
      for (i=0; i<M; ++i) {
        float sum = B ? B[i] : 0;
        const float* a = A + i*N;
        for (j=0; j<N; ++j) {
          sum += a[j]*a[j];
        }
        Y[i] = sum;
      }
    } else {
      for (i=0; i<M; ++i) {
        float sum = B ? B[i] : 0;
        const float* a = A + i*N;
        for (j=0; j<N; ++j) {
          sum += pow(a[j], alpha);
        }
        Y[i] = sum;
      }
    }
  } else if (CblasTrans==TransY) {
    if (B) {
      blas_scopy(N, B, Y);
    } else {
      blas_sset(N, 0, Y);
    }
    if (1.f==alpha) {
      for (i=0; i<M; ++i) {
        const float* a = A + i*N;
        for (j=0; j<N; ++j) {
          Y[j] += a[j];
        }
      }
    } else if (2==alpha) {
      for (i=0; i<M; ++i) {
        const float* a = A + i*N;
        for (j=0; j<N; ++j) {
          Y[j] += a[j]*a[j];
        }
      }
    } else {
      for (i=0; i<M; ++i) {
        const float* a = A + i*N;
        for (j=0; j<N; ++j) {
          Y[j] += pow(a[j], alpha);
        }
      }
    }
  } else {
    ASSERT(0);
  }
}

static void blas_smean(enum CBLAS_TRANSPOSE TransY, int M, int N, const float* A, float* Y) {
  int n = CblasNoTrans==TransY ? M : N;
  int m = CblasNoTrans==TransY ? N : M;
  blas_ssum(TransY, M, N, A, 1.f, NULL, Y);
  blas_sscale(n, 1./m, Y, Y);
}
static void blas_smeanvar(enum CBLAS_TRANSPOSE TransY, int M, int N, const float* A, float* Y, float* S) {
  int n = CblasNoTrans==TransY ? M : N;
  int m = CblasNoTrans==TransY ? N : M;
  blas_ssum(TransY, M, N, A, 1.f, NULL, Y);
  blas_ssum(TransY, M, N, A, 2.f, NULL, S);
  for (int i=0; i<n; ++i) {
    float y = Y[i]/m;
    float s = S[i]/m;
    S[i] = s - y*y;
  }
}
static void blas_svar(enum CBLAS_TRANSPOSE TransY, int M, int N, const float* A, float* S) {
  int n = CblasNoTrans==TransY ? M : N;
  float* Y = (float*)malloc(n*sizeof(float));
  blas_smeanvar(TransY, M, N, A, Y, S);
  free(Y);
}
static void blas_sstd(enum CBLAS_TRANSPOSE TransY, int M, int N, const float* A, float* S) {
  int n = CblasNoTrans==TransY ? M : N;
  blas_svar(TransY, M, N, A, S);
  blas_ssqrt(n, S, S);
}


// findHomography
//warpAffine
//warpPerspective
static int transformation_from_points(int n, const FPOINT* points1, const FPOINT* points2, double* dout6) {
  //Return an affine transformation [s * R | T] such that:
  // sum ||s*R*p1,i + T - p2,i||^2
  //is minimized.
  // 计算仿射矩阵
  float6 out6;
  // 参考：https://github.com/matthewearl/faceswap/blob/master/faceswap.py
  //Solve the procrustes problem by subtracting centroids, scaling by the
  //standard deviation, and then using the SVD to calculate the rotation. See
  //the following for more details:
  //   https://en.wikipedia.org/wiki/Orthogonal_Procrustes_problem
  img_t im[10] = {0};
  int i=0;
  img_t* pt1 = cvSetMat(im+i++, n, 1, CC_32F, 2, points1);
  img_t* pt2 = cvSetMat(im+i++, n, 1, CC_32F, 2, points2);
  float2 c1, c2;
  float4 A = {0}, S = {0}, U = {0}, V = {0}, R = {0};
  float s1, s2, cc;
  blas_smean(CblasTrans, n, 2, pt1->tt.fl, c1);
  blas_smean(CblasTrans, n, 2, pt2->tt.fl, c2);
  //points1 -= c1;
  //points2 -= c2;
  blas_saxpy2(CblasNoTrans, n, 2, 1, pt1->tt.fl, -1, c1, pt1->tt.fl);
  blas_saxpy2(CblasNoTrans, n, 2, 1, pt2->tt.fl, -1, c2, pt2->tt.fl);
  blas_sstd(CblasNoTrans, 1, n*2, pt1->tt.fl, &s1);
  blas_sstd(CblasNoTrans, 1, n*2, pt2->tt.fl, &s2);
  //points1 /= s1
  //points2 /= s2
  blas_sscale(n*2, 1./s1, pt1->tt.fl, pt1->tt.fl);
  blas_sscale(n*2, 1./s2, pt2->tt.fl, pt2->tt.fl);
  blas_sgemm(CblasTrans, CblasNoTrans, 2, 2, n, 1, pt1->tt.fl, pt2->tt.fl, 0, A);
  //U, S, Vt = np.linalg.svd(points1.T * points2)
  //icvSVD_32f(A, S, U, V, CC_SVD_MODIFY_A);
  ssvd2(U, S, V, A);
  // The R we seek is in fact the transpose of the one given by U * Vt. This
  // is because the above formulation assumes the matrix goes on the right
  // (with row vectors) where as our solution requires the matrix to be on the
  // left (with column vectors).
  //R = (U * Vt).T
  blas_sgemm(CblasNoTrans, CblasTrans, 2, 2, 2, 1, V, U, 0, R);
  //np.vstack([np.hstack(((s2 / s1) * R, c2.T - (s2 / s1) * R * c1.T)),
  //                 np.matrix([0., 0., 1.])])
  float33 out = {0};
  cc = s2/s1;
  out[0][0] = cc * R[0];
  out[0][1] = cc * R[1];
  out[1][0] = cc * R[2];
  out[1][1] = cc * R[3];
  float2 t1 = {0};
  blas_sgemm(CblasNoTrans, CblasTrans, 2, 1, 2, cc, R, c1, 0, t1);
  out[0][2] = c2[0]-t1[0];
  out[1][2] = c2[1]-t1[1];
  blas_scopy(6, &out[0][0], out6);
  COPY(6, out6, dout6);
  return 0;
}

static int test_transformation_from_points() {
  FPOINT pt1[] = { 0,  1,        1, 2,    2, 3,       3, 4, 9, 9};
  FPOINT pt2[] = {10,  21,        11, 22,    12, 23, 13, 30, 100, 100};
  double6 out;
  double6 out1 = { 2.1194531,   -1.00395147,  10.83069902,    1.00395147,   2.1194531,   17.19544004,  };
  transformation_from_points(countof(pt1), pt1, pt2, out);
  if (0) {
    double9 H1 = {0};
    img_t im[10] = {0};
    img_t tmp[10] = {0};
    img_t* H = im;
    int n = countof(pt1);
    n = 5;
    img_t* _points1 = cvSetMat(tmp+0, n, 2, CC_32F, 1, pt1);
    img_t* _points2 = cvSetMat(tmp+1, n, 2, CC_32F, 1, pt2);
    findHomography(H, _points1, _points2, RANSAC);
    //getPerspectiveTransform
    if (H->rows==3 && H->cols==3) {
      memcpy(H1, H->data, 9*sizeof(double));
    }
    printf("\n");
  }

  double err = blas_deuclidean(9, out, out1);
  ASSERT(err<0.000001);
  return 0;
}
static int draw_pts(img_t* im, const FPOINT* pt, int n, int drawlabel, CScalar clr) {
  int i;
  for (i=0; i<n; ++i) {
    CPoint ptc = cPoint(pt[i].x, pt[i].y);
    //printf("%d\n", i);
    //cvCircle(im, ptc, 3, clr, -1, CC_AA);
    if (drawlabel) {
      circle(im, ptc, 3, clr, -1, CC_AA);
      if (drawlabel) {
        char buf[256];
        _snprintf(buf, 256, "%d", i);
        //cvPutText(im, buf, ptc, NULL, clr);
        putText(im, buf, ptc, CC_FONT_HERSHEY_SIMPLEX, 0.5, clr, 1, 16);
      }
    }
  }
  return 0;
}
static int draw_land(img_t* im, const FPOINT* pt, int n, const FPOINT* fpt3, int test) {
  int i;
  CScalar clr_r = cScalar(0, 0, 255, 0);
  CScalar clr_g = cScalar(0, 255, 0, 0);
  FPOINT pthull[200];
  IPOINT ipthull[200];
  if (test) {
    draw_pts(im, pt, n, test, clr_r);
    draw_pts(im, fpt3, 3, test, clr_g);
  }
  if (!test) {
    FPOINT pt2[200];
    for (i=0; i<n; ++i) {
      pt2[i].x = pt[i].x;
      pt2[i].y = pt[i].y;
    }
    int n2 = hull_Graham_scan(pt2, pthull, n);
    for (i=0; i<n2; ++i) {
      ipthull[i].x = pthull[i].x;
      ipthull[i].y = pthull[i].y;
    }
    IPOINT* pp = ipthull;
    //cvPolyLine(im, &pp, &n2, 1, 1, clr_r, 1, CC_AA);
    //cvFillPoly(im, &pp, &n2, 1, clr_r, 1);
    CScalar clr_w = cScalar(255, 255, 255, 0);
    fillConvexPoly(im, pp, n2, clr_w, 0, 0);
  }
  return 0;
}

static void sinv2x3(float6 M) {
  double D = M[0]*M[4] - M[1]*M[3];
  D = D != 0 ? 1./D : 0;
  double A11 = M[4]*D, A22=M[0]*D;
  M[0] = A11; M[1] *= -D;
  M[3] *= -D; M[4] = A22;
  double b1 = -M[0]*M[2] - M[1]*M[5];
  double b2 = -M[3]*M[2] - M[4]*M[5];
  M[2] = b1; M[5] = b2;
}
static void dinv2x3(double6 M) {
  double D = M[0]*M[4] - M[1]*M[3];
  D = D != 0 ? 1./D : 0;
  double A11 = M[4]*D, A22=M[0]*D;
  M[0] = A11; M[1] *= -D;
  M[3] *= -D; M[4] = A22;
  double b1 = -M[0]*M[2] - M[1]*M[5];
  double b2 = -M[3]*M[2] - M[4]*M[5];
  M[2] = b1; M[5] = b2;
}
static img_t* warpAffine(const img_t* src, img_t* dst, const double6 M)  {
  int i, j;
  //double6 M;
  //cvSetMat(dst, size.h, size.w, CC_8U, src->c);
  //blas_dcopy(6, M0, M);
  //dinv2x3(M);
  for (i=0; i<dst->h; ++i) {
    uchar* d = dst->data + i*dst->s;
    for (j=0; j<dst->w; ++j) {
      int x = j*M[0]+i*M[1]+M[2];
      int y = j*M[3]+i*M[4]+M[5];
      if (x>0 && x<src->w && y>0 && y<src->h) {
        uchar* s = src->data + (y)*src->s + x*src->c;
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
      }
      d += 3;
    }
  }
  return dst;
}
static img_t* warpPerspective(const img_t* src, img_t* dst, const double9 M)  {
  int i, j;
  //double9 M;
  //cvSetMat(dst, size.h, size.w, CC_8U, src->c);
  //blas_scopy(6, M0, M);
  //sinv2x3(M);
  for (i=0; i<dst->h; ++i) {
    uchar* d = dst->data + i*dst->s;
    for (j=0; j<dst->w; ++j) {
      double z = j*M[6]+i*M[7]+M[8];
      int x = (j*M[0]+i*M[1]+M[2])/z;
      int y = (j*M[3]+i*M[4]+M[5])/z;
      if (x>0 && x<src->w && y>0 && y<src->h) {
        uchar* s = src->data + (y)*src->s + x*src->c;
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
      }
      d += 3;
    }
  }
  return dst;
}

#include "cv/photo/seamless_cloning.inl"

// 定位不到脸
// src - 照片
// dst - 年画
static int faceswap(const img_t* src, const img_t* dst, img_t* out, const FPOINT* ptsrc3, const FPOINT* ptdst3, bool debug) {
  FPOINT _flsrc[101] = {0};
  FPOINT _fldst[101] = {0};
  CPoint2f _ptsrc[4] = {0};
  CPoint2f _ptdst[4] = {0};
  const FPOINT* flsrc = facelandmark(src, _flsrc, ptsrc3, _ptsrc);
  const FPOINT* fldst = facelandmark(dst, _fldst, ptdst3, _ptdst);
  int ret = 0;
  IM_BEGIN(im, 10);
  if (0) {
    img_t im[10] = {0};
    img_t* src1 = imclone2(src, im+0);
    img_t* dst1 = imclone2(dst, im+1);
    draw_land(src1, flsrc, 101, _ptsrc, 1);
    draw_land(dst1, fldst, 101, _ptdst, 1);
    //imwrite("D:\\aaa.jpg", out);
    imshow_(out);
    imshow_(src1);
    imshow_(dst1);
    WaitKey(-1);
    imfrees2(im);
  }
  if (flsrc && fldst) {
    img_t* mask = cvSetMat(im++, dst->h, dst->w, CC_8U, 1);
    img_t* src2 = cvSetMat(im++, dst->h, dst->w, CC_8U, src->c);
    DRECT rc;
    int k = 10;
    double9 M;
    double9 invM;
    if (1) {
      if (1) {
        getAffineTransform(M, _ptdst, _ptsrc);
        getAffineTransform(invM, _ptsrc, _ptdst);
      } else {
        //transformation_from_points(72, flsrc, fldst, M);
        //transformation_from_points(4, _ptsrc, _ptdst, invM);
        //transformation_from_points(4, _ptdst, _ptsrc, M);
      }
      warpAffine(src, src2, M);
      if (ptdst3) {
        int i=0;
        for (i=0; i<101; ++i) {
          float x = _flsrc[i].x, y = _flsrc[i].y;
          _fldst[i].x = (x*invM[0]+y*invM[1]+invM[2]);
          _fldst[i].y = (x*invM[3]+y*invM[4]+invM[5]);
        }
      }
    }
    draw_land(mask, fldst, 72, _ptdst, 0);
    pt_boundrc(72, fldst, &rc);
    k = MAX((rc.r - rc.l)/40, 2);
    imdilate(mask, mask, cSize(k, k), CC_MORPH_RECT, iPOINT(k/2, k/2), BD_REPLICATE);
    
    IPOINT p = iPOINT(0, 0);
    //imshow_(src2);    imshow_(dst);    WaitKey(-1);
    cvSeamlessClone(src2, dst, mask, p, out, 1);
    ret = 1;
    if (debug) {
      img_t* src1 = imclone2(src, im++);
      img_t* dst1 = imclone2(dst, im++);
      draw_land(src1, flsrc, 101, _ptsrc, 1);
      draw_land(dst1, fldst, 101, _ptdst, 1);
      //imwrite("D:\\aaa.jpg", out);
      imshow_(out);
      imshow_(src1);
      imshow_(dst1);
      imshow_(mask);
      WaitKey(-1);
    }
  }
  IM_END(im);
  return ret;
}

static char* getopt_str(int argc, char** argv, const char* name, char* def) {
  int i;
  for (i=0; i<argc; ++i) {
    char* arg = argv[i];
    if ((arg[0]=='//' || arg[0]=='-') && 0==stricmp(arg+1, name)) {
      return argv[i+1];
    }
  }
  return def;
}
static bool getopt_bool(int argc, char** argv, const char* name, bool def) {
  int i;
  for (i=0; i<argc; ++i) {
    char* arg = argv[i];
    if ((arg[0]=='//' || arg[0]=='-') && 0==stricmp(arg+1, name)) {
      return !def;
    }
  }
  return def;
}
static double getopt_double(int argc, char** argv, const char* name, double def) {
  int i;
  for (i=0; i<argc; ++i) {
    char* arg = argv[i];
    if ((arg[0]=='//' || arg[0]=='-') && 0==stricmp(arg+1, name)) {
      return atof(argv[i+1]);
    }
  }
  return def;
}
static int getopt_int(int argc, char** argv, const char* name, int def) {
  int i;
  for (i=0; i<argc; ++i) {
    char* arg = argv[i];
    if ((arg[0]=='//' || arg[0]=='-') && 0==stricmp(arg+1, name)) {
      return atoi(argv[i+1]);
    }
  }
  return def;
}

int test_faceswap(int argc, char** argv) {CPCODETO("D:/code/end_code/rywfacedetect");
  //test_transformation_from_points();return 0;

  //const char* srcfn = "coarse_tilt_aligned_face.2114.11867065084_ff3a253813_o.jpg";
  //const char* srcfn = "coarse_tilt_aligned_face.2115.11867068474_33de2110d0_o.jpg";
  const char* srcfn = getopt_str(argc, argv, "src", "人脸.JPG");
  //const char* dstfn = "coarse_tilt_aligned_face.2112.11867070234_d349482564_o.jpg";
  const char* dstfn = getopt_str(argc, argv, "dst", "年画.jpg");
  const char* outfn = getopt_str(argc, argv, "out", "out.jpg");
  const char* dstptstr = getopt_str(argc, argv, "dstpt", NULL);
  FPOINT _dstpt[3] = {230, 203, 342,171, 305,249};
  FPOINT* dstpt = _dstpt;
#ifdef _DEBUG
  bool debug = true;
#else
  bool debug = getopt_bool(argc, argv, "debug", false);
#endif
  img_t im[20] = {0};
  int i=0;
  img_t* src = imread(srcfn, 3, 1, im+i++);
  img_t* dst = imread(dstfn, 3, 1, im+i++);
  if (dstptstr) {
    float a, b, c, d, e, f;
    sscanf(dstptstr, "%f %f %f %f %f %f", &a, &b, &c, &d, &e, &f);
    _dstpt[0] = fPOINT(a, b);
    _dstpt[1] = fPOINT(c, d);
    _dstpt[2] = fPOINT(e, f);
    dstpt = _dstpt;
  }
  //imresize1(src, 2, src);
  //imresize1(dst, 2, dst);
  if (src && dst) {
    img_t* out = im+i++;
    int err = faceswap(src, dst, out, NULL, dstpt, debug);
    imwrite(outfn, out);
  }
  imfrees2(im);

  return 0;
}
