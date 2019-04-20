

#ifndef __MFC_TYPEDEF_H__
#define __MFC_TYPEDEF_H__

#include "std/log_c.h"
#include "std/iconv_c.h"
#include <stdlib.h>
#include <io.h>

#include <tchar.h>


int afxTraceEnabled = 0;
#define _AFXCOLL_INLINE
#define ASSERT_KINDOF(a, b)
#define ASSERT_VALID(ptr) assert(ptr!=NULL)
#define AfxIsValidString(x)   (x)!=NULL
#define AfxIsValidAddress(a, b, c)  (a)!=NULL
#define _AFX_INLINE
#define DECLARE_SERIAL(a)
#define DECLARE_DYNAMIC(a)
struct __POSITION { };
typedef __POSITION* POSITION;
#define BEFORE_START_POSITION ((POSITION)-1L)
struct _AFX_DOUBLE { BYTE doubleBits[sizeof(double)]; };
struct _AFX_FLOAT { BYTE floatBits[sizeof(float)]; };

#define VERIFY(x)   (void)(x)
#define _strinc(x)  (x+1)
#define CSTR_CHARSET "gb2312"
/////////////////////////////////////////////////////////////////////////////
typedef char TCHAR;
typedef uchar _TUCHAR;
typedef TCHAR* LPTSTR;
typedef const TCHAR* LPCTSTR;
typedef const TCHAR* LPCSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
/////////////////////////////////////////////////////////////////////////////


#include "std/error_c.h"


#endif // __MFC_TYPEDEF_H__
