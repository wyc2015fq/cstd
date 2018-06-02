
#include "cstd.h"
#include "unzip.inl"
#include "zip.inl"

static uchar songti_12[] = {
#include "font/songti_12.txt"
};
int test_zip()
{
  enum { BUFSZ = 100000 };
  char aa[BUFSZ] = {0};
  char buf[256] = {0};
  int len = BUFSZ, i, j=0;
  int size;
  char* src2 = NULL;
  sys_chdir("E:/pub/bin/arch");
  //randstr(buf, );
  //memstream_init();
  {
    HZIP hz = CreateZip("bbb.zip", "123");
    for (i = 0; i < BUFSZ; ++i) {
      aa[i] = i % 200;
    }
    len = countof(songti_12);
    ZipAdd(hz, "a.txt", songti_12, len);
    //ZipAdd(hz, "b.txt", aa, len);
    size = ((TZipHandleData*)hz)->zip->opos;
    CloseZipZ(hz);
  }
  {
    char* bb=0;
    HZIP hz = OpenZip(bb, size, 0);
    ZIPENTRY ze;
    i = 0;
    FindZipItem(hz, "a.txt", true, &i, &ze);
    UnzipItem(hz, i, src2, ze.unc_size);
    for (i = 0; i < 1000; ++i) {
      aa[i] = i % 200;
    }
    //size = ((TZipHandleData* )hz)->zip->opos;
    CloseZipZ(hz);
  }
  return 0;
}
#include "sys_win32.inl"