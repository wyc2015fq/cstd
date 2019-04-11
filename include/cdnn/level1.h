
static void FUN(set)(int N, const const Dtype* alpha, Dtype* Y) {
  int i = 0;
  Dtype a = *alpha;
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (; i < N-3; i+=4) {
    Y[i+0] = a;
    Y[i+1] = a;
    Y[i+2] = a;
    Y[i+3] = a;
  }
  for (; i < N; ++i) {
    Y[i] = a;
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
static void FUN(pow)(int N, const Dtype* A, const Dtype b, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { pow(A[i], b); }
}
static void FUN(sqr)(int N, const Dtype* A, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = A[i] * A[i]; }
}
static void FUN(sqrt)(int N, const Dtype* A, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = sqrt(A[i]); }
}
static void FUN(exp)(int N, const Dtype* A, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = exp(A[i]); }
}
static void FUN(log)(int N, const Dtype* A, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = log(A[i]); }
}
static void FUN(abs)(int N, const Dtype* A, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = fabs(A[i]); }
}
static void FUN(add)(int N, const Dtype* A, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = A[i] + b[i]; }
}
static void FUN(sub)(int N, const Dtype* A, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = A[i] - b[i]; }
}
static void FUN(mul)(int N, const Dtype* A, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = A[i] * b[i]; }
}
static void FUN(div)(int N, const Dtype* A, const Dtype* b, Dtype* y) {
  int i = 0;ASSERT(N>0); ASSERT(A); ASSERT(y);
  for (; i < N; ++i) { y[i] = A[i] / b[i]; }
}
static Dtype FUN(dot)(int N, const Dtype *x, int incx, const Dtype *y, int incy) {
  Dtype sum=0;
  int i=0;
  for (; i<N; ++i) {
    sum += x[i] * y[i];
  }
  return sum;
}

#ifndef ABS
#define ABS(a) ( (a) > -(a) ? (a) : -(a) )
#endif
static Stype FUN(asum)(int N, const Dtype *X, int incx) {
  Stype sum=0;
  int i=0;
  if (1 == incx) {
    for (; i < N; ++i) {
      sum += ABS(X[i]);
    }
  }
  else {
    for (; i < N; ++i) {
      sum += ABS(X[i*incx]);
    }
  }
  return sum;
}
static double FUN(euclidean)(int N, const Dtype* a, const Dtype* b) {
  double sum = 0;
  int i;
  for (i=0; i<N; ++i) {
    Dtype t = a[i] - b[i];
    sum += t*t;
  }
  return sum;
}
static Dtype FUN(min)(int N, const Dtype* a) {
  int i;
  Dtype _min;
  if (N<=0) return 0;
  _min = a[0];
  for (i=1; i<N; ++i) {
    if (a[i]<_min) {
      _min = a[i];
    }
  }
  return _min;
}
static int FUN(argmin)(int N, const Dtype* a) {
  int i;
  int _min;
  if (N<=0) return -1;
  _min = 0;
  for (i=1; i<N; ++i) {
    if (a[i]<a[_min]) {
      _min = i;
    }
  }
  return _min;
}
static Dtype FUN(max)(int N, const Dtype* a) {
  int i;
  Dtype _max;
  if (N<=0) return 0;
  _max = a[0];
  for (i=1; i<N; ++i) {
    if (a[i]>_max) {
      _max = a[i];
    }
  }
  return _max;
}
static int FUN(argmax)(int N, const Dtype* a) {
  int i;
  int _max;
  if (N<=0) return -1;
  _max = 0;
  for (i=1; i<N; ++i) {
    if (a[i]>a[_max]) {
      _max = i;
    }
  }
  return _max;
}
static void FUN(minmax)(int N, const Dtype* a, Dtype* _min, Dtype* _max) {
  int i;
  if (N>0) {
    *_min = *_max = *a;
    for (i=1; i<N; ++i) {
      if (a[i]<*_min) {
        *_min = a[i];
      } else if (a[i]>*_max) {
        *_max = a[i];
      }
    }
  }
  return ;
}
static void FUN(scal)(int N, const Dtype* alpha, Dtype *X, int incX) {
	int i = 0;
	Dtype a = *alpha;
	for (; i<N; ++i) {
		X[i*incX] *= a;
	}
}
static void FUN(scalb)(int N, const Dtype* alpha, Dtype *X, int incX, const Dtype* beta) {
	int i = 0, n0 = N&(-3);
	Dtype a = *alpha;
	Dtype b = *beta;
#pragma omp parallel for
	for (i = 0; i<n0; i += 4) {
		X[(i + 0)*incX] = X[(i + 0)*incX] * a + b;
		X[(i + 1)*incX] = X[(i + 1)*incX] * a + b;
		X[(i + 2)*incX] = X[(i + 2)*incX] * a + b;
		X[(i + 3)*incX] = X[(i + 3)*incX] * a + b;
	}
	for (; i<N; ++i) {
		Dtype x = X[i*incX];
		X[i*incX] = x*a+b;
	}
}

static void FUN(axpy)(int N, const Dtype* alpha, const Dtype *x, int incx, Dtype *y, int incy) {
  int i = 0, n0 = N&(-3);
  Dtype a = *alpha;
#pragma omp parallel for
  for (; i<n0; i+=4) {
    y[(i+0)*incy] += a * x[(i+0)*incx];
    y[(i+1)*incy] += a * x[(i+1)*incx];
    y[(i+2)*incy] += a * x[(i+2)*incx];
    y[(i+3)*incy] += a * x[(i+3)*incx];
  }
  for (i=n0; i<N; ++i) {
    y[i*incy] += a * x[i*incx];
  }
}
static void FUN(axpby)(int N, const Dtype* alpha, const Dtype* X, int incX, const Dtype* beta, Dtype* Y, int incY) {
	Dtype a = *alpha;
	Dtype b = *beta;
  if (0) {
    FUN(scal)(N, beta, Y, incY);
    FUN(axpy)(N, alpha, X, incX, Y, incY);
  } else {
    int i, n0=N&-3;
#pragma omp parallel for
    for (i=0; i<N-3; i+=4) {
      Y[(i+0)*incY] = a * X[(i+0)*incX] + b * Y[(i+0)*incY];
      Y[(i+1)*incY] = a * X[(i+1)*incX] + b * Y[(i+1)*incY];
      Y[(i+2)*incY] = a * X[(i+2)*incX] + b * Y[(i+2)*incY];
      Y[(i+3)*incY] = a * X[(i+3)*incX] + b * Y[(i+3)*incY];
    }
    for (i=n0; i<N; ++i) {
      Y[i*incY] = a * X[i*incX] + b * Y[i*incY];
    }
  }
}

// y = alpha * A * x + beta
static void FUN(gemv)(enum CBLAS_ORDER order, enum CBLAS_TRANSPOSE trans, int M, int N,
		 const Dtype* alpha, const Dtype *A, int lda, const Dtype *X, int incx, const Dtype* beta, Dtype *Y, int incy) {
  int i, j;
  Dtype a = *alpha;
  Dtype b = *beta;
  if (CblasColMajor==order) {
    ASSERT(0);
  }
  if (CblasTrans==trans) {
    for (j=0; j<N; ++j) {
      Y[j*incy] *= b;
    }
    for (i=0; i<M; ++i) {
      Dtype x = a*X[i*incx];
      for (j=0; j<N; ++j) {
        Y[j*incy] += A[i*lda+j]*x;
      }
    }
  } else {
    for (i=0; i<M; ++i) {
      Dtype sum = Y[i*incy]*b;
      for (j=0; j<N; ++j) {
        sum += A[i*lda+j]*X[j*incx];
      }
      Y[i*incy] = sum;
    }
  }
  return ;
}

#ifndef T_SWAP
#define T_SWAP(_TYPE, _A,_B) {_TYPE _TMP=_A; _A = _B, _B = _TMP;}
#endif

static void FUN(geaxpy)(int m, int N, const Dtype* alpha, const Dtype *X, int incRowX, int incColX, Dtype *Y, int incRowY, int incColY) {
  int i, j;
  Dtype a = *alpha;
  if ((incRowX*incRowY)>(incColX*incColY)) {
    T_SWAP(int, m, N);
    T_SWAP(int, incRowX, incColX);
    T_SWAP(int, incRowY, incColY);
  }
  if (!fequal(a, 1.0)) {
    for (j = 0; j < N; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i * incRowY + j * incColY] += a * X[i * incRowX + j * incColX];
      }
    }
  } else {
    for (j = 0; j < N; ++j) {
      for (i = 0; i < m; ++i) {
        Y[i * incRowY + j * incColY] += X[i * incRowX + j * incColX];
      }
    }
  }
}
static void FUN(gescal)(int m, int N, const Dtype* alpha, Dtype *X, int incRowX, int incColX) {
  int i, j;
  Dtype a = *alpha;
  if (incRowX>incColX) {
    T_SWAP(int, m, N);
    T_SWAP(int, incRowX, incColX);
  }
  if (!fequal(a, 0.0f)) {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < N; ++j) {
        X[i * incRowX + j * incColX] *= a;
      }
    }
  } else {
    for (i = 0; i < m; ++i) {
      for (j = 0; j < N; ++j) {
        X[i * incRowX + j * incColX] = 0.0;
      }
    }
  }
}
