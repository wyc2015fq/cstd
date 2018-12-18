
#ifndef __CDUMPCONTEXT_HPP__
#define __CDUMPCONTEXT_HPP__

#include "log_c.h"
#include "iconv_c.h"

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
struct _AFX_DOUBLE  { BYTE doubleBits[sizeof(double)]; };
struct _AFX_FLOAT   { BYTE floatBits[sizeof(float)]; };

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


class CDumpContext
{
public:
  int m_nDepth;
public:
  // CDumpContext
  int GetDepth() const
  { return m_nDepth; }
  void SetDepth(int nNewDepth)
  { m_nDepth = nNewDepth; }
  CDumpContext(const CDumpContext& /* dcSrc */)
  { }
  void operator=(const CDumpContext& /* dcSrc */)
  { }
  void OutputString(LPCTSTR lpsz) {
    // all CDumpContext output is controlled by afxTraceEnabled
    if (!afxTraceEnabled)
      return;

    log_puts(lpsz);
    return;
  }

  CDumpContext()
  {
    m_nDepth = 0;
  }

  void Flush()
  {
  }

  CDumpContext& operator<<(LPCTSTR lpsz)
  {
    if (lpsz == NULL)
    {
      OutputString("(NULL)");
      return *this;
    }

#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
    if (!afxTraceEnabled)
      return *this;
#endif //_DEBUG

    char szBuffer[512];
    LPTSTR lpBuf = szBuffer;
    while (*lpsz != '\0')
    {
      if (lpBuf > szBuffer + countof(szBuffer) - 3)
      {
        *lpBuf = '\0';
        OutputString(szBuffer);
        lpBuf = szBuffer;
      }
      if (*lpsz == '\n')
        *lpBuf++ = '\r';
      *lpBuf++ = *lpsz++;
    }
    *lpBuf = '\0';
    OutputString(szBuffer);
    return *this;
  }

  CDumpContext& operator<<(BYTE by)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("%d"), (int)by);
    OutputString(szBuffer);
    return *this;
  }

  CDumpContext& operator<<(WORD w)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("%u"), (UINT) w);
    OutputString(szBuffer);

    return *this;
  }
  CDumpContext& operator<<(UINT u)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("0x%X"), u);
    OutputString(szBuffer);

    return *this;
  }
  
  #if 0
  CDumpContext& operator<<(INT_PTR u)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("0x%X"), u);
    OutputString(szBuffer);

    return *this;
  }
#endif

  CDumpContext& operator<<(LONG l)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("%ld"), l);
    OutputString(szBuffer);

    return *this;
  }

  CDumpContext& operator<<(DWORD dw)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("%lu"), dw);
    OutputString(szBuffer);

    return *this;
  }

  CDumpContext& operator<<(int n)
  {
    char szBuffer[32];

    snprintf(szBuffer, 32, ("%d"), n);
    OutputString(szBuffer);

    return *this;
  }

  CDumpContext& operator<<(float f)
  {
    char szBuffer[32];
    snprintf(szBuffer, 32, ("%f"), f);
    OutputString(szBuffer);
    return *this;
  }

  CDumpContext& operator<<(double d)
  {
    char szBuffer[32];
    snprintf(szBuffer, 32, ("%lf"), d);
    OutputString(szBuffer);
    return *this;
  }

  CDumpContext& operator<<(const void* lp)
  {
    char szBuffer[32];

    // prefix a pointer with "$" and print in hex
    snprintf(szBuffer, 32, ("$%lX"), (LONG)lp);
    OutputString(szBuffer);

    return *this;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Formatted output

  void HexDump(LPCTSTR lpszLine, BYTE* pby,
    int nBytes, int nWidth)
    // do a simple hex-dump (8 per line) to a CDumpContext
    //  the "lpszLine" is a string to print at the start of each line
    //    (%lx should be used to expand the current address)
  {
    ASSERT(nBytes > 0);
    ASSERT(nWidth > 0);
    ASSERT(AfxIsValidString(lpszLine));
    ASSERT(AfxIsValidAddress(pby, nBytes, FALSE));

#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
    if (!afxTraceEnabled)
      return;
#endif //_DEBUG

    int nRow = 0;
    char szBuffer[32];

    while (nBytes--)
    {
      if (nRow == 0)
      {
        snprintf(szBuffer, 32, lpszLine, pby);
        *this << szBuffer;
      }

      snprintf(szBuffer, 32, (" %02X"), *pby++);
      *this << szBuffer;

      if (++nRow >= nWidth)
      {
        *this << ("\n");
        nRow = 0;
      }
    }
    if (nRow != 0)
      *this << ("\n");
  }

  /////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
  // special version for ANSI characters
  CDumpContext& operator<<(LPCSTR lpsz)
  {
    if (lpsz == NULL)
    {
      OutputString(L"(NULL)");
      return *this;
    }

#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
    if (!afxTraceEnabled)
      return *this;
#endif //_DEBUG

    // limited length
    char szBuffer[512];
    _mbstowcsz(szBuffer, lpsz, _countof(szBuffer));
    return *this << szBuffer;
  }
#else   //_UNICODE
  // special version for WIDE characters
  CDumpContext& operator<<(LPCWSTR lpsz)
  {
    if (lpsz == NULL)
    {
      OutputString("(NULL)");
      return *this;
    }

#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
    if (!afxTraceEnabled)
      return *this;
#endif //_DEBUG

    // limited length
    char szBuffer[512];
    _wcstombsz(szBuffer, lpsz, _countof(szBuffer));
    return *this << szBuffer;
  }
#endif  //!_UNICODE


};

#endif // __CDUMPCONTEXT_HPP__
