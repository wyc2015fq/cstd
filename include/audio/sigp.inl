#define HError printf
#define T_MEL 0002 /* Mel filterbank */
#include "Math.inl"
/* -- MFCC Related Operations -- */
//#define MAX_INT 536870911
enum _BaseParmKind {
  WAVEFORM, /* Raw speech waveform (handled by HWave) */
  LPC, LPREFC, LPCEPSTRA, LPDELCEP, /* LP-based Coefficients */
  IREFC, /* Ref Coef in 16 bit form */
  MFCC, /* Mel-Freq Cepstra */
  FBANK, /* Log Filter Bank */
  MELSPEC, /* Mel-Freq Spectrum (Linear) */
  USER, /* Arbitrary user specified data */
  DISCRETE, /* Discrete VQ symbols (shorts) */
  PLP, /* Standard PLP coefficients */
  ANON
};
#define HASENERGY 0100 /* _E log energy included */
#define HASNULLE 0200 /* _N absolute energy suppressed */
#define HASDELTA 0400 /* _D delta coef appended */
#define HASACCS 01000 /* _A acceleration coefs appended */
#define HASCOMPX 02000 /* _C is compressed */
#define HASZEROM 04000 /* _Z zero meaned */
#define HASCRCC 010000 /* _K has CRC check */
#define HASZEROC 020000 /* _0 0'th Cepstra included */
#define HASVQ 040000 /* _V has VQ index attached */
#define HASTHIRD 0100000 /* _T has Delta-Delta-Delta index attached */
#define BASEMASK 077 /* Mask to remove qualifiers */
typedef enum {
  FALSE_dup = FALSE, /* 0 */
  TRUE_dup = TRUE, /* 1 */
  TRI_UNDEF = -1 /* -1 */
}
TriState;
typedef short ParmKind; /* BaseParmKind + Qualifiers */
typedef enum { FFTbased, LPCbased, VQbased};
typedef struct {
#define IOCONFIG(a, b, c, d, e) a b;
#include "ioconfig.txt"
#undef IOCONFIG
} IOConfigRec;
int IOConfigRec_initdef(IOConfigRec* cf)
{
  const static IOConfigRec defcf[1] = {
#define IOCONFIG(a, b, c, d, e) c,
#include "ioconfig.txt"
#undef IOCONFIG
  };
  *cf = *defcf;
  return 0;
}
/* -- Windowing and PreEmphasis --*/
/* ZeroMean: zero mean a complete speech waveform */
int ZeroMean(short* data, long nSamples)
{
  long i, hiClip = 0, loClip = 0;
  short* x;
  double sum = 0.0, y, mean;
  x = data;
  for (i = 0; i < nSamples; i++, x++) {
    sum += *x;
  }
  mean = sum / (double)nSamples;
  x = data;
  for (i = 0; i < nSamples; i++, x++) {
    y = (double)(*x) - mean;
    if (y < -32767.0) {
      y = -32767.0;
      ++loClip;
    }
    if (y > 32767.0) {
      y = 32767.0;
      ++hiClip;
    }
    *x = (short)((y > 0.0) ? y + 0.5 : y - 0.5);
  }
  if (loClip > 0) {
    HError("ZeroMean: %d samples too -ve\n", loClip);
  }
  if (hiClip > 0) {
    HError("ZeroMean: %d samples too +ve\n", hiClip);
  }
  return 0;
}
/* GenHamWindow: generate precomputed Hamming window function */
static int GenHamWindow(int frSize, float* hamWin)
{
  int i;
  double a;
  a = TPI / (frSize - 1);
  for (i = 0; i < frSize; i++) {
    hamWin[i] = 0.54 - 0.46 * cos(a * i);
  }
  return frSize;
}
/* EXPORT->Ham: Apply Hamming Window to Speech frame s */
int Ham(int frSize, float* s, const float* hamWin)
{
  int i;
  for (i = 0; i < frSize; i++) {
    s[i] *= hamWin[i];
  }
  return 0;
}
/* EXPORT->PreEmphasise: pre-emphasise signal in s */
int PreEmphasise_32f(int n, float* s, float k)
{
  int i;
  double preE;
  preE = k;
  for (i = n - 1; i > 0; i--) {
    s[i] -= s[i - 1] * preE;
  }
  s[0] *= 1.0 - preE;
  return 0;
}
/* -- Linear Prediction Coding Operations -- */
/* AutoCorrelate: store auto coef 1 to p in r and return energy r[0] */
static double AutoCorrelate(const float* s, float* r, int p, int frSize)
{
  double sum, energy;
  int i, j;
  energy = 0.0;
  for (i = 0; i <= p; i++) {
    sum = 0.0;
    for (j = 1; j <= frSize - i; j++) {
      sum += s[j] * s[j + i];
    }
    if (i == 0) {
      energy = sum;
    }
    else {
      r[i] = sum;
    }
  }
  return energy;
}
/* Durbins recursion to get LP coeffs for auto values */
static double Durbin(float* k, float* thisA, const float* r, double E, int order)
{
  float* newA = NULL;
  double ki; /* Current Reflection Coefficient */
  int i, j;
  MYREALLOC(newA, order);
  for (i = 0; i < order; i++) {
    ki = r[i]; /* Calc next reflection coef */
    for (j = 0; j < i; j++) {
      ki = ki + thisA[j] * r[i - j];
    }
    ki = ki / E;
    if (k != NULL) {
      k[i] = ki;
    }
    E *= 1 - ki * ki; /* Update Error */
    newA[i] = -ki; /* Calc new filter coef */
    for (j = 0; j < i; j++) {
      newA[j] = thisA[j] - ki * thisA[i - j];
    }
    for (j = 0; j <= i; j++) {
      thisA[j] = newA[j];
    }
  }
  FREE(newA);
  return(E);
}
/* EXPORT->Wave2LPC: Calculate LPCoef in a & RefC in k */
int Wave2LPC(int ns, const float* s, int na, float* a, int nk, float* k, float* re, float* te)
{
  float* _thisA = NULL;
  float* thisA = a; /* Current LP filter coefficients */
  float* r; /* AutoCorrelation Sequence */
  double E; /* Prediction Error */
  int p, frSize;
  if (a == NULL && k == NULL) {
    HError("Wave2LPC: Null a and k vectors in WaveToLPC");
  }
  if (a != NULL) {
    p = na;
  }
  else {
    p = nk;
  }
  MYREALLOC(r, p);
  thisA = (a != NULL) ? a : 0;
  if (NULL == thisA) {
    MYREALLOC(_thisA, p);
    thisA = _thisA;
  }
  frSize = ns;
  E = AutoCorrelate(s, r, p, frSize);
  *te = E;
  *re = Durbin(k, thisA, r, E, p);
  FREE(r);
  FREE(_thisA);
  return 0;
}
/* EXPORT->LPC2RefC: transfer from filter to ref coef */
int LPC2RefC(int na, const float* a, float* k)
{
  float* thisA = NULL; /* Current LP filter coefficients */
  float* newA = NULL; /* New LP filter coefficients */
  int i, j, p;
  double ki, x;
  p = na;
  MYREALLOC(thisA, p);
  MYREALLOC(newA, p);
  MEMCPY(thisA, a, p);
  for (i = p - 1; i >= 0; i--) {
    ki = -thisA[i];
    k[i] = ki;
    x = 1 - ki * ki;
    for (j = 0; j < i; j++) {
      newA[j] = (thisA[j] + ki * thisA[i - j]) / x;
    }
    for (j = 0; j < i; j++) {
      thisA[j] = newA[j];
    }
  }
  FREE(thisA);
  FREE(newA);
  return 0;
}
/* EXPORT->RefC2LPC: transfer from ref coef to filter */
int RefC2LPC(int nk, const float* k, float* a)
{
  float* thisA = NULL; /* Current LP filter coefficients */
  float* newA = NULL; /* New LP filter coefficients */
  int i, j, p;
  double ki;
  p = nk;
  MYREALLOC(thisA, p);
  MYREALLOC(newA, p);
  for (i = 0; i < p; i++) {
    ki = k[i];
    newA[i] = -ki;
    for (j = 0; j < i; j++) {
      newA[j] = thisA[j] - ki * thisA[i - j];
    }
    for (j = 0; j <= i; j++) {
      thisA[j] = newA[j];
    }
  }
  for (i = 0; i < p; i++) {
    a[i] = thisA[i];
  }
  FREE(thisA);
  FREE(newA);
  return 0;
}
/* EXPORT->LPC2Cepstrum: transfer from lpc to cepstral coef */
int LPC2Cepstrum(int nc, const float* a, float* c)
{
  int i, n, p;
  double sum;
  p = nc;
  for (n = 0; n < p; n++) {
    sum = 0.0;
    for (i = 0; i < n; i++) {
      sum = sum + (n - i) * a[i] * c[n - i];
    }
    c[n] = -(a[n] + sum / n);
  }
  return 0;
}
/* EXPORT->Cepstrum2LPC: transfer from cepstral coef to lpc */
int Cepstrum2LPC(int na, const float* c, float* a)
{
  int i, n, p;
  double sum;
  p = na;
  for (n = 0; n < p; n++) {
    sum = 0.0;
    for (i = 0; i < (n - 1); i++) {
      sum = sum + (n - i - 1) * a[i] * c[n - i - 1];
    }
    a[n - 1] = -(c[n - 1] + sum / n);
  }
  return 0;
}
/* -- FFT Based Operations -- */
/* EXPORT-> FVec2Spectrum: cvt feature vector f to a spectrum, fzero
 is the value of the 0'th feature vector coefficient which
 is typically omitted by HSigP routines eg a0 = 1.0 for LPC
*/
/* EXPORT-> FFT: apply fft/invfft to complex s */
int FFT1(int ns, float* s, int invert)
{
  int ii, jj, n, nn, limit, m, j, inc, i;
  double wx, wr, wpr, wpi, wi, theta;
  double xre, xri, x;
  n = ns;
  nn = n / 2;
  j = 0;
  for (ii = 0; ii < nn; ii++) {
    i = 2 * ii;
    if (j > i) {
      xre = s[j];
      xri = s[j + 1];
      ASSERT(j < ns);
      s[j] = s[i];
      s[j + 1] = s[i + 1];
      s[i] = xre;
      s[i + 1] = xri;
    }
    m = n / 2;
    while (m > 0 && j >= m) {
      j -= m;
      m /= 2;
    }
    j += m;
  };
  limit = 2;
  while (limit < n) {
    inc = 2 * limit;
    theta = TPI / limit;
    if (invert) {
      theta = -theta;
    }
    x = sin(0.5 * theta);
    wpr = -2.0 * x * x;
    wpi = sin(theta);
    wr = 1.0;
    wi = 0.0;
    for (ii = 0; ii < limit / 2; ii++) {
      m = 2 * ii;
      for (jj = 0; jj < (n - m) / inc; jj++) {
        i = m + jj * inc;
        j = i + limit;
        xre = wr * s[j] - wi * s[j + 1];
        xri = wr * s[j + 1] + wi * s[j];
        s[j] = s[i] - xre;
        s[j + 1] = s[i + 1] - xri;
        s[i] = s[i] + xre;
        s[i + 1] = s[i + 1] + xri;
      }
      wx = wr;
      wr = wr * wpr - wi * wpi + wr;
      wi = wi * wpr + wx * wpi + wi;
    }
    //cvShowMat_f4(s, ns, 1); cvWaitKey(-1);
    limit = inc;
  }
  if (invert) {
    for (i = 0; i < n; i++) {
      s[i] = s[i] / nn;
    }
  }
  return 0;
}
int FFT(int ns, float* s, int invert)
{
  int ii, jj, n, nn, limit, m, j, inc, i;
  double wx, wr, wpr, wpi, wi, theta;
  double xre, xri, x;
  --s;
  n = ns;
  nn = n / 2;
  j = 1;
  for (ii = 1; ii <= nn; ii++) {
    i = 2 * ii - 1;
    if (j > i) {
      xre = s[j];
      xri = s[j + 1];
      s[j] = s[i];
      s[j + 1] = s[i + 1];
      s[i] = xre;
      s[i + 1] = xri;
    }
    m = n / 2;
    while (m >= 2 && j > m) {
      j -= m;
      m /= 2;
    }
    j += m;
  };
  limit = 2;
  while (limit < n) {
    inc = 2 * limit;
    theta = TPI / limit;
    if (invert) {
      theta = -theta;
    }
    x = sin(0.5 * theta);
    wpr = -2.0 * x * x;
    wpi = sin(theta);
    wr = 1.0;
    wi = 0.0;
    for (ii = 1; ii <= limit / 2; ii++) {
      m = 2 * ii - 1;
      for (jj = 0; jj <= (n - m) / inc; jj++) {
        i = m + jj * inc;
        j = i + limit;
        xre = wr * s[j] - wi * s[j + 1];
        xri = wr * s[j + 1] + wi * s[j];
        s[j] = s[i] - xre;
        s[j + 1] = s[i + 1] - xri;
        s[i] = s[i] + xre;
        s[i + 1] = s[i + 1] + xri;
      }
      wx = wr;
      wr = wr * wpr - wi * wpi + wr;
      wi = wi * wpr + wx * wpi + wi;
    }
    limit = inc;
  }
  if (invert) {
    for (i = 1; i <= n; i++) {
      s[i] = s[i] / nn;
    }
  }
  return 0;
}
/* EXPORT-> Realft: apply fft to real s */
int Realft(int ns, float* s)
{
  int n, n2, i, i1, i2, i3, i4;
  double xr1, xi1, xr2, xi2, wrs, wis;
  double yr, yi, yr2, yi2, yr0, theta, x;
  n = ns / 2;
  n2 = n / 2;
  theta = PI / n;
  FFT(ns, s, FALSE);
  --s;
  x = sin(0.5 * theta);
  yr2 = -2.0 * x * x;
  yi2 = sin(theta);
  yr = 1.0 + yr2;
  yi = yi2;
  for (i = 2; i <= n2; i++) {
    i1 = i + i - 1;
    i2 = i1 + 1;
    i3 = n + n + 3 - i2;
    i4 = i3 + 1;
    wrs = yr;
    wis = yi;
    xr1 = (s[i1] + s[i3]) / 2.0;
    xi1 = (s[i2] - s[i4]) / 2.0;
    xr2 = (s[i2] + s[i4]) / 2.0;
    xi2 = (s[i3] - s[i1]) / 2.0;
    s[i1] = xr1 + wrs * xr2 - wis * xi2;
    s[i2] = xi1 + wrs * xi2 + wis * xr2;
    s[i3] = xr1 - wrs * xr2 + wis * xi2;
    s[i4] = -xi1 + wrs * xi2 + wis * xr2;
    yr0 = yr;
    yr = yr * yr2 - yi * yi2 + yr;
    yi = yi * yr2 + yr0 * yi2 + yi;
  }
  xr1 = s[1];
  s[1] = xr1 + s[2];
  s[2] = 0.0;
  return 0;
}
int FVec2Spectrum(double fzero, int nf, const float* f, int ns, float* s)
{
  int i, p, n;
  p = nf;
  n = ns;
  s[0] = fzero;
  for (i = 0; i < p; i++) {
    s[i + 1] = f[i];
  }
  for (i = p + 1; i < n; i++) {
    s[i] = 0.0;
  }
  Realft(ns, s);
  return 0;
}
/* EXPORT-> SpecModulus: store modulus of s in m */
int SpecModulus(int ns, const float* s, float* m)
{
  int i, j;
  double x, y;
  for (i = 0; i < ns / 2; i++) {
    j = i + i;
    x = s[j];
    y = s[j + 1];
    m[i] = sqrt(x * x + y * y);
  }
  return 0;
}
/* EXPORT-> SpecLogModulus: store log modulus of s in m */
int SpecLogModulus(int ns, const float* s, float* m, BOOL invert)
{
  int i, j;
  double x, y;
  for (i = 0; i < ns / 2; i++) {
    j = i + i;
    x = s[j];
    y = s[j + 1];
    x = 0.5 * log(x * x + y * y);
    m[i] = invert ? -x : x;
  }
  return 0;
}
/* EXPORT-> SpecPhase: store phase of s in m */
int SpecPhase(int ns, const float* s, float* m)
{
  int i, j;
  double ph, re, im;
  for (i = 0; i < ns / 2; i++) {
    j = i + i;
    re = s[j];
    im = s[j + 1];
    if (re == 0.0) {
      ph = (im >= 0.0) ? PI / 2.0 : -PI / 2.0;
    }
    else {
      ph = atan(im / re);
      if (ph < 0.0 && re < 0.0) {
        ph += PI;
      }
      else if (ph > 0.0 && im < 0.0) {
        ph -= PI;
      }
    }
    m[i] = ph;
  }
  return 0;
}
/* -- MFCC Related Operations -- */
/* EXPORT->Mel: return mel-frequency corresponding to given FFT index */
float Mel(int k, float fres)
{
  return 1127 * log(1 + (k - 1) * fres);
}
/* EXPORT->WarpFreq: return warped frequency */
double WarpFreq(double fcl, double fcu, double freq, double minFreq, double maxFreq , double alpha)
{
  if (alpha == 1.0) {
    return freq;
  }
  else {
    double scale = 1.0 / alpha;
    double cu = fcu * 2 / (1 + scale);
    double cl = fcl * 2 / (1 + scale);
    double au = (maxFreq - cu * scale) / (maxFreq - cu);
    double al = (cl * scale - minFreq) / (cl - minFreq);
    if (freq > cu) {
      return au * (freq - cu) + scale * cu ;
    }
    else if (freq < cl) {
      return al * (freq - minFreq) + minFreq ;
    }
    else {
      return scale * freq ;
    }
  }
  return 0;
}
typedef IOConfigRec IOConfigRec;
/* EXPORT->InitFBank: Initialise an IOConfigRec record */
int InitFBank(IOConfigRec* cf)
{
  float mlo, mhi, ms, melk;
  int k, chan, maxChan, Nby2;
  int sampPeriod = (int)cf->srcSampRate;
  float lopass = cf->loFBankFreq, hipass = cf->hiFBankFreq;
  float alpha = cf->warpFreq, warpLowCut = cf->warpLowerCutOff, warpUpCut = cf->warpUpperCutOff;
  /* Save sizes to cross-check subsequent usage */
  //cf->frSize = frSize;
  //cf->numChans = numChans;
  //cf->sampPeriod = sampPeriod;
  //cf->usePower = usePower;
  //cf->takeLogs = takeLogs;
  /* Calculate required FFT size */
  cf->fftN = 2;
  while (cf->frSize > cf->fftN) {
    cf->fftN *= 2;
  }
  if (cf->doubleFFT) {
    cf->fftN *= 2;
  }
  Nby2 = cf->fftN / 2;
  cf->fres = 1.0E7 / (sampPeriod * cf->fftN * 700.0);
  maxChan = cf->numChans + 1;
  /* set lo and hi pass cut offs if any */
  cf->klo = 2;
  cf->khi = Nby2; /* apply lo/hi pass filtering */
  mlo = 0;
  mhi = Mel(Nby2 + 1, cf->fres);
  if (lopass >= 0.0) {
    mlo = 1127 * log(1 + lopass / 700.0);
    cf->klo = (int)((lopass * sampPeriod * 1.0e-7 * cf->fftN) + 2.5);
    if (cf->klo < 2) {
      cf->klo = 2;
    }
  }
  if (hipass >= 0.0) {
    mhi = 1127 * log(1 + hipass / 700.0);
    cf->khi = (int)((hipass * sampPeriod * 1.0e-7 * cf->fftN) + 0.5);
    if (cf->khi > Nby2) {
      cf->khi = Nby2;
    }
  }
  if (0) {
    printf("FFT passband %d to %d out of 1 to %d\n", cf->klo, cf->khi, Nby2);
    printf("Mel passband %f to %f\n", mlo, mhi);
  }
  /* Create vector of fbank centre frequencies */
  MYREALLOC(cf->cf, maxChan);
  ms = mhi - mlo;
  for (chan = 0; chan < maxChan; chan++) {
    if (alpha == 1.0) {
      cf->cf[chan] = ((double)(chan + 1) / (double)maxChan) * ms + mlo;
    }
    else {
      /* scale assuming scaling starts at lopass */
      double minFreq = 700.0 * (exp(mlo / 1127.0) - 1.0);
      double maxFreq = 700.0 * (exp(mhi / 1127.0) - 1.0);
      double cf1 = ((double)chan / (double) maxChan) * ms + mlo;
      cf1 = 700 * (exp(cf1 / 1127.0) - 1.0);
      cf->cf[chan] = 1127.0 * log(1.0 + WarpFreq(warpLowCut, warpUpCut, cf1, minFreq, maxFreq, alpha) / 700.0);
    }
  }
  /* Create loChan map, loChan[fftindex] -> lower channel index */
  MYREALLOC(cf->loChan, Nby2);
  for (k = 0, chan = 0; k < Nby2; k++) {
    melk = Mel(k + 1, cf->fres);
    if (k < (cf->klo - 1) || k >= cf->khi) {
      cf->loChan[k] = -1;
    }
    else {
      while (cf->cf[chan] < melk && chan < maxChan) {
        ++chan;
      }
      cf->loChan[k] = chan;
    }
  }
  /* Create vector of lower channel weights */
  MYREALLOC(cf->loWt, Nby2);
  for (k = 0; k < Nby2; k++) {
    chan = cf->loChan[k];
    if (k < (cf->klo - 1) || k >= cf->khi) {
      cf->loWt[k] = 0.0;
    }
    else {
      if (chan > 0) {
        cf->loWt[k] = ((cf->cf[chan] - Mel(k + 1, cf->fres)) / (cf->cf[chan] - cf->cf[chan - 1]));
      }
      else {
        cf->loWt[k] = (cf->cf[0] - Mel(k + 1, cf->fres)) / (cf->cf[0] - mlo);
      }
    }
  }
  /* Create workspace for fft */
  MYREALLOC(cf->x, cf->fftN);
  return 0;
}
/* EXPORT->Wave2FBank: Perform filterbank analysis on speech s */
int Wave2FBank(const IOConfigRec* cf, const float* s, float* fbank, float* te)
{
  const double melfloor = 1.0;
  int k, bin;
  double t1, t2; /* real and imag parts */
  double ek; /* energy of k'th fft channel */
  /* Check that cf record is compatible */
  //if(cf->frSize != ns) HError("Wave2FBank: frame size mismatch");
  //if(cf->numChans != nfbank) HError("Wave2FBank: num channels mismatch");
  /* Compute frame energy if needed */
  if (te != NULL) {
    *te = 0.0;
    for (k = 0; k < cf->frSize; k++) {
      *te += (s[k] * s[k]);
    }
  }
  /* Apply FFT */
  for (k = 0; k < cf->frSize; k++) {
    cf->x[k] = s[k]; /* copy to workspace */
  }
  for (k = cf->frSize; k < cf->fftN; k++) {
    cf->x[k] = 0.0; /* pad with zeroes */
  }
  Realft(cf->fftN, cf->x); /* take fft */
  //cvShowMat_f4(cf->x, cf->fftN, 1); cvWaitKey(-1);
  /* Fill filterbank channels */
  for (k = 0; k < cf->numChans; ++k) {
    fbank[k] = 0.0;
  }
  for (k = cf->klo - 1; k < cf->khi; k++) { /* fill bins */
    t1 = cf->x[2 * k];
    t2 = cf->x[2 * k + 1];
    if (cf->usePower) {
      ek = t1 * t1 + t2 * t2;
    }
    else {
      ek = sqrt(t1 * t1 + t2 * t2);
    }
    bin = cf->loChan[k];
    t1 = cf->loWt[k] * ek;
    if (bin > 0) {
      fbank[bin - 1] += t1;
    }
    if (bin < cf->numChans) {
      fbank[bin] += ek - t1;
    }
  }
  //cvShowMat_f4(fbank, cf->numChans, 1); cvWaitKey(-1);
  /* Take logs */
  if (cf->takeLogs) {
    for (bin = 0; bin < cf->numChans; bin++) {
      t1 = fbank[bin];
      if (t1 < melfloor) {
        t1 = melfloor;
      }
      fbank[bin] = log(t1);
    }
  }
  return 0;
}
/* EXPORT->FBank2MFCC: compute first n cepstral coeff */
int FBank2MFCC(int nfbank, const float* fbank, float* c, int n)
{
  int j, k, numChan;
  float mfnorm, pi_factor, x;
  numChan = nfbank;
  mfnorm = sqrt(2.0 / (float)numChan);
  pi_factor = PI / (float)numChan;
  for (j = 0; j < n; j++) {
    c[j] = 0.0;
    x = (float)(j + 1) * pi_factor;
    for (k = 0; k < numChan; k++) {
      c[j] += fbank[k] * cos(x * (k + 0.5));
    }
    c[j] *= mfnorm;
  }
  return 0;
}
/* EXPORT->FBank2MelSpec: convert log fbank to linear */
int FBank2MelSpec(int nfbank, float* fbank)
{
  int i;
  for (i = 0; i < nfbank; i++) {
    fbank[i] = exp(fbank[i]);
  }
  return 0;
}
/* EXPORT->MelSpec2FBank: convert lin mel spectrum to log fbank */
int MelSpec2FBank(int nmelspec, float* melspec)
{
  int i;
  double x;
  for (i = 1; i < nmelspec; i++) {
    x = melspec[i];
    if (x < 1.0) {
      x = 1.0;
    }
    melspec[i] = log(x);
  }
  return 0;
}
/* EXPORT->FBank2C0: return zero'th cepstral coefficient */
float FBank2C0(int nfbank, const float* fbank)
{
  int k, numChan;
  float mfnorm, sum;
  numChan = nfbank;
  mfnorm = sqrt(2.0 / (float)numChan);
  sum = 0.0;
  for (k = 0; k < numChan; k++) {
    sum += fbank[k];
  }
  return sum * mfnorm;
}
/* -- PLP Related Operations -- */
/* EXPORT->InitPLP: Initialise equal-loudness curve & IDT cosine matrix */
int InitPLP(const IOConfigRec* cf, int lpcOrder, float* eql, float* cm)
{
  int i, j;
  double baseAngle;
  double f_hz_mid, fsub, fsq;
  int nAuto, nFreq;
  int cm_step = cf->numChans;
  /* Create the equal-loudness curve */
  for (i = 0; i < cf->numChans; i++) {
    f_hz_mid = 700 * (exp(cf->cf[i] / 1127) - 1); /* Mel to Hz conversion */
    fsq = (f_hz_mid * f_hz_mid);
    fsub = fsq / (fsq + 1.6e5);
    eql[i] = fsub * fsub * ((fsq + 1.44e6) / (fsq + 9.61e6));
  }
  /* Builds up matrix of cosines for IDFT */
  nAuto = lpcOrder + 1;
  nFreq = cf->numChans + 1;
  baseAngle = PI / (double)(nFreq);
  for (i = 0; i < nAuto; i++) {
    cm[i * cm_step + 0] = 1.0;
    for (j = 0; j < cf->numChans; j++) {
      cm[i * cm_step + j] = 2.0 * cos(baseAngle * (double)i * (double)j);
    }
    cm[i * cm_step + nFreq - 1] = cos(baseAngle * (double)i * (double)(nFreq));
  }
  return 0;
}
/* EXPORT->FBank2ASpec: Pre-emphasise filter bank output with the simulated
 equal-loudness curve and perform amplitude compression */
int FBank2ASpec(float* fbank, float* as, const float* eql, double compressFact,
    const IOConfigRec* cf)
{
  const double melfloor = 1.0;
  int i;
  for (i = 0; i < cf->numChans; i++) {
    if (fbank[i] < melfloor) {
      fbank[i] = melfloor;
    }
    as[i + 1] = fbank[i] * eql[i]; /* Apply equal-loudness curve */
    as[i + 1] = pow((double) as[i + 1], (double) compressFact);
  }
  as[0] = as[1]; /* Duplicate values at either end */
  as[cf->numChans + 1] = as[cf->numChans];
  return 0;
}
/* Matrix IDFT converts from auditory spectrum into autocorrelation values */
double MatrixIDFT(int nas, const float* as, int nac, float* ac, const float* cm, int cm_step)
{
  double acc;
  double E;
  int nAuto, nFreq;
  int i, j;
  nFreq = nas;
  nAuto = nac;
  E = 0.0;
  for (i = 0; i < nAuto; i++) {
    acc = cm[i * cm_step] * as[0];
    for (j = 1; j < nFreq; j++) {
      acc += cm[i * cm_step + j] * as[j];
    }
    if (i > 0) {
      ac[i] = (double)(acc / (double)(2.0 * (nFreq - 1)));
    }
    else {
      E = (double)(acc / (double)(2.0 * (nFreq - 1)));
    }
  }
  return E; /* Return zero'th auto value separately */
}
/* EXPORT->ASpec2LPCep: Perform IDFT to get autocorrelation values then
 produce autoregressive coeffs. and cepstral transform them */
int ASpec2LPCep(int nas, const float* as, int nac, float* ac, int nlp, float* lp, int nc, float* c, float* cm, int cm_step)
{
  double lpcGain, E;
  /* Do IDFT to get autocorrelation values */
  E = MatrixIDFT(nas, as, nac, ac, cm, cm_step);
  lp[nlp - 1] = 0.0; /* init to make Purify et al. happy */
  /* do Durbin recursion to get predictor coefficients */
  lpcGain = Durbin(NULL, lp, ac, E, nac - 1);
  if (lpcGain <= 0) {
    HError("ASpec2LPCep: Negative lpcgain");
  }
  LPC2Cepstrum(nlp, lp, c);
  c[nc - 1] = (double) - log((double) 1.0 / lpcGain); /* value forms C0 */
  return 0;
}
/* -- Feature Level Operations -- */
/* GenCepWin: generate a new cep liftering vector */
static int GenCepWin(int cepLiftering, int count, float* cepWin)
{
  int i;
  double a, Lby2;
  a = PI / cepLiftering;
  Lby2 = cepLiftering / 2.0;
  for (i = 0; i < count; i++) {
    cepWin[i] = 1.0 + Lby2 * sin((i + 1) * a);
  }
  //cepWinL = cepLiftering;
  //cepWinSize = count;
  return count;
}
/* EXPORT->WeightCepstrum: Apply cepstral weighting to c */
int WeightCepstrum(float* c, int start, int count, const float* cepWin)
{
  int i, j;
  j = start;
  for (i = 0; i < count; i++) {
    c[j++] *= cepWin[i];
  }
  return 0;
}
/* EXPORT->UnWeightCepstrum: Undo cepstral weighting of c */
int UnWeightCepstrum(float* c, int start, int count, int cepLiftering, const float* cepWin)
{
  int i, j;
  j = start;
  for (i = 0; i < count; i++) {
    c[j++] /= cepWin[i];
  }
  return 0;
}
/* The following operations apply to a sequence of n vectors step apart.
 They are used to operate on the 'columns' of data files
 containing a sequence of feature vectors packed together to form a
 continguous block of floats. The logical size of each vector is
 vSize(<=step) */
/* EXPORT->FZeroMean: Zero mean the given data sequence */
int FZeroMean(float* data, int vSize, int n, int step)
{
  double sum;
  float* fp, mean;
  int i, j;
  for (i = 0; i < vSize; i++) {
    /* find mean over i'th component */
    sum = 0.0;
    fp = data + i;
    for (j = 0; j < n; j++) {
      sum += *fp;
      fp += step;
    }
    mean = sum / (double)n;
    /* subtract mean from i'th components */
    fp = data + i;
    for (j = 0; j < n; j++) {
      *fp -= mean;
      fp += step;
    }
  }
  return 0;
}
/* Regression: add regression vector at +offset from source vector. If head
 or tail is less than delwin then duplicate first/last vector to compensate */
static int Regress(float* data, int vSize, int n, int step, int offset,
    int delwin, int head, int tail, BOOL simpleDiffs)
{
  float* fp, *fp1, *fp2, *back, *forw;
  double sum, sigmaT2;
  int i, t, j;
  sigmaT2 = 0.0;
  for (t = 1; t <= delwin; t++) {
    sigmaT2 += t * t;
  }
  sigmaT2 *= 2.0;
  fp = data;
  for (i = 1; i <= n; i++) {
    fp1 = fp;
    fp2 = fp + offset;
    for (j = 1; j <= vSize; j++) {
      back = forw = fp1;
      sum = 0.0;
      for (t = 1; t <= delwin; t++) {
        if (head + i - t > 0) {
          back -= step;
        }
        if (tail + n - i + 1 - t > 0) {
          forw += step;
        }
        if (!simpleDiffs) {
          sum += t * (*forw - *back);
        }
      }
      if (simpleDiffs) {
        *fp2 = (*forw - *back) / (2 * delwin);
      }
      else {
        *fp2 = sum / sigmaT2;
      }
      ++fp1;
      ++fp2;
    }
    fp += step;
  }
  return 0;
}
/* EXPORT->AddRegression: add regression vector at +offset from source vector */
int AddRegression(float* data, int vSize, int n, int step, int offset,
    int delwin, int head, int tail, BOOL simpleDiffs)
{
  Regress(data, vSize, n, step, offset, delwin, head, tail, simpleDiffs);
  return 0;
}
/* EXPORT->AddHeadRegress: add regression at start of data */
int AddHeadRegress(float* data, int vSize, int n, int step, int offset,
    int delwin, BOOL simpleDiffs)
{
  float* fp, *fp1, *fp2;
  int i, j;
  fp = data;
  if (delwin == 0) {
    for (i = 1; i <= n; i++) {
      fp1 = fp;
      fp2 = fp + offset;
      for (j = 1; j <= vSize; j++) {
        *fp2 = *(fp1 + step) - *fp1;
        ++fp1;
        ++fp2;
      }
      fp += step;
    }
  }
  else {
    Regress(data, vSize, n, step, offset, delwin, 0, delwin, simpleDiffs);
  }
  return 0;
}
/* EXPORT->AddTailRegress: add regression at end of data */
int AddTailRegress(float* data, int vSize, int n, int step, int offset,
    int delwin, BOOL simpleDiffs)
{
  float* fp, *fp1, *fp2;
  int i, j;
  fp = data;
  if (delwin == 0) {
    for (i = 1; i <= n; i++) {
      fp1 = fp;
      fp2 = fp + offset;
      for (j = 1; j <= vSize; j++) {
        *fp2 = *fp1 - *(fp1 - step);
        ++fp1;
        ++fp2;
      }
      fp += step;
    }
  }
  else {
    Regress(data, vSize, n, step, offset, delwin, delwin, 0, simpleDiffs);
  }
  return 0;
}
/* EXPORT->NormaliseLogEnergy: normalise log energy to range -X .. 1.0 */
int NormaliseLogEnergy(float* data, int n, int step, double silFloor, double escale)
{
  float* p, max, min;
  int i;
  /* find max log energy */
  p = data;
  max = *p;
  for (i = 1; i < n; i++) {
    p += step; /* step p to next e val */
    if (*p > max) {
      max = *p;
    }
  }
  min = max - (silFloor * log(10.0)) / 10.0; /* set the silence floor */
  /* normalise */
  p = data;
  for (i = 0; i < n; i++) {
    if (*p < min) {
      *p = min; /* clamp to silence floor */
    }
    *p = 1.0 - (max - *p) * escale; /* normalise */
    p += step;
  }
  return 0;
}

