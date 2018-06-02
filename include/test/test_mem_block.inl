

int test_mem_block() {
  int i;
  uchar* aa[200] = {0};
  int sz = countof(aa);
  my_memfun();
  MEM_CHECK_BEGIN();
  // uchar* aa = MALLOC(uchar, 123);
  tic;
  mem_block_t *b = NULL;
  for (i=0; i<sz; ++i) {
    aa[i] = MEM_BLOCK_ALLOC(uchar, b, 1000, 100);
  }
  MEM_BLOCK_FREEALL(b);
  toc;
  tic;
  for (i=0; i<sz; ++i) {
    aa[i] = MALLOC(uchar, 100);
  }
  for (i=0; i<sz; ++i) {
    FREE(aa[i]);
  }
  toc;
  MEM_CHECK_END();
  tictoc;
  return 0;
}