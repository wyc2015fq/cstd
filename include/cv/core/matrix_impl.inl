
static double CCFUNC(isdiag)(const T* A, int lda, int rows, int cols, double b0, double b1) {
  double dist = 0;
  if (rows>0 && cols>0) {
    double b[] = {b0, b1};
    int i, j;
    for (i=0; i<rows; ++i) {
      for (j=0; j<cols; ++j) {
        double d = A[i*lda + j] - b[i==j];
        dist += d*d;
      }
    }
    dist = sqrt(dist/rows*cols);
  }
  return dist;
}

static void CCFUNC(mat_symm)(T* A, int step0, int step1, int rows, int cols, int flags) {
  int i, j, n = MIN(rows, cols);
  if (flags & CC_SYMM_COLS) {
    T_SWAP(int, step0, step1);
  }
  for (i=0; i<n; ++i) {
    for (j=0; j<n; ++j) {
      A[i*step0 + j*step1] = A[j*step0 + i*step1];
      if (CC_SYMM_ABS & flags) {
        A[i*step0 + j*step1] = fabs(A[j*step0 + i*step1]);
      }
    }
  }
}
#undef T
#undef CCFUNC
