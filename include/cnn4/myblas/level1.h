static void FUN(set)(int N, const Dtype alpha, Dtype* Y) {
  int i = 0;
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (; i < N-3; i+=4) {
    Y[i+0] = alpha;
    Y[i+1] = alpha;
    Y[i+2] = alpha;
    Y[i+3] = alpha;
  }
  for (; i < N; ++i) {
    Y[i] = alpha;
  }
}

static void FUN(copy)(int N, const Dtype *X, int incx, Dtype *Y, int incy) {
  int i = 0;
  if (incx == 1 && incy == 1) {
    for (; i < N - 3; i += 4) {
      Y[i + 0] = X[i + 0];
      Y[i + 1] = X[i + 1];
      Y[i + 2] = X[i + 2];
      Y[i + 3] = X[i + 3];
    }
  }
  else {
    for (; i < N - 3; i += 4) {
      Y[(i + 0)*incy] = X[(i + 0)*incx];
      Y[(i + 1)*incy] = X[(i + 1)*incx];
      Y[(i + 2)*incy] = X[(i + 2)*incx];
      Y[(i + 3)*incy] = X[(i + 3)*incx];
    }
  }
  for (; i < N; ++i) {
    Y[i*incy] = X[i*incx];
  }
}
static void FUN(pow)(int n, const Dtype* a, const Dtype b, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { pow(a[i], b); }
}
static void FUN(sqr)(int n, const Dtype* a, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] * a[i]; }
}
static void FUN(sqrt)(int n, const Dtype* a, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = sqrt(a[i]); }
}
static void FUN(exp)(int n, const Dtype* a, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = exp(a[i]); }
}
static void FUN(log)(int n, const Dtype* a, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = log(a[i]); }
}
static void FUN(abs)(int n, const Dtype* a, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = fabs(a[i]); }
}
static void FUN(add)(int n, const Dtype* a, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] + b[i]; }
}
static void FUN(sub)(int n, const Dtype* a, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] - b[i]; }
}
static void FUN(mul)(int n, const Dtype* a, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] * b[i]; }
}
static void FUN(div)(int n, const Dtype* a, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(n>0); ASSERT(a); ASSERT(y);
  for (; i < n; ++i) { y[i] = a[i] / b[i]; }
}
static Dtype FUN(dot)(int n, const Dtype *x, int incx, const Dtype *y, int incy) {
  Dtype sum=0;
  int i=0;
  for (; i<n; ++i) {
    sum += x[i] * y[i];
  }
  return sum;
}

static Stype FUN(asum)(int n, const Dtype *X, int incx) {
  Stype sum=0;
  int i=0;
  if (1 == incx) {
    for (; i < n; ++i) {
      sum += ABS(X[i]);
    }
  }
  else {
    for (; i < n; ++i) {
      sum += ABS(X[i*incx]);
    }
  }
  return sum;
}
static double FUN(euclidean)(int n, const Dtype* a, const Dtype* b) {
  double sum = 0;
  int i;
  for (i=0; i<n; ++i) {
    Dtype t = a[i] - b[i];
    sum += t*t;
  }
  return sum;
}
static Dtype FUN(min)(int n, const Dtype* a) {
  int i;
  Dtype _min;
  if (n<=0) return 0;
  _min = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]<_min) {
      _min = a[i];
    }
  }
  return _min;
}
static int FUN(argmin)(int n, const Dtype* a) {
  int i;
  int _min;
  if (n<=0) return -1;
  _min = 0;
  for (i=1; i<n; ++i) {
    if (a[i]<a[_min]) {
      _min = i;
    }
  }
  return _min;
}
static Dtype FUN(max)(int n, const Dtype* a) {
  int i;
  Dtype _max;
  if (n<=0) return 0;
  _max = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]>_max) {
      _max = a[i];
    }
  }
  return _max;
}
static int FUN(argmax)(int n, const Dtype* a) {
  int i;
  int _max;
  if (n<=0) return -1;
  _max = 0;
  for (i=1; i<n; ++i) {
    if (a[i]>a[_max]) {
      _max = i;
    }
  }
  return _max;
}
static void FUN(minmax)(int n, const Dtype* a, Dtype* _min, Dtype* _max) {
  int i;
  if (n>0) {
    *_min = *_max = *a;
    for (i=1; i<n; ++i) {
      if (a[i]<*_min) {
        *_min = a[i];
      } else if (a[i]>*_max) {
        *_max = a[i];
      }
    }
  }
  return ;
}
static void FUN(scal)(int N, Dtype alpha, Dtype *X, int incX) {
  int i = 0;
  for (; i<N; ++i) {
    X[i*incX] *= alpha;
  }
}

static void FUN(axpy)(int n, Dtype alpha, const Dtype *x, int incx, Dtype *y, int incy) {
  int i, n0 = n&(-3);
#pragma omp parallel for
  for (i = 0; i<n0; i+=4) {
    y[(i+0)*incy] += alpha * x[(i+0)*incx];
    y[(i+1)*incy] += alpha * x[(i+1)*incx];
    y[(i+2)*incy] += alpha * x[(i+2)*incx];
    y[(i+3)*incy] += alpha * x[(i+3)*incx];
  }
  for (i=n0; i<n; ++i) {
    y[i*incy] += alpha * x[i*incx];
  }
}
static void FUN(axpby)(int N, Dtype alpha, const Dtype* X, int incX, Dtype beta, Dtype* Y, int incY) {
  if (0) {
    FUN(scal)(N, beta, Y, incY);
    FUN(axpy)(N, alpha, X, incX, Y, incY);
  } else {
    int i, n0=N&-3;
#pragma omp parallel for
    for (i=0; i<N-3; i+=4) {
      Y[(i+0)*incY] = alpha * X[(i+0)*incX] + beta * Y[(i+0)*incY];
      Y[(i+1)*incY] = alpha * X[(i+1)*incX] + beta * Y[(i+1)*incY];
      Y[(i+2)*incY] = alpha * X[(i+2)*incX] + beta * Y[(i+2)*incY];
      Y[(i+3)*incY] = alpha * X[(i+3)*incX] + beta * Y[(i+3)*incY];
    }
    for (i=n0; i<N; ++i) {
      Y[i*incY] = alpha * X[i*incX] + beta * Y[i*incY];
    }
  }
}

// y = alpha * A * x + beta
static void FUN(gemv)(enum CBLAS_ORDER order, enum CBLAS_TRANSPOSE trans, int M, int N,
		 Dtype alpha, const Dtype *A, int lda, const Dtype *X, int incx, Dtype beta, Dtype *Y, int incy) {
  int i, j;
  if (CblasColMajor==order) {
    ASSERT(0);
  }
  if (CblasTrans==trans) {
    for (j=0; j<N; ++j) {
      Y[j*incy] *= beta;
    }
    for (i=0; i<M; ++i) {
      Dtype x = alpha*X[i*incx];
      for (j=0; j<N; ++j) {
        Y[j*incy] += A[i*lda+j]*x;
      }
    }
  } else {
    for (i=0; i<M; ++i) {
      Dtype sum = Y[i*incy]*beta;
      for (j=0; j<N; ++j) {
        sum += A[i*lda+j]*X[j*incx];
      }
      Y[i*incy] = sum;
    }
  }
  return ;
}

static void FUN(geaxpy)(int m, int n, Dtype alpha, const Dtype *X, int incRowX, int incColX, Dtype *Y, int incRowY, int incColY) {
  int i, j;
  if ((incRowX*incRowY)>(incColX*incColY)) {
    T_SWAP(int, m, n);
    T_SWAP(int, incRowX, incColX);
    T_SWAP(int, incRowY, incColY);
  }
  if (!fequal(alpha, 1.0)) {
    for (j = 0; j < n; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i * incRowY + j * incColY] += alpha * X[i * incRowX + j * incColX];
      }
    }
  } else {
    for (j = 0; j < n; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i * incRowY + j * incColY] += X[i * incRowX + j * incColX];
      }
    }
  }
}
static void FUN(gescal)(int m, int n, Dtype alpha, Dtype *X, int incRowX, int incColX) {
  int i, j;
  if (incRowX>incColX) {
    T_SWAP(int, m, n);
    T_SWAP(int, incRowX, incColX);
  }
  if (!fequal(alpha, 0.0f)) {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        X[i * incRowX + j * incColX] *= alpha;
      }
    }
  } else {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        X[i * incRowX + j * incColX] = 0.0;
      }
    }
  }
}
