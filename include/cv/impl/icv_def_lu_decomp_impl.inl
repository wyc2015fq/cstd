{
  int n = sizeA.w;
  int m = 0, i;
  arrtype det = 1;

  assert(sizeA.w == sizeA.h);

  if (B) {
    assert(sizeA.h == sizeB.h);
    m = sizeB.w;
  }

  for (i = 0; i < n; i++, A += stepA, B += stepB) {
    int j, k = i;
    arrtype* tA = A;
    arrtype* tB = NULL;
    arrtype kval = fabs(A[i]), tval;

    /* find the pivot element */
    for (j = i + 1; j < n; j++) {
      tA += stepA;
      tval = fabs(tA[i]);

      if (tval > kval) {
        kval = tval;
        k = j;
      }
    }

    if (kval == 0) {
      det = 0;
      break;
    }

    /* swap rows */
    if (k != i) {
      tA = A + stepA * (k - i);
      det = -det;

      for (j = i; j < n; j++) {
        arrtype t;
        CC_SWAP(A[j], tA[j], t);
      }

      if (m > 0) {
        tB = B + stepB * (k - i);

        for (j = 0; j < m; j++) {
          arrtype t = B[j];
          CC_SWAP(B[j], tB[j], t);
        }
      }
    }

    tval = 1. / A[i];
    det *= A[i];
    tA = A;
    tB = B;
    A[i] = tval; /* to replace division with multiplication in LUBack */

    /* update matrix and the right side of the system */
    for (j = i + 1; j < n; j++) {
      arrtype alpha;
      tA += stepA;
      tB += stepB;
      alpha = -tA[i] * tval;

      for (k = i + 1; k < n; k++) {
        tA[k] = tA[k] + alpha * A[k];
      }

      if (m > 0) {
        for (k = 0; k < m; k++) {
          tB[k] = (arrtype)(tB[k] + alpha * B[k]);
        }
      }
    }
  }

  if (_det) {
    *_det = det;
  }
}
