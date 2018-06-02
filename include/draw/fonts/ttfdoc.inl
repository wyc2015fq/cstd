
#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <math.h>
#include <io.h>

/*
 class TtfDOC;

 class TableINFO;
 class cmapTableINFO;
 class glyfTableINFO;
 class headTableINFO;
 class hheaTableINFO;
 class hmtxTableINFO;
 class locaTableINFO;
 class maxpTableINFO;
 class nameTableINFO;
 class postTableINFO;
 class OS2TableINFO;
 class cvtTableINFO;
 class OtherTableINFO;
*/

char* ErrStr[] = {
  "该文件不能按一个ttf格式识别！",
  "写字体文件时发生一个未知错误！"
};

#define READFILE(Buf, Len, pFile) \
  if((int)fread(Buf, 1, (Len), pFile) != (Len)) { \
    if(ferror(pFile)) \
      Throw_StdCrtException(); \
    else \
      Throw_CustomException(ErrStr[0]); \
  }

#define WRITEFILE(Buf, Len, pFile) \
  if((int)fwrite(Buf, 1, (Len), pFile) != (Len)) { \
    if(ferror(pFile)) \
      Throw_StdCrtException(); \
    else \
      Throw_CustomException(ErrStr[1]); \
  }

void ttfReadFile(void* pBuf, const char* pFrame, int Count, FILE* pFile)
{
  int Size = GetDataFrameLength(pFrame);
  READFILE(pBuf, Size * Count, pFile);
  ReverseOrder(pBuf, pFrame, Count);
}

void ttfWriteFile(const void* pBuf, const char* pFrame, int Count, FILE* pFile)
{
  int Size = GetDataFrameLength(pFrame);
  int Len = Size * Count;
  void* pNewBuf = _alloca(Len);
  memmove(pNewBuf, pBuf, Len);
  ReverseOrder(pNewBuf, pFrame, Count);
  WRITEFILE(pNewBuf, Len, pFile);
}

WORD ttfGetWord(FILE* pFile)
{
  WORD Word;
  READFILE(&Word, sizeof(WORD), pFile);
  return ReverseWord(Word);
}

DWORD ttfGetDWord(FILE* pFile)
{
  DWORD DWord;
  READFILE(&DWord, sizeof(DWORD), pFile);
  return ReverseDWord(DWord);
}

void ttfPutWord(WORD Word, FILE* pFile)
{
  Word = ReverseWord(Word);
  WRITEFILE(&Word, sizeof(WORD), pFile);
}

void ttfPutDWord(DWORD DWord, FILE* pFile)
{
  DWord = ReverseDWord(DWord);
  WRITEFILE(&DWord, sizeof(DWORD), pFile);
}

BYTE ttfGetByte(FILE* pFile)
{
  int Ch;
  READFILE(&Ch, 1, pFile);
  return Ch;
}

void ttfPutByte(BYTE Ch, FILE* pFile)
{
  WRITEFILE(&Ch, 1, pFile);
}

#define ttfGetShort(pFile) (SHORT)ttfGetWord(pFile)
#define ttfGetUShort(pFile) (USHORT)ttfGetWord(pFile)
#define ttfPutShort(Num, pFile) ttfPutWord(Num, pFile)
#define ttfPutUShort(Num, pFile) ttfPutWord(Num, pFile)

// class TtfDOC Begin
TableINFO* CreateTableInfo(const TableENTRY* pTableEntry);

void TtfDOC::set()
{
  pTableEntry = NULL;
  pTableInfo = NULL;
}

void TtfDOC::set(const TtfDOC& TtfDoc)
{
  memmove(&TtfHeader, &TtfDoc.TtfHeader, sizeof(TtfHEADER));

  if (TtfDoc.pTableEntry) {
    pTableEntry = (TableENTRY*)memdup(TtfDoc.pTableEntry, TtfHeader.numTables * sizeof(TableENTRY));
  }
  else {
    pTableEntry = NULL;
  }

  pTableInfo = (TableINFO**)malloc(sizeof(TableINFO*) * TtfHeader.numTables);

  for (int i = 0; i < TtfHeader.numTables; i++) {
    pTableInfo[i] = CreateTableInfo(pTableEntry + i);
    pTableInfo[i]->SetTtfDoc(this);
    pTableInfo[i]->Duplicate(TtfDoc.pTableInfo[i]);
  }
}

void TtfDOC::Destroy()
{
  if (pTableInfo) {
    for (int i = 0; i < TtfHeader.numTables; i++) {
      delete(pTableInfo[i]);
    }
  }

  free(pTableInfo);
  free(pTableEntry);

  pTableInfo = NULL;
  pTableEntry = NULL;
}

void TtfDOC::LoadTtf(FILE* pFile)
{
  fseek(pFile, 0, SEEK_SET);
  ttfReadFile(&TtfHeader, "DWWWW", 1, pFile);

  pTableEntry = (TableENTRY*)malloc(sizeof(TableENTRY) * TtfHeader.numTables);
  ttfReadFile(pTableEntry, "BBBBDDD", TtfHeader.numTables, pFile);

  pTableInfo = (TableINFO**)malloc(sizeof(TableINFO*) * TtfHeader.numTables);
  memset(pTableInfo, 0, sizeof(TableINFO*) * TtfHeader.numTables);

  for (int i = 0; i < TtfHeader.numTables; i++) {
    if (!pTableInfo[i]) {
      pTableInfo[i] = CreateTableInfo(pTableEntry + i);
      pTableInfo[i]->SetTtfDoc(this);
      pTableInfo[i]->Load(pFile, pTableEntry + i);

#ifdef _DEBUG
      int ThisPos = ftell(pFile);
      assert((int)pTableEntry[i].Offset + (int)pTableEntry[i].Length <= 3 + ThisPos);
#endif
    }

  }

}

TableINFO* TtfDOC::GetTableInfo(const char* pTag, FILE* pFile)
{
  for (int i = 0; i < TtfHeader.numTables; i++) {
    if ((pTableEntry + i)->tagName == *((DWORD*)pTag)) {
      if (!pTableInfo[i]) { //这个表还未被load，load先
        pTableInfo[i] = CreateTableInfo(pTableEntry + i);
        pTableInfo[i]->SetTtfDoc(this);
        pTableInfo[i]->Load(pFile, pTableEntry + i);
      }

      return pTableInfo[i];
    }
  }

  return NULL;
}

TableINFO* TtfDOC::GetTableInfo(const char* pTag) const
{
  for (int i = 0; i < TtfHeader.numTables; i++) {
    if ((pTableEntry + i)->tagName == *((DWORD*)pTag)) {
      return pTableInfo[i];
    }
  }

  return NULL;
}

#define CREATE_TABLEINFO(Tag, StrTag, ClassName)  { \
    if((Tag) == *((DWORD *)StrTag)) return new ClassName;  }

TableINFO* CreateTableInfo(const TableENTRY* pTableEntry)
{
  DWORD tagName = pTableEntry->tagName;

  CREATE_TABLEINFO(tagName, "cmap", cmapTableINFO)
  CREATE_TABLEINFO(tagName, "glyf", glyfTableINFO)
  CREATE_TABLEINFO(tagName, "head", headTableINFO)
  CREATE_TABLEINFO(tagName, "hhea", hheaTableINFO)
  CREATE_TABLEINFO(tagName, "hmtx", hmtxTableINFO)

  CREATE_TABLEINFO(tagName, "loca", locaTableINFO)
  CREATE_TABLEINFO(tagName, "maxp", maxpTableINFO)
  CREATE_TABLEINFO(tagName, "name", nameTableINFO)
  CREATE_TABLEINFO(tagName, "post", postTableINFO)
  CREATE_TABLEINFO(tagName, "OS/2", OS2TableINFO)

  CREATE_TABLEINFO(tagName, "cvt ", cvtTableINFO)
  /*CREATE_TABLEINFO(tagName, "EBDT", EBDTTableINFO)
  CREATE_TABLEINFO(tagName, "EBLC", EBLCTableINFO)
  CREATE_TABLEINFO(tagName, "EBSC", EBSCTableINFO)
  CREATE_TABLEINFO(tagName, "fpgm", fpgmTableINFO)

  CREATE_TABLEINFO(tagName, "gasp", gaspTableINFO)
  CREATE_TABLEINFO(tagName, "hdmx", hdmxTableINFO)
  CREATE_TABLEINFO(tagName, "kern", kernTableINFO)
  CREATE_TABLEINFO(tagName, "LTSH", LTSHTableINFO)
  CREATE_TABLEINFO(tagName, "prep", prepTableINFO)

  CREATE_TABLEINFO(tagName, "PCLT", PCLTTableINFO)
  CREATE_TABLEINFO(tagName, "VDMX", VDMXTableINFO)
  CREATE_TABLEINFO(tagName, "vhea", vheaTableINFO)
  CREATE_TABLEINFO(tagName, "vmtx", vmtxTableINFO)*/
  return new OtherTableINFO;
}

ULONG CalcTableChecksum(ULONG Offset, ULONG Length, FILE* pFile)
{
  ULONG i;
  ULONG Sum = 0;
  ULONG Count = ((Length + 3) & ~3) / sizeof(ULONG);

  if (fseek(pFile, Offset, SEEK_SET)) {
    Throw_StdCrtException();
  }

  for (i = 0; i < Count; i++) {
    Sum += ttfGetDWord(pFile);
  }

  return Sum;
}

//边界Align字节对齐，不齐用零填充，返回对齐后的偏移
int Zero = 0;
ULONG PadingZero(FILE* pFile, int Align)
{
  assert(Align == 2 || Align == 4 || Align == 8);
  Align--;
  ULONG CurPos = ftell(pFile);
  ULONG Offset = ((CurPos + Align) & ~Align);
  CurPos = Offset - CurPos;

  if (CurPos > 0) {
    WRITEFILE(&Zero, CurPos, pFile);
  }

  return Offset;
}

void TtfDOC::StoreTtf(FILE* pFile) const
{
  fseek(pFile, 0, SEEK_SET);
  ttfWriteFile(&TtfHeader, "DWWWW", 1, pFile);
  ttfWriteFile(pTableEntry, "BBBBDDD", TtfHeader.numTables, pFile);

  int i, glyfIndex, locaIndex, headIndex;

  //注意：glyf应该在loca之前保存
  for (i = 0; i < TtfHeader.numTables; i++) {
    if (pTableEntry[i].tagName == *((DWORD*)"glyf")) {
      glyfIndex = i;
    }
    else if (pTableEntry[i].tagName == *((DWORD*)"loca")) {
      locaIndex = i;
    }
  }

  if (glyfIndex > locaIndex) {
    TableENTRY Temp;
    memmove(&Temp, pTableEntry + glyfIndex, sizeof(Temp));
    memmove(pTableEntry + glyfIndex, pTableEntry + locaIndex, sizeof(Temp));
    memmove(pTableEntry + locaIndex, &Temp, sizeof(Temp));
  }

  //保存每个table
  for (i = 0; i < TtfHeader.numTables; i++) {
    if (pTableEntry[i].tagName == *((DWORD*)"head")) {
      headIndex = i;
      ((headTableINFO*)pTableInfo[headIndex])->SetCheckSumAdjustment(0);
    }

    pTableEntry[i].Offset = PadingZero(pFile, 4); //边界4字节对齐
    pTableInfo[i]->Store(pFile, pTableEntry + i);
    pTableEntry[i].Length = ftell(pFile) - pTableEntry[i].Offset;

  }

  PadingZero(pFile, 4); //确保CheckSum的计算准确

  for (i = 0; i < TtfHeader.numTables; i++) {
    pTableEntry[i].CheckSum = CalcTableChecksum(pTableEntry[i].Offset, pTableEntry[i].Length, pFile);
  }

  //把纠正后的TableEntries再次写到文件头
  fseek(pFile, HEADSIZE, SEEK_SET);
  ttfWriteFile(pTableEntry, "BBBBDDD", TtfHeader.numTables, pFile);

  //计算head表里的CheckSumAdjustment
  ULONG SumVal = CalcTableChecksum(0, _filelength(_fileno(pFile)), pFile);
  fseek(pFile, pTableEntry[headIndex].Offset + 8, SEEK_SET);
  ttfPutDWord(0xB1B0AFBA - SumVal, pFile);

}

void TtfDOC::AppendGlyphByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  int maxpIndex;

  for (int i = 0; i < TtfHeader.numTables; i++) {
    if (pTableEntry[i].tagName == *((DWORD*)"maxp")) {
      maxpIndex = i;
    }
    else if (pTableEntry[i].tagName == *((DWORD*)"cmap")) {
      pTableInfo[i]->AppendGlyfByDoc(pAddedDoc, Param);
    }
    else {
      pTableInfo[i]->AppendGlyfByDoc(pAddedDoc, NULL);
    }
  }

  //保证maxp最后调用，因为有的table要引用该表内的NumGlyphs
  pTableInfo[maxpIndex]->AppendGlyfByDoc(pAddedDoc, NULL);
}

int TtfDOC::GetGlyfCount()
{
  maxpTableINFO* pmaxpTable = (maxpTableINFO*)GetTableInfo("maxp");
  return pmaxpTable->GetNumberOfGlyphs();
}

void TtfDOC::GetGlyph(ExportGlyfSTRUCT* pExport, int Idx) const
{
  glyfTableINFO* pGlyfTable = (glyfTableINFO*)GetTableInfo("glyf");
  pGlyfTable->GetGlyph(pExport, Idx);
}

USHORT TtfDOC::GetUnitsPerEm() const
{
  headTableINFO* pHeadTable = (headTableINFO*)GetTableInfo("head");
  return pHeadTable->GetUnitsPerEm();
}

int TtfDOC::GetFontName(char* Buf, BOOL* bUnicode, ULONG BufLen) const
{
  nameTableINFO* pNameTable = (nameTableINFO*)GetTableInfo("name");
  return pNameTable->GetFontName(Buf, bUnicode, BufLen);
}

void TtfDOC::SetFontName(const char* pStr, BOOL bUnicode)
{
  nameTableINFO* pNameTable = (nameTableINFO*)GetTableInfo("name");
  pNameTable->SetFontName(pStr, bUnicode);
}
// class TtfDOC End

// class TableINFO Begin
TableINFO* TableINFO::GetTableInfo(const char* pTag, FILE* pFile)
{
  return pTtfDoc->GetTableInfo(pTag, pFile);
}

TableINFO* TableINFO::GetTableInfo(const char* pTag) const
{
  return pTtfDoc->GetTableInfo(pTag);
}

void TableINFO::SetTtfDoc(TtfDOC* pDoc)
{
  pTtfDoc = pDoc;
}
// class TableINFO End

// class cmapTableINFO Begin
cmapTableINFO::cmapTableINFO() : MapEntries(NULL), MapTableInfo(NULL) {}

void DestroyMapTableInfo(FormatHEADER* pMapTable)
{
  switch (pMapTable->Format) {
  case 2: {
    EncodingMapFORMAT2* p = (EncodingMapFORMAT2*)pMapTable;
    free(p->SubHeaders);
    int Count = p->SubHeaderKeys[255] >> 3;

    for (int i = 0; i < Count; i++) {
      free(p->IndexArray[i]);
    }

    free(p->IndexArray);
    break;
  }

  case 4: {
    EncodingMapFORMAT4* p = (EncodingMapFORMAT4*)pMapTable;
    free(p->endCode);
    free(p->startCode);
    free(p->idDelta);
    free(p->idRangeOffset);
    int Count = p->segCountX2 >> 1;

    for (int i = 0; i < Count; i++) {
      free(p->IndexArray[i]);
    }

    free(p->IndexArray);
    break;
  }

  case 6: {
    EncodingMapFORMAT6* p = (EncodingMapFORMAT6*)pMapTable;
    free(p->glyphIdArray);
    break;
  }
  }
}

cmapTableINFO::~cmapTableINFO()
{
  for (int i = 0; i < TableNumber; i++) {
    if (MapEntries[i].CodingMapId == i) {
      DestroyMapTableInfo(MapTableInfo[i]);
      free(MapTableInfo[i]);
    }
  }

  free(MapTableInfo);
  free(MapEntries);
}

void cmapTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  int i;
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  TableVersion = ttfGetUShort(pFile);
  TableNumber = ttfGetUShort(pFile);

  MapEntries = (EncodingMapENTRY*)malloc(sizeof(EncodingMapENTRY) * TableNumber);

  for (i = 0; i < TableNumber; i++) {
    ttfReadFile(MapEntries + i, "WWD", 1, pFile);
  }

  int LastPos = 0;
  MapTableInfo = (FormatHEADER**)malloc(sizeof(FormatHEADER*) * TableNumber);
  memset(MapTableInfo, 0, sizeof(FormatHEADER*) * TableNumber);

  for (i = 0; i < TableNumber; i++) {
    //注意：不同的EncodingMapENTRY可能指向同一个编码信息
    MapEntries[i].CodingMapId = i;

    for (int j = 0; j < i; j++) {
      if (MapEntries[i].Offset == MapEntries[j].Offset) {
        MapEntries[i].CodingMapId = j;
        break;
      }
    }

    if (MapEntries[i].CodingMapId == i) {
      FormatHEADER TempHeader;
      fseek(pFile, pTableEntry->Offset + MapEntries[i].Offset, SEEK_SET);
      //先读格式头信息
      ttfReadFile(&TempHeader, "W", 3, pFile);
      MapTableInfo[i] = CreateAndReadMapFormat(&TempHeader, pFile);

      int CurPos = ftell(pFile);

      if (LastPos < CurPos) {
        LastPos = CurPos;
      }
    }
    else {
      MapTableInfo[i] = MapTableInfo[MapEntries[i].CodingMapId];
    }
  }

  fseek(pFile, LastPos, SEEK_SET);
}

FormatHEADER* cmapTableINFO::CreateAndReadMapFormat(FormatHEADER* pHeader, FILE* pFile)
{
  int ThisPos = ftell(pFile) - sizeof(USHORT) * 3;

  switch (pHeader->Format) {
  case 0: {
    EncodingMapFORMAT0* pFormat = (EncodingMapFORMAT0*)malloc(sizeof(EncodingMapFORMAT0));
    memmove(pFormat, pHeader, sizeof(FormatHEADER));
    ttfReadFile(pFormat->IndexArray, "B", sizeof(pFormat->IndexArray), pFile);
    return (FormatHEADER*)pFormat;
  }

  case 2: {
    EncodingMapFORMAT2* pFormat = (EncodingMapFORMAT2*)malloc(sizeof(EncodingMapFORMAT2));
    memmove(pFormat, pHeader, sizeof(FormatHEADER));
    ttfReadFile(pFormat->SubHeaderKeys, "W", 256, pFile);

    //因为subheaderkeys存放subheader的index*8
    int Len = (pFormat->SubHeaderKeys[255] >> 3) + 1;
    pFormat->SubHeaders = (SubHEADER*)malloc(sizeof(SubHEADER) * Len);
    pFormat->IndexArray = (USHORT**)malloc(sizeof(USHORT*) * Len);
    memset(pFormat->IndexArray, 0, sizeof(USHORT*) * Len);

    //读每个子区段
    int Offset = ftell(pFile);

    for (int i = 0; i < Len; i++) {
      fseek(pFile, Offset, SEEK_SET);
      ttfReadFile(pFormat->SubHeaders + i, "WWWW", 1, pFile);
      Offset = ftell(pFile);
      fseek(pFile, pFormat->SubHeaders[i].idRangeOffset - sizeof(USHORT), SEEK_CUR);///??
      pFormat->IndexArray[i] = (USHORT*)malloc(sizeof(USHORT) * pFormat->SubHeaders[i].entryCount);
      ttfReadFile(pFormat->IndexArray[i], "W", pFormat->SubHeaders[i].entryCount, pFile);
    }

    assert(pFormat->Length == ftell(pFile) - ThisPos);
    return (FormatHEADER*)pFormat;
  }

  case 4: {
    EncodingMapFORMAT4* pFormat = (EncodingMapFORMAT4*)malloc(sizeof(EncodingMapFORMAT4));
    memmove(pFormat, pHeader, sizeof(FormatHEADER));
    ttfReadFile(&pFormat->segCountX2, "W", 4, pFile);

    int SegCount = pFormat->segCountX2 >> 1;
    pFormat->endCode = (USHORT*)malloc(sizeof(USHORT) * SegCount);
    ttfReadFile(pFormat->endCode, "W", SegCount, pFile);
    pFormat->reservedPad = ttfGetUShort(pFile);
    pFormat->startCode = (USHORT*)malloc(sizeof(USHORT) * SegCount);
    ttfReadFile(pFormat->startCode, "W", SegCount, pFile);
    pFormat->idDelta = (USHORT*)malloc(sizeof(USHORT) * SegCount);
    ttfReadFile(pFormat->idDelta, "W", SegCount, pFile);
    pFormat->idRangeOffset = (USHORT*)malloc(sizeof(USHORT) * SegCount);
    int Offset = ftell(pFile);
    ttfReadFile(pFormat->idRangeOffset, "W", SegCount, pFile);

    pFormat->IndexArray = (USHORT**)malloc(sizeof(USHORT*) * SegCount);
    memset(pFormat->IndexArray, 0, sizeof(USHORT*) * SegCount);

    //读每个子区段的索引子表
    for (int i = 0; i < SegCount; i++) { //最后一个段(0xffff)不读文件,直接赋零
      if (pFormat->idRangeOffset[i]) {
        int Count = pFormat->endCode[i] - pFormat->startCode[i] + 1;
        pFormat->IndexArray[i] = (USHORT*)malloc(sizeof(USHORT) * Count);

        if (i < SegCount - 1 && !fseek(pFile, Offset + i * sizeof(USHORT) + pFormat->idRangeOffset[i], SEEK_SET)) {
          ttfReadFile(pFormat->IndexArray[i], "W", Count, pFile);
        }
        else {
          memset(pFormat->IndexArray[i], 0, sizeof(USHORT) * Count);
        }
      }
    }

#ifdef _DEBUG
    Offset = ftell(pFile);
    assert(pFormat->Length >= Offset - ThisPos && pFormat->Length <= Offset - ThisPos + 2);
#endif
    return (FormatHEADER*)pFormat;
  }

  case 6: {
    EncodingMapFORMAT6* pFormat = (EncodingMapFORMAT6*)malloc(sizeof(EncodingMapFORMAT6));
    memmove(pFormat, pHeader, sizeof(FormatHEADER));
    pFormat->firstCode = ttfGetShort(pFile);
    pFormat->entryCount = ttfGetShort(pFile);
    pFormat->glyphIdArray = (USHORT*)malloc(sizeof(USHORT) * pFormat->entryCount);
    ttfReadFile(pFormat->glyphIdArray, "W", pFormat->entryCount, pFile);
    return (FormatHEADER*)pFormat;
  }

  default: //未知格式
    Throw_CustomException(ErrStr[0]);
  }

  return NULL;
}

void cmapTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfPutUShort(TableVersion, pFile);
  ttfPutUShort(TableNumber, pFile);

  int i, EntriesPos = ftell(pFile);

  for (i = 0; i < TableNumber; i++) {
    ttfWriteFile(MapEntries + i, "WWD", 1, pFile);
  }

  int LastPos = 0;

  for (i = 0; i < TableNumber; i++) {
    if (MapEntries[i].CodingMapId == i) {
      PadingZero(pFile, 4);
      MapEntries[i].Offset = ftell(pFile) - pTableEntry->Offset;
      WriteMapFormat(MapTableInfo[i], pFile);

      int CurPos = ftell(pFile);

      if (LastPos < CurPos) {
        LastPos = CurPos;
      }
    }
    else {
      MapEntries[i].Offset = MapEntries[MapEntries[i].CodingMapId].Offset;
    }
  }

  //把修正后的MapEntries再次写入文件
  fseek(pFile, EntriesPos, SEEK_SET);

  for (i = 0; i < TableNumber; i++) {
    ttfWriteFile(MapEntries + i, "WWD", 1, pFile);
  }

  fseek(pFile, LastPos, SEEK_SET);
}

void cmapTableINFO::WriteMapFormat(FormatHEADER* MapTableInfo, FILE* pFile) const
{
  int ThisPos = ftell(pFile);
  ttfWriteFile(MapTableInfo, "W", 3, pFile);

  switch (MapTableInfo->Format) {
  case 0: {
    EncodingMapFORMAT0* pFormat = (EncodingMapFORMAT0*)MapTableInfo;
    ttfWriteFile(pFormat->IndexArray, "B", sizeof(pFormat->IndexArray), pFile);
    break;
  }

  case 2: {
    EncodingMapFORMAT2* pFormat = (EncodingMapFORMAT2*)MapTableInfo;
    ttfWriteFile(pFormat->SubHeaderKeys, "W", 256, pFile);
    //因为subheaderkeys存放subheader的index*8
    int Len = (pFormat->SubHeaderKeys[255] >> 3) + 1;
    int HeaderOffset = ftell(pFile);
    ttfWriteFile(pFormat->SubHeaders, "WWWW", Len, pFile);

    //写每个子区段
    for (int i = 0; i < Len; i++) {
      pFormat->SubHeaders[i].idRangeOffset = ftell(pFile) - (HeaderOffset + i * 8 + 6); //8:sizeof(SubHeader)
      ttfWriteFile(pFormat->IndexArray[i], "W", pFormat->SubHeaders[i].entryCount, pFile);
    }

    int Offset = ftell(pFile);
    //把调整过后的SubHeader数组重新写入
    fseek(pFile, HeaderOffset, SEEK_SET);
    ttfWriteFile(pFormat->SubHeaders, "WWWW", Len, pFile);
    fseek(pFile, Offset, SEEK_SET);
    break;
  }

  case 4: {
    EncodingMapFORMAT4* pFormat = (EncodingMapFORMAT4*)MapTableInfo;
    ttfWriteFile(&pFormat->segCountX2, "W", 4, pFile);
    int SegCount = pFormat->segCountX2 >> 1;

    ttfWriteFile(pFormat->endCode, "W", SegCount, pFile);
    ttfPutUShort(pFormat->reservedPad, pFile);
    ttfWriteFile(pFormat->startCode, "W", SegCount, pFile);
    ttfWriteFile(pFormat->idDelta, "W", SegCount, pFile);
    int Offset = ftell(pFile);
    ttfWriteFile(pFormat->idRangeOffset, "W", SegCount, pFile);

    //写每个子区段的索引子表
    for (int i = 0; i < SegCount; i++) { //(0xffff)
      if (pFormat->IndexArray[i]) {
        int Count = pFormat->endCode[i] - pFormat->startCode[i] + 1;
        pFormat->idRangeOffset[i] = ftell(pFile) - (Offset + i * sizeof(USHORT));
        ttfWriteFile(pFormat->IndexArray[i], "W", Count, pFile);
      }
      else {
        pFormat->idRangeOffset[i] = 0;
      }
    }

    int LastPos = ftell(pFile);
    //把修正后的索引值重写回
    fseek(pFile, Offset, SEEK_SET);
    ttfWriteFile(pFormat->idRangeOffset, "W", SegCount, pFile);
    fseek(pFile, LastPos, SEEK_SET);
    //ttfWriteFile(pFormat->IndexArray, "W", pFormat->IndexCount, pFile);
    break;
  }

  case 6: {
    EncodingMapFORMAT6* pFormat = (EncodingMapFORMAT6*)MapTableInfo;
    ttfPutUShort(pFormat->firstCode, pFile);
    ttfPutUShort(pFormat->entryCount, pFile);
    ttfWriteFile(pFormat->glyphIdArray, "W", pFormat->entryCount, pFile);
    break;
  }

  default: //未知格式
    Throw_CustomException(ErrStr[0]);
  }

  int CurPos = ftell(pFile);
  MapTableInfo->Length = CurPos - ThisPos;
  fseek(pFile, ThisPos, SEEK_SET);
  ttfWriteFile(MapTableInfo, "W", 3, pFile);
  fseek(pFile, CurPos, SEEK_SET);
}

//格式2主要用于日文，中文，韩文等语言字符。这些语言字符使用8位或16位混合编码的方式
//高位字节作为SubHeaderKeys数组的索引。对于8位表示字符码总是对应于SubHeaderKeys[0]
//SubHeaderKeys元素的值是SubHeader数组索引*8。SubHeader是一个结构数组。结构解释如下：
//域firstCode指示该区内第一个有效的低位字节的码值;
//域entryCount指示该区内有效的低位字节的码值的个数；
//域idDelta用来对取得的字形索引做调节的值。
//域idRangeOffset指示在后续索引数组里该区的第一个索引元素距该处的偏移

//举列：如字'大'的GB码为0x9f43，查找索引方法如下:
//先由高位字节0x9f找到SubHeaderKeys[0x9f - 1]---->Key
//再找到对该区的描述的结构体SubHeader[Key/8]，则计算字形索引值方法如下：
//*(&idRangeOffset + idRangeOffset/2 + (0x43 - firstCode)) -->Raw
//if(Raw != 0)
// glyfindex = Raw + idDelta
//else
// glyfindex = 0 (the index of missing char)

//往格式2的映射表里加入字符码
void AppendMapTable(EncodingMapFORMAT2* pMapTable, int FirstIndex, USHORT CharArr[], int DetaCount)
{
  //向子区段加入新字符形成新的index表
  for (int i = 0; i < DetaCount; i++) {
    int SubHeaderKey = pMapTable->SubHeaderKeys[*(CharArr + i) >> 8] >> 3;
    SubHEADER* pHeader = pMapTable->SubHeaders + SubHeaderKey;
    int ItemCount = pHeader->entryCount;
    USHORT Low = *(CharArr + i) & 0xff;

    if (Low > pHeader->firstCode) { //在firstCode后面加
      int CountAfter = Low - pHeader->firstCode + 1;

      if (CountAfter > ItemCount) {
        pMapTable->IndexArray[SubHeaderKey] =
            (USHORT*)realloc(pMapTable->IndexArray[SubHeaderKey], CountAfter * sizeof(USHORT));

        for (int i = ItemCount; i < CountAfter; i++) {
          pMapTable->IndexArray[SubHeaderKey][i] = 0;
        }

        pHeader->entryCount = CountAfter;
      }

      pMapTable->IndexArray[SubHeaderKey][Low - pHeader->firstCode] = FirstIndex + i - pHeader->idDelta;
    }
    else { //在firstCode前面加
      pHeader->entryCount += pHeader->firstCode - Low;
      USHORT* NewSubArray = (USHORT*)malloc(pHeader->entryCount * sizeof(USHORT));
      memset(NewSubArray, 0, pHeader->entryCount * sizeof(USHORT));
      memmove(NewSubArray + (pHeader->firstCode - Low), pMapTable->IndexArray[SubHeaderKey], ItemCount * sizeof(USHORT));
      NewSubArray[0] = FirstIndex + i - pHeader->idDelta;
      pHeader->firstCode = Low;
      free(pMapTable->IndexArray[SubHeaderKey]);
      pMapTable->IndexArray[SubHeaderKey] = NewSubArray;
    }
  }
}

//格式4主要用于windows的unicode编码，格式4将字符码分成若干区段分别由startCode和endCode数组指定
//通过字符码找到第i个区段，在由下面公式即得字形索引值:
//1。如果idRangeOffset[i]不为零按下方法：
//*(idRangeOffset[i]/2 + (c - startCount[i]) + &idRangeOffset[i])--->Raw
//如果Raw值不为零，则Raw+idDelta就是字形索引值，否则表示对应缺省字形
//2。如果idRangeOffset[i]为零则idDelta[i] + c就是字形索引值。

//往格式4的映射表里加入字符码
void InsertSegment(EncodingMapFORMAT4* pMapTable, int Loc);
void DeleteSegment(EncodingMapFORMAT4* pMapTable, int Idx);
void SetSegment(EncodingMapFORMAT4* pMapTable, int i, USHORT StartCode, USHORT EndCode,
    USHORT IdDelta, USHORT IdRangeOffset);
void AppendMapTable(EncodingMapFORMAT4* pMapTable, int FirstIndex, USHORT CharArr[], int Count)
{
  int SegCount = pMapTable->segCountX2 >> 1;

  for (int i = 0; i < Count; i++) {
    if (!CharArr[i]) {
      continue;
    }

    //找到相应的区段
    int j = 0;

    for (; j < SegCount; j++) {
      if (CharArr[i] <= pMapTable->endCode[j]) {
        break;
      }
    }

    if (CharArr[i] < pMapTable->startCode[j]) {
      if (pMapTable->idRangeOffset[j - 1] && (CharArr[i] - pMapTable->endCode[j - 1]) < 24) {
        //扩大前一个段
        j--;
      }
      else { //在前面插入一个段
        InsertSegment(pMapTable, j);
        //初始化新段
        SetSegment(pMapTable, j, CharArr[i], CharArr[i], 0, 1);
        pMapTable->IndexArray[j] = (USHORT*)malloc(sizeof(USHORT));
        SegCount++;
      }
    }
    else {
      if (!pMapTable->idRangeOffset[j]) { //该段无子索引表，在该段中间插入
        //先检查是否能扩充前一个段
        if (CharArr[i] == pMapTable->startCode[j] && pMapTable->idRangeOffset[j - 1] &&
            (CharArr[i] - pMapTable->endCode[j - 1]) < 24) {
          pMapTable->startCode[j] ++;

          if (pMapTable->startCode[j] > pMapTable->endCode[j]) {
            DeleteSegment(pMapTable, j);
            SegCount--;
          }

          j--;//扩大前一个段
        }
        else {
          USHORT OldendCode = pMapTable->endCode[j];
          USHORT OldidDelta = pMapTable->idDelta[j];

          if (CharArr[i] > pMapTable->startCode[j]) {
            j++;
            InsertSegment(pMapTable, j);
            pMapTable->endCode[j - 1] = CharArr[i] - 1; //修改前一个段endcode
            SegCount++;
          }

          //初始化新段
          SetSegment(pMapTable, j, CharArr[i], CharArr[i], 0, 1);
          pMapTable->IndexArray[j] = (USHORT*)malloc(sizeof(USHORT));

          //根据情况在后面再新建一个段,保存后面的码正确映射
          if (OldendCode > pMapTable->endCode[j]) {
            InsertSegment(pMapTable, j + 1);
            SetSegment(pMapTable, j + 1, CharArr[i] + 1, OldendCode, OldidDelta, 0);
            pMapTable->IndexArray[j + 1] = NULL;
            SegCount++;
          }
        }
      }
    }

    int ChCount = CharArr[i] - pMapTable->startCode[j];

    if (CharArr[i] > pMapTable->endCode[j]) {
      int OldCount = pMapTable->endCode[j] - pMapTable->startCode[j] + 1;
      pMapTable->endCode[j] = CharArr[i];
      pMapTable->IndexArray[j] = (USHORT*)realloc(pMapTable->IndexArray[j], (ChCount + 1) * sizeof(USHORT));

      for (int k = OldCount; k <= ChCount; k++) {
        pMapTable->IndexArray[j][k] = 0;
      }
    }

    pMapTable->IndexArray[j][ChCount] = FirstIndex + i - pMapTable->idDelta[j];
  }
}

void SetSegment(EncodingMapFORMAT4* pMapTable, int i, USHORT StartCode, USHORT EndCode,
    USHORT IdDelta, USHORT IdRangeOffset)
{
  pMapTable->startCode[i] = StartCode;
  pMapTable->endCode[i] = EndCode;
  pMapTable->idDelta[i] = IdDelta;
  pMapTable->idRangeOffset[i] = IdRangeOffset;
}

void SetSegmentCount(EncodingMapFORMAT4* pMapTable, int SegCount)
{
  pMapTable->segCountX2 = SegCount << 1;
  pMapTable->searchRange = 1 << int(1 + floor(log10(SegCount) / log10(2)));
  int a = pMapTable->searchRange / 2;
  pMapTable->entrySelector = (int)floor(log10(a) / log10(2));
  pMapTable->rangeShift = pMapTable->segCountX2 - pMapTable->searchRange;
}

//在Loc位置插入一个新段,合理的位置是(0, count_of_segment)
void InsertSegment(EncodingMapFORMAT4* pMapTable, int Loc)
{
  int SegCount = (pMapTable->segCountX2 >> 1) + 1;
  assert(Loc <= SegCount - 1 && Loc >= 0);

  //重设与段数相关的值
  SetSegmentCount(pMapTable, SegCount);

  pMapTable->endCode = (USHORT*)realloc(pMapTable->endCode, sizeof(USHORT) * SegCount);
  pMapTable->startCode = (USHORT*)realloc(pMapTable->startCode, sizeof(USHORT) * SegCount);
  pMapTable->idDelta = (USHORT*)realloc(pMapTable->idDelta, sizeof(USHORT) * SegCount);
  pMapTable->idRangeOffset = (USHORT*)realloc(pMapTable->idRangeOffset, sizeof(USHORT) * SegCount);
  pMapTable->IndexArray = (USHORT**)realloc(pMapTable->IndexArray, sizeof(USHORT*) * SegCount);

  //从第loc个往后移
  for (int i = SegCount - 1; i > Loc; i--) {
    int CurIdx = i, PreIdx = CurIdx - 1;
    pMapTable->endCode[CurIdx] = pMapTable->endCode[PreIdx];
    pMapTable->startCode[CurIdx] = pMapTable->startCode[PreIdx];
    pMapTable->idDelta[CurIdx] = pMapTable->idDelta[PreIdx];
    pMapTable->idRangeOffset[CurIdx] = pMapTable->idRangeOffset[PreIdx];
    pMapTable->IndexArray[CurIdx] = pMapTable->IndexArray[PreIdx];
  }

  pMapTable->IndexArray[Loc] = NULL;
}

//删除一个无用的段
void DeleteSegment(EncodingMapFORMAT4* pMapTable, int Idx)
{
  int SegCount = (pMapTable->segCountX2 >> 1) - 1;
  assert(Idx <= SegCount && Idx >= 0);

  //重设与段数相关的值
  SetSegmentCount(pMapTable, SegCount);

  //从第loc个往前移
  for (int i = Idx; i < SegCount; i++) {
    int CurIdx = i, NextIdx = CurIdx + 1;
    pMapTable->endCode[CurIdx] = pMapTable->endCode[NextIdx];
    pMapTable->startCode[CurIdx] = pMapTable->startCode[NextIdx];
    pMapTable->idDelta[CurIdx] = pMapTable->idDelta[NextIdx];
    pMapTable->idRangeOffset[CurIdx] = pMapTable->idRangeOffset[NextIdx];
    pMapTable->IndexArray[CurIdx] = pMapTable->IndexArray[NextIdx];
  }
}

//往格式6的映射表里加入字符码
void AppendMapTable(EncodingMapFORMAT6* pMapTable, int FirstIndex, USHORT CharArr[], int IndexCount)
{

}

void cmapTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  const maxpTableINFO* pmaxp = (maxpTableINFO*)GetTableInfo("maxp");
  int FirstIndex = pmaxp->GetNumberOfGlyphs();
  const maxpTableINFO* pmaxpAdded = (maxpTableINFO*)pAddedDoc->GetTableInfo("maxp");
  int IndexCount = pmaxpAdded->GetNumberOfGlyphs();

  //Param存放一个ushort数组指明每个字符的unicode码
  USHORT* CharArr = (USHORT*)Param;
  USHORT* NewArr = (USHORT*)_alloca(sizeof(USHORT) * IndexCount);

  const cmapTableINFO* pcmapAdded = (cmapTableINFO*)pAddedDoc->GetTableInfo("cmap");

  for (int i = 0; i < TableNumber; i++) {
    if (MapEntries[i].CodingMapId == i) {
      USHORT FirstCode = 0;

      if (MapTableInfo[i]->Format > 0) {
        //如果为简体中文字符集先转换unicode为gb码
        if (MapEntries[i].PlatformId == 1 && MapEntries[i].EncodingId == 0x19) {
          ///??
          for (int j = 0; j < IndexCount; j++) {
            if (CharArr[j] != 0) {
              int Rt = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(CharArr + j), 1, LPSTR(NewArr + j), 2, NULL, NULL);

              //在读文件时已经把文件里的顺序big endian转换成了small endian所以下面必须把
              //WideCharToMultiByte转换得到的big endian再变成small endian以便于比较大小xd
              if (CharArr[j] & 0xff00) { //最开始的128个字符gb码是一个字节
                NewArr[j] = ReverseWord(NewArr[j]);
              }
            }
            else {
              NewArr[j] = 0;
            }
          }

          CharArr = NewArr;
        }
        else {
          CharArr = (USHORT*)Param;
        }
      }

      switch (MapTableInfo[i]->Format) {
      case 2:
        AppendMapTable((EncodingMapFORMAT2*)MapTableInfo[i], FirstIndex, CharArr, IndexCount);
        break;

      case 4:
        AppendMapTable((EncodingMapFORMAT4*)MapTableInfo[i], FirstIndex, CharArr, IndexCount);
        break;

      case 6:
        AppendMapTable((EncodingMapFORMAT6*)MapTableInfo[i], FirstIndex, CharArr, IndexCount);
        break;
      }
    }
  }
}

void cmapTableINFO::Duplicate(const TableINFO* pTable)
{
  cmapTableINFO* pCmap = (cmapTableINFO*)pTable;
  TableVersion = pCmap->TableVersion;
  TableNumber = pCmap->TableNumber;
  MapEntries = (EncodingMapENTRY*)memdup(pCmap->MapEntries, sizeof(EncodingMapENTRY) * TableNumber);
  MapTableInfo = (FormatHEADER**)malloc(sizeof(FormatHEADER*) * TableNumber);

  for (int i = 0; i < TableNumber; i++) {
    if (MapEntries[i].CodingMapId != i) { //不等表示与其他入口指向同一个MapTable
      MapTableInfo[i] = MapTableInfo[MapEntries[i].CodingMapId];
      continue;
    }

    switch (pCmap->MapTableInfo[i]->Format) {
    case 0: {
      MapTableInfo[i] = (FormatHEADER*)memdup(pCmap->MapTableInfo[i], sizeof(EncodingMapFORMAT0));
      break;
    }

    case 2: {
      EncodingMapFORMAT2* pSrc = (EncodingMapFORMAT2*)pCmap->MapTableInfo[i];
      EncodingMapFORMAT2* pFormat = (EncodingMapFORMAT2*)memdup(pSrc, sizeof(EncodingMapFORMAT2));
      MapTableInfo[i] = (FormatHEADER*)pFormat;

      int Len = (pFormat->SubHeaderKeys[255] >> 3) + 1;
      pFormat->SubHeaders = (SubHEADER*)memdup(pSrc->SubHeaders, Len * sizeof(SubHEADER));
      pFormat->IndexArray = (USHORT**)malloc(sizeof(USHORT*) * Len);

      for (int i = 0; i < Len; i++) {
        pFormat->IndexArray[i] = (USHORT*)memdup(pSrc->IndexArray[i], sizeof(USHORT) * pFormat->SubHeaders[i].entryCount);
      }

      break;
    }

    case 4: {
      EncodingMapFORMAT4* pSrc = (EncodingMapFORMAT4*)pCmap->MapTableInfo[i];
      EncodingMapFORMAT4* pFormat = (EncodingMapFORMAT4*)memdup(pSrc, sizeof(EncodingMapFORMAT4));
      MapTableInfo[i] = (FormatHEADER*)pFormat;

      int Len = pSrc->segCountX2 >> 1;
      pFormat->endCode = (USHORT*)memdup(pSrc->endCode, Len * sizeof(USHORT));
      pFormat->startCode = (USHORT*)memdup(pSrc->startCode, Len * sizeof(USHORT));
      pFormat->idDelta = (USHORT*)memdup(pSrc->idDelta, Len * sizeof(USHORT));
      pFormat->idRangeOffset = (USHORT*)memdup(pSrc->idRangeOffset, Len * sizeof(USHORT));
      pFormat->IndexArray = (USHORT**)malloc(sizeof(USHORT*) * Len);

      for (int i = 0; i < Len; i++) {
        if (pSrc->IndexArray[i]) {
          int Count = pFormat->endCode[i] - pFormat->startCode[i] + 1;
          pFormat->IndexArray[i] = (USHORT*)memdup(pSrc->IndexArray[i], sizeof(USHORT) * Count);
        }
        else {
          pFormat->IndexArray[i] = NULL;
        }
      }

      break;
    }

    case 6: {
      EncodingMapFORMAT6* pSrc = (EncodingMapFORMAT6*)pCmap->MapTableInfo[i];
      EncodingMapFORMAT6* pFormat = (EncodingMapFORMAT6*)memdup(pSrc, sizeof(EncodingMapFORMAT6));
      MapTableInfo[i] = (FormatHEADER*)pFormat;
      pFormat->glyphIdArray = (USHORT*)memdup(pSrc->glyphIdArray, sizeof(USHORT) * pSrc->entryCount);
      break;
    }
    }
  }
}
// class cmapTableINFO End

// class glyfTableINFO Begin
glyfTableINFO::glyfTableINFO() : glyfHeader(NULL), glyfInfo(NULL) {}

glyfTableINFO::~glyfTableINFO()
{
  for (int i = 0; i < glyfCount; i++) {
    if (glyfHeader[i].NumberOfContours >= 0) {
      //It's a simple format;
      if (glyfInfo[i]) {
        glyfSimpleFORMAT* pFormat = (glyfSimpleFORMAT*)glyfInfo[i];
        free(pFormat->EndPtsOfContours);
        free(pFormat->Instructions);
        free(pFormat->Flags);
        free(pFormat->xCoordinates);
        free(pFormat->yCoordinates);
        free(pFormat->pxFormat);
        free(pFormat->pyFormat);
        free(pFormat);
      }
    }
    else {
      //It's a composite format;
      glyfCompositeFORMAT* pFormat = (glyfCompositeFORMAT*)glyfInfo[i];

      free(pFormat->pStructList);
      free(pFormat->Instructions);
      free(pFormat);
    }
  }

  free(glyfInfo);
  free(glyfHeader);
}

//计算简单字形格式的点数
int glyfTableINFO::GetPtCountOfSimpleGlyph(int NumberOfContours, const glyfSimpleFORMAT* pFormat) const
{
  assert(NumberOfContours > 0);

  //根据EndPtsOfContours算出点数
  int PtCount = 0;

  for (int i = 0; i < NumberOfContours; i++) {
    if (PtCount < pFormat->EndPtsOfContours[i]) {
      PtCount = pFormat->EndPtsOfContours[i];
    }
  }

  return PtCount + 1;
}

void glyfTableINFO::ReadFlagsAndPoints(const glyfHEADER* pHeader, glyfSimpleFORMAT* pFormat, FILE* pFile)
{
  int i, PtCount = GetPtCountOfSimpleGlyph(pHeader->NumberOfContours, pFormat);

  //根据点数读取flags,xCoordinates,yCoordinates
  int Pos = ftell(pFile);
  pFormat->Flags = (BYTE*)malloc(PtCount);
  READFILE(pFormat->Flags, PtCount, pFile);
  char* x = pFormat->pxFormat = (char*)malloc(PtCount + 1);
  char* y = pFormat->pyFormat = (char*)malloc(PtCount + 1);

  //分析flag
  //Each flag is a single byte. Their meanings are shown below. Flags Bit Description :
  //bit0: On Curve, If set, the point is on the curve; otherwise, it is off the curve.
  //bit1: x-Short Vector, If set, the corresponding x-coordinate is 1 byte long, not 2.
  //bit2: y-Short Vector, If set, the corresponding y-coordinate is 1 byte long, not 2.
  //bit3: Repeat, If set, the next byte specifies the number of additional times this set of flags
  // is to be repeated. In this way, the number of flags listed can be smaller than the number
  // of points in a character.
  //bit4: This x is same, (Positive x-Short Vector), This flag has two meanings, depending on how
  // the x-Short Vector flag is set. If x-Short Vector is set, this bit describes the sign of
  // the value, with 1 equalling positive and 0 negative. If the x-Short Vector bit is not set
  // and this bit is set, then the current x-coordinate is the same as the previous x-coordinate.
  // If the x-Short Vector bit is not set and this bit is also not set, the current x-coordinate
  // is a signed 16-bit delta vector.
  //bit5: This y is same, (Positive y-Short Vector), as bit4.
  //Reserved 6 This bit is reserved. Set it to zero.
  //Reserved 7 This bit is reserved. Set it to zero.

  BYTE* p = pFormat->Flags;

  for (i = 0; i < PtCount; p++, i++) {
    int Flag = *p;

    if (Flag & 0x2) {
      *x++ = 'B';
    }
    else {
      //两字节表示
      if (Flag & 0x10)
        ; //跟前面值相同，取前面的值,ttf文件已省略
      else {
        *x++ = 'W';
      }
    }

    if (Flag & 0x4) {
      *y++ = 'B';
    }
    else {
      //两字节表示
      if (Flag & 0x20)
        ; //跟前面值相同，取前面的值,ttf文件已省略
      else {
        *y++ = 'W';
      }
    }

    if (Flag & 0x8) {
      p++;
      i += *p;

      if (Flag & 0x2 || !(Flag & 0x10)) {
        char xCh = *(x - 1);

        for (int j = 0; j < *p; j++) {
          *x++ = xCh;
        }
      }

      if (Flag & 0x4 || !(Flag & 0x20)) {
        int yCh = *(y - 1);

        for (int k = 0; k < *p; k++) {
          *y++ = yCh;
        }
      }
    }
  }

  *x = '\0';
  *y = '\0';
  pFormat->LengthOfFlags = p - pFormat->Flags; //falgs数组的长度为p - pFlagBuf;

  fseek(pFile, Pos + pFormat->LengthOfFlags, SEEK_SET);
  int FrameLength = GetDataFrameLength(pFormat->pxFormat);
  pFormat->xCoordinates = malloc(FrameLength);
  ttfReadFile(pFormat->xCoordinates, pFormat->pxFormat, 1, pFile);

  FrameLength = GetDataFrameLength(pFormat->pyFormat);
  pFormat->yCoordinates = malloc(FrameLength);
  ttfReadFile(pFormat->yCoordinates, pFormat->pyFormat, 1, pFile);
}

glyfSimpleFORMAT* glyfTableINFO::CreateAndReadGlyfInfo(const glyfHEADER* pHeader, FILE* pFile)
{
  if (pHeader->NumberOfContours >= 0) {
    //It's a simple format;
    glyfSimpleFORMAT* pFormat = (glyfSimpleFORMAT*)malloc(sizeof(glyfSimpleFORMAT));
    pFormat->EndPtsOfContours = (USHORT*)malloc(sizeof(USHORT) * pHeader->NumberOfContours);
    ttfReadFile(pFormat->EndPtsOfContours, "W" , pHeader->NumberOfContours, pFile);

    pFormat->InstructionLength = ttfGetUShort(pFile);
    pFormat->Instructions = (BYTE*)malloc(pFormat->InstructionLength);
    READFILE(pFormat->Instructions, pFormat->InstructionLength, pFile);

    ReadFlagsAndPoints(pHeader, pFormat, pFile);
    return pFormat;
  }

  else {
    //It's a composite format;
    glyfCompositeSTRUCT* pCompositeStruct;
    glyfCompositeFORMAT* pFormat = (glyfCompositeFORMAT*)malloc(sizeof(glyfCompositeFORMAT));
    memset(pFormat, 0, sizeof(glyfCompositeFORMAT));

    do {
      pFormat->pStructList = (glyfCompositeSTRUCT*)my_memins(pFormat->pStructList, pFormat->nStructList, pFormat->nStructList, 1, sizeof(glyfCompositeSTRUCT));
      pCompositeStruct = pFormat->pStructList + pFormat->nStructList;
      memset(pCompositeStruct, 0, sizeof(glyfCompositeSTRUCT));
      pFormat->nStructList++;
      pCompositeStruct->Flag = ttfGetShort(pFile);
      pCompositeStruct->glyphIndex = ttfGetShort(pFile);

      pCompositeStruct->Argument = ttfGetShort(pFile);

      if (pCompositeStruct->Flag & ARG_1_AND_2_ARE_WORDS) {
        pCompositeStruct->Argument2 = ttfGetShort(pFile);
      }

      if (pCompositeStruct->Flag & WE_HAVE_A_SCALE) {
        pCompositeStruct->xScale = ttfGetShort(pFile);
      }
      else if (pCompositeStruct->Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
        pCompositeStruct->xScale = ttfGetWord(pFile);
        pCompositeStruct->yScale = ttfGetWord(pFile);
      }
      else if (pCompositeStruct->Flag & WE_HAVE_A_TWO_BY_TWO) {
        pCompositeStruct->xScale = ttfGetWord(pFile);
        pCompositeStruct->Scale01 = ttfGetWord(pFile);
        pCompositeStruct->Scale10 = ttfGetWord(pFile);
        pCompositeStruct->yScale = ttfGetWord(pFile);
      }
    }
    while (pCompositeStruct->Flag & MORE_COMPONENTS);

    if (pCompositeStruct->Flag & WE_HAVE_INSTRUCTIONS) {
      pFormat->NumberOfInstr = ttfGetShort(pFile);
      pFormat->Instructions = (BYTE*)malloc(pFormat->NumberOfInstr);
      ttfReadFile(pFormat->Instructions, "B", pFormat->NumberOfInstr, pFile);
    }
    else {
      pFormat->NumberOfInstr = 0;
      pFormat->Instructions = NULL;
    }

    return (glyfSimpleFORMAT*)pFormat;
  }
}

void glyfTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  maxpTableINFO* pmaxpTableInfo = (maxpTableINFO*)GetTableInfo("maxp", pFile);
  glyfCount = pmaxpTableInfo->GetNumberOfGlyphs();

  locaTableINFO* plocaTableInfo = (locaTableINFO*)GetTableInfo("loca", pFile);

  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  glyfHeader = (glyfHEADER*)malloc(sizeof(glyfHEADER) * glyfCount);

  glyfInfo = (glyfSimpleFORMAT**)malloc(sizeof(glyfSimpleFORMAT*) * glyfCount);
  memset(glyfInfo, 0, sizeof(glyfSimpleFORMAT*) * glyfCount);

  for (int i = 0; i < glyfCount; i++) {
    int CurOffset = plocaTableInfo->GetOffset(i);
    int NextOffset = plocaTableInfo->GetOffset(i + 1);
#ifdef _DEBUG
    int Pos = ftell(pFile);
    assert(Pos + 3 >= (int)pTableEntry->Offset + (int)CurOffset && Pos <= (int)pTableEntry->Offset + (int)CurOffset);
#endif

    //If the font does not contain an outline for the missing character,
    //then the first and second offsets should have the same value.
    //This also applies to any other character without an outline, such as the space character.
    if (CurOffset != NextOffset) {
      fseek(pFile, pTableEntry->Offset + CurOffset, SEEK_SET);
      ttfReadFile(glyfHeader + i, "WWWWW", 1, pFile);
      glyfInfo[i] = CreateAndReadGlyfInfo(glyfHeader + i, pFile);
    }
    else {
      memset(glyfHeader + i, 0, sizeof(glyfHEADER));
    }
  }
}

void glyfTableINFO::WriteGlyfInfo(const glyfSimpleFORMAT* pGlyfInfo, const glyfHEADER* pHeader, FILE* pFile)
{
  if (pHeader->NumberOfContours >= 0) {
    //It's a simple format;
    ttfWriteFile(pGlyfInfo->EndPtsOfContours, "W", pHeader->NumberOfContours, pFile);
    ttfPutUShort(pGlyfInfo->InstructionLength, pFile);
    ttfWriteFile(pGlyfInfo->Instructions, "B", pGlyfInfo->InstructionLength, pFile);

    WRITEFILE(pGlyfInfo->Flags, pGlyfInfo->LengthOfFlags, pFile);
    ttfWriteFile(pGlyfInfo->xCoordinates, pGlyfInfo->pxFormat, 1, pFile);
    ttfWriteFile(pGlyfInfo->yCoordinates, pGlyfInfo->pyFormat, 1, pFile);
  }
  else {
    //It's a composite format;
    glyfCompositeFORMAT* pFormat = (glyfCompositeFORMAT*)pGlyfInfo;
    int i;
    glyfCompositeSTRUCT* pCompositeStruct = NULL;

    for (i = 0; i < pFormat->nStructList; ++i) {
      pCompositeStruct = pFormat->pStructList + i;
      ttfPutUShort(pCompositeStruct->Flag, pFile);
      ttfPutUShort(pCompositeStruct->glyphIndex, pFile);

      ttfPutUShort(pCompositeStruct->Argument, pFile);

      if (pCompositeStruct->Flag & ARG_1_AND_2_ARE_WORDS) {
        ttfPutUShort(pCompositeStruct->Argument2, pFile);
      }

      if (pCompositeStruct->Flag & WE_HAVE_A_SCALE) {
        ttfPutUShort(pCompositeStruct->xScale, pFile);
      }
      else if (pCompositeStruct->Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
        ttfPutWord(pCompositeStruct->xScale, pFile);
        ttfPutWord(pCompositeStruct->yScale, pFile);
      }
      else if (pCompositeStruct->Flag & WE_HAVE_A_TWO_BY_TWO) {
        ttfPutWord(pCompositeStruct->xScale, pFile);
        ttfPutWord(pCompositeStruct->Scale01, pFile);
        ttfPutWord(pCompositeStruct->Scale10, pFile);
        ttfPutWord(pCompositeStruct->yScale, pFile);
      }

    }

    assert(pCompositeStruct);

    if (pCompositeStruct->Flag & WE_HAVE_INSTRUCTIONS) {
      ttfPutUShort(pFormat->NumberOfInstr, pFile);
      ttfWriteFile(pFormat->Instructions, "B", pFormat->NumberOfInstr, pFile);
    }
  }
}

void glyfTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  locaTableINFO* plocaTableInfo = (locaTableINFO*)GetTableInfo("loca");
  int i;

  for (i = 0; i < glyfCount; i++) {
    int Offset = ftell(pFile) - pTableEntry->Offset;
    plocaTableInfo->SetOffset(i, Offset);

    if (*(glyfInfo + i)) {
      ttfWriteFile(glyfHeader + i, "WWWWW", 1, pFile);
      WriteGlyfInfo(*(glyfInfo + i), glyfHeader + i, pFile);

      Offset = ftell(pFile) - pTableEntry->Offset;
      Offset = ((Offset + 1) & ~1) - Offset; //边界2字节对齐

      if (Offset > 0) {
        WRITEFILE(&Zero, Offset, pFile);
      }
    }
  }

  plocaTableInfo->SetOffset(i, ftell(pFile) - pTableEntry->Offset);
}

//从pAddedDoc里把glyph追加到后面
void glyfTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  glyfTableINFO* pglyfAdded = (glyfTableINFO*)pAddedDoc->GetTableInfo("glyf");
  headTableINFO* pheadAdded = (headTableINFO*)pAddedDoc->GetTableInfo("head");
  OS2TableINFO* pOS2Added = (OS2TableINFO*)pAddedDoc->GetTableInfo("OS/2");
  headTableINFO* pheadThis = (headTableINFO*)GetTableInfo("head");
  OS2TableINFO* pOS2This = (OS2TableINFO*)GetTableInfo("OS/2");

  double Scaling = double(pheadThis->GetUnitsPerEm()) / double(pheadAdded->GetUnitsPerEm());
  int vShift = (int)(pOS2Added->GetWinDescent() * Scaling - pOS2This->GetWinDescent()); //垂直方向的偏移
  int hShift = 0;//pheadThis->GetxMin() - pheadAdded->GetxMin() * Scaling; ///??水平方向的偏移

  int AddedGlyfCount = pglyfAdded->glyfCount;
  glyfHeader = (glyfHEADER*)realloc(glyfHeader, sizeof(glyfHEADER) * (glyfCount + AddedGlyfCount));
  glyfInfo = (glyfSimpleFORMAT**)realloc(glyfInfo, sizeof(glyfSimpleFORMAT*) * (glyfCount + AddedGlyfCount));

  for (int i = glyfCount; i < glyfCount + AddedGlyfCount; i++) {
    memmove(glyfHeader + i, pglyfAdded->glyfHeader + (i - glyfCount), sizeof(glyfHEADER));

    if (pglyfAdded->glyfInfo[i - glyfCount]) {
      glyfInfo[i] = CreateAndCopyGlyfInfo(glyfHeader + i, pglyfAdded->glyfInfo[i - glyfCount], Scaling, hShift, vShift, glyfCount);
      glyfHeader[i].xMin = (int)(glyfHeader[i].xMin * Scaling + hShift);
      glyfHeader[i].xMax = (int)(glyfHeader[i].xMax * Scaling + hShift);
      glyfHeader[i].yMin = (int)(glyfHeader[i].yMin * Scaling + vShift);
      glyfHeader[i].yMax = (int)(glyfHeader[i].yMax * Scaling + vShift);
    }
    else {
      glyfInfo[i] = NULL;
    }
  }

  glyfCount += AddedGlyfCount;
}

void ZoomInCoordinate(void*& pGlyfCoor, char* pType, double Scaling)
{
  //因为单字节的坐标放大后可能超过一个字节的存储范围，这里简单处理把每个BYTE转换成SHORT
  void* pCoor = pGlyfCoor;
  SHORT* pNewCoor = (SHORT*)malloc(sizeof(SHORT) * strlen(pType));
  SHORT* pCoorAddr = pNewCoor;

  for (char* p = pType; *p; p++) {
    if (*p == 'B') {
      *p = 'W'; //格式设为SHORT
      *pNewCoor = (SHORT)floor(*((BYTE*)pCoor) * Scaling + 0.5); //四舍五入
      pCoor = (BYTE*)pCoor + 1;
    }
    else {
      *pNewCoor = (SHORT)floor(*((SHORT*)pCoor) * Scaling + 0.5); //四舍五入
      pCoor = (SHORT*)pCoor + 1;
    }

    pNewCoor ++;
  }

  free(pGlyfCoor);
  pGlyfCoor = pCoorAddr;
}

//缩放后调整flag和坐标符号
void AdjustFlagsAndCoors(SHORT* pNewXCoor, SHORT* pNewYCoor, BYTE* pFlags, int FlagCount)
{
  for (BYTE* p = pFlags; p - pFlags < FlagCount; p++) {
    BYTE Flag = *p, Tip = 0x9;

    if (Flag & 0x2) {
      if (!(Flag & 0x10)) { //第五位表示x符号
        *pNewXCoor = -*pNewXCoor;
      }

      pNewXCoor++;
    }
    else {
      //两字节表示
      if (Flag & 0x10) {
        Tip |= 0x10; //跟前面值相同，取前面的值,ttf文件已省略
      }
      else {
        pNewXCoor++;
      }
    }

    if (Flag & 0x4) {
      if (!(Flag & 0x20)) { //第六位表示y符号
        *pNewYCoor = -*pNewYCoor;
      }

      pNewYCoor++;
    }
    else {
      //两字节表示
      if (Flag & 0x20) {
        Tip |= 0x20; //跟前面值相同，取前面的值,ttf文件已省略
      }
      else {
        pNewYCoor++;
      }
    }

    *p = Flag & Tip; //调整flag始终保留OnCurve和Repeat位，当取得前面的坐标值时相应的位也要保留

    if (Flag & 0x8) { //设置了重复位
      p++;

      for (int j = 0; j < *p; j++) { //*p表示flag重复次数
        if (Flag & 0x2) {
          if (!(Flag & 0x10)) { //第五位表示x符号
            *pNewXCoor = -*pNewXCoor;
          }

          pNewXCoor++;
        }
        else {
          //两字节表示
          if (!(Flag & 0x10)) {
            pNewXCoor++;
          }
        }

        if (Flag & 0x4) {
          if (!(Flag & 0x20)) { //第六位表示y符号
            *pNewYCoor = -*pNewYCoor;
          }

          pNewYCoor++;
        }
        else {
          //两字节表示
          if (!(Flag & 0x20)) {
            pNewYCoor++;
          }
        }
      }
    }
  }
}

//移动坐标点，注意只需在第一个点要加上相应的偏移
void ShiftCoordinate(BYTE*& pFlag, SHORT*& pCoor, char*& pFormat, BYTE SizeBit, BYTE SignBit, int Shift)
{
  assert(SizeBit == 0x2 && SignBit == 0x10 || SizeBit == 0x4 && SignBit == 0x20);
  assert(!(*pFlag & SizeBit)); //两字节表示

  if (*pFlag & SignBit) {
    int Count = strlen(pFormat);
    //如果第一个Flag重复了，必须添加相应个数的坐标和类型W
    int Delta = *pFlag & 0x8 ? *(pFlag + 1) + 1 : 1;
    pCoor = (SHORT*)realloc(pCoor, sizeof(SHORT) * (Count + Delta));
    memmove(pCoor + Delta, pCoor, sizeof(SHORT) * Count);
    pFormat = (char*)realloc(pFormat, Count + Delta + 1);
    memmove(pFormat + Delta, pFormat, Count + 1);

    for (int i = 0; i < Delta; i++) {
      pFormat[i] = 'W';
      pCoor[i] = 0;
    }

    pCoor[0] = Shift;

    *pFlag &= ~SignBit;
  }
  else {
    *pCoor += Shift;
  }
}

void ZoomGlyf(glyfSimpleFORMAT* pGlyf, double Scaling, int xShift, int yShift)
{
  if (fabs(Scaling - 1.0) < 1e-4 && xShift == 0 && yShift == 0) {
    return;
  }

  //因为单字节的坐标放大后可能超过一个字节的存储范围，这里简单处理把每个BYTE转换成SHORT
  ZoomInCoordinate(pGlyf->xCoordinates, pGlyf->pxFormat, Scaling);
  ZoomInCoordinate(pGlyf->yCoordinates, pGlyf->pyFormat, Scaling);

  AdjustFlagsAndCoors((SHORT*)pGlyf->xCoordinates, (SHORT*)pGlyf->yCoordinates, pGlyf->Flags,
      pGlyf->LengthOfFlags);

  if (xShift != 0) {
    SHORT* pCoor = (SHORT*)pGlyf->xCoordinates;
    ShiftCoordinate(pGlyf->Flags, pCoor, pGlyf->pxFormat, 0x2, 0x10, xShift);
    pGlyf->xCoordinates = pCoor;
  }

  if (yShift != 0) {
    SHORT* pCoor = (SHORT*)pGlyf->yCoordinates;
    ShiftCoordinate(pGlyf->Flags, pCoor, pGlyf->pyFormat, 0x4, 0x20, yShift);
    pGlyf->yCoordinates = pCoor;
  }
}

glyfSimpleFORMAT* glyfTableINFO::CreateAndCopyGlyfInfo(const glyfHEADER* pHeader, const glyfSimpleFORMAT* pCopiedGlyf, double Scaling,
    int xShift, int yShift, int glyfDelta)
{
  if (pHeader->NumberOfContours >= 0) {
    //It's a simple format;
    glyfSimpleFORMAT* pFormat = (glyfSimpleFORMAT*)malloc(sizeof(glyfSimpleFORMAT));
    pFormat->EndPtsOfContours = (USHORT*)memdup(pCopiedGlyf->EndPtsOfContours, sizeof(USHORT) * pHeader->NumberOfContours);

    pFormat->InstructionLength = pCopiedGlyf->InstructionLength;
    pFormat->Instructions = (BYTE*)memdup(pCopiedGlyf->Instructions, pFormat->InstructionLength);

    pFormat->LengthOfFlags = pCopiedGlyf->LengthOfFlags;
    pFormat->Flags = (BYTE*)memdup(pCopiedGlyf->Flags, pFormat->LengthOfFlags);
    pFormat->pxFormat = strdup(pCopiedGlyf->pxFormat);
    pFormat->pyFormat = strdup(pCopiedGlyf->pyFormat);

    pFormat->xCoordinates = memdup(pCopiedGlyf->xCoordinates, GetDataFrameLength(pFormat->pxFormat));
    pFormat->yCoordinates = memdup(pCopiedGlyf->yCoordinates, GetDataFrameLength(pFormat->pyFormat));
    ZoomGlyf(pFormat, Scaling, xShift, yShift);
    return pFormat;
  }

  else {
    //It's a composite format;
    glyfCompositeSTRUCT* pCompositeStruct;
    glyfCompositeFORMAT* pFormat = (glyfCompositeFORMAT*)malloc(sizeof(glyfCompositeFORMAT));
    glyfCompositeFORMAT* pFormatIn = (glyfCompositeFORMAT*)pCopiedGlyf;
    memset(pFormat, 0, sizeof(glyfCompositeFORMAT));
    int i;
    pFormat->pStructList = (glyfCompositeSTRUCT*)malloc(sizeof(glyfCompositeSTRUCT) * pFormatIn->nStructList);

    for (i = 0; i < pFormatIn->nStructList; ++i) {
      pCompositeStruct = pFormatIn->pStructList + i;
      glyfCompositeSTRUCT* pNewStruct = pFormat->pStructList + i;
      memcpy(pNewStruct, pCompositeStruct, sizeof(glyfCompositeSTRUCT));

      //调整组件字形的索引
      pNewStruct->glyphIndex += glyfDelta;

      //对组件偏移x,y的值做相应的调整
      if (pNewStruct->Flag & ARGS_ARE_XY_VALUES) {
        if (pNewStruct->Flag & ARG_1_AND_2_ARE_WORDS) {
          pNewStruct->Argument = (int)(pNewStruct->Argument * Scaling);
          pNewStruct->Argument2 = (int)(pNewStruct->Argument2 * Scaling);
        }
        else {
          char Low = (pNewStruct->Argument & 0xff);
          char Hight = (pNewStruct->Argument >> 8) & 0xff;

          if (Scaling <= 1.0) {
            Low = (int)(Low * Scaling);
            Hight = (int)(Hight * Scaling);
            pNewStruct->Argument = (Hight << 8) | Low;
          }
          else { //放大，为了防止溢出，变成WORD类型
            pNewStruct->Argument = (int)(Hight * Scaling);
            pNewStruct->Argument2 = (int)(Low * Scaling);
            pNewStruct->Flag |= ARG_1_AND_2_ARE_WORDS;
          }
        }
      }

    }

    if (pCompositeStruct->Flag & WE_HAVE_INSTRUCTIONS) {
      pFormat->NumberOfInstr = ((glyfCompositeFORMAT*)pCopiedGlyf)->NumberOfInstr;
      pFormat->Instructions = (BYTE*)memdup(((glyfCompositeFORMAT*)pCopiedGlyf)->Instructions, pFormat->NumberOfInstr);
    }
    else {
      pFormat->NumberOfInstr = 0;
      pFormat->Instructions = NULL;
    }

    return (glyfSimpleFORMAT*)pFormat;
  }
}

void glyfTableINFO::Duplicate(const TableINFO* pTable)
{
  glyfTableINFO* pGlyf = (glyfTableINFO*)pTable;
  glyfCount = pGlyf->glyfCount;
  glyfHeader = (glyfHEADER*)memdup(pGlyf->glyfHeader, glyfCount * sizeof(glyfHEADER));
  glyfInfo = (glyfSimpleFORMAT**)malloc(sizeof(glyfSimpleFORMAT*) * glyfCount);

  for (int i = 0; i < glyfCount; i++) {
    if (pGlyf->glyfInfo[i]) {
      glyfInfo[i] = CreateAndCopyGlyfInfo(glyfHeader + i, pGlyf->glyfInfo[i], 1, 0, 0);
    }
    else {
      glyfInfo[i] = NULL;
    }
  }
}

void glyfTableINFO::GetSimpleGlyph(ExportGlyfSTRUCT* pExport, int Index) const
{
  assert(Index < glyfCount && glyfHeader[Index].NumberOfContours > 0);

  if (glyfHeader[Index].NumberOfContours > 0) {
    glyfSimpleFORMAT* pSimple = glyfInfo[Index];
    pExport->EndPtOfContours = (USHORT*)memdup(pSimple->EndPtsOfContours, sizeof(USHORT) * glyfHeader[Index].NumberOfContours);
    pExport->PtCount = GetPtCountOfSimpleGlyph(glyfHeader[Index].NumberOfContours, glyfInfo[Index]);
    pExport->pPoints = (GlyfPOINT*)malloc(sizeof(GlyfPOINT) * pExport->PtCount);
    pExport->Flags = (BYTE*)malloc(sizeof(BYTE) * pExport->PtCount);
    pExport->ContourCount = glyfHeader[Index].NumberOfContours;

    void* pXCoor = pSimple->xCoordinates;
    void* pYCoor = pSimple->yCoordinates;
    GlyfPOINT* pPtx = pExport->pPoints;
    GlyfPOINT* pPty = pExport->pPoints;
    BYTE* pFlag = pExport->Flags;

    for (BYTE* p = pSimple->Flags; (p - pSimple->Flags) < pSimple->LengthOfFlags; p++) {
      int Repeat;
      BYTE Flag = *p;

      if (Flag & 0x8) { //设置了重复位
        p++;
        Repeat = *p + 1;//*p表示flag重复次数
      }
      else {
        Repeat = 1;
      }

      for (int j = 0; j < Repeat; j++) {
        *pFlag++ = Flag & 0x1;

        if (Flag & 0x2) {
          //取该点的x坐标，注意后续点是相对于前面点的坐标的,以下同
          pPtx->x = (pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x) + ((Flag & 0x10) ? * ((BYTE*)pXCoor) : - * ((BYTE*)pXCoor));
          pXCoor = (BYTE*)pXCoor + 1;
        }
        else {
          //两字节表示
          if (Flag & 0x10) {
            pPtx->x = pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x; //跟前面值相同，取前面的值
          }
          else {
            pPtx->x = (pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x) + *((SHORT*)pXCoor);
            pXCoor = (SHORT*)pXCoor + 1;
          }
        }

        pPtx++;

        if (Flag & 0x4) {
          pPty->y = (pPty == pExport->pPoints ? 0 : (pPty - 1)->y) + ((Flag & 0x20) ? * ((BYTE*)pYCoor) : - * ((BYTE*)pYCoor));
          pYCoor = (BYTE*)pYCoor + 1;
        }
        else {
          //两字节表示
          if (Flag & 0x20) {
            pPty->y = pPty == pExport->pPoints ? 0 : (pPty - 1)->y; //跟前面值相同，取前面的值
          }
          else {
            pPty->y = (pPty == pExport->pPoints ? 0 : (pPty - 1)->y) + *((SHORT*)pYCoor);
            pYCoor = (SHORT*)pYCoor + 1;
          }
        }

        pPty++;
      }
    }
  }
}

void glyfTableINFO::GetGlyph(ExportGlyfSTRUCT* pExport, int Index) const
{
  assert(Index < glyfCount);

  if (glyfHeader[Index].NumberOfContours > 0) {
    //简单字形
    GetSimpleGlyph(pExport, Index);
  }
  else {
    int i;
    //组合字形
    glyfCompositeFORMAT* pFormat = (glyfCompositeFORMAT*)glyfInfo[Index];

    if (!pFormat) {
      memset(pExport, 0, sizeof(ExportGlyfSTRUCT));
      return;
    }

    for (i = 0; i < pFormat->nStructList; ++i) {
      ExportGlyfSTRUCT GlyfStruct;
      glyfCompositeSTRUCT* pCompositeStruct = pFormat->pStructList + i;
      GetSimpleGlyph(&GlyfStruct, pCompositeStruct->glyphIndex);

      //调整点的坐标
      int Arg1, Arg2;

      if (pCompositeStruct->Flag & ARG_1_AND_2_ARE_WORDS) {
        Arg1 = (SHORT)pCompositeStruct->Argument;
        Arg2 = (SHORT)pCompositeStruct->Argument2;
      }
      else {
        Arg1 = (char)pCompositeStruct->Argument >> 8;
        Arg2 = (char)pCompositeStruct->Argument & 0xff;
      }

      GlyfPOINT* Pts = GlyfStruct.pPoints;

      for (int i = 0; i < GlyfStruct.PtCount; i++) {
        if (pCompositeStruct->Flag & ARGS_ARE_XY_VALUES) {
          Pts[i].x += Arg1;
          Pts[i].y += Arg2;
        }
        else {
          assert(0);///??两点怎么匹配
        }

        if (pCompositeStruct->Flag & WE_HAVE_A_SCALE) {
          Pts[i].x *= F2DOT14ToDouble(pCompositeStruct->xScale);
          Pts[i].y *= F2DOT14ToDouble(pCompositeStruct->xScale);
        }
        else if (pCompositeStruct->Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
          Pts[i].x *= F2DOT14ToDouble(pCompositeStruct->xScale);
          Pts[i].y *= F2DOT14ToDouble(pCompositeStruct->yScale);
        }
        else if (pCompositeStruct->Flag & WE_HAVE_A_TWO_BY_TWO) {
          assert(0);///??不明白文档意思
        }
      }

      //合并到输出的字形结构体中
      GlyphCat(pExport, &GlyfStruct);
    }
  }
}
// class glyfTableINFO End

// class headTableINFO Begin
void headTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  ttfReadFile(&HeadTable, "DDDDWWDDDDWWWWWWWWW", 1, pFile);
}

void headTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile(&HeadTable, "DDDDWWDDDDWWWWWWWWW", 1, pFile);
}

void headTableINFO::SetCheckSumAdjustment(ULONG Val)
{
  HeadTable.CheckSumAdjustment = Val;
}

SHORT headTableINFO::GetIndexToLocFormat() const
{
  return HeadTable.IndexToLocFormat;
}

void headTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  const headTableINFO* pheadAdded = (headTableINFO*)pAddedDoc->GetTableInfo("head");
  HeadTable.xMax = MAX(HeadTable.xMax, pheadAdded->HeadTable.xMax);
  HeadTable.yMax = MAX(HeadTable.yMax, pheadAdded->HeadTable.yMax);
  HeadTable.xMin = MIN(HeadTable.xMin, pheadAdded->HeadTable.xMin);
  HeadTable.yMin = MIN(HeadTable.yMin, pheadAdded->HeadTable.yMin);
}

USHORT headTableINFO::GetUnitsPerEm() const
{
  return HeadTable.UnitsPerEm;
}

const headTableSTRUCT* headTableINFO::GetHeadTableInfo() const
{
  return &HeadTable;
}

FWORD headTableINFO::GetxMin() const
{
  return HeadTable.xMin;
}

void headTableINFO::Duplicate(const TableINFO* pTable)
{
  headTableINFO* pHead = (headTableINFO*)pTable;
  memmove(&HeadTable, &pHead->HeadTable, sizeof(headTableSTRUCT));
}
// class headTableINFO End

// class hheaTableINFO Begin
void hheaTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  ttfReadFile(&hheaTable, "DWWWWWWWWWWWWWWWW", 1, pFile);
}

void hheaTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile(&hheaTable, "DWWWWWWWWWWWWWWWW", 1, pFile);
}

int hheaTableINFO::GetNumberOfHMetrics() const
{
  return hheaTable.NumberOfHMetrics;
}

void hheaTableINFO::SetNumberOfHMetrics(USHORT Count)
{
  hheaTable.NumberOfHMetrics = Count;
}

void hheaTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
}

void hheaTableINFO::Duplicate(const TableINFO* pTable)
{
  memmove(&hheaTable, &((hheaTableINFO*)pTable)->hheaTable, sizeof(hheaTableSTRUCT));
}
// class hheaTableINFO End

// class hmtxTableINFO Begin
hmtxTableINFO::hmtxTableINFO() : hMetrics(NULL), LeftSideBearing(NULL) {}

hmtxTableINFO::~hmtxTableINFO()
{
  free(hMetrics);
  free(LeftSideBearing);
}

void hmtxTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  hheaTableINFO* phheaTableInfo = (hheaTableINFO*)GetTableInfo("hhea", pFile);
  NumberOfHMetrics = phheaTableInfo->GetNumberOfHMetrics();
  maxpTableINFO* pmaxpTableInfo = (maxpTableINFO*)GetTableInfo("maxp", pFile);
  NumberOfGlyphs = pmaxpTableInfo->GetNumberOfGlyphs();

  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  hMetrics = (LongHorMETRIC*)malloc(sizeof(LongHorMETRIC) * NumberOfHMetrics);
  ttfReadFile(hMetrics, "WW", NumberOfHMetrics, pFile);
  unsigned Num = NumberOfGlyphs - NumberOfHMetrics;

  if (Num > 0) {
    LeftSideBearing = (FWORD*)malloc(Num * sizeof(FWORD));
    ttfReadFile(LeftSideBearing, "W", Num, pFile);
  }
  else {
    LeftSideBearing = NULL;
  }
}

void hmtxTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile(hMetrics, "WW", NumberOfHMetrics, pFile);
  int Num = NumberOfGlyphs - NumberOfHMetrics;

  if (Num > 0) {
    ttfWriteFile(LeftSideBearing, "W", Num, pFile);
  }
}

void hmtxTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  ///??这里作简化处理,直接在后面添加LeftSideBearing而LongHorMETRIC保持不变
  /*
  hmtxTableINFO *phmtxAdded = (hmtxTableINFO *)pAddedDoc->GetTableInfo("hmtx");
  unsigned LsbNum = NumberOfGlyphs - NumberOfHMetrics;

  int i;
  LeftSideBearing = (FWORD *)realloc(LeftSideBearing, (LsbNum + phmtxAdded->NumberOfGlyphs) * sizeof(FWORD));
  for(i = LsbNum; i < LsbNum + phmtxAdded->NumberOfHMetrics; i++)
  LeftSideBearing[i] = phmtxAdded->hMetrics[i - LsbNum].lsb;
  for(; i < LsbNum + phmtxAdded->NumberOfGlyphs; i++)
  LeftSideBearing[i] = phmtxAdded->LeftSideBearing[i - (LsbNum + phmtxAdded->NumberOfHMetrics)];

  NumberOfGlyphs += phmtxAdded->NumberOfGlyphs;*/

  hmtxTableINFO* phmtxAdded = (hmtxTableINFO*)pAddedDoc->GetTableInfo("hmtx");
  int LsbNum = NumberOfGlyphs - NumberOfHMetrics;

  int i;

  if (phmtxAdded->NumberOfHMetrics > 0) {
    //先把lsb转换成hmetrics纪录
    int Aw = hMetrics[NumberOfHMetrics - 1].AdvanceWidth;
    hMetrics = (LongHorMETRIC*)realloc(hMetrics, sizeof(LongHorMETRIC) * (NumberOfGlyphs + phmtxAdded->NumberOfHMetrics));

    for (i = 0; i < LsbNum; i++) {
      hMetrics[i + NumberOfHMetrics].AdvanceWidth = Aw;
      hMetrics[i + NumberOfHMetrics].lsb = LeftSideBearing[i];
    }

    LsbNum = 0;
  }

  headTableINFO* pheadAdded = (headTableINFO*)pAddedDoc->GetTableInfo("head");
  headTableINFO* pheadThis = (headTableINFO*)GetTableInfo("head");
  double Scaling = double(pheadThis->GetUnitsPerEm()) / double(pheadAdded->GetUnitsPerEm());

  LeftSideBearing = (FWORD*)realloc(LeftSideBearing, (LsbNum + phmtxAdded->NumberOfGlyphs - phmtxAdded->NumberOfHMetrics) * sizeof(FWORD));

  for (i = 0; i < phmtxAdded->NumberOfGlyphs - phmtxAdded->NumberOfHMetrics; i++) {
    LeftSideBearing[i + LsbNum] = (short)(phmtxAdded->LeftSideBearing[i] * Scaling);
  }

  for (i = 0; i < phmtxAdded->NumberOfHMetrics; i++) {
    hMetrics[i + NumberOfGlyphs].lsb = (short)(phmtxAdded->hMetrics[i].lsb * Scaling);
    hMetrics[i + NumberOfGlyphs].AdvanceWidth = (short)(phmtxAdded->hMetrics[i].AdvanceWidth * Scaling);
  }

  NumberOfHMetrics = NumberOfGlyphs - LsbNum + phmtxAdded->NumberOfHMetrics;
  NumberOfGlyphs += phmtxAdded->NumberOfGlyphs;
  //修改hhea表里的LongHorMetrics数量，在这里修改后其他地方就不要在修改了
  hheaTableINFO* phheaThis = (hheaTableINFO*)GetTableInfo("hhea");
  phheaThis->SetNumberOfHMetrics(NumberOfHMetrics);
}

void hmtxTableINFO::Duplicate(const TableINFO* pTable)
{
  hmtxTableINFO* pHmtx = (hmtxTableINFO*)pTable;
  NumberOfHMetrics = pHmtx->NumberOfHMetrics;
  NumberOfGlyphs = pHmtx->NumberOfGlyphs;
  hMetrics = (LongHorMETRIC*)memdup(pHmtx->hMetrics, sizeof(LongHorMETRIC) * NumberOfHMetrics);
  LeftSideBearing = (FWORD*)memdup(pHmtx->LeftSideBearing, sizeof(FWORD) * (NumberOfGlyphs - NumberOfHMetrics));
}
// class hmtxTableINFO End

// class locaTableINFO Begin
locaTableINFO::locaTableINFO() : pOffset(NULL) {}

locaTableINFO::~locaTableINFO()
{
  free(pOffset);
}

void locaTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  maxpTableINFO* pmaxpTableInfo = (maxpTableINFO*)GetTableInfo("maxp", pFile);
  NumGlyphs = pmaxpTableInfo->GetNumberOfGlyphs();
  headTableINFO* pheadTableInfo = (headTableINFO*)GetTableInfo("head", pFile);
  LongFormat = pheadTableInfo->GetIndexToLocFormat();

  fseek(pFile, pTableEntry->Offset, SEEK_SET);

  if (LongFormat) {
    //In order to compute the length of the last glyph element,
    //there is an extra entry after the last valid index.
    pOffset = malloc(sizeof(ULONG) * (NumGlyphs + 1));
    ttfReadFile(pOffset, "D", pTableEntry->Length / sizeof(ULONG), pFile);
  }
  else {
    pOffset = malloc(sizeof(USHORT) * (NumGlyphs + 1));
    ttfReadFile(pOffset, "W", pTableEntry->Length / sizeof(USHORT), pFile);
  }
}

ULONG locaTableINFO::GetOffset(int Index)
{
  if (LongFormat) {
    return *((ULONG*)pOffset + Index);
  }
  else {
    return *((USHORT*)pOffset + Index) << 1;
  }
}

void locaTableINFO::SetOffset(int i, ULONG Offset)
{
  if (LongFormat) {
    //assert(*((ULONG *)pOffset + i) == Offset);
    *((ULONG*)pOffset + i) = Offset;
  }
  else {
    *((USHORT*)pOffset + i) = (USHORT)(Offset >> 1); //The actual local offset divided by 2 is stored.
  }
}

void locaTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  if (LongFormat) {
    ttfWriteFile(pOffset, "D", NumGlyphs + 1, pFile);
  }
  else {
    ttfWriteFile(pOffset, "W", NumGlyphs + 1, pFile);
  }
}

void locaTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  locaTableINFO* plocaAdded = (locaTableINFO*)pAddedDoc->GetTableInfo("loca");
  NumGlyphs += plocaAdded->NumGlyphs;

  if (LongFormat) {
    pOffset = realloc(pOffset, sizeof(ULONG) * (NumGlyphs + 1));
  }
  else {
    pOffset = realloc(pOffset, sizeof(USHORT) * (NumGlyphs + 1));
  }
}

void locaTableINFO::Duplicate(const TableINFO* pTable)
{
  locaTableINFO* pLoca = (locaTableINFO*)pTable;
  LongFormat = pLoca->LongFormat;
  NumGlyphs = pLoca->NumGlyphs;
  pOffset = memdup(pLoca->pOffset, (LongFormat ? sizeof(ULONG) : sizeof(USHORT)) * (NumGlyphs + 1));
}
// class locaTableINFO End

// class maxpTableINFO Begin
void maxpTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  ttfReadFile(&MaxpTable, "DWWWWWWWWWWWWWW", 1, pFile);
}

void maxpTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile(&MaxpTable, "DWWWWWWWWWWWWWW", 1, pFile);
}

int maxpTableINFO::GetNumberOfGlyphs() const
{
  return MaxpTable.NumGlyphs;
}

void maxpTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  maxpTableINFO* pmaxpAdded = (maxpTableINFO*)pAddedDoc->GetTableInfo("maxp");
  MaxpTable.NumGlyphs += pmaxpAdded->GetNumberOfGlyphs();
  MaxpTable.MaxPoints = MAX(MaxpTable.MaxPoints, pmaxpAdded->MaxpTable.MaxPoints);
  MaxpTable.MaxContours = MAX(MaxpTable.MaxContours, pmaxpAdded->MaxpTable.MaxContours);
  MaxpTable.MaxCompositePoints = MAX(MaxpTable.MaxCompositePoints, pmaxpAdded->MaxpTable.MaxCompositePoints);
  MaxpTable.MaxCompositeContours = MAX(MaxpTable.MaxCompositeContours, pmaxpAdded->MaxpTable.MaxCompositeContours);

  MaxpTable.MaxTwilightPoints = MAX(MaxpTable.MaxTwilightPoints, pmaxpAdded->MaxpTable.MaxTwilightPoints);
  MaxpTable.MaxStorage = MAX(MaxpTable.MaxStorage, pmaxpAdded->MaxpTable.MaxStorage);
  MaxpTable.MaxStackElements = MAX(MaxpTable.MaxStackElements, pmaxpAdded->MaxpTable.MaxStackElements);
  MaxpTable.MaxSizeOfInstructions = MAX(MaxpTable.MaxSizeOfInstructions, pmaxpAdded->MaxpTable.MaxSizeOfInstructions);

  MaxpTable.MaxComponentElements = MAX(MaxpTable.MaxComponentElements, pmaxpAdded->MaxpTable.MaxComponentElements);
  MaxpTable.MaxComponentDepth = MAX(MaxpTable.MaxComponentDepth, pmaxpAdded->MaxpTable.MaxComponentDepth);
}

void maxpTableINFO::Duplicate(const TableINFO* pTable)
{
  memmove(&MaxpTable, &((maxpTableINFO*)pTable)->MaxpTable, sizeof(maxpTableSTRUCT));
}
// class maxpTableINFO End

// class nameTableINFO Begin
nameTableINFO::nameTableINFO()
{
  NumberOfRecord = 0;
  pString = NULL;
  NameRecords = NULL;
}

nameTableINFO::~nameTableINFO()
{
  free(pString);
  free(NameRecords);
}

void nameTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  FormatSelector = ttfGetUShort(pFile);
  NumberOfRecord = ttfGetUShort(pFile);
  OffsetOfString = ttfGetUShort(pFile);
  NameRecords = (NameRECORD*)malloc(sizeof(NameRECORD) * NumberOfRecord);
  ttfReadFile(NameRecords, "WWWWWW", NumberOfRecord, pFile);

  int Pos = ftell(pFile);
  StrLen = pTableEntry->Length - (Pos - pTableEntry->Offset);
  pString = (char*)malloc(StrLen + 1);
  READFILE(pString, StrLen, pFile);
}

void nameTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfPutUShort(FormatSelector, pFile);
  ttfPutUShort(NumberOfRecord, pFile);
  ttfPutUShort(sizeof(USHORT) * 3 + sizeof(NameRECORD) * NumberOfRecord, pFile);
  ttfWriteFile(NameRecords, "WWWWWW", NumberOfRecord, pFile);
  WRITEFILE(pString, StrLen, pFile);
}

void nameTableINFO::Duplicate(const TableINFO* pTable)
{
  nameTableINFO* pCopied = (nameTableINFO*)pTable;
  FormatSelector = pCopied->FormatSelector;
  NumberOfRecord = pCopied->NumberOfRecord;
  OffsetOfString = pCopied->OffsetOfString;
  StrLen = pCopied->StrLen;
  NameRecords = (NameRECORD*)memdup(pCopied->NameRecords, sizeof(NameRECORD) * NumberOfRecord);
  pString = (char*)memdup(pCopied->pString, StrLen);
}

//取得最有可能作为显示名的namerecord索引
int nameTableINFO::GetNameIndex() const
{
  int i, NameIndex[] = { -1, -1, -1, -1};
  enum {UNI_DEFAULT = 0, UNI_ENGLISH, MAC_CHINESE, OTHER};

  for (i = 0; i < NumberOfRecord; i++) {
    if (NameRecords[i].NameId != 1) {
      continue;
    }

    if (NameRecords[i].PlatformId == 3 && NameRecords[i].EncodingId == 1) {
      if (NameRecords[i].LanguageId == GetSystemDefaultLCID()) {
        NameIndex[UNI_DEFAULT] = i;
      }
      else {
        NameIndex[UNI_ENGLISH] = i;
      }
    }
    else if (NameRecords[i].PlatformId == 1 && NameRecords[i].EncodingId == 0x19) {
      NameIndex[MAC_CHINESE] = i;
    }
    else {
      NameIndex[OTHER] = i;
    }
  }

  for (i = 0; i < 4; i++)
    if (NameIndex[i] >= 0) {
      break;
    }

  return NameIndex[i];
}

int nameTableINFO::GetFontName(char* pBuf, BOOL* bUnicode, ULONG BufLen) const
{
  //如果BufLen为零返回所需要的内存大小
  int i = GetNameIndex();

  if (BufLen > 0) {
    memmove(pBuf, pString + NameRecords[i].Offset, NameRecords[i].StrLen);

    if (NameRecords[i].PlatformId == 3 && NameRecords[i].EncodingId == 1) {
      //文件里的unicode编码是big endian,而内存中要求small endian
      ReverseOrder(pBuf, "W", NameRecords[i].StrLen / 2);
      *bUnicode = TRUE;
    }
    else {
      *bUnicode = FALSE;
    }
  }

  return NameRecords[i].StrLen;
}

void nameTableINFO::SetFontName(const char* pStr, BOOL bUnicode)
{
  void* pNew;
  int i = GetNameIndex(), Len;

  if (NameRecords[i].PlatformId == 3 && NameRecords[i].EncodingId == 1) {
    if (!bUnicode) { //先转换为Unicode
      Len = MultiByteToWideChar(CP_ACP, 0, LPCSTR(pStr), -1, NULL, 0);
      pNew = alloca(Len + 2);
      Len = MultiByteToWideChar(CP_ACP, 0, LPCSTR(pStr), -1, LPWSTR(pNew), Len + 2);
    }
    else {
      Len = lstrlenW(LPCWSTR(pStr));
      pNew = memdup(pStr, Len * 2);
      //MessageBoxW(NULL, LPWSTR(pStr), NULL, MB_OK);
    }

    //文件里的unicode编码是big endian,而内存中要求small endian
    ReverseOrder(pNew, "W", Len);
    Len *= 2;
  }
  else {
    if (bUnicode) {
      Len = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(pStr), -1, NULL, 0, NULL, NULL);
      pNew = alloca(Len + 2);
      Len = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(pStr), -1, LPSTR(pNew), Len + 2, NULL, NULL);
    }
    else {
      pNew = (void*)pStr;
      Len = strlen(pStr);
    }
  }

  if (Len > NameRecords[i].StrLen) {
    int AppendLen = Len - NameRecords[i].StrLen;
    pString = (char*)realloc(pString, StrLen + AppendLen);
    memmove(pString + NameRecords[i].Offset + AppendLen, pString + NameRecords[i].Offset,
        StrLen - NameRecords[i].Offset);

    //把后面的段偏移依次后移AppendLen
    for (int j = 0; j < NumberOfRecord; j++) {
      if (NameRecords[j].Offset == NameRecords[i].Offset) {
        NameRecords[j].StrLen += AppendLen;
      }

      if (NameRecords[j].Offset > NameRecords[i].Offset) {
        NameRecords[j].Offset += AppendLen;
      }
    }

    StrLen += AppendLen;
  }

  memmove(pString + NameRecords[i].Offset, pNew, Len);
  NameRecords[i].StrLen = Len;
}
// class nameTableINFO End

// class postTableINFO Begin

postTableINFO::~postTableINFO()
{
  if (pCommonHeader->FormatType == 0x00020000) {
    post20FORMAT* p20Format = (post20FORMAT*)pCommonHeader;
    free(p20Format->glyphNameIndex);
    free(p20Format->glyphNames);
  }
  else if (pCommonHeader->FormatType == 0x00020005) {
    post25FORMAT* p25Format = (post25FORMAT*)pCommonHeader;
    free(p25Format->Offset);
  }

  free(pCommonHeader);
}

void postTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  pCommonHeader = (postSimpleFORMAT*)malloc(sizeof(postSimpleFORMAT));
  ttfReadFile(pCommonHeader, "DDWWDDDDD", 1, pFile);

  if (pCommonHeader->FormatType == 0x00020000) {
    post20FORMAT* p20Format = (post20FORMAT*)realloc(pCommonHeader, sizeof(post20FORMAT));
    pCommonHeader = (postSimpleFORMAT*)p20Format;
    p20Format->NumberOfGlyphs = ttfGetUShort(pFile);
    p20Format->glyphNameIndex = (USHORT*)malloc(sizeof(USHORT) * p20Format->NumberOfGlyphs);
    ttfReadFile(p20Format->glyphNameIndex, "W", p20Format->NumberOfGlyphs, pFile);

    //读剩余的名字表
    p20Format->NameLength = pTableEntry->Length - (ftell(pFile) - pTableEntry->Offset);
    p20Format->glyphNames = (char*)malloc(p20Format->NameLength + 2);
    ttfReadFile(p20Format->glyphNames, "B", p20Format->NameLength, pFile);
  }
  else if (pCommonHeader->FormatType == 0x00020005) {
    post25FORMAT* p25Format = (post25FORMAT*)realloc(pCommonHeader, sizeof(post25FORMAT));
    pCommonHeader = (postSimpleFORMAT*)p25Format;
    p25Format->NameLength = pTableEntry->Length - sizeof(postSimpleFORMAT);
    p25Format->Offset = (char*)malloc(p25Format->NameLength);
    ttfReadFile(p25Format->Offset, "B", p25Format->NameLength, pFile);
  }
}

void postTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile(pCommonHeader, "DDWWDDDDD", 1, pFile);

  if (pCommonHeader->FormatType == 0x00020000) {
    post20FORMAT* p20Format = (post20FORMAT*)pCommonHeader;
    ttfPutUShort(p20Format->NumberOfGlyphs, pFile);
    ttfWriteFile(p20Format->glyphNameIndex, "W", p20Format->NumberOfGlyphs, pFile);

    //写剩余的名字表
    ttfWriteFile(p20Format->glyphNames, "B", p20Format->NameLength, pFile);
  }
  else if (pCommonHeader->FormatType == 0x00020005) {
    post25FORMAT* p25Format = (post25FORMAT*)pCommonHeader;
    ttfWriteFile(p25Format->Offset, "B", p25Format->NameLength, pFile);
  }
}

void postTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
  maxpTableINFO* pmaxpAdded = (maxpTableINFO*)pAddedDoc->GetTableInfo("maxp");

  if (pCommonHeader->FormatType == 0x00020000) {
    post20FORMAT* p20Format = (post20FORMAT*)pCommonHeader;
    int AddedCount = pmaxpAdded->GetNumberOfGlyphs();
    p20Format->glyphNameIndex = (USHORT*)realloc(p20Format->glyphNameIndex, (p20Format->NumberOfGlyphs + AddedCount) * sizeof(USHORT));

    for (int i = p20Format->NumberOfGlyphs; i < p20Format->NumberOfGlyphs + AddedCount; i++) {
      p20Format->glyphNameIndex[i] = 0;
    }

    p20Format->NumberOfGlyphs += AddedCount;
  }
  else if (pCommonHeader->FormatType == 0x00020005) {
    //post25FORMAT *p25Format = (post25FORMAT *)pCommonHeader;
  }
}

void postTableINFO::Duplicate(const TableINFO* pTable)
{
  postTableINFO* pPost = (postTableINFO*)pTable;

  if (pPost->pCommonHeader->FormatType == 0x00020000) {
    post20FORMAT* pSrc = (post20FORMAT*)pPost->pCommonHeader;
    pCommonHeader = (postSimpleFORMAT*)memdup(pSrc, sizeof(post20FORMAT));
    ((post20FORMAT*)pCommonHeader)->glyphNameIndex = (USHORT*)memdup(pSrc->glyphNameIndex, sizeof(USHORT) * pSrc->NumberOfGlyphs);
    ((post20FORMAT*)pCommonHeader)->glyphNames = (char*)memdup(pSrc->glyphNames, pSrc->NameLength);
  }
  else if (pPost->pCommonHeader->FormatType == 0x00020005) {
    post25FORMAT* pSrc = (post25FORMAT*)pPost->pCommonHeader;
    pCommonHeader = (postSimpleFORMAT*)memdup(pSrc, sizeof(post25FORMAT));
    ((post25FORMAT*)pCommonHeader)->Offset = (char*)memdup(pSrc->Offset, pSrc->NameLength);
  }
  else {
    pCommonHeader = (postSimpleFORMAT*)memdup(pPost->pCommonHeader, sizeof(postSimpleFORMAT));
  }
}
// class postTableINFO End

// class OS2TableINFO Begin
void OS2TableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  ttfReadFile((void*)&OS2Table, "WWWWWWWWWWWWWWWWBBBBBBBBBB", 1, pFile);
  const char* pFrame = OS2Table.Version ? "DDDDBBBBWWWWWWWWDD" : "DDDDBBBBWWWWWWWW";
  ttfReadFile((void*)&OS2Table.UnicodeRange1, pFrame, 1, pFile);
}

void OS2TableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile((void*)&OS2Table, "WWWWWWWWWWWWWWWWBBBBBBBBBB", 1, pFile);
  const char* pFrame = OS2Table.Version ? "DDDDBBBBWWWWWWWWDD" : "DDDDBBBBWWWWWWWW";
  ttfWriteFile((void*)&OS2Table.UnicodeRange1, pFrame, 1, pFile);
}

void OS2TableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
}

void OS2TableINFO::Duplicate(const TableINFO* pTable)
{
  memmove(&OS2Table, &((OS2TableINFO*)pTable)->OS2Table, sizeof(OS2TableSTRUCT));
}

int OS2TableINFO::GetWinDescent() const
{
  return (SHORT)OS2Table.WinDescent;
}
// class OS2TableINFO End

// class cvtTableINFO Begin
cvtTableINFO::cvtTableINFO() : ControlValue(NULL) {}

cvtTableINFO::~cvtTableINFO()
{
  free(ControlValue);
}

void cvtTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  ControlValue = (FWORD*)malloc(pTableEntry->Length);
  Count = pTableEntry->Length / sizeof(FWORD);
  ttfReadFile(ControlValue, "W", Count, pFile);
}

void cvtTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  ttfWriteFile(ControlValue, "W", Count, pFile);
}

void cvtTableINFO::AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param)
{
}

void cvtTableINFO::Duplicate(const TableINFO* pTable)
{
  Count = ((cvtTableINFO*)pTable)->Count;
  ControlValue = (FWORD*)memdup(((cvtTableINFO*)pTable)->ControlValue, Count * sizeof(FWORD));
}
// class cvtTableINFO End

// class OtherTableINFO Begin
void OtherTableINFO::Load(FILE* pFile, const TableENTRY* pTableEntry)
{
  fseek(pFile, pTableEntry->Offset, SEEK_SET);
  pDataBuf = realloc(pDataBuf, pTableEntry->Length);
  READFILE(pDataBuf, pTableEntry->Length, pFile);
  Length = pTableEntry->Length;
}

void OtherTableINFO::Store(FILE* pFile, TableENTRY* pTableEntry) const
{
  WRITEFILE(pDataBuf, Length, pFile);
}

void OtherTableINFO::Duplicate(const TableINFO* pTable)
{
  Length = ((OtherTableINFO*)pTable)->Length;
  pDataBuf = memdup(((OtherTableINFO*)pTable)->pDataBuf, Length);
}
// class OtherTableINFO End

int LoadTtfDoc(TtfDOC* pDoc, const char* pName)
{
  FILE* pFile = fopen(pName, "rb");

  if (pFile) {
    pDoc->LoadTtf(pFile);
    fclose(pFile);
  }

  return 0;
}

// 高位字节使用了0xA1-0xF7；低位字节使用了0xA1-0xFE
int DrawChar(HDDC hDC, int x, int y, int Width, int Height, TtfDOC* pTtf, int GlyfIndex)
{
  assert(GlyfIndex < pTtf->GetGlyfCount());
  ExportGlyfSTRUCT Glyph[1] = {0};
  pTtf->GetGlyph(Glyph, GlyfIndex);

  if (Glyph->PtCount > 0) {
    //先纠正并转换点的坐标
    int UnitPerEm = pTtf->GetUnitsPerEm();
    double xScaling = Width * 1. / UnitPerEm;
    double yScaling = Height * 1. / UnitPerEm;

    DrawGlyph(hDC, x, y, Width, Height, Glyph, xScaling, yScaling);
  }

  return 0;
}


#if 0
int KTrueType::DecodeGlyph(int index, KCurve& curve, XFORM* xm) const
{
  const GlyphHeader* pHeader = GetGlyph(index);

  if (pHeader == NULL) {
    // assert(false);
    return 0;
  }

  int nContour = (short) reverse(pHeader->numberOfContours);

  if (nContour < 0) {
    return DecodeCompositeGlyph(pHeader + 1, curve); // after the header
  }

  if (nContour == 0) {
    return 0;
  }

  curve.SetBound(reverse((WORD)pHeader->xMin), reverse((WORD)pHeader->yMin),
      reverse((WORD)pHeader->xMax), reverse((WORD)pHeader->yMax));

  const USHORT* pEndPoint = (const USHORT*)(pHeader + 1);

  int nPoints = reverse(pEndPoint[nContour - 1]) + 1; // endpoint of last contour + 1
  int nInst = reverse(pEndPoint[nContour]); // instructon length

  curve.m_glyphindex = index;
  curve.m_glyphsize = (int) GetGlyph(index + 1) - (int) GetGlyph(index);
  curve.m_Ascender = m_Ascender;
  curve.m_Descender = m_Descender;
  curve.m_LineGap = m_LineGap;

  GetMetrics(index, curve.m_advancewidth, curve.m_lsb);

  if (curve.m_glyphsize == 0) {
    return 0;
  }

  curve.m_instrsize = nInst;

  const BYTE* pFlag = (const BYTE*) & pEndPoint[nContour] + 2 + nInst;   // first byte in flag
  const BYTE* pX = pFlag;

  int xlen = 0;

  for (int i = 0; i < nPoints; i++, pX++) {
    int unit = 0;

    switch (pX[0] & G_XMASK) {
    case G_XADDBYTE:
    case G_XSUBBYTE:
      unit = 1;
      break;

    case G_XADDINT:
      unit = 2;
    }

    if (pX[0] & G_REPEAT) {
      xlen += unit * (pX[1] + 1);

      i += pX[1];
      pX ++;
    }
    else {
      xlen += unit;
    }
  }

  const BYTE* pY = pX + xlen;

  int x = 0;

}

int KTrueType::DecodeCompositeGlyph(const int* pGlyph, KCurve& curve) const
{
  KDataStream str(pGlyph);
  unsigned flags;
  int len = 0;

  do {
    flags = str.GetWord();
    unsigned glyphIndex = str.GetWord();
    // Argument1 and argument2 can be either x and y offsets to be added to the glyph or two point numbers.
    // In the latter case, the first point number indicates the point that is to be matched to the new glyph.
    // The second number indicates the new glyph's "matched" point. Once a glyph is added, its point numbers
    // begin directly after the last glyphs (endpoint of first glyph + 1).
    // When arguments 1 and 2 are an x and a y offset instead of points and the bit ROUND_XY_TO_GRID is set to 1,
    // the values are rounded to those of the closest grid lines before they are added to the glyph.
    // X and Y offsets are described in FUnits.
    signed short argument1;
    signed short argument2;

    if (flags & ARG_1_AND_2_ARE_WORDS) {
      argument1 = str.GetWord(); // (SHORT or FWord) argument1;
      argument2 = str.GetWord(); // (SHORT or FWord) argument2;
    }
    else {
      argument1 = (signed char) str.GetByte();
      argument2 = (signed char) str.GetByte();
    }

    signed short xscale, yscale, scale01, scale10;
    xscale = 1;
    yscale = 1;
    scale01 = 0;
    scale10 = 0;

    if (flags & WE_HAVE_A_SCALE) {
      xscale = str.GetWord();
      yscale = xscale; // Format 2.14
    }
    else if (flags & WE_HAVE_AN_X_AND_Y_SCALE) {
      xscale = str.GetWord();
      yscale = str.GetWord();
    }
    else if (flags & WE_HAVE_A_TWO_BY_TWO) {
      xscale = str.GetWord();
      scale01 = str.GetWord();
      scale10 = str.GetWord();
      yscale = str.GetWord();
    }

    if (flags & ARGS_ARE_XY_VALUES) {
      XFORM xm;
      xm.eDx = (float) argument1;
      xm.eDy = (float) argument2;
      xm.eM11 = xscale / (float) 16384.0;
      xm.eM12 = scale01 / (float) 16384.0;
      xm.eM21 = scale10 / (float) 16384.0;
      xm.eM22 = yscale / (float) 16384.0;
      len += DecodeGlyph(glyphIndex, curve, & xm);
    }
    else {
      assert(false);
    }
  }
  while (flags & MORE_COMPONENTS);

  if (flags & WE_HAVE_INSTRUCTIONS) {
    unsigned numInstr = str.GetWord();

    for (unsigned i = 0; i < numInstr; i++) {
      str.GetByte();
    }
  }

  // The purpose of USE_MY_METRICS is to force the lsb and rsb to take on a desired value.
  // For example, an i-circumflex (Unicode 00ef) is often composed of the circumflex and a dotless-i.
  // In order to force the composite to have the same metrics as the dotless-i,
  // set USE_MY_METRICS for the dotless-i component of the composite. Without this bit,
  // the rsb and lsb would be calculated from the HMTX entry for the composite (or would need to be
  // explicitly set with TrueType instructions).
  // Note that the behavior of the USE_MY_METRICS operation is undefined for rotated composite components.
  return len;
}
#endif