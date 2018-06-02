
typedef struct gauss3_coefs {
  double b[10];
  double B, sigma;
  int N;
} gauss3_coefs;
/*
 * Calculate the coefficients for the recursive filter algorithm
 * Fast Computation of gaussian blurring.
 */
static void compute_coefs3(gauss3_coefs* c, double sigma)
{
  /*
   * Papers:  "Recursive Implementation of the gaussian filter.",
   *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
   * formula: 11b       computation of q
   *          8c        computation of b0..b1
   *          10        alpha is normalization constant B
   */
  double q, q2, q3;

  q = 0;

  if (sigma >= 2.5) {
    q = 0.98711 * sigma - 0.96330;
  }
  else if ((sigma >= 0.5) && (sigma < 2.5)) {
    q = 3.97156 - 4.14554 * (double) sqrt((double) 1 - 0.26891 * sigma);
  }
  else {
    q = 0.1147705018520355224609375;
  }

  q2 = q * q;
  q3 = q * q2;
  c->b[0] = (1.57825 + (2.44413 * q) + (1.4281 * q2) + (0.422205 * q3));
  c->b[1] = ((2.44413 * q) + (2.85619 * q2) + (1.26661 * q3));
  c->b[2] = (-((1.4281 * q2) + (1.26661 * q3)));
  c->b[3] = ((0.422205 * q3));
  c->B = 1.0 - ((c->b[1] + c->b[2] + c->b[3]) / c->b[0]);
  c->sigma = sigma;
  c->N = 3;

}

// Papers:  "Recursive Implementation of the gaussian filter.",
//          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
// formula: 9a        forward filter
//          9b        backward filter
//          fig7      algorithm
static void gausssmooth(double* in, double* out, int size, int rowstride, gauss3_coefs* c)
{
  int i, n, bufsize;
  double* w1, *w2;

  /* forward pass */
  bufsize = size + 3;
  size -= 1;
  w1 = (double*)pmalloc(bufsize * sizeof(double));
  w2 = (double*)pmalloc(bufsize * sizeof(double));
  w1[0] = in[0];
  w1[1] = in[0];
  w1[2] = in[0];

  for (i = 0 , n = 3; i <= size ; i++, n++) {
    w1[n] = (double)(c->B * in[i * rowstride] +
        ((c->b[1] * w1[n - 1] +
            c->b[2] * w1[n - 2] +
            c->b[3] * w1[n - 3]) / c->b[0]));
  }

  /* backward pass */
  w2[size + 1] = w1[size + 3];
  w2[size + 2] = w1[size + 3];
  w2[size + 3] = w1[size + 3];

  for (i = size, n = i; i >= 0; i--, n--) {
    w2[n] = out[i * rowstride] = (double)(c->B * w1[n] +
        ((c->b[1] * w2[n + 1] +
            c->b[2] * w2[n + 2] +
            c->b[3] * w2[n + 3]) / c->b[0]));
  }

  pfree(w1);
  pfree(w2);
}

// Filtering (smoothing) Gaussian recursive.
int blur_guass(int height, int width, const unsigned char* src, int srcstep, unsigned char* dst, int dststep, double sigma)
{
  int row, col, i, pos, n = height * width;
  gauss3_coefs coef[1] = {0};
  double* dsrc = MALLOC(double, n);
  double* ddst = MALLOC(double, n);
  compute_coefs3(coef, sigma);

  for (row = 0 ; row < height; row++) {
    pos = row * width;

    for (i = 0; i < width; ++i) {
      (dsrc + pos)[i] = src[row * srcstep + i];
    }

    gausssmooth(dsrc + pos, ddst + pos, width, 1, coef);
  }

  memcpy(dsrc, ddst, n * sizeof(double));
  memset(ddst, 0   , n * sizeof(double));

  for (col = 0; col < width; col++) {
    pos = col;
    gausssmooth(dsrc + pos, ddst + pos, height, width, coef);
  }

  for (row = 0 ; row < height; row++) {
    pos = row * width;

    for (i = 0; i < width; ++i) {
      dst[row * dststep + i] = (int)BOUND((ddst + pos)[i], 0, 255);
    }
  }

  pfree(dsrc);
  pfree(ddst);
  return 0;
}