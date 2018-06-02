#include "ttffmt.h"
//#define BIG_ENDIAN_W()
FormatHEADER* CreateAndReadMapFormat(FormatHEADER* pHeader)
{
  char* ThisPos = (char*)pHeader;
  char* p;
  char* p2 = (char*)(pHeader + 1);
  int i;
  switch (pHeader->Format) {
  case 0:
    if (1) {
      EncodingMapFORMAT0* pFormat = (EncodingMapFORMAT0*)pHeader;
      ReverseOrder2(p2, "B", sizeof(pFormat->IndexArray), &p2);
      return pHeader;
    }
  case 2:
    if (1) {
      int i, Len;
      SubHEADER* SubHeaders;
      EncodingMapFORMAT2* pFormat = (EncodingMapFORMAT2*)pHeader;
      ReverseOrder2(p2, "W", 256, &p2);
      //因为subheaderkeys存放subheader的index*8
      Len = (pFormat->SubHeaderKeys[255] >> 3) + 1;
      SubHeaders = (SubHEADER*)(pFormat->SubHeaderKeys + 256);
      //读每个子区段
      for (i = 0; i < Len; i++) {
        SubHeaders = (SubHEADER*)ReverseOrder2(p2, "WWWW", 1, &p2);
        ReverseOrder2(p2 + SubHeaders->idRangeOffset - sizeof(USHORT), "W", SubHeaders->entryCount, &p2);
      }
      //assert(pFormat->Length == p2 - ThisPos);
      return pHeader;
    }
  case 4:
    if (1) {
      USHORT* endCode; //[segCount] End characterCode for each segment,last =0xFFFF.
      USHORT* reservedPad; //Set to 0.
      //USHORT* RevPad; //这里添加一个USHORT为文件4字节对齐, 注意文件里并无该域
      USHORT* startCode; //[segCount] Start character code for each segment.
      USHORT* idDelta; //[segCount] Delta for all character codes in segment.
      USHORT* idRangeOffset; //[segCount] Offsets into glyphIdArray or 0
      USHORT* IndexArray; //Glyph index array (arbitrary length)
      EncodingMapFORMAT4* pFormat = (EncodingMapFORMAT4*)pHeader;
      int SegCount;
      ReverseOrder2(p2, "W", 4, &p2);
      SegCount = pFormat->segCountX2 >> 1;
      endCode = (USHORT*)ReverseOrder2(p2, "W", SegCount, &p2);
      reservedPad = (USHORT*)ReverseOrder2(p2, "W", 1, &p2);
      startCode = (USHORT*)ReverseOrder2(p2, "W", SegCount, &p2);
      idDelta = (USHORT*)ReverseOrder2(p2, "W", SegCount, &p2);
      p = p2;
      idRangeOffset = (USHORT*)ReverseOrder2(p2, "W", SegCount, &p2);
      //读每个子区段的索引子表
      for (i = 0; i < SegCount; i++) { //最后一个段(0xffff)不读文件,直接赋零
        if (idRangeOffset[i]) {
          int Count = endCode[i] - startCode[i] + 1;
          if (i < SegCount - 1) {
            IndexArray = (USHORT*)(p + i * sizeof(USHORT) + idRangeOffset[i]);
            ReverseOrder2(IndexArray, "W", Count, &p2);
          }
          else {
            //memset(pFormat->IndexArray[i], 0, sizeof(USHORT) * Count);
          }
        }
      }
      return pHeader;
    }
  case 6:
    if (1) {
      EncodingMapFORMAT6* pFormat = (EncodingMapFORMAT6*)pHeader;
      ReverseOrder2(p2, "W", 2, &p2);
      ReverseOrder2(p2, "W", pFormat->entryCount, &p2);
      return pHeader;
    }
  default: //未知格式
    Throw_CustomException(ErrStr[0]);
  }
  return NULL;
}
int ttf_cmap_load(ttffmt_t* s, void* p, int n)
{
  int i, j;
  char* p2 = (char*)p;
  EncodingMapENTRY1* MapEntries;
  s->cmap = (cmapTableSTRUCT*)ReverseOrder2(p2, "WW", 1, &p2);
  MapEntries = (EncodingMapENTRY1*)ReverseOrder2(p2, "WWD", s->cmap->TableNumber, &p2);
  for (i = 0; i < s->cmap->TableNumber; ++i) {
    int has = 0;
    //注意：不同的EncodingMapENTRY可能指向同一个编码信息
    for (j = 0; j < i; j++) {
      if (MapEntries[i].Offset == MapEntries[j].Offset) {
        //MapEntries[i].CodingMapId = j;
        has = 1;
        break;
      }
    }
    if (!has) {
      //先读格式头信息
      FormatHEADER* TempHeader = (FormatHEADER*)ReverseOrder2((char*)p + MapEntries[i].Offset, "W", 3, &p2);
      CreateAndReadMapFormat(TempHeader);
    }
    else {
      //MapTableInfo[i] = MapTableInfo[MapEntries[i].CodingMapId];
    }
  }
  return 0;
}
OS2TableSTRUCT* ttf_OS2_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  OS2TableSTRUCT* OS2Table = (OS2TableSTRUCT*)ReverseOrder2(p, "WWWWWWWWWWWWWWWWBBBBBBBBBB", 1, &p2);
  const char* pFrame = OS2Table->Version ? "DDDDBBBBWWWWWWWWDD" : "DDDDBBBBWWWWWWWW";
  const char* p0 = (char*)ReverseOrder2(p2, pFrame, 1, &p2);
  ASSERT(p0 == (char*)&OS2Table->UnicodeRange1);
  //int t1 = GetDataFrameLength(pFrame);
  //memcpy(&OS2Table->UnicodeRange1, p0, t1);
  return OS2Table;
}
//计算简单字形格式的点数
int GetPtCountOfSimpleGlyph(int NumberOfContours, const glyfSimpleFORMAT* pFormat)
{
  int i, PtCount = 0;
  if (NumberOfContours <= 0) {
    return 0;
  }
  assert(NumberOfContours > 0);
  //根据EndPtsOfContours算出点数
  for (i = 0; i < NumberOfContours; i++) {
    if (PtCount < pFormat->EndPtsOfContours[i]) {
      PtCount = pFormat->EndPtsOfContours[i];
    }
  }
  return PtCount + 1;
}
#define PTR2(pBuf, n) (pBuf+=(n), pBuf-(n))
#define PTR3(pBuf, _T, n) (_T*)(pBuf+=sizeof(_T)*(n), pBuf-sizeof(_T)*(n))
int GetCompositeCount(char* p2)
{
  int nStructList = 0;
  DWORD Flag;
  do {
    int k = 2;
    Flag = ReverseWord2(p2, &p2);
    nStructList++;
    if (Flag & ARG_1_AND_2_ARE_WORDS) {
      k += 1;
    }
    if (Flag & WE_HAVE_A_SCALE) {
      k += 1;
    }
    else if (Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
      k += 2;
    }
    else if (Flag & WE_HAVE_A_TWO_BY_TWO) {
      k += 4;
    }
    p2 = (char*)((WORD*)p2 + k);
  }
  while (Flag & MORE_COMPONENTS);
  return nStructList;
}
int GetglyfCompositeFORMAT(glyfHEADER* glyfHeader, glyfCompositeFORMAT* pFormat)
{
  //It's a composite format;
  int i;
  char* p2 = (char*)(glyfHeader + 1);
  glyfCompositeSTRUCT* pStructList = 0;
  pFormat->pStructList = pStructList;
  pFormat->nStructList = GetCompositeCount(p2);
  pFormat->pStructList = (glyfCompositeSTRUCT*)malloc(sizeof(glyfCompositeSTRUCT) * pFormat->nStructList);
  for (i = 0; i < pFormat->nStructList; ++i) {
    pStructList = pFormat->pStructList + i;
    pStructList->Flag = ReverseWord2(p2, &p2);
    pStructList->glyphIndex = ReverseWord2(p2, &p2);
    pStructList->Argument = (SHORT)ReverseWord2(p2, &p2);
    if (pStructList->Flag & ARG_1_AND_2_ARE_WORDS) {
      pStructList->Argument2 = (SHORT)ReverseWord2(p2, &p2);
    }
    if (pStructList->Flag & WE_HAVE_A_SCALE) {
      pStructList->xScale = (SHORT)ReverseWord2(p2, &p2);
    }
    else if (pStructList->Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
      pStructList->xScale = ReverseWord2(p2, &p2);
      pStructList->yScale = ReverseWord2(p2, &p2);
    }
    else if (pStructList->Flag & WE_HAVE_A_TWO_BY_TWO) {
      pStructList->xScale = ReverseWord2(p2, &p2);
      pStructList->Scale01 = ReverseWord2(p2, &p2);
      pStructList->Scale10 = ReverseWord2(p2, &p2);
      pStructList->yScale = ReverseWord2(p2, &p2);
    }
  }
  if (pStructList->Flag & WE_HAVE_INSTRUCTIONS) {
    pFormat->NumberOfInstr = ReverseWord2(p2, &p2);
    pFormat->Instructions = (BYTE*)memdup(p2, pFormat->NumberOfInstr);
  }
  else {
    pFormat->NumberOfInstr = 0;
    pFormat->Instructions = NULL;
  }
  return 0;
}
int GetglyfSimpleFORMAT(glyfHEADER* pHeader, glyfSimpleFORMAT* pFormat)
{
  int i, j, k, PtCount;
  int xFrameLength, yFrameLength;
  char* p2 = (char*)(pHeader + 1);
  char* _x, *_y, *x, *y;
  BYTE* p;
  pFormat->EndPtsOfContours = (USHORT*)PTR2(p2, sizeof(USHORT) * pHeader->NumberOfContours);
  pFormat->InstructionLength = *(USHORT*)PTR2(p2, sizeof(USHORT));
  pFormat->Instructions = (BYTE*)PTR2(p2, pFormat->InstructionLength);
  PtCount = GetPtCountOfSimpleGlyph(pHeader->NumberOfContours, pFormat);
  pFormat->Flags = (BYTE*)p2;
  x = _x = (char*)malloc(4 * PtCount);
  y = _y = _x + 2 * PtCount;
  *x = *y = 0;
  p = pFormat->Flags;
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
        for (j = 0; j < *p; j++) {
          *x++ = xCh;
        }
      }
      if (Flag & 0x4 || !(Flag & 0x20)) {
        int yCh = *(y - 1);
        for (k = 0; k < *p; k++) {
          *y++ = yCh;
        }
      }
    }
  }
  *x = '\0';
  *y = '\0';
  pFormat->LengthOfFlags = p - pFormat->Flags; //falgs数组的长度为p - pFlagBuf;
  p2 += pFormat->LengthOfFlags;
  xFrameLength = GetDataFrameLength(_x);
  yFrameLength = GetDataFrameLength(_y);
  pFormat->xCoordinates = PTR2(p2, xFrameLength);
  pFormat->yCoordinates = PTR2(p2, yFrameLength);
  free(_x);
  return 0;
}
int CreateAndReadGlyfInfo(glyfHEADER* pHeader)
{
  char* p2 = (char*)(pHeader + 1);
  if (pHeader->NumberOfContours >= 0) {
    //It's a simple format;
    glyfSimpleFORMAT pFormat[1] = {0};
    pFormat->EndPtsOfContours = (USHORT*)ReverseOrder2(p2, "W" , pHeader->NumberOfContours, &p2);
    pFormat->InstructionLength = *(USHORT*)ReverseOrder2(p2, "W", 1, &p2);
    pFormat->Instructions = (BYTE*)p2;
    p2 += pFormat->InstructionLength;
    if (0) {
      int i, j, k, PtCount = GetPtCountOfSimpleGlyph(pHeader->NumberOfContours, pFormat);
      //根据点数读取flags,xCoordinates,yCoordinates
      char* Pos = p2;
      char* px, *py, *x, *y;
      BYTE* p;
      pFormat->Flags = (BYTE*)Pos;
      x = px = pFormat->pxFormat = (char*)malloc(PtCount + 1);
      y = py = pFormat->pyFormat = (char*)malloc(PtCount + 1);
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
      p = pFormat->Flags;
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
            for (j = 0; j < *p; j++) {
              *x++ = xCh;
            }
          }
          if (Flag & 0x4 || !(Flag & 0x20)) {
            int yCh = *(y - 1);
            for (k = 0; k < *p; k++) {
              *y++ = yCh;
            }
          }
        }
      }
      *x = '\0';
      *y = '\0';
      pFormat->LengthOfFlags = p - pFormat->Flags; //falgs数组的长度为p - pFlagBuf;
      //int xFrameLength = GetDataFrameLength(x);
      //int yFrameLength = GetDataFrameLength(y);
      pFormat->xCoordinates = ReverseOrder2(p2 + pFormat->LengthOfFlags, x, 1, &p2);
      pFormat->yCoordinates = ReverseOrder2(p2, y, 1, &p2);
      ASSERT((int)strlen(px) <= PtCount);
      ASSERT((int)strlen(py) <= PtCount);
      free(px);
      free(py);
    }
    return 0;
  }
  else {
    //It's a composite format;
    glyfCompositeFORMAT pFormat[1] = {0};
    glyfCompositeSTRUCT* pStructList = (glyfCompositeSTRUCT*)(pHeader + 1);
    char* p2 = (char*)pStructList;
    pFormat->pStructList = pStructList;
    if (0) {
      do {
        pFormat->nStructList++;
        pStructList = (glyfCompositeSTRUCT*)ReverseOrder2(p2, "WWW", 1, &p2);
        if (pStructList->Flag & ARG_1_AND_2_ARE_WORDS) {
          ReverseOrder2(p2, "W", 1, &p2);
        }
        if (pStructList->Flag & WE_HAVE_A_SCALE) {
          ReverseOrder2(p2, "W", 1, &p2);
        }
        else if (pStructList->Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
          ReverseOrder2(p2, "WW", 1, &p2);
        }
        else if (pStructList->Flag & WE_HAVE_A_TWO_BY_TWO) {
          ReverseOrder2(p2, "WWWW", 1, &p2);
        }
      }
      while (pStructList->Flag & MORE_COMPONENTS);
      if (pStructList->Flag & WE_HAVE_INSTRUCTIONS) {
        pFormat->NumberOfInstr = *(USHORT*)ReverseOrder2(p2, "W", 1, &p2);
        pFormat->Instructions = (BYTE*)(p2);
      }
      else {
        pFormat->NumberOfInstr = 0;
        pFormat->Instructions = NULL;
      }
    }
    return 0;
  }
}
int ttf_GetOffset(const ttffmt_t* s, int Index)
{
  if ((s)->loca->LongFormat) {
    return *((ULONG*)(s)->loca->pOffset + Index);
  }
  else {
    return *((USHORT*)(s)->loca->pOffset + Index) << 1;
  }
}
glyfHEADER* ttf_GetGlyfHeader(const ttffmt_t* s, int Index)
{
  int CurOffset = ttf_GetOffset(s, Index);
  int NextOffset = ttf_GetOffset(s, Index + 1);
  glyfHEADER* pHeader = 0;
  if (CurOffset != NextOffset) {
    pHeader = (glyfHEADER*)((char*)(s->glyf->glyfHeader) + CurOffset);
  }
  return pHeader;
}
int ttf_glyf_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  int i, glyfCount = s->maxp->NumGlyphs;
  glyfHEADER* glyfHeader = (glyfHEADER*)p;
  s->glyf->glyfHeader = glyfHeader;
  s->glyf->glyfCount = glyfCount;
  for (i = 0; i < glyfCount; i++) {
    int CurOffset = ttf_GetOffset(s, i);
    int NextOffset = ttf_GetOffset(s, i + 1);
    //If the font does not contain an outline for the missing character,
    //then the first and second offsets should have the same value.
    //This also applies to any other character without an outline, such as the space character.
    if (CurOffset != NextOffset) {
      glyfHeader = (glyfHEADER*)ReverseOrder2((char*)p + CurOffset, "WWWWW", 1, &p2);
      CreateAndReadGlyfInfo(glyfHeader);
    }
  }
  return 0;
}
int ttf_maxp_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  s->maxp = (maxpTableSTRUCT*)ReverseOrder2(p2, "DWWWWWWWWWWWWWW", 1, &p2);
  return 0;
}
int ttf_cvt_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  int Count = n / sizeof(FWORD);
  ReverseOrder2(p2, "W", Count, &p2);
  return 0;
}
int ttf_head_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  s->head = (headTableSTRUCT*)ReverseOrder2(p2, "DDDDWWDDDDWWWWWWWWW", 1, &p2);
  return 0;
}
int ttf_hhea_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  s->hhea = (hheaTableSTRUCT*)ReverseOrder2(p2, "DWWWWWWWWWWWWWWWW", 1, &p2);
  return 0;
}
int ttf_loca_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  //int NumGlyphs = maxp->MaxpTable.NumGlyphs;
  s->loca->LongFormat = s->head->IndexToLocFormat;
  s->loca->pOffset = p;
  if (s->loca->LongFormat) {
    //In order to compute the length of the last glyph element,
    //there is an extra entry after the last valid index.
    ReverseOrder2(p2, "D", n / sizeof(ULONG), &p2);
  }
  else {
    ReverseOrder2(p2, "W", n / sizeof(USHORT), &p2);
  }
  return 0;
}
int ttf_hmtx_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  int Num;
  s->hmtx->NumberOfHMetrics = s->hhea->NumberOfHMetrics;
  s->hmtx->NumberOfGlyphs = s->maxp->NumGlyphs;
  s->hmtx->hMetrics = (LongHorMETRIC*)ReverseOrder2(p2, "WW", s->hmtx->NumberOfHMetrics, &p2);
  Num = s->hmtx->NumberOfGlyphs - s->hmtx->NumberOfHMetrics;
  if (Num > 0) {
    s->hmtx->LeftSideBearing = (FWORD*)ReverseOrder2(p2, "W", Num, &p2);
  }
  else {
    s->hmtx->LeftSideBearing = NULL;
  }
  return 0;
}
int ttf_name_load(ttffmt_t* s, void* p, int n)
{
  char* p2;
  s->name->NameHeader = (nameTableHeader*)ReverseOrder2(p, "WWW", 1, &p2);
  s->name->NameRecords = (NameRECORD*)ReverseOrder2(p2, "WWWWWW", s->name->NameHeader->NumberOfRecord, &p2);
  s->name->pString = (char*)(p2);
  s->name->StrLen = (char*)p + n - s->name->pString;
  return 0;
}
int ttf_post_load(ttffmt_t* s, void* p, int n)
{
  char* p2 = (char*)p;
  postSimpleFORMAT* pCommonHeader = (postSimpleFORMAT*)ReverseOrder2(p2, "DDWWDDDDD", 1, &p2);
  if (pCommonHeader->FormatType == 0x00020000) {
    post20FORMAT* p20Format = (post20FORMAT*)pCommonHeader;
    USHORT* pNumberOfGlyphs = (USHORT*)ReverseOrder2(p2, "W", 1, &p2);
    USHORT* glyphNameIndex = (USHORT*)ReverseOrder2(p2, "W", p20Format->NumberOfGlyphs, &p2);
    //读剩余的名字表
    char* glyphNames = (char*)(glyphNameIndex + p20Format->NumberOfGlyphs);
    int NameLength = (char*)p + n - glyphNames;
  }
  else if (pCommonHeader->FormatType == 0x00020005) {
    post25FORMAT* p25Format = (post25FORMAT*)pCommonHeader;
    char* Offset = (char*)(pCommonHeader + 1);
    int NameLength = (char*)p + n - Offset;
  }
  return 0;
}
int ttf_init(ttffmt_t* s, void* p, int n)
{
  int i;
  char* p2;
  memset(s, 0, sizeof(ttffmt_t));
  bfinit(s->bf, p, n);
  s->pTtfHeader = (TtfHEADER*)ReverseOrder2(p, "DWWWW", 1, &p2);
  s->pTableEntry = (TableENTRY*)ReverseOrder2(p2, "BBBBDDD", s->pTtfHeader->numTables, &p2);
#define STRINT(a,b,c,d) (d<<24|c<<16|b<<8|a)
#define TTFTABLECASE(a,b,c,d, DO) case STRINT(a,b,c,d): DO; break;
#define STRINT1(s) ()
  ASSERT(STRINT('c', 'm', 'a', 'p') == *(int*)"cmap");
  for (i = 0; i < s->pTtfHeader->numTables; i++) {
    char* p1 = (char*)p + s->pTableEntry[i].Offset;
    int n1 = s->pTableEntry[i].Length;
    switch (s->pTableEntry[i].tagName) {
      TTFTABLECASE('c', 'm', 'a', 'p', ttf_cmap_load(s, p1, n1));
      TTFTABLECASE('h', 'e', 'a', 'd', ttf_head_load(s, p1, n1));
      TTFTABLECASE('h', 'h', 'e', 'a', ttf_hhea_load(s, p1, n1));
      TTFTABLECASE('m', 'a', 'x', 'p', ttf_maxp_load(s, p1, n1));
      TTFTABLECASE('n', 'a', 'm', 'e', ttf_name_load(s, p1, n1));
      TTFTABLECASE('p', 'o', 's', 't', ttf_post_load(s, p1, n1));
      TTFTABLECASE('O', 'S', '/', '2', ttf_OS2_load(s, p1, n1));
      TTFTABLECASE('c', 'v', 't', ' ', ttf_cvt_load(s, p1, n1));
      TTFTABLECASE('l', 'o', 'c', 'a', ttf_loca_load(s, p1, n1));
    }
  }
  for (i = 0; i < s->pTtfHeader->numTables; i++) {
    char* p1 = (char*)p + s->pTableEntry[i].Offset;
    int n1 = s->pTableEntry[i].Length;
    switch (s->pTableEntry[i].tagName) {
      TTFTABLECASE('g', 'l', 'y', 'f', ttf_glyf_load(s, p1, n1));
      TTFTABLECASE('h', 'm', 't', 'x', ttf_hmtx_load(s, p1, n1));
    }
  }
  return 0;
}
int ttf_load(const char* fn, ttffmt_t* ttf)
{
  buf_load(fn, ttf->bf);
  if (ttf->bf->data) {
    ttf_init(ttf, ttf->bf->data, ttf->bf->len);
  }
  return 0;
}
int ttf_free(ttffmt_t* ttf)
{
  bffree(ttf->bf);
  memset(ttf, 0, sizeof(ttffmt_t));
  return 0;
}
//取得最有可能作为显示名的namerecord索引
int ttf_GetNameIndex(const ttffmt_t* s)
{
  const nameTableSTRUCT* name = s->name;
  int i, NameIndex[] = { -1, -1, -1, -1};
  enum {UNI_DEFAULT = 0, UNI_ENGLISH, MAC_CHINESE, OTHER};
  for (i = 0; i < name->NameHeader->NumberOfRecord; i++) {
    if (name->NameRecords[i].NameId != 1) {
      continue;
    }
    if (name->NameRecords[i].PlatformId == 3 && name->NameRecords[i].EncodingId == 1) {
      if (name->NameRecords[i].LanguageId == GetSystemDefaultLCID()) {
        NameIndex[UNI_DEFAULT] = i;
      }
      else {
        NameIndex[UNI_ENGLISH] = i;
      }
    }
    else if (name->NameRecords[i].PlatformId == 1 && name->NameRecords[i].EncodingId == 0x19) {
      NameIndex[MAC_CHINESE] = i;
    }
    else {
      NameIndex[OTHER] = i;
    }
  }
  for (i = 0; i < 4; i++) {
    if (NameIndex[i] >= 0) {
      break;
    }
  }
  return NameIndex[i];
}
#define ttf_GetGlyfCount(s) (s)->maxp->NumGlyphs
#define ttf_GetUnitsPerEm(s) (s)->head->UnitsPerEm
int ttf_GetFontName(ttffmt_t* s, str_t* str)
{
  char* p2 = 0;
  nameTableSTRUCT* name = s->name;
  //如果BufLen为零返回所需要的内存大小
  int i = ttf_GetNameIndex(s);
  if (name->NameRecords[i].PlatformId == 3 && name->NameRecords[i].EncodingId == 1) {
    str_setsize(str, name->NameRecords[i].StrLen);
    memmove(str->s, name->pString + name->NameRecords[i].Offset, name->NameRecords[i].StrLen);
    //文件里的unicode编码是big endian,而内存中要求small endian
    ReverseOrder2(str->s, "W", name->NameRecords[i].StrLen / 2, &p2);
    str_wctomb(*str, str);
  }
  return 0;
}
int ttf_GetSimpleGlyph(const ttffmt_t* s, ExportGlyfSTRUCT* pExport, int Index)
{
  const glyfTableSTRUCT* glyf = s->glyf;
  glyfHEADER* glyfHeader = ttf_GetGlyfHeader(s, Index);
  assert(Index >= 0 && Index < glyf->glyfCount);
  if (glyfHeader->NumberOfContours <= 0) {
    return 0;
  }
  if (glyfHeader->NumberOfContours > 0) {
    glyfSimpleFORMAT pSimple[1] = {0};
    char* pXCoor, *pYCoor;
    GlyfPOINT* pPtx, *pPty;
    BYTE* pFlag;
    BYTE* p;
    int j;
    GetglyfSimpleFORMAT(glyfHeader, pSimple);
    pExport->PtCount = GetPtCountOfSimpleGlyph(glyfHeader->NumberOfContours, pSimple);
    REALLOC(USHORT, pExport->EndPtOfContours, glyfHeader->NumberOfContours);
    memcpy(pExport->EndPtOfContours, pSimple->EndPtsOfContours, sizeof(USHORT) * glyfHeader->NumberOfContours);
    REALLOC(GlyfPOINT, pExport->pPoints, pExport->PtCount);
    REALLOC(BYTE, pExport->Flags, pExport->PtCount);
    pExport->ContourCount = glyfHeader->NumberOfContours;
    pXCoor = (char*)pSimple->xCoordinates;
    pYCoor = (char*)pSimple->yCoordinates;
    pPtx = pExport->pPoints;
    pPty = pExport->pPoints;
    pFlag = pExport->Flags;
    p = pSimple->Flags;
    for (; (p - pSimple->Flags) < pSimple->LengthOfFlags; p++) {
      int Repeat;
      BYTE Flag = *p;
      if (Flag & 0x8) { //设置了重复位
        p++;
        Repeat = *p + 1;//*p表示flag重复次数
      }
      else {
        Repeat = 1;
      }
      for (j = 0; j < Repeat; j++) {
        *pFlag++ = Flag & 0x1;
        if (Flag & 0x2) {
          //取该点的x坐标，注意后续点是相对于前面点的坐标的，以下同
          pPtx->x = (pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x) + ((Flag & 0x10) ? * ((BYTE*)pXCoor) : - * ((BYTE*)pXCoor));
          pXCoor = (char*)pXCoor + 1;
        }
        else {
          //两字节表示
          if (Flag & 0x10) {
            pPtx->x = pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x; //跟前面值相同，取前面的值
          }
          else {
            //pPtx->x = (pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x) + *((SHORT*)pXCoor);
            //pXCoor = (SHORT*)pXCoor + 1;
            pPtx->x = (pPtx == pExport->pPoints ? 0 : (pPtx - 1)->x) + (SHORT)ReverseWord2(pXCoor, &pXCoor);
          }
        }
        pPtx++;
        if (Flag & 0x4) {
          pPty->y = (pPty == pExport->pPoints ? 0 : (pPty - 1)->y) + ((Flag & 0x20) ? * ((BYTE*)pYCoor) : - * ((BYTE*)pYCoor));
          pYCoor = (char*)pYCoor + 1;
        }
        else {
          //两字节表示
          if (Flag & 0x20) {
            pPty->y = pPty == pExport->pPoints ? 0 : (pPty - 1)->y; //跟前面值相同，取前面的值
          }
          else {
            //pPty->y = (pPty == pExport->pPoints ? 0 : (pPty - 1)->y) + *((SHORT*)pYCoor);
            //pYCoor = (SHORT*)pYCoor + 1;
            pPty->y = (pPty == pExport->pPoints ? 0 : (pPty - 1)->y) + (SHORT)ReverseWord2(pYCoor, &pYCoor);
          }
        }
        pPty++;
      }
    }
  }
  return 0;
}
int ttf_GetCompositeGlyph(const ttffmt_t* s, ExportGlyfSTRUCT* pExport, int Index)
{
  int glyfCount = s->maxp->NumGlyphs;
  glyfHEADER* glyfHeader = ttf_GetGlyfHeader(s, Index);
  //组合字形
  glyfCompositeFORMAT pFormat[1] = {0};
  int nStructList = 0;
  USHORT Flag;
  USHORT glyphIndex;
  char* p2 = (char*)(glyfHeader + 1);
  do {
    ExportGlyfSTRUCT GlyfStruct[1] = {0};
    int Arg1 = 0, Arg2 = 0, xScale = 1 << 14, yScale = 1 << 14, Scale01 = 0, Scale10 = 0;
    double m3[6] = {0};
    Flag = ReverseWord2(p2, &p2);
    glyphIndex = ReverseWord2(p2, &p2);
    nStructList++;
    ttf_GetSimpleGlyph(s, GlyfStruct, glyphIndex);
    if (Flag & ARG_1_AND_2_ARE_WORDS) {
      Arg1 = (short)ReverseWord2(p2, &p2);
      Arg2 = (short)ReverseWord2(p2, &p2);
    }
    else {
      Arg1 = *p2++;
      Arg2 = *p2++;
    }
    if (Flag & WE_HAVE_A_SCALE) {
      xScale = (SHORT)ReverseWord2(p2, &p2);
      yScale = xScale;
    }
    else if (Flag & WE_HAVE_AN_X_AND_Y_SCALE) {
      xScale = (SHORT)ReverseWord2(p2, &p2);
      yScale = (SHORT)ReverseWord2(p2, &p2);
    }
    else if (Flag & WE_HAVE_A_TWO_BY_TWO) {
      xScale = (SHORT)ReverseWord2(p2, &p2);
      Scale01 = (SHORT)ReverseWord2(p2, &p2);
      Scale10 = (SHORT)ReverseWord2(p2, &p2);
      yScale = (SHORT)ReverseWord2(p2, &p2);
    }
    V6SET(m3, xScale * 1. / (1 << 14), Scale10 * 1. / (1 << 14), Arg1, Scale01 * 1. / (1 << 14), yScale * 1. / (1 << 14), Arg2);
    if (0) {
      double a = M300(m3), c = M301(m3), b = M310(m3), d = M311(m3);
      double m = MAX(fabs(a), fabs(b));
      double n = MAX(fabs(c), fabs(d));
      M302(m3) *= m;
      M312(m3) *= n;
    }
    pt_trans(GlyfStruct->PtCount, GlyfStruct->pPoints, m3);
    //合并到输出的字形结构体中
    GlyphCat(pExport, GlyfStruct);
    free_ExportGlyfSTRUCT(GlyfStruct);
  }
  while (Flag & MORE_COMPONENTS);
  free_glyfCompositeFORMAT(pFormat);
  return 0;
}
int ttf_GetGlyph(const ttffmt_t* s, ExportGlyfSTRUCT* pExport, int Index)
{
  int glyfCount = s->maxp->NumGlyphs;
  glyfHEADER* glyfHeader = ttf_GetGlyfHeader(s, Index);
  assert(Index < glyfCount);
  if (NULL == glyfHeader) {
    return 0;
  }
  if (glyfHeader->NumberOfContours > 0) {
    //简单字形
    ttf_GetSimpleGlyph(s, pExport, Index);
  }
  else {
    ttf_GetCompositeGlyph(s, pExport, Index);
  }
  if (1) {
    double m3[6] = {0};
    V6SET(m3, 1, 0, , -glyfHeader->xMin, 0, 1, -glyfHeader->yMin);
    pt_trans(pExport->PtCount, pExport->pPoints, m3);
  }
  return 0;
}
#include "math\math.def"
#include "img\edge.inl"
int imdrawaa_ttf_index(int h, int w, uchar* img, int step, int cn, IRECT rc, ttffmt_t* s, int GlyfIndex, COLOR crFill, COLOR crLine, double wline)
{
  ExportGlyfSTRUCT Glyph[1] = {0};
  //GlyfIndex=1281;
  if (GlyfIndex < 0 || GlyfIndex >= s->maxp->NumGlyphs) {
    return 0;
  }
  ttf_GetGlyph(s, Glyph, GlyfIndex);
  if (Glyph->PtCount > 0) {
    //先纠正并转换点的坐标
    double UnitPerEm = s->head->UnitsPerEm;
    double sx = RCW(&rc) / UnitPerEm, sy = RCH(&rc) / UnitPerEm;
    double m3[6] = {sx, 0, rc.l, 0, -sy, rc.b};
    //double m3[6] = {sx, 0, rc.left, 0, -sy, rc.top + sy*(s->head->yMax+s->head->yMin)};
    pt_trans(Glyph->PtCount, Glyph->pPoints, m3);
    imdrawaa_glyph(h, w, img, step, cn, Glyph, crFill, crLine, wline);
  }
  free_ExportGlyfSTRUCT(Glyph);
  return 0;
}
FormatHEADER* ttf_cmap_find_UGI(ttffmt_t* s)
{
  int i, j;
  const cmapTableSTRUCT* cmap = s->cmap;
  EncodingMapENTRY1* MapEntries = (EncodingMapENTRY1*)(cmap + 1);
  for (i = 0; i < cmap->TableNumber; ++i) {
    int has = 0;
    for (j = 0; j < i; j++) {
      if (MapEntries[i].Offset == MapEntries[j].Offset) {
        //MapEntries[i].CodingMapId = j;
        has = 1;
        break;
      }
    }
    if (has) {
      continue;
    }
    // Micrsoft系UGI代码体系
    if (MapEntries[i].PlatformId == 3 && MapEntries[i].EncodingId == 1) {
      FormatHEADER* pHeader = (FormatHEADER*)((char*)cmap + MapEntries[i].Offset);
      return pHeader;
    }
  }
  return 0;
}
int ttf_cmap(ttffmt_t* s, int high, int low)
{
  int index = 0;
  FormatHEADER* pHeader = ttf_cmap_find_UGI(s);
  char* p2 = (char*)(pHeader + 1);
  if (NULL == pHeader) {
    return 0;
  }
  switch (pHeader->Format) {
  case 0:
    if (1) {
      EncodingMapFORMAT0* pFormat = (EncodingMapFORMAT0*)pHeader;
      return 0;
    }
  case 2:
    if (1) {
      int i, Len;
      SubHEADER* SubHeaders;
      EncodingMapFORMAT2* pFormat = (EncodingMapFORMAT2*)pHeader;
      USHORT high_map_key;
      USHORT firstCode, endCode = 0;
      SubHEADER* subHeader;
      USHORT* subArray;
      int index = 0;
      ReverseOrder2(p2, "W", 256, &p2);
      //因为subheaderkeys存放subheader的index*8
      Len = (pFormat->SubHeaderKeys[255] >> 3) + 1;
      SubHeaders = (SubHEADER*)(pFormat->SubHeaderKeys + 256);
      //读每个子区段
      for (i = 0; i < Len; i++) {
        SubHeaders = (SubHEADER*)ReverseOrder2(p2, "WWWW", 1, &p2);
        ReverseOrder2(p2 + SubHeaders->idRangeOffset - sizeof(USHORT), "W", SubHeaders->entryCount, &p2);
      }
      high_map_key = pFormat->SubHeaderKeys[high] / 8; //得到subHeaders序号
      if (high_map_key == 0) { //subHeaders序号为0，为西文字符
        low = high;
      }
      subHeader = pFormat->SubHeaders + high_map_key;
      firstCode = subHeader->firstCode;
      //firstCode = firstCode - subHeader->entryCount - 1; //低字节有效范围为 firstCode
      //endCode = subHeader->endCode;
      //到endCode
      if ((low > endCode) || (low < firstCode)) {
        index = 0; //低字节不在有效范围之内，返回文字序号0
      }
      else {
        subArray = subHeader->idRangeOffset + &(subHeader->idRangeOffset);
        //得到低字节映射数组的首地址subArray[]
        index = subArray[low - firstCode]; //得到 index初值
        if (index != 0) { //调整映射值 index
          index = index + subHeader->idDelta;
        }
      }
      return index;
    }
  case 4:
    if (1) {
      USHORT* endCode; //[segCount] End characterCode for each segment,last =0xFFFF.
      USHORT* reservedPad; //Set to 0.
      //USHORT* RevPad; //这里添加一个USHORT为文件4字节对齐, 注意文件里并无该域
      USHORT* startCode; //[segCount] Start character code for each segment.
      SHORT* idDelta; //[segCount] Delta for all character codes in segment.
      USHORT* idRangeOffset; //[segCount] Offsets into glyphIdArray or 0
      //USHORT* IndexArray; //Glyph index array (arbitrary length)
      EncodingMapFORMAT4* pFormat = (EncodingMapFORMAT4*)pHeader;
      int SegCount, i;
      USHORT char_code = high << 8 | low;
      PTR3(p2, USHORT, 4);
      SegCount = pFormat->segCountX2 >> 1;
      endCode = PTR3(p2, USHORT, SegCount);
      reservedPad = PTR3(p2, USHORT, 1);
      startCode = PTR3(p2, USHORT, SegCount);
      idDelta = PTR3(p2, SHORT, SegCount);
      idRangeOffset = PTR3(p2, USHORT, SegCount);
      index = 0;
      for (i = 0 ; i < SegCount; i++) { //确定char_code所在的段
        if (char_code < startCode[i]) { //如果char_code不在此段中，返回0
          break;
        }
        else if (char_code <= endCode[i]) {
          if (0 == idRangeOffset[i]) {
            index = char_code + idDelta[i];
          }
          else {
            index = *(idRangeOffset[i] / 2 + (char_code - startCode[i]) + &idRangeOffset[i]);
          }
        }
      }
      return index;
    }
  case 6:
    if (1) {
      EncodingMapFORMAT6* pFormat = (EncodingMapFORMAT6*)pHeader;
      ReverseOrder2(p2, "W", 2, &p2);
      ReverseOrder2(p2, "W", pFormat->entryCount, &p2);
      return 0;
    }
  default: //未知格式
    Throw_CustomException(ErrStr[0]);
  }
  return 0;
}
int test_ttf()
{
  int i;
  ttffmt_t ttf[1] = {0};
  ExportGlyfSTRUCT ggg[1] = {0};
  const char* fn;
  str_t sname[1] = {0};
  int ch = 7, t = 0;
  unsigned short www[1];
  _chdir("D:\\code\\c\\Font\\Fonts2");
  fn = "arial.ttf";
  fn = "simfang.ttf";
  fn = "YaHei.Consolas.1.11b.ttf";
  fn = "庞中华行书.ttf";
  fn = "華康少女字W6.ttf";
  ch = 1281;
#if 0
  TtfDOC pttf[1] = {0};
  LoadTtfDoc(pttf, fn);
  pttf->GetGlyph(ggg, ch);
  for (i = 0; i < ggg->PtCount; ++i) {
    printf("%d %d\n", ggg->pPoints[i].x, ggg->pPoints[i].y);
  }
  printf("===================\n");
  free_ExportGlyfSTRUCT(ggg);
#endif
  ttf_load(fn, ttf);
  chr_mbtowc("字", 2, www, 1);
  chr_mbtowc("乁", 2, www, 1);
  t = ttf_cmap(ttf, www[0] >> 8, www[0] & 0xff);
  ttf_GetFontName(ttf, sname);
  ttf_GetGlyph(ttf, ggg, ch);
  for (i = 0; i < ggg->PtCount; ++i) {
    printf("%.f %.f\n", ggg->pPoints[i].x, ggg->pPoints[i].y);
  }
  printf("===================\n");
  free_ExportGlyfSTRUCT(ggg);
  str_free(sname);
  ttf_free(ttf);
  return 0;
}

