#include "cstd.h"
#include "cfile.h"
#include "cmath.h"
#include "lib/zlib/libzlib.inl"
#include <assert.h>
//静态加载如下：
/*zlib中两个主要函数的声明:
int compress OF((Bytef *dest,   uLongf *destLen,const Bytef *source, uLong sourceLen));
int uncompress OF((Bytef *dest,   uLongf *destLen,const Bytef *source, uLong sourceLen));
*/
#define MAX_LEN 1024*100//本例只适用于小于100KB的文件  
static uchar songti_12[] = {
#include "font/songti_12.txt"
};
int test_zlib()
{
  uchar* aa = MALLOC(uchar, MAX_LEN * 3);
  uchar* bb = aa + MAX_LEN;
  uchar* cc = aa + 2 * MAX_LEN;
  int nError;
  int i;
  uint u = 12;
  sys_chdir("E:/pub/bin/arch");
  if (1) {
    uLongf naa1 = 87200, naa, nbb = MAX_LEN, ncc = MAX_LEN;
    uchar aa[87200] = {0};
    loadfile("feat.dat", aa, naa1, 0);
    {
      nError = compress(bb, &nbb, songti_12, countof(songti_12));
    }
    //nError = compress(bb, &nbb, aa, naa = naa1);
    if (0 == nError) {
      //nbb必须大于lsize，详情请参照zlib文档
      printf("压缩成功(%d/%d),压缩率为: %f%%\n", nbb, naa, 100.*nbb / naa);
    }
  }
  for (i = 0; i < 100; ++i) {
    uLongf naa = MAX_LEN, nbb = MAX_LEN, ncc = MAX_LEN;
    randstr(&u, (char*)aa, MAX_LEN, "012");
    nError = compress(bb, &nbb, aa, naa);
    savefile("aaa.zip", bb, nbb);
    if (0 == nError) {
      //nbb必须大于lsize，详情请参照zlib文档
      printf("压缩成功,压缩率为: %f%%\n", 100.*nbb / naa);
    }
    else {
      int asdf = 0;
    }
    nError = uncompress(cc, &ncc, bb, nbb);
    //ASSERT(ncc==naa && 0==memcmp(aa, cc, naa));
  }
  free(aa);
  return 0;
}

