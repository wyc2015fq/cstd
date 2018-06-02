/*
Font Name Library 1.0.0
Copyright (c) 2009 Wong Shao Voon
The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/
#ifndef _TTF_H_
#define _TTF_H_
//#include "debug.h"
#ifndef _WIN32_WINNT  // Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

//#include "publicfunc.h"
#include <tchar.h>
#include <windows.h>
#include <wingdi.h>
/////////////
typedef struct MemPtrReader {
  //! Original pointer
  BYTE* m_pOrigin;
  //! Current pointer
  BYTE* m_ptr;
  //! Number of steps to move
  size_t m_pSteps;
  //! Size of the buffer
  size_t m_size;
  //! Offset of the m_pOrigin from the starting byte
  size_t m_nOffset;
}
MemPtrReader;
//! Get the current buffer ptr.
BYTE* GetPtr(MemPtrReader* This)
{
  return This->m_ptr;
}
//! Get the size of the buffer
size_t MemPtrReader_Size(MemPtrReader* This)
{
  return This->m_size;
};
void MemPtrReader_Init(MemPtrReader* This, BYTE* pOrigin, size_t size, size_t offset)
{
  This->m_pOrigin = pOrigin;
  This->m_ptr = pOrigin;
  This->m_pSteps = 0;
  This->m_size = size;
  This->m_nOffset = offset;
}
int Seek(MemPtrReader* This, long offset, int origin)
{
  if (origin == SEEK_SET) {
    This->m_ptr = This->m_pOrigin - This->m_nOffset;
    This->m_ptr = This->m_ptr + offset;
    return 0;
  }
  else if (origin == SEEK_END) {
    This->m_ptr = This->m_pOrigin - This->m_nOffset + This->m_size;
    This->m_ptr = This->m_ptr + offset;
    return 0;
  }
  else if (origin == SEEK_CUR) {
    This->m_ptr = This->m_ptr + offset;
    return 0;
  }

  return 1;
}
size_t Read(MemPtrReader* This, void* buffer, size_t size, size_t count)
{
  BYTE* buffer2 = (BYTE*)(buffer);

  for (size_t i = 0; i < count; ++i) {
    memcpy(buffer2, This->m_ptr, size);
    buffer2 = buffer2 + size;
    This->m_ptr = This->m_ptr + size;
  }

  return size * count;
}
union unionUshort {
  USHORT usNum;
  BYTE bytes[ 2 ];
};
union unionShort {
  SHORT nNum;
  BYTE bytes[ 2 ];
};
union unionUint {
  UINT nNum;
  BYTE bytes[ 4 ];
};
union unionInt {
  INT nNum;
  BYTE bytes[ 4 ];
};
union unionUlong {
  ULONG nNum;
  BYTE bytes[ 4 ];
};
union unionLong {
  LONG nNum;
  BYTE bytes[ 4 ];
};
union unionString {
  CHAR charr[ 1024 ];
  WCHAR wcharr[ 512 ];
};
void Switch(USHORT& SrcDest)
{
  unionUshort num;
  num.usNum = SrcDest;
  BYTE temp = num.bytes[ 0 ];
  num.bytes[ 0 ] = num.bytes[ 1 ];
  num.bytes[ 1 ] = temp;
  SrcDest = num.usNum;
}
void Switch(SHORT& SrcDest)
{
  unionShort num;
  num.nNum = SrcDest;
  BYTE temp = num.bytes[ 0 ];
  num.bytes[ 0 ] = num.bytes[ 1 ];
  num.bytes[ 1 ] = temp;
  SrcDest = num.nNum;
}
void Switch(UINT& SrcDest)
{
  unionUint num;
  num.nNum = SrcDest;
  BYTE temp = num.bytes[ 0 ];
  num.bytes[ 0 ] = num.bytes[ 3 ];
  num.bytes[ 3 ] = temp;
  temp = num.bytes[ 1 ];
  num.bytes[ 1 ] = num.bytes[ 2 ];
  num.bytes[ 2 ] = temp;
  SrcDest = num.nNum;
}
void Switch(INT& SrcDest)
{
  unionInt num;
  num.nNum = SrcDest;
  BYTE temp = num.bytes[ 0 ];
  num.bytes[ 0 ] = num.bytes[ 3 ];
  num.bytes[ 3 ] = temp;
  temp = num.bytes[ 1 ];
  num.bytes[ 1 ] = num.bytes[ 2 ];
  num.bytes[ 2 ] = temp;
  SrcDest = num.nNum;
}
void Switch(ULONG& SrcDest)
{
  unionUlong num;
  num.nNum = SrcDest;
  BYTE temp = num.bytes[ 0 ];
  num.bytes[ 0 ] = num.bytes[ 3 ];
  num.bytes[ 3 ] = temp;
  temp = num.bytes[ 1 ];
  num.bytes[ 1 ] = num.bytes[ 2 ];
  num.bytes[ 2 ] = temp;
  SrcDest = num.nNum;
}
void Switch(LONG& SrcDest)
{
  unionLong num;
  num.nNum = SrcDest;
  BYTE temp = num.bytes[ 0 ];
  num.bytes[ 0 ] = num.bytes[ 3 ];
  num.bytes[ 3 ] = temp;
  temp = num.bytes[ 1 ];
  num.bytes[ 1 ] = num.bytes[ 2 ];
  num.bytes[ 2 ] = temp;
  SrcDest = num.nNum;
}
void Switch(WCHAR* SrcDest, size_t Size)
{
  for (size_t i = 0; i < Size; ++i) {
    Switch((USHORT&) SrcDest[ i ]);
  }
}
///////////////////////////////////////////////////////////////////////////////
typedef struct stOffsetTable {
  UINT version;
  USHORT numTables;
  USHORT searchRange;
  USHORT entrySelector;
  USHORT rangeShift;
} stOffsetTable;
typedef struct stTableDirectory {
  CHAR tag[ 4 ];
  ULONG checkSum;
  ULONG offset;
  ULONG length;
} stTableDirectory;
typedef struct stNamingTable {
  USHORT FormatSelector;
  USHORT NumberOfNameRecords;
  USHORT OffsetStartOfStringStorage;
} stNamingTable;
typedef struct stNameRecord {
  USHORT PlatformID;
  USHORT EncodingID;
  USHORT LanguageID;
  USHORT NameID;
  USHORT StringLength;
  USHORT OffsetFromStorageArea;
} stNameRecord;
typedef struct TTF {
  TCHAR m_szFontName[ _MAX_PATH ];  //! Font name
  TCHAR m_szCopyright[ _MAX_PATH ];  //! Copyright
  TCHAR m_szFontFamilyName[ _MAX_PATH ];  //! Font family name
  TCHAR m_szFontSubFamilyName[ _MAX_PATH ];  //! States whether the font is bold, italics or regular
  TCHAR m_szFontID[ _MAX_PATH ];  //! Font ID
  TCHAR m_szVersion[ _MAX_PATH ];  //! Font version text
  TCHAR m_szPostScriptName[ _MAX_PATH ];  //! Font Postscript name
  TCHAR m_szTrademark[ _MAX_PATH ];  //! Trademark
  BYTE m_nInfo;  //! Masked Flags to indicate if the vital information is filled
  BOOL m_bBold;  //! IsBold?
  BOOL m_bRegular;  //! IsRegular?
  BOOL m_bItalic;  //! IsItalic?
}
TTF;

TCHAR* ToLower(const TCHAR* szNormal, TCHAR* szLower)
{
  for (int i = 0; i < (int) _tcslen(szNormal) ; ++i) {
    szLower[ i ] = (char)(towlower(szNormal[ i ]));
  }

  return szLower;
}

void Init(TTF* This)
{
  This->m_nInfo = 0;
  This->m_bBold = false;
  This->m_bRegular = false;
  This->m_bItalic = false;
}

void ParseStyles(TTF* This)
{
  TCHAR buf[ _MAX_PATH ];
  This->m_bBold = false;

  if (_tcsstr(ToLower(This->m_szFontSubFamilyName, buf), "bold") != NULL) {
    This->m_bBold = true;
  }

  This->m_bItalic = false;

  if (_tcsstr(ToLower(This->m_szFontSubFamilyName, buf), "italic") != NULL) {
    This->m_bItalic = true;
  }

  This->m_bRegular = false;

  if (_tcsstr(ToLower(This->m_szFontSubFamilyName, buf), "regular") != NULL) {
    This->m_bRegular = true;
  }
  else if (_tcsstr(ToLower(This->m_szFontSubFamilyName, buf), "normal") != NULL) {
    This->m_bRegular = true;
  }
  else if (_tcsstr(ToLower(This->m_szFontSubFamilyName, buf), "standard") != NULL) {
    This->m_bRegular = true;
  }
}

BOOL IsBold(TTF* This)
{
  return This->m_bBold;
}

BOOL IsItalic(TTF* This)
{
  return This->m_bItalic;
}

BOOL IsRegular(TTF* This)
{
  return This->m_bRegular;
}

void SetNameIDValue(TTF* This, USHORT uNameID, const CHAR* str)
{
  switch (uNameID) {
  case 0:
    if (!(This->m_nInfo & (1 << 0))) {
      strcpy(This->m_szCopyright, str);
    }

    This->m_nInfo |= (1 << 0);
    break;

  case 1:
    if (!(This->m_nInfo & (1 << 1))) {
      strcpy(This->m_szFontFamilyName, str);
    }

    This->m_nInfo |= (1 << 1);
    break;

  case 2:
    if (!(This->m_nInfo & (1 << 2))) {
      strcpy(This->m_szFontSubFamilyName, str);
      ParseStyles(This);
    }

    This->m_nInfo |= (1 << 2);
    break;

  case 3:
    if (!(This->m_nInfo & (1 << 3))) {
      strcpy(This->m_szFontID, str);
    }

    This->m_nInfo |= (1 << 3);
    break;

  case 4:
    if (!(This->m_nInfo & (1 << 4))) {
      strcpy(This->m_szFontName, str);
    }

    This->m_nInfo |= (1 << 4);
    break;

  case 5:
    if (!(This->m_nInfo & (1 << 5))) {
      strcpy(This->m_szVersion, str);
    }

    This->m_nInfo |= (1 << 5);
    break;

  case 6:
    if (!(This->m_nInfo & (1 << 6))) {
      strcpy(This->m_szPostScriptName, str);
    }

    This->m_nInfo |= (1 << 6);
    break;

  case 7:
    if (!(This->m_nInfo & (1 << 7))) {
      strcpy(This->m_szTrademark, str);
    }

    This->m_nInfo |= (1 << 7);
    break;
  }
}
#define WideToNarrow(a, b, c)
void SetNameIDValue(TTF* This, USHORT uNameID, const WCHAR* str)
{
  int l = (wcslen(str) + 1);
  CHAR* s = (CHAR*) malloc(l * sizeof(CHAR));
  WideToNarrow(str, s, l);
  SetNameIDValue(This, uNameID, s);
  free(s);
}
BOOL ParseName(TTF* This, FILE* m_pFile, stTableDirectory& TableDirectory)
{
  Switch(TableDirectory.offset);
  fseek(m_pFile, TableDirectory.offset, SEEK_SET);
  stNamingTable NamingTable;
  memset(&NamingTable, 0, sizeof(NamingTable));
  fread(&NamingTable, sizeof(NamingTable), 1, m_pFile);
  Switch(NamingTable.NumberOfNameRecords);
  Switch(NamingTable.OffsetStartOfStringStorage);
  Switch(NamingTable.FormatSelector);
  stNameRecord* vec = (stNameRecord*) malloc(NamingTable.NumberOfNameRecords * sizeof(stNameRecord));

  for (USHORT i = 0; i < NamingTable.NumberOfNameRecords; ++i) {
    stNameRecord NameRecord;
    memset(&NameRecord, 0, sizeof(NameRecord));
    fread(&NameRecord, sizeof(NameRecord), 1, m_pFile);
    Switch(NameRecord.NameID);
    Switch(NameRecord.EncodingID);
    Switch(NameRecord.OffsetFromStorageArea);
    Switch(NameRecord.StringLength);
    Switch(NameRecord.PlatformID);
    Switch(NameRecord.LanguageID);
    vec[ i ] = (NameRecord);
  }

  for (int a = 0; a < NamingTable.NumberOfNameRecords; ++a) {
    if (vec[ a ].PlatformID == 0 || vec[ a ].PlatformID == 3) {
      fseek(m_pFile, TableDirectory.offset + NamingTable.OffsetStartOfStringStorage + vec[ a ].OffsetFromStorageArea, SEEK_SET);
      int size = vec[ a ].StringLength / 2 + 1;
      WCHAR* pwBuf = (WCHAR*) malloc(sizeof(WCHAR) * size);
      memset(pwBuf, 0, size * sizeof(WCHAR));
      fread(pwBuf, vec[ a ].StringLength, 1, m_pFile);
      Switch(pwBuf, size);
      SetNameIDValue(This, vec[ a ].NameID, pwBuf);
      memset(pwBuf, 0, size * sizeof(WCHAR));
      free(pwBuf);
      pwBuf = NULL;

      if (This->m_nInfo == 0xFF) {
        break;
      }
    }
    else {
      fseek(m_pFile, TableDirectory.offset + NamingTable.OffsetStartOfStringStorage + vec[ a ].OffsetFromStorageArea, SEEK_SET);
      int size = vec[ a ].StringLength + 1;
      CHAR* pwBuf = (CHAR*) malloc(sizeof(CHAR) * size);
      memset(pwBuf, 0, size * sizeof(CHAR));
      fread(pwBuf, vec[ a ].StringLength, 1, m_pFile);
      SetNameIDValue(This, vec[ a ].NameID, pwBuf);
      memset(pwBuf, 0, size * sizeof(CHAR));
      free(pwBuf);
      pwBuf = NULL;

      if (This->m_nInfo == 0xFF) {
        break;
      }
    }
  }

  free(vec);
  return true;
}

BOOL ParseNameMem(TTF* This, MemPtrReader* m_pMemPtrReader, stTableDirectory& TableDirectory)
{
  Switch(TableDirectory.offset);
  Seek(m_pMemPtrReader, TableDirectory.offset, SEEK_SET);
  stNamingTable NamingTable;
  memset(&NamingTable, 0, sizeof(NamingTable));
  Read(m_pMemPtrReader, &NamingTable, sizeof(NamingTable), 1);
  Switch(NamingTable.NumberOfNameRecords);
  Switch(NamingTable.OffsetStartOfStringStorage);
  Switch(NamingTable.FormatSelector);
  stNameRecord* vec = (stNameRecord*) malloc(NamingTable.NumberOfNameRecords * sizeof(stNameRecord));

  for (USHORT i = 0; i < NamingTable.NumberOfNameRecords; ++i) {
    stNameRecord NameRecord;
    memset(&NameRecord, 0, sizeof(NameRecord));
    Read(m_pMemPtrReader, &NameRecord, sizeof(NameRecord), 1);
    Switch(NameRecord.NameID);
    Switch(NameRecord.EncodingID);
    Switch(NameRecord.OffsetFromStorageArea);
    Switch(NameRecord.StringLength);
    Switch(NameRecord.PlatformID);
    Switch(NameRecord.LanguageID);
    vec[ i ] = (NameRecord);
  }

  for (int a = 0; a < NamingTable.NumberOfNameRecords; ++a) {
    if (vec[ a ].PlatformID == 0 || vec[ a ].PlatformID == 3) {
      Seek(m_pMemPtrReader, TableDirectory.offset + NamingTable.OffsetStartOfStringStorage + vec[ a ].OffsetFromStorageArea, SEEK_SET);
      int size = vec[ a ].StringLength / 2 + 1;
      WCHAR* pwBuf = (WCHAR*) malloc(sizeof(WCHAR) * size);
      memset(pwBuf, 0, size * sizeof(WCHAR));
      Read(m_pMemPtrReader, pwBuf, vec[ a ].StringLength, 1);
      Switch(pwBuf, size);
      SetNameIDValue(This, vec[ a ].NameID, pwBuf);
      memset(pwBuf, 0, size * sizeof(WCHAR));
      free(pwBuf);
      pwBuf = NULL;

      if (This->m_nInfo == 0xFF) {
        break;
      }
    }
    else {
      Seek(m_pMemPtrReader, TableDirectory.offset + NamingTable.OffsetStartOfStringStorage + vec[ a ].OffsetFromStorageArea, SEEK_SET);
      int size = vec[ a ].StringLength + 1;
      CHAR* pwBuf = (CHAR*) malloc(sizeof(CHAR) * size);
      memset(pwBuf, 0, size * sizeof(CHAR));
      Read(m_pMemPtrReader, pwBuf, vec[ a ].StringLength, 1);
      SetNameIDValue(This, vec[ a ].NameID, pwBuf);
      memset(pwBuf, 0, size * sizeof(CHAR));
      free(pwBuf);
      pwBuf = NULL;

      if (This->m_nInfo == 0xFF) {
        break;
      }
    }
  }

  free(vec);
  return true;
}

BOOL ParseOffsetTable(TTF* This, FILE* m_pFile)
{
  stOffsetTable OffsetTable;
  memset(&OffsetTable, 0, sizeof(OffsetTable));
  fread(&OffsetTable, sizeof(OffsetTable), 1, m_pFile);
  Switch(OffsetTable.numTables);

  for (USHORT i = 0; i < OffsetTable.numTables; ++i) {
    stTableDirectory TableDirectory;
    memset(&TableDirectory, 0, sizeof(TableDirectory));
    fread(&TableDirectory, sizeof(TableDirectory), 1, m_pFile);
    TCHAR strTag[ 8 ] = "";
    strTag[ 0 ] = TableDirectory.tag[ 0 ];
    strTag[ 1 ] = TableDirectory.tag[ 1 ];
    strTag[ 2 ] = TableDirectory.tag[ 2 ];
    strTag[ 3 ] = TableDirectory.tag[ 3 ];
    strTag[ 4 ] = 0;

    if (0 == stricmp(strTag, "name")) {
      return ParseName(This, m_pFile, TableDirectory);
    }
  }

  return false;
}

BOOL ParseOffsetTableMem(TTF* This, MemPtrReader* m_pMemPtrReader)
{
  stOffsetTable OffsetTable;
  memset(&OffsetTable, 0, sizeof(OffsetTable));
  Read(m_pMemPtrReader, &OffsetTable, sizeof(OffsetTable), 1);
  Switch(OffsetTable.numTables);

  for (USHORT i = 0; i < OffsetTable.numTables; ++i) {
    stTableDirectory TableDirectory;
    memset(&TableDirectory, 0, sizeof(TableDirectory));
    Read(m_pMemPtrReader, &TableDirectory, sizeof(TableDirectory), 1);
    TCHAR strTag[ 8 ] = "";
    strTag[ 0 ] = TableDirectory.tag[ 0 ];
    strTag[ 1 ] = TableDirectory.tag[ 1 ];
    strTag[ 2 ] = TableDirectory.tag[ 2 ];
    strTag[ 3 ] = TableDirectory.tag[ 3 ];
    strTag[ 4 ] = 0;

    if (0 == stricmp(strTag, "name")) {
      return ParseNameMem(This, m_pMemPtrReader, TableDirectory);
    }
  }

  if (m_pMemPtrReader) {
    delete m_pMemPtrReader;
  }

  m_pMemPtrReader = NULL;
  return false;
}

const char* GetNameID(TTF* This, USHORT uNameID)
{
  switch (uNameID) {
  case 0:
    return "Copyright notice";

  case 1:
    return "Font Family name";

  case 2:
    return "Font Subfamily name";

  case 3:
    return "Unique font identifier";

  case 4:
    return "Full font name";

  case 5:
    return "Version string";

  case 6:
    return "Postscript name for the font";

  case 7:
    return "Trademark";

  case 8:
    return "Manufacturer Name";

  case 9:
    return "Designer";

  case 10:
    return "Description";

  case 11:
    return "URL Vendor";

  case 12:
    return "URL Designer";

  case 13:
    return "License Description";

  case 14:
    return "License Info URL";

  case 15:
    return "Reserved";

  case 16:
    return "Preferred Family";

  case 17:
    return "Preferred Subfamily";

  case 18:
    return "Compatible Full (Macintosh only)";

  case 19:
    return "Sample text";

  case 20:
    return "PostScript CID findfont name";

  case 21:
    return "WWS Family Name";

  case 22:
    return "WWS Subfamily Name";

  default:
    return "Unknown Name ID";
  }

  return "Unknown Name ID";
}
BOOL ParseTTFFile(TTF* This, const TCHAR* szFilePath)
{
  FILE* m_pFile = _tfopen(szFilePath, _T("rb"));
  BOOL ret = false;

  if (NULL != m_pFile) {
    ret = ParseOffsetTable(This, m_pFile);
    fclose(m_pFile);
  }

  return ret;
}

BOOL ParseTTFFilePtr(TTF* This, FILE* pFile, int off)
{
  return ParseOffsetTable(This, pFile);
}

BOOL ParseTTFMem(TTF* This, BYTE* pFile, int size, int offset)
{
  MemPtrReader aa;

  if (pFile == NULL) {
    return false;
  }

  MemPtrReader_Init(&aa, pFile, size, offset);
  return ParseOffsetTableMem(This, &aa);
}
struct stTTCHeader {
  CHAR tag[ 4 ];
  UINT Version;
  ULONG numFonts;
};
int ParseFontMem(TTF* This, size_t nMaxLen, BYTE* pFile, size_t size, int offset)
{
  stTTCHeader TTCHeader;
  memset(&TTCHeader, 0, sizeof(TTCHeader));
  MemPtrReader memPtrReader;
  MemPtrReader_Init(&memPtrReader, pFile, size, offset);
  Read(&memPtrReader, &TTCHeader, sizeof(TTCHeader), 1);
  Switch(TTCHeader.numFonts);

  if (NULL == This) {
    return TTCHeader.numFonts;
  }

  char strTag[ 8 ] = "";
  strTag[ 0 ] = TTCHeader.tag[ 0 ];
  strTag[ 1 ] = TTCHeader.tag[ 1 ];
  strTag[ 2 ] = TTCHeader.tag[ 2 ];
  strTag[ 3 ] = TTCHeader.tag[ 3 ];
  strTag[ 4 ] = 0;

  if (0 != stricmp(strTag, "ttcf")) {
    return false;
  }

  UINT* vecOffset = (UINT*) malloc(TTCHeader.numFonts);
  size_t i, j, numFonts;

  for (i = 0; i < TTCHeader.numFonts; ++i) {
    UINT nOffset = 0;
    memset(&nOffset, 0, sizeof(nOffset));
    Read(&memPtrReader, &nOffset, sizeof(nOffset), 1);
    Switch(nOffset);
    vecOffset[ i ] = (nOffset);
  }

#ifndef MIN
#define MIN(a, b)  ((a)<(b)?(a):(b))
#endif // MIN
  numFonts = MIN(TTCHeader.numFonts, nMaxLen);

  for (j = i = 0; i < numFonts; ++i) {
    TTF* pTtf = This + j;
    Seek(&memPtrReader, vecOffset[ i ], SEEK_SET);
    BOOL b = ::ParseTTFMem(pTtf, GetPtr(&memPtrReader), MemPtrReader_Size(&memPtrReader), vecOffset[ i ]);

    if (!b) {
      ++j;
    }
  }

  free(vecOffset);
  return j;
}
int ParseFont(TTF* This, size_t nMaxLen, FILE* m_pFile)
{
  stTTCHeader TTCHeader;
  memset(&TTCHeader, 0, sizeof(TTCHeader));
  fread(&TTCHeader, sizeof(TTCHeader), 1, m_pFile);
  Switch(TTCHeader.numFonts);

  if (NULL == This) {
    return TTCHeader.numFonts;
  }

  char strTag[ 8 ] = "";
  strTag[ 0 ] = TTCHeader.tag[ 0 ];
  strTag[ 1 ] = TTCHeader.tag[ 1 ];
  strTag[ 2 ] = TTCHeader.tag[ 2 ];
  strTag[ 3 ] = TTCHeader.tag[ 3 ];
  strTag[ 4 ] = 0;

  if (0 != stricmp(strTag, "ttcf")) {
    return false;
  }

  UINT* vecOffset = (UINT*) malloc(TTCHeader.numFonts);
  size_t i, j, numFonts;

  for (i = 0; i < TTCHeader.numFonts; ++i) {
    UINT nOffset = 0;
    memset(&nOffset, 0, sizeof(nOffset));
    fread(&nOffset, sizeof(nOffset), 1, m_pFile);
    Switch(nOffset);
    vecOffset[ i ] = (nOffset);
  }

  numFonts = MIN(TTCHeader.numFonts, nMaxLen);

  for (j = i = 0; i < numFonts; ++i) {
    TTF* pTtf = This + j;
    fseek(m_pFile, vecOffset[ i ], SEEK_SET);
    BOOL b = ::ParseTTFFilePtr(pTtf, m_pFile, vecOffset[ i ]);

    if (!b) {
      ++j;
    }
  }

  free(vecOffset);
  return j;
}
int ParseTTCFile(TTF* This, int nMaxLen, const TCHAR* szFilePath)
{
  FILE* m_pFile = _tfopen(szFilePath, _T("rb"));

  if (NULL == m_pFile) {
    return false;
  }

  return ParseFont(This, nMaxLen, m_pFile);
}
int ParseTTCMem(TTF* This, int nMaxLen, BYTE* pFile, int size, int offset)
{
  if (pFile == NULL) {
    return false;
  }

  return ParseFontMem(This, nMaxLen, pFile, size, offset);
}
int test_TTF(const char* szFile)
{
  const char* m_szExt;
  m_szExt = strrchr(szFile, '.');

  if (NULL == m_szExt) {
    return 0;
  }

  if (0 == stricmp(m_szExt, ".ttf") || 0 == stricmp(m_szExt, ".otf")) {
    TTF ttf = {0};

    if (false == ParseTTFFile(&ttf, (LPCTSTR)(szFile))) {
      printf(_T("Font file cannot be parsed!"));
      return 0;
    }
    else {
      printf("FontName : %s\r\n\r\n", ttf.m_szFontName);
#define OUTPUT(_NAME)  printf(#_NAME" : %s\r\n\r\n", ttf.m_sz##_NAME)
      OUTPUT(FontName);
      OUTPUT(Copyright);
      OUTPUT(FontFamilyName);
      OUTPUT(FontSubFamilyName);
      OUTPUT(FontID);
      OUTPUT(Version);
      OUTPUT(PostScriptName);
      OUTPUT(Trademark);
#undef OUTPUT
    }
  }

  if (m_szExt == "ttc") {
    TTF ttc[ 100 ];
    int ttclen;

    if (ttclen = ParseTTCFile(ttc, 100, (LPCTSTR)(szFile))) {
      char szStr[ 256 ] = _T("");

      for (int i = 0; i < ttclen; ++i) {
#define OUTPUT(_NAME)  printf(#_NAME" : %s\r\n\r\n", ttc[i].m_sz##_NAME)
        OUTPUT(FontName);
        OUTPUT(Copyright);
        OUTPUT(FontFamilyName);
        OUTPUT(FontSubFamilyName);
        OUTPUT(FontID);
        OUTPUT(Version);
        OUTPUT(PostScriptName);
        OUTPUT(Trademark);
#undef OUTPUT
        printf("==========================================\r\n\r\n");
      }
    }
  }

  return 0;
}
HANDLE AddTTFFontResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType, LOGFONT* plf)
{
  HANDLE m_fonthandle = NULL;
  HRSRC res = FindResource(hModule, lpName, lpType);

  if (res) {
    HGLOBAL mem = LoadResource(hModule, res);
    void* data = LockResource(mem);
    size_t len = SizeofResource(hModule, res);
    TTF m_Ttf;
    DWORD nFonts;
    m_fonthandle = AddFontMemResourceEx(data, len, NULL, &nFonts);
    ParseTTFMem(&m_Ttf, (BYTE*)(data), len, 0);

    if (m_fonthandle) {
      memset(plf, 0, sizeof(LOGFONT));
      plf->lfHeight = -30;
      plf->lfWeight = FW_NORMAL;
      plf->lfOutPrecision = OUT_TT_ONLY_PRECIS;
      _tcscpy(plf->lfFaceName, (m_Ttf.m_szFontFamilyName));
    }
  }

  return m_fonthandle;
}
int AddTTFFontFile(LPCTSTR szFontFile, LOGFONT* plf)
{
  int m_nResults = 0;
  TTF m_Ttf;
  m_nResults = AddFontResourceEx(szFontFile, FR_PRIVATE, NULL);
  ParseTTFFile(&m_Ttf, (szFontFile));

  if (m_nResults != 0) {
    memset(plf, 0, sizeof(LOGFONT));
    plf->lfHeight = -30;
    plf->lfWeight = FW_NORMAL;
    plf->lfOutPrecision = OUT_TT_ONLY_PRECIS;
    _tcscpy(plf->lfFaceName, (m_Ttf.m_szFontFamilyName));
  }

  return m_nResults;
}
#endif // _TTF_H_
