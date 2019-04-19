
#ifndef __CDUMPCONTEXT_HPP__
#define __CDUMPCONTEXT_HPP__


#include "mfc.h"

class CDumpContext : public DumpContext
{
public:
	int m_nDepth;
public:
	// CDumpContext
	int GetDepth() const
	{
		return m_nDepth;
	}
	void SetDepth(int nNewDepth)
	{
		m_nDepth = nNewDepth;
	}
	DumpContext* OutputString(const char* fmt, ...) {
		// all CDumpContext output is controlled by afxTraceEnabled
		if (!afxTraceEnabled)
			return this;

		char buf[4096];
		int len;
		va_list arglist;
		va_start(arglist, fmt);
		len = vsnprintf(buf, countof(buf), fmt, arglist);
		char* p = buf;
		strim(&p, &len, "\r\n");
		p[len] = 0;
		log_puts(p);
		va_end(arglist);
		return this;
	}

	CDumpContext()
	{
		m_nDepth = 0;
	}

	void Flush()
	{
	}


  DumpContext* def(const char* name, const CString& string)
  {
	  return OutputString("%s:%s\n", name, string.m_pchData);
  }
  virtual DumpContext* def(const char* name, const char* string) {
	  return OutputString("%s:%s\n", name, string);
  }
  virtual DumpContext* def(const char* name, const int i32) {
	  return OutputString("%s:%d\n", name, i32);
  }
  virtual DumpContext* def(const char* name, const BYTE by) {
	  return OutputString("%s:%d\n", name, (int)by);
  }
  virtual DumpContext* def(const char* name, const WORD w) {
	  return OutputString("%s:%u\n", name, (UINT)w);
  }
  virtual DumpContext* def(const char* name, const UINT u) {
	  return OutputString("%s:0x%X\n", name, (UINT)u);
  }
  virtual DumpContext* def(const char* name, const void* u) {
	  return OutputString("%s:0x%X\n", name, u);
  }
  virtual DumpContext* def(const char* name, const LONG l) {
	  return OutputString("%s:%ld\n", name, l);
  }
  virtual DumpContext* def(const char* name, const int64_t i64) {
	  return OutputString("%s:%lld\n", name, i64);
  }
  virtual DumpContext* beginStruct(const char* name) = 0;
  virtual DumpContext* endStruct() = 0;

  DumpContext* def(const char* name, DWORD dw)
  {
    return OutputString(("%lu"), dw);
  }

  DumpContext* def(const char* name, float f)
  {
	return OutputString(("%f"), f);
  }

  DumpContext* def(const char* name, double d)
  {
	return OutputString(("%lf"), d);
  }

  DumpContext* HexDump(LPCTSTR lpszLine, BYTE* pby,
    int nBytes, int nWidth)
    // do a simple hex-dump (8 per line) to a CDumpContext
    //  the "lpszLine" is a string to print at the start of each line
    //    (%lx should be used to expand the current address)
  {
    ASSERT(nBytes > 0);
    ASSERT(nWidth > 0);
    ASSERT(AfxIsValidString(lpszLine));
    ASSERT(AfxIsValidAddress(pby, nBytes, FALSE));
#if 0
    if (!afxTraceEnabled)
      return this;

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
#endif
  }

  /////////////////////////////////////////////////////////////////////////////

  // special version for WIDE characters
  DumpContext* def(const char* name, LPCWSTR lpsz)
  {
    if (lpsz == NULL)
    {
      return OutputString("%s:NULL\n", name);
    }

    if (!afxTraceEnabled)
      return this;

    // limited length
	char* szBuffer = iconv_w2a(lpsz, -1);
	OutputString(name, szBuffer);
	free(szBuffer);
    return this;
  }



};

#endif // __CDUMPCONTEXT_HPP__
