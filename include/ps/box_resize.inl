
int box_resize1D_f64(int an, const double* A, int bn, double* B)
{
  int i;
  double w = bn * 1. / an;

  for (i = 0; i < bn; ++i) {
    B[i] = 0;
  }

  for (i = 0; i < an; ++i) {
    double t = i * bn * 1. / an;
    int t0 = (int)t;
    int t1 = t0 + 1;

    if (t + w <= t0 + 1) {
      B[t0] += A[i];
    }
    else {
      B[t0] += A[i] * (t1 - t) / w;

      if (t + w - t1 > 0) {
        B[t1] += A[i] * (t + w - t1) / w;
      }
    }
  }

  return 0;
}

int box_resize1D(int an, const int* A, int bn, int* B)
{
  int i;
  int w = bn * 256 / an;

  for (i = 0; i < bn; ++i) {
    B[i] = 0;
  }

  for (i = 0; i < an; ++i) {
    int t = i * bn * 256 / an;
    int t0 = t & (~255);
    int t1 = (t + 255) & (~255);

    if (t + w <= t1) {
      B[t0 >> 8] += A[i];
    }
    else {
      B[t0 >> 8] += (A[i] * (t1 - t) * an / bn) >> 8;

      if (t + w - t1 > 0) {
        B[t1 >> 8] += (A[i] * (t + w - t1) * an / bn) >> 8;
      }
    }
  }

  return 0;
}

int box_resize1Da(int an, const int* A, int bn, int* B)
{
  int i, j = 0;

  for (i = 0; i < bn; ++i) {
    B[i] = 0;
  }

  for (i = 0; i < an * bn;) {
    if (i == j) {
      if (an < bn) {
        B[j / an] += A[i / bn];
      }
      else {
        B[j / an] += A[i / bn];
      }
    }

    i += bn, j += an;
  }

  return 0;
}



int box_resize(int ah, int aw, const unsigned char* A, int al, int bh, int bw, int* B, int bl)
{
  int i, j = 0;

  for (i = 0; i < ah; ++i) {

  }

  return 0;
}


int test_box_resize()
{
  {
    double a[10] = {0};
    double b[10] = {0};
    int i;

    for (i = 0; i < 10; ++i) {
      a[i] = i;
    }

    box_resize1D_f64(10, a, 3, b);
    box_resize1D_f64(10, a, 3, b);
  }
  {
    int a[10] = {0};
    int b[10] = {0};
    int i;

    for (i = 0; i < 10; ++i) {
      a[i] = i;
    }

    box_resize1D(10, a, 3, b);
    box_resize1D(10, a, 3, b);
  }
  return 0;
}
