int test_push_heap()
{
  int a[] = {5, 1, 9, 4, 6, 2, 0, 3, 8, 7};
  enum {an = countof(a)};
  int b[5] = {0};
  int i, bn = 0, t;
  for (i = 0; i < an; i++) {
    if (bn < 5) {
      b[bn++] = a[i];
    }
    PUSH_HEAP(bn, b, LESS, t);
  }
  SORT_HEAP(bn, b, LESS, t);
  return 0;
}

