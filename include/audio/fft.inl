#define CV_MAX_LOCAL_DFT_SIZE (1 << 15)
static const uchar log2tab[] = { 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 };
static int icvlog2(int n)
{
  int m = 0;
  int f = (n >= (1 << 16)) * 16;
  n >>= f;
  m += f;
  f = (n >= (1 << 8)) * 8;
  n >>= f;
  m += f;
  f = (n >= (1 << 4)) * 4;
  n >>= f;
  return m + f + log2tab[n];
}
static unsigned char icvRevTable[] = {
  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};
static const double icvDxtTab[][2] = {
  { 1.00000000000000000, 0.00000000000000000 },
  { -1.00000000000000000, 0.00000000000000000 },
  { 0.00000000000000000, 1.00000000000000000 },
  { 0.70710678118654757, 0.70710678118654746 },
  { 0.92387953251128674, 0.38268343236508978 },
  { 0.98078528040323043, 0.19509032201612825 },
  { 0.99518472667219693, 0.09801714032956060 },
  { 0.99879545620517241, 0.04906767432741802 },
  { 0.99969881869620425, 0.02454122852291229 },
  { 0.99992470183914450, 0.01227153828571993 },
  { 0.99998117528260111, 0.00613588464915448 },
  { 0.99999529380957619, 0.00306795676296598 },
  { 0.99999882345170188, 0.00153398018628477 },
  { 0.99999970586288223, 0.00076699031874270 },
  { 0.99999992646571789, 0.00038349518757140 },
  { 0.99999998161642933, 0.00019174759731070 },
  { 0.99999999540410733, 0.00009587379909598 },
  { 0.99999999885102686, 0.00004793689960307 },
  { 0.99999999971275666, 0.00002396844980842 },
  { 0.99999999992818922, 0.00001198422490507 },
  { 0.99999999998204725, 0.00000599211245264 },
  { 0.99999999999551181, 0.00000299605622633 },
  { 0.99999999999887801, 0.00000149802811317 },
  { 0.99999999999971945, 0.00000074901405658 },
  { 0.99999999999992983, 0.00000037450702829 },
  { 0.99999999999998246, 0.00000018725351415 },
  { 0.99999999999999567, 0.00000009362675707 },
  { 0.99999999999999889, 0.00000004681337854 },
  { 0.99999999999999978, 0.00000002340668927 },
  { 0.99999999999999989, 0.00000001170334463 },
  { 1.00000000000000000, 0.00000000585167232 },
  { 1.00000000000000000, 0.00000000292583616 }
};
#define icvBitRev(i,shift) \
  ((int)((((unsigned)icvRevTable[(i)&255] << 24)+ \
      ((unsigned)icvRevTable[((i)>> 8)&255] << 16)+ \
      ((unsigned)icvRevTable[((i)>>16)&255] << 8)+ \
      ((unsigned)icvRevTable[((i)>>24)])) >> (shift)))
static int icvDFTFactorize(int n, int* factors)
{
  int nf = 0, f, i, j;
  if (n <= 5) {
    factors[0] = n;
    return 1;
  }
  f = (((n - 1) ^ n) + 1) >> 1;
  if (f > 1) {
    factors[nf++] = f;
    n = f == n ? 1 : n / f;
  }
  for (f = 3; n > 1;) {
    int d = n / f;
    if (d * f == n) {
      factors[nf++] = f;
      n = d;
    }
    else {
      f += 2;
      if (f * f > n) {
        break;
      }
    }
  }
  if (n > 1) {
    factors[nf++] = n;
  }
  f = (factors[0] & 1) == 0;
  for (i = f; i < (nf + f) / 2; i++) {
    CV_SWAP(factors[i], factors[nf - i - 1 + f], j);
  }
  return nf;
}
static int icvDFTInit(int n0, int nf, const int* factors, int* itab, CvComplex64f* wave, int inv_itab)
{
  int digits[34], radix[34];
  int n = factors[0], m = 0;
  int* itab0 = itab;
  int i, j, k;
  CvComplex64f w, w1;
  double t;
  if (n0 <= 5) {
    itab[0] = 0;
    itab[n0 - 1] = n0 - 1;
    if (n0 != 4) {
      for (i = 1; i < n0 - 1; i++) {
        itab[i] = i;
      }
    }
    else {
      itab[1] = 2;
      itab[2] = 1;
    }
    if (n0 == 5) {
      wave[0] = cvComplex64f(1., 0.);
    }
    if (n0 != 4) {
      return 0;
    }
    m = 2;
  }
  else {
    radix[nf] = 1;
    digits[nf] = 0;
    for (i = 0; i < nf; i++) {
      digits[i] = 0;
      radix[nf - i - 1] = radix[nf - i] * factors[nf - i - 1];
    }
    if (inv_itab && factors[0] != factors[nf - 1]) {
      itab = (int*)wave;
    }
    if ((n & 1) == 0) {
      int a = radix[1], na2 = n * a >> 1, na4 = na2 >> 1;
      m = icvlog2(n);
      if (n <= 2) {
        itab[0] = 0;
        itab[1] = na2;
      }
      else if (n <= 256) {
        int shift = 10 - m;
        for (i = 0; i <= n - 4; i += 4) {
          j = (icvRevTable[i >> 2] >> shift) * a;
          itab[i] = j;
          itab[i + 1] = j + na2;
          itab[i + 2] = j + na4;
          itab[i + 3] = j + na2 + na4;
        }
      }
      else {
        int shift = 34 - m;
        for (i = 0; i < n; i += 4) {
          int i4 = i >> 2;
          j = icvBitRev(i4, shift) * a;
          itab[i] = j;
          itab[i + 1] = j + na2;
          itab[i + 2] = j + na4;
          itab[i + 3] = j + na2 + na4;
        }
      }
      digits[1]++;
      for (i = n, j = radix[2]; i < n0;) {
        for (k = 0; k < n; k++) {
          itab[i + k] = itab[k] + j;
        }
        if ((i += n) >= n0) {
          break;
        }
        j += radix[2];
        for (k = 1; ++digits[k] >= factors[k]; k++) {
          digits[k] = 0;
          j += radix[k + 2] - radix[k];
        }
      }
    }
    else {
      for (i = 0, j = 0;;) {
        itab[i] = j;
        if (++i >= n0) {
          break;
        }
        j += radix[1];
        for (k = 0; ++digits[k] >= factors[k]; k++) {
          digits[k] = 0;
          j += radix[k + 2] - radix[k];
        }
      }
    }
    if (itab != itab0) {
      itab0[0] = 0;
      for (i = n0 & 1; i < n0; i += 2) {
        int k0 = itab[i];
        int k1 = itab[i + 1];
        itab0[k0] = i;
        itab0[k1] = i + 1;
      }
    }
  }
  if ((n0 & (n0 - 1)) == 0) {
    w.re = w1.re = icvDxtTab[m][0];
    w.im = w1.im = -icvDxtTab[m][1];
  }
  else {
    t = -CV_PI * 2 / n0;
    w.im = w1.im = sin(t);
    w.re = w1.re = sqrt(1. - w1.im * w1.im);
  }
  n = (n0 + 1) / 2;
  wave[0].re = 1.;
  wave[0].im = 0.;
  if ((n0 & 1) == 0) {
    wave[n].re = -1.;
    wave[n].im = 0;
  }
  for (i = 1; i < n; i++) {
    wave[i] = w;
    wave[n0 - i].re = w.re;
    wave[n0 - i].im = -w.im;
    t = w.re * w1.re - w.im * w1.im;
    w.im = w.re * w1.im + w.im * w1.re;
    w.re = t;
  }
  return 0;
}
#define ICV_DFT_NO_PERMUTE 2
#define ICV_DFT_COMPLEX_INPUT_OR_OUTPUT 4
// mixed-radix complex discrete Fourier transform: double-precision version
static int icvDFT_64fc(CvComplex64f* dst, int n, int nf, int* factors, const int* itab,
    const CvComplex64f* wave, int tab_size, const void* spec, CvComplex64f* buf,
    int flags, double scale)
{
  static const double icv_sin_120 = 0.86602540378443864676372317075294;
  static const double icv_sin_45 = 0.70710678118654752440084436210485;
  static const double icv_fft5_2 = 0.559016994374947424102293417182819;
  static const double icv_fft5_3 = -0.951056516295153572116439333379382;
  static const double icv_fft5_4 = -1.538841768587626701285145288018455;
  static const double icv_fft5_5 = 0.363271264002680442947733378740309;
  int n0 = n, f_idx, nx;
  int inv = flags & CV_DXT_INVERSE;
  int dw0 = tab_size, dw;
  int i, j, k;
  CvComplex64f t;
  int tab_step;
  tab_step = tab_size == n ? 1 : tab_size == n * 2 ? 2 : tab_size / n;
  // 0. shuffle data
  if ((flags & ICV_DFT_NO_PERMUTE) == 0) {
    if (factors[0] != factors[nf - 1]) {
      return CV_INPLACE_NOT_SUPPORTED_ERR;
    }
    if (nf == 1) {
      if ((n & 3) == 0) {
        int n2 = n / 2;
        CvComplex64f* dsth = dst + n2;
        for (i = 0; i < n2; i += 2, itab += tab_step * 2) {
          j = itab[0];
          assert((unsigned)j < (unsigned)n2);
          CV_SWAP(dst[i + 1], dsth[j], t);
          if (j > i) {
            CV_SWAP(dst[i], dst[j], t);
            CV_SWAP(dsth[i + 1], dsth[j + 1], t);
          }
        }
      }
      // else do nothing
    }
    else {
      for (i = 0; i < n; i++, itab += tab_step) {
        j = itab[0];
        assert((unsigned)j < (unsigned)n);
        if (j > i) {
          CV_SWAP(dst[i], dst[j], t);
        }
      }
    }
  }
  if (inv) {
    for (i = 0; i <= n - 2; i += 2) {
      double t0 = -dst[i].im;
      double t1 = -dst[i + 1].im;
      dst[i].im = t0;
      dst[i + 1].im = t1;
    }
    if (i < n) {
      dst[n - 1].im = -dst[n - 1].im;
    }
  }
  n = 1;
  // 1. power-2 transforms
  if ((factors[0] & 1) == 0) {
    // radix-4 transform
    for (; n * 4 <= factors[0];) {
      nx = n;
      n *= 4;
      dw0 /= 4;
      for (i = 0; i < n0; i += n) {
        CvComplex64f* v0;
        CvComplex64f* v1;
        double r0, i0, r1, i1, r2, i2, r3, i3, r4, i4;
        v0 = dst + i;
        v1 = v0 + nx * 2;
        r2 = v0[0].re;
        i2 = v0[0].im;
        r1 = v0[nx].re;
        i1 = v0[nx].im;
        r0 = r1 + r2;
        i0 = i1 + i2;
        r2 -= r1;
        i2 -= i1;
        i3 = v1[nx].re;
        r3 = v1[nx].im;
        i4 = v1[0].re;
        r4 = v1[0].im;
        r1 = i4 + i3;
        i1 = r4 + r3;
        r3 = r4 - r3;
        i3 = i3 - i4;
        v0[0].re = r0 + r1;
        v0[0].im = i0 + i1;
        v1[0].re = r0 - r1;
        v1[0].im = i0 - i1;
        v0[nx].re = r2 + r3;
        v0[nx].im = i2 + i3;
        v1[nx].re = r2 - r3;
        v1[nx].im = i2 - i3;
        for (j = 1, dw = dw0; j < nx; j++, dw += dw0) {
          v0 = dst + i + j;
          v1 = v0 + nx * 2;
          r2 = v0[nx].re * wave[dw * 2].re - v0[nx].im * wave[dw * 2].im;
          i2 = v0[nx].re * wave[dw * 2].im + v0[nx].im * wave[dw * 2].re;
          r0 = v1[0].re * wave[dw].im + v1[0].im * wave[dw].re;
          i0 = v1[0].re * wave[dw].re - v1[0].im * wave[dw].im;
          r3 = v1[nx].re * wave[dw * 3].im + v1[nx].im * wave[dw * 3].re;
          i3 = v1[nx].re * wave[dw * 3].re - v1[nx].im * wave[dw * 3].im;
          r1 = i0 + i3;
          i1 = r0 + r3;
          r3 = r0 - r3;
          i3 = i3 - i0;
          r4 = v0[0].re;
          i4 = v0[0].im;
          r0 = r4 + r2;
          i0 = i4 + i2;
          r2 = r4 - r2;
          i2 = i4 - i2;
          v0[0].re = r0 + r1;
          v0[0].im = i0 + i1;
          v1[0].re = r0 - r1;
          v1[0].im = i0 - i1;
          v0[nx].re = r2 + r3;
          v0[nx].im = i2 + i3;
          v1[nx].re = r2 - r3;
          v1[nx].im = i2 - i3;
        }
      }
    }
    for (; n < factors[0];) {
      // do the remaining radix-2 transform
      nx = n;
      n *= 2;
      dw0 /= 2;
      for (i = 0; i < n0; i += n) {
        CvComplex64f* v = dst + i;
        double r0 = v[0].re + v[nx].re;
        double i0 = v[0].im + v[nx].im;
        double r1 = v[0].re - v[nx].re;
        double i1 = v[0].im - v[nx].im;
        v[0].re = r0;
        v[0].im = i0;
        v[nx].re = r1;
        v[nx].im = i1;
        for (j = 1, dw = dw0; j < nx; j++, dw += dw0) {
          v = dst + i + j;
          r1 = v[nx].re * wave[dw].re - v[nx].im * wave[dw].im;
          i1 = v[nx].im * wave[dw].re + v[nx].re * wave[dw].im;
          r0 = v[0].re;
          i0 = v[0].im;
          v[0].re = r0 + r1;
          v[0].im = i0 + i1;
          v[nx].re = r0 - r1;
          v[nx].im = i0 - i1;
        }
      }
    }
  }
  // 2. all the other transforms
  for (f_idx = (factors[0] & 1) ? 0 : 1; f_idx < nf; f_idx++) {
    int factor = factors[f_idx];
    nx = n;
    n *= factor;
    dw0 /= factor;
    if (factor == 3) {
      // radix-3
      for (i = 0; i < n0; i += n) {
        CvComplex64f* v = dst + i;
        double r1 = v[nx].re + v[nx * 2].re;
        double i1 = v[nx].im + v[nx * 2].im;
        double r0 = v[0].re;
        double i0 = v[0].im;
        double r2 = icv_sin_120 * (v[nx].im - v[nx * 2].im);
        double i2 = icv_sin_120 * (v[nx * 2].re - v[nx].re);
        v[0].re = r0 + r1;
        v[0].im = i0 + i1;
        r0 -= 0.5 * r1;
        i0 -= 0.5 * i1;
        v[nx].re = r0 + r2;
        v[nx].im = i0 + i2;
        v[nx * 2].re = r0 - r2;
        v[nx * 2].im = i0 - i2;
        for (j = 1, dw = dw0; j < nx; j++, dw += dw0) {
          v = dst + i + j;
          r0 = v[nx].re * wave[dw].re - v[nx].im * wave[dw].im;
          i0 = v[nx].re * wave[dw].im + v[nx].im * wave[dw].re;
          i2 = v[nx * 2].re * wave[dw * 2].re - v[nx * 2].im * wave[dw * 2].im;
          r2 = v[nx * 2].re * wave[dw * 2].im + v[nx * 2].im * wave[dw * 2].re;
          r1 = r0 + i2;
          i1 = i0 + r2;
          r2 = icv_sin_120 * (i0 - r2);
          i2 = icv_sin_120 * (i2 - r0);
          r0 = v[0].re;
          i0 = v[0].im;
          v[0].re = r0 + r1;
          v[0].im = i0 + i1;
          r0 -= 0.5 * r1;
          i0 -= 0.5 * i1;
          v[nx].re = r0 + r2;
          v[nx].im = i0 + i2;
          v[nx * 2].re = r0 - r2;
          v[nx * 2].im = i0 - i2;
        }
      }
    }
    else if (factor == 5) {
      // radix-5
      for (i = 0; i < n0; i += n) {
        for (j = 0, dw = 0; j < nx; j++, dw += dw0) {
          CvComplex64f* v0 = dst + i + j;
          CvComplex64f* v1 = v0 + nx * 2;
          CvComplex64f* v2 = v1 + nx * 2;
          double r0, i0, r1, i1, r2, i2, r3, i3, r4, i4, r5, i5;
          r3 = v0[nx].re * wave[dw].re - v0[nx].im * wave[dw].im;
          i3 = v0[nx].re * wave[dw].im + v0[nx].im * wave[dw].re;
          r2 = v2[0].re * wave[dw * 4].re - v2[0].im * wave[dw * 4].im;
          i2 = v2[0].re * wave[dw * 4].im + v2[0].im * wave[dw * 4].re;
          r1 = r3 + r2;
          i1 = i3 + i2;
          r3 -= r2;
          i3 -= i2;
          r4 = v1[nx].re * wave[dw * 3].re - v1[nx].im * wave[dw * 3].im;
          i4 = v1[nx].re * wave[dw * 3].im + v1[nx].im * wave[dw * 3].re;
          r0 = v1[0].re * wave[dw * 2].re - v1[0].im * wave[dw * 2].im;
          i0 = v1[0].re * wave[dw * 2].im + v1[0].im * wave[dw * 2].re;
          r2 = r4 + r0;
          i2 = i4 + i0;
          r4 -= r0;
          i4 -= i0;
          r0 = v0[0].re;
          i0 = v0[0].im;
          r5 = r1 + r2;
          i5 = i1 + i2;
          v0[0].re = r0 + r5;
          v0[0].im = i0 + i5;
          r0 -= 0.25 * r5;
          i0 -= 0.25 * i5;
          r1 = icv_fft5_2 * (r1 - r2);
          i1 = icv_fft5_2 * (i1 - i2);
          r2 = -icv_fft5_3 * (i3 + i4);
          i2 = icv_fft5_3 * (r3 + r4);
          i3 *= -icv_fft5_5;
          r3 *= icv_fft5_5;
          i4 *= -icv_fft5_4;
          r4 *= icv_fft5_4;
          r5 = r2 + i3;
          i5 = i2 + r3;
          r2 -= i4;
          i2 -= r4;
          r3 = r0 + r1;
          i3 = i0 + i1;
          r0 -= r1;
          i0 -= i1;
          v0[nx].re = r3 + r2;
          v0[nx].im = i3 + i2;
          v2[0].re = r3 - r2;
          v2[0].im = i3 - i2;
          v1[0].re = r0 + r5;
          v1[0].im = i0 + i5;
          v1[nx].re = r0 - r5;
          v1[nx].im = i0 - i5;
        }
      }
    }
    else {
      // radix-"factor" - an odd number
      int p, q, factor2 = (factor - 1) / 2;
      int d, dd, dw_f = tab_size / factor;
      CvComplex64f* a = buf;
      CvComplex64f* b = buf + factor2;
      for (i = 0; i < n0; i += n) {
        for (j = 0, dw = 0; j < nx; j++, dw += dw0) {
          CvComplex64f* v = dst + i + j;
          CvComplex64f v_0 = v[0];
          CvComplex64f vn_0 = v_0;
          if (j == 0) {
            for (p = 1, k = nx; p <= factor2; p++, k += nx) {
              double r0 = v[k].re + v[n - k].re;
              double i0 = v[k].im - v[n - k].im;
              double r1 = v[k].re - v[n - k].re;
              double i1 = v[k].im + v[n - k].im;
              vn_0.re += r0;
              vn_0.im += i1;
              a[p - 1].re = r0;
              a[p - 1].im = i0;
              b[p - 1].re = r1;
              b[p - 1].im = i1;
            }
          }
          else {
            const CvComplex64f* wave_ = wave + dw * factor;
            d = dw;
            for (p = 1, k = nx; p <= factor2; p++, k += nx, d += dw) {
              double r2 = v[k].re * wave[d].re - v[k].im * wave[d].im;
              double i2 = v[k].re * wave[d].im + v[k].im * wave[d].re;
              double r1 = v[n - k].re * wave_[-d].re - v[n - k].im * wave_[-d].im;
              double i1 = v[n - k].re * wave_[-d].im + v[n - k].im * wave_[-d].re;
              double r0 = r2 + r1;
              double i0 = i2 - i1;
              r1 = r2 - r1;
              i1 = i2 + i1;
              vn_0.re += r0;
              vn_0.im += i1;
              a[p - 1].re = r0;
              a[p - 1].im = i0;
              b[p - 1].re = r1;
              b[p - 1].im = i1;
            }
          }
          v[0] = vn_0;
          for (p = 1, k = nx; p <= factor2; p++, k += nx) {
            CvComplex64f s0 = v_0, s1 = v_0;
            d = dd = dw_f * p;
            for (q = 0; q < factor2; q++) {
              double r0 = wave[d].re * a[q].re;
              double i0 = wave[d].im * a[q].im;
              double r1 = wave[d].re * b[q].im;
              double i1 = wave[d].im * b[q].re;
              s1.re += r0 + i0;
              s0.re += r0 - i0;
              s1.im += r1 - i1;
              s0.im += r1 + i1;
              d += dd;
              d -= -(d >= tab_size) & tab_size;
            }
            v[k] = s0;
            v[n - k] = s1;
          }
        }
      }
    }
  }
  if (fabs(scale - 1.) > DBL_EPSILON) {
    double re_scale = scale, im_scale = scale;
    if (inv) {
      im_scale = -im_scale;
    }
    for (i = 0; i < n0; i++) {
      double t0 = dst[i].re * re_scale;
      double t1 = dst[i].im * im_scale;
      dst[i].re = t0;
      dst[i].im = t1;
    }
  }
  else if (inv) {
    for (i = 0; i <= n0 - 2; i += 2) {
      double t0 = -dst[i].im;
      double t1 = -dst[i + 1].im;
      dst[i].im = t0;
      dst[i + 1].im = t1;
    }
    if (i < n0) {
      dst[n0 - 1].im = -dst[n0 - 1].im;
    }
  }
  return CV_OK;
}
int cvfft(int n, CvComplex64f* x)
{
  int factors[34] = {0};
  int nf = icvDFTFactorize(n, factors);
  int* itab = NULL;
  CvComplex64f* w = NULL;
  void* spec = NULL;
  MYREALLOC(w, n);
  MYREALLOC(itab, n);
  icvDFTInit(n, nf, factors, itab, w, 1);
  icvDFT_64fc(x, n, nf, factors, itab, w, n, spec, 0, 0, 1);
  FREE(w);
  FREE(itab);
  return 0;
}
void conjugate_complex(int n, CvComplex64f in[], CvComplex64f out[])
{
  int i = 0;
  for (i = 0; i < n; i++) {
    out[i].im = -in[i].im;
    out[i].re = in[i].re;
  }
}
void Wn_i(int n, int i, CvComplex64f* Wn, char flag)
{
  Wn->re = cos(2 * PI * i / n);
  if (flag == 1) {
    Wn->im = -sin(2 * PI * i / n);
  }
  else if (flag == 0) {
    Wn->im = -sin(2 * PI * i / n);
  }
}
//傅里叶变化
int fft(int N, CvComplex64f* f)
{
  CvComplex64f t, wn; //中间变量
  int i, j, k, m, n, l, r, M;
  int la, lb, lc;
  // 计算分解的级数M=log2(N)
  for (i = N, M = 1; (i = i / 2) != 1; M++);
  // ?凑盏刮恍蛑匦屡帕性藕?
  for (i = 1, j = N / 2; i <= N - 2; i++) {
    if (i < j) {
      t = f[j];
      f[j] = f[i];
      f[i] = t;
    }
    k = N / 2;
    while (k <= j) {
      j = j - k;
      k = k / 2;
    }
    j = j + k;
  }
  // FFT算法
  for (m = 1; m <= M; m++) {
    la = 1 << m; //la=2^m代表第m级每个分组所含节点数
    lb = la / 2; //lb代表第m级每个分组所含碟形单元数
    //同时它也表示每个碟形单元上下节点之间的距离
    // 碟形运算
    for (l = 1; l <= lb; l++) {
      r = (l - 1) * pow(2, M - m);
      Wn_i(N, r, &wn, 1); //wn=Wnr
      for (n = l - 1; n < N - 1; n = n + la) { //遍历每个分组，分组总数为N/la
        lc = n + lb; //n,lc分别代表一个碟形单元的上、下节点编号
        CvComplex_mul(t, f[lc], wn); //t = f[lc] * wn复???
        CvComplex_sub(f[lc], f[n], t); //f[lc] = f[n] - f[lc] * Wnr
        CvComplex_add(f[n], f[n], t); //f[n] = f[n] + f[lc] * Wnr
      }
    }
  }
  return 0;
}
//傅里叶逆变换
void ifft(int N, CvComplex64f* f)
{
  int i = 0;
  conjugate_complex(N, f, f);
  fft(N, f);
  conjugate_complex(N, f, f);
  for (i = 0; i < N; i++) {
    f[i].im = (f[i].im) / N;
    f[i].re = (f[i].re) / N;
  }
}
int fourier(int n, CvComplex64f* x)
{
  int i, j;
  CvComplex64f* y = NULL, *w = NULL, wi, t1;
  MYREALLOC(y, 2 * n);
  w = y + n;
  for (i = 0; i < n; ++i) {
    double a = (i) * (2 * PI) / n;
    CvComplex_set(w[i], cos(a), -sin(a));
  }
  for (i = 0; i < n; ++i) {
    CvComplex_set(t1, 0, 0);
    for (j = 0; j < n; ++j) {
      wi = w[(i * j) % n];
      CvComplex_muladd(t1, x[j], wi);
    }
    y[i] = t1;
  }
  MEMCPY(x, y, n);
  FREE(y);
  return 0;
}
int fft_abs_b2(int m, int n, double* x, int xl, int xlm)
{
  CvComplex64f* cx = NULL;
  int i, j;
  MYREALLOC(cx, n);
  for (j = 0; j < m; ++j) {
    for (i = 0; i < n; ++i) {
      CvComplex_set(cx[i], x[i * xl], 0);
    }
    //fft_b2(n, cx);
    fft(n, cx);
    //fourier(n, cx);
    for (i = 0; i < n; ++i) {
      x[i * xl] = CvComplex_abs2(cx[i]);
    }
    x += xlm;
  }
  FREE(cx);
  return 0;
}
int test_fft()
{
  int i, j = 0, t = 0;
  if (1) {
    CvComplex64f* x0 = NULL;
    CvComplex64f* x1 = NULL;
    CvComplex64f* x2 = NULL;
    CvComplex64f t0;
    int n = 1 << 12;
    double sum = 0;
    MYREALLOC(x0, n * 3);
    x1 = x0 + n;
    x2 = x0 + n * 2;
    for (j = 0; j < 100; ++j) {
      for (i = 0; i < n; ++i) {
        CvComplex_set(x0[i], rand() * 1. / RAND_MAX, rand() * 1. / RAND_MAX);
        x2[i] = x1[i] = x0[i];
      }
      {
        utime_start(_start_time);
        fourier(n, x0);
        printf("%f ", utime_elapsed(_start_time));
      }
      {
        utime_start(_start_time);
        cvfft(n, x1);
        printf("%f ", utime_elapsed(_start_time));
      }
      {
        utime_start(_start_time);
        fft(n, x2);
        printf("%f ", utime_elapsed(_start_time));
      }
      for (i = 0; i < n; ++i) {
        CvComplex_sub(t0, x0[i], x1[i]);
        sum += CvComplex_abs2(t0);
        CvComplex_sub(t0, x0[i], x2[i]);
        sum += CvComplex_abs2(t0);
      }
      printf("%f\n", sum);
    }
    printf("\n");
  }
  if (0) {
    enum {N = 8};
    double x[N * N] = { 0 };
    double result[N * 4] = { 0 };
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; j++) {
        x[i * N + j] = j + 1;
      }
    }
    fft_abs_b2(N, N, x, 1, N);
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; j++) {
        printf("%8.5g,", x[i * N + j]);
      }
      printf("\n");
    }
  }
  return 0;
}

