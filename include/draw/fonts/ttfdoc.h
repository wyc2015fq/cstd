
#pragma pack(push)//保存对齐状态
#pragma pack(1)

double F2DOT14ToDouble(F2DOT14 Fixed);
#define HEADSIZE 12
struct TtfDOC;
struct TableINFO {
  TtfDOC* pTtfDoc;
  TableINFO* GetTableInfo(const char* pTag, FILE* pFile);
  TableINFO* GetTableInfo(const char* pTag) const;

  virtual void Load(FILE* pFile, const TableENTRY* pTableEntry) = 0;
  virtual void Store(FILE* pFile, TableENTRY* pTableEntry) const = 0;
  virtual void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param) {};
  virtual void Duplicate(const TableINFO* pTable) = 0;
  void SetTtfDoc(TtfDOC* pDoc);
};

struct ExportGlyfSTRUCT;
struct TtfDOC {
  TtfHEADER TtfHeader;
  TableENTRY* pTableEntry;
  TableINFO** pTableInfo;

  void Destroy();

  void set();
  void set(const TtfDOC& TtfDoc);
  TableINFO* GetTableInfo(const char* pTag, FILE* pFile);
  TableINFO* GetTableInfo(const char* pTag) const;
  void LoadTtf(FILE* pFile);
  void StoreTtf(FILE* pFile) const;
  void AppendGlyphByDoc(const TtfDOC* pAddedDoc, void* Param);

  int GetGlyfCount();
  void GetGlyph(ExportGlyfSTRUCT* pExport, int Index) const;
  USHORT GetUnitsPerEm() const;
  int GetFontName(char* pBuf, BOOL* bUnicode, ULONG BufLen) const; //in bytes
  void SetFontName(const char* pStr, BOOL bUnicode);
};

struct OtherTableINFO : public TableINFO { //for expand table information
  void* pDataBuf;
  ULONG Length;

  OtherTableINFO() : pDataBuf(NULL) {}
  ~OtherTableINFO() {
    free(pDataBuf);
  }
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void Duplicate(const TableINFO* pTable);
};

struct EncodingMapENTRY {
  USHORT PlatformId;
  USHORT EncodingId;
  ULONG Offset; //from beginning of this table
  int CodingMapId; //程序内使用的，不属于ttf文件
};

struct cmapTableINFO : public TableINFO { //character to glyph mapping
  USHORT TableVersion;
  USHORT TableNumber;
  EncodingMapENTRY* MapEntries;
  FormatHEADER** MapTableInfo;

  void WriteMapFormat(FormatHEADER* MapTableInfo, FILE* pFile) const;
  FormatHEADER* CreateAndReadMapFormat(FormatHEADER* pHeader, FILE* pFile);

  cmapTableINFO();
  ~cmapTableINFO();
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
};

struct glyfTableINFO : public TableINFO { //glyph data
  int glyfCount;
  glyfHEADER* glyfHeader;
  glyfSimpleFORMAT** glyfInfo;

  glyfSimpleFORMAT* CreateAndReadGlyfInfo(const glyfHEADER* pHeader, FILE* pFile);
  glyfSimpleFORMAT* CreateAndCopyGlyfInfo(const glyfHEADER* pHeader, const glyfSimpleFORMAT* pFormat, double Scaling,
      int xShift, int yShift, int glyfIdxDelta = 0);
  //scaling:缩放比率；xShift:水平方向的偏移(缩放后)；yShift:垂直方向的偏移(缩放后)；glyfDelta：字形索引增加值（主要为了纠正复和字形里的组成部分的索引)
  void ReadFlagsAndPoints(const glyfHEADER* pHeader, glyfSimpleFORMAT* pFormat, FILE* pFile);
  static void WriteGlyfInfo(const glyfSimpleFORMAT* pGlyfInfo, const glyfHEADER* pHeader, FILE* pFile);
  int GetPtCountOfSimpleGlyph(int NumberOfGlyph, const glyfSimpleFORMAT* pFormat) const;
  void GetSimpleGlyph(ExportGlyfSTRUCT* pExport, int Index) const;

  glyfTableINFO();
  ~glyfTableINFO();
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
  void GetGlyph(ExportGlyfSTRUCT* pExport, int Index) const;
};

struct headTableINFO : public TableINFO { //font header
  headTableSTRUCT HeadTable;

  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);

  void SetCheckSumAdjustment(ULONG Val);
  SHORT GetIndexToLocFormat() const;
  USHORT GetUnitsPerEm() const;
  FWORD GetxMin() const;
  const headTableSTRUCT* GetHeadTableInfo()const;
};


struct hheaTableINFO : public TableINFO { //horizontal header
  hheaTableSTRUCT hheaTable;

  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  int GetNumberOfHMetrics() const;
  void SetNumberOfHMetrics(USHORT Count);
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
};

struct hmtxTableINFO : public TableINFO { //horizontal metrics
  int NumberOfHMetrics;
  int NumberOfGlyphs;
  LongHorMETRIC* hMetrics; //numOfHMetrics comes from the 'hhea' table.
  FWORD* LeftSideBearing; //The number of entries in this array is derived from numGlyphs (from 'maxp' table) minus numberOfHMetrics.


  hmtxTableINFO();
  ~hmtxTableINFO();
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
};
//for loca
struct locaTableINFO : public TableINFO { //index to location
  //以下两个成员为了分析方便设置，不属于ttf文件内容
  BOOL LongFormat; //用来保存数组元素类型
  int NumGlyphs; //保存glyph个数

  void* pOffset;


  locaTableINFO();
  ~locaTableINFO();
  ULONG GetOffset(int Index);
  void SetOffset(int Index, ULONG Offset);
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
};

struct maxpTableINFO : public TableINFO { //maximum profile
  maxpTableSTRUCT MaxpTable;
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
  int GetNumberOfGlyphs() const;
};

struct nameTableINFO : public TableINFO { //naming table
  USHORT FormatSelector;
  USHORT NumberOfRecord;
  USHORT OffsetOfString;
  USHORT RevPad;
  NameRECORD* NameRecords;
  char* pString; //name storage
  int StrLen;

  int GetNameIndex() const;

  nameTableINFO();
  ~nameTableINFO();
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void Duplicate(const TableINFO* pTable);
  int GetFontName(char* pBuf, BOOL* bUnicode, ULONG BufLen) const; //in bytes
  void SetFontName(const char* pStr, BOOL bUnicode);
};

struct postTableINFO : public TableINFO { //PostScript information
  postSimpleFORMAT* pCommonHeader;
  ~postTableINFO();
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
};

struct OS2TableINFO : public TableINFO { //OS/2 and Windows specific metrics
  OS2TableSTRUCT OS2Table;

  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
  int GetWinDescent() const;
};

struct cvtTableINFO : public TableINFO { //Control Value Table
  int Count;
  FWORD* ControlValue;

  cvtTableINFO();
  ~cvtTableINFO();
  void Load(FILE* pFile, const TableENTRY* pTableEntry);
  void Store(FILE* pFile, TableENTRY* pTableEntry) const;
  void AppendGlyfByDoc(const TtfDOC* pAddedDoc, void* Param);
  void Duplicate(const TableINFO* pTable);
};

#pragma pack(pop)//恢复对齐状态


#include "ttfdoc.inl"


