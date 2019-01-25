#ifndef __UNICODE_H__
#define __UNICODE_H__

#include <string.h>

#ifdef __DEFLINUX__
//#include <netinet/in.h>
#endif

//#include<Winsock2.h>
//#include <windows.h>

// gb uni
unsigned short giGB2312[21243][2] = {
#include "unigb_tab.txt"
};

const unsigned short giGBCount = 21243;
static unsigned short tab_gb2un[1 << 16];
static unsigned short tab_un2gb[1 << 16];

int make_tab()
{
  static int inited = 0;
  if (!inited) {
    int i;
    inited = 1;
    for (i = 0; i < 21243; ++i) {
      unsigned short a, b;
      a = giGB2312[i][0];
      b = giGB2312[i][1];
      tab_gb2un[a] = b;
      tab_un2gb[b] = a;
    }
  }
  return inited;
}
int inited = make_tab();

#define _istlead(ch) ((unsigned char)(ch)>=0x80)

int enc_gb_to_ucs_one(const unsigned char* gb, unsigned long* unic)
{
  unsigned short unicode = 0;
  if (!_istlead(gb[0])) {
    *unic = (gb[0]);
    return 1;
  } else {
    *unic = tab_gb2un[*(unsigned short*)(gb)];
    return 2;
  }
  return 0;
}
int enc_ucs_to_gb_one(unsigned long unic, unsigned char* gb)
{
  //unic = ntohs(unic);
  if (!((unic) & 0xff00)) {
    gb[0] = (unic);
    return 1;
  } else {
    *(unsigned short*)(gb) = tab_un2gb[(unsigned short)unic];
    return 2;
  }
  return 0;
}

//1) 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.

/*****************************************************************************
* 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
*
* 參数:
* unic 字符的Unicode编码值
* pOutput 指向输出的用于存储UTF8编码值的缓冲区的指针
* outsize pOutput缓冲的大小
*
* 返回值:
* 返回转换后的字符的UTF8编码所占的字节数, 假设出错则返回 0 .
*
* 注意:
* 1. UTF8没有字节序问题, 可是Unicode有字节序要求;
* 字节序分为大端(Big Endian)和小端(Little Endian)两种;
* 在Intel处理器中採用小端法表示, 在此採用小端法表示. (低地址存低位)
* 2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
****************************************************************************/
int enc_ucs_to_utf8_one(unsigned long unic, unsigned char* pOutput)
{
  //assert(pOutput != NULL);
  if ( unic <= 0x0000007F ) {
    // * U-00000000 - U-0000007F: 0xxxxxxx
    *pOutput = (unic & 0x7F);
    return 1;
  } else if ( unic >= 0x00000080 && unic <= 0x000007FF ) {
    // * U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
    *(pOutput + 1) = (unic & 0x3F) | 0x80;
    *pOutput = ((unic >> 6) & 0x1F) | 0xC0;
    return 2;
  } else if ( unic >= 0x00000800 && unic <= 0x0000FFFF ) {
    // * U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
    *(pOutput + 2) = (unic & 0x3F) | 0x80;
    *(pOutput + 1) = ((unic >> 6) & 0x3F) | 0x80;
    *pOutput = ((unic >> 12) & 0x0F) | 0xE0;
    return 3;
  } else if ( unic >= 0x00010000 && unic <= 0x001FFFFF ) {
    // * U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    *(pOutput + 3) = (unic & 0x3F) | 0x80;
    *(pOutput + 2) = ((unic >> 6) & 0x3F) | 0x80;
    *(pOutput + 1) = ((unic >> 12) & 0x3F) | 0x80;
    *pOutput = ((unic >> 18) & 0x07) | 0xF0;
    return 4;
  } else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF ) {
    // * U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    *(pOutput + 4) = (unic & 0x3F) | 0x80;
    *(pOutput + 3) = ((unic >> 6) & 0x3F) | 0x80;
    *(pOutput + 2) = ((unic >> 12) & 0x3F) | 0x80;
    *(pOutput + 1) = ((unic >> 18) & 0x3F) | 0x80;
    *pOutput = ((unic >> 24) & 0x03) | 0xF8;
    return 5;
  } else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF ) {
    // * U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    *(pOutput + 5) = (unic & 0x3F) | 0x80;
    *(pOutput + 4) = ((unic >> 6) & 0x3F) | 0x80;
    *(pOutput + 3) = ((unic >> 12) & 0x3F) | 0x80;
    *(pOutput + 2) = ((unic >> 18) & 0x3F) | 0x80;
    *(pOutput + 1) = ((unic >> 24) & 0x3F) | 0x80;
    *pOutput = ((unic >> 30) & 0x01) | 0xFC;
    return 6;
  }
  return 0;
}
// 表 1. UTF-8的编码规则
// | Unicode符号范围 | UTF-8编码方式
// n | (十六进制)  | (二进制)
// ---+-----------------------+------------------------------------------------------
// 1 | 0000 0000 - 0000 007F |      0xxxxxxx
// 2 | 0000 0080 - 0000 07FF |     110xxxxx 10xxxxxx
// 3 | 0000 0800 - 0000 FFFF |    1110xxxx 10xxxxxx 10xxxxxx
// 4 | 0001 0000 - 0010 FFFF |   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// 5 | 0020 0000 - 03FF FFFF |  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

int enc_get_utf8_size(unsigned char c)
{
  if (c >= 0xFC) {
    return 6;
  } else if (c >= 0xF8) {
    return 5;
  } else if (c >= 0xF0) {
    return 4;
  } else if (c >= 0xE0) {
    return 3;
  } else if (c >= 0xC0) {
    return 2;
  }
  return 1;
}

// 2) 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
/*****************************************************************************
* 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
*
* 參数:
* pInput 指向输入缓冲区, 以UTF-8编码
* unic 指向输出缓冲区, 其保存的数据即是Unicode编码值,
*  类型为unsigned long .
*
* 返回值:
* 成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
*
* 注意:
* 1. UTF8没有字节序问题, 可是Unicode有字节序要求;
* 字节序分为大端(Big Endian)和小端(Little Endian)两种;
* 在Intel处理器中採用小端法表示, 在此採用小端法表示. (低地址存低位)
****************************************************************************/
int enc_utf8_to_ucs_one(const unsigned char* pInput, unsigned long* unic)
{
  //assert(pInput != NULL && unic != NULL);
  // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
  char b1, b2, b3, b4, b5, b6;
  *unic = 0x0; // 把 *unic 初始化为全零
  int utfbytes = enc_get_utf8_size(*pInput);
  unsigned char* pOutput = (unsigned char*) unic;
  switch ( utfbytes ) {
  case 1:
    *pOutput = *pInput;
    break;
  case 2:
    b1 = *pInput;
    b2 = *(pInput + 1);
    if ( (b2 & 0xE0) != 0x80 ) {
      return 0;
    }
    *pOutput = (b1 << 6) + (b2 & 0x3F);
    *(pOutput + 1) = (b1 >> 2) & 0x07;
    break;
  case 3:
    b1 = *pInput;
    b2 = *(pInput + 1);
    b3 = *(pInput + 2);
    if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) ) {
      return 0;
    }
    *pOutput = (b2 << 6) + (b3 & 0x3F);
    *(pOutput + 1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
    break;
  case 4:
    b1 = *pInput;
    b2 = *(pInput + 1);
    b3 = *(pInput + 2);
    b4 = *(pInput + 3);
    if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
         || ((b4 & 0xC0) != 0x80) ) {
      return 0;
    }
    *pOutput = (b3 << 6) + (b4 & 0x3F);
    *(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
    *(pOutput + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
    break;
  case 5:
    b1 = *pInput;
    b2 = *(pInput + 1);
    b3 = *(pInput + 2);
    b4 = *(pInput + 3);
    b5 = *(pInput + 4);
    if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
         || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) ) {
      return 0;
    }
    *pOutput = (b4 << 6) + (b5 & 0x3F);
    *(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
    *(pOutput + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
    *(pOutput + 3) = (b1 << 6);
    break;
  case 6:
    b1 = *pInput;
    b2 = *(pInput + 1);
    b3 = *(pInput + 2);
    b4 = *(pInput + 3);
    b5 = *(pInput + 4);
    b6 = *(pInput + 5);
    if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
         || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
         || ((b6 & 0xC0) != 0x80) ) {
      return 0;
    }
    *pOutput = (b5 << 6) + (b6 & 0x3F);
    *(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
    *(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
    *(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
    break;
  default:
    return 0;
    break;
  }
  return utfbytes;
}

int gb_to_ucs2le(char* unicode, int maxoutlen, const char* gb, int len)
{
  int i, outlen = 0;
  unsigned long unic = 0;
  maxoutlen -= 4;
  maxoutlen /= 2;
  for (i = 0; i < len;) {
    i += enc_gb_to_ucs_one((unsigned char*)gb + i, &unic);
    ((unsigned short* )unicode)[outlen++] = unic;
  }
  return outlen * 2;
}
int ucs2le_to_gb(char* gb, int maxoutlen, const char* unicode, int len)
{
  int i, outlen = 0;
  maxoutlen -= 4;
  len /= 2;
  for (i = 0; i < len; ++i) {
    outlen += enc_ucs_to_gb_one(((unsigned short* )unicode)[i], (unsigned char*)gb + outlen);
  }
  return outlen;
}

int ucs2le_to_utf8(char* utf, int maxoutlen, const char* unicode, int len)
{
  int i, outlen = 0;
  maxoutlen -= 4;
  len /= 2;
  for (i = 0; i < len && outlen < maxoutlen; ++i) {
    outlen += enc_ucs_to_utf8_one(((unsigned short* )unicode)[i], (unsigned char*)utf + outlen);
  }
  return outlen;
}
int utf8_to_ucs2le(char* unicode, int maxoutlen, const char* utf, int len)
{
  int i, outlen = 0;
  unsigned long unic = 0;
  maxoutlen /= 2;
  for (i = 0; i < len && outlen < maxoutlen; ) {
    i += enc_utf8_to_ucs_one((unsigned char*)utf + i, &unic);
    ((unsigned short* )unicode)[outlen++] = (unsigned short)unic;
  }
  return outlen * 2;
}

int gb_to_utf8(char* utf, int maxoutlen, const char* gb, int len)
{
  int i, outlen = 0;
  unsigned long unic = 0;
  maxoutlen -= 4;
  for (i = 0; i < len && outlen < maxoutlen; ) {
    i += enc_gb_to_ucs_one((unsigned char*)gb + i, &unic);
    outlen += enc_ucs_to_utf8_one(unic, (unsigned char*)utf + outlen);
  }
  return outlen;
}
int utf8_to_gb(char* gb, int maxoutlen, const char* utf, int len)
{
  int i, outlen = 0;
  unsigned long unic = 0;
  maxoutlen -= 4;
  for (i = 0; i < len && outlen < maxoutlen; ) {
    i += enc_utf8_to_ucs_one((unsigned char*)utf + i, &unic);
    outlen += enc_ucs_to_gb_one(unic, (unsigned char*)gb + outlen);
  }
  return outlen;
}

#if 0
unsigned short gb2un(unsigned short int gb)
{
  int i;
  for (i = 0; i < giGBCount; i++) {
    if (giGB2312[i][0] == gb) {
      return htons(giGB2312[i][1]);
    }
  }
  return 0xFFFF;
}
unsigned short un2gb(unsigned short int unicode)
{
  int i;
  for (i = 0; i < giGBCount; i++) {
    if (giGB2312[i][1] == ntohs(unicode)) {
      return giGB2312[i][0];
    }
  }
  return 0xFFFF;
}
int gb2unicode(unsigned short int* unicode, char* gb, int len)
{
  int i, j;
  unsigned short iTmp;
  i = 0;
  for (j = 0; i < len; j++) {
    if ((unsigned char)gb[i] <= 0x80) {
      unicode[j] = gb[i];
      unicode[j] = htons(unicode[j]);
      i++;
    } else {
      unicode[j] = gb2un(*(unsigned short*)(gb + i));
      i += 2;
    }
  }
  return j * 2;
}
int unicode2gb(char* gb, const unsigned short* unicode, int len)
{
  int j;
  int i;
  unsigned short iTmp;
  len = len / 2;
  j = 0;
  for (i = 0; i < len; i++) {
    if (!(ntohs(unicode[i]) & 0xff00)) {
      gb[j] = ntohs(unicode[i]);
      j++;
    } else {
      iTmp = un2gb(unicode[i]);
      /* printf(" 0x%04X,0x%04X",unicode[i],iTmp);*/
      *(unsigned short*)(gb + j) = iTmp;
      j += 2;
    }
  }
  return j;
}
#endif


//测试代码：（在32winxp、RedLinux 位下测试通过）
#ifdef TEST
#include <stdio.h>

void test_unigb()
{
  const char* pU = "华";
  unsigned short buf[100] = {0};
  if (gb2unicode(buf, pU, 1) == 0xFFFF) {
    cout << "fail" << endl;
    return;
  }
  cout << hex << buf[0] << endl;
  return 0;
}
#endif // TEST

#endif // __UNICODE_H__
