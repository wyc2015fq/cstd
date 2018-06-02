/*====================================================================
| Version: May 14, 2012
\===================================================================*/

/*====================================================================
| Philippe Thevenaz
| EPFL/STI/IMT/LIB/BM4137
| Station 17
| CH-1015 Lausanne VD
| Switzerland
|
| phone (CET): +41(21)693.51.61
| fax: +41(21)693.68.10
| RFC-822: philippe.thevenaz@epfl.ch
| X-400: /C=ch/A=400net/P=switch/O=epfl/S=thevenaz/G=philippe/
| URL: http://bigwww.epfl.ch/
\===================================================================*/

/*====================================================================
| This work is based on the following paper:
|
| P. Thevenaz, D. Sage, M. Unser
| Bi-Exponential Edge-Preserving Smoother
| IEEE Transactions on Image Processing, in press
|
| Other relevant on-line publications are available at
| http://bigwww.epfl.ch/publications/
\===================================================================*/

/*====================================================================
| Additional help available at http://bigwww.epfl.ch/
|
| You'll be free to use this software for research purposes, but you
| should not redistribute it without our consent. In addition, we
| expect you to include a citation or acknowledgment whenever you
| present or publish results that are based on it. EPFL makes no
| warranties of any kind on this software and shall in no event be
| liable for damages of any kind in connection with the use and
| exploitation of this technology.
\===================================================================*/


// BEEPS_
// begin class BEEPS_
// protected variables
double BEEPS__ZETA_3 =
    1.2020569031595942853997381615114499907649862923404988817922715553418382057;



// ExtendedPlugInFilter methods

/* begin class BEEPSGain */
//variables
typedef struct BEEPSGain_t {
  double mu;
} BEEPSGain_t; /* end class BEEPSGain */

// Runnable methods
int BEEPSGain_setup(BEEPSGain_t* p, double spatialContraDecay)
{
  p->mu = (1.0 - spatialContraDecay) / (1.0 + spatialContraDecay);
  return 0;
}
int BEEPSGain_run(BEEPSGain_t* p, double* data, int startIndex, int length)
{
  int k, K;

  for (k = startIndex, K = startIndex + length; (k < K); k++) {
    data[k] *= p->mu;
  }

  return 0;
} /* end run */


// BEEPSProgressive
typedef struct BEEPSProgressive_t {
  double c;
  double rho;
  double spatialContraDecay;
  int rangeFilter;
} BEEPSProgressive_t;

int BEEPSProgressive_setup(BEEPSProgressive_t* p, int sharedRangeFilter, double photometricStandardDeviation, double sharedSpatialContraDecay)
{
  p->rangeFilter = sharedRangeFilter;
  p->spatialContraDecay = sharedSpatialContraDecay;
  p->rho = 1.0 + p->spatialContraDecay;

  switch (p->rangeFilter) {
  case 0: { // gauss
    p->c = -0.5 / (photometricStandardDeviation * photometricStandardDeviation);
    break;
  }

  case 1: { // sech
    p->c = PI / (2.0 * photometricStandardDeviation);
    break;
  }

  case 2: { // tanh
    p->c = pow((0.75 * BEEPS__ZETA_3) / (photometricStandardDeviation * photometricStandardDeviation), 1.0 / 3.0);
    p->c *= (photometricStandardDeviation < 0.0) ? (-1.0) : ((0.0 == photometricStandardDeviation) ? (0.0) : (1.0));
    break;
  }

  case 3: { // oneSided
    p->c = pow((0.75 * BEEPS__ZETA_3) / (photometricStandardDeviation * photometricStandardDeviation), 1.0 / 3.0);
    p->c *= (photometricStandardDeviation < 0.0) ? (-1.0) : ((0.0 == photometricStandardDeviation) ? (0.0) : (1.0));
    break;
  }
  }

  return 0;
}

int BEEPSProgressive_run(BEEPSProgressive_t* p, double* data, int startIndex, int length)
{
  double mu = 0.0;
  int k, K;
  data[startIndex] /= p->rho;

  switch (p->rangeFilter) {
  case 0: { // gauss
    for (k = startIndex + 1, K = startIndex + length;
        (k < K); k++) {
      mu = data[k] - p->rho * data[k - 1];
      mu = p->spatialContraDecay * exp(p->c * mu * mu);
      data[k] = data[k - 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }

  case 1: { // sech
    for (k = startIndex + 1, K = startIndex + length;
        (k < K); k++) {
      mu = p->spatialContraDecay
          / cosh(p->c * (data[k] - p->rho * data[k - 1]));
      data[k] = data[k - 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }

  case 2: { // tanh
    for (k = startIndex + 1, K = startIndex + length;
        (k < K); k++) {
      mu = data[k] - p->rho * data[k - 1];
      mu = p->spatialContraDecay * tanh(p->c * mu);
      data[k] = data[k - 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }

  case 3: { // oneSided
    for (k = startIndex + 1, K = startIndex + length;
        (k < K); k++) {
      mu = data[k] - p->rho * data[k - 1];
      mu = p->spatialContraDecay * (1.0 + tanh(p->c * mu)) * 0.5;
      data[k] = data[k - 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }
  }

  return 0;
} /* end run */

// BEEPSRegressive
// begin class BEEPSRegressive
typedef struct BEEPSRegressive_t {
  double c;
  double rho;
  double spatialContraDecay;
  int rangeFilter;
} BEEPSRegressive_t;

int BEEPSRegressive_setup(BEEPSRegressive_t* p,
    int sharedRangeFilter,
    double photometricStandardDeviation,
    double sharedSpatialContraDecay
                         )
{
  p->rangeFilter = sharedRangeFilter;
  p->spatialContraDecay = sharedSpatialContraDecay;
  p->rho = 1.0 + p->spatialContraDecay;

  switch (p->rangeFilter) {
  case 0: { // gauss
    p->c = -0.5
        / (photometricStandardDeviation * photometricStandardDeviation);
    break;
  }

  case 1: { // sech
    p->c = PI / (2.0 * photometricStandardDeviation);
    break;
  }

  case 2: { // tanh
    p->c = pow((0.75 * BEEPS__ZETA_3) / (photometricStandardDeviation
        * photometricStandardDeviation), 1.0 / 3.0);
    p->c *= (photometricStandardDeviation < 0.0) ? (-1.0)
        : ((0.0 == photometricStandardDeviation) ? (0.0) : (1.0));
    break;
  }

  case 3: { // oneSided
    p->c = pow((0.75 * BEEPS__ZETA_3) / (photometricStandardDeviation
        * photometricStandardDeviation), 1.0 / 3.0);
    p->c *= (photometricStandardDeviation < 0.0) ? (-1.0)
        : ((0.0 == photometricStandardDeviation) ? (0.0) : (1.0));
    break;
  }
  }

  return 0;
} /* end setup */


int BEEPSRegressive_run(BEEPSRegressive_t* p, double* data, int startIndex, int length)
{
  double mu = 0.0;
  int k;
  data[startIndex + length - 1] /= p->rho;

  switch (p->rangeFilter) {
  case 0: { // gauss
    for (k = startIndex + length - 2; (startIndex <= k); k--) {
      mu = data[k] - p->rho * data[k + 1];
      mu = p->spatialContraDecay * exp(p->c * mu * mu);
      data[k] = data[k + 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }

  case 1: { // sech
    for (k = startIndex + length - 2; (startIndex <= k); k--) {
      mu = p->spatialContraDecay
          / cosh(p->c * (data[k] - p->rho * data[k + 1]));
      data[k] = data[k + 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }

  case 2: { // tanh
    for (k = startIndex + length - 2; (startIndex <= k); k--) {
      mu = data[k] - p->rho * data[k + 1];
      mu = p->spatialContraDecay * tanh(p->c * mu);
      data[k] = data[k + 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }

  case 3: { // oneSided
    for (k = startIndex + length - 2; (startIndex <= k); k--) {
      mu = data[k] - p->rho * data[k + 1];
      mu = p->spatialContraDecay * (1.0 + tanh(p->c * mu)) * 0.5;
      data[k] = data[k + 1] * mu + data[k] * (1.0 - mu) / p->rho;
    }

    break;
  }
  }

  return 0;
} /* end run */

int BEEPSHorizontalVertical(float* data, int width, int height, int rangeFilter, double photometricStandardDeviation, double spatialDecay)
{
  int k, k1, k2, K, m;
  int data_length = width * height;
  BEEPSProgressive_t progressive[1] = {0};
  BEEPSGain_t gain[1] = {0};
  BEEPSRegressive_t regressive[1] = {0};
  double* g, * p, * r;

  g = MALLOC(double, width * height);
  BEEPSProgressive_setup(progressive, rangeFilter, photometricStandardDeviation, 1.0 - spatialDecay);
  BEEPSGain_setup(gain, 1.0 - spatialDecay);
  BEEPSRegressive_setup(regressive, rangeFilter, photometricStandardDeviation, 1.0 - spatialDecay);

  for (k = 0, K = data_length; (k < K); k++) {
    g[k] = (double)data[k];
  }

  p = MEMDUP(double, g, width * height);
  r = MEMDUP(double, g, width * height);

  for (k2 = 0; (k2 < height); k2++) {
    BEEPSProgressive_run(progressive, p, k2 * width, width);
    BEEPSGain_run(gain, g, k2 * width, width);
    BEEPSRegressive_run(regressive, r, k2 * width, width);
  }

  for (k = 0, K = data_length; (k < K); k++) {
    r[k] += p[k] - g[k];
  }

  m = 0;

  for (k2 = 0; (k2 < height); k2++) {
    int n = k2;

    for (k1 = 0; (k1 < width); k1++) {
      g[n] = r[m++];
      n += height;
    }
  }

  MEMCPY(p, g, width * height);
  MEMCPY(r, g, width * height);

  for (k1 = 0; (k1 < width); k1++) {
    BEEPSProgressive_run(progressive, p, k1 * height, height);
    BEEPSGain_run(gain, g, k1 * height, height);
    BEEPSRegressive_run(regressive, r, k1 * height, height);
  }

  for (k = 0, K = data_length; (k < K); k++) {
    r[k] += p[k] - g[k];
  }

  m = 0;

  for (k1 = 0; (k1 < width); k1++) {
    int n = k1;

    for (k2 = 0; (k2 < height); k2++) {
      data[n] = (float)r[m++];
      n += width;
    }
  }

  free(g);
  free(p);
  free(r);
  return 0;
} /* end run */


// BEEPSVerticalHorizontal
int BEEPSVerticalHorizontal(float* data, int width, int height, int rangeFilter, double photometricStandardDeviation, double spatialDecay)
{
  int k, k1, k2, K, m;
  int data_length = width * height;
  BEEPSProgressive_t progressive[1] = {0};
  BEEPSGain_t gain[1] = {0};
  BEEPSRegressive_t regressive[1] = {0};
  double* g, * p, * r;

  g = MALLOC(double, height * width);
  m = 0;
  BEEPSProgressive_setup(progressive, rangeFilter, photometricStandardDeviation, 1.0 - spatialDecay);
  BEEPSGain_setup(gain, 1.0 - spatialDecay);
  BEEPSRegressive_setup(regressive, rangeFilter, photometricStandardDeviation, 1.0 - spatialDecay);

  for (k2 = 0; (k2 < height); k2++) {
    int n = k2;

    for (k1 = 0; (k1 < width); k1++) {
      g[n] = (double)data[m++];
      n += height;
    }
  }

  p = MEMDUP(double, g, width * height);
  r = MEMDUP(double, g, width * height);

  for (k1 = 0; (k1 < width); k1++) {
    BEEPSProgressive_run(progressive, p, k1 * height, height);
    BEEPSGain_run(gain, g, k1 * height, height);
    BEEPSRegressive_run(regressive, r, k1 * height, height);
  }

  for (k = 0, K = data_length; (k < K); k++) {
    r[k] += p[k] - g[k];
  }

  m = 0;

  for (k1 = 0; k1 < width; k1++) {
    int n = k1;

    for (k2 = 0; k2 < height; k2++) {
      g[n] = r[m++];
      n += width;
    }
  }

  MEMCPY(p, g, width * height);
  MEMCPY(r, g, width * height);

  for (k2 = 0; (k2 < height); k2++) {
    BEEPSProgressive_run(progressive, p, k2 * width, width);
    BEEPSGain_run(gain, g, k2 * width, width);
    BEEPSRegressive_run(regressive, r, k2 * width, width);
  }

  for (k = 0, K = data_length; (k < K); k++) {
    data[k] = (float)(p[k] - g[k] + r[k]);
  }

  free(p);
  free(r);
  return 0;
}

// variables

#define error printf

int BEEPS_run(int width, int height, float* data)
{
  int i, k, K;
  int data_length = width * height;

  double spatialDecay = 0.01;
  double photometricStandardDeviation = 30.0;
  int iterations = 1;
  int rangeFilter = 0;

  if (rangeFilter < 0) {
    error("Internal error: unexpected type of range filter");
    return 0;
  }

  for (i = 0; i < iterations; i++) {
    float* duplicate = MEMDUP(float, data, data_length);
    // 对原始图像数据先进行水平迭代，再对该结果进行垂直迭代
    BEEPSHorizontalVertical(duplicate, width, height, rangeFilter, photometricStandardDeviation, spatialDecay);
    // 对原始图像数据先进行垂直迭代，再对该结果进行垂直迭代
    BEEPSVerticalHorizontal(data, width, height, rangeFilter, photometricStandardDeviation, spatialDecay);

    for (k = 0, K = data_length; (k < K); k++) {
      data[k] = 0.5F * (data[k] + duplicate[k]);
    }

    free(duplicate);
  }

  return 0;
} /* end run */

int BEEPS(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl, int cn)
{
  float* data = MALLOC(float, w * h);
  int i, j, k;

  for (k = 0; k < cn; ++k) {
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        data[i * w + j] = a[i * al + j * cn + k];
      }
    }

    BEEPS_run(w, h, data);

    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        data[i * w + j] = CLAMP255(data[i * w + j]);
        b[i * bl + j * cn + k] = (int)data[i * w + j];
      }
    }
  }

  free(data);
  return 0;
}
