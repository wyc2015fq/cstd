
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "chalg.h"
#include "utime.h"
#include "macro.h"
#include "sys/findfile.h"
#include "lzari.h"
#include "Huffman.inl"
#include "huff.h"

#include "jpeg.h"

#include "comp.inl"
//#include "LZ77.h"

int fsize(FILE* pf) {
  int sz, oldpos = ftell(pf);
  fseek(pf, 0, SEEK_END);
  sz = ftell(pf);
  fseek(pf, oldpos, SEEK_SET);
  return sz;
}
int fmap(const char* fname, void* buf) {
  FILE* pf = fopen(fname, "rb");
  int sz=0;
  if (pf) {
    sz = fsize(pf);
    if (buf) {
      sz = fread(buf, 1, sz, pf);
    }
    fclose(pf);
  }
  return sz;
}

int fsave(const char* fname, const void* buf, int buflen) {
  FILE* pf = fopen(fname, "wb");
  int sz=0;
  if (pf) {
    sz = fwrite(buf, 1, buflen, pf);
    fclose(pf);
  }
  return sz;
}
unsigned char buf[1000000];
unsigned char dat[1000000];
unsigned char lzf[1000000];
int test_comp4() {
  int sz_dat, sz_lzf, sz, fram=0;
  char buf1[256];
  char buf2[256];
  FILE* plisttxt;
  _chdir("F:/pub/bin/codec/jpeg");
  plisttxt= fopen("list.txt", "rb");
  for ( fram = 0; fgets(buf2, 256, plisttxt)>0; ++fram ) {
    while ('\n'==strend(buf2)[-1]||'\r'==strend(buf2)[-1]) {strend(buf2)[-1]=0;}
    sz_dat = fmap(buf2, dat);
    m_low_strm = dat;
    m_strm = dat;
    ReadHeader();
    _snprintf(buf1, 256, "%ss.dat", buf2);
    fsave(buf1, buf, sz);
    printf("%d\n", sz);
  }
  printf("%s OK!\n", buf2);
  return 0;
}
int test_comp3() {
  int sz_dat, sz_lzf, sz, fram=0;
  char buf1[256];
  char buf2[256];
  unsigned char buf[100000];
  unsigned char dat[100000];
  unsigned char lzf[100000];
  FILE* plisttxt;
  _chdir("F:/pub/cstd/DSP/问题特征/断电后");
  _chdir("F:/pub/cstd/DSP/问题特征/断电前");
  _chdir("F:/pub/bin/codec/Feats");
  _chdir("F:/pub/bin/codec/新建文件夹2");
  plisttxt= fopen("list.txt", "rb");
  for ( fram = 0; fgets(buf2, 256, plisttxt)>0; ++fram ) {
    while ('\n'==strend(buf2)[-1]||'\r'==strend(buf2)[-1]) {strend(buf2)[-1]=0;}
    sz_dat = fmap(buf2, dat);
    //sz = HUFF_Decode(dat, sz_dat, buf, 100000);
    sz = HUFF_Decode(dat, sz_dat, buf, 100000);
    //sz = LZF_Decode(dat, sz_dat, buf, 100000);
    _snprintf(buf1, 256, "%s.huf", buf2);
    fsave(buf1, buf, sz);
    printf("%d\n", sz);
  }
  printf("%s OK!\n", buf2);
  return 0;
}
int test_comp2() {
  int sz_dat, sz_lzf, sz, fram=0;
  char buf2[256];
  unsigned char buf[100000];
  unsigned char dat[100000];
  unsigned char lzf[100000];
  FILE* plisttxt;
  _chdir("F:/pub/bin/codec");
  plisttxt= fopen("list.txt", "rb");
  if (0==plisttxt)
    return 0;
  for ( fram = 0; fgets(buf2, 256, plisttxt)>0; ++fram ) {
    while ('\n'==strend(buf2)[-1]||'\r'==strend(buf2)[-1]) {strend(buf2)[-1]=0;}
    sz_dat = fmap(buf2, dat);
    strcpy(strend(buf2)-5, ".dat");
    sz_lzf = fmap(buf2, lzf);
    sz = LZF_Encode(dat, sz_dat, buf, 100000);
    assert(sz_lzf==sz && 0==memcmp(buf, lzf, sz));
    sz = LZF_Decode(lzf, sz_lzf, buf, 100000);
    assert(sz_dat==sz && 0==memcmp(buf, dat, sz));
    printf("%s OK!\n", buf2);
  }
  return 0;
}

int test_comp() {
  BYTE * str1 = NULL;
  BYTE *str2 = NULL;
  BYTE *str3 = NULL;
  int len, i, j, slen;
  
  len = 100000;
  slen = 10000+2 * len;
  //printf(str1);
  str1 = MALLOC( BYTE, slen );
  str2 = MALLOC( BYTE, slen );
  str3 = MALLOC( BYTE, slen );
  logclear();
  logprintf("算法 压缩后大小 压缩前大小 压缩率 压缩耗时 解压耗时\n");
  srand( time( 0 ) );
  
#define TEST(_N) \
  {double t1, t2; int l1, l2; {utime_start(_start_time); l1 = _N##_Encode(str1, len, str2, slen); t1=utime_elapsed(_start_time);}\
  {utime_start(_start_time); l2 = _N##_Decode(str2,l1,str3, slen); t2=utime_elapsed(_start_time);}\
  logprintf("%-5s %8d/%d=%1.3f %2.3f %2.3f\n", #_N, l1, len, (double)l1/len, t1, t2); \
  assert(len==l2 && 0==memcmp((char *)str3, str1, len)); }
  
  for ( j = 0; j < 10000; ++j, len*=1 ) {
    srand(231+j*j);
    for ( i = 0; i < len; ++i ) {
      str1[ i ] = '0' + rand() % 10;
      //str1[ i ] = i%13;
    }
    //TEST( Huffman );
    //TEST(LZARI);
    TEST(HUFF);
    //TEST(LZF);
  }
  
  SAFEFREE( str1 );
  SAFEFREE( str2 );
  SAFEFREE( str3 );
  return 0;
}

int main() {
  //test_comp4();
  test_comp3();
  test_comp();
  return 0;
}
