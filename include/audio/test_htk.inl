#include "cstd.h"
#include "ui/window.inl"
#include "sigp.inl"
#include "math.inl"
#include "wav.inl"
//#include "esignal/esignal.inl"
#include "wave.inl"
/* ZeroMeanFrame: remove dc offset from given vector */
void ZeroMean_32f(int n, float* v)
{
  int size, i;
  float sum = 0.0, off;
  size = n;
  for (i = 0; i < size; i++) {
    sum += v[i];
  }
  off = sum / size;
  for (i = 0; i < size; i++) {
    v[i] -= off;
  }
}
int GetFrameFeat(IOConfigRec* cf, const short* data, double sampPeriod, float* pbuf)
{
  int i = 0, j = 0, x;
  double m, e, curVol;
  short btgt = cf->tgtPK & BASEMASK;
  float re = 0, rawte = 0.0, te = 0, *p = 0, cepScale = 1.0;
  int bsize = 0;
  float* v = NULL;
  char buf[50] = {0};
  BOOL rawE = 0;
  int frSize, frRate, ns;
  float* s = NULL;
  //int nSamples = w->nSamples;
  //const short* data = w->data;
  ns = frSize = (int)(cf->winDur / sampPeriod);
  frRate = (int)(cf->tgtSampRate / sampPeriod);
  //if (frSize > nSamples) { return 0; }
  //n = (nSamples - frSize) / frRate + 1;
  /* Calc frame energy 0.0-100dB */
  MYREALLOC(s, ns);
  for (j = 0, m = e = 0.0; j < frSize; ++j) {
    s[j] = x = (int)data[j];
    m += x;
    e += x * x;
  }
  m = m / frSize;
  e = e / frSize - m * m;
  e = (e > 0.0) ? 10.0 * log10(e / 0.32768) : 0.0;
  curVol = e;
  p = pbuf;
  rawE = cf->rawEnergy;
  if (btgt < MFCC && cf->v1Compat) {
    rawE = FALSE;
  }
  if (cf->addDither != 0.0) {
    for (i = 0; i < ns; i++) {
      s[i] += (RAND01() * 2.0 - 1.0) * cf->addDither;
    }
  }
  if (cf->zMeanSrc && !cf->v1Compat) {
    ZeroMean_32f(frSize, s);
  }
  if ((cf->tgtPK & HASENERGY) && rawE) {
    rawte = 0.0;
    for (i = 1; i <= ns; i++) {
      rawte += s[i] * s[i];
    }
  }
  if (cf->preEmph > 0.0) {
    PreEmphasise_32f(ns, s, cf->preEmph);
  }
  if (cf->useHam) {
    ASSERT(cf->winCoef);
    Ham(ns, s, cf->winCoef);
    //cvShowMat_f4(s, ns, 1); cvWaitKey(-1);
  }
  //cvShowMat_f4(s, ns, 1); cvWaitKey(-1);
  switch (btgt) {
  case MFCC:
    Wave2FBank(cf, s, cf->fbank, rawE ? NULL : &te);
    FBank2MFCC(cf->numChans, cf->fbank, cf->c, cf->numCepCoef);
    if (cf->cepLifter > 0) {
      WeightCepstrum(cf->c, 0, cf->numCepCoef, cf->cepWin);
    }
    //cvShowMat_f4(cf->c, cf->numCepCoef, 1); cvWaitKey(-1);
    v = cf->c;
    bsize = cf->numCepCoef;
    break;
#if 0
  case LPC:
    Wave2LPC(s, cf->a, cf->k, &re, &te);
    v = cf->a;
    bsize = cf->lpcOrder;
    break;
  case LPREFC:
    Wave2LPC(s, cf->a, cf->k, &re, &te);
    v = cf->k;
    bsize = cf->lpcOrder;
    break;
  case LPCEPSTRA:
    Wave2LPC(s, cf->a, cf->k, &re, &te);
    LPC2Cepstrum(cf->a, cf->c);
    if (cf->cepLifter > 0) {
      WeightCepstrum(cf->c, 1, cf->numCepCoef, cf->cepLifter);
    }
    v = cf->c;
    bsize = cf->numCepCoef;
    break;
  case MELSPEC:
  case FBANK:
    Wave2FBank(s, cf->fbank, rawE ? NULL : &te, cf->fbInfo);
    v = cf->fbank;
    bsize = cf->numChans;
    break;
  case PLP:
    Wave2FBank(s, cf->fbank, rawE ? NULL : &te, cf->fbInfo);
    FBank2ASpec(cf->fbank, cf->as, cf->eql, cf->compressFact, cf->fbInfo);
    ASpec2LPCep(cf->as, cf->ac, cf->lp, cf->c, cf->cm);
    if (cf->cepLifter > 0) {
      WeightCepstrum(cf->c, 1, cf->numCepCoef, cf->cepLifter);
    }
    v = cf->c;
    bsize = cf->numCepCoef;
    break;
#endif
  default:
    HError("ConvertFrame: target %s is not a parameterised form", ParmKind2Str(cf->tgtPK, buf));
  }
  if (btgt == PLP || btgt == MFCC) {
    cepScale = (cf->v1Compat) ? 1.0 : cf->cepScale;
  }
  for (i = 0; i < bsize; i++) {
    *p++ = v[i] * cepScale;
  }
  if (cf->tgtPK & HASZEROC) {
    if (btgt == MFCC) {
      *p = FBank2C0(cf->numChans, cf->fbank) * cepScale;
      if (cf->v1Compat) {
        *p *= cf->eScale;
      }
      ++p;
    }
    else { /* For PLP include gain as C0 */
      *p++ = v[bsize + 1] * cepScale;
    }
    cf->curPK |= HASZEROC ;
  }
  if (cf->tgtPK & HASENERGY) {
    if (rawE) {
      te = rawte;
    }
    *p++ = (te < MINLARG) ? LZERO : log(te);
    cf->curPK |= HASENERGY;
  }
  FREE(s);
  return frRate;
}
int FramesInWave(IOConfigRec* cf, int nSamples)
{
  if (cf->frSize > nSamples) {
    return 0;
  }
  return (nSamples - cf->frSize) / cf->frRate + 1;
}
int GetFeat(IOConfigRec* cf, wav_t* w, img_t* f)
{
  int i, n;
  if (cf->frSize > w->nSamples) {
    return 0;
  }
  n = (w->nSamples - cf->frSize) / cf->frRate + 1;
  imsetsize(f, n, cf->nCols, sizeof(float), 1);
  for (i = 0; i < n; ++i) {
    //cvShowMat("asdf", "%I32f", w->frSize, 1, buf, sizeof(float), sizeof(float), -1); cvWaitKey(-1);
    GetFrameFeat(cf, w->data + i * cf->frRate, w->sampPeriod, f->tt.f4 + i * cf->nCols);
  }
  //imshowmat_f4(f); cvWaitKey(-1);
  return n;
}
int test_htk()
{
  int i, n = 0;
  vstr_t sv[1] = {0};
  wav_t w[1] = {0};
  IOConfigRec cf[1] = {0};
  img_t im[10] = {0};
  IOConfigRec_initdef(cf);
  cf->tgtPK = Str2ParmKind("MFCC_0_D_A");
  cf->winDur = 250000;
  cf->tgtSampRate = 100000;
  cf->numCepCoef = 12;
  cf->useHam = TRUE;
  cf->preEmph = 0.97;
  cf->numChans = 26;
  cf->cepLifter = 22;
  wavresize(w, 2 * 16000, 1);
  //wavrecord(w->data, w->nSamples, 16000, 16, NULL);
  //wavplay(w->data, w->nSamples, 16000, 16, NULL);
  _chdir("E:/pub/bin/htk/train11");
  vstr_load("list.txt", sv);
  for (i = 0; i < sv->n; ++i) {
    wavread_filename(sv->v[i].s, w, imSum);
    wavwrite_filename("aaa.wav", w);
    //wavplay(w->data, w->nSamples, w->fs, 16, NULL);
    IOConfigRec_reinit(cf, w->sampPeriod);
    //cvShowMat("asdf", "%I16d", 100, 10, w->data, sizeof(short), sizeof(short), -1); cvWaitKey(-1);
    //wavplay(w->data, w->n, w->fs, 16, 0);
    GetFeat(cf, w, im);
  }
  IOConfigRec_free(cf);
  imfrees2(im);
  return 0;
}

