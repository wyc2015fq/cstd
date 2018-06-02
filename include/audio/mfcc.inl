#include "melbankm.inl"
#include "enframe.inl"
#include "img/imgopt.inl"
#include "math/gencoswin.inl"
//#include "img\cxdxt.inl"
#include "fft.inl"
// a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
// - a(2)*y(n-1) - ... - a(na+1)*y(n-na)
int filter(int nb, const double* B, int na, const double* A, int n, const double* X, double* Y)
{
  double* X1 = 0;
  double A1[1] = {1};
  int i, j, nab = MAX(na, nb);
  if (NULL == A) {
    A1[0] = na;
    A = A1;
    na = 1;
  }
  MYREALLOC(X1, n + nab - 1);
  MEMCPY(X1 + nab - 1, X, n);
  for (i = 0; i < n; ++i) {
    double ca = 0, cb = 0;
    double* x = X1 + i;
    for (j = 0; j < nb; ++j) {
      cb += x[j] * B[nb - j - 1];
    }
    for (j = 1; j < na; ++j) {
      ca += x[j] * A[na - j - 1];
    }
    Y[i] = (cb - ca) / A[0];
  }
  FREE(X1);
  return 0;
}
int mfcc(int n, const double* k, img_t* cc)
{
  // cc=mfcc(k)计算语音k的MFCC系数
  int i = 0, j;
  // M为滤波器个数，N为一帧语音采样点数
  int M = 24, N = 256;
  img_t im1[10] = {0};
  img_t* bank = im1 + i++;
  img_t* dctcoef = im1 + i++;
  img_t* AggrK = im1 + i++;
  img_t* FrameK = im1 + i++;
  img_t* S = im1 + i++;
  img_t* P = im1 + i++;
  img_t* D = im1 + i++;
  img_t* m = im1 + i++;
  img_t* dtm = im1 + i++;
  img_t* cc1 = im1 + i++;
  double w[12], wmax;
  double fB[] = {1, -0.9375};
  double hamming[256];
  gencoswin(N, cw_hamming, hamming);
  // 归一化mel滤波器组系数
  melbankm(M, N, 8000, 0, 0.5, melbankm_flag_m | melbankm_flag_nor, bank, 0, 0);
  //imshowmat_f8(bank); cvWaitKey(-1);
  //bank = full(bank);
  //bank = bank / max(bank(:));
  // DCT系数,12*24
  imsetsize_f8(dctcoef, 12, 24, 1);
  for (i = 0; i < 12; ++i) {
    for (j = 0; j < 24; ++j) {
      dctcoef->tt.f8[i * 24 + j] = cos((2 * j + 1) * (i + 1) * PI / (2 * 24));
    }
  }
  //imshowmat_f8(dctcoef); cvWaitKey(-1);
  // 归一化倒谱提升窗口
  wmax = 0;
  for (i = 0; i < 12; ++i) {
    w[i] = 1 + 6 * sin(PI * (i + 1) / 12.);
    if (w[i] > wmax) {
      wmax = w[i];
    }
  }
  for (i = 0; i < 12; ++i) {
    w[i] /= wmax;
  }
  imsetsize_f8(AggrK, n, 1, 1);
  // 预加重
  //test_fft();
  filter(2, fB, 1, 0, n, k, AggrK->tt.f8);
  //imscale2(AggrK, 1<<16, AggrK, 0, 0, AggrK); imshowmat_f8(AggrK); cvWaitKey(-1);
  // 分帧
  // 加窗
  //test_fft();
  enframe(n, AggrK->tt.f8, N, hamming, 80, FrameK);
  //imscale2(FrameK, 1<<16, FrameK, 0, 0, FrameK); imshowmat_f8(FrameK); cvWaitKey(-1);
  //imtrans(FrameK, S, 1, 0, 0);
  imclone2(FrameK, S);
  fft_abs_b2(S->h, S->w, S->tt.f8, 1, S->w);
  // 计算功率谱
  //S = (abs(fft(FrameK))). ^ 2;
  //imscale2(S, 1 << 16, S, 0, 0, S);
  //imshowmat_f8(bank); imshowmat_f8(S); cvWaitKey(-1);
  // 将功率谱通过滤波器组
  //P = bank * S(1: 129, :);
  imsetsize_f8(P, bank->h, S->h, 1);
  mat_mul_ABT(P->h, P->w, bank->w, bank->tt.f8, bank->w, S->tt.f8, S->w, P->tt.f8, P->w);
  //imscale2(P, 1 << 16, P, 0, 0, P); imshowmat_f8(P); cvWaitKey(-1);
  // 取对数后作离散余弦变换
  //D = dctcoef * log(P);
  for (i = 0; i < P->h * P->w; ++i) {
    P->tt.f8[i] = log(P->tt.f8[i]);
  }
  imsetsize_f8(D, P->w, dctcoef->h, 1);
  mat_mul_ATBT(D->h, D->w, dctcoef->w, dctcoef->tt.f8, dctcoef->w, P->tt.f8, P->w, D->tt.f8, D->w);
  //imshowmat_f8(D); cvWaitKey(-1);
  // 倒谱提升窗
  imsetsize_f8(m, D->h, D->w, 1);
  mat_dot_Ab(D->h, D->w, D->tt.f8, D->w, w, m->tt.f8, m->w);
  //imshowmat_f8(m); cvWaitKey(-1);
  imsetsize_f8(dtm, D->h, D->w, 1);
  // 差分系数
  for (i = 2; i < m->h - 2; ++i) {
    const double* m1 = m->tt.f8 + (i - 2) * m->w;
    const double* m2 = m->tt.f8 + (i - 1) * m->w;
    const double* m3 = m->tt.f8 + (i + 1) * m->w;
    const double* m4 = m->tt.f8 + (i + 2) * m->w;
    double* dtm1 = dtm->tt.f8 + i * dtm->w;
    for (j = 0; j < m->w; ++j) {
      dtm1[j] = (-2 * m1[j] - m2[j] + m3[j] + 2 * m4[j]) / 3;
    }
  }
  //合并mfcc参数和一阶差分mfcc参数
  //cc = [m, dtm];
  imcath2(m, dtm, cc1);
  //去除首尾两帧，因为这两帧的一阶差分参数为0
  imsubcopy(cc1, iRECT(0, 2, cc1->w, cc1->h - 2), cc);
  //imshowmat_f8(cc); cvWaitKey(-1);
  imfrees(im1, 10);
  return 0;
}

