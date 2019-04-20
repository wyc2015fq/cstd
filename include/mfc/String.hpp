
#ifndef __CSTRING_H__
#define __CSTRING_H__


#include "typedef.h"
#include "std/stddef_c.h"
#include "std/log_c.h"
#include "std/iconv_c.h"
#include "std/atomic_c.h"


/////////////////////////////////////////////////////////////////////////////
// static class data, special inlines
// afxChNil is left for backward compatibility
static TCHAR afxChNil = '\0';

struct CStringData
{
  volatile atomic_t nRefs;             // reference count
  int nDataLength;        // length of data (including terminator)
  int nAllocLength;       // length of allocation
  // TCHAR data[nAllocLength]

  TCHAR* data()           // TCHAR* to managed data
  { return (TCHAR*)(this+1); }
};

int SafeStrlen(const char* lpsz)
{ return (lpsz == NULL) ? 0 : strlen(lpsz); }

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
static int _afxInitData[] = { -1, 0, 0, 0 };
static CStringData* _afxDataNil = (CStringData*)&_afxInitData;
const char* _afxPchNil = (const char*)(((BYTE*)&_afxInitData)+sizeof(CStringData));
// special function to make afxEmptyString work even during initialization


//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

#define afxEmptyString ((CString&)*(CString*)&_afxPchNil)


struct CString {
  static const CString& AfxGetEmptyString() { return *(CString*)&_afxPchNil; }
  char* m_pchData;   // pointer to ref counted string data

  // CString
  CStringData* GetData() const
  { ASSERT(m_pchData != NULL); return ((CStringData*)m_pchData)-1; }
  void Init()
  { m_pchData = afxEmptyString.m_pchData; }
  CString()
  { m_pchData = afxEmptyString.m_pchData; }

  CString(const unsigned char* lpsz)
  { Init(); AssignCopy((const char*)lpsz, -1); }
  const CString& operator=(const unsigned char* lpsz)
  { *this = (LPCSTR)lpsz; return *this; }
  const CString& operator+=(char ch)
  { *this += (TCHAR)ch; return *this; }
  const CString& operator=(char ch)
  { *this = (TCHAR)ch; return *this; }

  int GetLength() const
  { return GetData()->nDataLength; }
  int GetAllocLength() const
  { return GetData()->nAllocLength; }
  BOOL IsEmpty() const
  { return GetData()->nDataLength == 0; }
  char* c_str() const { return m_pchData; }

  // CString support (windows specific)
  int Compare(const char* lpsz) const
  {  return strcmp(m_pchData, lpsz); }    // MBCS/Unicode aware
  int CompareNoCase(const char* lpsz) const
  {  return stricmp(m_pchData, lpsz); }   // MBCS/Unicode aware
  // Collate is often slower than Compare but is MBSC/Unicode
  //  aware as well as locale-sensitive with respect to sort order.
  int Collate(const char* lpsz) const
  {  return strcoll(m_pchData, lpsz); }   // locale sensitive
  #if 0
  int CollateNoCase(const char* lpsz) const
  {  return _stricoll(m_pchData, lpsz); }   // locale sensitive
#endif

  TCHAR GetAt(int nIndex) const
  {
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
  }
  TCHAR operator[](int nIndex) const
  {
    // same as GetAt
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
  }

  CString(const CString& stringSrc)
  {
    ASSERT(stringSrc.GetData()->nRefs != 0);
    if (stringSrc.GetData()->nRefs >= 0)
    {
      ASSERT(stringSrc.GetData() != _afxDataNil);
      m_pchData = stringSrc.m_pchData;
      atomic_inc(&GetData()->nRefs);
    }
    else
    {
      Init();
      *this = stringSrc.m_pchData;
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // More sophisticated construction

  void AssignCopy(const char* lpsz, int nSrcLen, int nCount = 1, const char* charset = NULL)
  {
    nSrcLen = nSrcLen<0 ? strlen(lpsz) : nSrcLen;
    if (charset==NULL || 0==stricmp(charset, CSTR_CHARSET)) {
      AllocBeforeWrite(nSrcLen*nCount);
      memcpy(m_pchData, lpsz, nSrcLen*sizeof(TCHAR));
      GetData()->nDataLength = nSrcLen;
    } else {
      AllocBeforeWrite(nSrcLen*2*nCount);
      nSrcLen = iconv_cs(CSTR_CHARSET, charset, lpsz, (nSrcLen*2)+1, m_pchData, nSrcLen*2);
      ReleaseBuffer();
    }
    if (nCount>1) {
      memunroll(m_pchData, nSrcLen, nSrcLen*nCount);
    }
    m_pchData[nSrcLen*nCount] = '\0';
  }

  /////////////////////////////////////////////////////////////////////////////
  // Special conversion constructors
  
  CString(const char* lpsz, const char* charset = NULL)
  {
    Init();
    AssignCopy(lpsz, strlen(lpsz), 1, charset);
  }

  //////////////////////////////////////////////////////////////////////////////
  // More sophisticated construction

  CString(TCHAR ch, int nLength)
  {
    Init();
    if(nLength>0) {
      AssignCopy(&ch, 1, nLength, NULL);
    }
  }

  CString(const char* lpch, int nLength, const char* charset = NULL)
  {
    Init();
    if (nLength != 0)
    {
      AssignCopy(lpch, nLength, 1, charset);
    }
  }

  // always allocate one extra character for '\0' termination
  // assumes [optimistically] that data length will equal allocation length
  void AllocBuffer(int nLen)
  {
    ASSERT(nLen >= 0);
    ASSERT(nLen <= INT_MAX-1);    // MAX size (enough room for 1 extra)

    if (nLen == 0)
      Init();
    else
    {
      CStringData* pData;
      {
        pData = (CStringData*)new BYTE[sizeof(CStringData) + (nLen+1)*sizeof(TCHAR)];
        pData->nAllocLength = nLen;
      }
      pData->nRefs = 1;
      pData->data()[nLen] = '\0';
      pData->nDataLength = nLen;
      m_pchData = pData->data();
    }
  }

  static void FreeData(CStringData* pData)
  {
    delete[] (BYTE*)pData;
  }

  void Release()
  {
    if (GetData() != _afxDataNil)
    {
      ASSERT(GetData()->nRefs != 0);
      if (atomic_dec(&GetData()->nRefs) <= 0)
        FreeData(GetData());
      Init();
    }
  }

  void Release(CStringData* pData)
  {
    if (pData != _afxDataNil)
    {
      ASSERT(pData->nRefs != 0);
      if (atomic_dec(&pData->nRefs) <= 0)
        FreeData(pData);
    }
  }

  void Empty()
  {
    if (GetData()->nDataLength == 0)
      return;
    if (GetData()->nRefs >= 0)
      Release();
    else
      *this = &afxChNil;
    ASSERT(GetData()->nDataLength == 0);
    ASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
  }

  void CopyBeforeWrite()
  {
    if (GetData()->nRefs > 1)
    {
      CStringData* pData = GetData();
      Release();
      AllocBuffer(pData->nDataLength);
      memcpy(m_pchData, pData->data(), (pData->nDataLength+1)*sizeof(TCHAR));
    }
    ASSERT(GetData()->nRefs <= 1);
  }

  void AllocBeforeWrite(int nLen)
  {
    if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
    {
      Release();
      AllocBuffer(nLen);
    }
    ASSERT(GetData()->nRefs <= 1);
  }

  ~CString()
    //  free any attached data
  {
    if (GetData() != _afxDataNil)
    {
      if (atomic_dec(&GetData()->nRefs) <= 0)
        FreeData(GetData());
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Helpers for the rest of the implementation

  void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex,
    int nExtraLen) const
  {
    // will clone the data attached to this string
    // allocating 'nExtraLen' characters
    // Places results in uninitialized string 'dest'
    // Will copy the part or all of original data to start of new string

    int nNewLen = nCopyLen + nExtraLen;
    if (nNewLen == 0)
    {
      dest.Init();
    }
    else
    {
      dest.AllocBuffer(nNewLen);
      memcpy(dest.m_pchData, m_pchData+nCopyIndex, nCopyLen*sizeof(TCHAR));
    }
  }

  const CString& operator=(const CString& stringSrc)
  {
    if (m_pchData != stringSrc.m_pchData)
    {
      if ((GetData()->nRefs < 0 && GetData() != _afxDataNil) ||
        stringSrc.GetData()->nRefs < 0)
      {
        // actual copy necessary since one of the strings is locked
        AssignCopy(stringSrc.m_pchData, stringSrc.GetData()->nDataLength);
      }
      else
      {
        // can just copy references around
        Release();
        ASSERT(stringSrc.GetData() != _afxDataNil);
        m_pchData = stringSrc.m_pchData;
        atomic_inc(&GetData()->nRefs);
      }
    }
    return *this;
  }

  const CString& operator=(const char* lpsz)
  {
    ASSERT(lpsz == NULL);
    AssignCopy(lpsz, SafeStrlen(lpsz));
    return *this;
  }

  //////////////////////////////////////////////////////////////////////////////
  // concatenation

  // NOTE: "operator+" is done as friend functions for simplicity
  //      There are three variants:
  //          CString + CString
  // and for ? = TCHAR, const char*
  //          CString + ?
  //          ? + CString

  void ConcatCopy(int nSrc1Len, const char* lpszSrc1Data,
    int nSrc2Len, const char* lpszSrc2Data)
  {
    int nNewLen = nSrc1Len + nSrc2Len;
    if (nNewLen != 0)
    {
      AllocBuffer(nNewLen);
      memcpy(m_pchData, lpszSrc1Data, nSrc1Len*sizeof(TCHAR));
      memcpy(m_pchData+nSrc1Len, lpszSrc2Data, nSrc2Len*sizeof(TCHAR));
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // concatenate in place

  void ConcatInPlace(int nSrcLen, const char* lpszSrcData)
  {
    //  -- the main routine for += operators

    // concatenating an empty string is a no-op!
    if (nSrcLen == 0)
      return;

    // if the buffer is too small, or we have a width mis-match, just
    //   allocate a new buffer (slow but sure)
    if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
    {
      // we have to grow the buffer, use the ConcatCopy routine
      CStringData* pOldData = GetData();
      ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
      ASSERT(pOldData != NULL);
      Release(pOldData);
    }
    else
    {
      // fast concatenation when buffer big enough
      memcpy(m_pchData+GetData()->nDataLength, lpszSrcData, nSrcLen*sizeof(TCHAR));
      GetData()->nDataLength += nSrcLen;
      ASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
      m_pchData[GetData()->nDataLength] = '\0';
    }
  }

  const CString& operator+=(const char* lpsz)
  {
    ASSERT(lpsz == NULL);
    ConcatInPlace(SafeStrlen(lpsz), lpsz);
    return *this;
  }


  const CString& operator+=(const CString& string)
  {
    ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
    return *this;
  }

  ///////////////////////////////////////////////////////////////////////////////
  // Advanced direct buffer access

  char* GetBuffer(int nMinBufLength)
  {
    ASSERT(nMinBufLength >= 0);

    if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
    {
#ifdef _DEBUG
      // give a warning in case locked string becomes unlocked
      if (GetData() != _afxDataNil && GetData()->nRefs < 0)
        TRACE0("Warning: GetBuffer on locked CString creates unlocked CString!\n");
#endif
      // we have to grow the buffer
      CStringData* pOldData = GetData();
      int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
      if (nMinBufLength < nOldLen)
        nMinBufLength = nOldLen;
      AllocBuffer(nMinBufLength);
      memcpy(m_pchData, pOldData->data(), (nOldLen+1)*sizeof(TCHAR));
      GetData()->nDataLength = nOldLen;
      Release(pOldData);
    }
    ASSERT(GetData()->nRefs <= 1);

    // return a pointer to the character storage for this string
    ASSERT(m_pchData != NULL);
    return m_pchData;
  }

  void ReleaseBuffer(int nNewLength = -1)
  {
    CopyBeforeWrite();  // just in case GetBuffer was not called

    if (nNewLength == -1)
      nNewLength = strlen(m_pchData); // zero terminated

    ASSERT(nNewLength <= GetData()->nAllocLength);
    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = '\0';
  }

  char* GetBufferSetLength(int nNewLength)
  {
    ASSERT(nNewLength >= 0);

    GetBuffer(nNewLength);
    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = '\0';
    return m_pchData;
  }

  void FreeExtra()
  {
    ASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
    if (GetData()->nDataLength != GetData()->nAllocLength)
    {
      CStringData* pOldData = GetData();
      AllocBuffer(GetData()->nDataLength);
      memcpy(m_pchData, pOldData->data(), pOldData->nDataLength*sizeof(TCHAR));
      ASSERT(m_pchData[GetData()->nDataLength] == '\0');
      Release(pOldData);
    }
    ASSERT(GetData() != NULL);
  }

  char* LockBuffer()
  {
    char* lpsz = GetBuffer(0);
    GetData()->nRefs = -1;
    return lpsz;
  }

  void UnlockBuffer()
  {
    ASSERT(GetData()->nRefs == -1);
    if (GetData() != _afxDataNil)
      GetData()->nRefs = 1;
  }

  ///////////////////////////////////////////////////////////////////////////////
  // Commonly used routines (rarely used routines in STREX.CPP)
  int Find(TCHAR ch) const
  {
    return Find(ch, 0);
  }

  int Find(TCHAR ch, int nStart) const
  {
    int nLength = GetData()->nDataLength;
    if (nStart >= nLength)
      return -1;

    // find first single character
    char* lpsz = strchr(m_pchData + nStart, ch);

    // return -1 if not found and index otherwise
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
  }

  int FindOneOf(const char* lpszCharSet) const
  {
    char* lpsz = strpbrk(m_pchData, lpszCharSet);
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
  }

  void MakeUpper()
  {
    CopyBeforeWrite();
    strupr(m_pchData);
  }

  void MakeLower()
  {
    CopyBeforeWrite();
    strlwr(m_pchData);
  }

  void MakeReverse()
  {
    CopyBeforeWrite();
    strrev(m_pchData);
  }

  void SetAt(int nIndex, TCHAR ch)
  {
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);

    CopyBeforeWrite();
    m_pchData[nIndex] = ch;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Advanced manipulation

  int Delete(int nIndex, int nCount /* = 1 */)
  {
    if (nIndex < 0)
      nIndex = 0;
    int nNewLength = GetData()->nDataLength;
    if (nCount > 0 && nIndex < nNewLength)
    {
      CopyBeforeWrite();
      int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

      memcpy(m_pchData + nIndex,
        m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
      GetData()->nDataLength = nNewLength - nCount;
    }

    return nNewLength;
  }

  int Insert(int nIndex, TCHAR ch)
  {
    CopyBeforeWrite();

    if (nIndex < 0)
      nIndex = 0;

    int nNewLength = GetData()->nDataLength;
    if (nIndex > nNewLength)
      nIndex = nNewLength;
    nNewLength++;

    if (GetData()->nAllocLength < nNewLength)
    {
      CStringData* pOldData = GetData();
      char* pstr = m_pchData;
      AllocBuffer(nNewLength);
      memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
      Release(pOldData);
    }

    // move existing bytes down
    memcpy(m_pchData + nIndex + 1,
      m_pchData + nIndex, (nNewLength-nIndex)*sizeof(TCHAR));
    m_pchData[nIndex] = ch;
    GetData()->nDataLength = nNewLength;

    return nNewLength;
  }

  int Insert(int nIndex, const char* pstr)
  {
    if (nIndex < 0)
      nIndex = 0;

    int nInsertLength = SafeStrlen(pstr);
    int nNewLength = GetData()->nDataLength;
    if (nInsertLength > 0)
    {
      CopyBeforeWrite();
      if (nIndex > nNewLength)
        nIndex = nNewLength;
      nNewLength += nInsertLength;

      if (GetData()->nAllocLength < nNewLength)
      {
        CStringData* pOldData = GetData();
        char* pstr = m_pchData;
        AllocBuffer(nNewLength);
        memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
        Release(pOldData);
      }

      // move existing bytes down
      memcpy(m_pchData + nIndex + nInsertLength,
        m_pchData + nIndex,
        (nNewLength-nIndex-nInsertLength+1)*sizeof(TCHAR));
      memcpy(m_pchData + nIndex,
        pstr, nInsertLength*sizeof(TCHAR));
      GetData()->nDataLength = nNewLength;
    }

    return nNewLength;
  }

  int Replace(TCHAR chOld, TCHAR chNew)
  {
    int nCount = 0;

    // short-circuit the nop case
    if (chOld != chNew)
    {
      // otherwise modify each character that matches in the string
      CopyBeforeWrite();
      char* psz = m_pchData;
      char* pszEnd = psz + GetData()->nDataLength;
      while (psz < pszEnd)
      {
        // replace instances of the specified character only
        if (*psz == chOld)
        {
          *psz = chNew;
          nCount++;
        }
        psz = _strinc(psz);
      }
    }
    return nCount;
  }

  int Replace(const char* lpszOld, const char* lpszNew)
  {
    // can't have empty or NULL lpszOld

    int nSourceLen = SafeStrlen(lpszOld);
    if (nSourceLen == 0)
      return 0;
    int nReplacementLen = SafeStrlen(lpszNew);

    // loop once to figure out the size of the result string
    int nCount = 0;
    char* lpszStart = m_pchData;
    char* lpszEnd = m_pchData + GetData()->nDataLength;
    char* lpszTarget;
    while (lpszStart < lpszEnd)
    {
      while ((lpszTarget = strstr(lpszStart, lpszOld)) != NULL)
      {
        nCount++;
        lpszStart = lpszTarget + nSourceLen;
      }
      lpszStart += strlen(lpszStart) + 1;
    }

    // if any changes were made, make them
    if (nCount > 0)
    {
      CopyBeforeWrite();

      // if the buffer is too small, just
      //   allocate a new buffer (slow but sure)
      int nOldLength = GetData()->nDataLength;
      int nNewLength =  nOldLength + (nReplacementLen-nSourceLen)*nCount;
      if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
      {
        CStringData* pOldData = GetData();
        char* pstr = m_pchData;
        AllocBuffer(nNewLength);
        memcpy(m_pchData, pstr, pOldData->nDataLength*sizeof(TCHAR));
        Release(pOldData);
      }
      // else, we just do it in-place
      lpszStart = m_pchData;
      lpszEnd = m_pchData + GetData()->nDataLength;

      // loop again to actually do the work
      while (lpszStart < lpszEnd)
      {
        while ( (lpszTarget = strstr(lpszStart, lpszOld)) != NULL)
        {
          int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen);
          memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
            nBalance * sizeof(TCHAR));
          memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
          lpszStart = lpszTarget + nReplacementLen;
          lpszStart[nBalance] = '\0';
          nOldLength += (nReplacementLen - nSourceLen);
        }
        lpszStart += strlen(lpszStart) + 1;
      }
      ASSERT(m_pchData[nNewLength] == '\0');
      GetData()->nDataLength = nNewLength;
    }

    return nCount;
  }

  int Remove(TCHAR chRemove)
  {
    CopyBeforeWrite();

    char* pstrSource = m_pchData;
    char* pstrDest = m_pchData;
    char* pstrEnd = m_pchData + GetData()->nDataLength;

    while (pstrSource < pstrEnd)
    {
      if (*pstrSource != chRemove)
      {
        *pstrDest = *pstrSource;
        pstrDest = _strinc(pstrDest);
      }
      pstrSource = _strinc(pstrSource);
    }
    *pstrDest = '\0';
    int nCount = pstrSource - pstrDest;
    GetData()->nDataLength -= nCount;

    return nCount;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Very simple sub-string extraction

  CString Mid(int nFirst) const
  {
    return Mid(nFirst, GetData()->nDataLength - nFirst);
  }

  CString Mid(int nFirst, int nCount) const
  {
    // out-of-bounds requests return sensible things
    if (nFirst < 0)
      nFirst = 0;
    if (nCount < 0)
      nCount = 0;

    if (nFirst + nCount > GetData()->nDataLength)
      nCount = GetData()->nDataLength - nFirst;
    if (nFirst > GetData()->nDataLength)
      nCount = 0;

    ASSERT(nFirst >= 0);
    ASSERT(nFirst + nCount <= GetData()->nDataLength);

    // optimize case of returning entire string
    if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
      return *this;

    CString dest;
    AllocCopy(dest, nCount, nFirst, 0);
    return dest;
  }

  CString Right(int nCount) const
  {
    if (nCount < 0)
      nCount = 0;
    if (nCount >= GetData()->nDataLength)
      return *this;

    CString dest;
    AllocCopy(dest, nCount, GetData()->nDataLength-nCount, 0);
    return dest;
  }

  CString Left(int nCount) const
  {
    if (nCount < 0)
      nCount = 0;
    if (nCount >= GetData()->nDataLength)
      return *this;

    CString dest;
    AllocCopy(dest, nCount, 0, 0);
    return dest;
  }

  // strspn equivalent
  CString SpanIncluding(const char* lpszCharSet) const
  {
    ASSERT(lpszCharSet!=NULL);
    return Left(strspn(m_pchData, lpszCharSet));
  }

  // strcspn equivalent
  CString SpanExcluding(const char* lpszCharSet) const
  {
    ASSERT(lpszCharSet!=NULL);
    return Left(strcspn(m_pchData, lpszCharSet));
  }

  //////////////////////////////////////////////////////////////////////////////
  // Finding

  int ReverseFind(TCHAR ch) const
  {
    // find last single character
    char* lpsz = strrchr(m_pchData,  ch);

    // return -1 if not found, distance from beginning otherwise
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
  }

  // find a sub-string (like strstr)
  int Find(const char* lpszSub) const
  {
    return Find(lpszSub, 0);
  }

  int Find(const char* lpszSub, int nStart) const
  {
    ASSERT(lpszSub!=NULL);

    int nLength = GetData()->nDataLength;
    if (nStart > nLength)
      return -1;

    // find first matching substring
    char* lpsz = strstr(m_pchData + nStart, lpszSub);

    // return -1 for not found, distance from beginning otherwise
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
  }


  /////////////////////////////////////////////////////////////////////////////
  // CString formatting

#define TCHAR_ARG   char
#define WCHAR_ARG   wchar_t
#define CHAR_ARG    char
typedef wchar_t* LPWSTR;
#define DOUBLE_ARG  double

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

  void FormatV(const char* lpszFormat, va_list argList)
  {
    ASSERT(lpszFormat!=NULL);

    va_list argListSave = argList;

    // make a guess at the maximum length of the resulting string
    int nMaxLen = 0;
    for (const char* lpsz = lpszFormat; *lpsz != '\0'; lpsz = _strinc(lpsz))
    {
      // handle '%' character, but watch out for '%%'
      if (*lpsz != '%' || *(lpsz = _strinc(lpsz)) == '%')
      {
        nMaxLen += strlen(lpsz);
        continue;
      }

      int nItemLen = 0;

      // handle '%' character with format
      int nWidth = 0;
      for (; *lpsz != '\0'; lpsz = _strinc(lpsz))
      {
        // check for valid flags
        if (*lpsz == '#')
          nMaxLen += 2;   // for '0x'
        else if (*lpsz == '*')
          nWidth = va_arg(argList, int);
        else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
          *lpsz == ' ')
          ;
        else // hit non-flag character
          break;
      }
      // get width and skip it
      if (nWidth == 0)
      {
        // width indicated by
        nWidth = atoi(lpsz);
        for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _strinc(lpsz))
          ;
      }
      ASSERT(nWidth >= 0);

      int nPrecision = 0;
      if (*lpsz == '.')
      {
        // skip past '.' separator (width.precision)
        lpsz = _strinc(lpsz);

        // get precision and skip it
        if (*lpsz == '*')
        {
          nPrecision = va_arg(argList, int);
          lpsz = _strinc(lpsz);
        }
        else
        {
          nPrecision = atoi(lpsz);
          for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = _strinc(lpsz))
            ;
        }
        ASSERT(nPrecision >= 0);
      }

      // should be on type modifier or specifier
      int nModifier = 0;
      if (strncmp(lpsz, ("I64"), 3) == 0)
      {
        lpsz += 3;
        nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
        // __int64 is only available on X86 and ALPHA platforms
        ASSERT(FALSE);
#endif
      }
      else
      {
        switch (*lpsz)
        {
          // modifiers that affect size
        case 'h':
          nModifier = FORCE_ANSI;
          lpsz = _strinc(lpsz);
          break;
        case 'l':
          nModifier = FORCE_UNICODE;
          lpsz = _strinc(lpsz);
          break;

          // modifiers that do not affect size
        case 'F':
        case 'N':
        case 'L':
          lpsz = _strinc(lpsz);
          break;
        }
      }

      // now should be on specifier
      switch (*lpsz | nModifier)
      {
        // single characters
      case 'c':
      case 'C':
        nItemLen = 2;
        va_arg(argList, TCHAR_ARG);
        break;
      case 'c'|FORCE_ANSI:
      case 'C'|FORCE_ANSI:
        nItemLen = 2;
        va_arg(argList, CHAR_ARG);
        break;
      case 'c'|FORCE_UNICODE:
      case 'C'|FORCE_UNICODE:
        nItemLen = 2;
        va_arg(argList, WCHAR_ARG);
        break;

        // strings
      case 's':
        {
          const char* pstrNextArg = va_arg(argList, const char*);
          if (pstrNextArg == NULL)
            nItemLen = 6;  // "(null)"
          else
          {
            nItemLen = strlen(pstrNextArg);
            nItemLen = MAX(1, nItemLen);
          }
        }
        break;

      case 'S':
        {
          LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
          if (pstrNextArg == NULL)
            nItemLen = 6; // "(null)"
          else
          {
            nItemLen = strlen(pstrNextArg);
            nItemLen = MAX(1, nItemLen);
          }
        }
        break;

      case 's'|FORCE_ANSI:
      case 'S'|FORCE_ANSI:
        {
          LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
          if (pstrNextArg == NULL)
            nItemLen = 6; // "(null)"
          else
          {
            nItemLen = strlen(pstrNextArg);
            nItemLen = MAX(1, nItemLen);
          }
        }
        break;

      case 's'|FORCE_UNICODE:
      case 'S'|FORCE_UNICODE:
        {
          LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
          if (pstrNextArg == NULL)
            nItemLen = 6; // "(null)"
          else
          {
            nItemLen = wcslen(pstrNextArg);
            nItemLen = MAX(1, nItemLen);
          }
        }
        break;
      }

      // adjust nItemLen for strings
      if (nItemLen != 0)
      {
        if (nPrecision != 0)
          nItemLen = MIN(nItemLen, nPrecision);
        nItemLen = MAX(nItemLen, nWidth);
      }
      else
      {
        switch (*lpsz)
        {
          // integers
        case 'd':
        case 'i':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
          if (nModifier & FORCE_INT64)
            va_arg(argList, int64);
          else
            va_arg(argList, int);
          nItemLen = 32;
          nItemLen = MAX(nItemLen, nWidth+nPrecision);
          break;

        case 'e':
        case 'g':
        case 'G':
          va_arg(argList, DOUBLE_ARG);
          nItemLen = 128;
          nItemLen = MAX(nItemLen, nWidth+nPrecision);
          break;

        case 'f':
          {
            double f;
            char pszTemp[64];

            // 312 == strlen("-1+(309 zeroes).")
            // 309 zeroes == MAX precision of a double
            // 6 == adjustment in case precision is not specified,
            //   which means that the precision defaults to 6
            //pszTemp = (char*)_alloca(MAX(nWidth, 312+nPrecision+6));

            f = va_arg(argList, double);
            snprintf( pszTemp, 64, ( "%*.*f" ), nWidth, nPrecision+6, f );
            nItemLen = strlen(pszTemp);
          }
          break;

        case 'p':
          va_arg(argList, void*);
          nItemLen = 32;
          nItemLen = MAX(nItemLen, nWidth+nPrecision);
          break;

          // no output
        case 'n':
          va_arg(argList, int*);
          break;

        default:
          ASSERT(FALSE);  // unknown formatting option
        }
      }

      // adjust nMaxLen for output nItemLen
      nMaxLen += nItemLen;
  }

  GetBuffer(nMaxLen);
  VERIFY(_vsnprintf(m_pchData, nMaxLen, lpszFormat, argListSave) <= GetAllocLength());
  ReleaseBuffer();

  va_end(argListSave);
}

// formatting (using wsprintf style formatting)
void Format(const char* lpszFormat, ...)
{
  ASSERT(lpszFormat!=NULL);

  va_list argList;
  va_start(argList, lpszFormat);
  FormatV(lpszFormat, argList);
  va_end(argList);
}

#ifdef _WIN32
// formatting (using FormatMessage style formatting)
int FormatMessage(const char* lpszFormat, ...)
{
  // format message into temporary buffer lpszTemp
  va_list argList;
  va_start(argList, lpszFormat);
  char* lpszTemp;

  if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
    lpszFormat, 0, 0, (char*)&lpszTemp, 0, &argList) == 0 ||
    lpszTemp == NULL)
  {
    return 0;
  }

  // assign lpszTemp into the resulting string and free the temporary
  *this = lpszTemp;
  LocalFree(lpszTemp);
  va_end(argList);
  return 1;
}
#endif

void TrimRight(const char* lpszTargetList)
{
  // find beginning of trailing matches
  // by starting at beginning (DBCS aware)

  CopyBeforeWrite();
  char* lpsz = m_pchData;
  char* lpszLast = NULL;

  while (*lpsz != '\0')
  {
    if (strchr(lpszTargetList, *lpsz) != NULL)
    {
      if (lpszLast == NULL)
        lpszLast = lpsz;
    }
    else
      lpszLast = NULL;
    lpsz = _strinc(lpsz);
  }

  if (lpszLast != NULL)
  {
    // truncate at left-most matching character
    *lpszLast = '\0';
    GetData()->nDataLength = lpszLast - m_pchData;
  }
}

void TrimRight(TCHAR chTarget)
{
  // find beginning of trailing matches
  // by starting at beginning (DBCS aware)

  CopyBeforeWrite();
  char* lpsz = m_pchData;
  char* lpszLast = NULL;

  while (*lpsz != '\0')
  {
    if (*lpsz == chTarget)
    {
      if (lpszLast == NULL)
        lpszLast = lpsz;
    }
    else
      lpszLast = NULL;
    lpsz = _strinc(lpsz);
  }

  if (lpszLast != NULL)
  {
    // truncate at left-most matching character
    *lpszLast = '\0';
    GetData()->nDataLength = lpszLast - m_pchData;
  }
}

void TrimRight()
{
  // find beginning of trailing spaces by starting at beginning (DBCS aware)

  CopyBeforeWrite();
  char* lpsz = m_pchData;
  char* lpszLast = NULL;

  while (*lpsz != '\0')
  {
    if (isspace(*lpsz))
    {
      if (lpszLast == NULL)
        lpszLast = lpsz;
    }
    else
      lpszLast = NULL;
    lpsz = _strinc(lpsz);
  }

  if (lpszLast != NULL)
  {
    // truncate at trailing space start
    *lpszLast = '\0';
    GetData()->nDataLength = lpszLast - m_pchData;
  }
}

void TrimLeft(const char* lpszTargets)
{
  // if we're not trimming anything, we're not doing any work
  if (SafeStrlen(lpszTargets) == 0)
    return;

  CopyBeforeWrite();
  const char* lpsz = m_pchData;

  while (*lpsz != '\0')
  {
    if (strchr(lpszTargets, *lpsz) == NULL)
      break;
    lpsz = _strinc(lpsz);
  }

  if (lpsz != m_pchData)
  {
    // fix up data and length
    int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
    memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(TCHAR));
    GetData()->nDataLength = nDataLength;
  }
}

void TrimLeft(TCHAR chTarget)
{
  // find first non-matching character

  CopyBeforeWrite();
  const char* lpsz = m_pchData;

  while (chTarget == *lpsz)
    lpsz = _strinc(lpsz);

  if (lpsz != m_pchData)
  {
    // fix up data and length
    int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
    memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(TCHAR));
    GetData()->nDataLength = nDataLength;
  }
}

void TrimLeft()
{
  // find first non-space character

  CopyBeforeWrite();
  const char* lpsz = m_pchData;

  while (isspace(*lpsz))
    lpsz = _strinc(lpsz);

  if (lpsz != m_pchData)
  {
    // fix up data and length
    int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
    memmove(m_pchData, lpsz, (nDataLength+1)*sizeof(TCHAR));
    GetData()->nDataLength = nDataLength;
  }
}



};


///////////////////////////////////////////////////////////////////////////////


CString operator+(const CString& string1, TCHAR ch)
{
  CString s;
  s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &ch);
  return s;
}

CString operator+(TCHAR ch, const CString& string)
{
  CString s;
  s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pchData);
  return s;
}

CString operator+(const CString& string1, const CString& string2)
{
  CString s;
  s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData,
    string2.GetData()->nDataLength, string2.m_pchData);
  return s;
}

CString operator+(const CString& string, const char* lpsz)
{
  ASSERT(lpsz == NULL);
  CString s;
  s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData,
    SafeStrlen(lpsz), lpsz);
  return s;
}

CString operator+(const char* lpsz, const CString& string)
{
  ASSERT(lpsz != NULL);
  CString s;
  s.ConcatCopy(SafeStrlen(lpsz), lpsz, string.GetData()->nDataLength,
    string.m_pchData);
  return s;
}

bool operator==(const CString& s1, const CString& s2)
{ return s1.Compare(s2.c_str()) == 0; }
bool operator==(const CString& s1, const char* s2)
{ return s1.Compare(s2) == 0; }
bool operator==(const char* s1, const CString& s2)
{ return s2.Compare(s1) == 0; }
bool operator!=(const CString& s1, const CString& s2)
{ return s1.Compare(s2.c_str()) != 0; }
bool operator!=(const CString& s1, const char* s2)
{ return s1.Compare(s2) != 0; }
bool operator!=(const char* s1, const CString& s2)
{ return s2.Compare(s1) != 0; }
bool operator<(const CString& s1, const CString& s2)
{ return s1.Compare(s2.c_str()) < 0; }
bool operator<(const CString& s1, const char* s2)
{ return s1.Compare(s2) < 0; }
bool operator<(const char* s1, const CString& s2)
{ return s2.Compare(s1) > 0; }
bool operator>(const CString& s1, const CString& s2)
{ return s1.Compare(s2.c_str()) > 0; }
bool operator>(const CString& s1, const char* s2)
{ return s1.Compare(s2) > 0; }
bool operator>(const char* s1, const CString& s2)
{ return s2.Compare(s1) < 0; }
bool operator<=(const CString& s1, const CString& s2)
{ return s1.Compare(s2.c_str()) <= 0; }
bool operator<=(const CString& s1, const char* s2)
{ return s1.Compare(s2) <= 0; }
bool operator<=(const char* s1, const CString& s2)
{ return s2.Compare(s1) >= 0; }
bool operator>=(const CString& s1, const CString& s2)
{ return s1.Compare(s2.c_str()) >= 0; }
bool operator>=(const CString& s1, const char* s2)
{ return s1.Compare(s2) >= 0; }
bool operator>=(const char* s1, const CString& s2)
{ return s2.Compare(s1) <= 0; }

///////////////////////////////////////////////////////////////////////////////

static inline void ConstructElement(CString* pNewData)
{
  memcpy(pNewData, &afxEmptyString, sizeof(CString));
}

static inline void DestructElement(CString* pOldData)
{
  pOldData->~CString();
}

static inline void CopyElement(CString* pSrc, CString* pDest)
{
  *pSrc = *pDest;
}



static void ConstructElements(CString* pNewData, int nCount)
{
  ASSERT(nCount >= 0);
  
  while (nCount--)
  {
    ConstructElement(pNewData);
    pNewData++;
  }
}

static void DestructElements(CString* pOldData, int nCount)
{
  ASSERT(nCount >= 0);
  
  while (nCount--)
  {
    DestructElement(pOldData);
    pOldData++;
  }
}

static void CopyElements(CString* pDest, CString* pSrc, int nCount)
{
  ASSERT(nCount >= 0);
  
  while (nCount--)
  {
    *pDest = *pSrc;
    ++pDest;
    ++pSrc;
  }
}



#endif // __CSTRING_H__
