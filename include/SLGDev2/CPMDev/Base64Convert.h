#pragma once
#include <string>
using namespace std;
class CBase64Convert
{
public:
  CBase64Convert(void);
  ~CBase64Convert(void);
public:
  static void _enBase64Help(unsigned char chasc[3], unsigned char chuue[4]);
  static void _deBase64Help(unsigned char chuue[4], unsigned char chasc[3]);
  static string enBase64(const char* inbuf, size_t inbufLen);
  static string enBase64(const string& inbuf);
  static int    deBase64(const string& src, char* outbuf);
  static int    deBase64(const char* pSrc , int len , char* outbuf);
  static string deBase64(const string& src);
};

