int test_memcheck()
{
  enum {N = 10};
  int id[N] = {0};
  char* aa[N] = {0};
  int i, j, t;
  for (i = 0; i < N; ++i) {
    id[i] = i;
  }
  for (i = 0; i < N; ++i) {
    j = rand() % N;
    CV_SWAP(id[i], id[j], t);
  }
  {
    MEM_CHECK_BEGIN();
    for (j = 0; j < 1; ++j) {
      for (i = 0; i < N; ++i) {
        t = rand();
        MYREALLOC(aa[i], t);
      }
    }
    for (i = 0; i < N; ++i) {
      FREE(aa[i]);
    }
    MEM_CHECK_END();
  }
  return 0;
}

