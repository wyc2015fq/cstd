
#include "cstd.h"
#include "cfile.h"
#include "str.h"

// 得到文件长度
// handle 用open()函数打开的文件句柄


#define SWAP_T(a, b, TYPE)  do { TYPE t = a; a = b; b = t; } while(0);

int test_unicodetoutf8(const char* infilename, const char* outfilename)
{
  FILE* pf = fopen(infilename, "rb");
  FILE* pf2;
  char* buf;
  char* buf1;
  int n;
  
  if (pf) {
    FILESIZE(pf, n);
    buf = (char*)pmalloc(n);
    buf1 = (char*)pmalloc(n * 3);
    memset(buf, 0, n);
    memset(buf1, 0, n * 3);
    fread(buf, n, 1, pf);
    fclose(pf);
    //n = utf8tounicode(256, buf, out2);
    //n = unicodetoutf8(n, out2, out);
    //n = iconv(CE_GB2312, buf, n, buf1);
    SWAP_T(buf, buf1, char*);
    printf("asdf%d\n", n);
    pf2 = fopen(outfilename, "wb");
    fwrite(buf, n, 1, pf2);
    fclose(pf2);
    pfree(buf);
    pfree(buf1);
  }
  
  return 0;
}

int test_codecvt(int argc, char** argv)
{
  char* spec = "*.txt";
  int totyp = CE_GB2312;
  int tourl = 0;
  int i = 1;
  if (argc<2) {
    printf("%s\n", "usage: codecvt [/utf|/uni|/gb|/url|/gburl] [*.txt]");
  }
  
  if (argc == 3 && '/' == argv[ 1 ][ 0 ] && 0 == stricmp("/gburl2", argv[ 1 ])) {
    str_t s = STR1(argv[2]);
    str_t d[1] = {0};
    str_str2url(s, d);
    puts(d->s);
    str_free(d);
    return 0;
  }
  // "%E7%AE%A1%E7%90%86%E5%AD%A6--%E5%91%A8%E4%B8%89%E5%A4%9A%202010.pdf";
  if (argc == 3 && '/' == argv[ 1 ][ 0 ] && 0 == stricmp("/gburl", argv[ 1 ])) {
    str_t s = STR1(argv[ 2 ]);
    str_t d[3] = {0};
    //iconv(CE_UNKNOW, s, CE_UTF8, d);
    str_url2str(s, d+1);
    iconv(CE_UTF8, d[1], CE_GB2312, d+2);
    puts(d[2].s);
    str_frees(d, 3);
    return 0;
  }
  
  // 直接处理url编码
  if (argc == 3 && '/' == argv[ 1 ][ 0 ] && 'r' == argv[ 1 ][ 2 ]) {
    str_t s = STR1(argv[ 2 ]);
    str_t d[3] = {0};
    iconv(CE_UNKNOW, s, CE_UTF8, d);
    str_str2url(d[0], d+1);
    puts(d[1].s);
    str_frees(d, 3);
    return 0;
  }
  
  for (; i < argc; ++i) {
    char* arg = argv[ i ];
    
    if ('/' == arg[ 0 ] || '-' == arg[ 0 ]) {
      if ('t' == arg[ 2 ]) {
        totyp = CE_UTF8;
      }
      
      if ('n' == arg[ 2 ]) {
        totyp = CE_UTF16;
      }
      
      if ('r' == arg[ 2 ]) {
        tourl = 1;
      }
    }
    else {
      spec = arg;
    }
  }
  
  {
    findinfo_t s[1] = {0};
    fileinfo_t f[1] = {0};
    str_t str[5] = {0};
    str_t* out1 = str + 1;
    str_t* out2 = str + 2;
    while(sys_find_next_file(s, ".", spec, f, 0)) {
      if (str_load(f->name, str)) {
        if (str->l>0) {
          iconv(CE_UNKNOW, *str, totyp, out1);
          if (tourl) {
            str_str2url(*out1, out2);
            puts(out2->s);
          }
          str_save(f->name, out1);
          printf("%s \t OK!\n", f->name);
        }
        else {
          printf("%s is not utf8\n", f->name);
        }
      }
    }
    str_frees(str, countof(str));
  }
  return 0;
}

#if 0
//函數輸入Big5字元(pszBig5)，返回Gb簡體字符(pszGbs)
int Big2Gb(const char* pszBig5, char* pszGbs)
{
  wchar_t* wszUnicode = NULL;  //Unicode編碼的字元
  char* pszGbt = NULL; //Gb編碼的繁體字符
  
  //返回的字串  sGb;
  int iLen = 0; //需要轉換的字元數
  
  //計算轉換的字元數
  iLen = MultiByteToWideChar(950, 0, pszBig5, -1, NULL, 0) ;
  //給wszUnicode分配記憶體
  wszUnicode = MALLOC(wchar_t, iLen + 1);
  //轉換Big5碼到Unicode碼，使用了API函數MultiByteToWideChar
  MultiByteToWideChar(950, 0, pszBig5, -1, wszUnicode, iLen);
  
  //計算轉換的字元數
  iLen = WideCharToMultiByte(936, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL) ;
  //給pszGbt分配記憶體
  pszGbt = MALLOC(char, iLen + 1);
  //給pszGbs分配記憶體
  pszGbs = MALLOC(char, iLen + 1);
  //轉換Unicode碼到Gb碼繁體，使用API函數WideCharToMultiByte
  WideCharToMultiByte(936, 0, (PWSTR) wszUnicode, -1, pszGbt, iLen, NULL, NULL) ;
  
  //轉換Gb碼繁體到Gb碼簡體，使用API函數LCMapString
  LCMapString(0x0804, LCMAP_SIMPLIFIED_CHINESE, pszGbt, -1, pszGbs, iLen);
  
  //釋放記憶體
  SAFEFREE(wszUnicode);
  SAFEFREE(pszGbt);
  
  return 0;
}

//函數輸入Gb字元(pszGbs)，返回Big5字元(pszBig5)
int Gb2Big(const char* pszGbs, char* pszBig5)
{
  const char* pszGbt = NULL;  //Gb編碼的繁體字符
  
  wchar_t* wszUnicode = NULL; //Unicode編碼的字元
  //Big5編碼的字元 char* pszBig5=NULL;
  //返回的字串  sBig5;
  int iLen = 0; //需要轉換的字元數
  
  //計算轉換的字元數
  iLen = MultiByteToWideChar(936, 0, pszGbs, -1, NULL, 0) ;
  
  //給pszGbt分配記憶體
  pszGbt = MALLOC(char, iLen * 2 + 1);
  //轉換Gb碼簡體到Gb碼繁體，使用API函數LCMapString
  LCMapString(0x0804, LCMAP_TRADITIONAL_CHINESE, pszGbs, -1, pszGbt, iLen * 2);
  
  //給wszUnicode分配記憶體
  wszUnicode = MALLOC(wchar_t, iLen + 1);
  //轉換Gb碼到Unicode碼，使用了API函數MultiByteToWideChar
  MultiByteToWideChar(936, 0, pszGbt, -1, wszUnicode, iLen);
  
  //計算轉換的字元數
  iLen = WideCharToMultiByte(950, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL) ;
  //給pszBig5分配記憶體
  pszBig5 = MALLOC(char, iLen + 1);
  //轉換Unicode碼到Big5碼，使用API函數WideCharToMultiByte
  WideCharToMultiByte(950, 0, (PWSTR) wszUnicode, -1, pszBig5, iLen, NULL, NULL) ;
  
  //返回Big5碼字元
  
  //釋放記憶體
  SAFEFREE(wszUnicode);
  SAFEFREE(pszGbt);
  return 0;
}

int main(int argc, char** argv)
{
  char* aa = "靖难之役";
  char asdf[ 199 ] = {0};
  //Big2Gb("人類器官衰老時間表", asdf);
  test_codecvt(argc, argv);
  //test_unicodetoutf8("au.txt", "bua.txt");
  return 0;
}
#endif
