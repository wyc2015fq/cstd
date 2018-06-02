// stdafx.cpp : source file that includes just the standard includes
//  CPMDev.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

BOOL StrIsIP(const char* cszIP)
{
  if (NULL == cszIP) {
    return false;
  }

  int nLen = strlen(cszIP);

  if (nLen < 7 || nLen > 15) {
    return false;
  }

  int nDot = 0;

  for (int i = 0; i < nLen; ++i) {
    if (cszIP[i] == '.') {
      nDot++;
    }
  }

  return 3 == nDot;
}

//点分IP转换为网络字节序long
unsigned long DotIpToLong(const char* ip)
{
  if (!StrIsIP(ip)) {
    return 0;
  }

  int a  = 0;
  int b  = 0;
  int c  = 0;
  int d  = 0;
  int rs = 0;
  BYTE* pByte = (BYTE*)&rs;
  sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
  pByte[0] = a;
  pByte[1] = b;
  pByte[2] = c;
  pByte[3] = d;
  return rs;
}

//网络字节序long转换为点分IP
void LongToDotIp(char ip[], unsigned long l)
{
  if (NULL == ip || l == 0) {
    return;
  }

  BYTE* pByte = (BYTE*)&l;
  sprintf(ip, "%d.%d.%d.%d", pByte[0], pByte[1], pByte[2], pByte[3]);
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
