#include "cstd.h"
#include "zip.inl"
//#include "codec/LZ77.inl"
static uchar songti_12[] = {
#include "font/songti_12.txt"
};
#include "cfile.h"
int test_zip()
{
  int len = 0, i, j = 0;
  int size, sz2 = 0;
  char* src2 = NULL;
  const char* pass = "123";
  char* bb = 0;
  char* aa = 0;
  char* zbuf = 0;
  ZIPENTRY ze;
  char buf[256] = {0};
  int t = 3;
  TUnzip unz[1] = {0};
  TZip zip[1] = {0};
  sys_chdir("D:/pub/bin/compress");
  //randstr(zbuf, );
  //memstream_init();
  len = countof(songti_12);
  size = len * 20;
  MYREALLOC(zbuf, size);
  MYREALLOC(aa, size);
  if (0) {
    if (TUnzip_OpenFile(unz, "testc.zip", 0)) {
      TUnzip_Find(unz, "testc.exe", true, &i, &ze);
      TUnzip_UnzipMem(unz, i, aa, ze.unc_size);
      TUnzip_Close(unz);
    }
  }
  if (1) {
    //pass = NULL;
    if (TZip_OpenFile(zip, "bbb.zip", pass)) {
      for (j = 0; j < t; ++j) {
        _snprintf(buf, 255, "out/a%d.txt", j);
        TZip_AddMem(zip, buf, songti_12, len);
      }
      TZip_Close(zip);
      sz2 = zip->writ;
    }
    if (TUnzip_OpenFile(unz, "bbb.zip", pass)) {
      for (j = 0; j < t; ++j) {
        _snprintf(buf, 255, "out/a%d.txt", j);
        TUnzip_Find(unz, buf, true, &i, &ze);
        TUnzip_UnzipMem(unz, i, aa, ze.unc_size);
        ASSERT(ze.unc_size == countof(songti_12));
        ASSERT(0 == memcmp(aa, songti_12, ze.unc_size));
      }
      TUnzip_Close(unz);
    }
  }
  if (1) {
    sz2 = 0;
    if (TZip_OpenMem(zip, zbuf, size, pass)) {
      for (j = 0; j < t; ++j) {
        _snprintf(buf, 255, "a%d.txt", j);
        TZip_AddMem(zip, buf, songti_12, len);
      }
      //ZipAdd(hz, "b.txt", aa, len);
      TZip_Close(zip);
      sz2 = zip->writ;
    }
    //savefile("ccc.zip", zbuf, size);
    i = 0;
    if (TUnzip_OpenMem(unz, zbuf, sz2, pass)) {
      for (j = 0; j < t; ++j) {
        memset(&ze, 0, sizeof(ze));
        _snprintf(buf, 255, "a%d.txt", j);
        TUnzip_Find(unz, buf, true, &i, &ze);
        TUnzip_UnzipMem(unz, i, aa, size);
        ASSERT(ze.unc_size == countof(songti_12));
        ASSERT(0 == memcmp(aa, songti_12, ze.unc_size));
      }
      TUnzip_Close(unz);
    }
  }
  FREE(zbuf);
  return 0;
}
//#include "sys_win32.inl"

