// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6473EA4E_8EFF_48EB_B400_699D9EF6FDC3__INCLUDED_)
#define AFX_STDAFX_H__6473EA4E_8EFF_48EB_B400_699D9EF6FDC3__INCLUDED_

#define DEBUG_NEW new

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )
// Insert your headers here
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#ifdef _DEBUG
#define _OUTPUT_INFO_
#endif

typedef long LONG;

//#include <windows.h>
#include <assert.h>
#ifdef _DEBUG
#define ASSERT assert
#else
#define ASSERT()
#endif
#include <map>
using namespace std;
#include "memmap.inl"

#ifdef WIN32
#else
#define BI_RGB        0
typedef char* LPTSTR;
typedef unsigned long DWORD;
typedef short WORD;
typedef unsigned short USHORT;
typedef unsigned char BYTE;
typedef long long int64;
typedef unsigned int UINT;

typedef struct tagBITMAPINFOHEADER {
  DWORD      biSize;
  LONG       biWidth;
  LONG       biHeight;
  WORD       biPlanes;
  WORD       biBitCount;
  DWORD      biCompression;
  DWORD      biSizeImage;
  LONG       biXPelsPerMeter;
  LONG       biYPelsPerMeter;
  DWORD      biClrUsed;
  DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
  BYTE    rgbBlue;
  BYTE    rgbGreen;
  BYTE    rgbRed;
  BYTE    rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPFILEHEADER {
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} BITMAPFILEHEADER,  *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER    bmiHeader;
  RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

#define  ZeroMemory(a, c)  memset(a, 0, c)
#define InterlockedIncrement(x)   (0)
#define InterlockedDecrement(x)  (0)

#endif



#include "CPMDev.h"
#include "./sock/MemPool.h"
#include "./test/DebugAssistant.h"
//#include "./test/sftime.hpp"
#include "./Base64Convert.h"

extern unsigned long DotIpToLong(const char* ip);
extern void LongToDotIp(char ip[], unsigned long l);
extern BOOL StrIsIP(const char* cszIP);

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6473EA4E_8EFF_48EB_B400_699D9EF6FDC3__INCLUDED_)
