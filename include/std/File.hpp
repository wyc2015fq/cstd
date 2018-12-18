
#ifndef __FILE_HPP__
#define __FILE_HPP__

#include "DumpContext.hpp"
#include "Time.hpp"

inline BOOL IsDirSep(TCHAR ch)
{
  return (ch == '\\' || ch == '/');
}


void AfxGetRoot(LPCTSTR lpszPath, CString& strRoot)
{
  ASSERT(lpszPath != NULL);
  // determine the root name of the volume
  LPTSTR lpszRoot = strRoot.GetBuffer(_MAX_PATH);
  memset(lpszRoot, 0, _MAX_PATH);
  lstrcpyn(lpszRoot, lpszPath, _MAX_PATH);
  LPTSTR lpsz = lpszRoot;
  for (; *lpsz != '\0'; lpsz = _strinc(lpsz))
  {
    // find first double slash and stop
    if (IsDirSep(lpsz[0]) && IsDirSep(lpsz[1]))
      break;
  }
  if (*lpsz != '\0')
  {
    // it is a UNC name, find second slash past '\\'
    ASSERT(IsDirSep(lpsz[0]));
    ASSERT(IsDirSep(lpsz[1]));
    lpsz += 2;
    while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
      lpsz = _strinc(lpsz);
    if (*lpsz != '\0')
      lpsz = _strinc(lpsz);
    while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
      lpsz = _strinc(lpsz);
    // terminate it just after the UNC root (ie. '\\server\share\')
    if (*lpsz != '\0')
      lpsz[1] = '\0';
  }
  else
  {
    // not a UNC, look for just the first slash
    lpsz = lpszRoot;
    while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
      lpsz = _strinc(lpsz);
    // terminate it just after root (ie. 'x:\')
    if (*lpsz != '\0')
      lpsz[1] = '\0';
  }
  strRoot.ReleaseBuffer();
}


// turn a file, relative path or other into an absolute path
BOOL AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn)
	// lpszPathOut = buffer of _MAX_PATH
	// lpszFileIn = file, relative path or absolute path
	// (both in ANSI character set)
{
	ASSERT(AfxIsValidAddress(lpszPathOut, _MAX_PATH));

	// first, fully qualify the path name
	LPTSTR lpszFilePart;
	if (!GetFullPathName(lpszFileIn, _MAX_PATH, lpszPathOut, &lpszFilePart))
	{
#ifdef _DEBUG
		if (lpszFileIn[0] != '\0')
			TRACE1("Warning: could not parse the path '%s'.\n", lpszFileIn);
#endif
		lstrcpyn(lpszPathOut, lpszFileIn, _MAX_PATH); // take it literally
		return FALSE;
	}

	CString strRoot;
	// determine the root name of the volume
	AfxGetRoot(lpszPathOut, strRoot);

	// get file system information for the volume
	DWORD dwFlags, dwDummy;
	if (!GetVolumeInformation(strRoot.c_str(), NULL, 0, NULL, &dwDummy, &dwFlags,
		NULL, 0))
	{
		TRACE1("Warning: could not get volume information '%s'.\n",
			strRoot.c_str());
		return FALSE;   // preserving case may not be correct
	}

	// not all characters have complete uppercase/lowercase
	if (!(dwFlags & FS_CASE_IS_PRESERVED))
		CharUpper(lpszPathOut);

	// assume non-UNICODE file systems, use OEM character set
	if (!(dwFlags & FS_UNICODE_STORED_ON_DISK))
	{
		WIN32_FIND_DATA data;
		HANDLE h = FindFirstFile(lpszFileIn, &data);
		if (h != INVALID_HANDLE_VALUE)
		{
			FindClose(h);
			lstrcpy(lpszFilePart, data.cFileName);
		}
	}
	return TRUE;
}

BOOL AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2)
{
	// use case insensitive compare as a starter
	if (lstrcmpi(lpszPath1, lpszPath2) != 0)
		return FALSE;

	// on non-DBCS systems, we are done
	if (!GetSystemMetrics(SM_DBCSENABLED))
		return TRUE;

	// on DBCS systems, the file name may not actually be the same
	// in particular, the file system is case sensitive with respect to
	// "full width" roman characters.
	// (ie. fullwidth-R is different from fullwidth-r).
	int nLen = lstrlen(lpszPath1);
	if (nLen != lstrlen(lpszPath2))
		return FALSE;
	ASSERT(nLen < _MAX_PATH);

	// need to get both CT_CTYPE1 and CT_CTYPE3 for each filename
	LCID lcid = GetThreadLocale();
	WORD aCharType11[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
	WORD aCharType13[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
	WORD aCharType21[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
#ifdef _DEBUG
	WORD aCharType23[_MAX_PATH];
	VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
#endif

	// for every C3_FULLWIDTH character, make sure it has same C1 value
	int i = 0;
	for (LPCTSTR lpsz = lpszPath1; *lpsz != 0; lpsz = _strinc(lpsz))
	{
		// check for C3_FULLWIDTH characters only
		if (aCharType13[i] & C3_FULLWIDTH)
		{
#ifdef _DEBUG
			ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
#endif

			// if CT_CTYPE1 is different then file system considers these
			// file names different.
			if (aCharType11[i] != aCharType21[i])
				return FALSE;
		}
		++i; // look at next character type
	}
	return TRUE; // otherwise file name is truly the same
}
#undef GetFileTitle
short GetFileTitle(LPCTSTR lpszFile, LPTSTR lpszTitle, WORD cbBuf)
{ return 0;}

UINT AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
  ASSERT(lpszTitle == NULL ||
    AfxIsValidAddress(lpszTitle, _MAX_FNAME));
  ASSERT(AfxIsValidString(lpszPathName));
  
  // always capture the complete file name including extension (if present)
  LPTSTR lpszTemp = (LPTSTR)lpszPathName;
  for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _strinc(lpsz))
  {
    // remember last directory/drive separator
    if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
      lpszTemp = (LPTSTR)_strinc(lpsz);
  }
  
  // lpszTitle can be NULL which just returns the number of bytes
  if (lpszTitle == NULL)
    return lstrlen(lpszTemp)+1;
  
  // otherwise copy it into the buffer provided
  lstrcpyn(lpszTitle, lpszTemp, nMax);
  return 0;
}
UINT AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
	ASSERT(lpszTitle == NULL ||
		AfxIsValidAddress(lpszTitle, _MAX_FNAME));
	ASSERT(AfxIsValidString(lpszPathName));

	// use a temporary to avoid bugs in ::GetFileTitle when lpszTitle is NULL
	TCHAR szTemp[_MAX_PATH];
	LPTSTR lpszTemp = lpszTitle;
	if (lpszTemp == NULL)
	{
		lpszTemp = szTemp;
		nMax = _countof(szTemp);
	}
	if (::GetFileTitle(lpszPathName, lpszTemp, (WORD)nMax) != 0)
	{
		// when ::GetFileTitle fails, use cheap imitation
		return AfxGetFileName(lpszPathName, lpszTitle, nMax);
	}
	return lpszTitle == NULL ? lstrlen(lpszTemp)+1 : 0;
}

/////////////////////////////////////////////////////////////////////////////
// File status
#ifndef _MAX_PATH
#define _MAX_PATH  260
#endif
struct CFileStatus
{
	CTime m_ctime;          // creation date/time of file
	CTime m_mtime;          // last modification date/time of file
	CTime m_atime;          // last access date/time of file
	LONG m_size;            // logical size of file in bytes
	BYTE m_attribute;       // logical OR of CFile::Attribute enum values
	BYTE _m_padding;        // pad the structure to a WORD
	TCHAR m_szFullName[_MAX_PATH]; // absolute path name

  void Dump(CDumpContext& dc) const
  {
    dc << "a CFileStatus at " << (void*)this;

    dc << "\nm_ctime = " << m_ctime;
    dc << "\nm_mtime = " << m_mtime;
    dc << "\nm_atime = " << m_atime;
    dc << "\nm_size = " << m_size;
    dc << "\nm_attribute = " << m_attribute;
    dc << "\nm_szFullName = " << m_szFullName;

    dc << "\n";
}

};

/////////////////////////////////////////////////////////////////////////////
// File - raw unbuffered disk file I/O

class CFile
{
	DECLARE_DYNAMIC(CFile)

public:
// Flag values
	enum OpenFlags {
		modeRead =          0x0000,
		modeWrite =         0x0001,
		modeReadWrite =     0x0002,
		shareCompat =       0x0000,
		shareExclusive =    0x0010,
		shareDenyWrite =    0x0020,
		shareDenyRead =     0x0030,
		shareDenyNone =     0x0040,
		modeNoInherit =     0x0080,
		modeCreate =        0x1000,
		modeNoTruncate =    0x2000,
		typeText =          0x4000, // typeText and typeBinary are used in
		typeBinary =   (int)0x8000 // derived classes only
		};

	enum Attribute {
		normal =    0x00,
		readOnly =  0x01,
		hidden =    0x02,
		system =    0x04,
		volume =    0x08,
		directory = 0x10,
		archive =   0x20
		};

	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
  
	enum { hFileNull = -1 };
	enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };

protected:
	UINT m_hFile;
	BOOL m_bCloseOnDelete;
  CString m_strFileName;
  // CFile
  _AFX_INLINE CFile::operator HFILE() const
  { return m_hFile; }
  _AFX_INLINE DWORD CFile::ReadHuge(void* lpBuffer, DWORD dwCount)
  { return (DWORD)Read(lpBuffer, (UINT)dwCount); }
  _AFX_INLINE void CFile::WriteHuge(const void* lpBuffer, DWORD dwCount)
  { Write(lpBuffer, (UINT)dwCount); }
  _AFX_INLINE DWORD CFile::SeekToEnd()
  { return Seek(0, CFile::end); }
  _AFX_INLINE void CFile::SeekToBegin()
  { Seek(0, CFile::begin); }
  _AFX_INLINE void CFile::SetFilePath(LPCTSTR lpszNewName)
  {
    ASSERT_VALID(this);
    ASSERT(AfxIsValidString(lpszNewName));
    m_strFileName = lpszNewName;
  }

CFile::CFile()
{
	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;
}

CFile::CFile(int hFile)
{
	m_hFile = hFile;
	m_bCloseOnDelete = FALSE;
}

CFile::CFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	ASSERT(AfxIsValidString(lpszFileName));

  if (!Open(lpszFileName, nOpenFlags)) {
		//AfxThrowFileException(e.m_cause, e.m_lOsError, e.m_strFileName);
  }
}

CFile::~CFile()
{
	if (m_hFile != (UINT)hFileNull && m_bCloseOnDelete)
		Close();
}

CFile* CFile::Duplicate() const
{
#if 0
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	CFile* pFile = new CFile(hFileNull);
	HANDLE hFile;
	if (!::DuplicateHandle(::GetCurrentProcess(), (HANDLE)m_hFile,
		::GetCurrentProcess(), &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		delete pFile;
		FileError((LONG)::GetLastError());
	}
	pFile->m_hFile = (UINT)hFile;
	ASSERT(pFile->m_hFile != (UINT)hFileNull);
	pFile->m_bCloseOnDelete = m_bCloseOnDelete;
	return pFile;
#else
  return NULL;
#endif
}

BOOL CFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidString(lpszFileName));
	//ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));
	ASSERT((nOpenFlags & typeText) == 0);   // text mode not supported

	// CFile objects are always binary and CreateFile does not need flag
	nOpenFlags &= ~(UINT)typeBinary;

	m_bCloseOnDelete = FALSE;
	m_hFile = (UINT)hFileNull;
	m_strFileName.Empty();

	TCHAR szTemp[_MAX_PATH];
	AfxFullPath(szTemp, lpszFileName);
	m_strFileName = szTemp;

	ASSERT(sizeof(HANDLE) == sizeof(UINT));
	ASSERT(shareCompat == 0);

	// map read/write mode
	ASSERT((modeRead|modeWrite|modeReadWrite) == 3);
	DWORD dwAccess = 0;
	switch (nOpenFlags & 3)
	{
	case modeRead:
		dwAccess = GENERIC_READ;
		break;
	case modeWrite:
		dwAccess = GENERIC_WRITE;
		break;
	case modeReadWrite:
		dwAccess = GENERIC_READ|GENERIC_WRITE;
		break;
	default:
		ASSERT(FALSE);  // invalid share mode
	}

	// map share mode
	DWORD dwShareMode = 0;
	switch (nOpenFlags & 0x70)    // map compatibility mode to exclusive
	{
	default:
		ASSERT(FALSE);  // invalid share mode?
	case shareCompat:
	case shareExclusive:
		dwShareMode = 0;
		break;
	case shareDenyWrite:
		dwShareMode = FILE_SHARE_READ;
		break;
	case shareDenyRead:
		dwShareMode = FILE_SHARE_WRITE;
		break;
	case shareDenyNone:
		dwShareMode = FILE_SHARE_WRITE|FILE_SHARE_READ;
		break;
	}

	// Note: typeText and typeBinary are used in derived classes only.

	// map modeNoInherit flag
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

	// map creation flags
	DWORD dwCreateFlag;
	if (nOpenFlags & modeCreate)
	{
		if (nOpenFlags & modeNoTruncate)
			dwCreateFlag = OPEN_ALWAYS;
		else
			dwCreateFlag = CREATE_ALWAYS;
	}
	else
		dwCreateFlag = OPEN_EXISTING;

	// attempt file creation
	HANDLE hFile = ::CreateFile(lpszFileName, dwAccess, dwShareMode, &sa,
		dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		{
			int m_lOsError = ::GetLastError();
//			pException->m_strFileName = lpszFileName;
		}
		return FALSE;
	}
	m_hFile = (HFILE)hFile;
	m_bCloseOnDelete = TRUE;

	return TRUE;
}

UINT CFile::Read(void* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	if (nCount == 0)
		return 0;   // avoid Win32 "null-read"

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	DWORD dwRead;
  if (!::ReadFile((HANDLE)m_hFile, lpBuf, nCount, &dwRead, NULL)) {
    // FileError((LONG)::GetLastError());
  }

	return (UINT)dwRead;
}

void CFile::Write(const void* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	if (nCount == 0)
		return;     // avoid Win32 "null-write" option

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	DWORD nWritten;
  if (!::WriteFile((HANDLE)m_hFile, lpBuf, nCount, &nWritten, NULL)) {
		// FileError((LONG)::GetLastError(), m_strFileName);
  }

	// Win32s will not return an error all the time (usually DISK_FULL)
  if (nWritten != nCount) {
		// AfxThrowFileException(CFileException::diskFull, -1, m_strFileName);
  }
}

LONG CFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);
	ASSERT(nFrom == begin || nFrom == end || nFrom == current);
	ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);
#define FileError(x) return (x)
	DWORD dwNew = ::SetFilePointer((HANDLE)m_hFile, lOff, NULL, (DWORD)nFrom);
  if (dwNew  == (DWORD)-1) {
		FileError((LONG)::GetLastError());
  }

	return dwNew;
}

DWORD CFile::GetPosition() const
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	DWORD dwPos = ::SetFilePointer((HANDLE)m_hFile, 0, NULL, FILE_CURRENT);
	if (dwPos  == (DWORD)-1)
		FileError((LONG)::GetLastError());

	return dwPos;
}

int CFile::Flush()
{
	ASSERT_VALID(this);

	if (m_hFile == (UINT)hFileNull)
		return ;

	if (!::FlushFileBuffers((HANDLE)m_hFile))
		FileError((LONG)::GetLastError());
  return 0;
}

void CFile::Close()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	BOOL bError = FALSE;
	if (m_hFile != (UINT)hFileNull)
		bError = !::CloseHandle((HANDLE)m_hFile);

	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;
	m_strFileName.Empty();

	if (bError)
		FileError((LONG)::GetLastError());
}

void CFile::Abort()
{
	ASSERT_VALID(this);
	if (m_hFile != (UINT)hFileNull)
	{
		// close but ignore errors
		::CloseHandle((HANDLE)m_hFile);
		m_hFile = (UINT)hFileNull;
	}
	m_strFileName.Empty();
}

void CFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	if (!::LockFile((HANDLE)m_hFile, dwPos, 0, dwCount, 0))
		FileError((LONG)::GetLastError());
}

void CFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	if (!::UnlockFile((HANDLE)m_hFile, dwPos, 0, dwCount, 0))
		FileError((LONG)::GetLastError());
}

void CFile::SetLength(DWORD dwNewLen)
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != (UINT)hFileNull);

	Seek((LONG)dwNewLen, (UINT)begin);

	if (!::SetEndOfFile((HANDLE)m_hFile))
		FileError((LONG)::GetLastError());
}

DWORD CFile::GetLength() const
{
	ASSERT_VALID(this);

	DWORD dwLen, dwCur;

	// Seek is a non const operation
	CFile* pFile = (CFile*)this;
	dwCur = pFile->Seek(0L, current);
	dwLen = pFile->SeekToEnd();
	VERIFY(dwCur == (DWORD)pFile->Seek(dwCur, begin));

	return dwLen;
}

// CFile does not support direct buffering (CMemFile does)
UINT CFile::GetBufferPtr(UINT nCommand, UINT nCount = 0,
	void** ppBufStart = NULL, void** ppBufMax = NULL)
{
	ASSERT(nCommand == bufferCheck);
	UNUSED(nCommand);    // not used in retail build

	return 0;   // no support
}

void PASCAL CFile::Rename(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
{
	if (!::MoveFile((LPTSTR)lpszOldName, (LPTSTR)lpszNewName))
		FileError((LONG)::GetLastError());
}

void PASCAL CFile::Remove(LPCTSTR lpszFileName)
{
	if (!::DeleteFile((LPTSTR)lpszFileName))
		FileError((LONG)::GetLastError());
}

void CFile::AssertValid() const
{
  CObject::AssertValid();
  // we permit the descriptor m_hFile to be any value for derived classes
}

void CFile::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
  
  dc << "with handle " << (UINT)m_hFile;
  dc << " and name \"" << m_strFileName << "\"";
  dc << "\n";
}


CString CFile::GetFileName() const
{
	ASSERT_VALID(this);

	CFileStatus status;
	GetStatus(status);
	CString strResult;
	AfxGetFileName(status.m_szFullName, strResult.GetBuffer(_MAX_FNAME),
		_MAX_FNAME);
	strResult.ReleaseBuffer();
	return strResult;
}

CString CFile::GetFileTitle() const
{
	ASSERT_VALID(this);

	CFileStatus status;
	GetStatus(status);
	CString strResult;
	AfxGetFileTitle(status.m_szFullName, strResult.GetBuffer(_MAX_FNAME),
		_MAX_FNAME);
	strResult.ReleaseBuffer();
	return strResult;
}

CString CFile::GetFilePath() const
{
	ASSERT_VALID(this);

	CFileStatus status;
	GetStatus(status);
	return status.m_szFullName;
}

/////////////////////////////////////////////////////////////////////////////
// CFile Status implementation

static BOOL CFile::GetStatus(CFileStatus& rStatus) 
{
	ASSERT_VALID(this);

	memset(&rStatus, 0, sizeof(CFileStatus));

	// copy file name from cached m_strFileName
	lstrcpyn(rStatus.m_szFullName, m_strFileName,
		_countof(rStatus.m_szFullName));

	if (m_hFile != hFileNull)
	{
		// get time current file size
		FILETIME ftCreate, ftAccess, ftModify;
		if (!::GetFileTime((HANDLE)m_hFile, &ftCreate, &ftAccess, &ftModify))
			return FALSE;

		if ((rStatus.m_size = ::GetFileSize((HANDLE)m_hFile, NULL)) == (DWORD)-1L)
			return FALSE;

		if (m_strFileName.IsEmpty())
			rStatus.m_attribute = 0;
		else
		{
			DWORD dwAttribute = ::GetFileAttributes(m_strFileName);

			// don't return an error for this because previous versions of MFC didn't
			if (dwAttribute == 0xFFFFFFFF)
				rStatus.m_attribute = 0;
			else
			{
				rStatus.m_attribute = (BYTE) dwAttribute;
#ifdef _DEBUG
				// MFC BUG: m_attribute is only a BYTE wide
				if (dwAttribute & ~0xFF)
					TRACE0("Warning: CFile::GetStatus() returns m_attribute without high-order flags.\n");
#endif
			}
		}

		// convert times as appropriate
		rStatus.m_ctime = CTime(ftCreate);
		rStatus.m_atime = CTime(ftAccess);
		rStatus.m_mtime = CTime(ftModify);

		if (rStatus.m_ctime.GetTime() == 0)
			rStatus.m_ctime = rStatus.m_mtime;

		if (rStatus.m_atime.GetTime() == 0)
			rStatus.m_atime = rStatus.m_mtime;
	}
	return TRUE;
}

BOOL PASCAL CFile::GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus)
{
	// attempt to fully qualify path first
	if (!AfxFullPath(rStatus.m_szFullName, lpszFileName))
	{
		rStatus.m_szFullName[0] = '\0';
		return FALSE;
	}

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((LPTSTR)lpszFileName, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	VERIFY(FindClose(hFind));

	// strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
	rStatus.m_attribute = (BYTE)
		(findFileData.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL);

	// get just the low DWORD of the file size
	ASSERT(findFileData.nFileSizeHigh == 0);
	rStatus.m_size = (LONG)findFileData.nFileSizeLow;

	// convert times as appropriate
	rStatus.m_ctime = CTime(findFileData.ftCreationTime);
	rStatus.m_atime = CTime(findFileData.ftLastAccessTime);
	rStatus.m_mtime = CTime(findFileData.ftLastWriteTime);

	if (rStatus.m_ctime.GetTime() == 0)
		rStatus.m_ctime = rStatus.m_mtime;

	if (rStatus.m_atime.GetTime() == 0)
		rStatus.m_atime = rStatus.m_mtime;

	return TRUE;
}



static void PASCAL CFile::SetStatus(LPCTSTR lpszFileName, const CFileStatus& status)
{
  DWORD wAttr;
  FILETIME creationTime;
  FILETIME lastAccessTime;
  FILETIME lastWriteTime;
  LPFILETIME lpCreationTime = NULL;
  LPFILETIME lpLastAccessTime = NULL;
  LPFILETIME lpLastWriteTime = NULL;
  
  if ((wAttr = GetFileAttributes((LPTSTR)lpszFileName)) == (DWORD)-1L)
    FileError((LONG)GetLastError());
  
  if ((DWORD)status.m_attribute != wAttr && (wAttr & readOnly))
  {
    // Set file attribute, only if currently readonly.
    // This way we will be able to modify the time assuming the
    // caller changed the file from readonly.
    
    if (!SetFileAttributes((LPTSTR)lpszFileName, (DWORD)status.m_attribute))
      FileError((LONG)GetLastError());
  }
  
  // last modification time
  if (status.m_mtime.GetTime() != 0)
  {
    AfxTimeToFileTime(status.m_mtime, &lastWriteTime);
    lpLastWriteTime = &lastWriteTime;
    
    // last access time
    if (status.m_atime.GetTime() != 0)
    {
      AfxTimeToFileTime(status.m_atime, &lastAccessTime);
      lpLastAccessTime = &lastAccessTime;
    }
    
    // create time
    if (status.m_ctime.GetTime() != 0)
    {
      AfxTimeToFileTime(status.m_ctime, &creationTime);
      lpCreationTime = &creationTime;
    }
    
    HANDLE hFile = ::CreateFile(lpszFileName, GENERIC_READ|GENERIC_WRITE,
      FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
      NULL);
    
    if (hFile == INVALID_HANDLE_VALUE)
      FileError((LONG)::GetLastError());
    
    if (!SetFileTime((HANDLE)hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime))
      FileError((LONG)::GetLastError());
    
    if (!::CloseHandle(hFile))
      FileError((LONG)::GetLastError());
  }
  
  if ((DWORD)status.m_attribute != wAttr && !(wAttr & readOnly))
  {
    if (!SetFileAttributes((LPTSTR)lpszFileName, (DWORD)status.m_attribute))
      FileError((LONG)GetLastError());
  }
}

};

/////////////////////////////////////////////////////////////////////////////
// STDIO file implementation

class CStdioFile : public CFile
{
	DECLARE_DYNAMIC(CStdioFile)

public:
// Constructors
	CStdioFile();
	CStdioFile(FILE* pOpenStream);
	CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags);

// Attributes
	FILE* m_pStream;    // stdio FILE
						// m_hFile from base class is _fileno(m_pStream)

// Operations
	// reading and writing strings
	virtual void WriteString(LPCTSTR lpsz);
	virtual LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
	virtual BOOL ReadString(CString& rString);

// Implementation
public:
	virtual ~CStdioFile();
#ifdef _DEBUG
	void Dump(CDumpContext& dc) const;
#endif
	virtual DWORD GetPosition() const;
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags);
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual LONG Seek(LONG lOff, UINT nFrom);
	virtual void Abort();
	virtual void Flush();
	virtual void Close();

	// Unsupported APIs
	virtual CFile* Duplicate() const;
	virtual void LockRange(DWORD dwPos, DWORD dwCount);
	virtual void UnlockRange(DWORD dwPos, DWORD dwCount);

  
CStdioFile::CStdioFile()
{
	m_pStream = NULL;
}

CStdioFile::CStdioFile(FILE* pOpenStream) : CFile(hFileNull)
{
	m_pStream = pOpenStream;
	m_hFile = (UINT)_get_osfhandle(_fileno(pOpenStream));
	ASSERT(!m_bCloseOnDelete);
}

CStdioFile::CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	ASSERT(lpszFileName != NULL);
	ASSERT(AfxIsValidString(lpszFileName));

	CFileException e;
	if (!Open(lpszFileName, nOpenFlags, &e))
		AfxThrowFileException(e.m_cause, e.m_lOsError, e.m_strFileName);
}

CStdioFile::~CStdioFile()
{
	ASSERT_VALID(this);

	if (m_pStream != NULL && m_bCloseOnDelete)
		Close();
}

BOOL CStdioFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags,
	CFileException* pException)
{
	ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));
	ASSERT(lpszFileName != NULL);
	ASSERT(AfxIsValidString(lpszFileName));

	m_pStream = NULL;
	if (!CFile::Open(lpszFileName, (nOpenFlags & ~typeText), pException))
		return FALSE;

	ASSERT(m_hFile != hFileNull);
	ASSERT(m_bCloseOnDelete);

	char szMode[4]; // C-runtime open string
	int nMode = 0;

	// determine read/write mode depending on CFile mode
	if (nOpenFlags & modeCreate)
	{
		if (nOpenFlags & modeNoTruncate)
			szMode[nMode++] = 'a';
		else
			szMode[nMode++] = 'w';
	}
	else if (nOpenFlags & modeWrite)
		szMode[nMode++] = 'a';
	else
		szMode[nMode++] = 'r';

	// add '+' if necessary (when read/write modes mismatched)
	if (szMode[0] == 'r' && (nOpenFlags & modeReadWrite) ||
		szMode[0] != 'r' && !(nOpenFlags & modeWrite))
	{
		// current szMode mismatched, need to add '+' to fix
		szMode[nMode++] = '+';
	}

	// will be inverted if not necessary
	int nFlags = _O_RDONLY|_O_TEXT;
	if (nOpenFlags & (modeWrite|modeReadWrite))
		nFlags ^= _O_RDONLY;

	if (nOpenFlags & typeBinary)
		szMode[nMode++] = 'b', nFlags ^= _O_TEXT;
	else
		szMode[nMode++] = 't';
	szMode[nMode++] = '\0';

	// open a C-runtime low-level file handle
	int nHandle = _open_osfhandle(m_hFile, nFlags);

	// open a C-runtime stream from that handle
	if (nHandle != -1)
		m_pStream = _fdopen(nHandle, szMode);

	if (m_pStream == NULL)
	{
		// an error somewhere along the way...
		if (pException != NULL)
		{
			pException->m_lOsError = _doserrno;
			pException->m_cause = CFileException::OsErrorToException(_doserrno);
		}

		CFile::Abort(); // close m_hFile
		return FALSE;
	}

	return TRUE;
}

UINT CStdioFile::Read(void* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	if (nCount == 0)
		return 0;   // avoid Win32 "null-read"

	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	UINT nRead = 0;

	if ((nRead = fread(lpBuf, sizeof(BYTE), nCount, m_pStream)) == 0 && !feof(m_pStream))
		AfxThrowFileException(CFileException::generic, _doserrno, m_strFileName);
	if (ferror(m_pStream))
	{
		clearerr(m_pStream);
		AfxThrowFileException(CFileException::generic, _doserrno, m_strFileName);
	}
	return nRead;
}

void CStdioFile::Write(const void* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	if (fwrite(lpBuf, sizeof(BYTE), nCount, m_pStream) != nCount)
		AfxThrowFileException(CFileException::generic, _doserrno, m_strFileName);
}

void CStdioFile::WriteString(LPCTSTR lpsz)
{
	ASSERT(lpsz != NULL);
	ASSERT(m_pStream != NULL);

	if (_fputts(lpsz, m_pStream) == _TEOF)
		AfxThrowFileException(CFileException::diskFull, _doserrno, m_strFileName);
}

LPTSTR CStdioFile::ReadString(LPTSTR lpsz, UINT nMax)
{
	ASSERT(lpsz != NULL);
	ASSERT(AfxIsValidAddress(lpsz, nMax));
	ASSERT(m_pStream != NULL);

	LPTSTR lpszResult = _fgetts(lpsz, nMax, m_pStream);
	if (lpszResult == NULL && !feof(m_pStream))
	{
		clearerr(m_pStream);
		AfxThrowFileException(CFileException::generic, _doserrno, m_strFileName);
	}
	return lpszResult;
}

BOOL CStdioFile::ReadString(CString& rString)
{
	ASSERT_VALID(this);

	rString = &afxChNil;    // empty string without deallocating
	const int nMaxSize = 128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen = 0;
	for (;;)
	{
		lpszResult = _fgetts(lpsz, nMaxSize+1, m_pStream);
		rString.ReleaseBuffer();

		// handle error/eof case
		if (lpszResult == NULL && !feof(m_pStream))
		{
			clearerr(m_pStream);
			AfxThrowFileException(CFileException::generic, _doserrno,
				m_strFileName);
		}

		// if string is read completely or EOF
		if (lpszResult == NULL ||
			(nLen = lstrlen(lpsz)) < nMaxSize ||
			lpsz[nLen-1] == '\n')
			break;

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

LONG CStdioFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(nFrom == begin || nFrom == end || nFrom == current);
	ASSERT(m_pStream != NULL);

	if (fseek(m_pStream, lOff, nFrom) != 0)
		AfxThrowFileException(CFileException::badSeek, _doserrno,
			m_strFileName);

	long pos = ftell(m_pStream);
	return pos;
}

DWORD CStdioFile::GetPosition() const
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	long pos = ftell(m_pStream);
	if (pos == -1)
		AfxThrowFileException(CFileException::invalidFile, _doserrno,
			m_strFileName);
	return pos;
}

void CStdioFile::Flush()
{
	ASSERT_VALID(this);

	if (m_pStream != NULL && fflush(m_pStream) != 0)
		AfxThrowFileException(CFileException::diskFull, _doserrno,
			m_strFileName);
}

void CStdioFile::Close()
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	int nErr = 0;

	if (m_pStream != NULL)
		nErr = fclose(m_pStream);

	m_hFile = (UINT) hFileNull;
	m_bCloseOnDelete = FALSE;
	m_pStream = NULL;

	if (nErr != 0)
		AfxThrowFileException(CFileException::diskFull, _doserrno,
			m_strFileName);
}

void CStdioFile::Abort()
{
	ASSERT_VALID(this);

	if (m_pStream != NULL && m_bCloseOnDelete)
		fclose(m_pStream);  // close but ignore errors
	m_hFile = (UINT) hFileNull;
	m_pStream = NULL;
	m_bCloseOnDelete = FALSE;
}

CFile* CStdioFile::Duplicate() const
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	AfxThrowNotSupportedException();
	return NULL;
}

void CStdioFile::LockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	AfxThrowNotSupportedException();
}

void CStdioFile::UnlockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	ASSERT(m_pStream != NULL);

	AfxThrowNotSupportedException();
}

#ifdef _DEBUG
void CStdioFile::Dump(CDumpContext& dc) const
{
	CFile::Dump(dc);

	dc << "m_pStream = " << (void*)m_pStream;
	dc << "\n";
}
#endif



};

////////////////////////////////////////////////////////////////////////////
// Memory based file implementation

class CMemFile : public CFile
{
	DECLARE_DYNAMIC(CMemFile)

public:
// Constructors
	CMemFile(UINT nGrowBytes = 1024);
	CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);

// Operations
	void Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);
	BYTE* Detach();

// Advanced Overridables
protected:
	virtual BYTE* Alloc(DWORD nBytes);
	virtual BYTE* Realloc(BYTE* lpMem, DWORD nBytes);
	virtual BYTE* Memcpy(BYTE* lpMemTarget, const BYTE* lpMemSource, UINT nBytes);
	virtual void Free(BYTE* lpMem);
	virtual void GrowFile(DWORD dwNewLen);

// Implementation
protected:
	UINT m_nGrowBytes;
	DWORD m_nPosition;
	DWORD m_nBufferSize;
	DWORD m_nFileSize;
	BYTE* m_lpBuffer;
	BOOL m_bAutoDelete;

public:
	virtual ~CMemFile();
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif
	virtual DWORD GetPosition() const;
	BOOL GetStatus(CFileStatus& rStatus) const;
	virtual LONG Seek(LONG lOff, UINT nFrom);
	virtual void SetLength(DWORD dwNewLen);
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual void Abort();
	virtual void Flush();
	virtual void Close();
	virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
		void** ppBufStart = NULL, void** ppBufMax = NULL);

	// Unsupported APIs
	virtual CFile* Duplicate() const;
	virtual void LockRange(DWORD dwPos, DWORD dwCount);
	virtual void UnlockRange(DWORD dwPos, DWORD dwCount);

  
CMemFile::CMemFile(UINT nGrowBytes)
{
	ASSERT(nGrowBytes <= UINT_MAX);

	m_nGrowBytes = nGrowBytes;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
	m_lpBuffer = NULL;
	m_bAutoDelete = TRUE;
}

CMemFile::CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes)
{
	ASSERT(nGrowBytes <= UINT_MAX);

	m_nGrowBytes = nGrowBytes;
	m_nPosition = 0;
	m_nBufferSize = nBufferSize;
	m_nFileSize = nGrowBytes == 0 ? nBufferSize : 0;
	m_lpBuffer = lpBuffer;
	m_bAutoDelete = FALSE;
}

void CMemFile::Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes)
{
	ASSERT(m_lpBuffer == NULL);

	m_nGrowBytes = nGrowBytes;
	m_nPosition = 0;
	m_nBufferSize = nBufferSize;
	m_nFileSize = nGrowBytes == 0 ? nBufferSize : 0;
	m_lpBuffer = lpBuffer;
	m_bAutoDelete = FALSE;
}

BYTE* CMemFile::Detach()
{
	BYTE* lpBuffer = m_lpBuffer;
	m_lpBuffer = NULL;
	m_nFileSize = 0;
	m_nBufferSize = 0;
	m_nPosition = 0;

	return lpBuffer;
}

CMemFile::~CMemFile()
{
	// Close should have already been called, but we check anyway
	if (m_lpBuffer)
		Close();
	ASSERT(m_lpBuffer == NULL);

	m_nGrowBytes = 0;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
}

BYTE* CMemFile::Alloc(DWORD nBytes)
{
	return (BYTE*)malloc((UINT)nBytes);
}

BYTE* CMemFile::Realloc(BYTE* lpMem, DWORD nBytes)
{
	return (BYTE*)realloc(lpMem, (UINT)nBytes);
}

#pragma intrinsic(memcpy)
BYTE* CMemFile::Memcpy(BYTE* lpMemTarget, const BYTE* lpMemSource,
	UINT nBytes)
{
	ASSERT(lpMemTarget != NULL);
	ASSERT(lpMemSource != NULL);

	ASSERT(AfxIsValidAddress(lpMemTarget, nBytes));
	ASSERT(AfxIsValidAddress(lpMemSource, nBytes, FALSE));

	return (BYTE*)memcpy(lpMemTarget, lpMemSource, nBytes);
}
#pragma function(memcpy)

void CMemFile::Free(BYTE* lpMem)
{
	ASSERT(lpMem != NULL);

	free(lpMem);
}

DWORD CMemFile::GetPosition() const
{
	ASSERT_VALID(this);
	return m_nPosition;
}

void CMemFile::GrowFile(DWORD dwNewLen)
{
	ASSERT_VALID(this);

	if (dwNewLen > m_nBufferSize)
	{
		// grow the buffer
		DWORD dwNewBufferSize = (DWORD)m_nBufferSize;

		// watch out for buffers which cannot be grown!
		ASSERT(m_nGrowBytes != 0);
		if (m_nGrowBytes == 0)
			AfxThrowMemoryException();

		// determine new buffer size
		while (dwNewBufferSize < dwNewLen)
			dwNewBufferSize += m_nGrowBytes;

		// allocate new buffer
		BYTE* lpNew;
		if (m_lpBuffer == NULL)
			lpNew = Alloc(dwNewBufferSize);
		else
			lpNew = Realloc(m_lpBuffer, dwNewBufferSize);

		if (lpNew == NULL)
			AfxThrowMemoryException();

		m_lpBuffer = lpNew;
		m_nBufferSize = dwNewBufferSize;
	}
	ASSERT_VALID(this);
}

void CMemFile::SetLength(DWORD dwNewLen)
{
	ASSERT_VALID(this);

	if (dwNewLen > m_nBufferSize)
		GrowFile(dwNewLen);

	if (dwNewLen < m_nPosition)
		m_nPosition = dwNewLen;

	m_nFileSize = dwNewLen;
	ASSERT_VALID(this);
}

UINT CMemFile::Read(void* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);

	if (nCount == 0)
		return 0;

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	if (m_nPosition > m_nFileSize)
		return 0;

	UINT nRead;
	if (m_nPosition + nCount > m_nFileSize)
		nRead = (UINT)(m_nFileSize - m_nPosition);
	else
		nRead = nCount;

	Memcpy((BYTE*)lpBuf, (BYTE*)m_lpBuffer + m_nPosition, nRead);
	m_nPosition += nRead;

	ASSERT_VALID(this);

	return nRead;
}

void CMemFile::Write(const void* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);

	if (nCount == 0)
		return;

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	if (m_nPosition + nCount > m_nBufferSize)
		GrowFile(m_nPosition + nCount);

	ASSERT(m_nPosition + nCount <= m_nBufferSize);

	Memcpy((BYTE*)m_lpBuffer + m_nPosition, (BYTE*)lpBuf, nCount);

	m_nPosition += nCount;

	if (m_nPosition > m_nFileSize)
		m_nFileSize = m_nPosition;

	ASSERT_VALID(this);
}

LONG CMemFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT(nFrom == begin || nFrom == end || nFrom == current);

	LONG lNewPos = m_nPosition;

	if (nFrom == begin)
		lNewPos = lOff;
	else if (nFrom == current)
		lNewPos += lOff;
	else if (nFrom == end)
		lNewPos = m_nFileSize + lOff;
	else
		return -1;

	if (lNewPos < 0)
		AfxThrowFileException(CFileException::badSeek);

	m_nPosition = lNewPos;

	ASSERT_VALID(this);
	return m_nPosition;
}

void CMemFile::Flush()
{
	ASSERT_VALID(this);
}

void CMemFile::Close()
{
	ASSERT((m_lpBuffer == NULL && m_nBufferSize == 0) ||
		!m_bAutoDelete || AfxIsValidAddress(m_lpBuffer, (UINT)m_nBufferSize, FALSE));
	ASSERT(m_nFileSize <= m_nBufferSize);

	m_nGrowBytes = 0;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
	if (m_lpBuffer && m_bAutoDelete)
		Free(m_lpBuffer);
	m_lpBuffer = NULL;
}

void CMemFile::Abort()
{
	ASSERT_VALID(this);

	Close();
}

void CMemFile::LockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	AfxThrowNotSupportedException();
}


void CMemFile::UnlockRange(DWORD /* dwPos */, DWORD /* dwCount */)
{
	ASSERT_VALID(this);
	AfxThrowNotSupportedException();
}

CFile* CMemFile::Duplicate() const
{
	ASSERT_VALID(this);
	AfxThrowNotSupportedException();
	return NULL;
}

// only CMemFile supports "direct buffering" interaction with CArchive
UINT CMemFile::GetBufferPtr(UINT nCommand, UINT nCount = 0,
	void** ppBufStart = NULL, void**ppBufMax = NULL)
{
	ASSERT(nCommand == bufferCheck || nCommand == bufferCommit ||
		nCommand == bufferRead || nCommand == bufferWrite);

	if (nCommand == bufferCheck)
		return 1;   // just a check for direct buffer support

	if (nCommand == bufferCommit)
	{
		// commit buffer
		ASSERT(ppBufStart == NULL);
		ASSERT(ppBufMax == NULL);
		m_nPosition += nCount;
		if (m_nPosition > m_nFileSize)
			m_nFileSize = m_nPosition;
		return 0;
	}

	ASSERT(nCommand == bufferWrite || nCommand == bufferRead);
	ASSERT(ppBufStart != NULL);
	ASSERT(ppBufMax != NULL);

	// when storing, grow file as necessary to satisfy buffer request
	if (nCommand == bufferWrite && m_nPosition + nCount > m_nBufferSize)
		GrowFile(m_nPosition + nCount);

	// store buffer max and min
	*ppBufStart = m_lpBuffer + m_nPosition;

	// end of buffer depends on whether you are reading or writing
	if (nCommand == bufferWrite)
		*ppBufMax = m_lpBuffer + min(m_nBufferSize, m_nPosition + nCount);
	else
	{
		if (nCount == (UINT)-1)
			nCount = m_nBufferSize - m_nPosition;
		*ppBufMax = m_lpBuffer + min(m_nFileSize, m_nPosition + nCount);
		m_nPosition += LPBYTE(*ppBufMax) - LPBYTE(*ppBufStart);
	}

	// return number of bytes in returned buffer space (may be <= nCount)
	return LPBYTE(*ppBufMax) - LPBYTE(*ppBufStart);
}

/////////////////////////////////////////////////////////////////////////////
// CMemFile diagonstics

#ifdef _DEBUG
void CMemFile::Dump(CDumpContext& dc) const
{
	CFile::Dump(dc);

	dc << "m_nFileSize = " << m_nFileSize;
	dc << "\nm_nBufferSize = " << m_nBufferSize;
	dc << "\nm_nPosition = " << m_nPosition;
	dc << "\nm_nGrowBytes = " << m_nGrowBytes;

	dc << "\n";
}

void CMemFile::AssertValid() const
{
	CFile::AssertValid();

	ASSERT((m_lpBuffer == NULL && m_nBufferSize == 0) ||
		AfxIsValidAddress(m_lpBuffer, (UINT)m_nBufferSize, FALSE));
	ASSERT(m_nFileSize <= m_nBufferSize);
	// m_nPosition might be after the end of file, so we cannot ASSERT
	// its validity
}
#endif // _DEBUG


BOOL CMemFile::GetStatus(CFileStatus& rStatus) const
{
  ASSERT_VALID(this);
  
  rStatus.m_ctime = 0;
  rStatus.m_mtime = 0;
  rStatus.m_atime = 0;
  rStatus.m_size = m_nFileSize;
  rStatus.m_attribute = normal;
  rStatus.m_szFullName[0] = '\0';
  return TRUE;
}

};

#endif // __FILE_HPP__
