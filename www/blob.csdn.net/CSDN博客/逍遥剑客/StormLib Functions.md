# StormLib Functions - 逍遥剑客 - CSDN博客
2008年12月24日 23:54:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3799
记录一下, 方便查阅
官方链接: [The StormLib library](http://www.zezula.net/en/mpq/stormlib.html)
//----------------------------------------------------------------------------- 
// Functions in StormLib - compatible with Storm.dll 
// Typedefs for functions exported by Storm.dll 
typedef LCID  (WINAPI * SFILESETLOCALE)(LCID);
typedef BOOL  (WINAPI * SFILEOPENARCHIVE)(const char *, DWORD, DWORD, HANDLE *);
typedef BOOL  (WINAPI * SFILECLOSEARCHIVE)(HANDLE);
typedef BOOL  (WINAPI * SFILEOPENFILEEX)(HANDLE, const char *, DWORD, HANDLE *);
typedef BOOL  (WINAPI * SFILECLOSEFILE)(HANDLE);
typedef DWORD (WINAPI * SFILEGETFILESIZE)(HANDLE, DWORD *);
typedef DWORD (WINAPI * SFILESETFILEPOINTER)(HANDLE, LONG, LONG *, DWORD);
typedef BOOL  (WINAPI * SFILEREADFILE)(HANDLE, VOID *, DWORD, DWORD *, LPOVERLAPPED);
// Archive opening/closing 
LCID  WINAPI SFileSetLocale(LCID lcNewLocale);
LCID  WINAPI SFileGetLocale();
BOOL  WINAPI SFileOpenArchive(const char * szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE * phMpq);
BOOL  WINAPI SFileFlushArchive(HANDLE hMpq);
BOOL  WINAPI SFileCloseArchive(HANDLE hMpq);
// File opening/closing 
BOOL  WINAPI SFileOpenFileEx(HANDLE hMpq, const char * szFileName, DWORD dwSearchScope, HANDLE * phFile);
BOOL  WINAPI SFileCloseFile(HANDLE hFile);
// File I/O 
DWORD WINAPI SFileGetFilePos(HANDLE hFile, DWORD * pdwFilePosHigh = NULL);
DWORD WINAPI SFileGetFileSize(HANDLE hFile, DWORD * pdwFileSizeHigh = NULL);
DWORD WINAPI SFileSetFilePointer(HANDLE hFile, LONG lFilePos, LONG * pdwFilePosHigh, DWORD dwMethod);
BOOL  WINAPI SFileReadFile(HANDLE hFile, VOID * lpBuffer, DWORD dwToRead, DWORD * pdwRead = NULL, LPOVERLAPPED lpOverlapped = NULL);
// Adds another listfile into MPQ. The currently added listfile(s) remain, 
// so you can use this API to combining more listfiles. 
// Note that this function is internally called by SFileFindFirstFile 
int   WINAPI SFileAddListFile(HANDLE hMpq, const char * szListFile);
//----------------------------------------------------------------------------- 
// Functions in StormLib - not implemented in Storm.dll 
// Archive creating and editing 
BOOL  WINAPI SFileCreateArchiveEx(const char * szMpqName, DWORD dwCreationDisposition, DWORD dwHashTableSize, HANDLE * phMpq);
BOOL  WINAPI SFileAddFile(HANDLE hMpq, const char * szFileName, const char * szArchivedName, DWORD dwFlags); 
BOOL  WINAPI SFileAddWave(HANDLE hMpq, const char * szFileName, const char * szArchivedName, DWORD dwFlags, DWORD dwQuality); 
BOOL  WINAPI SFileRemoveFile(HANDLE hMpq, const char * szFileName, DWORD dwSearchScope = SFILE_OPEN_BY_INDEX);
BOOL  WINAPI SFileRenameFile(HANDLE hMpq, const char * szOldFileName, const char * szNewFileName);
BOOL  WINAPI SFileSetFileLocale(HANDLE hFile, LCID lcNewLocale);
// Retrieving info about the file 
BOOL  WINAPI SFileHasFile(HANDLE hMpq, char * szFileName);
BOOL  WINAPI SFileGetFileName(HANDLE hFile, char * szFileName);
DWORD_PTR WINAPI SFileGetFileInfo(HANDLE hMpqOrFile, DWORD dwInfoType);
// File search 
// Note that the SFileFindFirstFileEx has been removed. Use SListFileFindFirst/Next 
HANDLE WINAPI SFileFindFirstFile(HANDLE hMpq, const char * szMask, SFILE_FIND_DATA * lpFindFileData, const char * szListFile);
BOOL   WINAPI SFileFindNextFile(HANDLE hFind, SFILE_FIND_DATA * lpFindFileData);
BOOL   WINAPI SFileFindClose(HANDLE hFind);
// Listfile search 
HANDLE SListFileFindFirstFile(HANDLE hMpq, const char * szListFile, const char * szMask, SFILE_FIND_DATA * lpFindFileData);
BOOL   SListFileFindNextFile(HANDLE hFind, SFILE_FIND_DATA * lpFindFileData);
BOOL   SListFileFindClose(HANDLE hFind);
// Archive compacting 
typedef void  (WINAPI * COMPACTCB)(void * lpUserData, DWORD dwWorkType, DWORD dwParam1, DWORD dwParam2);
BOOL  WINAPI SFileSetCompactCallback(HANDLE hMpq, COMPACTCB CompactCB, void * lpData);
BOOL  WINAPI SFileCompactArchive(HANDLE hMpq, const char * szListFile = NULL, BOOL bReserved = 0);
// Locale support 
int   WINAPI SFileEnumLocales(HANDLE hMpq, const char * szFileName, LCID * plcLocales, DWORD * pdwMaxLocales, DWORD dwSearchScope = SFILE_OPEN_BY_INDEX);
// (De)compression 
int WINAPI SCompCompress   (char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength, int uCompressions, int nCmpType, int nCmpLevel);
int WINAPI SCompDecompress (char * pbOutBuffer, int * pdwOutLength, char * pbInBuffer, int dwInLength);
// Sets the default data compression for files added to MPQ, 
// if MPQ_FILE_COMPRESS_MULTI has been specified in call to SFileAddFile 
// Use one of the MPQ_COMPRESSION_XXX values 
int WINAPI SCompSetDataCompression(int nDataCompression);
// Verifies file against its extended attributes (depending on dwFlags). 
// For dwFlags, use one or more of MPQ_ATTRIBUTE_MD5 
BOOL  WINAPI SFileVerifyFile(HANDLE hMpq, const char * szFileName, DWORD dwFlags);
// High-level extract function 
BOOL  WINAPI SFileExtractFile(HANDLE hMpq, const char * szToExtract, const char * szExtracted);
