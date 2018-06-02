#include "ui/window.inl"
#include "wav.inl"
#include "mfcc.inl"
#include "trainhmm.inl"
#include "sigp.inl"
int imgs_loaddat(const char* fn, img_t* im, int maxn)
{
  FILE* pf = fopen(fn, "rb");
  if (pf) {
    int n, i, zz[2];
    fread(&n, sizeof(int), 1, pf);
    n = MIN(n, maxn);
    for (i = 0; i < n; ++i) {
      fread(zz, sizeof(int), 2, pf);
      imsetsize_f8(im + i, zz[0], zz[1], 1);
      fread(im[i].tt.data, sizeof(double), zz[0]*zz[1], pf);
    }
    fclose(pf);
    return n;
  }
  return 0;
}
int hmm_loaddat(const char* fn, hmm_t* hmm, int maxn)
{
  FILE* pf = fopen(fn, "rb");
  if (pf) {
    int n, i, j, zz[2], M[HMMMAXMIX], N, D;
    fread(&n, sizeof(int), 1, pf);
    n = MIN(n, maxn);
    for (i = 0; i < n; ++i) {
      hmm_t* hmmi = hmm + i;
      fread(zz, sizeof(int), 2, pf);
      fread(M, sizeof(int), N = zz[0], pf);
      hmm_setsize(hmm + i, zz[0], D = zz[1], M);
      fread(hmmi->init->tt.f8, sizeof(double), N, pf);
      fread(hmmi->trans->tt.f8, sizeof(double), N * N, pf);
      for (j = 0; j < N; ++j) {
        fread(hmmi->mix[j].tt.f8, sizeof(double), M[j] * (D + D + 1), pf);
        //imshowmat_f8((hmm->mix+j));cvWaitKey(-1);
      }
      //imshowmat_f8(hmm[i].trans);cvWaitKey(-1);
    }
    fclose(pf);
    return n;
  }
  return 0;
}
int test_chmm()
{
  const char* workpath = NULL;
  int n, i = 0, j;
  img_t samples[200] = {0};
  img_t im[10] = {0};
  img_t* y = im + i++;
  hmm_t hmm[10] = {0};
  workpath = "E:/code/c/AI/ÓïÒôÊ¶±ð/HMM-GMM/chmm";
  _chdir(workpath);
  if (0) {
    wav_t w[1] = {0};
    vstr_t sv[1] = {0};
    vstr_load("list.txt", sv);
    n = sv->n;
    ASSERT(n < 200);
    for (j = 0; j < n; ++j) {
      wavread_filename(sv->v[j].s, w, 1);
      imsetsize_f8(y, w->nSamples, 1, 1);
      for (i = 0; i < w->nSamples; ++i) {
        y->tt.f8[i] = w->data[i] * (1. / (1 << 15));
      }
      //MEM_CHECK_BEGIN();
      //{utime_start(_start_time);
      mfcc(w->nSamples, y->tt.f8, samples + j);
      //printf("%f\n", utime_elapsed(_start_time));}
      //imfree(cc); MEM_CHECK_END();
      //imshowmat_f8((samples+i));cvWaitKey(-1);
    }
    wavfree(w);
  }
  else {
    n = imgs_loaddat("samples.dat", samples, countof(samples));
    // imshowmat_f8((samples));cvWaitKey(-1);
  }
  {
    int M[] = {3, 3, 3, 3};
    int N = countof(M);
    ASSERT(n == 100);
    if (0) {
      for (i = 0; i < 10; ++i) {
        inithmm(hmm + i, 10, samples, N, M);
      }
    }
    else {
      hmm_loaddat("hmm.dat", hmm, 10);
      //imshowmat_f8((hmm->mix));cvWaitKey(-1);
    }
    for (i = 0; i < 10; ++i) {
      trainhmm(hmm + i, 10, samples, N, M);
    }
  }
  imfrees2(samples);
  imfrees2(im);
  return 0;
}

