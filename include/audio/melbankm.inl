#define melbankm_flag_t 1<<1
#define melbankm_flag_n 1<<2
#define melbankm_flag_m 1<<3
#define melbankm_flag_z 1<<4
#define melbankm_flag_y 1<<5
#define melbankm_flag_nor 1<<6
// Inputs: p number of filters in filterbank
// n length of fft
// fs sample rate in Hz
// fl low end of the lowest filter as a fraction of fs (default = 0)
// fh high end of highest filter as a fraction of fs (default = 0.5)
// w any sensible combination of the following:
// 't' triangular shaped filters in mel domain (default)
// 'n' hanning shaped filters in mel domain
// 'm' hamming shaped filters in mel domain
//
// 'z' highest and lowest filters taper down to zero (default)
// 'y' lowest filter remains at 1 down to 0 frequency and
// highest filter remains at 1 up to nyquist freqency
//
// If 'ty' or 'ny' is specified, the total power in the fft is preserved.
//
// Outputs: x a sparse matrix containing the filterbank amplitudes
// If x is the only output argument then size(x)=[p,1+floor(n/2)]
// otherwise size(x)=[p,mx-mn+1]
// mn the lowest fft bin with a non-zero coefficient
// mx the highest fft bin with a non-zero coefficient
//
// Usage: f=fft(s); f=fft(s);
// x=melbankm(p,n,fs); [x,na,nb]=melbankm(p,n,fs);
// n2=1+floor(n/2); z=log(x*(f(na:nb)).*conj(f(na:nb)));
// z=log(x*abs(f(1:n2)).^2);
// c=dct(z); c(1)=[];
//
// To plot filterbanks e.g. plot(melbankm(20,256,8000)')
int melbankm(int p, int n, int fs, int fl, double fh, int w, img_t* x, int* pmn, int* pmx)
{
  double f0, lr;
  double bl[4];
  enum {nn = 300};
  double pf[nn];
  double pm[nn];
  double v[nn];
  int fp[nn];
  int r[nn];
  int c[nn];
  int fn2, i, j, b1, b2, b3, b4, nv, mn, mx;
  if (0 == w) {
    w = melbankm_flag_t | melbankm_flag_z;
  }
  if (fh == 0) {
    fh = 0.5;
  }
  if (fl < 0) {
    fl = 0;
  }
  f0 = 700. / fs;
  fn2 = floor(n / 2);
  lr = log((f0 + fh) / (f0 + fl)) / (p + 1);
  // convert to fft bin numbers with 0 for DC term
  {
    double aa[] = {0, 1, p, p + 1};
    for (i = 0; i < 4; ++i) {
      bl[i] = n * ((f0 + fl) * exp(aa[i] * lr) - f0);
    }
  }
  b2 = ceil(bl[1]);
  b3 = floor(bl[2]);
  if (w & melbankm_flag_y) {
#if 0
pf = log((f0 + (b2: b3) / n) / (f0 + fl)) / lr;
    fp = floor(pf);
    r = [ones(1, b2) fp fp + 1 p * ones(1, fn2 - b3)];
c = [1: b3 + 1 b2 + 1: fn2 + 1];
    v = 2 * [0.5 ones(1, b2 - 1) 1 - pf + fp pf - fp ones(1, fn2 - b3 - 1) 0.5];
    mn = 1;
    mx = fn2 + 1;
#endif
  }
  else {
    int k2, k3, k4;
    b1 = floor(bl[0]) + 1;
    b4 = min(fn2, ceil(bl[3])) - 1;
    k2 = b2 - b1 + 1;
    k3 = b3 - b1 + 1;
    k4 = b4 - b1 + 1;
    mn = b1 + 1;
    mx = b4 + 1;
    nv = k4 - k2 + 1 + k3;
    imsetsize(x, p, 1 + fn2, sizeof(double), 1);
    for (i = 0; i <= (b4 - b1); ++i) {
      pf[i] = log((f0 + (i + b1) * 1. / n) / (f0 + fl)) / lr;
      fp[i] = (int)floor(pf[i]);
      pm[i] = pf[i] - fp[i];
    }
    j = 0;
    for (i = k2 - 1; i < k4; ++i) {
      c[j] = BOUND(i, 0, x->w - 1);
      r[j] = fp[i] - 1;
      r[j] = BOUND(r[j], 0, x->h - 1);
      v[j] = 2 * (1 - pm[i]);
      ++j;
    }
    for (i = 0; i < k3; ++i) {
      c[j] = BOUND(i, 0, x->w - 1);
      r[j] = BOUND(fp[i], 0, x->h - 1);
      v[j] = 2 * (pm[i]);
      ++j;
    }
    nv = j;
#if 0
r = [fp(k2: k4) 1 + fp(1: k3)];
c = [k2: k4 1: k3];
v = 2 * [1 - pm(k2: k4) pm(1: k3)];
#endif
    mn = b1 + 1;
    mx = b4 + 1;
  }
  if (w & melbankm_flag_n) {
    //v = 1 - cos(v * pi / 2);
    for (i = 0; i < nv; ++i) {
      v[i] = 1 - cos(v[i] * PI / 2);
    }
  }
  else if (w & melbankm_flag_m) {
    //v = 1 - 0.92 / 1.08 * cos(v * pi / 2);
    for (i = 0; i < nv; ++i) {
      v[i] = 1 - (0.92 / 1.08) * cos(v[i] * PI / 2);
    }
  }
  if (w & melbankm_flag_nor) {
    double vmax = v[0];
    for (i = 1; i < nv; ++i) {
      if (v[i] > vmax) {
        vmax = v[i];
      }
    }
    for (i = 0; i < nv; ++i) {
      v[i] /= vmax;
    }
  }
  if (pmx) {
    *pmx = mx;
  }
  if (pmn) {
    *pmn = mn;
  }
  if (0) {
    //x = sparse(r, c, v);
  }
  else {
    //x = sparse(r, c + mn - 1, v, p, 1 + fn2);
    for (i = 0; i < nv; ++i) {
      x->tt.f8[r[i]*x->w + c[i] + mn - 1] = v[i];
    }
  }
  return 0;
}

