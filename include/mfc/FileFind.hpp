
#ifndef __FILEFIND_HPP__
#define __FILEFIND_HPP__

#include "DumpContext.hpp"
#include "String.hpp"
#include "Time.hpp"
#include "std/dir_c.h"
#define UNUSED_ALWAYS(x) x

////////////////////////////////////////////////////////////////////////////
// Local file searches

struct CFileFind
{
  fileinfo_t m_pFoundInfo[1];
  dir_t m_hContext[1];
  BOOL m_bGotLast;
  CString m_strRoot;
  TCHAR m_chDirSeparator;     // not '\\' for Internet classes
  int m_Flag;
  
  // CFileFind
  _AFX_INLINE BOOL IsReadOnly() const
  { return MatchesMask(FILE_ATTRIBUTE_READONLY); }
  _AFX_INLINE BOOL IsDirectory() const
  { return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
  _AFX_INLINE BOOL IsCompressed() const
  { return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
  _AFX_INLINE BOOL IsSystem() const
  { return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
  _AFX_INLINE BOOL IsHidden() const
  { return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
  _AFX_INLINE BOOL IsTemporary() const
  { return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
  _AFX_INLINE BOOL IsNormal() const
  { return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
  _AFX_INLINE BOOL IsArchived() const
  { return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }
  
  CFileFind()
  {
    memset(m_pFoundInfo, 0, sizeof(*m_pFoundInfo));
    memset(m_hContext, 0, sizeof(*m_hContext));
    m_chDirSeparator = '\\';
    m_Flag = 0;
  }
  
  ~CFileFind()
  {
    Close();
  }
  
  void Close()
  { 
    if (m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
    {
      CloseContext();
    }
  }
  
  void CloseContext()
  {
    ::sys_find_close(m_hContext);
    return;
  }
  
  BOOL FindFile(LPCTSTR pstrName = NULL,	DWORD flag = 0)
  {
    Close();
    m_bGotLast = FALSE;
    m_Flag = flag;
    
    if (pstrName == NULL)
      pstrName = ("*.*");
    //lstrcpy(m_pFoundInfo->name, pstrName);
    
    sys_find_next_file(m_hContext, m_strRoot.c_str(), pstrName, m_pFoundInfo, m_Flag);
    
    if (m_hContext == INVALID_HANDLE_VALUE)
    {
      DWORD dwTemp = ::GetLastError();
      Close();
      ::SetLastError(dwTemp);
      return FALSE;
    }
    
    LPTSTR pstrRoot = m_strRoot.GetBufferSetLength(_MAX_PATH);
    LPCTSTR pstr = _fullpath(pstrRoot, pstrName, _MAX_PATH);
    
    // passed name isn't a valid path but was found by the API
    ASSERT(pstr != NULL);
    if (pstr == NULL)
    {
      m_strRoot.ReleaseBuffer(-1);
      Close();
      ::SetLastError(ERROR_INVALID_NAME);
      return FALSE;
    }
    else
    {
      // find the last forward or backward whack
      LPTSTR pstrBack  = strchr(pstrRoot, '\\');
      LPTSTR pstrFront = strchr(pstrRoot, '/');
      
      if (pstrFront != NULL || pstrBack != NULL)
      {
        if (pstrFront == NULL)
          pstrFront = pstrRoot;
        if (pstrBack == NULL)
          pstrBack = pstrRoot;
        
        // from the start to the last whack is the root
        
        if (pstrFront >= pstrBack)
          *pstrFront = '\0';
        else
          *pstrBack = '\0';
      }
      m_strRoot.ReleaseBuffer(-1);
    }
    
    return TRUE;
  }
  
  BOOL MatchesMask(DWORD dwMask) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL)
      return (!!(((LPWIN32_FIND_DATA) m_pFoundInfo)->dwFileAttributes & dwMask));
    else
      return FALSE;
  }
  
  BOOL GetLastAccessTime(FILETIME* pTimeStamp) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT(pTimeStamp != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL && pTimeStamp != NULL)
    {
      *pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastAccessTime;
      return TRUE;
    }
    else
      return FALSE;
  }
  
  BOOL GetLastWriteTime(FILETIME* pTimeStamp) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT(pTimeStamp != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL && pTimeStamp != NULL)
    {
      *pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastWriteTime;
      return TRUE;
    }
    else
      return FALSE;
  }
  
  BOOL GetCreationTime(FILETIME* pTimeStamp) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL && pTimeStamp != NULL)
    {
      *pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftCreationTime;
      return TRUE;
    }
    else
      return FALSE;
  }
  
  BOOL GetLastAccessTime(CTime& refTime) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL)
    {
      refTime = CTime(((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastAccessTime);
      return TRUE;
    }
    else
      return FALSE;
  }
  
  BOOL GetLastWriteTime(CTime& refTime) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL)
    {
      refTime = CTime(((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastWriteTime);
      return TRUE;
    }
    else
      return FALSE;
  }
  
  BOOL GetCreationTime(CTime& refTime) const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL)
    {
      refTime = CTime(((LPWIN32_FIND_DATA) m_pFoundInfo)->ftCreationTime);
      return TRUE;
    }
    else
      return FALSE;
  }
  
  BOOL IsDots() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    // return TRUE if the file name is "." or ".." and
    // the file is a directory
    
    BOOL bResult = FALSE;
    if (m_pFoundInfo != NULL && IsDirectory())
    {
      LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA) m_pFoundInfo;
      if (pFindData->cFileName[0] == '.')
      {
        if (pFindData->cFileName[1] == '\0' ||
          (pFindData->cFileName[1] == '.' &&
          pFindData->cFileName[2] == '\0'))
        {
          bResult = TRUE;
        }
      }
    }
    
    return bResult;
  }
  
  BOOL FindNextFile()
  {
    ASSERT_VALID(this);
    return sys_find_next_file(m_hContext, m_strRoot.c_str(), NULL, m_pFoundInfo, m_Flag);
  }
  
  CString GetFileURL() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    CString strResult("file://");
    strResult += GetFilePath();
    return strResult;
  }
  
  CString GetRoot() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    return m_strRoot;
  }
  
  CString GetFilePath() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    CString strResult = m_strRoot;
    if (strResult[strResult.GetLength()-1] != '\\' &&
      strResult[strResult.GetLength()-1] != '/')
      strResult += m_chDirSeparator;
    strResult += GetFileName();
    return strResult;
  }
  
  CString GetFileTitle() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    CString strFullName = GetFileName();
    CString strResult;
    
    _splitpath(strFullName.c_str(), NULL, NULL, strResult.GetBuffer(_MAX_PATH), NULL);
    strResult.ReleaseBuffer();
    return strResult;
  }
  
  CString GetFileName() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    CString ret;
    
    if (m_pFoundInfo != NULL)
      ret = ((LPWIN32_FIND_DATA) m_pFoundInfo)->cFileName;
    return ret;
  }
  
  DWORD GetLength() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL)
      return ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow;
    else
      return 0;
  }
  
#if defined(_X86_) || defined(_ALPHA_)
  __int64 GetLength64() const
  {
    ASSERT(m_hContext != NULL);
    ASSERT_VALID(this);
    
    if (m_pFoundInfo != NULL)
      return ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow +
      (((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeHigh << 32);
    else
      return 0;
  }
#endif
  
  void Dump(CDumpContext& dc) const
  {
    dc << "\nm_hContext = " << (UINT) m_hContext;
  }
  
  void AssertValid() const
  {
    // if you trip the ASSERT in the else side, you've called
    // a Get() function without having done at least one
    // FindNext() call
    
    if (m_hContext == NULL)
      ASSERT(m_pFoundInfo == NULL && m_pFoundInfo == NULL);
    else
      ASSERT(m_pFoundInfo != NULL && m_pFoundInfo != NULL);
    
  }
  
};

#endif // __FILEFIND_HPP__
