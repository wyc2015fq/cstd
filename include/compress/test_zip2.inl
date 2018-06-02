#include "cstd.h"
#include "cfile.h"
#include "cmath.h"
#include "sys_win32.inl"
#include "zipext.inl"
#include "zip.inl"
#include "unzip.inl"
//#include <direct.h>
// stat
static uchar songti_12[] = {
#include "font/songti_12.txt"
};
int test_zip()
{
  enum { BUFSZ = 100000 };
  char aa[BUFSZ] = {0};
  char buf[256] = {0};
  int len = BUFSZ, i, j;
  stream_t s[1] = {0};
  ZIPENTRY ze[1] = {0};
  sys_chdir("E:/pub/bin/arch");
  //randstr(buf, );
  //memstream_init();
  if (1) {
    stream_t ss[1] = {0};
    stream_t mem[1] = {0};
    TZip zip[1] = {0};
    int len = countof(songti_12);
    //savefile("aaaa.dat", songti_12, len);
    memstream_init(mem, songti_12, len);
    fstream_init(ss, fopen("aaa.zip", "wb"));
    strcpy(ze->name, "songti_12");
    TZip_set(zip, ss, 0);
    TZip_Add(zip, mem, ze);
    TZip_iclose(zip);
    stream_close(mem);
    stream_close(ss);
  }
  if (0) {
    stream_t ss[1] = {0};
    stream_t ss2[1] = {0};
    TZip zip[1] = {0};
    fstream_init(ss, fopen("aaa.zip", "wb"));
    //HZIP hz = CreateZip(bb, len, 0);
    TZip_set(zip, ss, 0);
    if (0) {
      for (i = 0; i < 100; ++i) {
        stream_t mem[1] = {0};
        _snprintf(ze->name, MAX_PATH, "aa%03d.txt", i);
        //randstr(aa, BUFSZ-1, "0123456789");
        for (j = 0; j < BUFSZ; ++j) {
          aa[j] = '0' + (j % 10);
        }
        memstream_init(mem, aa, BUFSZ);
        TZip_Add(zip, mem, ze);
      }
    }
    if (0) {
      stream_t ff[1] = {0};
      const char* fn = "feat.dat";
      fstream_init(ff, fopen(fn, "rb"));
      _snprintf(ze->name, MAX_PATH, "%s", fn);
      TZip_Add(zip, ff, ze);
      stream_close(ff);
    }
    //size = zip->writ;
    TZip_Close(zip);
    stream_close(ss);
  }
  if (1) {
    stream_t ss[1] = {0};
    TUnzip uz[1] = {0};
    int n;
    fstream_init(ss, fopen("file.zip", "rb"));
    TUnzip_Open(uz, ss, 0);
    TUnzip_Get(uz, -1, ze, 0);
    n = ze->index;
    for (i = 0; i < n; ++i) {
      stream_t ff[1] = {0};
      TUnzip_Get(uz, i, ze, ff);
      printf("%s %d %d\n", ze->name, ze->unc_size, ze->comp_size);
      _snprintf(buf, 256, "out/%s", ze->name);
      stream_savefile(ff, buf, ze->unc_size);
      setfiletime(buf, ze->ctime, ze->atime, ze->mtime);
      stream_close(ff);
    }
    TUnzip_Close(uz);
  }
#if 0
  {
    HZIP hz = CreateZip("bbb.zip", 0);
    for (i = 0; i < BUFSZ; ++i) {
      aa[i] = i % 200;
    }
    ZipAdd(hz, "a.txt", aa, len);
    ZipAdd(hz, "b.txt", aa, len);
    size = ((TZipHandleData*)hz)->zip->opos;
    CloseZipZ(hz);
  }
  {
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
#endif
  return 0;
}
#if 0
#if 0
//#define OpenZip OpenZipU
//#define CloseZip(hz) CloseZipU(hz)
int test_zip()
{
  HZIP hz = NULL;
  const char* sFile = "D:/code/c/GUI/bcgsoft/BCG/Bin/BCGPStyle2007Luna1030/BCGPStyle2007Luna1030.zip";
  ZIPENTRY ze;
  int i;
  const char* m_lpstr = "BCGPStyle2007Luna1030_IDX_OFFICE2007_STYLE_STYLE_XML.xml";
  m_lpstr = "BCGPStyle2007Luna1030_BLUE_IDB_OFFICE2007_STATUSBAR_VERT_BACK_1_PNG.png";
  hz = OpenZip(sFile, 0);
  if (hz) {
    int res, dwSize;
    BYTE* pData;
    if (FindZipItem(hz, m_lpstr, true, &i, &ze) != 0)  {
      return 0;
    }
    dwSize = ze.unc_size;
    if (dwSize == 0) {
      return 0;
    }
    pData = (BYTE*)malloc(dwSize);
    res = UnzipItem(hz, i, pData, dwSize);
    if (res == 0) {
      free(pData);
      CloseZip(hz);
    }
  }
  return 0;
}
#else
//#include "XUnzip.cpp"
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
#ifdef _UNICODE
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
#endif
int test_zip()
{
  HZIP hz = NULL;
  const char* sFile = "D:/code/c/GUI/bcgsoft/BCG/Bin/BCGPStyle2007Luna1030/BCGPStyle2007Luna1030.zip";
  ZIPENTRY ze;
  int i;
  const char* m_lpstr = "BCGPStyle2007Luna1030_IDX_OFFICE2007_STYLE_STYLE_XML.xml";
  m_lpstr = "BCGPStyle2007Luna1030_BLUE_IDB_OFFICE2007_STATUSBAR_VERT_BACK_1_PNG.png";
  hz = OpenZip((void*)sFile, 0, 2);
  if (hz) {
    int res, dwSize;
    BYTE* pData;
    if (FindZipItem(hz, m_lpstr, true, &i, &ze) != 0)  {
      return 0;
    }
    dwSize = ze.unc_size;
    if (dwSize == 0) {
      return 0;
    }
    pData = (BYTE*)malloc(dwSize);
    res = UnzipItem(hz, i, pData, dwSize, 3);
    res = UnzipItem(hz, i, pData, dwSize, 3);
    if (res == 0) {
      free(pData);
      CloseZip(hz);
    }
  }
  return 0;
}
#endif

