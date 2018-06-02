{
  int n = sizeA.w;
  int m = sizeB.w, i;

  assert(m > 0 && sizeA.w == sizeA.h && sizeA.h == sizeB.h);

  A += stepA * (n - 1);
  B += stepB * (n - 1);

  for (i = n - 1; i >= 0; i--, A -= stepA) {
    int j, k;

    for (j = 0; j < m; j++) {
      arrtype* tB = B + j;
      double x = 0;

      for (k = n - 1; k > i; k--, tB -= stepB) {
        x += A[k] * tB[0];
      }

      tB[0] = (arrtype)((tB[0] - x) * A[i]);
    }
  }
}