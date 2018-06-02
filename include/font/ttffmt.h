
#pragma pack(push)//保存对齐状态
#pragma pack(1)

typedef DWORD F16DOT16;
typedef DWORD FUNIT;
typedef short FWORD;
typedef WORD UFWORD;
typedef short F2DOT14;
typedef short SHORT;
typedef unsigned short USHORT;

typedef struct TtfHEADER {
  F16DOT16 Version; //0x00010000 for version 1.0.
  USHORT numTables; //Number of tables.
  USHORT searchRange; //(Maximum power of 2 ￡ numTables) x 16.
  USHORT entrySelector; //Log2(maximum power of 2 ￡ numTables).
  USHORT rangeShift; //NumTables x 16-searchRange.
} TtfHEADER;

typedef struct TableENTRY {
  ULONG tagName; //4 -byte identifier.
  ULONG CheckSum; //CheckSum for this table.
  ULONG Offset; //Offset from beginning of TrueType font file.
  ULONG Length; //Length of this table.
} TableENTRY;

typedef struct EncodingMapENTRY1 {
  USHORT PlatformId;
  USHORT EncodingId;
  ULONG Offset; //from beginning of this table
} EncodingMapENTRY1;

//header for arbitrary format
typedef struct FormatHEADER {
  USHORT Format;
  USHORT Length;
  USHORT Version;
} FormatHEADER;

//format0 typedef struct
typedef struct EncodingMapFORMAT0 {
  USHORT Format;
  USHORT Length;
  USHORT Version;
  BYTE IndexArray[256];
} EncodingMapFORMAT0;

//format2 typedef struct
typedef struct SubHEADER {
  USHORT firstCode; //First valid low byte for this subHeader.
  USHORT entryCount; //Number of valid low bytes for this subHeader.
  SHORT idDelta;
  USHORT idRangeOffset;
} SubHEADER;

typedef struct EncodingMapFORMAT2 {
  USHORT Format;
  USHORT Length;
  USHORT Version;
  USHORT SubHeaderKeys[256];
  SubHEADER* SubHeaders;
  USHORT** IndexArray; //索引表
  //int IndexCount; //Number Of Index，程序添加，不属于TTF文件内容
} EncodingMapFORMAT2;

//format4 typedef struct
typedef struct EncodingMapFORMAT4 {
  USHORT Format;
  USHORT Length;
  USHORT Version;

  USHORT segCountX2; //2 x segCount.
  USHORT searchRange; //2 x (2**floor(log2(segCount)))
  USHORT entrySelector; //log2(searchRange/2)
  USHORT rangeShift; //2 x segCount - searchRange

  USHORT* endCode; //[segCount] End characterCode for each segment,last =0xFFFF.

  USHORT reservedPad; //Set to 0.
  USHORT* startCode; //[segCount] Start character code for each segment.
  USHORT* idDelta; //[segCount] Delta for all character codes in segment.
  USHORT* idRangeOffset; //[segCount] Offsets into glyphIdArray or 0
  USHORT** IndexArray; //Glyph index array (arbitrary length)

  //int IndexCount; //Number Of Index，程序添加，不属于TTF文件内容
} EncodingMapFORMAT4;

//format6 typedef struct
typedef struct EncodingMapFORMAT6 {
  USHORT Format;
  USHORT Length;
  USHORT Version;
  USHORT firstCode; //First character code of subrange.
  USHORT entryCount; //Number of character codes in subrange.
  USHORT* glyphIdArray; //[entryCount] Array of glyph index values for character codes in the range.
} EncodingMapFORMAT6;

typedef struct cmapTableSTRUCT {
  USHORT TableVersion;
  USHORT TableNumber;
} cmapTableSTRUCT;

//for head
typedef struct headTableSTRUCT {
  F16DOT16 TableVersion;
  F16DOT16 FontRevision;
  ULONG CheckSumAdjustment;
  ULONG MagicNumber;
  USHORT Flags;
  USHORT UnitsPerEm;
  BYTE CreatedTime[8]; //International date (8-byte field).
  BYTE ModifiedTime[8]; //International date (8-byte field).
  FWORD xMin;
  FWORD yMin;
  FWORD xMax;
  FWORD yMax;
  USHORT MacStyle;
  USHORT LowestRecPPEM;
  SHORT FontDirectionHint;
  SHORT IndexToLocFormat;
  SHORT glyphDataFormat;
  SHORT RevPad; //为4字节对齐设置
} headTableSTRUCT;

//for maxp
typedef struct maxpTableSTRUCT {
  F16DOT16 TableVersion;// number 0x00010000 for version 1.0.
  USHORT NumGlyphs; // The number of glyphs in the font.
  USHORT MaxPoints; // Maximum points in a non-composite glyph.
  USHORT MaxContours; // Maximum contours in a non-composite glyph.
  USHORT MaxCompositePoints; //Maximum points in a composite glyph.
  USHORT MaxCompositeContours;//Maximum contours in a composite glyph.
  USHORT MaxZones; //1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0; should be set to 2 in most cases.
  USHORT MaxTwilightPoints; //Maximum points used in Z0.
  USHORT MaxStorage; //Number of Storage Area locations.
  USHORT MaxFunctionDefs; //Number of FDEFs.
  USHORT MaxInstructionDefs; //Number of IDEFs.
  USHORT MaxStackElements; //Maximum stack depth .
  USHORT MaxSizeOfInstructions; //Maximum byte count for glyph instructions.
  USHORT MaxComponentElements; //Maximum number of components referenced at "top level" for any composite glyph.
  USHORT MaxComponentDepth; //Maximum levels of recursion; 1 for simple components.
} maxpTableSTRUCT;

//for hhea
typedef struct hheaTableSTRUCT {
  F16DOT16 TableVersion; //number 0x00010000 for version 1.0.
  FWORD Ascender; //Typographic ascent.
  FWORD Descender; //Typographic descent.
  FWORD LineGap; //Typographic line gap. Negative LineGap values are treated as zero in Windows 3.1, System 6, and System 7.
  UFWORD AdvanceWidthMax; //Maximum advance width value in 'hmtx' table.
  FWORD MinLeftSideBearing; //Minimum left sidebearing value in 'hmtx' table.
  FWORD MinRightSideBearing; //Minimum right sidebearing value; calculated as Min(aw - lsb - (xMax - xMin)).
  FWORD xMaxExtent; //Max(lsb + (xMax - xMin)).
  SHORT CaretSlopeRise; //Used to calculate the slope of the cursor (rise/run); 1 for vertical.
  SHORT CaretSlopeRun; //0 for vertical.
  SHORT Reserved[5]; //(reserved) set to 0
  SHORT MetricDataFormat; //0 for current format.
  USHORT NumberOfHMetrics; //Number of hMetric entries in 'hmtx' table; may be smaller than the total number of glyphs in the font.
} hheaTableSTRUCT;
typedef struct locaTableSTRUCT {
  BOOL LongFormat; //用来保存数组元素类型
  //int NumGlyphs; //保存glyph个数
  void* pOffset;
} locaTableSTRUCT;

//for hmtx
typedef struct LongHorMETRIC {
  UFWORD AdvanceWidth;
  FWORD lsb;
} LongHorMETRIC;

typedef struct hmtxTableSTRUCT {
  int NumberOfHMetrics;
  int NumberOfGlyphs;
  LongHorMETRIC* hMetrics; //numOfHMetrics comes from the 'hhea' table.
  FWORD* LeftSideBearing; //The number of entries in this array is derived from numGlyphs (from 'maxp' table) minus numberOfHMetrics.
} hmtxTableSTRUCT;
// for os/2
#ifndef WIN32
typedef struct PANOSE {
  BYTE bFamilyType;
  BYTE bSerifStyle;
  BYTE bWeight;
  BYTE bProportion;
  BYTE bContrast;
  BYTE bStrokeVariation;
  BYTE bArmStyle;
  BYTE bLetterform;
  BYTE bMidline;
  BYTE bXHeight;
} PANOSE;
#endif // WIN32

typedef struct OS2TableSTRUCT {
  USHORT Version; //0x0001
  SHORT xAvgCharWidth;
  USHORT WeightClass;
  USHORT WidthClass;
  SHORT Type;
  SHORT ySubscriptXSize;
  SHORT ySubscriptYSize;
  SHORT ySubscriptXOffset;
  SHORT ySubscriptYOffset;
  SHORT ySuperscriptXSize;
  SHORT ySuperscriptYSize;
  SHORT ySuperscriptXOffset;
  SHORT ySuperscriptYOffset;
  SHORT yStrikeoutSize;
  SHORT yStrikeoutPosition;
  SHORT FamilyClass;
  PANOSE Panose;
  //BYTE RevPad[2]; //为四字节对齐设置，读写文件时将齐排除在外

  ULONG UnicodeRange1; //Bits 0-31
  ULONG UnicodeRange2; //Bits 32-63
  ULONG UnicodeRange3; //Bits 64-95
  ULONG UnicodeRange4; //Bits 96-127
  CHAR AchVendID[4];
  USHORT Selection;
  USHORT FirstCharIndex;
  USHORT LastCharIndex;
  USHORT TypoAscender;
  USHORT TypoDescender;
  USHORT TypoLineGap;
  USHORT WinAscent;
  USHORT WinDescent;
  ULONG CodePageRange1; //Bits 0-31
  ULONG CodePageRange2; //Bits 32-63
} OS2TableSTRUCT;

//for post
typedef struct postSimpleFORMAT {
  F16DOT16 FormatType;
  F16DOT16 ItalicAngle;
  FWORD UnderlinePosition;
  FWORD UnderlineThickness;
  ULONG IsFixedPitch;
  ULONG MinMemType42;
  ULONG MaxMemType42;
  ULONG MinMemType1;
  ULONG MaxMemType1;
} postSimpleFORMAT;

typedef struct post20FORMAT {
  postSimpleFORMAT CommonHeader;
  USHORT NumberOfGlyphs;
  USHORT RevPad; //为4字节对齐设置
  USHORT* glyphNameIndex;
  char* glyphNames;
  ULONG NameLength; //程序添加用来记录glyphNames长度不属于ttf文件
} post20FORMAT;

typedef struct post25FORMAT {
  postSimpleFORMAT CommonHeader;
  char* Offset;
  ULONG NameLength; //程序添加用来记录glyphNames长度不属于ttf文件
} post25FORMAT;

//for name
typedef struct NameRECORD {
  USHORT PlatformId;
  USHORT EncodingId;
  USHORT LanguageId;
  USHORT NameId;
  USHORT StrLen;
  USHORT Offset; //from start of storage area(in bytes)
} NameRECORD;

typedef struct nameTableHeader {
  USHORT FormatSelector;
  USHORT NumberOfRecord;
  USHORT OffsetOfString;
} nameTableHeader;
typedef struct nameTableSTRUCT {
  nameTableHeader* NameHeader;
  NameRECORD* NameRecords;
  char* pString;
  int StrLen;
} nameTableSTRUCT;
//for glyf
typedef struct glyfHEADER {
  SHORT NumberOfContours; //If the number of contours is greater than or equal to zero, this is a single glyph; if negative, this is a composite glyph.
  FWORD xMin; //Minimum x for coordinate data.
  FWORD yMin; //Minimum y for coordinate data.
  FWORD xMax; //Maximum x for coordinate data.
  FWORD yMax; //Maximum y for coordinate data.
  //WORD RevPad; //为结构4字节对齐添加的一个WORD
} glyfHEADER;

typedef struct glyfSimpleFORMAT {
  USHORT* EndPtsOfContours; //Array of last points of each contour; n is the number of contours.
  USHORT InstructionLength; //Total number of bytes for instructions.
  //USHORT RevPad; //为4字节对齐添加
  BYTE* Instructions; //Array of instructions for each glyph; n is the number of instructions.
  BYTE* Flags; //Array of flags for each coordinate in outline; n is the number of flags.

  void* xCoordinates; //First coordinates relative to (0,0); others are relative to previous point.
  void* yCoordinates; //First coordinates relative to (0,0); others are relative to previous point.

  //以下成员是为程序分析方便添加的，不属于ttf文件部分
  int LengthOfFlags; //保存flags数组的长度
  char* pxFormat; //用来描述x坐标的格式每字符代表一个x坐标值,B:byte,W:Short
  char* pyFormat; //用来描述y坐标的格式每字符代表一个y坐标值,B:byte,W:Short
} glyfSimpleFORMAT;

typedef struct glyfCompositeSTRUCT {
  USHORT Flag;
  USHORT glyphIndex;
  SHORT Argument;
  SHORT Argument2;
  F2DOT14 xScale;
  F2DOT14 Scale01;
  F2DOT14 Scale10;
  F2DOT14 yScale;
} glyfCompositeSTRUCT;

typedef struct glyfCompositeFORMAT {
  glyfCompositeSTRUCT* pStructList;
  USHORT NumberOfInstr;
  USHORT RevPad; //为4字节对齐设置
  BYTE* Instructions;
  int nStructList;
} glyfCompositeFORMAT;

int free_glyfCompositeFORMAT(glyfCompositeFORMAT* pFormat)
{
  FREE(pFormat->pStructList);
  FREE(pFormat->Instructions);
  memset(pFormat, 0, sizeof(glyfCompositeFORMAT));
  return 0;
}

typedef Point2d GlyfPOINT;

typedef struct ExportGlyfSTRUCT {
  GlyfPOINT* pPoints;
  BYTE* Flags;
  USHORT* EndPtOfContours;
  USHORT PtCount;
  USHORT ContourCount;
} ExportGlyfSTRUCT;

int free_ExportGlyfSTRUCT(ExportGlyfSTRUCT* s)
{
  FREE(s->pPoints);
  FREE(s->Flags);
  FREE(s->EndPtOfContours);
  memset(s, 0, sizeof(ExportGlyfSTRUCT));
  return 0;
}

enum {
  G_ONCURVE = 0x01, // on curve ,off curve
  G_REPEAT = 0x08, //next byte is flag repeat count
  G_XMASK = 0x12,
  G_XADDBYTE = 0x12, //X is positive byte
  G_XSUBBYTE = 0x12, //X is negative byte
  G_XSAME = 0x10, //X is same
  G_XADDINT = 0x00, //X is signed word

  G_YMASK = 0x24,
  G_YADDBYTE = 0x24, //Y is positive byte
  G_YSUBBYTE = 0x04, //Y is negative byte
  G_YSAME = 0x20 , //Y is same
  G_YADDINT = 0x00, //Y is signed word
};

enum {
  ARG_1_AND_2_ARE_WORDS = 0x1, //If this is set, the arguments are words; otherwise, they are bytes.
  ARGS_ARE_XY_VALUES = 0x2, //If this is set, the arguments are xy values; otherwise, they are points.
  ROUND_XY_TO_GRID = 0x4, //For the xy values if the preceding is true.
  WE_HAVE_A_SCALE = 0x8, //This indicates that there is a simple scale for the component. Otherwise, scale = 1.0.
  RESERVED = 0x10, //This bit is reserved. Set it to 0.
  MORE_COMPONENTS = 0x20, //Indicates at least one more glyph after this one.
  WE_HAVE_AN_X_AND_Y_SCALE = 0x40, //The x direction will use a different scale from the y direction.
  WE_HAVE_A_TWO_BY_TWO = 0x80, //There is a 2 by 2 transformation that will be used to scale the component.
  WE_HAVE_INSTRUCTIONS = 0x100, //Following the last component are instructions for the composite character.
  USE_MY_METRICS = 0x200,
};

typedef struct glyfTableSTRUCT {
  int glyfCount;
  glyfHEADER* glyfHeader;
} glyfTableSTRUCT;

typedef struct ttffmt_t {
  buf_t bf[1];
  TtfHEADER* pTtfHeader;
  TableENTRY* pTableEntry;
  headTableSTRUCT* head;
  maxpTableSTRUCT* maxp;
  hheaTableSTRUCT* hhea;
  cmapTableSTRUCT* cmap;
  locaTableSTRUCT loca[1];
  hmtxTableSTRUCT hmtx[1];
  nameTableSTRUCT name[1];
  glyfTableSTRUCT glyf[1];
} ttffmt_t;

#pragma pack(pop)//恢复对齐状态

#define Throw_StdCrtException() (0)
#define Throw_CustomException(x) (0)


double F2DOT14ToDouble(F2DOT14 FixedNum)
{
  int Mantissa[] = {0, 1, -2, -1};
  return Mantissa[FixedNum >> 14] + (FixedNum & 0x3fff) / 16384.;
}

WORD ReverseWord(WORD Word)
{
  return ((Word << 8) | (Word >> 8));
}

DWORD ReverseDWord(DWORD DWord)
{
  return ((DWord << 24) | (DWord >> 24) | ((DWord >> 8) & 0xff00) | ((DWord << 8) & 0xff0000));
}

BYTE ReverseByte2(void* p, char** pp)
{
  BYTE Word = *(BYTE*)p;
  *pp = (char*)((BYTE*)p + 1);
  return (Word);
}

WORD ReverseWord2(void* p, char** pp)
{
  WORD Word = *(WORD*)p;
  *pp = (char*)((WORD*)p + 1);
  return ((Word << 8) | (Word >> 8));
}

DWORD ReverseDWord2(void* p, char** pp)
{
  DWORD DWord = *(DWORD*)p;
  *pp = (char*)((DWORD*)p + 1);
  return ((DWord << 24) | (DWord >> 24) | ((DWord >> 8) & 0xff00) | ((DWord << 8) & 0xff0000));
}

//转换结构体数据成员字节顺序，主要因为ttf文件的非单字节数据类型和内存中相
//反。pDataFrame是数据格式，表示方法如：
//格式序列WWD表示如下结构：
//typedef struct
//{
// WORD a;
// WORD b;
// DWORD c;
//}
void* ReverseOrder(void* pBuf, const char* pDataFrame, int Count)
{
  int i;
  const char* pFrame;
  char* pBuf0 = (char*)pBuf;

  for (i = 0; i < Count; i++) {
    for (pFrame = pDataFrame; *pFrame; pFrame++) {
      if (*pFrame == 'W') {
        *(WORD*)pBuf = ReverseWord(*(WORD*)pBuf);
        pBuf = (WORD*)pBuf + 1;
      }
      else if (*pFrame == 'D') {
        *(DWORD*)pBuf = ReverseDWord(*(DWORD*)pBuf);
        pBuf = (DWORD*)pBuf + 1;
      }
      else if (*pFrame == 'B') {
        pBuf = (char*)pBuf + 1;
      }
      else {
        assert(0);
      }
    }
  }

  return pBuf0;
}

void* ReverseOrder2(void* pBuf, const char* pDataFrame, int Count, char** ppBuf)
{
  int i;
  const char* pFrame;
  char* pBuf0 = (char*)pBuf;

  for (i = 0; i < Count; i++) {
    for (pFrame = pDataFrame; *pFrame; pFrame++) {
      if (*pFrame == 'W') {
        *(WORD*)pBuf = ReverseWord(*(WORD*)pBuf);
        pBuf = (WORD*)pBuf + 1;
      }
      else if (*pFrame == 'D') {
        *(DWORD*)pBuf = ReverseDWord(*(DWORD*)pBuf);
        pBuf = (DWORD*)pBuf + 1;
      }
      else if (*pFrame == 'B') {
        pBuf = (char*)pBuf + 1;
      }
      else {
        assert(0);
      }
    }
  }

  *ppBuf = (char*)pBuf;
  return pBuf0;
}

int GetDataFrameLength(const char* pFrame)
{
  int Size = 0;
  const char* p;

  for (p = pFrame; *p; p++) {
    if (*p == 'B' || *p == 'b') { //BYTE
      Size += 1;
    }
    else if (*p == 'W' || *p == 'w') { //WORD
      Size += 2;
    }
    else if (*p == 'D' || *p == 'd') { //DWORD
      Size += 4;
    }
    else { //不支持得格式字符
      assert(0);
    }
  }

  return Size;
}

int GlyphCat(ExportGlyfSTRUCT* pDest, const ExportGlyfSTRUCT* pStruct)
{
  int i;
  MEMCAT(USHORT, pDest->EndPtOfContours, pDest->ContourCount, pStruct->EndPtOfContours, pStruct->ContourCount);
  MEMCAT(BYTE, pDest->Flags, pDest->PtCount, pStruct->Flags, pStruct->PtCount);
  MEMCAT(GlyfPOINT, pDest->pPoints, pDest->PtCount, pStruct->pPoints, pStruct->PtCount);

  for (i = pDest->ContourCount; i < pDest->ContourCount + pStruct->ContourCount; i++) {
    pDest->EndPtOfContours[i] += pDest->PtCount;
  }

  pDest->ContourCount += pStruct->ContourCount;
  pDest->PtCount += pStruct->PtCount;
  return 0;
}

#define pt2dbl(pt) cPoint2d(pt.x, pt.y)

int vcgen_curve_n(int n, Point2d* pt, int ContourLen, const GlyfPOINT* pPt, int Start, int End)
{
  Point2d p0, p1, p2;
  int k;

  if (Start >= 0) {
    p0 = pt2dbl(pPt[Start]);
    pt[n++] = (pPt[Start]);
    k = Start + 1;
  }
  else {
    p0 = cPoint2d((pPt[0].x + pPt[ContourLen - 1].x) / 2., (pPt[0].y + pPt[ContourLen - 1].y) / 2.);
    pt[n++] = (pPt[0]);
    k = 0;
    End = ContourLen;
  }

  for (; k < End; ++k) {
    int k0 = k % ContourLen;
    int k1 = (k + 1) % ContourLen;
    p1 = (pPt[k0]);
    //char buf[256]; _snprintf(buf, 256, "%d", k0); draw_textPt(hDC, buf, -1, pPt[k0].x, pPt[k0].y, 0, _RGB(0,0,0));
    p2 = (pPt[k1]);

    if (k < End - 1 || Start < 0) {
      p2 = cPoint2d((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    }
    else {
      int asdf = 0;
    }

    n--;
    n += vcgen_curve3_div(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, pt + n);
    p0 = p2;
  }

  return n;
}
int glyph_to_poly2d(const ExportGlyfSTRUCT* Glyph, poly2d_t* po)
{
  Point2d* pt = 0;
  int* len = 0;
  int i, j, n = 0, m = 0, nn = 0;
  USHORT* pEndPt = Glyph->EndPtOfContours;
  GlyfPOINT* pPt = Glyph->pPoints;
  int ContourCount = Glyph->ContourCount;
  int PtCount = Glyph->PtCount, PreEnd = -1;
  BYTE* Flags = Glyph->Flags;

  //char buf[256];
  if (PtCount <= 0) {
    return 0;
  }

  pt = MALLOC(Point2d, 32 * PtCount);
  len = MALLOC(int, 2 * ContourCount);

  for (i = 0; i < ContourCount; ++i) {
    int s = -1, ContourLen = pEndPt[i] - PreEnd, Start = 0;
    int n0 = n;

    for (j = 0; j < ContourLen; ++j) {
      if (Flags[j]) {
        s = j;
        break;
      }
    }

    Start = s;

    ASSERT(n < 32 * PtCount);

    if (Start >= 0) {
      pt[n++] = (pPt[Start]);

      for (j = s + 1; j < ContourLen + s; ++j) {
        int j0 = j % ContourLen;

        if (Flags[j0]) {
          if (1 == j - Start) {
            pt[n++] = (pPt[j0]);
            //char buf[256]; _snprintf(buf, 256, "%d", j0); draw_textPt(hDC, buf, -1, pPt[j0].x, pPt[j0].y, 0, _RGB(0,0,0));
          }
          else {
            n = vcgen_curve_n(n, pt, ContourLen, pPt, Start, j);
          }

          Start = j;
        }
      }

      if (Start < j) {
        n = vcgen_curve_n(n - 1, pt, ContourLen, pPt, Start, j);
      }
    }
    else {
      n = vcgen_curve_n(n, pt, ContourLen, pPt, -1, ContourLen);
    }

    ASSERT(n < 32 * PtCount);
    PreEnd = pEndPt[i];
    pPt += ContourLen;
    Flags += ContourLen;
    len[m++] = n - n0;
    nn += len[m - 1];
  }

  poly2d_setsize(po, m, nn);
  MEMCPY(po->pt, pt, nn);
  MEMCPY(po->len, len, m);
  FREE(pt);
  FREE(len);
  return 0;
}

int imdrawaa_glyph(int h, int w, uchar* img, int step, int cn, const ExportGlyfSTRUCT* Glyph, COLOR crFill, COLOR crLine, double wline)
{
  Point2d* pt = 0;
  int* len = 0;
  int i, j, n = 0, m = 0, nn = 0;
  USHORT* pEndPt = Glyph->EndPtOfContours;
  GlyfPOINT* pPt = Glyph->pPoints;
  int ContourCount = Glyph->ContourCount;
  int PtCount = Glyph->PtCount, PreEnd = -1;
  BYTE* Flags = Glyph->Flags;

  //char buf[256];
  if (PtCount <= 0) {
    return 0;
  }

  pt = MALLOC(Point2d, 32 * PtCount);
  len = MALLOC(int, 2 * ContourCount);

  for (i = 0; i < ContourCount; ++i) {
    int s = -1, ContourLen = pEndPt[i] - PreEnd, Start = 0;
    int n0 = n;

    for (j = 0; j < ContourLen; ++j) {
      if (Flags[j]) {
        s = j;
        break;
      }
    }

    Start = s;

    ASSERT(n < 32 * PtCount);

    if (Start >= 0) {
      pt[n++] = (pPt[Start]);

      for (j = s + 1; j < ContourLen + s; ++j) {
        int j0 = j % ContourLen;

        if (Flags[j0]) {
          if (1 == j - Start) {
            pt[n++] = (pPt[j0]);
            //char buf[256]; _snprintf(buf, 256, "%d", j0); draw_textPt(hDC, buf, -1, pPt[j0].x, pPt[j0].y, 0, _RGB(0,0,0));
          }
          else {
            n = vcgen_curve_n(n, pt, ContourLen, pPt, Start, j);
          }

          Start = j;
        }
      }

      if (Start < j) {
        n = vcgen_curve_n(n - 1, pt, ContourLen, pPt, Start, j);
      }
    }
    else {
      n = vcgen_curve_n(n, pt, ContourLen, pPt, -1, ContourLen);
    }

    ASSERT(n < 32 * PtCount);
    PreEnd = pEndPt[i];
    pPt += ContourLen;
    Flags += ContourLen;
    len[m++] = n - n0;
    nn += len[m - 1];
  }

  imdrawaa_polygon(h, w, img, step, cn, pt, len, m, crFill, crLine, wline);
  FREE(pt);
  FREE(len);
  return 0;
}
