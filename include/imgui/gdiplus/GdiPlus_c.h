#ifndef _GDIPLUS_C_H_
#define _GDIPLUS_C_H_
EXTERN_C_BEGIN
#define WINGDIPAPI __stdcall
#define GDIPCONST const
//--------------------------------------------------------------------------
// Default bezier flattening tolerance in device pixels.
//--------------------------------------------------------------------------
const float FlatnessDefault = 1.0f / 4.0f;
//--------------------------------------------------------------------------
// Graphics and Container State cookies
//--------------------------------------------------------------------------
typedef UINT GraphicsState;
typedef UINT GraphicsContainer;
//--------------------------------------------------------------------------
// Fill mode constants
//--------------------------------------------------------------------------
typedef enum {
  FillModeAlternate, // 0
  FillModeWinding // 1
} FillMode;
//--------------------------------------------------------------------------
// Quality mode constants
//--------------------------------------------------------------------------
typedef enum {
  QualityModeInvalid = -1,
  QualityModeDefault = 0,
  QualityModeLow = 1, // Best performance
  QualityModeHigh = 2 // Best rendering quality
} QualityMode;
//--------------------------------------------------------------------------
// Alpha Compositing mode constants
//--------------------------------------------------------------------------
typedef enum {
  CompositingModeSourceOver, // 0
  CompositingModeSourceCopy // 1
} CompositingMode;
//--------------------------------------------------------------------------
// Alpha Compositing quality constants
//--------------------------------------------------------------------------
typedef enum {
  CompositingQualityInvalid = QualityModeInvalid,
  CompositingQualityDefault = QualityModeDefault,
  CompositingQualityHighSpeed = QualityModeLow,
  CompositingQualityHighQuality = QualityModeHigh,
  CompositingQualityGammaCorrected,
  CompositingQualityAssumeLinear
} CompositingQuality;
//--------------------------------------------------------------------------
// Unit constants
//--------------------------------------------------------------------------
typedef enum {
  UnitWorld, // 0 -- World coordinate (non-physical unit)
  UnitDisplay, // 1 -- Variable -- for PageTransform only
  UnitPixel, // 2 -- Each unit is one device pixel.
  UnitPoint, // 3 -- Each unit is a printer's point, or 1/72 inch.
  UnitInch, // 4 -- Each unit is 1 inch.
  UnitDocument, // 5 -- Each unit is 1/300 inch.
  UnitMillimeter // 6 -- Each unit is 1 millimeter.
} Unit;
//--------------------------------------------------------------------------
// MetafileFrameUnit
//
// The frameRect for creating a metafile can be specified in any of these
// units. There is an extra frame unit value (MetafileFrameUnitGdi) so
// that units can be supplied in the same units that GDI expects for
// frame rects -- these units are in .01 (1/100ths) millimeter units
// as defined by GDI.
//--------------------------------------------------------------------------
typedef enum {
  MetafileFrameUnitPixel = UnitPixel,
  MetafileFrameUnitPoint = UnitPoint,
  MetafileFrameUnitInch = UnitInch,
  MetafileFrameUnitDocument = UnitDocument,
  MetafileFrameUnitMillimeter = UnitMillimeter,
  MetafileFrameUnitGdi // GDI compatible .01 MM units
} MetafileFrameUnit;
//--------------------------------------------------------------------------
// Coordinate space identifiers
//--------------------------------------------------------------------------
typedef enum {
  CoordinateSpaceWorld, // 0
  CoordinateSpacePage, // 1
  CoordinateSpaceDevice // 2
} CoordinateSpace;
//--------------------------------------------------------------------------
// Various wrap modes for brushes
//--------------------------------------------------------------------------
typedef enum {
  WrapModeTile, // 0
  WrapModeTileFlipX, // 1
  WrapModeTileFlipY, // 2
  WrapModeTileFlipXY, // 3
  WrapModeClamp // 4
} WrapMode;
//--------------------------------------------------------------------------
// Various hatch styles
//--------------------------------------------------------------------------
typedef enum {
  HatchStyleHorizontal, // 0
  HatchStyleVertical, // 1
  HatchStyleForwardDiagonal, // 2
  HatchStyleBackwardDiagonal, // 3
  HatchStyleCross, // 4
  HatchStyleDiagonalCross, // 5
  HatchStyle05Percent, // 6
  HatchStyle10Percent, // 7
  HatchStyle20Percent, // 8
  HatchStyle25Percent, // 9
  HatchStyle30Percent, // 10
  HatchStyle40Percent, // 11
  HatchStyle50Percent, // 12
  HatchStyle60Percent, // 13
  HatchStyle70Percent, // 14
  HatchStyle75Percent, // 15
  HatchStyle80Percent, // 16
  HatchStyle90Percent, // 17
  HatchStyleLightDownwardDiagonal, // 18
  HatchStyleLightUpwardDiagonal, // 19
  HatchStyleDarkDownwardDiagonal, // 20
  HatchStyleDarkUpwardDiagonal, // 21
  HatchStyleWideDownwardDiagonal, // 22
  HatchStyleWideUpwardDiagonal, // 23
  HatchStyleLightVertical, // 24
  HatchStyleLightHorizontal, // 25
  HatchStyleNarrowVertical, // 26
  HatchStyleNarrowHorizontal, // 27
  HatchStyleDarkVertical, // 28
  HatchStyleDarkHorizontal, // 29
  HatchStyleDashedDownwardDiagonal, // 30
  HatchStyleDashedUpwardDiagonal, // 31
  HatchStyleDashedHorizontal, // 32
  HatchStyleDashedVertical, // 33
  HatchStyleSmallConfetti, // 34
  HatchStyleLargeConfetti, // 35
  HatchStyleZigZag, // 36
  HatchStyleWave, // 37
  HatchStyleDiagonalBrick, // 38
  HatchStyleHorizontalBrick, // 39
  HatchStyleWeave, // 40
  HatchStylePlaid, // 41
  HatchStyleDivot, // 42
  HatchStyleDottedGrid, // 43
  HatchStyleDottedDiamond, // 44
  HatchStyleShingle, // 45
  HatchStyleTrellis, // 46
  HatchStyleSphere, // 47
  HatchStyleSmallGrid, // 48
  HatchStyleSmallCheckerBoard, // 49
  HatchStyleLargeCheckerBoard, // 50
  HatchStyleOutlinedDiamond, // 51
  HatchStyleSolidDiamond, // 52
  HatchStyleTotal, // must be after all unique hatch styles
  HatchStyleLargeGrid = HatchStyleCross, // 4
  HatchStyleMin = HatchStyleHorizontal,
  HatchStyleMax = HatchStyleTotal - 1,
} HatchStyle;
//--------------------------------------------------------------------------
// Dash style constants
//--------------------------------------------------------------------------
typedef enum {
  DashStyleSolid, // 0
  DashStyleDash, // 1
  DashStyleDot, // 2
  DashStyleDashDot, // 3
  DashStyleDashDotDot, // 4
  DashStyleCustom // 5
} DashStyle;
//--------------------------------------------------------------------------
// Dash cap constants
//--------------------------------------------------------------------------
typedef enum {
  DashCapFlat = 0,
  DashCapRound = 2,
  DashCapTriangle = 3
} DashCap;
//--------------------------------------------------------------------------
// Line cap constants (only the lowest 8 bits are used).
//--------------------------------------------------------------------------
typedef enum {
  LineCapFlat = 0,
  LineCapSquare = 1,
  LineCapRound = 2,
  LineCapTriangle = 3,
  LineCapNoAnchor = 0x10, // corresponds to flat cap
  LineCapSquareAnchor = 0x11, // corresponds to square cap
  LineCapRoundAnchor = 0x12, // corresponds to round cap
  LineCapDiamondAnchor = 0x13, // corresponds to triangle cap
  LineCapArrowAnchor = 0x14, // no correspondence
  LineCapCustom = 0xff, // custom cap
  LineCapAnchorMask = 0xf0 // mask to check for anchor or not.
} LineCap;
//--------------------------------------------------------------------------
// Custom Line cap type constants
//--------------------------------------------------------------------------
typedef enum {
  CustomLineCapTypeDefault = 0,
  CustomLineCapTypeAdjustableArrow = 1
} CustomLineCapType;
//--------------------------------------------------------------------------
// Line join constants
//--------------------------------------------------------------------------
typedef enum {
  LineJoinMiter = 0,
  LineJoinBevel = 1,
  LineJoinRound = 2,
  LineJoinMiterClipped = 3
} LineJoin;
//--------------------------------------------------------------------------
// Path point types (only the lowest 8 bits are used.)
// The lowest 3 bits are interpreted as point type
// The higher 5 bits are reserved for flags.
//--------------------------------------------------------------------------
typedef enum {
  PathPointTypeStart = 0, // move
  PathPointTypeLine = 1, // line
  PathPointTypeBezier = 3, // default Bezier (= cubic Bezier)
  PathPointTypePathTypeMask = 0x07, // type mask (lowest 3 bits).
  PathPointTypeDashMode = 0x10, // currently in dash mode.
  PathPointTypePathMarker = 0x20, // a marker for the path.
  PathPointTypeCloseSubpath = 0x80, // closed flag
  // Path types used for advanced path.
  PathPointTypeBezier2    = 2,    // quadratic Beizer
  PathPointTypeBezier3    = 3,    // cubic Bezier
  PathPointTypeBezier4    = 4,    // quartic (4th order) Beizer
  PathPointTypeBezier5    = 5,    // quintic (5th order) Bezier
  PathPointTypeBezier6    = 6     // hexaic (6th order) Bezier
} PathPointType;
//--------------------------------------------------------------------------
// WarpMode constants
//--------------------------------------------------------------------------
typedef enum {
  WarpModePerspective, // 0
  WarpModeBilinear // 1
} WarpMode;
//--------------------------------------------------------------------------
// LineGradient Mode
//--------------------------------------------------------------------------
typedef enum {
  LinearGradientModeHorizontal, // 0
  LinearGradientModeVertical, // 1
  LinearGradientModeForwardDiagonal, // 2
  LinearGradientModeBackwardDiagonal // 3
} LinearGradientMode;
//--------------------------------------------------------------------------
// Region Comine Modes
//--------------------------------------------------------------------------
typedef enum {
  CombineModeReplace, // 0
  CombineModeIntersect, // 1
  CombineModeUnion, // 2
  CombineModeXor, // 3
  CombineModeExclude, // 4
  CombineModeComplement // 5 (Exclude From)
} CombineMode;
//--------------------------------------------------------------------------
// Image types
//--------------------------------------------------------------------------
typedef enum {
  ImageTypeUnknown, // 0
  ImageTypeBitmap, // 1
  ImageTypeMetafile // 2
} ImageType;
//--------------------------------------------------------------------------
// Interpolation modes
//--------------------------------------------------------------------------
typedef enum {
  InterpolationModeInvalid = QualityModeInvalid,
  InterpolationModeDefault = QualityModeDefault,
  InterpolationModeLowQuality = QualityModeLow,
  InterpolationModeHighQuality = QualityModeHigh,
  InterpolationModeBilinear,
  InterpolationModeBicubic,
  InterpolationModeNearestNeighbor,
  InterpolationModeHighQualityBilinear,
  InterpolationModeHighQualityBicubic
} InterpolationMode;
//--------------------------------------------------------------------------
// Pen types
//--------------------------------------------------------------------------
typedef enum {
  PenAlignmentCenter = 0,
  PenAlignmentInset = 1
} PenAlignment;
//--------------------------------------------------------------------------
// Brush types
//--------------------------------------------------------------------------
typedef enum BrushType {
  BrushTypeSolidColor = 0,
  BrushTypeHatchFill = 1,
  BrushTypeTextureFill = 2,
  BrushTypePathGradient = 3,
  BrushTypeLinearGradient = 4
};
//--------------------------------------------------------------------------
// Pen's Fill types
//--------------------------------------------------------------------------
typedef enum {
  PenTypeSolidColor = BrushTypeSolidColor,
  PenTypeHatchFill = BrushTypeHatchFill,
  PenTypeTextureFill = BrushTypeTextureFill,
  PenTypePathGradient = BrushTypePathGradient,
  PenTypeLinearGradient = BrushTypeLinearGradient,
  PenTypeUnknown = -1
} PenType;
//--------------------------------------------------------------------------
// Matrix Order
//--------------------------------------------------------------------------
typedef enum {
  MatrixOrderPrepend = 0,
  MatrixOrderAppend = 1
} MatrixOrder;
//--------------------------------------------------------------------------
// Generic font families
//--------------------------------------------------------------------------
typedef enum {
  GenericFontFamilySerif,
  GenericFontFamilySansSerif,
  GenericFontFamilyMonospace
} GenericFontFamily;
//--------------------------------------------------------------------------
// FontStyle: face types and common styles
//--------------------------------------------------------------------------
typedef enum {
  FontStyleRegular = 0,
  FontStyleBold = 1,
  FontStyleItalic = 2,
  FontStyleBoldItalic = 3,
  FontStyleUnderline = 4,
  FontStyleStrikeout = 8
} FontStyle;
//---------------------------------------------------------------------------
// Smoothing Mode
//---------------------------------------------------------------------------
typedef enum {
  SmoothingModeInvalid = QualityModeInvalid,
  SmoothingModeDefault = QualityModeDefault,
  SmoothingModeHighSpeed = QualityModeLow,
  SmoothingModeHighQuality = QualityModeHigh,
  SmoothingModeNone,
  SmoothingModeAntiAlias
} SmoothingMode;
//---------------------------------------------------------------------------
// Pixel Format Mode
//---------------------------------------------------------------------------
typedef enum {
  PixelOffsetModeInvalid = QualityModeInvalid,
  PixelOffsetModeDefault = QualityModeDefault,
  PixelOffsetModeHighSpeed = QualityModeLow,
  PixelOffsetModeHighQuality = QualityModeHigh,
  PixelOffsetModeNone, // No pixel offset
  PixelOffsetModeHalf // Offset by -0.5, -0.5 for fast anti-alias perf
} PixelOffsetMode;
//---------------------------------------------------------------------------
// Text Rendering Hint
//---------------------------------------------------------------------------
typedef enum {
  TextRenderingHintSystemDefault = 0, // Glyph with system default rendering hint
  TextRenderingHintSingleBitPerPixelGridFit, // Glyph bitmap with hinting
  TextRenderingHintSingleBitPerPixel, // Glyph bitmap without hinting
  TextRenderingHintAntiAliasGridFit, // Glyph anti-alias bitmap with hinting
  TextRenderingHintAntiAlias, // Glyph anti-alias bitmap without hinting
  TextRenderingHintClearTypeGridFit // Glyph CT bitmap with hinting
} TextRenderingHint;
//---------------------------------------------------------------------------
// Metafile Types
//---------------------------------------------------------------------------
typedef enum {
  MetafileTypeInvalid, // Invalid metafile
  MetafileTypeWmf, // Standard WMF
  MetafileTypeWmfPlaceable, // Placeable WMF
  MetafileTypeEmf, // EMF (not EMF+)
  MetafileTypeEmfPlusOnly, // EMF+ without dual, down-level records
  MetafileTypeEmfPlusDual // EMF+ with dual, down-level records
} MetafileType;
//---------------------------------------------------------------------------
// Specifies the type of EMF to record
//---------------------------------------------------------------------------
typedef enum {
  EmfTypeEmfOnly = MetafileTypeEmf, // no EMF+, only EMF
  EmfTypeEmfPlusOnly = MetafileTypeEmfPlusOnly, // no EMF, only EMF+
  EmfTypeEmfPlusDual = MetafileTypeEmfPlusDual // both EMF+ and EMF
} EmfType;
//---------------------------------------------------------------------------
// EMF+ Persistent object types
//---------------------------------------------------------------------------
typedef enum {
  ObjectTypeInvalid,
  ObjectTypeBrush,
  ObjectTypePen,
  ObjectTypePath,
  ObjectTypeRegion,
  ObjectTypeImage,
  ObjectTypeFont,
  ObjectTypeStringFormat,
  ObjectTypeImageAttributes,
  ObjectTypeCustomLineCap,
  ObjectTypeMax = ObjectTypeCustomLineCap,
  ObjectTypeMin = ObjectTypeBrush
} ObjectType;
//---------------------------------------------------------------------------
// EMF+ Records
//---------------------------------------------------------------------------
// We have to change the WMF record numbers so that they don't conflict with
// the EMF and EMF+ record numbers.
#define GDIP_EMFPLUS_RECORD_BASE 0x00004000
#define GDIP_WMF_RECORD_BASE 0x00010000
#define GDIP_WMF_RECORD_TO_EMFPLUS(n) ((int)((n) | GDIP_WMF_RECORD_BASE))
#define GDIP_EMFPLUS_RECORD_TO_WMF(n) ((n) & (~GDIP_WMF_RECORD_BASE))
#define GDIP_IS_WMF_RECORDTYPE(n) (((n) & GDIP_WMF_RECORD_BASE) != 0)
typedef enum {
  // Since we have to enumerate GDI records right along with GDI+ records,
  // We list all the GDI records here so that they can be part of the
  // same enumeration type which is used in the enumeration callback.
  WmfRecordTypeSetBkColor = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETBKCOLOR),
  WmfRecordTypeSetBkMode = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETBKMODE),
  WmfRecordTypeSetMapMode = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETMAPMODE),
  WmfRecordTypeSetROP2 = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETROP2),
  WmfRecordTypeSetRelAbs = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETRELABS),
  WmfRecordTypeSetPolyFillMode = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETPOLYFILLMODE),
  WmfRecordTypeSetStretchBltMode = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETSTRETCHBLTMODE),
  WmfRecordTypeSetTextCharExtra = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTCHAREXTRA),
  WmfRecordTypeSetTextColor = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTCOLOR),
  WmfRecordTypeSetTextJustification = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTJUSTIFICATION),
  WmfRecordTypeSetWindowOrg = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETWINDOWORG),
  WmfRecordTypeSetWindowExt = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETWINDOWEXT),
  WmfRecordTypeSetViewportOrg = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETVIEWPORTORG),
  WmfRecordTypeSetViewportExt = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETVIEWPORTEXT),
  WmfRecordTypeOffsetWindowOrg = GDIP_WMF_RECORD_TO_EMFPLUS(META_OFFSETWINDOWORG),
  WmfRecordTypeScaleWindowExt = GDIP_WMF_RECORD_TO_EMFPLUS(META_SCALEWINDOWEXT),
  WmfRecordTypeOffsetViewportOrg = GDIP_WMF_RECORD_TO_EMFPLUS(META_OFFSETVIEWPORTORG),
  WmfRecordTypeScaleViewportExt = GDIP_WMF_RECORD_TO_EMFPLUS(META_SCALEVIEWPORTEXT),
  WmfRecordTypeLineTo = GDIP_WMF_RECORD_TO_EMFPLUS(META_LINETO),
  WmfRecordTypeMoveTo = GDIP_WMF_RECORD_TO_EMFPLUS(META_MOVETO),
  WmfRecordTypeExcludeClipRect = GDIP_WMF_RECORD_TO_EMFPLUS(META_EXCLUDECLIPRECT),
  WmfRecordTypeIntersectClipRect = GDIP_WMF_RECORD_TO_EMFPLUS(META_INTERSECTCLIPRECT),
  WmfRecordTypeArc = GDIP_WMF_RECORD_TO_EMFPLUS(META_ARC),
  WmfRecordTypeEllipse = GDIP_WMF_RECORD_TO_EMFPLUS(META_ELLIPSE),
  WmfRecordTypeFloodFill = GDIP_WMF_RECORD_TO_EMFPLUS(META_FLOODFILL),
  WmfRecordTypePie = GDIP_WMF_RECORD_TO_EMFPLUS(META_PIE),
  WmfRecordTypeRectangle = GDIP_WMF_RECORD_TO_EMFPLUS(META_RECTANGLE),
  WmfRecordTypeRoundRect = GDIP_WMF_RECORD_TO_EMFPLUS(META_ROUNDRECT),
  WmfRecordTypePatBlt = GDIP_WMF_RECORD_TO_EMFPLUS(META_PATBLT),
  WmfRecordTypeSaveDC = GDIP_WMF_RECORD_TO_EMFPLUS(META_SAVEDC),
  WmfRecordTypeSetPixel = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETPIXEL),
  WmfRecordTypeOffsetClipRgn = GDIP_WMF_RECORD_TO_EMFPLUS(META_OFFSETCLIPRGN),
  WmfRecordTypeTextOut = GDIP_WMF_RECORD_TO_EMFPLUS(META_TEXTOUT),
  WmfRecordTypeBitBlt = GDIP_WMF_RECORD_TO_EMFPLUS(META_BITBLT),
  WmfRecordTypeStretchBlt = GDIP_WMF_RECORD_TO_EMFPLUS(META_STRETCHBLT),
  WmfRecordTypePolygon = GDIP_WMF_RECORD_TO_EMFPLUS(META_POLYGON),
  WmfRecordTypePolyline = GDIP_WMF_RECORD_TO_EMFPLUS(META_POLYLINE),
  WmfRecordTypeEscape = GDIP_WMF_RECORD_TO_EMFPLUS(META_ESCAPE),
  WmfRecordTypeRestoreDC = GDIP_WMF_RECORD_TO_EMFPLUS(META_RESTOREDC),
  WmfRecordTypeFillRegion = GDIP_WMF_RECORD_TO_EMFPLUS(META_FILLREGION),
  WmfRecordTypeFrameRegion = GDIP_WMF_RECORD_TO_EMFPLUS(META_FRAMEREGION),
  WmfRecordTypeInvertRegion = GDIP_WMF_RECORD_TO_EMFPLUS(META_INVERTREGION),
  WmfRecordTypePaintRegion = GDIP_WMF_RECORD_TO_EMFPLUS(META_PAINTREGION),
  WmfRecordTypeSelectClipRegion = GDIP_WMF_RECORD_TO_EMFPLUS(META_SELECTCLIPREGION),
  WmfRecordTypeSelectObject = GDIP_WMF_RECORD_TO_EMFPLUS(META_SELECTOBJECT),
  WmfRecordTypeSetTextAlign = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETTEXTALIGN),
  WmfRecordTypeDrawText = GDIP_WMF_RECORD_TO_EMFPLUS(0x062F), // META_DRAWTEXT
  WmfRecordTypeChord = GDIP_WMF_RECORD_TO_EMFPLUS(META_CHORD),
  WmfRecordTypeSetMapperFlags = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETMAPPERFLAGS),
  WmfRecordTypeExtTextOut = GDIP_WMF_RECORD_TO_EMFPLUS(META_EXTTEXTOUT),
  WmfRecordTypeSetDIBToDev = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETDIBTODEV),
  WmfRecordTypeSelectPalette = GDIP_WMF_RECORD_TO_EMFPLUS(META_SELECTPALETTE),
  WmfRecordTypeRealizePalette = GDIP_WMF_RECORD_TO_EMFPLUS(META_REALIZEPALETTE),
  WmfRecordTypeAnimatePalette = GDIP_WMF_RECORD_TO_EMFPLUS(META_ANIMATEPALETTE),
  WmfRecordTypeSetPalEntries = GDIP_WMF_RECORD_TO_EMFPLUS(META_SETPALENTRIES),
  WmfRecordTypePolyPolygon = GDIP_WMF_RECORD_TO_EMFPLUS(META_POLYPOLYGON),
  WmfRecordTypeResizePalette = GDIP_WMF_RECORD_TO_EMFPLUS(META_RESIZEPALETTE),
  WmfRecordTypeDIBBitBlt = GDIP_WMF_RECORD_TO_EMFPLUS(META_DIBBITBLT),
  WmfRecordTypeDIBStretchBlt = GDIP_WMF_RECORD_TO_EMFPLUS(META_DIBSTRETCHBLT),
  WmfRecordTypeDIBCreatePatternBrush = GDIP_WMF_RECORD_TO_EMFPLUS(META_DIBCREATEPATTERNBRUSH),
  WmfRecordTypeStretchDIB = GDIP_WMF_RECORD_TO_EMFPLUS(META_STRETCHDIB),
  WmfRecordTypeExtFloodFill = GDIP_WMF_RECORD_TO_EMFPLUS(META_EXTFLOODFILL),
  WmfRecordTypeSetLayout = GDIP_WMF_RECORD_TO_EMFPLUS(0x0149), // META_SETLAYOUT
  WmfRecordTypeResetDC = GDIP_WMF_RECORD_TO_EMFPLUS(0x014C), // META_RESETDC
  WmfRecordTypeStartDoc = GDIP_WMF_RECORD_TO_EMFPLUS(0x014D), // META_STARTDOC
  WmfRecordTypeStartPage = GDIP_WMF_RECORD_TO_EMFPLUS(0x004F), // META_STARTPAGE
  WmfRecordTypeEndPage = GDIP_WMF_RECORD_TO_EMFPLUS(0x0050), // META_ENDPAGE
  WmfRecordTypeAbortDoc = GDIP_WMF_RECORD_TO_EMFPLUS(0x0052), // META_ABORTDOC
  WmfRecordTypeEndDoc = GDIP_WMF_RECORD_TO_EMFPLUS(0x005E), // META_ENDDOC
  WmfRecordTypeDeleteObject = GDIP_WMF_RECORD_TO_EMFPLUS(META_DELETEOBJECT),
  WmfRecordTypeCreatePalette = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEPALETTE),
  WmfRecordTypeCreateBrush = GDIP_WMF_RECORD_TO_EMFPLUS(0x00F8), // META_CREATEBRUSH
  WmfRecordTypeCreatePatternBrush = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEPATTERNBRUSH),
  WmfRecordTypeCreatePenIndirect = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEPENINDIRECT),
  WmfRecordTypeCreateFontIndirect = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEFONTINDIRECT),
  WmfRecordTypeCreateBrushIndirect = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEBRUSHINDIRECT),
  WmfRecordTypeCreateBitmapIndirect = GDIP_WMF_RECORD_TO_EMFPLUS(0x02FD), // META_CREATEBITMAPINDIRECT
  WmfRecordTypeCreateBitmap = GDIP_WMF_RECORD_TO_EMFPLUS(0x06FE), // META_CREATEBITMAP
  WmfRecordTypeCreateRegion = GDIP_WMF_RECORD_TO_EMFPLUS(META_CREATEREGION),
  EmfRecordTypeHeader = EMR_HEADER,
  EmfRecordTypePolyBezier = EMR_POLYBEZIER,
  EmfRecordTypePolygon = EMR_POLYGON,
  EmfRecordTypePolyline = EMR_POLYLINE,
  EmfRecordTypePolyBezierTo = EMR_POLYBEZIERTO,
  EmfRecordTypePolyLineTo = EMR_POLYLINETO,
  EmfRecordTypePolyPolyline = EMR_POLYPOLYLINE,
  EmfRecordTypePolyPolygon = EMR_POLYPOLYGON,
  EmfRecordTypeSetWindowExtEx = EMR_SETWINDOWEXTEX,
  EmfRecordTypeSetWindowOrgEx = EMR_SETWINDOWORGEX,
  EmfRecordTypeSetViewportExtEx = EMR_SETVIEWPORTEXTEX,
  EmfRecordTypeSetViewportOrgEx = EMR_SETVIEWPORTORGEX,
  EmfRecordTypeSetBrushOrgEx = EMR_SETBRUSHORGEX,
  EmfRecordTypeEOF = EMR_EOF,
  EmfRecordTypeSetPixelV = EMR_SETPIXELV,
  EmfRecordTypeSetMapperFlags = EMR_SETMAPPERFLAGS,
  EmfRecordTypeSetMapMode = EMR_SETMAPMODE,
  EmfRecordTypeSetBkMode = EMR_SETBKMODE,
  EmfRecordTypeSetPolyFillMode = EMR_SETPOLYFILLMODE,
  EmfRecordTypeSetROP2 = EMR_SETROP2,
  EmfRecordTypeSetStretchBltMode = EMR_SETSTRETCHBLTMODE,
  EmfRecordTypeSetTextAlign = EMR_SETTEXTALIGN,
  EmfRecordTypeSetColorAdjustment = EMR_SETCOLORADJUSTMENT,
  EmfRecordTypeSetTextColor = EMR_SETTEXTCOLOR,
  EmfRecordTypeSetBkColor = EMR_SETBKCOLOR,
  EmfRecordTypeOffsetClipRgn = EMR_OFFSETCLIPRGN,
  EmfRecordTypeMoveToEx = EMR_MOVETOEX,
  EmfRecordTypeSetMetaRgn = EMR_SETMETARGN,
  EmfRecordTypeExcludeClipRect = EMR_EXCLUDECLIPRECT,
  EmfRecordTypeIntersectClipRect = EMR_INTERSECTCLIPRECT,
  EmfRecordTypeScaleViewportExtEx = EMR_SCALEVIEWPORTEXTEX,
  EmfRecordTypeScaleWindowExtEx = EMR_SCALEWINDOWEXTEX,
  EmfRecordTypeSaveDC = EMR_SAVEDC,
  EmfRecordTypeRestoreDC = EMR_RESTOREDC,
  EmfRecordTypeSetWorldTransform = EMR_SETWORLDTRANSFORM,
  EmfRecordTypeModifyWorldTransform = EMR_MODIFYWORLDTRANSFORM,
  EmfRecordTypeSelectObject = EMR_SELECTOBJECT,
  EmfRecordTypeCreatePen = EMR_CREATEPEN,
  EmfRecordTypeCreateBrushIndirect = EMR_CREATEBRUSHINDIRECT,
  EmfRecordTypeDeleteObject = EMR_DELETEOBJECT,
  EmfRecordTypeAngleArc = EMR_ANGLEARC,
  EmfRecordTypeEllipse = EMR_ELLIPSE,
  EmfRecordTypeRectangle = EMR_RECTANGLE,
  EmfRecordTypeRoundRect = EMR_ROUNDRECT,
  EmfRecordTypeArc = EMR_ARC,
  EmfRecordTypeChord = EMR_CHORD,
  EmfRecordTypePie = EMR_PIE,
  EmfRecordTypeSelectPalette = EMR_SELECTPALETTE,
  EmfRecordTypeCreatePalette = EMR_CREATEPALETTE,
  EmfRecordTypeSetPaletteEntries = EMR_SETPALETTEENTRIES,
  EmfRecordTypeResizePalette = EMR_RESIZEPALETTE,
  EmfRecordTypeRealizePalette = EMR_REALIZEPALETTE,
  EmfRecordTypeExtFloodFill = EMR_EXTFLOODFILL,
  EmfRecordTypeLineTo = EMR_LINETO,
  EmfRecordTypeArcTo = EMR_ARCTO,
  EmfRecordTypePolyDraw = EMR_POLYDRAW,
  EmfRecordTypeSetArcDirection = EMR_SETARCDIRECTION,
  EmfRecordTypeSetMiterLimit = EMR_SETMITERLIMIT,
  EmfRecordTypeBeginPath = EMR_BEGINPATH,
  EmfRecordTypeEndPath = EMR_ENDPATH,
  EmfRecordTypeCloseFigure = EMR_CLOSEFIGURE,
  EmfRecordTypeFillPath = EMR_FILLPATH,
  EmfRecordTypeStrokeAndFillPath = EMR_STROKEANDFILLPATH,
  EmfRecordTypeStrokePath = EMR_STROKEPATH,
  EmfRecordTypeFlattenPath = EMR_FLATTENPATH,
  EmfRecordTypeWidenPath = EMR_WIDENPATH,
  EmfRecordTypeSelectClipPath = EMR_SELECTCLIPPATH,
  EmfRecordTypeAbortPath = EMR_ABORTPATH,
  EmfRecordTypeReserved_069 = 69, // Not Used
  EmfRecordTypeGdiComment = EMR_GDICOMMENT,
  EmfRecordTypeFillRgn = EMR_FILLRGN,
  EmfRecordTypeFrameRgn = EMR_FRAMERGN,
  EmfRecordTypeInvertRgn = EMR_INVERTRGN,
  EmfRecordTypePaintRgn = EMR_PAINTRGN,
  EmfRecordTypeExtSelectClipRgn = EMR_EXTSELECTCLIPRGN,
  EmfRecordTypeBitBlt = EMR_BITBLT,
  EmfRecordTypeStretchBlt = EMR_STRETCHBLT,
  EmfRecordTypeMaskBlt = EMR_MASKBLT,
  EmfRecordTypePlgBlt = EMR_PLGBLT,
  EmfRecordTypeSetDIBitsToDevice = EMR_SETDIBITSTODEVICE,
  EmfRecordTypeStretchDIBits = EMR_STRETCHDIBITS,
  EmfRecordTypeExtCreateFontIndirect = EMR_EXTCREATEFONTINDIRECTW,
  EmfRecordTypeExtTextOutA = EMR_EXTTEXTOUTA,
  EmfRecordTypeExtTextOutW = EMR_EXTTEXTOUTW,
  EmfRecordTypePolyBezier16 = EMR_POLYBEZIER16,
  EmfRecordTypePolygon16 = EMR_POLYGON16,
  EmfRecordTypePolyline16 = EMR_POLYLINE16,
  EmfRecordTypePolyBezierTo16 = EMR_POLYBEZIERTO16,
  EmfRecordTypePolylineTo16 = EMR_POLYLINETO16,
  EmfRecordTypePolyPolyline16 = EMR_POLYPOLYLINE16,
  EmfRecordTypePolyPolygon16 = EMR_POLYPOLYGON16,
  EmfRecordTypePolyDraw16 = EMR_POLYDRAW16,
  EmfRecordTypeCreateMonoBrush = EMR_CREATEMONOBRUSH,
  EmfRecordTypeCreateDIBPatternBrushPt = EMR_CREATEDIBPATTERNBRUSHPT,
  EmfRecordTypeExtCreatePen = EMR_EXTCREATEPEN,
  EmfRecordTypePolyTextOutA = EMR_POLYTEXTOUTA,
  EmfRecordTypePolyTextOutW = EMR_POLYTEXTOUTW,
  EmfRecordTypeSetICMMode = 98, // EMR_SETICMMODE,
  EmfRecordTypeCreateColorSpace = 99, // EMR_CREATECOLORSPACE,
  EmfRecordTypeSetColorSpace = 100, // EMR_SETCOLORSPACE,
  EmfRecordTypeDeleteColorSpace = 101, // EMR_DELETECOLORSPACE,
  EmfRecordTypeGLSRecord = 102, // EMR_GLSRECORD,
  EmfRecordTypeGLSBoundedRecord = 103, // EMR_GLSBOUNDEDRECORD,
  EmfRecordTypePixelFormat = 104, // EMR_PIXELFORMAT,
  EmfRecordTypeDrawEscape = 105, // EMR_RESERVED_105,
  EmfRecordTypeExtEscape = 106, // EMR_RESERVED_106,
  EmfRecordTypeStartDoc = 107, // EMR_RESERVED_107,
  EmfRecordTypeSmallTextOut = 108, // EMR_RESERVED_108,
  EmfRecordTypeForceUFIMapping = 109, // EMR_RESERVED_109,
  EmfRecordTypeNamedEscape = 110, // EMR_RESERVED_110,
  EmfRecordTypeColorCorrectPalette = 111, // EMR_COLORCORRECTPALETTE,
  EmfRecordTypeSetICMProfileA = 112, // EMR_SETICMPROFILEA,
  EmfRecordTypeSetICMProfileW = 113, // EMR_SETICMPROFILEW,
  EmfRecordTypeAlphaBlend = 114, // EMR_ALPHABLEND,
  EmfRecordTypeSetLayout = 115, // EMR_SETLAYOUT,
  EmfRecordTypeTransparentBlt = 116, // EMR_TRANSPARENTBLT,
  EmfRecordTypeReserved_117 = 117, // Not Used
  EmfRecordTypeGradientFill = 118, // EMR_GRADIENTFILL,
  EmfRecordTypeSetLinkedUFIs = 119, // EMR_RESERVED_119,
  EmfRecordTypeSetTextJustification = 120, // EMR_RESERVED_120,
  EmfRecordTypeColorMatchToTargetW = 121, // EMR_COLORMATCHTOTARGETW,
  EmfRecordTypeCreateColorSpaceW = 122, // EMR_CREATECOLORSPACEW,
  EmfRecordTypeMax = 122,
  EmfRecordTypeMin = 1,
  // That is the END of the GDI EMF records.
  // Now we start the list of EMF+ records. We leave quite
  // a bit of room here for the addition of any new GDI
  // records that may be added later.
  EmfPlusRecordTypeInvalid = GDIP_EMFPLUS_RECORD_BASE,
  EmfPlusRecordTypeHeader,
  EmfPlusRecordTypeEndOfFile,
  EmfPlusRecordTypeComment,
  EmfPlusRecordTypeGetDC,
  EmfPlusRecordTypeMultiFormatStart,
  EmfPlusRecordTypeMultiFormatSection,
  EmfPlusRecordTypeMultiFormatEnd,
  // For all persistent objects
  EmfPlusRecordTypeObject,
  // Drawing Records
  EmfPlusRecordTypeClear,
  EmfPlusRecordTypeFillRects,
  EmfPlusRecordTypeDrawRects,
  EmfPlusRecordTypeFillPolygon,
  EmfPlusRecordTypeDrawLines,
  EmfPlusRecordTypeFillEllipse,
  EmfPlusRecordTypeDrawEllipse,
  EmfPlusRecordTypeFillPie,
  EmfPlusRecordTypeDrawPie,
  EmfPlusRecordTypeDrawArc,
  EmfPlusRecordTypeFillRegion,
  EmfPlusRecordTypeFillPath,
  EmfPlusRecordTypeDrawPath,
  EmfPlusRecordTypeFillClosedCurve,
  EmfPlusRecordTypeDrawClosedCurve,
  EmfPlusRecordTypeDrawCurve,
  EmfPlusRecordTypeDrawBeziers,
  EmfPlusRecordTypeDrawImage,
  EmfPlusRecordTypeDrawImagePoints,
  EmfPlusRecordTypeDrawString,
  // Graphics State Records
  EmfPlusRecordTypeSetRenderingOrigin,
  EmfPlusRecordTypeSetAntiAliasMode,
  EmfPlusRecordTypeSetTextRenderingHint,
  EmfPlusRecordTypeSetTextContrast,
  EmfPlusRecordTypeSetInterpolationMode,
  EmfPlusRecordTypeSetPixelOffsetMode,
  EmfPlusRecordTypeSetCompositingMode,
  EmfPlusRecordTypeSetCompositingQuality,
  EmfPlusRecordTypeSave,
  EmfPlusRecordTypeRestore,
  EmfPlusRecordTypeBeginContainer,
  EmfPlusRecordTypeBeginContainerNoParams,
  EmfPlusRecordTypeEndContainer,
  EmfPlusRecordTypeSetWorldTransform,
  EmfPlusRecordTypeResetWorldTransform,
  EmfPlusRecordTypeMultiplyWorldTransform,
  EmfPlusRecordTypeTranslateWorldTransform,
  EmfPlusRecordTypeScaleWorldTransform,
  EmfPlusRecordTypeRotateWorldTransform,
  EmfPlusRecordTypeSetPageTransform,
  EmfPlusRecordTypeResetClip,
  EmfPlusRecordTypeSetClipRect,
  EmfPlusRecordTypeSetClipPath,
  EmfPlusRecordTypeSetClipRegion,
  EmfPlusRecordTypeOffsetClip,
  EmfPlusRecordTypeDrawDriverString,
  EmfPlusRecordTotal,
  EmfPlusRecordTypeMax = EmfPlusRecordTotal - 1,
  EmfPlusRecordTypeMin = EmfPlusRecordTypeHeader,
} EmfPlusRecordType;
//---------------------------------------------------------------------------
// StringFormatFlags
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// String format flags
//
// DirectionRightToLeft - For horizontal text, the reading order is
// right to left. This value is called
// the base embedding level by the Unicode
// bidirectional engine.
// For vertical text, columns are read from
// right to left.
// By default, horizontal or vertical text is
// read from left to right.
//
// DirectionVertical - Individual lines of text are vertical. In
// each line, characters progress from top to
// bottom.
// By default, lines of text are horizontal,
// each new line below the previous line.
//
// NoFitBlackBox - Allows parts of glyphs to overhang the
// bounding rectangle.
// By default glyphs are first aligned
// inside the margines, then any glyphs which
// still overhang the bounding box are
// repositioned to avoid any overhang.
// For example when an italic
// lower case letter f in a font such as
// Garamond is aligned at the far left of a
// rectangle, the lower part of the f will
// reach slightly further left than the left
// edge of the rectangle. Setting this flag
// will ensure the character aligns visually
// with the lines above and below, but may
// cause some pixels outside the formatting
// rectangle to be clipped or painted.
//
// DisplayFormatControl - Causes control characters such as the
// left-to-right mark to be shown in the
// output with a representative glyph.
//
// NoFontFallback - Disables fallback to alternate fonts for
// characters not supported in the requested
// font. Any missing characters will be
// be displayed with the fonts missing glyph,
// usually an open square.
//
// NoWrap - Disables wrapping of text between lines
// when formatting within a rectangle.
// NoWrap is implied when a point is passed
// instead of a rectangle, or when the
// specified rectangle has a zero line length.
//
// NoClip - By default text is clipped to the
// formatting rectangle. Setting NoClip
// allows overhanging pixels to affect the
// device outside the formatting rectangle.
// Pixels at the end of the line may be
// affected if the glyphs overhang their
// cells, and either the NoFitBlackBox flag
// has been set, or the glyph extends to far
// to be fitted.
// Pixels above/before the first line or
// below/after the last line may be affected
// if the glyphs extend beyond their cell
// ascent / descent. This can occur rarely
// with unusual diacritic mark combinations.
//---------------------------------------------------------------------------
typedef enum {
  StringFormatFlagsDirectionRightToLeft = 0x00000001,
  StringFormatFlagsDirectionVertical = 0x00000002,
  StringFormatFlagsNoFitBlackBox = 0x00000004,
  StringFormatFlagsDisplayFormatControl = 0x00000020,
  StringFormatFlagsNoFontFallback = 0x00000400,
  StringFormatFlagsMeasureTrailingSpaces = 0x00000800,
  StringFormatFlagsNoWrap = 0x00001000,
  StringFormatFlagsLineLimit = 0x00002000,
  StringFormatFlagsNoClip = 0x00004000
} StringFormatFlags;
//---------------------------------------------------------------------------
// StringTrimming
//---------------------------------------------------------------------------
typedef enum {
  StringTrimmingNone = 0,
  StringTrimmingCharacter = 1,
  StringTrimmingWord = 2,
  StringTrimmingEllipsisCharacter = 3,
  StringTrimmingEllipsisWord = 4,
  StringTrimmingEllipsisPath = 5
} StringTrimming;
//---------------------------------------------------------------------------
// National language digit substitution
//---------------------------------------------------------------------------
typedef enum {
  StringDigitSubstituteUser = 0, // As NLS setting
  StringDigitSubstituteNone = 1,
  StringDigitSubstituteNational = 2,
  StringDigitSubstituteTraditional = 3
} StringDigitSubstitute;
//---------------------------------------------------------------------------
// Hotkey prefix interpretation
//---------------------------------------------------------------------------
typedef enum {
  HotkeyPrefixNone = 0,
  HotkeyPrefixShow = 1,
  HotkeyPrefixHide = 2
} HotkeyPrefix;
//---------------------------------------------------------------------------
// String alignment flags
//---------------------------------------------------------------------------
typedef enum {
  // Left edge for left-to-right text,
  // right for right-to-left text,
  // and top for vertical
  StringAlignmentNear = 0,
  StringAlignmentCenter = 1,
  StringAlignmentFar = 2
} StringAlignment;
//---------------------------------------------------------------------------
// DriverStringOptions
//---------------------------------------------------------------------------
typedef enum {
  DriverStringOptionsCmapLookup = 1,
  DriverStringOptionsVertical = 2,
  DriverStringOptionsRealizedAdvance = 4,
  DriverStringOptionsLimitSubpixel = 8
} DriverStringOptions;
//---------------------------------------------------------------------------
// Flush Intention flags
//---------------------------------------------------------------------------
typedef enum {
  FlushIntentionFlush = 0, // Flush all batched rendering operations
  FlushIntentionSync = 1 // Flush all batched rendering operations
      // and wait for them to complete
} FlushIntention;
//---------------------------------------------------------------------------
// Image encoder parameter related types
//---------------------------------------------------------------------------
typedef enum {
  EncoderParameterValueTypeByte = 1, // 8-bit unsigned int
  EncoderParameterValueTypeASCII = 2, // 8-bit byte containing one 7-bit ASCII
  // code. NULL terminated.
  EncoderParameterValueTypeShort = 3, // 16-bit unsigned int
  EncoderParameterValueTypeLong = 4, // 32-bit unsigned int
  EncoderParameterValueTypeRational = 5, // Two Longs. The first Long is the
  // numerator, the second Long expresses the
  // denomintor.
  EncoderParameterValueTypeLongRange = 6, // Two longs which specify a range of
  // integer values. The first Long specifies
  // the lower end and the second one
  // specifies the higher end. All values
  // are inclusive at both ends
  EncoderParameterValueTypeUndefined = 7, // 8-bit byte that can take any value
  // depending on field definition
  EncoderParameterValueTypeRationalRange = 8 // Two Rationals. The first Rational
      // specifies the lower end and the second
      // specifies the higher end. All values
      // are inclusive at both ends
} EncoderParameterValueType;
//---------------------------------------------------------------------------
// Image encoder value types
//---------------------------------------------------------------------------
typedef enum {
  EncoderValueColorTypeCMYK,
  EncoderValueColorTypeYCCK,
  EncoderValueCompressionLZW,
  EncoderValueCompressionCCITT3,
  EncoderValueCompressionCCITT4,
  EncoderValueCompressionRle,
  EncoderValueCompressionNone,
  EncoderValueScanMethodInterlaced,
  EncoderValueScanMethodNonInterlaced,
  EncoderValueVersionGif87,
  EncoderValueVersionGif89,
  EncoderValueRenderProgressive,
  EncoderValueRenderNonProgressive,
  EncoderValueTransformRotate90,
  EncoderValueTransformRotate180,
  EncoderValueTransformRotate270,
  EncoderValueTransformFlipHorizontal,
  EncoderValueTransformFlipVertical,
  EncoderValueMultiFrame,
  EncoderValueLastFrame,
  EncoderValueFlush,
  EncoderValueFrameDimensionTime,
  EncoderValueFrameDimensionResolution,
  EncoderValueFrameDimensionPage
} EncoderValue;
//---------------------------------------------------------------------------
// Conversion of Emf To WMF Bits flags
//---------------------------------------------------------------------------
typedef enum {
  EmfToWmfBitsFlagsDefault = 0x00000000,
  EmfToWmfBitsFlagsEmbedEmf = 0x00000001,
  EmfToWmfBitsFlagsIncludePlaceable = 0x00000002,
  EmfToWmfBitsFlagsNoXORClip = 0x00000004
} EmfToWmfBitsFlags;
//---------------------------------------------------------------------------
// Test Control flags
//---------------------------------------------------------------------------
typedef enum {
  TestControlForceBilinear = 0,
  TestControlNoICM = 1,
  TestControlGetBuildNumber = 2
} GpTestControlEnum;
//--------------------------------------------------------------------------
// Callback functions
//--------------------------------------------------------------------------
typedef BOOL (CALLBACK* ImageAbort)(VOID*);
typedef ImageAbort DrawImageAbort;
typedef ImageAbort GetThumbnailImageAbort;
// Callback for EnumerateMetafile methods. The parameters are:
// recordType WMF, EMF, or EMF+ record type
// flags (always 0 for WMF/EMF records)
// dataSize size of the record data (in bytes), or 0 if no data
// data pointer to the record data, or NULL if no data
// callbackData pointer to callbackData, if any
// This method can then call Metafile::PlayRecord to play the
// record that was just enumerated. If this method returns
// FALSE, the enumeration process is aborted. Otherwise, it continues.
typedef BOOL (CALLBACK* EnumerateMetafileProc)(EmfPlusRecordType, UINT, UINT, const BYTE*, VOID*);
//--------------------------------------------------------------------------
// Primitive data types
//
// NOTE:
// Types already defined in standard header files:
// INT8
// UINT8
// INT16
// UINT16
// INT32
// UINT32
// INT64
// UINT64
//
// Avoid using the following types:
// LONG - use INT
// ULONG - use UINT
// DWORD - use UINT32
//--------------------------------------------------------------------------
typedef float REAL;
typedef unsigned int ARGB;
typedef DWORDLONG ARGB64;
typedef struct {
  int x, y, width, height;
} Rect;
typedef struct {
  REAL x, y, width, height;
} RectF;
typedef struct {
  int x, y;
} Point;
typedef struct {
  REAL x, y;
} PointF;
typedef struct {
  int width, height;
} Size;
typedef struct {
  REAL width, height;
} SizeF;
typedef struct {
  INT Count;
  PointF* Points;
  BYTE* Types;
} PathData;
typedef struct {
  INT First, Length;
} CharacterRange;
#define REAL_MAX FLT_MAX
#define REAL_MIN FLT_MIN
#define REAL_TOLERANCE (FLT_MIN * 100)
#define REAL_EPSILON 1.192092896e-07F /* FLT_EPSILON */
//--------------------------------------------------------------------------
// Status return values from GDI+ methods
//--------------------------------------------------------------------------
typedef enum {
  Ok = 0,
  GenericError = 1,
  InvalidParameter = 2,
  OutOfMemory = 3,
  ObjectBusy = 4,
  InsufficientBuffer = 5,
  NotImplemented = 6,
  Win32Error = 7,
  WrongState = 8,
  Aborted = 9,
  FileNotFound = 10,
  ValueOverflow = 11,
  AccessDenied = 12,
  UnknownImageFormat = 13,
  FontFamilyNotFound = 14,
  FontStyleNotFound = 15,
  NotTrueTypeFont = 16,
  UnsupportedGdiplusVersion = 17,
  GdiplusNotInitialized = 18,
  PropertyNotFound = 19,
  PropertyNotSupported = 20
} Status;
typedef enum {
  DebugEventLevelFatal,
  DebugEventLevelWarning
} DebugEventLevel;
#define GDIP_HANDLE(GpPath) typedef void GpPath;
//---------------------------------------------------------------------------
// Private GDI+ classes for internal type checking
//---------------------------------------------------------------------------
GDIP_HANDLE(GpGraphics)
GDIP_HANDLE(GpBrush)
GDIP_HANDLE(GpTexture)
GDIP_HANDLE(GpSolidFill)
GDIP_HANDLE(GpLineGradient)
GDIP_HANDLE(GpPathGradient)
GDIP_HANDLE(GpHatch)
GDIP_HANDLE(GpPen)
GDIP_HANDLE(GpCustomLineCap)
GDIP_HANDLE(GpAdjustableArrowCap)
GDIP_HANDLE(GpImage)
GDIP_HANDLE(GpBitmap)
GDIP_HANDLE(GpMetafile)
GDIP_HANDLE(GpImageAttributes)
GDIP_HANDLE(GpPath)
GDIP_HANDLE(GpRegion)
GDIP_HANDLE(GpPathIterator)
GDIP_HANDLE(GpFontFamily)
GDIP_HANDLE(GpFont)
GDIP_HANDLE(GpStringFormat)
GDIP_HANDLE(GpFontCollection)
GDIP_HANDLE(GpInstalledFontCollection)
GDIP_HANDLE(GpPrivateFontCollection)
GDIP_HANDLE(GpCachedBitmap);
//typedef int Status;
typedef Status GpStatus;
typedef FillMode GpFillMode;
typedef WrapMode GpWrapMode;
typedef Unit GpUnit;
typedef CoordinateSpace GpCoordinateSpace;
typedef PointF GpPointF;
typedef Point GpPoint;
typedef RectF GpRectF;
typedef Rect GpRect;
typedef SizeF GpSizeF;
typedef HatchStyle GpHatchStyle;
typedef DashStyle GpDashStyle;
typedef LineCap GpLineCap;
typedef DashCap GpDashCap;
typedef PenAlignment GpPenAlignment;
typedef LineJoin GpLineJoin;
typedef PenType GpPenType;
GDIP_HANDLE(GpMatrix);
GDIP_HANDLE(GpBrushType);
typedef int GpMatrixOrder;
typedef int GpFlushIntention;
GDIP_HANDLE(GpPathData);
//typedef GpImage GpBitmap;
//typedef GpBrush GpSolidFill;
#define ALPHA_SHIFT 24
#define RED_SHIFT 16
#define GREEN_SHIFT 8
#define BLUE_SHIFT 0
#define ALPHA_MASK ((ARGB) 0xff << ALPHA_SHIFT)
// In-memory pixel data formats:
// bits 0-7 = format index
// bits 8-15 = pixel size (in bits)
// bits 16-23 = flags
// bits 24-31 = reserved
typedef INT PixelFormat;
#define PixelFormatIndexed   0x00010000 // Indexes into a palette
#define PixelFormatGDI       0x00020000 // Is a GDI-supported format
#define PixelFormatAlpha     0x00040000 // Has an alpha component
#define PixelFormatPAlpha    0x00080000 // Pre-multiplied alpha
#define PixelFormatExtended  0x00100000 // Extended color 16 bits/channel
#define PixelFormatCanonical 0x00200000
#define PixelFormatUndefined 0
#define PixelFormatDontCare  0
#define PixelFormat1bppIndexed (1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define PixelFormat4bppIndexed (2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define PixelFormat8bppIndexed (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define PixelFormat16bppGrayScale (4 | (16 << 8) | PixelFormatExtended)
#define PixelFormat16bppRGB555 (5 | (16 << 8) | PixelFormatGDI)
#define PixelFormat16bppRGB565 (6 | (16 << 8) | PixelFormatGDI)
#define PixelFormat16bppARGB1555 (7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
#define PixelFormat24bppRGB (8 | (24 << 8) | PixelFormatGDI)
#define PixelFormat32bppRGB (9 | (32 << 8) | PixelFormatGDI)
#define PixelFormat32bppARGB (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
#define PixelFormat32bppPARGB (11 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
#define PixelFormat48bppRGB (12 | (48 << 8) | PixelFormatExtended)
#define PixelFormat64bppARGB (13 | (64 << 8) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended)
#define PixelFormat64bppPARGB (14 | (64 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatExtended)
#define PixelFormatMax 15
CV_INLINE UINT GetPixelFormatSize(PixelFormat pixfmt)
{
  return (pixfmt >> 8) & 0xff;
}
CV_INLINE BOOL IsIndexedPixelFormat(PixelFormat pixfmt)
{
  return (pixfmt & PixelFormatIndexed) != 0;
}
CV_INLINE BOOL IsAlphaPixelFormat(PixelFormat pixfmt)
{
  return (pixfmt & PixelFormatAlpha) != 0;
}
CV_INLINE BOOL IsExtendedPixelFormat(PixelFormat pixfmt)
{
  return (pixfmt & PixelFormatExtended) != 0;
}
//--------------------------------------------------------------------------
// Determine if the Pixel Format is Canonical format:
// PixelFormat32bppARGB
// PixelFormat32bppPARGB
// PixelFormat64bppARGB
// PixelFormat64bppPARGB
//--------------------------------------------------------------------------
CV_INLINE BOOL IsCanonicalPixelFormat(PixelFormat pixfmt)
{
  return (pixfmt & PixelFormatCanonical) != 0;
}
typedef enum {
  PaletteFlagsHasAlpha = 0x0001,
  PaletteFlagsGrayScale = 0x0002,
  PaletteFlagsHalftone = 0x0004
} PaletteFlags;
typedef struct {
  UINT Flags; // Palette flags
  UINT Count; // Number of color entries
  ARGB Entries[1]; // Palette color entries
} ColorPalette;
typedef struct {
  CLSID Clsid;
  GUID FormatID;
  const WCHAR* CodecName;
  const WCHAR* DllName;
  const WCHAR* FormatDescription;
  const WCHAR* FilenameExtension;
  const WCHAR* MimeType;
  DWORD Flags;
  DWORD Version;
  DWORD SigCount;
  DWORD SigSize;
  const BYTE* SigPattern;
  const BYTE* SigMask;
} ImageCodecInfo;
//--------------------------------------------------------------------------
// Information flags about image codecs
//--------------------------------------------------------------------------
typedef enum {
  ImageCodecFlagsEncoder = 0x00000001,
  ImageCodecFlagsDecoder = 0x00000002,
  ImageCodecFlagsSupportBitmap = 0x00000004,
  ImageCodecFlagsSupportVector = 0x00000008,
  ImageCodecFlagsSeekableEncode = 0x00000010,
  ImageCodecFlagsBlockingDecode = 0x00000020,
  ImageCodecFlagsBuiltin = 0x00010000,
  ImageCodecFlagsSystem = 0x00020000,
  ImageCodecFlagsUser = 0x00040000
} ImageCodecFlags;
//---------------------------------------------------------------------------
// Access modes used when calling Image::LockBits
//---------------------------------------------------------------------------
typedef enum {
  ImageLockModeRead = 0x0001,
  ImageLockModeWrite = 0x0002,
  ImageLockModeUserInputBuf = 0x0004
} ImageLockMode;
//---------------------------------------------------------------------------
// Information about image pixel data
//---------------------------------------------------------------------------
typedef struct {
  UINT Width;
  UINT Height;
  INT Stride;
  PixelFormat PixelFormat;
  VOID* Scan0;
  UINT_PTR Reserved;
} BitmapData;
//---------------------------------------------------------------------------
// Image flags
//---------------------------------------------------------------------------
typedef enum {
  ImageFlagsNone = 0,
  // Low-word: shared with SINKFLAG_x
  ImageFlagsScalable = 0x0001,
  ImageFlagsHasAlpha = 0x0002,
  ImageFlagsHasTranslucent = 0x0004,
  ImageFlagsPartiallyScalable = 0x0008,
  // Low-word: color space definition
  ImageFlagsColorSpaceRGB = 0x0010,
  ImageFlagsColorSpaceCMYK = 0x0020,
  ImageFlagsColorSpaceGRAY = 0x0040,
  ImageFlagsColorSpaceYCBCR = 0x0080,
  ImageFlagsColorSpaceYCCK = 0x0100,
  // Low-word: image size info
  ImageFlagsHasRealDPI = 0x1000,
  ImageFlagsHasRealPixelSize = 0x2000,
  // High-word
  ImageFlagsReadOnly = 0x00010000,
  ImageFlagsCaching = 0x00020000
} ImageFlags;
typedef enum {
  RotateNoneFlipNone = 0,
  Rotate90FlipNone = 1,
  Rotate180FlipNone = 2,
  Rotate270FlipNone = 3,
  RotateNoneFlipX = 4,
  Rotate90FlipX = 5,
  Rotate180FlipX = 6,
  Rotate270FlipX = 7,
  RotateNoneFlipY = Rotate180FlipX,
  Rotate90FlipY = Rotate270FlipX,
  Rotate180FlipY = RotateNoneFlipX,
  Rotate270FlipY = Rotate90FlipX,
  RotateNoneFlipXY = Rotate180FlipNone,
  Rotate90FlipXY = Rotate270FlipNone,
  Rotate180FlipXY = RotateNoneFlipNone,
  Rotate270FlipXY = Rotate90FlipNone
} RotateFlipType;
//---------------------------------------------------------------------------
// Encoder Parameter structure
//---------------------------------------------------------------------------
typedef struct {
  GUID Guid; // GUID of the parameter
  ULONG NumberOfValues; // Number of the parameter values
  ULONG Type; // Value type, like ValueTypeLONG etc.
  VOID* Value; // A pointer to the parameter values
} EncoderParameter;
//---------------------------------------------------------------------------
// Encoder Parameters structure
//---------------------------------------------------------------------------
typedef struct {
  UINT Count; // Number of parameters in this structure
  EncoderParameter Parameter[1]; // Parameter values
} EncoderParameters;
//---------------------------------------------------------------------------
// Property Item
//---------------------------------------------------------------------------
typedef struct {
  PROPID id; // ID of this property
  ULONG length; // Length of the property value, in bytes
  WORD type; // Type of the value, as one of TAG_TYPE_XXX
  // defined above
  VOID* value; // property value
} PropertyItem;
//---------------------------------------------------------------------------
// Image property types
//---------------------------------------------------------------------------
#define PropertyTagTypeByte 1
#define PropertyTagTypeASCII 2
#define PropertyTagTypeShort 3
#define PropertyTagTypeLong 4
#define PropertyTagTypeRational 5
#define PropertyTagTypeUndefined 7
#define PropertyTagTypeSLONG 9
#define PropertyTagTypeSRational 10
//---------------------------------------------------------------------------
// Image property ID tags
//---------------------------------------------------------------------------
#define PropertyTagExifIFD 0x8769
#define PropertyTagGpsIFD 0x8825
#define PropertyTagNewSubfileType 0x00FE
#define PropertyTagSubfileType 0x00FF
#define PropertyTagImageWidth 0x0100
#define PropertyTagImageHeight 0x0101
#define PropertyTagBitsPerSample 0x0102
#define PropertyTagCompression 0x0103
#define PropertyTagPhotometricInterp 0x0106
#define PropertyTagThreshHolding 0x0107
#define PropertyTagCellWidth 0x0108
#define PropertyTagCellHeight 0x0109
#define PropertyTagFillOrder 0x010A
#define PropertyTagDocumentName 0x010D
#define PropertyTagImageDescription 0x010E
#define PropertyTagEquipMake 0x010F
#define PropertyTagEquipModel 0x0110
#define PropertyTagStripOffsets 0x0111
#define PropertyTagOrientation 0x0112
#define PropertyTagSamplesPerPixel 0x0115
#define PropertyTagRowsPerStrip 0x0116
#define PropertyTagStripBytesCount 0x0117
#define PropertyTagMinSampleValue 0x0118
#define PropertyTagMaxSampleValue 0x0119
#define PropertyTagXResolution 0x011A // Image resolution in width direction
#define PropertyTagYResolution 0x011B // Image resolution in height direction
#define PropertyTagPlanarConfig 0x011C // Image data arrangement
#define PropertyTagPageName 0x011D
#define PropertyTagXPosition 0x011E
#define PropertyTagYPosition 0x011F
#define PropertyTagFreeOffset 0x0120
#define PropertyTagFreeByteCounts 0x0121
#define PropertyTagGrayResponseUnit 0x0122
#define PropertyTagGrayResponseCurve 0x0123
#define PropertyTagT4Option 0x0124
#define PropertyTagT6Option 0x0125
#define PropertyTagResolutionUnit 0x0128 // Unit of X and Y resolution
#define PropertyTagPageNumber 0x0129
#define PropertyTagTransferFuncition 0x012D
#define PropertyTagSoftwareUsed 0x0131
#define PropertyTagDateTime 0x0132
#define PropertyTagArtist 0x013B
#define PropertyTagHostComputer 0x013C
#define PropertyTagPredictor 0x013D
#define PropertyTagWhitePoint 0x013E
#define PropertyTagPrimaryChromaticities 0x013F
#define PropertyTagColorMap 0x0140
#define PropertyTagHalftoneHints 0x0141
#define PropertyTagTileWidth 0x0142
#define PropertyTagTileLength 0x0143
#define PropertyTagTileOffset 0x0144
#define PropertyTagTileByteCounts 0x0145
#define PropertyTagInkSet 0x014C
#define PropertyTagInkNames 0x014D
#define PropertyTagNumberOfInks 0x014E
#define PropertyTagDotRange 0x0150
#define PropertyTagTargetPrinter 0x0151
#define PropertyTagExtraSamples 0x0152
#define PropertyTagSampleFormat 0x0153
#define PropertyTagSMinSampleValue 0x0154
#define PropertyTagSMaxSampleValue 0x0155
#define PropertyTagTransferRange 0x0156
#define PropertyTagJPEGProc 0x0200
#define PropertyTagJPEGInterFormat 0x0201
#define PropertyTagJPEGInterLength 0x0202
#define PropertyTagJPEGRestartInterval 0x0203
#define PropertyTagJPEGLosslessPredictors 0x0205
#define PropertyTagJPEGPointTransforms 0x0206
#define PropertyTagJPEGQTables 0x0207
#define PropertyTagJPEGDCTables 0x0208
#define PropertyTagJPEGACTables 0x0209
#define PropertyTagYCbCrCoefficients 0x0211
#define PropertyTagYCbCrSubsampling 0x0212
#define PropertyTagYCbCrPositioning 0x0213
#define PropertyTagREFBlackWhite 0x0214
#define PropertyTagICCProfile 0x8773 // This TAG is defined by ICC
// for embedded ICC in TIFF
#define PropertyTagGamma 0x0301
#define PropertyTagICCProfileDescriptor 0x0302
#define PropertyTagSRGBRenderingIntent 0x0303
#define PropertyTagImageTitle 0x0320
#define PropertyTagCopyright 0x8298
// Extra TAGs (Like Adobe Image Information tags etc.)
#define PropertyTagResolutionXUnit 0x5001
#define PropertyTagResolutionYUnit 0x5002
#define PropertyTagResolutionXLengthUnit 0x5003
#define PropertyTagResolutionYLengthUnit 0x5004
#define PropertyTagPrintFlags 0x5005
#define PropertyTagPrintFlagsVersion 0x5006
#define PropertyTagPrintFlagsCrop 0x5007
#define PropertyTagPrintFlagsBleedWidth 0x5008
#define PropertyTagPrintFlagsBleedWidthScale 0x5009
#define PropertyTagHalftoneLPI 0x500A
#define PropertyTagHalftoneLPIUnit 0x500B
#define PropertyTagHalftoneDegree 0x500C
#define PropertyTagHalftoneShape 0x500D
#define PropertyTagHalftoneMisc 0x500E
#define PropertyTagHalftoneScreen 0x500F
#define PropertyTagJPEGQuality 0x5010
#define PropertyTagGridSize 0x5011
#define PropertyTagThumbnailFormat 0x5012 // 1 = JPEG, 0 = RAW RGB
#define PropertyTagThumbnailWidth 0x5013
#define PropertyTagThumbnailHeight 0x5014
#define PropertyTagThumbnailColorDepth 0x5015
#define PropertyTagThumbnailPlanes 0x5016
#define PropertyTagThumbnailRawBytes 0x5017
#define PropertyTagThumbnailSize 0x5018
#define PropertyTagThumbnailCompressedSize 0x5019
#define PropertyTagColorTransferFunction 0x501A
#define PropertyTagThumbnailData 0x501B// RAW thumbnail bits in
// JPEG format or RGB format
// depends on
// PropertyTagThumbnailFormat
// Thumbnail related TAGs
#define PropertyTagThumbnailImageWidth 0x5020 // Thumbnail width
#define PropertyTagThumbnailImageHeight 0x5021 // Thumbnail height
#define PropertyTagThumbnailBitsPerSample 0x5022 // Number of bits per
// component
#define PropertyTagThumbnailCompression 0x5023 // Compression Scheme
#define PropertyTagThumbnailPhotometricInterp 0x5024 // Pixel composition
#define PropertyTagThumbnailImageDescription 0x5025 // Image Tile
#define PropertyTagThumbnailEquipMake 0x5026 // Manufacturer of Image
// Input equipment
#define PropertyTagThumbnailEquipModel 0x5027 // Model of Image input
// equipment
#define PropertyTagThumbnailStripOffsets 0x5028 // Image data location
#define PropertyTagThumbnailOrientation 0x5029 // Orientation of image
#define PropertyTagThumbnailSamplesPerPixel 0x502A // Number of components
#define PropertyTagThumbnailRowsPerStrip 0x502B // Number of rows per strip
#define PropertyTagThumbnailStripBytesCount 0x502C // Bytes per compressed
// strip
#define PropertyTagThumbnailResolutionX 0x502D // Resolution in width
// direction
#define PropertyTagThumbnailResolutionY 0x502E // Resolution in height
// direction
#define PropertyTagThumbnailPlanarConfig 0x502F // Image data arrangement
#define PropertyTagThumbnailResolutionUnit 0x5030 // Unit of X and Y
// Resolution
#define PropertyTagThumbnailTransferFunction 0x5031 // Transfer function
#define PropertyTagThumbnailSoftwareUsed 0x5032 // Software used
#define PropertyTagThumbnailDateTime 0x5033 // File change date and
// time
#define PropertyTagThumbnailArtist 0x5034 // Person who created the
// image
#define PropertyTagThumbnailWhitePoint 0x5035 // White point chromaticity
#define PropertyTagThumbnailPrimaryChromaticities 0x5036
// Chromaticities of
// primaries
#define PropertyTagThumbnailYCbCrCoefficients 0x5037 // Color space transforma-
// tion coefficients
#define PropertyTagThumbnailYCbCrSubsampling 0x5038 // Subsampling ratio of Y
// to C
#define PropertyTagThumbnailYCbCrPositioning 0x5039 // Y and C position
#define PropertyTagThumbnailRefBlackWhite 0x503A // Pair of black and white
// reference values
#define PropertyTagThumbnailCopyRight 0x503B // CopyRight holder
#define PropertyTagLuminanceTable 0x5090
#define PropertyTagChrominanceTable 0x5091
#define PropertyTagFrameDelay 0x5100
#define PropertyTagLoopCount 0x5101
#define PropertyTagPixelUnit 0x5110 // Unit specifier for pixel/unit
#define PropertyTagPixelPerUnitX 0x5111 // Pixels per unit in X
#define PropertyTagPixelPerUnitY 0x5112 // Pixels per unit in Y
#define PropertyTagPaletteHistogram 0x5113 // Palette histogram
// EXIF specific tag
#define PropertyTagExifExposureTime 0x829A
#define PropertyTagExifFNumber 0x829D
#define PropertyTagExifExposureProg 0x8822
#define PropertyTagExifSpectralSense 0x8824
#define PropertyTagExifISOSpeed 0x8827
#define PropertyTagExifOECF 0x8828
#define PropertyTagExifVer 0x9000
#define PropertyTagExifDTOrig 0x9003 // Date & time of original
#define PropertyTagExifDTDigitized 0x9004 // Date & time of digital data generation
#define PropertyTagExifCompConfig 0x9101
#define PropertyTagExifCompBPP 0x9102
#define PropertyTagExifShutterSpeed 0x9201
#define PropertyTagExifAperture 0x9202
#define PropertyTagExifBrightness 0x9203
#define PropertyTagExifExposureBias 0x9204
#define PropertyTagExifMaxAperture 0x9205
#define PropertyTagExifSubjectDist 0x9206
#define PropertyTagExifMeteringMode 0x9207
#define PropertyTagExifLightSource 0x9208
#define PropertyTagExifFlash 0x9209
#define PropertyTagExifFocalLength 0x920A
#define PropertyTagExifMakerNote 0x927C
#define PropertyTagExifUserComment 0x9286
#define PropertyTagExifDTSubsec 0x9290 // Date & Time subseconds
#define PropertyTagExifDTOrigSS 0x9291 // Date & Time original subseconds
#define PropertyTagExifDTDigSS 0x9292 // Date & TIme digitized subseconds
#define PropertyTagExifFPXVer 0xA000
#define PropertyTagExifColorSpace 0xA001
#define PropertyTagExifPixXDim 0xA002
#define PropertyTagExifPixYDim 0xA003
#define PropertyTagExifRelatedWav 0xA004 // related sound file
#define PropertyTagExifInterop 0xA005
#define PropertyTagExifFlashEnergy 0xA20B
#define PropertyTagExifSpatialFR 0xA20C // Spatial Frequency Response
#define PropertyTagExifFocalXRes 0xA20E // Focal Plane X Resolution
#define PropertyTagExifFocalYRes 0xA20F // Focal Plane Y Resolution
#define PropertyTagExifFocalResUnit 0xA210 // Focal Plane Resolution Unit
#define PropertyTagExifSubjectLoc 0xA214
#define PropertyTagExifExposureIndex 0xA215
#define PropertyTagExifSensingMethod 0xA217
#define PropertyTagExifFileSource 0xA300
#define PropertyTagExifSceneType 0xA301
#define PropertyTagExifCfaPattern 0xA302
#define PropertyTagGpsVer 0x0000
#define PropertyTagGpsLatitudeRef 0x0001
#define PropertyTagGpsLatitude 0x0002
#define PropertyTagGpsLongitudeRef 0x0003
#define PropertyTagGpsLongitude 0x0004
#define PropertyTagGpsAltitudeRef 0x0005
#define PropertyTagGpsAltitude 0x0006
#define PropertyTagGpsGpsTime 0x0007
#define PropertyTagGpsGpsSatellites 0x0008
#define PropertyTagGpsGpsStatus 0x0009
#define PropertyTagGpsGpsMeasureMode 0x00A
#define PropertyTagGpsGpsDop 0x000B // Measurement precision
#define PropertyTagGpsSpeedRef 0x000C
#define PropertyTagGpsSpeed 0x000D
#define PropertyTagGpsTrackRef 0x000E
#define PropertyTagGpsTrack 0x000F
#define PropertyTagGpsImgDirRef 0x0010
#define PropertyTagGpsImgDir 0x0011
#define PropertyTagGpsMapDatum 0x0012
#define PropertyTagGpsDestLatRef 0x0013
#define PropertyTagGpsDestLat 0x0014
#define PropertyTagGpsDestLongRef 0x0015
#define PropertyTagGpsDestLong 0x0016
#define PropertyTagGpsDestBearRef 0x0017
#define PropertyTagGpsDestBear 0x0018
#define PropertyTagGpsDestDistRef 0x0019
#define PropertyTagGpsDestDist 0x001A
typedef struct IDirectDrawSurface7 IDirectDrawSurface7;
//----------------------------------------------------------------------------
// Color mode
//----------------------------------------------------------------------------
typedef enum {
  ColorModeARGB32 = 0,
  ColorModeARGB64 = 1
} ColorMode;
//----------------------------------------------------------------------------
// Color Channel flags
//----------------------------------------------------------------------------
typedef enum {
  ColorChannelFlagsC = 0,
  ColorChannelFlagsM,
  ColorChannelFlagsY,
  ColorChannelFlagsK,
  ColorChannelFlagsLast
} ColorChannelFlags;
//----------------------------------------------------------------------------
// Color matrix
//----------------------------------------------------------------------------
typedef struct {
  REAL m[5][5];
} ColorMatrix;
//----------------------------------------------------------------------------
// Color Matrix flags
//----------------------------------------------------------------------------
typedef enum {
  ColorMatrixFlagsDefault = 0,
  ColorMatrixFlagsSkipGrays = 1,
  ColorMatrixFlagsAltGray = 2
} ColorMatrixFlags;
//----------------------------------------------------------------------------
// Color Adjust Type
//----------------------------------------------------------------------------
typedef enum {
  ColorAdjustTypeDefault,
  ColorAdjustTypeBitmap,
  ColorAdjustTypeBrush,
  ColorAdjustTypePen,
  ColorAdjustTypeText,
  ColorAdjustTypeCount,
  ColorAdjustTypeAny // Reserved
} ColorAdjustType;
//typedef struct { ARGB Argb; } Color;
typedef ARGB Color;
//----------------------------------------------------------------------------
// Color Map
//----------------------------------------------------------------------------
typedef struct {
  Color oldColor;
  Color newColor;
} ColorMap;
//----------------------------------------------------------------------------
// GraphicsPath APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreatePath(GpFillMode brushMode, GpPath** path);
GpStatus WINGDIPAPI GdipCreatePath2(GDIPCONST GpPointF*, GDIPCONST BYTE*, INT, GpFillMode, GpPath** path);
GpStatus WINGDIPAPI GdipCreatePath2I(GDIPCONST GpPoint*, GDIPCONST BYTE*, INT, GpFillMode, GpPath** path);
GpStatus WINGDIPAPI GdipClonePath(GpPath* path, GpPath** clonePath);
GpStatus WINGDIPAPI GdipDeletePath(GpPath* path);
GpStatus WINGDIPAPI GdipResetPath(GpPath* path);
GpStatus WINGDIPAPI GdipGetPointCount(GpPath* path, INT* count);
GpStatus WINGDIPAPI GdipGetPathTypes(GpPath* path, BYTE* types, INT count);
GpStatus WINGDIPAPI GdipGetPathPoints(GpPath*, GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipGetPathPointsI(GpPath*, GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipGetPathFillMode(GpPath* path, GpFillMode* fillmode);
GpStatus WINGDIPAPI GdipSetPathFillMode(GpPath* path, GpFillMode fillmode);
GpStatus WINGDIPAPI GdipGetPathData(GpPath* path, GpPathData* pathData);
GpStatus WINGDIPAPI GdipStartPathFigure(GpPath* path);
GpStatus WINGDIPAPI GdipClosePathFigure(GpPath* path);
GpStatus WINGDIPAPI GdipClosePathFigures(GpPath* path);
GpStatus WINGDIPAPI GdipSetPathMarker(GpPath* path);
GpStatus WINGDIPAPI GdipClearPathMarkers(GpPath* path);
GpStatus WINGDIPAPI GdipReversePath(GpPath* path);
GpStatus WINGDIPAPI GdipGetPathLastPoint(GpPath* path, GpPointF* lastPoint);
GpStatus WINGDIPAPI GdipAddPathLine(GpPath* path, REAL x1, REAL y1, REAL x2, REAL y2);
GpStatus WINGDIPAPI GdipAddPathLine2(GpPath* path, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipAddPathArc(GpPath* path, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipAddPathBezier(GpPath* path, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
GpStatus WINGDIPAPI GdipAddPathBeziers(GpPath* path, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipAddPathCurve(GpPath* path, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipAddPathCurve2(GpPath* path, GDIPCONST GpPointF* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipAddPathCurve3(GpPath* path, GDIPCONST GpPointF* points, INT count, INT offset, INT numberOfSegments, REAL tension);
GpStatus WINGDIPAPI GdipAddPathClosedCurve(GpPath* path, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipAddPathClosedCurve2(GpPath* path, GDIPCONST GpPointF* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipAddPathRectangle(GpPath* path, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipAddPathRectangles(GpPath* path, GDIPCONST GpRectF* rects, INT count);
GpStatus WINGDIPAPI GdipAddPathEllipse(GpPath* path, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipAddPathPie(GpPath* path, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipAddPathPolygon(GpPath* path, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipAddPathPath(GpPath* path, GDIPCONST GpPath* addingPath, BOOL connect);
GpStatus WINGDIPAPI GdipAddPathString(GpPath* path, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFontFamily* family, INT style, REAL emSize, GDIPCONST RectF* layoutRect, GDIPCONST GpStringFormat* format);
GpStatus WINGDIPAPI GdipAddPathStringI(GpPath* path, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFontFamily* family, INT style, REAL emSize, GDIPCONST Rect* layoutRect, GDIPCONST GpStringFormat* format);
GpStatus WINGDIPAPI GdipAddPathLineI(GpPath* path, INT x1, INT y1, INT x2, INT y2);
GpStatus WINGDIPAPI GdipAddPathLine2I(GpPath* path, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipAddPathArcI(GpPath* path, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipAddPathBezierI(GpPath* path, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4);
GpStatus WINGDIPAPI GdipAddPathBeziersI(GpPath* path, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipAddPathCurveI(GpPath* path, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipAddPathCurve2I(GpPath* path, GDIPCONST GpPoint* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipAddPathCurve3I(GpPath* path, GDIPCONST GpPoint* points, INT count, INT offset, INT numberOfSegments, REAL tension);
GpStatus WINGDIPAPI GdipAddPathClosedCurveI(GpPath* path, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipAddPathClosedCurve2I(GpPath* path, GDIPCONST GpPoint* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipAddPathRectangleI(GpPath* path, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipAddPathRectanglesI(GpPath* path, GDIPCONST GpRect* rects, INT count);
GpStatus WINGDIPAPI GdipAddPathEllipseI(GpPath* path, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipAddPathPieI(GpPath* path, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipAddPathPolygonI(GpPath* path, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipFlattenPath(GpPath* path, GpMatrix* matrix, REAL flatness);
GpStatus WINGDIPAPI GdipWindingModeOutline(GpPath* path, GpMatrix* matrix, REAL flatness);
GpStatus WINGDIPAPI GdipWidenPath(GpPath* nativePath, GpPen* pen, GpMatrix* matrix, REAL flatness);
GpStatus WINGDIPAPI GdipWarpPath(GpPath* path, GpMatrix* matrix, GDIPCONST GpPointF* points, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, WarpMode warpMode, REAL flatness);
GpStatus WINGDIPAPI GdipTransformPath(GpPath* path, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipGetPathWorldBounds(GpPath* path, GpRectF* bounds, GDIPCONST GpMatrix* matrix, GDIPCONST GpPen* pen);
GpStatus WINGDIPAPI GdipGetPathWorldBoundsI(GpPath* path, GpRect* bounds, GDIPCONST GpMatrix* matrix, GDIPCONST GpPen* pen);
GpStatus WINGDIPAPI GdipIsVisiblePathPoint(GpPath* path, REAL x, REAL y, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisiblePathPointI(GpPath* path, INT x, INT y, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsOutlineVisiblePathPoint(GpPath* path, REAL x, REAL y, GpPen* pen, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsOutlineVisiblePathPointI(GpPath* path, INT x, INT y, GpPen* pen, GpGraphics* graphics, BOOL* result);
//----------------------------------------------------------------------------
// PathIterator APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreatePathIter(GpPathIterator** iterator, GpPath* path);
GpStatus WINGDIPAPI GdipDeletePathIter(GpPathIterator* iterator);
GpStatus WINGDIPAPI GdipPathIterNextSubpath(GpPathIterator* iterator, INT* resultCount, INT* startIndex, INT* endIndex, BOOL* isClosed);
GpStatus WINGDIPAPI GdipPathIterNextSubpathPath(GpPathIterator* iterator, INT* resultCount, GpPath* path, BOOL* isClosed);
GpStatus WINGDIPAPI GdipPathIterNextPathType(GpPathIterator* iterator, INT* resultCount, BYTE* pathType, INT* startIndex, INT* endIndex);
GpStatus WINGDIPAPI GdipPathIterNextMarker(GpPathIterator* iterator, INT* resultCount, INT* startIndex, INT* endIndex);
GpStatus WINGDIPAPI GdipPathIterNextMarkerPath(GpPathIterator* iterator, INT* resultCount, GpPath* path);
GpStatus WINGDIPAPI GdipPathIterGetCount(GpPathIterator* iterator, INT* count);
GpStatus WINGDIPAPI GdipPathIterGetSubpathCount(GpPathIterator* iterator, INT* count);
GpStatus WINGDIPAPI GdipPathIterIsValid(GpPathIterator* iterator, BOOL* valid);
GpStatus WINGDIPAPI GdipPathIterHasCurve(GpPathIterator* iterator, BOOL* hasCurve);
GpStatus WINGDIPAPI GdipPathIterRewind(GpPathIterator* iterator);
GpStatus WINGDIPAPI GdipPathIterEnumerate(GpPathIterator* iterator, INT* resultCount, GpPointF* points, BYTE* types, INT count);
GpStatus WINGDIPAPI GdipPathIterCopyData(GpPathIterator* iterator, INT* resultCount, GpPointF* points, BYTE* types, INT startIndex, INT endIndex);
//----------------------------------------------------------------------------
// Matrix APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateMatrix(GpMatrix** matrix);
GpStatus WINGDIPAPI GdipCreateMatrix2(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy, GpMatrix** matrix);
GpStatus WINGDIPAPI GdipCreateMatrix3(GDIPCONST GpRectF* rect, GDIPCONST GpPointF* dstplg, GpMatrix** matrix);
GpStatus WINGDIPAPI GdipCreateMatrix3I(GDIPCONST GpRect* rect, GDIPCONST GpPoint* dstplg, GpMatrix** matrix);
GpStatus WINGDIPAPI GdipCloneMatrix(GpMatrix* matrix, GpMatrix** cloneMatrix);
GpStatus WINGDIPAPI GdipDeleteMatrix(GpMatrix* matrix);
GpStatus WINGDIPAPI GdipSetMatrixElements(GpMatrix* matrix, REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy);
GpStatus WINGDIPAPI GdipMultiplyMatrix(GpMatrix* matrix, GpMatrix* matrix2, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipTranslateMatrix(GpMatrix* matrix, REAL offsetX, REAL offsetY, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipScaleMatrix(GpMatrix* matrix, REAL scaleX, REAL scaleY, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipRotateMatrix(GpMatrix* matrix, REAL angle, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipShearMatrix(GpMatrix* matrix, REAL shearX, REAL shearY, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipInvertMatrix(GpMatrix* matrix);
GpStatus WINGDIPAPI GdipTransformMatrixPoints(GpMatrix* matrix, GpPointF* pts, INT count);
GpStatus WINGDIPAPI GdipTransformMatrixPointsI(GpMatrix* matrix, GpPoint* pts, INT count);
GpStatus WINGDIPAPI GdipVectorTransformMatrixPoints(GpMatrix* matrix, GpPointF* pts, INT count);
GpStatus WINGDIPAPI GdipVectorTransformMatrixPointsI(GpMatrix* matrix, GpPoint* pts, INT count);
GpStatus WINGDIPAPI GdipGetMatrixElements(GDIPCONST GpMatrix* matrix, REAL* matrixOut);
GpStatus WINGDIPAPI GdipIsMatrixInvertible(GDIPCONST GpMatrix* matrix, BOOL* result);
GpStatus WINGDIPAPI GdipIsMatrixIdentity(GDIPCONST GpMatrix* matrix, BOOL* result);
GpStatus WINGDIPAPI GdipIsMatrixEqual(GDIPCONST GpMatrix* matrix, GDIPCONST GpMatrix* matrix2, BOOL* result);
//----------------------------------------------------------------------------
// Region APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateRegion(GpRegion** region);
GpStatus WINGDIPAPI GdipCreateRegionRect(GDIPCONST GpRectF* rect, GpRegion** region);
GpStatus WINGDIPAPI GdipCreateRegionRectI(GDIPCONST GpRect* rect, GpRegion** region);
GpStatus WINGDIPAPI GdipCreateRegionPath(GpPath* path, GpRegion** region);
GpStatus WINGDIPAPI GdipCreateRegionRgnData(GDIPCONST BYTE* regionData, INT size, GpRegion** region);
GpStatus WINGDIPAPI GdipCreateRegionHrgn(HRGN hRgn, GpRegion** region);
GpStatus WINGDIPAPI GdipCloneRegion(GpRegion* region, GpRegion** cloneRegion);
GpStatus WINGDIPAPI GdipDeleteRegion(GpRegion* region);
GpStatus WINGDIPAPI GdipSetInfinite(GpRegion* region);
GpStatus WINGDIPAPI GdipSetEmpty(GpRegion* region);
GpStatus WINGDIPAPI GdipCombineRegionRect(GpRegion* region, GDIPCONST GpRectF* rect, CombineMode combineMode);
GpStatus WINGDIPAPI GdipCombineRegionRectI(GpRegion* region, GDIPCONST GpRect* rect, CombineMode combineMode);
GpStatus WINGDIPAPI GdipCombineRegionPath(GpRegion* region, GpPath* path, CombineMode combineMode);
GpStatus WINGDIPAPI GdipCombineRegionRegion(GpRegion* region, GpRegion* region2, CombineMode combineMode);
GpStatus WINGDIPAPI GdipTranslateRegion(GpRegion* region, REAL dx, REAL dy);
GpStatus WINGDIPAPI GdipTranslateRegionI(GpRegion* region, INT dx, INT dy);
GpStatus WINGDIPAPI GdipTransformRegion(GpRegion* region, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipGetRegionBounds(GpRegion* region, GpGraphics* graphics, GpRectF* rect);
GpStatus WINGDIPAPI GdipGetRegionBoundsI(GpRegion* region, GpGraphics* graphics, GpRect* rect);
GpStatus WINGDIPAPI GdipGetRegionHRgn(GpRegion* region, GpGraphics* graphics, HRGN* hRgn);
GpStatus WINGDIPAPI GdipIsEmptyRegion(GpRegion* region, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsInfiniteRegion(GpRegion* region, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsEqualRegion(GpRegion* region, GpRegion* region2, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipGetRegionDataSize(GpRegion* region, UINT* bufferSize);
GpStatus WINGDIPAPI GdipGetRegionData(GpRegion* region, BYTE* buffer, UINT bufferSize, UINT* sizeFilled);
GpStatus WINGDIPAPI GdipIsVisibleRegionPoint(GpRegion* region, REAL x, REAL y, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisibleRegionPointI(GpRegion* region, INT x, INT y, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisibleRegionRect(GpRegion* region, REAL x, REAL y, REAL width, REAL height, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisibleRegionRectI(GpRegion* region, INT x, INT y, INT width, INT height, GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipGetRegionScansCount(GpRegion* region, UINT* count, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipGetRegionScans(GpRegion* region, GpRectF* rects, INT* count, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipGetRegionScansI(GpRegion* region, GpRect* rects, INT* count, GpMatrix* matrix);
//----------------------------------------------------------------------------
// Brush APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCloneBrush(GpBrush* brush, GpBrush** cloneBrush);
GpStatus WINGDIPAPI GdipDeleteBrush(GpBrush* brush);
GpStatus WINGDIPAPI GdipGetBrushType(GpBrush* brush, GpBrushType* type);
//----------------------------------------------------------------------------
// HatchBrush APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateHatchBrush(GpHatchStyle hatchstyle, ARGB forecol, ARGB backcol, GpHatch** brush);
GpStatus WINGDIPAPI GdipGetHatchStyle(GpHatch* brush, GpHatchStyle* hatchstyle);
GpStatus WINGDIPAPI GdipGetHatchForegroundColor(GpHatch* brush, ARGB* forecol);
GpStatus WINGDIPAPI GdipGetHatchBackgroundColor(GpHatch* brush, ARGB* backcol);
//----------------------------------------------------------------------------
// TextureBrush APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateTexture(GpImage* image, GpWrapMode wrapmode, GpTexture** texture);
GpStatus WINGDIPAPI GdipCreateTexture2(GpImage* image, GpWrapMode wrapmode, REAL x, REAL y, REAL width, REAL height, GpTexture** texture);
GpStatus WINGDIPAPI GdipCreateTextureIA(GpImage* image, GDIPCONST GpImageAttributes* imageAttributes, REAL x, REAL y, REAL width, REAL height, GpTexture** texture);
GpStatus WINGDIPAPI GdipCreateTexture2I(GpImage* image, GpWrapMode wrapmode, INT x, INT y, INT width, INT height, GpTexture** texture);
GpStatus WINGDIPAPI GdipCreateTextureIAI(GpImage* image, GDIPCONST GpImageAttributes* imageAttributes, INT x, INT y, INT width, INT height, GpTexture** texture);
GpStatus WINGDIPAPI GdipGetTextureTransform(GpTexture* brush, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipSetTextureTransform(GpTexture* brush, GDIPCONST GpMatrix* matrix);
GpStatus WINGDIPAPI GdipResetTextureTransform(GpTexture* brush);
GpStatus WINGDIPAPI GdipMultiplyTextureTransform(GpTexture* brush, GDIPCONST GpMatrix* matrix, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipTranslateTextureTransform(GpTexture* brush, REAL dx, REAL dy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipScaleTextureTransform(GpTexture* brush, REAL sx, REAL sy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipRotateTextureTransform(GpTexture* brush, REAL angle, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipSetTextureWrapMode(GpTexture* brush, GpWrapMode wrapmode);
GpStatus WINGDIPAPI GdipGetTextureWrapMode(GpTexture* brush, GpWrapMode* wrapmode);
GpStatus WINGDIPAPI GdipGetTextureImage(GpTexture* brush, GpImage** image);
//----------------------------------------------------------------------------
// SolidBrush APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateSolidFill(ARGB color, GpSolidFill** brush);
GpStatus WINGDIPAPI GdipSetSolidFillColor(GpSolidFill* brush, ARGB color);
GpStatus WINGDIPAPI GdipGetSolidFillColor(GpSolidFill* brush, ARGB* color);
//----------------------------------------------------------------------------
// LineBrush APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateLineBrush(GDIPCONST GpPointF* point1, GDIPCONST GpPointF* point2, ARGB color1, ARGB color2, GpWrapMode wrapMode, GpLineGradient** lineGradient);
GpStatus WINGDIPAPI GdipCreateLineBrushI(GDIPCONST GpPoint* point1, GDIPCONST GpPoint* point2, ARGB color1, ARGB color2, GpWrapMode wrapMode, GpLineGradient** lineGradient);
GpStatus WINGDIPAPI GdipCreateLineBrushFromRect(GDIPCONST GpRectF* rect, ARGB color1, ARGB color2, LinearGradientMode mode, GpWrapMode wrapMode, GpLineGradient** lineGradient);
GpStatus WINGDIPAPI GdipCreateLineBrushFromRectI(GDIPCONST GpRect* rect, ARGB color1, ARGB color2, LinearGradientMode mode, GpWrapMode wrapMode, GpLineGradient** lineGradient);
GpStatus WINGDIPAPI GdipCreateLineBrushFromRectWithAngle(GDIPCONST GpRectF* rect, ARGB color1, ARGB color2, REAL angle, BOOL isAngleScalable, GpWrapMode wrapMode, GpLineGradient** lineGradient);
GpStatus WINGDIPAPI GdipCreateLineBrushFromRectWithAngleI(GDIPCONST GpRect* rect, ARGB color1, ARGB color2, REAL angle, BOOL isAngleScalable, GpWrapMode wrapMode, GpLineGradient** lineGradient);
GpStatus WINGDIPAPI GdipSetLineColors(GpLineGradient* brush, ARGB color1, ARGB color2);
GpStatus WINGDIPAPI GdipGetLineColors(GpLineGradient* brush, ARGB* colors);
GpStatus WINGDIPAPI GdipGetLineRect(GpLineGradient* brush, GpRectF* rect);
GpStatus WINGDIPAPI GdipGetLineRectI(GpLineGradient* brush, GpRect* rect);
GpStatus WINGDIPAPI GdipSetLineGammaCorrection(GpLineGradient* brush, BOOL useGammaCorrection);
GpStatus WINGDIPAPI GdipGetLineGammaCorrection(GpLineGradient* brush, BOOL* useGammaCorrection);
GpStatus WINGDIPAPI GdipGetLineBlendCount(GpLineGradient* brush, INT* count);
GpStatus WINGDIPAPI GdipGetLineBlend(GpLineGradient* brush, REAL* blend, REAL* positions, INT count);
GpStatus WINGDIPAPI GdipSetLineBlend(GpLineGradient* brush, GDIPCONST REAL* blend, GDIPCONST REAL* positions, INT count);
GpStatus WINGDIPAPI GdipGetLinePresetBlendCount(GpLineGradient* brush, INT* count);
GpStatus WINGDIPAPI GdipGetLinePresetBlend(GpLineGradient* brush, ARGB* blend, REAL* positions, INT count);
GpStatus WINGDIPAPI GdipSetLinePresetBlend(GpLineGradient* brush, GDIPCONST ARGB* blend, GDIPCONST REAL* positions, INT count);
GpStatus WINGDIPAPI GdipSetLineSigmaBlend(GpLineGradient* brush, REAL focus, REAL scale);
GpStatus WINGDIPAPI GdipSetLineLinearBlend(GpLineGradient* brush, REAL focus, REAL scale);
GpStatus WINGDIPAPI GdipSetLineWrapMode(GpLineGradient* brush, GpWrapMode wrapmode);
GpStatus WINGDIPAPI GdipGetLineWrapMode(GpLineGradient* brush, GpWrapMode* wrapmode);
GpStatus WINGDIPAPI GdipGetLineTransform(GpLineGradient* brush, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipSetLineTransform(GpLineGradient* brush, GDIPCONST GpMatrix* matrix);
GpStatus WINGDIPAPI GdipResetLineTransform(GpLineGradient* brush);
GpStatus WINGDIPAPI GdipMultiplyLineTransform(GpLineGradient* brush, GDIPCONST GpMatrix* matrix, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipTranslateLineTransform(GpLineGradient* brush, REAL dx, REAL dy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipScaleLineTransform(GpLineGradient* brush, REAL sx, REAL sy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipRotateLineTransform(GpLineGradient* brush, REAL angle, GpMatrixOrder order);
//---------------------------------------------------------------------------- // PathGradientBrush APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreatePathGradient(GDIPCONST GpPointF* points, INT count, GpWrapMode wrapMode, GpPathGradient** polyGradient);
GpStatus WINGDIPAPI GdipCreatePathGradientI(GDIPCONST GpPoint* points, INT count, GpWrapMode wrapMode, GpPathGradient** polyGradient);
GpStatus WINGDIPAPI GdipCreatePathGradientFromPath(GDIPCONST GpPath* path, GpPathGradient** polyGradient);
GpStatus WINGDIPAPI GdipGetPathGradientCenterColor(GpPathGradient* brush, ARGB* colors);
GpStatus WINGDIPAPI GdipSetPathGradientCenterColor(GpPathGradient* brush, ARGB colors);
GpStatus WINGDIPAPI GdipGetPathGradientSurroundColorsWithCount(GpPathGradient* brush, ARGB* color, INT* count);
GpStatus WINGDIPAPI GdipSetPathGradientSurroundColorsWithCount(GpPathGradient* brush, GDIPCONST ARGB* color, INT* count);
GpStatus WINGDIPAPI GdipGetPathGradientPath(GpPathGradient* brush, GpPath* path);
GpStatus WINGDIPAPI GdipSetPathGradientPath(GpPathGradient* brush, GDIPCONST GpPath* path);
GpStatus WINGDIPAPI GdipGetPathGradientCenterPoint(GpPathGradient* brush, GpPointF* points);
GpStatus WINGDIPAPI GdipGetPathGradientCenterPointI(GpPathGradient* brush, GpPoint* points);
GpStatus WINGDIPAPI GdipSetPathGradientCenterPoint(GpPathGradient* brush, GDIPCONST GpPointF* points);
GpStatus WINGDIPAPI GdipSetPathGradientCenterPointI(GpPathGradient* brush, GDIPCONST GpPoint* points);
GpStatus WINGDIPAPI GdipGetPathGradientRect(GpPathGradient* brush, GpRectF* rect);
GpStatus WINGDIPAPI GdipGetPathGradientRectI(GpPathGradient* brush, GpRect* rect);
GpStatus WINGDIPAPI GdipGetPathGradientPointCount(GpPathGradient* brush, INT* count);
GpStatus WINGDIPAPI GdipGetPathGradientSurroundColorCount(GpPathGradient* brush, INT* count);
GpStatus WINGDIPAPI GdipSetPathGradientGammaCorrection(GpPathGradient* brush, BOOL useGammaCorrection);
GpStatus WINGDIPAPI GdipGetPathGradientGammaCorrection(GpPathGradient* brush, BOOL* useGammaCorrection);
GpStatus WINGDIPAPI GdipGetPathGradientBlendCount(GpPathGradient* brush, INT* count);
GpStatus WINGDIPAPI GdipGetPathGradientBlend(GpPathGradient* brush, REAL* blend, REAL* positions, INT count);
GpStatus WINGDIPAPI GdipSetPathGradientBlend(GpPathGradient* brush, GDIPCONST REAL* blend, GDIPCONST REAL* positions, INT count);
GpStatus WINGDIPAPI GdipGetPathGradientPresetBlendCount(GpPathGradient* brush, INT* count);
GpStatus WINGDIPAPI GdipGetPathGradientPresetBlend(GpPathGradient* brush, ARGB* blend, REAL* positions, INT count);
GpStatus WINGDIPAPI GdipSetPathGradientPresetBlend(GpPathGradient* brush, GDIPCONST ARGB* blend, GDIPCONST REAL* positions, INT count);
GpStatus WINGDIPAPI GdipSetPathGradientSigmaBlend(GpPathGradient* brush, REAL focus, REAL scale);
GpStatus WINGDIPAPI GdipSetPathGradientLinearBlend(GpPathGradient* brush, REAL focus, REAL scale);
GpStatus WINGDIPAPI GdipGetPathGradientWrapMode(GpPathGradient* brush, GpWrapMode* wrapmode);
GpStatus WINGDIPAPI GdipSetPathGradientWrapMode(GpPathGradient* brush, GpWrapMode wrapmode);
GpStatus WINGDIPAPI GdipGetPathGradientTransform(GpPathGradient* brush, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipSetPathGradientTransform(GpPathGradient* brush, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipResetPathGradientTransform(GpPathGradient* brush);
GpStatus WINGDIPAPI GdipMultiplyPathGradientTransform(GpPathGradient* brush, GDIPCONST GpMatrix* matrix, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipTranslatePathGradientTransform(GpPathGradient* brush, REAL dx, REAL dy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipScalePathGradientTransform(GpPathGradient* brush, REAL sx, REAL sy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipRotatePathGradientTransform(GpPathGradient* brush, REAL angle, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipGetPathGradientFocusScales(GpPathGradient* brush, REAL* xScale, REAL* yScale);
GpStatus WINGDIPAPI GdipSetPathGradientFocusScales(GpPathGradient* brush, REAL xScale, REAL yScale);
//----------------------------------------------------------------------------
// Pen APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreatePen1(ARGB color, REAL width, GpUnit unit, GpPen** pen);
GpStatus WINGDIPAPI GdipCreatePen2(GpBrush* brush, REAL width, GpUnit unit, GpPen** pen);
GpStatus WINGDIPAPI GdipClonePen(GpPen* pen, GpPen** clonepen);
GpStatus WINGDIPAPI GdipDeletePen(GpPen* pen);
GpStatus WINGDIPAPI GdipSetPenWidth(GpPen* pen, REAL width);
GpStatus WINGDIPAPI GdipGetPenWidth(GpPen* pen, REAL* width);
GpStatus WINGDIPAPI GdipSetPenUnit(GpPen* pen, GpUnit unit);
GpStatus WINGDIPAPI GdipGetPenUnit(GpPen* pen, GpUnit* unit);
GpStatus WINGDIPAPI GdipSetPenLineCap197819(GpPen* pen, GpLineCap startCap, GpLineCap endCap, GpDashCap dashCap);
GpStatus WINGDIPAPI GdipSetPenStartCap(GpPen* pen, GpLineCap startCap);
GpStatus WINGDIPAPI GdipSetPenEndCap(GpPen* pen, GpLineCap endCap);
GpStatus WINGDIPAPI GdipSetPenDashCap197819(GpPen* pen, GpDashCap dashCap);
GpStatus WINGDIPAPI GdipGetPenStartCap(GpPen* pen, GpLineCap* startCap);
GpStatus WINGDIPAPI GdipGetPenEndCap(GpPen* pen, GpLineCap* endCap);
GpStatus WINGDIPAPI GdipGetPenDashCap197819(GpPen* pen, GpDashCap* dashCap);
GpStatus WINGDIPAPI GdipSetPenLineJoin(GpPen* pen, GpLineJoin lineJoin);
GpStatus WINGDIPAPI GdipGetPenLineJoin(GpPen* pen, GpLineJoin* lineJoin);
GpStatus WINGDIPAPI GdipSetPenCustomStartCap(GpPen* pen, GpCustomLineCap* customCap);
GpStatus WINGDIPAPI GdipGetPenCustomStartCap(GpPen* pen, GpCustomLineCap** customCap);
GpStatus WINGDIPAPI GdipSetPenCustomEndCap(GpPen* pen, GpCustomLineCap* customCap);
GpStatus WINGDIPAPI GdipGetPenCustomEndCap(GpPen* pen, GpCustomLineCap** customCap);
GpStatus WINGDIPAPI GdipSetPenMiterLimit(GpPen* pen, REAL miterLimit);
GpStatus WINGDIPAPI GdipGetPenMiterLimit(GpPen* pen, REAL* miterLimit);
GpStatus WINGDIPAPI GdipSetPenMode(GpPen* pen, GpPenAlignment penMode);
GpStatus WINGDIPAPI GdipGetPenMode(GpPen* pen, GpPenAlignment* penMode);
GpStatus WINGDIPAPI GdipSetPenTransform(GpPen* pen, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipGetPenTransform(GpPen* pen, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipResetPenTransform(GpPen* pen);
GpStatus WINGDIPAPI GdipMultiplyPenTransform(GpPen* pen, GDIPCONST GpMatrix* matrix, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipTranslatePenTransform(GpPen* pen, REAL dx, REAL dy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipScalePenTransform(GpPen* pen, REAL sx, REAL sy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipRotatePenTransform(GpPen* pen, REAL angle, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipSetPenColor(GpPen* pen, ARGB argb);
GpStatus WINGDIPAPI GdipGetPenColor(GpPen* pen, ARGB* argb);
GpStatus WINGDIPAPI GdipSetPenBrushFill(GpPen* pen, GpBrush* brush);
GpStatus WINGDIPAPI GdipGetPenBrushFill(GpPen* pen, GpBrush** brush);
GpStatus WINGDIPAPI GdipGetPenFillType(GpPen* pen, GpPenType* type);
GpStatus WINGDIPAPI GdipGetPenDashStyle(GpPen* pen, GpDashStyle* dashstyle);
GpStatus WINGDIPAPI GdipSetPenDashStyle(GpPen* pen, GpDashStyle dashstyle);
GpStatus WINGDIPAPI GdipGetPenDashOffset(GpPen* pen, REAL* offset);
GpStatus WINGDIPAPI GdipSetPenDashOffset(GpPen* pen, REAL offset);
GpStatus WINGDIPAPI GdipGetPenDashCount(GpPen* pen, INT* count);
GpStatus WINGDIPAPI GdipSetPenDashArray(GpPen* pen, GDIPCONST REAL* dash, INT count);
GpStatus WINGDIPAPI GdipGetPenDashArray(GpPen* pen, REAL* dash, INT count);
GpStatus WINGDIPAPI GdipGetPenCompoundCount(GpPen* pen, INT* count);
GpStatus WINGDIPAPI GdipSetPenCompoundArray(GpPen* pen, GDIPCONST REAL* dash, INT count);
GpStatus WINGDIPAPI GdipGetPenCompoundArray(GpPen* pen, REAL* dash, INT count);
//----------------------------------------------------------------------------
// CustomLineCap APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateCustomLineCap(GpPath* fillPath, GpPath* strokePath, GpLineCap baseCap, REAL baseInset, GpCustomLineCap** customCap);
GpStatus WINGDIPAPI GdipDeleteCustomLineCap(GpCustomLineCap* customCap);
GpStatus WINGDIPAPI GdipCloneCustomLineCap(GpCustomLineCap* customCap, GpCustomLineCap** clonedCap);
GpStatus WINGDIPAPI GdipGetCustomLineCapType(GpCustomLineCap* customCap, CustomLineCapType* capType);
GpStatus WINGDIPAPI GdipSetCustomLineCapStrokeCaps(GpCustomLineCap* customCap, GpLineCap startCap, GpLineCap endCap);
GpStatus WINGDIPAPI GdipGetCustomLineCapStrokeCaps(GpCustomLineCap* customCap, GpLineCap* startCap, GpLineCap* endCap);
GpStatus WINGDIPAPI GdipSetCustomLineCapStrokeJoin(GpCustomLineCap* customCap, GpLineJoin lineJoin);
GpStatus WINGDIPAPI GdipGetCustomLineCapStrokeJoin(GpCustomLineCap* customCap, GpLineJoin* lineJoin);
GpStatus WINGDIPAPI GdipSetCustomLineCapBaseCap(GpCustomLineCap* customCap, GpLineCap baseCap);
GpStatus WINGDIPAPI GdipGetCustomLineCapBaseCap(GpCustomLineCap* customCap, GpLineCap* baseCap);
GpStatus WINGDIPAPI GdipSetCustomLineCapBaseInset(GpCustomLineCap* customCap, REAL inset);
GpStatus WINGDIPAPI GdipGetCustomLineCapBaseInset(GpCustomLineCap* customCap, REAL* inset);
GpStatus WINGDIPAPI GdipSetCustomLineCapWidthScale(GpCustomLineCap* customCap, REAL widthScale);
GpStatus WINGDIPAPI GdipGetCustomLineCapWidthScale(GpCustomLineCap* customCap, REAL* widthScale);
//----------------------------------------------------------------------------
// AdjustableArrowCap APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateAdjustableArrowCap(REAL height, REAL width, BOOL isFilled, GpAdjustableArrowCap** cap);
GpStatus WINGDIPAPI GdipSetAdjustableArrowCapHeight(GpAdjustableArrowCap* cap, REAL height);
GpStatus WINGDIPAPI GdipGetAdjustableArrowCapHeight(GpAdjustableArrowCap* cap, REAL* height);
GpStatus WINGDIPAPI GdipSetAdjustableArrowCapWidth(GpAdjustableArrowCap* cap, REAL width);
GpStatus WINGDIPAPI GdipGetAdjustableArrowCapWidth(GpAdjustableArrowCap* cap, REAL* width);
GpStatus WINGDIPAPI GdipSetAdjustableArrowCapMiddleInset(GpAdjustableArrowCap* cap, REAL middleInset);
GpStatus WINGDIPAPI GdipGetAdjustableArrowCapMiddleInset(GpAdjustableArrowCap* cap, REAL* middleInset);
GpStatus WINGDIPAPI GdipSetAdjustableArrowCapFillState(GpAdjustableArrowCap* cap, BOOL fillState);
GpStatus WINGDIPAPI GdipGetAdjustableArrowCapFillState(GpAdjustableArrowCap* cap, BOOL* fillState);
//----------------------------------------------------------------------------
// Image APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipLoadImageFromStream(IStream* stream, GpImage** image);
GpStatus WINGDIPAPI GdipLoadImageFromFile(GDIPCONST WCHAR* filename, GpImage** image);
GpStatus WINGDIPAPI GdipLoadImageFromStreamICM(IStream* stream, GpImage** image);
GpStatus WINGDIPAPI GdipLoadImageFromFileICM(GDIPCONST WCHAR* filename, GpImage** image);
GpStatus WINGDIPAPI GdipCloneImage(GpImage* image, GpImage** cloneImage);
GpStatus WINGDIPAPI GdipDisposeImage(GpImage* image);
GpStatus WINGDIPAPI GdipSaveImageToFile(GpImage* image, GDIPCONST WCHAR* filename, GDIPCONST CLSID* clsidEncoder, GDIPCONST EncoderParameters* encoderParams);
GpStatus WINGDIPAPI GdipSaveImageToStream(GpImage* image, IStream* stream, GDIPCONST CLSID* clsidEncoder, GDIPCONST EncoderParameters* encoderParams);
GpStatus WINGDIPAPI GdipSaveAdd(GpImage* image, GDIPCONST EncoderParameters* encoderParams);
GpStatus WINGDIPAPI GdipSaveAddImage(GpImage* image, GpImage* newImage, GDIPCONST EncoderParameters* encoderParams);
GpStatus WINGDIPAPI GdipGetImageGraphicsContext(GpImage* image, GpGraphics** graphics);
GpStatus WINGDIPAPI GdipGetImageBounds(GpImage* image, GpRectF* srcRect, GpUnit* srcUnit);
GpStatus WINGDIPAPI GdipGetImageDimension(GpImage* image, REAL* width, REAL* height);
GpStatus WINGDIPAPI GdipGetImageType(GpImage* image, ImageType* type);
GpStatus WINGDIPAPI GdipGetImageWidth(GpImage* image, UINT* width);
GpStatus WINGDIPAPI GdipGetImageHeight(GpImage* image, UINT* height);
GpStatus WINGDIPAPI GdipGetImageHorizontalResolution(GpImage* image, REAL* resolution);
GpStatus WINGDIPAPI GdipGetImageVerticalResolution(GpImage* image, REAL* resolution);
GpStatus WINGDIPAPI GdipGetImageFlags(GpImage* image, UINT* flags);
GpStatus WINGDIPAPI GdipGetImageRawFormat(GpImage* image, GUID* format);
GpStatus WINGDIPAPI GdipGetImagePixelFormat(GpImage* image, PixelFormat* format);
GpStatus WINGDIPAPI GdipGetImageThumbnail(GpImage* image, UINT thumbWidth, UINT thumbHeight, GpImage** thumbImage, GetThumbnailImageAbort callback, VOID* callbackData);
GpStatus WINGDIPAPI GdipGetEncoderParameterListSize(GpImage* image, GDIPCONST CLSID* clsidEncoder, UINT* size);
GpStatus WINGDIPAPI GdipGetEncoderParameterList(GpImage* image, GDIPCONST CLSID* clsidEncoder, UINT size, EncoderParameters* buffer);
GpStatus WINGDIPAPI GdipImageGetFrameDimensionsCount(GpImage* image, UINT* count);
GpStatus WINGDIPAPI GdipImageGetFrameDimensionsList(GpImage* image, GUID* dimensionIDs, UINT count);
GpStatus WINGDIPAPI GdipImageGetFrameCount(GpImage* image, GDIPCONST GUID* dimensionID, UINT* count);
GpStatus WINGDIPAPI GdipImageSelectActiveFrame(GpImage* image, GDIPCONST GUID* dimensionID, UINT frameIndex);
GpStatus WINGDIPAPI GdipImageRotateFlip(GpImage* image, RotateFlipType rfType);
GpStatus WINGDIPAPI GdipGetImagePalette(GpImage* image, ColorPalette* palette, INT size);
GpStatus WINGDIPAPI GdipSetImagePalette(GpImage* image, GDIPCONST ColorPalette* palette);
GpStatus WINGDIPAPI GdipGetImagePaletteSize(GpImage* image, INT* size);
GpStatus WINGDIPAPI GdipGetPropertyCount(GpImage* image, UINT* numOfProperty);
GpStatus WINGDIPAPI GdipGetPropertyIdList(GpImage* image, UINT numOfProperty, PROPID* list);
GpStatus WINGDIPAPI GdipGetPropertyItemSize(GpImage* image, PROPID propId, UINT* size);
GpStatus WINGDIPAPI GdipGetPropertyItem(GpImage* image, PROPID propId, UINT propSize, PropertyItem* buffer);
GpStatus WINGDIPAPI GdipGetPropertySize(GpImage* image, UINT* totalBufferSize, UINT* numProperties);
GpStatus WINGDIPAPI GdipGetAllPropertyItems(GpImage* image, UINT totalBufferSize, UINT numProperties, PropertyItem* allItems);
GpStatus WINGDIPAPI GdipRemovePropertyItem(GpImage* image, PROPID propId);
GpStatus WINGDIPAPI GdipSetPropertyItem(GpImage* image, GDIPCONST PropertyItem* item);
GpStatus WINGDIPAPI GdipImageForceValidation(GpImage* image);
//----------------------------------------------------------------------------
// Bitmap APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateBitmapFromStream(IStream* stream, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromFile(GDIPCONST WCHAR* filename, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromStreamICM(IStream* stream, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromFileICM(GDIPCONST WCHAR* filename, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromScan0(INT width, INT height, INT stride, PixelFormat format, BYTE* scan0, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromGraphics(INT width, INT height, GpGraphics* target, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromDirectDrawSurface(IDirectDrawSurface7* surface, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromGdiDib(GDIPCONST BITMAPINFO* gdiBitmapInfo, VOID* gdiBitmapData, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateBitmapFromHBITMAP(HBITMAP hbm, HPALETTE hpal, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateHBITMAPFromBitmap(GpBitmap* bitmap, HBITMAP* hbmReturn, ARGB background);
GpStatus WINGDIPAPI GdipCreateBitmapFromHICON(HICON hicon, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCreateHICONFromBitmap(GpBitmap* bitmap, HICON* hbmReturn);
GpStatus WINGDIPAPI GdipCreateBitmapFromResource(HINSTANCE hInstance, GDIPCONST WCHAR* lpBitmapName, GpBitmap** bitmap);
GpStatus WINGDIPAPI GdipCloneBitmapArea(REAL x, REAL y, REAL width, REAL height, PixelFormat format, GpBitmap* srcBitmap, GpBitmap** dstBitmap);
GpStatus WINGDIPAPI GdipCloneBitmapAreaI(INT x, INT y, INT width, INT height, PixelFormat format, GpBitmap* srcBitmap, GpBitmap** dstBitmap);
GpStatus WINGDIPAPI GdipBitmapLockBits(GpBitmap* bitmap, GDIPCONST GpRect* rect, UINT flags, PixelFormat format, BitmapData* lockedBitmapData);
GpStatus WINGDIPAPI GdipBitmapUnlockBits(GpBitmap* bitmap, BitmapData* lockedBitmapData);
GpStatus WINGDIPAPI GdipBitmapGetPixel(GpBitmap* bitmap, INT x, INT y, ARGB* color);
GpStatus WINGDIPAPI GdipBitmapSetPixel(GpBitmap* bitmap, INT x, INT y, ARGB color);
GpStatus WINGDIPAPI GdipBitmapSetResolution(GpBitmap* bitmap, REAL xdpi, REAL ydpi);
//----------------------------------------------------------------------------
// ImageAttributes APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateImageAttributes(GpImageAttributes** imageattr);
GpStatus WINGDIPAPI GdipCloneImageAttributes(GDIPCONST GpImageAttributes* imageattr, GpImageAttributes** cloneImageattr);
GpStatus WINGDIPAPI GdipDisposeImageAttributes(GpImageAttributes* imageattr);
GpStatus WINGDIPAPI GdipSetImageAttributesToIdentity(GpImageAttributes* imageattr, ColorAdjustType type);
GpStatus WINGDIPAPI GdipResetImageAttributes(GpImageAttributes* imageattr, ColorAdjustType type);
GpStatus WINGDIPAPI GdipSetImageAttributesColorMatrix(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, GDIPCONST ColorMatrix* colorMatrix, GDIPCONST ColorMatrix* grayMatrix, ColorMatrixFlags flags);
GpStatus WINGDIPAPI GdipSetImageAttributesThreshold(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, REAL threshold);
GpStatus WINGDIPAPI GdipSetImageAttributesGamma(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, REAL gamma);
GpStatus WINGDIPAPI GdipSetImageAttributesNoOp(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag);
GpStatus WINGDIPAPI GdipSetImageAttributesColorKeys(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, ARGB colorLow, ARGB colorHigh);
GpStatus WINGDIPAPI GdipSetImageAttributesOutputChannel(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, ColorChannelFlags channelFlags);
GpStatus WINGDIPAPI GdipSetImageAttributesOutputChannelColorProfile(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, GDIPCONST WCHAR* colorProfileFilename);
GpStatus WINGDIPAPI GdipSetImageAttributesRemapTable(GpImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, UINT mapSize, GDIPCONST ColorMap* map);
GpStatus WINGDIPAPI GdipSetImageAttributesWrapMode(GpImageAttributes* imageAttr, WrapMode wrap, ARGB argb, BOOL clamp);
GpStatus WINGDIPAPI GdipSetImageAttributesICMMode(GpImageAttributes* imageAttr, BOOL on);
GpStatus WINGDIPAPI GdipGetImageAttributesAdjustedPalette(GpImageAttributes* imageAttr, ColorPalette* colorPalette, ColorAdjustType colorAdjustType);
//----------------------------------------------------------------------------
// Graphics APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipFlush(GpGraphics* graphics, GpFlushIntention intention);
GpStatus WINGDIPAPI GdipCreateFromHDC(HDC hdc, GpGraphics** graphics);
GpStatus WINGDIPAPI GdipCreateFromHDC2(HDC hdc, HANDLE hDevice, GpGraphics** graphics);
GpStatus WINGDIPAPI GdipCreateFromHWND(HWND hwnd, GpGraphics** graphics);
GpStatus WINGDIPAPI GdipCreateFromHWNDICM(HWND hwnd, GpGraphics** graphics);
GpStatus WINGDIPAPI GdipDeleteGraphics(GpGraphics* graphics);
GpStatus WINGDIPAPI GdipGetDC(GpGraphics* graphics, HDC* hdc);
GpStatus WINGDIPAPI GdipReleaseDC(GpGraphics* graphics, HDC hdc);
GpStatus WINGDIPAPI GdipSetCompositingMode(GpGraphics* graphics, CompositingMode compositingMode);
GpStatus WINGDIPAPI GdipGetCompositingMode(GpGraphics* graphics, CompositingMode* compositingMode);
GpStatus WINGDIPAPI GdipSetRenderingOrigin(GpGraphics* graphics, INT x, INT y);
GpStatus WINGDIPAPI GdipGetRenderingOrigin(GpGraphics* graphics, INT* x, INT* y);
GpStatus WINGDIPAPI GdipSetCompositingQuality(GpGraphics* graphics, CompositingQuality compositingQuality);
GpStatus WINGDIPAPI GdipGetCompositingQuality(GpGraphics* graphics, CompositingQuality* compositingQuality);
GpStatus WINGDIPAPI GdipSetSmoothingMode(GpGraphics* graphics, SmoothingMode smoothingMode);
GpStatus WINGDIPAPI GdipGetSmoothingMode(GpGraphics* graphics, SmoothingMode* smoothingMode);
GpStatus WINGDIPAPI GdipSetPixelOffsetMode(GpGraphics* graphics, PixelOffsetMode pixelOffsetMode);
GpStatus WINGDIPAPI GdipGetPixelOffsetMode(GpGraphics* graphics, PixelOffsetMode* pixelOffsetMode);
GpStatus WINGDIPAPI GdipSetTextRenderingHint(GpGraphics* graphics, TextRenderingHint mode);
GpStatus WINGDIPAPI GdipGetTextRenderingHint(GpGraphics* graphics, TextRenderingHint* mode);
GpStatus WINGDIPAPI GdipSetTextContrast(GpGraphics* graphics, UINT contrast);
GpStatus WINGDIPAPI GdipGetTextContrast(GpGraphics* graphics, UINT* contrast);
GpStatus WINGDIPAPI GdipSetInterpolationMode(GpGraphics* graphics, InterpolationMode interpolationMode);
GpStatus WINGDIPAPI GdipGetInterpolationMode(GpGraphics* graphics, InterpolationMode* interpolationMode);
GpStatus WINGDIPAPI GdipSetWorldTransform(GpGraphics* graphics, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipResetWorldTransform(GpGraphics* graphics);
GpStatus WINGDIPAPI GdipMultiplyWorldTransform(GpGraphics* graphics, GDIPCONST GpMatrix* matrix, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipTranslateWorldTransform(GpGraphics* graphics, REAL dx, REAL dy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipScaleWorldTransform(GpGraphics* graphics, REAL sx, REAL sy, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipRotateWorldTransform(GpGraphics* graphics, REAL angle, GpMatrixOrder order);
GpStatus WINGDIPAPI GdipGetWorldTransform(GpGraphics* graphics, GpMatrix* matrix);
GpStatus WINGDIPAPI GdipResetPageTransform(GpGraphics* graphics);
GpStatus WINGDIPAPI GdipGetPageUnit(GpGraphics* graphics, GpUnit* unit);
GpStatus WINGDIPAPI GdipGetPageScale(GpGraphics* graphics, REAL* scale);
GpStatus WINGDIPAPI GdipSetPageUnit(GpGraphics* graphics, GpUnit unit);
GpStatus WINGDIPAPI GdipSetPageScale(GpGraphics* graphics, REAL scale);
GpStatus WINGDIPAPI GdipGetDpiX(GpGraphics* graphics, REAL* dpi);
GpStatus WINGDIPAPI GdipGetDpiY(GpGraphics* graphics, REAL* dpi);
GpStatus WINGDIPAPI GdipTransformPoints(GpGraphics* graphics, GpCoordinateSpace destSpace, GpCoordinateSpace srcSpace, GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipTransformPointsI(GpGraphics* graphics, GpCoordinateSpace destSpace, GpCoordinateSpace srcSpace, GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipGetNearestColor(GpGraphics* graphics, ARGB* argb);
// Creates the Win9x Halftone Palette (even on NT) with correct Desktop colors
HPALETTE WINGDIPAPI GdipCreateHalftonePalette();
GpStatus WINGDIPAPI GdipDrawLine(GpGraphics* graphics, GpPen* pen, REAL x1, REAL y1, REAL x2, REAL y2);
GpStatus WINGDIPAPI GdipDrawLineI(GpGraphics* graphics, GpPen* pen, INT x1, INT y1, INT x2, INT y2);
GpStatus WINGDIPAPI GdipDrawLines(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipDrawLinesI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipDrawArc(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipDrawArcI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipDrawBezier(GpGraphics* graphics, GpPen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
GpStatus WINGDIPAPI GdipDrawBezierI(GpGraphics* graphics, GpPen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4);
GpStatus WINGDIPAPI GdipDrawBeziers(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipDrawBeziersI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipDrawRectangle(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipDrawRectangleI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipDrawRectangles(GpGraphics* graphics, GpPen* pen, GDIPCONST GpRectF* rects, INT count);
GpStatus WINGDIPAPI GdipDrawRectanglesI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpRect* rects, INT count);
GpStatus WINGDIPAPI GdipDrawEllipse(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipDrawEllipseI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipDrawPie(GpGraphics* graphics, GpPen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipDrawPieI(GpGraphics* graphics, GpPen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipDrawPolygon(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipDrawPolygonI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipDrawPath(GpGraphics* graphics, GpPen* pen, GpPath* path);
GpStatus WINGDIPAPI GdipDrawCurve(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipDrawCurveI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipDrawCurve2(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipDrawCurve2I(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipDrawCurve3(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count, INT offset, INT numberOfSegments, REAL tension);
GpStatus WINGDIPAPI GdipDrawCurve3I(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count, INT offset, INT numberOfSegments, REAL tension);
GpStatus WINGDIPAPI GdipDrawClosedCurve(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipDrawClosedCurveI(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipDrawClosedCurve2(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPointF* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipDrawClosedCurve2I(GpGraphics* graphics, GpPen* pen, GDIPCONST GpPoint* points, INT count, REAL tension);
GpStatus WINGDIPAPI GdipGraphicsClear(GpGraphics* graphics, ARGB color);
GpStatus WINGDIPAPI GdipFillRectangle(GpGraphics* graphics, GpBrush* brush, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipFillRectangleI(GpGraphics* graphics, GpBrush* brush, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipFillRectangles(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpRectF* rects, INT count);
GpStatus WINGDIPAPI GdipFillRectanglesI(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpRect* rects, INT count);
GpStatus WINGDIPAPI GdipFillPolygon(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count, GpFillMode fillMode);
GpStatus WINGDIPAPI GdipFillPolygonI(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count, GpFillMode fillMode);
GpStatus WINGDIPAPI GdipFillPolygon2(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipFillPolygon2I(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipFillEllipse(GpGraphics* graphics, GpBrush* brush, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipFillEllipseI(GpGraphics* graphics, GpBrush* brush, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipFillPie(GpGraphics* graphics, GpBrush* brush, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipFillPieI(GpGraphics* graphics, GpBrush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
GpStatus WINGDIPAPI GdipFillPath(GpGraphics* graphics, GpBrush* brush, GpPath* path);
GpStatus WINGDIPAPI GdipFillClosedCurve(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count);
GpStatus WINGDIPAPI GdipFillClosedCurveI(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count);
GpStatus WINGDIPAPI GdipFillClosedCurve2(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPointF* points, INT count, REAL tension, GpFillMode fillMode);
GpStatus WINGDIPAPI GdipFillClosedCurve2I(GpGraphics* graphics, GpBrush* brush, GDIPCONST GpPoint* points, INT count, REAL tension, GpFillMode fillMode);
GpStatus WINGDIPAPI GdipFillRegion(GpGraphics* graphics, GpBrush* brush, GpRegion* region);
GpStatus WINGDIPAPI GdipDrawImage(GpGraphics* graphics, GpImage* image, REAL x, REAL y);
GpStatus WINGDIPAPI GdipDrawImageI(GpGraphics* graphics, GpImage* image, INT x, INT y);
GpStatus WINGDIPAPI GdipDrawImageRect(GpGraphics* graphics, GpImage* image, REAL x, REAL y, REAL width, REAL height);
GpStatus WINGDIPAPI GdipDrawImageRectI(GpGraphics* graphics, GpImage* image, INT x, INT y, INT width, INT height);
GpStatus WINGDIPAPI GdipDrawImagePoints(GpGraphics* graphics, GpImage* image, GDIPCONST GpPointF* dstpoints, INT count);
GpStatus WINGDIPAPI GdipDrawImagePointsI(GpGraphics* graphics, GpImage* image, GDIPCONST GpPoint* dstpoints, INT count);
GpStatus WINGDIPAPI GdipDrawImagePointRect(GpGraphics* graphics, GpImage* image, REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, GpUnit srcUnit);
GpStatus WINGDIPAPI GdipDrawImagePointRectI(GpGraphics* graphics, GpImage* image, INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight, GpUnit srcUnit);
GpStatus WINGDIPAPI GdipDrawImageRectRect(GpGraphics* graphics, GpImage* image, REAL dstx, REAL dsty, REAL dstwidth, REAL dstheight, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
GpStatus WINGDIPAPI GdipDrawImageRectRectI(GpGraphics* graphics, GpImage* image, INT dstx, INT dsty, INT dstwidth, INT dstheight, INT srcx, INT srcy, INT srcwidth, INT srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
GpStatus WINGDIPAPI GdipDrawImagePointsRect(GpGraphics* graphics, GpImage* image, GDIPCONST GpPointF* points, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
GpStatus WINGDIPAPI GdipDrawImagePointsRectI(GpGraphics* graphics, GpImage* image, GDIPCONST GpPoint* points, INT count, INT srcx, INT srcy, INT srcwidth, INT srcheight, GpUnit srcUnit, GDIPCONST GpImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
#if 0
GpStatus WINGDIPAPI GdipEnumerateMetafileDestPoint(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST PointF& destPoint, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileDestPointI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST Point& destPoint, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileDestRect(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST RectF& destRect, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileDestRectI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST Rect& destRect, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileDestPoints(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST PointF* destPoints, INT count, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileDestPointsI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST Point* destPoints, INT count, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileSrcRectDestPoint(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST PointF& destPoint, GDIPCONST RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileSrcRectDestPointI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST Point& destPoint, GDIPCONST Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileSrcRectDestRect(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST RectF& destRect, GDIPCONST RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileSrcRectDestRectI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST Rect& destRect, GDIPCONST Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileSrcRectDestPoints(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST PointF* destPoints, INT count, GDIPCONST RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
GpStatus WINGDIPAPI GdipEnumerateMetafileSrcRectDestPointsI(GpGraphics* graphics, GDIPCONST GpMetafile* metafile, GDIPCONST Point* destPoints, INT count, GDIPCONST Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST GpImageAttributes* imageAttributes);
#endif
GpStatus WINGDIPAPI GdipPlayMetafileRecord(GDIPCONST GpMetafile* metafile, EmfPlusRecordType recordType, UINT flags, UINT dataSize, GDIPCONST BYTE* data);
GpStatus WINGDIPAPI GdipSetClipGraphics(GpGraphics* graphics, GpGraphics* srcgraphics, CombineMode combineMode);
GpStatus WINGDIPAPI GdipSetClipRect(GpGraphics* graphics, REAL x, REAL y, REAL width, REAL height, CombineMode combineMode);
GpStatus WINGDIPAPI GdipSetClipRectI(GpGraphics* graphics, INT x, INT y, INT width, INT height, CombineMode combineMode);
GpStatus WINGDIPAPI GdipSetClipPath(GpGraphics* graphics, GpPath* path, CombineMode combineMode);
GpStatus WINGDIPAPI GdipSetClipRegion(GpGraphics* graphics, GpRegion* region, CombineMode combineMode);
GpStatus WINGDIPAPI GdipSetClipHrgn(GpGraphics* graphics, HRGN hRgn, CombineMode combineMode);
GpStatus WINGDIPAPI GdipResetClip(GpGraphics* graphics);
GpStatus WINGDIPAPI GdipTranslateClip(GpGraphics* graphics, REAL dx, REAL dy);
GpStatus WINGDIPAPI GdipTranslateClipI(GpGraphics* graphics, INT dx, INT dy);
GpStatus WINGDIPAPI GdipGetClip(GpGraphics* graphics, GpRegion* region);
GpStatus WINGDIPAPI GdipGetClipBounds(GpGraphics* graphics, GpRectF* rect);
GpStatus WINGDIPAPI GdipGetClipBoundsI(GpGraphics* graphics, GpRect* rect);
GpStatus WINGDIPAPI GdipIsClipEmpty(GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipGetVisibleClipBounds(GpGraphics* graphics, GpRectF* rect);
GpStatus WINGDIPAPI GdipGetVisibleClipBoundsI(GpGraphics* graphics, GpRect* rect);
GpStatus WINGDIPAPI GdipIsVisibleClipEmpty(GpGraphics* graphics, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisiblePoint(GpGraphics* graphics, REAL x, REAL y, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisiblePointI(GpGraphics* graphics, INT x, INT y, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisibleRect(GpGraphics* graphics, REAL x, REAL y, REAL width, REAL height, BOOL* result);
GpStatus WINGDIPAPI GdipIsVisibleRectI(GpGraphics* graphics, INT x, INT y, INT width, INT height, BOOL* result);
GpStatus WINGDIPAPI GdipSaveGraphics(GpGraphics* graphics, GraphicsState* state);
GpStatus WINGDIPAPI GdipRestoreGraphics(GpGraphics* graphics, GraphicsState state);
GpStatus WINGDIPAPI GdipBeginContainer(GpGraphics* graphics, GDIPCONST GpRectF* dstrect, GDIPCONST GpRectF* srcrect, GpUnit unit, GraphicsContainer* state);
GpStatus WINGDIPAPI GdipBeginContainerI(GpGraphics* graphics, GDIPCONST GpRect* dstrect, GDIPCONST GpRect* srcrect, GpUnit unit, GraphicsContainer* state);
GpStatus WINGDIPAPI GdipBeginContainer2(GpGraphics* graphics, GraphicsContainer* state);
GpStatus WINGDIPAPI GdipEndContainer(GpGraphics* graphics, GraphicsContainer state);
#if 0
GpStatus GdipGetMetafileHeaderFromWmf(HMETAFILE hWmf, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, MetafileHeader* header);
GpStatus WINGDIPAPI GdipGetMetafileHeaderFromEmf(HENHMETAFILE hEmf, MetafileHeader* header);
GpStatus WINGDIPAPI GdipGetMetafileHeaderFromFile(GDIPCONST WCHAR* filename, MetafileHeader* header);
GpStatus WINGDIPAPI GdipGetMetafileHeaderFromStream(IStream* stream, MetafileHeader* header);
GpStatus WINGDIPAPI GdipGetMetafileHeaderFromMetafile(GpMetafile* metafile, MetafileHeader* header);
GpStatus WINGDIPAPI GdipGetHemfFromMetafile(GpMetafile* metafile, HENHMETAFILE* hEmf);
GpStatus WINGDIPAPI GdipCreateStreamOnFile(GDIPCONST WCHAR* filename, UINT access, IStream** stream);
GpStatus WINGDIPAPI GdipCreateMetafileFromWmf(HMETAFILE hWmf, BOOL deleteWmf, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipCreateMetafileFromEmf(HENHMETAFILE hEmf, BOOL deleteEmf, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipCreateMetafileFromFile(GDIPCONST WCHAR* file, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipCreateMetafileFromWmfFile(GDIPCONST WCHAR* file, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipCreateMetafileFromStream(IStream* stream, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipRecordMetafile(HDC referenceHdc, EmfType type, GDIPCONST GpRectF* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipRecordMetafileI(HDC referenceHdc, EmfType type, GDIPCONST GpRect* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipRecordMetafileFileName(GDIPCONST WCHAR* fileName, HDC referenceHdc, EmfType type, GDIPCONST GpRectF* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipRecordMetafileFileNameI(GDIPCONST WCHAR* fileName, HDC referenceHdc, EmfType type, GDIPCONST GpRect* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipRecordMetafileStream(IStream* stream, HDC referenceHdc, EmfType type, GDIPCONST GpRectF* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipRecordMetafileStreamI(IStream* stream, HDC referenceHdc, EmfType type, GDIPCONST GpRect* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, GpMetafile** metafile);
GpStatus WINGDIPAPI GdipSetMetafileDownLevelRasterizationLimit(GpMetafile* metafile, UINT metafileRasterizationLimitDpi);
GpStatus WINGDIPAPI GdipGetMetafileDownLevelRasterizationLimit(GDIPCONST GpMetafile* metafile, UINT* metafileRasterizationLimitDpi);
#endif
GpStatus WINGDIPAPI GdipGetImageDecodersSize(UINT* numDecoders, UINT* size);
GpStatus WINGDIPAPI GdipGetImageDecoders(UINT numDecoders, UINT size, ImageCodecInfo* decoders);
GpStatus WINGDIPAPI GdipGetImageEncodersSize(UINT* numEncoders, UINT* size);
GpStatus WINGDIPAPI GdipGetImageEncoders(UINT numEncoders, UINT size, ImageCodecInfo* encoders);
GpStatus WINGDIPAPI GdipComment(GpGraphics* graphics, UINT sizeData, GDIPCONST BYTE* data);
//----------------------------------------------------------------------------
// FontFamily APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateFontFamilyFromName(GDIPCONST WCHAR* name, GpFontCollection* fontCollection, GpFontFamily** FontFamily);
GpStatus WINGDIPAPI GdipDeleteFontFamily(GpFontFamily* FontFamily);
GpStatus WINGDIPAPI GdipCloneFontFamily(GpFontFamily* FontFamily, GpFontFamily** clonedFontFamily);
GpStatus WINGDIPAPI GdipGetGenericFontFamilySansSerif(GpFontFamily** nativeFamily);
GpStatus WINGDIPAPI GdipGetGenericFontFamilySerif(GpFontFamily** nativeFamily);
GpStatus WINGDIPAPI GdipGetGenericFontFamilyMonospace(GpFontFamily** nativeFamily);
GpStatus WINGDIPAPI GdipGetFamilyName(GDIPCONST GpFontFamily* family, WCHAR name[LF_FACESIZE], LANGID language);
GpStatus WINGDIPAPI GdipIsStyleAvailable(GDIPCONST GpFontFamily* family, INT style, BOOL* IsStyleAvailable);
GpStatus WINGDIPAPI GdipFontCollectionEnumerable(GpFontCollection* fontCollection, GpGraphics* graphics, INT* numFound);
GpStatus WINGDIPAPI GdipFontCollectionEnumerate(GpFontCollection* fontCollection, INT numSought, GpFontFamily* gpfamilies[], INT* numFound, GpGraphics* graphics);
GpStatus WINGDIPAPI GdipGetEmHeight(GDIPCONST GpFontFamily* family, INT style, UINT16* EmHeight);
GpStatus WINGDIPAPI GdipGetCellAscent(GDIPCONST GpFontFamily* family, INT style, UINT16* CellAscent);
GpStatus WINGDIPAPI GdipGetCellDescent(GDIPCONST GpFontFamily* family, INT style, UINT16* CellDescent);
GpStatus WINGDIPAPI GdipGetLineSpacing(GDIPCONST GpFontFamily* family, INT style, UINT16* LineSpacing);
//----------------------------------------------------------------------------
// Font APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateFontFromDC(HDC hdc, GpFont** font);
GpStatus WINGDIPAPI GdipCreateFontFromLogfontA(HDC hdc, GDIPCONST LOGFONTA* logfont, GpFont** font);
GpStatus WINGDIPAPI GdipCreateFontFromLogfontW(HDC hdc, GDIPCONST LOGFONTW* logfont, GpFont** font);
GpStatus WINGDIPAPI GdipCreateFont(GDIPCONST GpFontFamily* fontFamily, REAL emSize, INT style, Unit unit, GpFont** font);
GpStatus WINGDIPAPI GdipCloneFont(GpFont* font, GpFont** cloneFont);
GpStatus WINGDIPAPI GdipDeleteFont(GpFont* font);
GpStatus WINGDIPAPI GdipGetFamily(GpFont* font, GpFontFamily** family);
GpStatus WINGDIPAPI GdipGetFontStyle(GpFont* font, INT* style);
GpStatus WINGDIPAPI GdipGetFontSize(GpFont* font, REAL* size);
GpStatus WINGDIPAPI GdipGetFontUnit(GpFont* font, Unit* unit);
GpStatus WINGDIPAPI GdipGetFontHeight(GDIPCONST GpFont* font, GDIPCONST GpGraphics* graphics, REAL* height);
GpStatus WINGDIPAPI GdipGetFontHeightGivenDPI(GDIPCONST GpFont* font, REAL dpi, REAL* height);
GpStatus WINGDIPAPI GdipGetLogFontA(GpFont* font, GpGraphics* graphics, LOGFONTA* logfontA);
GpStatus WINGDIPAPI GdipGetLogFontW(GpFont* font, GpGraphics* graphics, LOGFONTW* logfontW);
GpStatus WINGDIPAPI GdipNewInstalledFontCollection(GpFontCollection** fontCollection);
GpStatus WINGDIPAPI GdipNewPrivateFontCollection(GpFontCollection** fontCollection);
GpStatus WINGDIPAPI GdipDeletePrivateFontCollection(GpFontCollection** fontCollection);
GpStatus WINGDIPAPI GdipGetFontCollectionFamilyCount(GpFontCollection* fontCollection, INT* numFound);
GpStatus WINGDIPAPI GdipGetFontCollectionFamilyList(GpFontCollection* fontCollection, INT numSought, GpFontFamily* gpfamilies[], INT* numFound);
GpStatus WINGDIPAPI GdipPrivateAddFontFile(GpFontCollection* fontCollection, GDIPCONST WCHAR* filename);
GpStatus WINGDIPAPI GdipPrivateAddMemoryFont(GpFontCollection* fontCollection, GDIPCONST void* memory, INT length);
//----------------------------------------------------------------------------
// Text APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipDrawString(GpGraphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFont* font, GDIPCONST RectF* layoutRect, GDIPCONST GpStringFormat* stringFormat, GDIPCONST GpBrush* brush);
GpStatus WINGDIPAPI GdipMeasureString(GpGraphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFont* font, GDIPCONST RectF* layoutRect, GDIPCONST GpStringFormat* stringFormat, RectF* boundingBox, INT* codepointsFitted, INT* linesFilled);
#if 0
GpStatus
WINGDIPAPI GdipMeasureCharacterRanges(GpGraphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST GpFont* font, GDIPCONST RectF& layoutRect, GDIPCONST GpStringFormat* stringFormat, INT regionCount, GpRegion** regions);
#endif
GpStatus WINGDIPAPI GdipDrawDriverString(GpGraphics* graphics, GDIPCONST UINT16* text, INT length, GDIPCONST GpFont* font, GDIPCONST GpBrush* brush, GDIPCONST PointF* positions, INT flags, GDIPCONST GpMatrix* matrix);
GpStatus WINGDIPAPI GdipMeasureDriverString(GpGraphics* graphics, GDIPCONST UINT16* text, INT length, GDIPCONST GpFont* font, GDIPCONST PointF* positions, INT flags, GDIPCONST GpMatrix* matrix, RectF* boundingBox);
//----------------------------------------------------------------------------
// String format APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateStringFormat(INT formatAttributes, LANGID language, GpStringFormat** format);
GpStatus WINGDIPAPI GdipStringFormatGetGenericDefault(GpStringFormat** format);
GpStatus WINGDIPAPI GdipStringFormatGetGenericTypographic(GpStringFormat** format);
GpStatus WINGDIPAPI GdipDeleteStringFormat(GpStringFormat* format);
GpStatus WINGDIPAPI GdipCloneStringFormat(GDIPCONST GpStringFormat* format, GpStringFormat** newFormat);
GpStatus WINGDIPAPI GdipSetStringFormatFlags(GpStringFormat* format, INT flags);
GpStatus WINGDIPAPI GdipGetStringFormatFlags(GDIPCONST GpStringFormat* format, INT* flags);
GpStatus WINGDIPAPI GdipSetStringFormatAlign(GpStringFormat* format, StringAlignment align);
GpStatus WINGDIPAPI GdipGetStringFormatAlign(GDIPCONST GpStringFormat* format, StringAlignment* align);
GpStatus WINGDIPAPI GdipSetStringFormatLineAlign(GpStringFormat* format, StringAlignment align);
GpStatus WINGDIPAPI GdipGetStringFormatLineAlign(GDIPCONST GpStringFormat* format, StringAlignment* align);
GpStatus WINGDIPAPI GdipSetStringFormatTrimming(GpStringFormat* format, StringTrimming trimming);
GpStatus WINGDIPAPI GdipGetStringFormatTrimming(GDIPCONST GpStringFormat* format, StringTrimming* trimming);
GpStatus WINGDIPAPI GdipSetStringFormatHotkeyPrefix(GpStringFormat* format, INT hotkeyPrefix);
GpStatus WINGDIPAPI GdipGetStringFormatHotkeyPrefix(GDIPCONST GpStringFormat* format, INT* hotkeyPrefix);
GpStatus WINGDIPAPI GdipSetStringFormatTabStops(GpStringFormat* format, REAL firstTabOffset, INT count, GDIPCONST REAL* tabStops);
GpStatus WINGDIPAPI GdipGetStringFormatTabStops(GDIPCONST GpStringFormat* format, INT count, REAL* firstTabOffset, REAL* tabStops);
GpStatus WINGDIPAPI GdipGetStringFormatTabStopCount(GDIPCONST GpStringFormat* format, INT* count);
GpStatus WINGDIPAPI GdipSetStringFormatDigitSubstitution(GpStringFormat* format, LANGID language, StringDigitSubstitute substitute);
GpStatus WINGDIPAPI GdipGetStringFormatDigitSubstitution(GDIPCONST GpStringFormat* format, LANGID* language, StringDigitSubstitute* substitute);
GpStatus WINGDIPAPI GdipGetStringFormatMeasurableCharacterRangeCount(GDIPCONST GpStringFormat* format, INT* count);
GpStatus WINGDIPAPI GdipSetStringFormatMeasurableCharacterRanges(GpStringFormat* format, INT rangeCount, GDIPCONST CharacterRange* ranges);
//----------------------------------------------------------------------------
// Cached Bitmap APIs
//----------------------------------------------------------------------------
GpStatus WINGDIPAPI GdipCreateCachedBitmap(GpBitmap* bitmap, GpGraphics* graphics, GpCachedBitmap** cachedBitmap);
GpStatus WINGDIPAPI GdipDeleteCachedBitmap(GpCachedBitmap* cachedBitmap);
GpStatus WINGDIPAPI GdipDrawCachedBitmap(GpGraphics* graphics, GpCachedBitmap* cachedBitmap, INT x, INT y);
UINT WINGDIPAPI GdipEmfToWmfBits(HENHMETAFILE hemf, UINT cbData16, LPBYTE pData16, INT iMapMode, INT eFlags);
GpStatus WINGDIPAPI GdipSetImageAttributesCachedBackground(GpImageAttributes* imageattr, BOOL enableFlag);
GpStatus WINGDIPAPI GdipTestControl(GpTestControlEnum control, void* param);
GpStatus WINAPI GdiplusNotificationHook(OUT ULONG_PTR* token);
VOID WINAPI GdiplusNotificationUnhook(ULONG_PTR token);
// Callback function that GDI+ can call, on debug builds, for assertions
// and warnings.
typedef VOID (WINAPI* DebugEventProc)(DebugEventLevel level, CHAR* message);
// Notification functions which the user must call appropriately if
// "SuppressBackgroundThread" (below) is set.
typedef Status(WINAPI* NotificationHookProc)(OUT ULONG_PTR* token);
typedef VOID (WINAPI* NotificationUnhookProc)(ULONG_PTR token);
// Input structure for GdiplusStartup()
typedef struct {
  UINT32 GdiplusVersion; // Must be 1
  DebugEventProc DebugEventCallback; // Ignored on free builds
  BOOL SuppressBackgroundThread; // FALSE unless you're prepared to call
  // the hook/unhook functions properly
  BOOL SuppressExternalCodecs; // FALSE unless you want GDI+ only to use
  // its internal image codecs.
} GdiplusStartupInput;
// Output structure for GdiplusStartup()
typedef struct {
  // The following 2 fields are NULL if SuppressBackgroundThread is FALSE.
  // Otherwise, they are functions which must be called appropriately to
  // replace the background thread.
  //
  // These should be called on the application's main message loop - i.e.
  // a message loop which is active for the lifetime of GDI+.
  // "NotificationHook" should be called before starting the loop,
  // and "NotificationUnhook" should be called after the loop ends.
  NotificationHookProc NotificationHook;
  NotificationUnhookProc NotificationUnhook;
} GdiplusStartupOutput;
// GDI+ initialization. Must be called before GDI+ API's are used.
//
// token - may not be NULL - accepts a token to be passed in the corresponding
// GdiplusShutdown call.
// input - may not be NULL
// output - may be NULL only if input->SuppressBackgroundThread is FALSE.
Status WINAPI GdiplusStartup(OUT ULONG_PTR* token, const GdiplusStartupInput* input, OUT GdiplusStartupOutput* output);
// GDI+ termination. Must be called before GDI+ is unloaded. GDI+ API's may not
// be called after this.
VOID WINAPI GdiplusShutdown(ULONG_PTR token);
EXTERN_C_END
#endif // !_GDIPLUS_C_H_

