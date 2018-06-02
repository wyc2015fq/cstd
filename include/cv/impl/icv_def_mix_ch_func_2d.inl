
{
  int i, k;
  int block_size0 = n == 1 ? size.w : 1024;
  arrtype** src = (arrtype**)src0;
  arrtype** dst = (arrtype**)dst0;

  for (; size.h--;) {
    int remaining = size.w;

    for (; remaining > 0;) {
      int block_size = MIN(remaining, block_size0);

      for (k = 0; k < n; k++) {
        arrtype* s = src[k];
        arrtype* d = dst[k];
        int ds = sdelta1[k], dd = ddelta1[k];

        if (s) {
          for (i = 0; i <= block_size - 2; i += 2, s += ds * 2, d += dd * 2) {
            arrtype t0 = s[0], t1 = s[ds];
            d[0] = t0;
            d[dd] = t1;
          }

          if (i < block_size) {
            d[0] = s[0], s += ds, d += dd;
          }

          src[k] = s;
        }
        else {
          for (i = 0; i <= block_size - 2; i += 2, d += dd * 2) {
            d[0] = d[dd] = 0;
          }

          if (i < block_size) {
            d[0] = 0, d += dd;
          }
        }

        dst[k] = d;
      }

      remaining -= block_size;
    }

    for (k = 0; k < n; k++) {
      src[k] += sdelta0[k], dst[k] += ddelta0[k];
    }
  }

}
