
#ifndef __ARCHIVE_HPP__
#define __ARCHIVE_HPP__

#include "mfc.h"
#include "String.hpp"
#include "File.hpp"

enum CArchiveError {
		CArchiveError_none,
      CArchiveError_generic,
      CArchiveError_readOnly,
      CArchiveError_endOfFile,
      CArchiveError_writeOnly,
      CArchiveError_badIndex,
      CArchiveError_badClass,
      CArchiveError_badSchema
};


class CArchive
{
public:
// Flag values
	enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };
	// advanced operations (used when storing/loading many objects)

// Implementation
public:
	BOOL m_bForceFlat;  // for COleClientItem implementation (default TRUE)
	BOOL m_bDirectBuffer;   // TRUE if m_pFile supports direct buffering
	// public for advanced use
	UINT m_nObjectSchema;
	CString m_strFileName;

protected:

	BOOL m_nMode;
	BOOL m_bUserBuf;
	int m_nBufSize;
	CFile* m_pFile;
	BYTE* m_lpBufCur;
	BYTE* m_lpBufMax;
	BYTE* m_lpBufStart;

	// advanced parameters (controls performance with large archives)
	UINT m_nGrowSize;
	UINT m_nHashSize;

public:
  
// CArchive
_AFX_INLINE BOOL CArchive::IsLoading() const
	{ return (m_nMode & CArchive::load) != 0; }
_AFX_INLINE BOOL CArchive::IsStoring() const
	{ return (m_nMode & CArchive::load) == 0; }
_AFX_INLINE BOOL CArchive::IsByteSwapping() const
	{ return (m_nMode & CArchive::bNoByteSwap) == 0; }
_AFX_INLINE BOOL CArchive::IsBufferEmpty() const
	{ return m_lpBufCur == m_lpBufMax; }
_AFX_INLINE CFile* CArchive::GetFile() const
	{ return m_pFile; }
_AFX_INLINE void CArchive::SetObjectSchema(UINT nSchema)
	{ m_nObjectSchema = nSchema; }
_AFX_INLINE void CArchive::SetStoreParams(UINT nHashSize= 2053, UINT nBlockSize= 128)
{
	ASSERT(IsStoring());
	m_nHashSize = nHashSize;
	m_nGrowSize = nBlockSize;
}
_AFX_INLINE void CArchive::SetLoadParams(UINT nGrowBy= 1024)
{
	ASSERT(IsLoading());
	m_nGrowSize = nGrowBy;
}
_AFX_INLINE CArchive& CArchive::operator<<(int i)
	{ return CArchive::operator<<((LONG)i); }
_AFX_INLINE CArchive& CArchive::operator<<(unsigned u)
	{ return CArchive::operator<<((LONG)u); }
_AFX_INLINE CArchive& CArchive::operator<<(short w)
	{ return CArchive::operator<<((WORD)w); }
_AFX_INLINE CArchive& CArchive::operator<<(char ch)
	{ return CArchive::operator<<((BYTE)ch); }
_AFX_INLINE CArchive& CArchive::operator<<(BYTE by)
	{ if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax) Flush();
		*(UNALIGNED BYTE*)m_lpBufCur = by; m_lpBufCur += sizeof(BYTE); return *this; }
#ifndef _AFX_BYTESWAP
_AFX_INLINE CArchive& CArchive::operator<<(WORD w)
	{ if (m_lpBufCur + sizeof(WORD) > m_lpBufMax) Flush();
		*(UNALIGNED WORD*)m_lpBufCur = w; m_lpBufCur += sizeof(WORD); return *this; }
_AFX_INLINE CArchive& CArchive::operator<<(LONG l)
	{ if (m_lpBufCur + sizeof(LONG) > m_lpBufMax) Flush();
		*(UNALIGNED LONG*)m_lpBufCur = l; m_lpBufCur += sizeof(LONG); return *this; }
_AFX_INLINE CArchive& CArchive::operator<<(DWORD dw)
	{ if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax) Flush();
		*(UNALIGNED DWORD*)m_lpBufCur = dw; m_lpBufCur += sizeof(DWORD); return *this; }
_AFX_INLINE CArchive& CArchive::operator<<(float f)
	{ if (m_lpBufCur + sizeof(float) > m_lpBufMax) Flush();
		*(UNALIGNED _AFX_FLOAT*)m_lpBufCur = *(_AFX_FLOAT*)&f; m_lpBufCur += sizeof(float); return *this;
	}
_AFX_INLINE CArchive& CArchive::operator<<(double d)
	{ if (m_lpBufCur + sizeof(double) > m_lpBufMax) Flush();
		*(UNALIGNED _AFX_DOUBLE*)m_lpBufCur = *(_AFX_DOUBLE*)&d; m_lpBufCur += sizeof(double); return *this; }
#endif
_AFX_INLINE CArchive& CArchive::operator>>(int& i)
	{ return CArchive::operator>>((LONG&)i); }
_AFX_INLINE CArchive& CArchive::operator>>(unsigned& u)
	{ return CArchive::operator>>((LONG&)u); }
_AFX_INLINE CArchive& CArchive::operator>>(short& w)
	{ return CArchive::operator>>((WORD&)w); }
_AFX_INLINE CArchive& CArchive::operator>>(char& ch)
	{ return CArchive::operator>>((BYTE&)ch); }
_AFX_INLINE CArchive& CArchive::operator>>(BYTE& by)
	{ if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax)
			FillBuffer(sizeof(BYTE) - (UINT)(m_lpBufMax - m_lpBufCur));
		by = *(UNALIGNED BYTE*)m_lpBufCur; m_lpBufCur += sizeof(BYTE); return *this; }
#ifndef _AFX_BYTESWAP
_AFX_INLINE CArchive& CArchive::operator>>(WORD& w)
	{ if (m_lpBufCur + sizeof(WORD) > m_lpBufMax)
			FillBuffer(sizeof(WORD) - (UINT)(m_lpBufMax - m_lpBufCur));
		w = *(UNALIGNED WORD*)m_lpBufCur; m_lpBufCur += sizeof(WORD); return *this; }
_AFX_INLINE CArchive& CArchive::operator>>(DWORD& dw)
	{ if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax)
			FillBuffer(sizeof(DWORD) - (UINT)(m_lpBufMax - m_lpBufCur));
		dw = *(UNALIGNED DWORD*)m_lpBufCur; m_lpBufCur += sizeof(DWORD); return *this; }
_AFX_INLINE CArchive& CArchive::operator>>(float& f)
	{ if (m_lpBufCur + sizeof(float) > m_lpBufMax)
			FillBuffer(sizeof(float) - (UINT)(m_lpBufMax - m_lpBufCur));
		*(_AFX_FLOAT*)&f = *(UNALIGNED _AFX_FLOAT*)m_lpBufCur; m_lpBufCur += sizeof(float); return *this; }
_AFX_INLINE CArchive& CArchive::operator>>(double& d)
	{ if (m_lpBufCur + sizeof(double) > m_lpBufMax)
			FillBuffer(sizeof(double) - (UINT)(m_lpBufMax - m_lpBufCur));
		*(_AFX_DOUBLE*)&d = *(UNALIGNED _AFX_DOUBLE*)m_lpBufCur; m_lpBufCur += sizeof(double); return *this; }
_AFX_INLINE CArchive& CArchive::operator>>(LONG& l)
	{ if (m_lpBufCur + sizeof(LONG) > m_lpBufMax)
			FillBuffer(sizeof(LONG) - (UINT)(m_lpBufMax - m_lpBufCur));
		l = *(UNALIGNED LONG*)m_lpBufCur; m_lpBufCur += sizeof(LONG); return *this; }
#endif
_AFX_INLINE CArchive::CArchive(const CArchive& /* arSrc */)
	{ }
_AFX_INLINE void CArchive::operator=(const CArchive& /* arSrc */)
	{ }

#ifdef _AFX_BYTESWAP
CArchive& CArchive::operator<<(WORD w)
{
	if (m_lpBufCur + sizeof(WORD) > m_lpBufMax)
		Flush();

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(w, m_lpBufCur);
	else
		*(WORD*)m_lpBufCur = w;

	m_lpBufCur += sizeof(WORD);
	return *this;
}

CArchive& CArchive::operator<<(LONG l)
{
	ASSERT(sizeof(LONG) == sizeof(DWORD));
	return operator<<((DWORD) l);
}

CArchive& CArchive::operator<<(DWORD dw)
{
	if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax)
		Flush();

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(dw, m_lpBufCur);
	else
		*(DWORD*)m_lpBufCur = dw;

	m_lpBufCur += sizeof(DWORD);
	return *this;
}

CArchive& CArchive::operator<<(float f)
{
	if (m_lpBufCur + sizeof(float) > m_lpBufMax)
		Flush();

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(f, m_lpBufCur);
	else
		*(_AFXFLOAT*)m_lpBufCur = *(_AFXFLOAT*)&f;

	m_lpBufCur += sizeof(float);
	return *this;
}

CArchive& CArchive::operator<<(double d)
{
	if (m_lpBufCur + sizeof(double) > m_lpBufMax)
		Flush();

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(d, m_lpBufCur);
	else
		*(_AFXDOUBLE*)m_lpBufCur = *(_AFXDOUBLE*)&d;

	m_lpBufCur += sizeof(double);
	return *this;
}

CArchive& CArchive::operator>>(WORD& w)
{
	if (m_lpBufCur + sizeof(WORD) > m_lpBufMax)
		FillBuffer(sizeof(WORD) - (UINT)(m_lpBufMax - m_lpBufCur));

	w = *(WORD*)m_lpBufCur;
	m_lpBufCur += sizeof(WORD);

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(w, (BYTE*)&w);

	return *this;
}

CArchive& CArchive::operator>>(LONG& l)
{
	ASSERT(sizeof(LONG) == sizeof(DWORD));
	return operator>>((DWORD&) l);
}

CArchive& CArchive::operator>>(DWORD& dw)
{
	if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax)
		FillBuffer(sizeof(DWORD) - (UINT)(m_lpBufMax - m_lpBufCur));

	dw = *(DWORD*)m_lpBufCur;
	m_lpBufCur += sizeof(DWORD);

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(dw, (BYTE*)&dw);

	return *this;
}

CArchive& CArchive::operator>>(float& f)
{
	if (m_lpBufCur + sizeof(float) > m_lpBufMax)
		FillBuffer(sizeof(float) - (UINT)(m_lpBufMax - m_lpBufCur));

	*(_AFXFLOAT*)&f = *(_AFXFLOAT*)m_lpBufCur;
	m_lpBufCur += sizeof(float);

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(f, (BYTE*)&f);

	return *this;
}

CArchive& CArchive::operator>>(double& d)
{
	if (m_lpBufCur + sizeof(double) > m_lpBufMax)
		FillBuffer(sizeof(double) - (UINT)(m_lpBufMax - m_lpBufCur));

	*(_AFXDOUBLE*)&d = *(_AFXDOUBLE*)m_lpBufCur;
	m_lpBufCur += sizeof(double);

	if (!(m_nMode & bNoByteSwap))
		_AfxByteSwap(d, (BYTE*)&d);

	return *this;
}
#endif //_AFX_BYTESWAP

////////////////////////////////////////////////////////////////////////////
// Archive object input/output

// minimum buffer size
enum { nBufSizeMin = 128 };

// default amount to grow m_pLoadArray upon insert
enum { nGrowSize = 64 };
// default size of hash table in m_pStoreMap when storing
enum { nHashSize = 137 };
// default size to grow collision blocks when storing
enum { nBlockSize = 16 };


////////////////////////////////////////////////////////////////////////////

CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize= 4096, void* lpBuf= NULL) :
	m_strFileName(pFile->GetFilePath())
{
	ASSERT_VALID(pFile);

	// initialize members not dependent on allocated buffer
	m_nMode = nMode;
	m_pFile = pFile;
	m_bForceFlat = TRUE;
	m_nObjectSchema = (UINT)-1; // start with invalid schema
	if (IsStoring())
		m_nGrowSize = nBlockSize;
	else
		m_nGrowSize = nGrowSize;
	m_nHashSize = nHashSize;

	// initialize the buffer.  minimum size is 128
	m_lpBufStart = (BYTE*)lpBuf;
	m_bUserBuf = TRUE;
	m_bDirectBuffer = FALSE;

	if (nBufSize < nBufSizeMin)
	{
		// force use of private buffer of minimum size
		m_nBufSize = nBufSizeMin;
		m_lpBufStart = NULL;
	}
	else
		m_nBufSize = nBufSize;

	nBufSize = m_nBufSize;
	if (m_lpBufStart == NULL)
	{
		// check for CFile providing buffering support
		m_bDirectBuffer = m_pFile->GetBufferPtr(CFile::bufferCheck);
		if (!m_bDirectBuffer)
		{
			// no support for direct buffering, allocate new buffer
			m_lpBufStart = new BYTE[m_nBufSize];
			m_bUserBuf = FALSE;
		}
		else
		{
			// CFile* supports direct buffering!
			nBufSize = 0;   // will trigger initial FillBuffer
		}
	}

	if (!m_bDirectBuffer)
	{
		ASSERT(m_lpBufStart != NULL);
		ASSERT(AfxIsValidAddress(m_lpBufStart, nBufSize, IsStoring()));
	}
	m_lpBufMax = m_lpBufStart + nBufSize;
	m_lpBufCur = (IsLoading()) ? m_lpBufMax : m_lpBufStart;

}

CArchive::~CArchive()
{
	// Close makes m_pFile NULL. If it is not NULL, we must Close the CArchive
	if (m_pFile != NULL && !(m_nMode & bNoFlushOnDelete))
		Close();

	Abort();    // abort completely shuts down the archive
}

void CArchive::Abort()
{
	ASSERT(m_bDirectBuffer || m_lpBufStart == NULL ||
		AfxIsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, IsStoring()));
	ASSERT(m_bDirectBuffer || m_lpBufCur == NULL ||
		AfxIsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, IsStoring()));

	// disconnect from the file
	m_pFile = NULL;

	if (!m_bUserBuf)
	{
		ASSERT(!m_bDirectBuffer);
		delete[] m_lpBufStart;
		m_lpBufStart = NULL;
		m_lpBufCur = NULL;
	}
}

void CArchive::Close()
{
	ASSERT_VALID(m_pFile);

	Flush();
	m_pFile = NULL;
}

UINT CArchive::Read(void* lpBuf, UINT nMax)
{
	ASSERT_VALID(m_pFile);

	if (nMax == 0)
		return 0;

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nMax));
	ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	ASSERT(m_lpBufStart == NULL ||
		AfxIsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, FALSE));
	ASSERT(m_lpBufCur == NULL ||
		AfxIsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, FALSE));
	ASSERT(IsLoading());

	// try to fill from buffer first
	UINT nMaxTemp = nMax;
	UINT nTemp = MIN(nMaxTemp, (UINT)(m_lpBufMax - m_lpBufCur));
	memcpy(lpBuf, m_lpBufCur, nTemp);
	m_lpBufCur += nTemp;
	lpBuf = (BYTE*)lpBuf + nTemp;
	nMaxTemp -= nTemp;

	if (nMaxTemp != 0)
	{
		ASSERT(m_lpBufCur == m_lpBufMax);

		// read rest in buffer size chunks
		nTemp = nMaxTemp - (nMaxTemp % m_nBufSize);
		UINT nRead = 0;

		UINT nLeft = nTemp;
		UINT nBytes;
		do
		{
			nBytes = m_pFile->Read(lpBuf, nLeft);
			lpBuf = (BYTE*)lpBuf + nBytes;
			nRead += nBytes;
			nLeft -= nBytes;
		}
		while ((nBytes > 0) && (nLeft > 0));

		nMaxTemp -= nRead;

		// read last chunk into buffer then copy
		if (nRead == nTemp)
		{
			ASSERT(m_lpBufCur == m_lpBufMax);
			ASSERT(nMaxTemp < (UINT)m_nBufSize);

			// fill buffer (similar to CArchive::FillBuffer, but no exception)
			if (!m_bDirectBuffer)
			{
				UINT nLeft = MAX(nMaxTemp, (UINT)m_nBufSize);
				UINT nBytes;
				BYTE* lpTemp = m_lpBufStart;
				nRead = 0;
				do
				{
					nBytes = m_pFile->Read(lpTemp, nLeft);
					lpTemp = lpTemp + nBytes;
					nRead += nBytes;
					nLeft -= nBytes;
				}
				while ((nBytes > 0) && (nLeft > 0) && nRead < nMaxTemp);

				m_lpBufCur = m_lpBufStart;
				m_lpBufMax = m_lpBufStart + nRead;
			}
			else
			{
				nRead = m_pFile->GetBufferPtr(CFile::bufferRead, m_nBufSize,
					(void**)&m_lpBufStart, (void**)&m_lpBufMax);
				ASSERT(nRead == (UINT)(m_lpBufMax - m_lpBufStart));
				m_lpBufCur = m_lpBufStart;
			}

			// use first part for rest of read
			nTemp = MIN(nMaxTemp, (UINT)(m_lpBufMax - m_lpBufCur));
			memcpy(lpBuf, m_lpBufCur, nTemp);
			m_lpBufCur += nTemp;
			nMaxTemp -= nTemp;
		}
	}
	return nMax - nMaxTemp;
}

void CArchive::Write(const void* lpBuf, UINT nMax)
{
	ASSERT_VALID(m_pFile);

	if (nMax == 0)
		return;

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nMax, FALSE));  // read-only access needed
	ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	ASSERT(m_lpBufStart == NULL ||
		AfxIsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart));
	ASSERT(m_lpBufCur == NULL ||
		AfxIsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur));
	ASSERT(IsStoring());

	// copy to buffer if possible
	UINT nTemp = MIN(nMax, (UINT)(m_lpBufMax - m_lpBufCur));
	memcpy(m_lpBufCur, lpBuf, nTemp);
	m_lpBufCur += nTemp;
	lpBuf = (BYTE*)lpBuf + nTemp;
	nMax -= nTemp;

	if (nMax > 0)
	{
		Flush();    // flush the full buffer

		// write rest of buffer size chunks
		nTemp = nMax - (nMax % m_nBufSize);
		m_pFile->Write(lpBuf, nTemp);
		lpBuf = (BYTE*)lpBuf + nTemp;
		nMax -= nTemp;

		if (m_bDirectBuffer)
		{
			// sync up direct mode buffer to new file position
			VERIFY(m_pFile->GetBufferPtr(CFile::bufferWrite, m_nBufSize,
				(void**)&m_lpBufStart, (void**)&m_lpBufMax) == (UINT)m_nBufSize);
			ASSERT((UINT)m_nBufSize == (UINT)(m_lpBufMax - m_lpBufStart));
			m_lpBufCur = m_lpBufStart;
		}

		// copy remaining to active buffer
		ASSERT(nMax < (UINT)m_nBufSize);
		ASSERT(m_lpBufCur == m_lpBufStart);
		memcpy(m_lpBufCur, lpBuf, nMax);
		m_lpBufCur += nMax;
	}
}

void CArchive::Flush()
{
	ASSERT_VALID(m_pFile);
	ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	ASSERT(m_lpBufStart == NULL ||
		AfxIsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, IsStoring()));
	ASSERT(m_lpBufCur == NULL ||
		AfxIsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, IsStoring()));

	if (IsLoading())
	{
		// unget the characters in the buffer, seek back unused amount
		if (m_lpBufMax != m_lpBufCur)
			m_pFile->Seek(-(m_lpBufMax - m_lpBufCur), CFile::current);
		m_lpBufCur = m_lpBufMax;    // empty
	}
	else
	{
		if (!m_bDirectBuffer)
		{
			// write out the current buffer to file
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->Write(m_lpBufStart, m_lpBufCur - m_lpBufStart);
		}
		else
		{
			// commit current buffer
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->GetBufferPtr(CFile::bufferCommit, m_lpBufCur - m_lpBufStart);
			// get next buffer
			VERIFY(m_pFile->GetBufferPtr(CFile::bufferWrite, m_nBufSize,
				(void**)&m_lpBufStart, (void**)&m_lpBufMax) == (UINT)m_nBufSize);
			ASSERT((UINT)m_nBufSize == (UINT)(m_lpBufMax - m_lpBufStart));
		}
		m_lpBufCur = m_lpBufStart;
	}
}

CArchiveError CArchive::FillBuffer(UINT nBytesNeeded)
{
	ASSERT_VALID(m_pFile);
	ASSERT(IsLoading());
	ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	ASSERT(nBytesNeeded > 0);
	ASSERT(nBytesNeeded <= (UINT)m_nBufSize);
	ASSERT(m_lpBufStart == NULL ||
		AfxIsValidAddress(m_lpBufStart, m_lpBufMax - m_lpBufStart, FALSE));
	ASSERT(m_lpBufCur == NULL ||
		AfxIsValidAddress(m_lpBufCur, m_lpBufMax - m_lpBufCur, FALSE));

	UINT nUnused = m_lpBufMax - m_lpBufCur;
	ULONG nTotalNeeded = ((ULONG)nBytesNeeded) + nUnused;

	// fill up the current buffer from file
	if (!m_bDirectBuffer)
	{
		ASSERT(m_lpBufCur != NULL);
		ASSERT(m_lpBufStart != NULL);
		ASSERT(m_lpBufMax != NULL);

		if (m_lpBufCur > m_lpBufStart)
		{
			// copy unused
			if ((int)nUnused > 0)
			{
				memmove(m_lpBufStart, m_lpBufCur, nUnused);
				m_lpBufCur = m_lpBufStart;
				m_lpBufMax = m_lpBufStart + nUnused;
			}

			// read to satisfy nBytesNeeded or nLeft if possible
			UINT nRead = nUnused;
			UINT nLeft = m_nBufSize-nUnused;
			UINT nBytes;
			BYTE* lpTemp = m_lpBufStart + nUnused;
			do
			{
				nBytes = m_pFile->Read(lpTemp, nLeft);
				lpTemp = lpTemp + nBytes;
				nRead += nBytes;
				nLeft -= nBytes;
			}
			while (nBytes > 0 && nLeft > 0 && nRead < nBytesNeeded);

			m_lpBufCur = m_lpBufStart;
			m_lpBufMax = m_lpBufStart + nRead;
		}
	}
	else
	{
		// seek to unused portion and get the buffer starting there
		if (nUnused != 0)
			m_pFile->Seek(-(LONG)nUnused, CFile::current);
		UINT nActual = m_pFile->GetBufferPtr(CFile::bufferRead, m_nBufSize,
			(void**)&m_lpBufStart, (void**)&m_lpBufMax);
		ASSERT(nActual == (UINT)(m_lpBufMax - m_lpBufStart));
		m_lpBufCur = m_lpBufStart;
	}

	// not enough data to fill request?
	if ((ULONG)(m_lpBufMax - m_lpBufCur) < nTotalNeeded)
		return (CArchiveError_endOfFile);
  return CArchiveError_none;
}

void CArchive::WriteCount(DWORD dwCount)
{
	if (dwCount < 0xFFFF)
		*this << (WORD)dwCount;
	else
	{
		*this << (WORD)0xFFFF;
		*this << dwCount;
	}
}

DWORD CArchive::ReadCount()
{
	WORD wCount;
	*this >> wCount;
	if (wCount != 0xFFFF)
		return wCount;

	DWORD dwCount;
	*this >> dwCount;
	return dwCount;
}

// special functions for text file input and output

void CArchive::WriteString(LPCTSTR lpsz)
{
	ASSERT(AfxIsValidString(lpsz));
	Write(lpsz, lstrlen(lpsz) * sizeof(TCHAR));
}

LPTSTR CArchive::ReadString(LPTSTR lpsz, UINT nMax)
{
	// if nMax is negative (such a large number doesn't make sense given today's
	// 2gb address space), then assume it to mean "keep the newline".
	int nStop = (int)nMax < 0 ? -(int)nMax : (int)nMax;
	ASSERT(AfxIsValidAddress(lpsz, (nStop+1) * sizeof(TCHAR)));

	_TUCHAR ch;
	int nRead = 0;

		while (nRead < nStop)
		{
			*this >> ch;

			// stop and end-of-line (trailing '\n' is ignored)
			if (ch == '\n' || ch == '\r')
			{
				if (ch == '\r')
					*this >> ch;
				// store the newline when called with negative nMax
				if ((int)nMax != nStop)
					lpsz[nRead++] = ch;
				break;
			}
			lpsz[nRead++] = ch;
		}

	lpsz[nRead] = '\0';
	return lpsz;
}

BOOL CArchive::ReadString(CString& rString)
{
	rString = &afxChNil;    // empty string without deallocating
	const int nMaxSize = 128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen;
	for (;;)
	{
		lpszResult = ReadString(lpsz, (UINT)-nMaxSize); // store the newline
		rString.ReleaseBuffer();

		// if string is read completely or EOF
		if (lpszResult == NULL ||
			(nLen = lstrlen(lpsz)) < nMaxSize ||
			lpsz[nLen-1] == '\n')
		{
			break;
		}

		nLen = rString.GetLength();
		lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
	}

	// remove '\n' from end of string if present
	lpsz = rString.GetBuffer(0);
	nLen = rString.GetLength();
	if (nLen != 0 && lpsz[nLen-1] == '\n')
		rString.GetBufferSetLength(nLen-1);

	return lpszResult != NULL;
}


};


// CString serialization code
// String format:
//      UNICODE strings are always prefixed by 0xff, 0xfffe
//      if < 0xff chars: len:BYTE, TCHAR chars
//      if >= 0xff characters: 0xff, len:WORD, TCHAR chars
//      if >= 0xfffe characters: 0xff, 0xffff, len:DWORD, TCHARs

CArchive& operator<<(CArchive& ar, const CString& string)
{
	// special signature to recognize unicode strings
#ifdef _UNICODE
	ar << (BYTE)0xff;
	ar << (WORD)0xfffe;
#endif

	if (string.GetData()->nDataLength < 255)
	{
		ar << (BYTE)string.GetData()->nDataLength;
	}
	else if (string.GetData()->nDataLength < 0xfffe)
	{
		ar << (BYTE)0xff;
		ar << (WORD)string.GetData()->nDataLength;
	}
	else
	{
		ar << (BYTE)0xff;
		ar << (WORD)0xffff;
		ar << (DWORD)string.GetData()->nDataLength;
	}
	ar.Write(string.m_pchData, string.GetData()->nDataLength*sizeof(TCHAR));
	return ar;
}

#define AFX_STATIC static
// return string length or -1 if UNICODE string is found in the archive
AFX_STATIC UINT _AfxReadStringLength(CArchive& ar)
{
	DWORD nNewLen;

	// attempt BYTE length first
	BYTE bLen;
	ar >> bLen;

	if (bLen < 0xff)
		return bLen;

	// attempt WORD length
	WORD wLen;
	ar >> wLen;
	if (wLen == 0xfffe)
	{
		// UNICODE string prefix (length will follow)
		return (UINT)-1;
	}
	else if (wLen == 0xffff)
	{
		// read DWORD of length
		ar >> nNewLen;
		return (UINT)nNewLen;
	}
	else
		return wLen;
}

CArchive& operator>>(CArchive& ar, CString& string)
{
#ifdef _UNICODE
	int nConvert = 1;   // if we get ANSI, convert
#else
	int nConvert = 0;   // if we get UNICODE, convert
#endif

	UINT nNewLen = _AfxReadStringLength(ar);
	if (nNewLen == (UINT)-1)
	{
		nConvert = 1 - nConvert;
		nNewLen = _AfxReadStringLength(ar);
		ASSERT(nNewLen != -1);
	}

	// set length of string to new length
	UINT nByteLen = nNewLen;
#ifdef _UNICODE
	string.GetBufferSetLength((int)nNewLen);
	nByteLen += nByteLen * (1 - nConvert);  // bytes to read
#else
	nByteLen += nByteLen * nConvert;    // bytes to read
	if (nNewLen == 0)
		string.GetBufferSetLength(0);
	else
		string.GetBufferSetLength((int)nByteLen+nConvert);
#endif

	// read in the characters
	if (nNewLen != 0)
	{
		ASSERT(nByteLen != 0);

		// read new data
    if (ar.Read(string.m_pchData, nByteLen) != nByteLen) {
			//AfxThrowArchiveException(CArchiveError_endOfFile);
    }

		// convert the data if as necessary
		if (nConvert != 0)
		{
			CStringData* pOldData = string.GetData();
			LPSTR lpsz = string.m_pchData;
			lpsz[nNewLen] = '\0';    // must be NUL terminated
			string.Init();   // don't delete the old data
			string = lpsz;   // convert with operator=(LPWCSTR)
			CString::FreeData(pOldData);
		}
	}
	return ar;
}

// specialized version of SerializeElements for CString (used in collections)
void SerializeElements(CArchive& ar, CString* pElements, int nCount)
{
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(CString)));

	if (ar.IsStoring())
	{
		for (; nCount--; ++pElements)
			ar << *pElements;
	}
	else
	{
		for (; nCount--; ++pElements)
			ar >> *pElements;
	}
}



CArchive& operator <<(CArchive& ar, CTime time)
{
  return ar << (DWORD) time.m_time;
}

CArchive& operator >>(CArchive& ar, CTime& rtime)
{
  return ar >> (DWORD&) rtime.m_time;
}

CArchive& operator <<(CArchive& ar, CTimeSpan timeSpan)
{
  return ar << (DWORD) timeSpan.m_timeSpan;
}

CArchive& operator >>(CArchive& ar, CTimeSpan& rtimeSpan)
{
  return ar >> (DWORD&) rtimeSpan.m_timeSpan;
}



#endif // __ARCHIVE_HPP__