#include "str.h"
#include "sys\findfile.h"
int test_charenc()
{
  char buf[256];
  int n = 0;
  buf_t bf[1] = {0};
  buf_t bf2[1] = {0};
  buf_load("aaa.txt", bf);
  //short buf2[256];
  randstr_cn(buf, 256);
  //mb2wc();
  n = mb2wc(CE_GB2312, bf->data, bf->len, 0, 0);
  bfsetsize(bf2, n * 2 + 2);
  //bom
  // big Endian(FE FF) ºÍ Little Endian(FF FE)
  bf2->data[0] = 0xff;
  bf2->data[1] = 0xfe;
  n = mb2wc(CE_GB2312, bf->data, bf->len, ((unsigned short*)bf2->data) + 1, n);
  buf_save("aaauni.txt", bf2);
  bffree(bf);
  bffree(bf2);
  return 0;
}

