#include <windows.h>
#include <tchar.h>
#include <stdio.h>
void GBKSimplified2GBKTraditional(LPTSTR szSimplified)
{
  LCID dwLocale;
  WORD wLangID;
  int nLength;
  char* pBuffer;
  wLangID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
  dwLocale = MAKELCID(wLangID, SORT_CHINESE_PRC);
  nLength = LCMapString(dwLocale, LCMAP_TRADITIONAL_CHINESE, (LPCTSTR)szSimplified, strlen(szSimplified), NULL, 0);
  pBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (nLength + 1));
  pBuffer[nLength] = 0;
  LCMapString(dwLocale, LCMAP_TRADITIONAL_CHINESE, (LPCTSTR)szSimplified, strlen(szSimplified),
      pBuffer, nLength);
  strcpy(szSimplified, pBuffer);
  free(pBuffer);
  return;
}
// 简转繁
char* j2f(char* S)
{
  char* buf = (S);
  int n = strlen(buf);
  char* BS = S;
  int r = LCMapString(
      MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC),
      LCMAP_TRADITIONAL_CHINESE,
      BS, n, NULL, 0);
  TCHAR* ft = (TCHAR*)malloc(sizeof(TCHAR) * (n + 1));
  TCHAR* getbg = (TCHAR*)malloc(sizeof(TCHAR) * (n + 1));
  r = LCMapString(
      MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC),
      LCMAP_TRADITIONAL_CHINESE,
      BS, n, ft, n); //涴跺api詻菽
  _tcsncpy(getbg, ft, n);
  free(ft);
  return getbg;
}
//WCHAR 转换成 CHAR函数
void ConvertUnicodeToAnsi(WCHAR* Source, char* obj)
{
  DWORD dwNum = WideCharToMultiByte(950, 0, Source, -1, NULL, 0, NULL, FALSE);
  WideCharToMultiByte(950, 0, Source, -1, obj, dwNum, NULL, FALSE);
}
//CHAR TO WCHAR
void ConvertAnsiTounicode(char* source, WCHAR* obj)
{
  int nLength = 0;
  nLength = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
  MultiByteToWideChar(CP_ACP, 0, source, -1, obj, nLength);
}
// 输入Big5字符，返回Gb简体字符
char* Big2Gb(char* sBig)
{
  char* pszBig5 = NULL; //Big5编码的字符
  wchar_t* wszUnicode = NULL; //Unicode编码的字符
  char* pszGbt = NULL; //Gb编码的繁体字符
  char* pszGbs = NULL; //Gb编码的简体字符
  char* sGb; //返回的字符串
  int iLen = 0; //需要转换的字符数
  pszBig5 = sBig; //读入需要转换的字符参数
  //计算转换的字符数
  iLen = MultiByteToWideChar(950, 0, pszBig5, -1, NULL, 0) ;
  //给wszUnicode分配内存
  wszUnicode = (wchar_t*)malloc(sizeof(wchar_t) * (iLen + 1));
  //转换Big5码到Unicode码，使用了API函数MultiByteToWideChar
  MultiByteToWideChar(950, 0, pszBig5, -1, wszUnicode, iLen);
  //计算转换的字符数
  iLen = WideCharToMultiByte(936, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL) ;
  //给pszGbt分配内存
  pszGbt = (char*)malloc(sizeof(char) * (iLen + 1));
  //给pszGbs分配内存
  pszGbs = (char*)malloc(sizeof(char) * (iLen + 1));
  //转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte
  WideCharToMultiByte(936, 0, (PWSTR) wszUnicode, -1, pszGbt, iLen, NULL, NULL) ;
  //转换Gb码繁体到Gb码简体，使用API函数LCMapString
  LCMapString(0x0804, LCMAP_SIMPLIFIED_CHINESE, pszGbt, -1, pszGbs, iLen);
  //返回Gb码简体字符
  sGb = pszGbs;
  //释放内存
  free(wszUnicode);
  free(pszGbt);
  //free(pszGbs);
  return sGb;
}
//函数输入Gb字符，返回Big5字符
char* Gb2Big(char* sGb)
{
  char* pszGbt = NULL; //Gb编码的繁体字符
  char* pszGbs = NULL; //Gb编码的简体字符
  wchar_t* wszUnicode = NULL; //Unicode编码的字符
  char* pszBig5 = NULL; //Big5编码的字符
  char* sBig5; //返回的字符串
  int iLen = 0; //需要转换的字符数
  pszGbs = sGb; //读入需要转换的字符参数
  //计算转换的字符数
  iLen = MultiByteToWideChar(936, 0, pszGbs, -1, NULL, 0) ;
  //给pszGbt分配内存
  pszGbt = (char*)malloc(sizeof(char) * (iLen * 2 + 1));
  //转换Gb码简体到Gb码繁体，使用API函数LCMapString
  LCMapString(0x0804, LCMAP_TRADITIONAL_CHINESE, pszGbs, -1, pszGbt, iLen * 2);
  //给wszUnicode分配内存
  wszUnicode = (wchar_t*)malloc(sizeof(wchar_t) * (iLen + 1));
  //转换Gb码到Unicode码，使用了API函数MultiByteToWideChar
  MultiByteToWideChar(936, 0, pszGbt, -1, wszUnicode, iLen);
  //计算转换的字符数
  iLen = WideCharToMultiByte(950, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL) ;
  //给pszBig5分配内存
  pszBig5 = (char*)malloc(sizeof(char) * (iLen + 1));
  //转换Unicode码到Big5码，使用API函数WideCharToMultiByte
  WideCharToMultiByte(950, 0, (PWSTR) wszUnicode, -1, pszBig5, iLen, NULL, NULL) ;
  //返回Big5码字符
  sBig5 = pszBig5;
  //释放内存
  free(wszUnicode);
  free(pszGbt);
  free(pszBig5);
  return sBig5;
}
// 判断是否gb2312编码字符
int isgb(const void* pzInfo, int length)
{
  //int nWSize = MultiByteToWideChar( 936,MB_ERR_INVALID_CHARS,pzInfo,-1,NULL,0 );
  int error, nWSize;
  nWSize = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, (const char*)pzInfo, length, NULL, 0);
  if (0 == nWSize) {
    error = GetLastError();
    if (error == ERROR_NO_UNICODE_TRANSLATION) {
      return FALSE;
    }
  }
  //判断是否是CP_UTF8,只要把936用CP_UTF8代替即可
  return TRUE;
}
int isuni(const void* pzInfo, int length)
{
  //判断是否是CP_UTF8,只要把936用CP_UTF8代替即可
  return TRUE;
}
int isutf(const void* pzInfo, int length)
{
  int error, nWSize;
  nWSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char*)pzInfo, length, NULL, 0);
  if (0 == nWSize) {
    error = GetLastError();
    if (error == ERROR_NO_UNICODE_TRANSLATION) {
      return FALSE;
    }
  }
  //判断是否是gb2312,只要把 CP_UTF8 用936代替即可.
  return TRUE;
}
// 判断字符串是否UTF8编码
// UTF8是以8bits即1Bytes为编码的最基本单位，当然也可以有基于16bits和32bits的形式，分别称为UTF16和UTF32，但目前用得不多，而UTF8则被广泛应用在文件储存和网络传输中。
// 编码原理
// 先看这个模板：
// UCS-4 range (hex.) UTF-8 octet sequence (binary)
// 0000 0000-0000 007F 0xxxxxxx
// 0000 0080-0000 07FF 110xxxxx 10xxxxxx
// 0000 0800-0000 FFFF 1110xxxx 10xxxxxx 10xxxxxx
// 0001 0000-001F FFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// 0020 0000-03FF FFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 0400 0000-7FFF FFFF 1111110x 10xxxxxx ... 10xxxxxx
// 编码步骤：
// 1) 首先确定需要多少个8bits(octets)
// 2) 按照上述模板填充每个octets的高位bits
// 3) 把字符的bits填充至x中，字符顺序：低位→高位，UTF8顺序：最后一个octet的最末位x→第一个octet最高位x
// 根据UTF8编码,最多可由6个字节组成,所以UTF8是1-6字节编码组成
int IsTextUTF8(const char* str, int length)
{
  int i;
  DWORD nBytes = 0; //UFT8可用1-6个字节编码,ASCII用一个字节
  UCHAR chr;
  BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
  for (i = 0; i < length; i++) {
    chr = *(str + i);
    if ((chr & 0x80) != 0) { // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
      bAllAscii = FALSE;
    }
    if (nBytes == 0) { //如果不是ASCII码,应该是多字节符,计算字节数
      if (chr >= 0x80) {
        if (chr >= 0xFC && chr <= 0xFD) {
          nBytes = 6;
        }
        else if (chr >= 0xF8) {
          nBytes = 5;
        }
        else if (chr >= 0xF0) {
          nBytes = 4;
        }
        else if (chr >= 0xE0) {
          nBytes = 3;
        }
        else if (chr >= 0xC0) {
          nBytes = 2;
        }
        else {
          return FALSE;
        }
        nBytes--;
      }
    }
    else { //多字节符的非首字节,应为 10xxxxxx
      if ((chr & 0xC0) != 0x80) {
        return FALSE;
      }
      nBytes--;
    }
  }
  if (nBytes > 0) { //违返规则
    return FALSE;
  }
  if (bAllAscii) { //如果全部都是ASCII, 说明不是UTF-8
    return FALSE;
  }
  return TRUE;
}
typedef unsigned char uint8_t;
int isutf8(const char* s, int ns)
{
  uint8_t x = 0, i = 0, j = 0, nbytes = 0, n = 0;
  for (i = 1; i < 7; i++) {
    x = (uint8_t)(255 << i);
    if (((uint8_t)*s & x) == x) {
      n = nbytes = (8 - i);
      for (j = 0; (j < nbytes && j < ns); j++) {
        if ((uint8_t)s[j] <= 0x80 && (uint8_t)s[j] >= 0xc0) {
          break;
        }
        else {
          n--;
        }
      }
      if (n == 0) {
        return nbytes;
      }
    }
  }
  return 0;
}
//
int isgbk(const char* s, size_t ns)
{
  if (ns > 2 && (uint8_t)*s >= 0x81 && (uint8_t)*s <= 0xfe
      && (
          ((uint8_t) * (s + 1) >= 0x80 && (uint8_t) * (s + 1) <= 0x7e)
          || ((uint8_t) * (s + 1) >= 0xa1 && (uint8_t) * (s + 1) <= 0xfe)
      )
     ) {
    return 1;
  }
  return 0;
}
//
int isGBKCode(const char* s, size_t ns)
{
  unsigned char ch1;
  unsigned char ch2;
  if (ns >= 2) {
    ch1 = (unsigned char)s[0];
    ch2 = (unsigned char)s[1];
    if (ch1 >= 129 && ch1 <= 254 && ch2 >= 64 && ch2 <= 254) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}
// 是否gb2312
int IsGB(const char* pText)
{
  unsigned char* sqChar = (unsigned char*)pText;
  if (sqChar[0] >= 0xa1) {
    if (sqChar[0] == 0xa3) {
      return 1;//全角字符
    }
    else {
      return 2;//汉字
    }
  }
  else {
    return 0;//英文、数字、英文标点
  }
}
/***************************************************
* Function: IsChinese
* Description: 是否有中文
* Input: pzInfo：待判断字符
* Output:
* Return:
* Others:
***************************************************/
BOOL IsChinese(const char* pzInfo)
{
  int i;
  char* pText = (char*)pzInfo;
  while (*pText != '\0') {
    i = IsGB(pText);
    switch (i) {
    case 0:
      pText++;
      break;
    case 1:
      pText++;
      pText++;
      break;
    case 2:
      pText++;
      pText++;
      return TRUE;
      break;
    }
  }
  return FALSE;
}
int gb2uni(const void* src, int srclen, void* dst)
{
  int utflen = 2 * srclen;
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)dst;
  //unilen = utf2uni((char*)src, srclen, uni);//ff fe e3 4e
  *uni++ = 0xfeff;
  unilen = 1 + MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (const char*)src, srclen, uni, unilen);
  return 2 * unilen;
}
int utf2uni(const void* src, int srclen, void* dst)
{
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)dst;
  unilen = 2 * MultiByteToWideChar(CP_UTF8, 0, (const char*)src, srclen, uni, unilen);
  return unilen;
}
int uni2gb(const void* src, int srclen, void* dst)
{
  int gblen = 2 * srclen;
  gblen = WideCharToMultiByte(CP_ACP , 0, (const unsigned short*)src, srclen / 2, (char*)dst, gblen, NULL, NULL);
  return gblen;
}
int uni2utf(const void* src, int srclen, void* dst)
{
  int gblen = 2 * srclen;
  gblen = WideCharToMultiByte(CP_UTF8 , 0, (const unsigned short*)src, srclen / 2, (char*)dst, gblen, NULL, NULL);
  return gblen;
}
// UTF8格式转换为ASCII格式字符串
int utf2gb(const void* src, int srclen, void* dst)
{
  int gblen = 2 * srclen;
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)malloc(unilen);
  //memset(uni, 0, unilen);
  //unilen = utf2uni((char*)src, srclen, uni);
  unilen = MultiByteToWideChar(CP_UTF8, 0, (const char*)src, srclen, uni, unilen);
  gblen = WideCharToMultiByte(CP_ACP , 0, uni, unilen, (char*)dst, gblen, NULL, NULL);
  free(uni);
  return gblen;
}
// 将acii字符串转换为utf8格式; 返回转换后的长度
// szBuf:原字符串缓冲区
// szDest:utf-8目标
int gb2utf(const void* src, int srclen, void* dst)
{
  int utflen = 2 * srclen;
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)malloc(unilen);
  //unilen = utf2uni((char*)src, srclen, uni);
  unilen = MultiByteToWideChar(CP_ACP, 0, (const char*)src, srclen, uni, unilen);
  utflen = WideCharToMultiByte(CP_UTF8 , 0, uni, unilen, (char*)dst, utflen, NULL, NULL);
  free(uni);
  return utflen;
}
// Big5 => GBK：
// い地チ㎝瓣 --> 中華人民共和國
int big2gbk(const char* szBuf, int nStrLen, char* pws)
{
  if (NULL != pws) {
    BOOL bValue = FALSE;
    int nReturn = MultiByteToWideChar(950, 0, szBuf, nStrLen, (wchar_t*)pws, nStrLen);
    nReturn = WideCharToMultiByte(936, 0, (wchar_t*)pws, nReturn, pws, nStrLen, "?", &bValue);
  }
  return sizeof(wchar_t) * nStrLen;
}
// GBK => Big5
// 中華人民共和國 --> い地チ㎝瓣
int gbk2big(const char* szBuf, int nStrLen, char* pws)
{
  if (NULL != pws) {
    BOOL bValue = FALSE;
    MultiByteToWideChar(936, 0, szBuf, nStrLen, (wchar_t*)pws, nStrLen);
    WideCharToMultiByte(950, 0, (wchar_t*)pws, nStrLen, pws, nStrLen, "?", &bValue);
  }
  return sizeof(wchar_t) * nStrLen;
}
// GB2312 => GBK
// 中华人民共和国 --> 中華人民共和國
int gb2gbk(const TCHAR* szBuf, int nStrLen, TCHAR* pcBuf)
{
  WORD wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
  int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nStrLen, NULL, 0);
  if (NULL != pcBuf) {
    wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
    LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
  }
  return sizeof(char) * nReturn;
}
// GBK =〉GB2312
// 中華人民共和國 --> 中华人民共和国
int gbk2gb(const TCHAR* szBuf, int nStrLen, TCHAR* pcBuf)
{
  WORD wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
  int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nStrLen, NULL, 0);
  if (NULL != pcBuf) {
    wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
    LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
  }
  return sizeof(char) * nReturn;
}
int gb2big(const char* szBuf, int nStrLen, char* pcBuf)
{
  int len = gb2gbk(szBuf, nStrLen, 0);
  TCHAR* gbk = (TCHAR*)malloc(len);
  gb2gbk(szBuf, nStrLen, gbk);
  len = gbk2big(gbk, len, pcBuf);
  free(gbk);
  return len;
}
int big2gb(const char* szBuf, int nStrLen, char* pcBuf)
{
  int len = big2gbk(szBuf, nStrLen, 0);
  TCHAR* gbk = (TCHAR*)malloc(len);
  len = big2gbk(szBuf, nStrLen, gbk);
  len = gbk2gb(gbk, len, gbk);
  free(gbk);
  return len;
}
//1.str 转GBK繁体
//2.str 转UNICODE
//3.STR 转BIG5
int test_GBK2()
{
  char p[200] = { 0 };
  WCHAR buf[200] = { 0 };
  strcpy(p, "语言");
  GBKSimplified2GBKTraditional(p);
  ConvertAnsiTounicode(p, buf);
  strcpy(p, "");
  ConvertUnicodeToAnsi(buf, p);
  return 0;
}

