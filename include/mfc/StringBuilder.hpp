
#ifndef __STRINGBUILDER_HPP__
#define __STRINGBUILDER_HPP__

#include "String.hpp"


struct StringBuilder {
	CString& out;
	StringBuilder(CString& s) : out(s) {}
	StringBuilder& OutputString(const char* szBuffer) {
		out += szBuffer;
		return *this;
	}
	StringBuilder& operator<<(BYTE by)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%d"), (int)by);
		OutputString(szBuffer);
		return *this;
	}

	StringBuilder& operator<<(WORD w)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%u"), (UINT)w);
		OutputString(szBuffer);

		return *this;
	}
	StringBuilder& operator<<(UINT u)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("0x%X"), u);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(LONG l)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%ld"), l);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(DWORD dw)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%lu"), dw);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(int64_t dw)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%lld"), dw);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(uint64_t dw)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%llu"), dw);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(int n)
	{
		char szBuffer[32];

		snprintf(szBuffer, 32, ("%d"), n);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(float f)
	{
		char szBuffer[32];
		snprintf(szBuffer, 32, ("%f"), f);
		OutputString(szBuffer);
		return *this;
	}

	StringBuilder& operator<<(double d)
	{
		char szBuffer[32];
		snprintf(szBuffer, 32, ("%lf"), d);
		OutputString(szBuffer);
		return *this;
	}

	StringBuilder& operator<<(const void* lp)
	{
		char szBuffer[32];

		// prefix a pointer with "$" and print in hex
		snprintf(szBuffer, 32, ("$%lX"), (LONG)lp);
		OutputString(szBuffer);

		return *this;
	}

	StringBuilder& operator<<(const CString& string)
	{
		OutputString(string.m_pchData);
		return *this;
	}

	// do a simple hex-dump (8 per line) to a CDumpContext
	//  the "lpszLine" is a string to print at the start of each line
	//    (%lx should be used to expand the current address)
	StringBuilder& HexDump(const char* lpszLine, BYTE* pby, int nBytes, int nWidth)
	{
		ASSERT(nBytes > 0);
		ASSERT(nWidth > 0);
		ASSERT(AfxIsValidString(lpszLine));
		ASSERT(AfxIsValidAddress(pby, nBytes, FALSE));

		if (!afxTraceEnabled)
			return *this;

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
		return *this;
	}
};



#endif // __STRINGBUILDER_HPP__

