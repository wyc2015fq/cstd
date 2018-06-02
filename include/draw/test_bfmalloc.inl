
#include "cstd.h"


#define MEM_FREE(p)  (pfree(p), p = NULL)

int test_mem_malloc() {
  int len = 1 << 20;
  uchar* buf = (uchar*)malloc(len);
  int i;
  void* p[10] = {0};
  mem_init(buf, len);
  p[0] = pmalloc(10);
  MEM_FREE(p[0]);
  for (i=0; i<10; ++i) {
    p[i] = pmalloc(10);
  }
  for (i=0; i<10; ++i) {
    MEM_FREE(p[10-1-i]);
  }
  for (i=0; i<10; ++i) {
    p[i] = prealloc(p[i], 20);
  }
  for (i=0; i<10; ++i) {
    p[i] = prealloc(p[i], 40);
  }
  for (i=0; i<10; ++i) {
    MEM_FREE(p[10-1-i]);
  }
  FREE(buf);
  return 0;
}

int test_bfmalloc()
{
  int len = 1 << 20;
  uchar* buf = GMALLOC(uchar, len);
  buf_t bf[1] = {0};
  char* p = 0;
  char* p2 = 0;
  bfinit(bf, buf, len);
  bf_malloc(bf, &p, 100);
  bf_malloc(bf, &p2, 100);
  //p[-1]=0;
  //p[100] = 0;
  bf_free(bf, &p, 100);
  bf_free(bf, &p2, 100);
  free(buf);
  return 0;
}
