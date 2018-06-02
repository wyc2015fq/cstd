#ifndef _GDIPLUS_C_H_
#define _GDIPLUS_C_H_
EXTERN_C_BEGIN
#define  __stdcall
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
#define GDIP_HANDLE(Path) typedef void Path;
//---------------------------------------------------------------------------
// Private GDI+ classes for internal type checking
//---------------------------------------------------------------------------
GDIP_HANDLE(Graphics)
GDIP_HANDLE(Brush)
GDIP_HANDLE(Texture)
GDIP_HANDLE(SolidFill)
GDIP_HANDLE(LineGradient)
GDIP_HANDLE(PathGradient)
GDIP_HANDLE(Hatch)
GDIP_HANDLE(Pen)
GDIP_HANDLE(CustomLineCap)
GDIP_HANDLE(AdjustableArrowCap)
GDIP_HANDLE(Image)
GDIP_HANDLE(Bitmap)
GDIP_HANDLE(Metafile)
GDIP_HANDLE(ImageAttributes)
GDIP_HANDLE(Path)
GDIP_HANDLE(Region)
GDIP_HANDLE(PathIterator)
GDIP_HANDLE(FontFamily)
GDIP_HANDLE(Font)
GDIP_HANDLE(StringFormat)
GDIP_HANDLE(FontCollection)
GDIP_HANDLE(GpInstalledFontCollection)
GDIP_HANDLE(GpPrivateFontCollection)
GDIP_HANDLE(CachedBitmap);
//typedef int Status;
typedef Status Status;
typedef FillMode GpFillMode;
typedef WrapMode WrapMode;
typedef Unit Unit;
typedef CoordinateSpace CoordinateSpace;
typedef PointF PointF;
typedef Point Point;
typedef RectF RectF;
typedef Rect Rect;
typedef SizeF GpSizeF;
typedef HatchStyle GpHatchStyle;
typedef DashStyle DashStyle;
typedef LineCap LineCap;
typedef DashCap DashCap;
typedef PenAlignment PenAlignment;
typedef LineJoin LineJoin;
typedef PenType PenType;
GDIP_HANDLE(Matrix);
GDIP_HANDLE(BrushType);
typedef int MatrixOrder;
typedef int GpFlushIntention;
GDIP_HANDLE(PathData);
//typedef Image Bitmap;
//typedef Brush SolidFill;
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
Status  PathCreate(GpFillMode brushMode, Path** path);
Status  PathCreate2(GDIPCONST PointF*, GDIPCONST BYTE*, INT, GpFillMode, Path** path);
Status  PathCreate2I(GDIPCONST Point*, GDIPCONST BYTE*, INT, GpFillMode, Path** path);
Status  GdipClonePath(Path* path, Path** clonePath);
Status  PathDelete(Path* path);
Status  GdipResetPath(Path* path);
Status  GdipGetPointCount(Path* path, INT* count);
Status  PathGetTypes(Path* path, BYTE* types, INT count);
Status  PathGetPoints(Path*, PointF* points, INT count);
Status  PathGetPointsI(Path*, Point* points, INT count);
Status  PathGetFillMode(Path* path, GpFillMode* fillmode);
Status  GdipSetPathFillMode(Path* path, GpFillMode fillmode);
Status  PathGetData(Path* path, PathData* pathData);
Status  GdipStartPathFigure(Path* path);
Status  GdipClosePathFigure(Path* path);
Status  GdipClosePathFigures(Path* path);
Status  GdipSetPathMarker(Path* path);
Status  GdipClearPathMarkers(Path* path);
Status  GdipReversePath(Path* path);
Status  PathGetLastPoint(Path* path, PointF* lastPoint);
Status  PathAddLine(Path* path, REAL x1, REAL y1, REAL x2, REAL y2);
Status  PathAddLine2(Path* path, GDIPCONST PointF* points, INT count);
Status  PathAddArc(Path* path, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
Status  PathAddBezier(Path* path, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
Status  PathAddBeziers(Path* path, GDIPCONST PointF* points, INT count);
Status  PathAddCurve(Path* path, GDIPCONST PointF* points, INT count);
Status  PathAddCurve2(Path* path, GDIPCONST PointF* points, INT count, REAL tension);
Status  PathAddCurve3(Path* path, GDIPCONST PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension);
Status  PathAddClosedCurve(Path* path, GDIPCONST PointF* points, INT count);
Status  PathAddClosedCurve2(Path* path, GDIPCONST PointF* points, INT count, REAL tension);
Status  PathAddRectangle(Path* path, REAL x, REAL y, REAL width, REAL height);
Status  PathAddRectangles(Path* path, GDIPCONST RectF* rects, INT count);
Status  PathAddEllipse(Path* path, REAL x, REAL y, REAL width, REAL height);
Status  PathAddPie(Path* path, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
Status  PathAddPolygon(Path* path, GDIPCONST PointF* points, INT count);
Status  PathAddPath(Path* path, GDIPCONST Path* addingPath, BOOL connect);
Status  PathAddString(Path* path, GDIPCONST WCHAR* string, INT length, GDIPCONST FontFamily* family, INT style, REAL emSize, GDIPCONST RectF* layoutRect, GDIPCONST StringFormat* format);
Status  PathAddStringI(Path* path, GDIPCONST WCHAR* string, INT length, GDIPCONST FontFamily* family, INT style, REAL emSize, GDIPCONST Rect* layoutRect, GDIPCONST StringFormat* format);
Status  PathAddLineI(Path* path, INT x1, INT y1, INT x2, INT y2);
Status  PathAddLine2I(Path* path, GDIPCONST Point* points, INT count);
Status  PathAddArcI(Path* path, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
Status  PathAddBezierI(Path* path, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4);
Status  PathAddBeziersI(Path* path, GDIPCONST Point* points, INT count);
Status  PathAddCurveI(Path* path, GDIPCONST Point* points, INT count);
Status  PathAddCurve2I(Path* path, GDIPCONST Point* points, INT count, REAL tension);
Status  PathAddCurve3I(Path* path, GDIPCONST Point* points, INT count, INT offset, INT numberOfSegments, REAL tension);
Status  PathAddClosedCurveI(Path* path, GDIPCONST Point* points, INT count);
Status  PathAddClosedCurve2I(Path* path, GDIPCONST Point* points, INT count, REAL tension);
Status  PathAddRectangleI(Path* path, INT x, INT y, INT width, INT height);
Status  PathAddRectanglesI(Path* path, GDIPCONST Rect* rects, INT count);
Status  PathAddEllipseI(Path* path, INT x, INT y, INT width, INT height);
Status  PathAddPieI(Path* path, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
Status  PathAddPolygonI(Path* path, GDIPCONST Point* points, INT count);
Status  GdipFlattenPath(Path* path, Matrix* matrix, REAL flatness);
Status  GdipWindingModeOutline(Path* path, Matrix* matrix, REAL flatness);
Status  GdipWidenPath(Path* nativePath, Pen* pen, Matrix* matrix, REAL flatness);
Status  GdipWarpPath(Path* path, Matrix* matrix, GDIPCONST PointF* points, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, WarpMode warpMode, REAL flatness);
Status  GdipTransformPath(Path* path, Matrix* matrix);
Status  PathGetWorldBounds(Path* path, RectF* bounds, GDIPCONST Matrix* matrix, GDIPCONST Pen* pen);
Status  PathGetWorldBoundsI(Path* path, Rect* bounds, GDIPCONST Matrix* matrix, GDIPCONST Pen* pen);
Status  GdipIsVisiblePathPoint(Path* path, REAL x, REAL y, Graphics* graphics, BOOL* result);
Status  GdipIsVisiblePathPointI(Path* path, INT x, INT y, Graphics* graphics, BOOL* result);
Status  GdipIsOutlineVisiblePathPoint(Path* path, REAL x, REAL y, Pen* pen, Graphics* graphics, BOOL* result);
Status  GdipIsOutlineVisiblePathPointI(Path* path, INT x, INT y, Pen* pen, Graphics* graphics, BOOL* result);
//----------------------------------------------------------------------------
// PathIterator APIs
//----------------------------------------------------------------------------
Status  PathCreateIter(PathIterator** iterator, Path* path);
Status  PathDeleteIter(PathIterator* iterator);
Status  GdipPathIterNextSubpath(PathIterator* iterator, INT* resultCount, INT* startIndex, INT* endIndex, BOOL* isClosed);
Status  GdipPathIterNextSubpathPath(PathIterator* iterator, INT* resultCount, Path* path, BOOL* isClosed);
Status  GdipPathIterNextPathType(PathIterator* iterator, INT* resultCount, BYTE* pathType, INT* startIndex, INT* endIndex);
Status  GdipPathIterNextMarker(PathIterator* iterator, INT* resultCount, INT* startIndex, INT* endIndex);
Status  GdipPathIterNextMarkerPath(PathIterator* iterator, INT* resultCount, Path* path);
Status  GdipPathIterGetCount(PathIterator* iterator, INT* count);
Status  GdipPathIterGetSubpathCount(PathIterator* iterator, INT* count);
Status  GdipPathIterIsValid(PathIterator* iterator, BOOL* valid);
Status  GdipPathIterHasCurve(PathIterator* iterator, BOOL* hasCurve);
Status  GdipPathIterRewind(PathIterator* iterator);
Status  GdipPathIterEnumerate(PathIterator* iterator, INT* resultCount, PointF* points, BYTE* types, INT count);
Status  GdipPathIterCopyData(PathIterator* iterator, INT* resultCount, PointF* points, BYTE* types, INT startIndex, INT endIndex);
//----------------------------------------------------------------------------
// Matrix APIs
//----------------------------------------------------------------------------
Status  GdipCreateMatrix(Matrix** matrix);
Status  GdipCreateMatrix2(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy, Matrix** matrix);
Status  GdipCreateMatrix3(GDIPCONST RectF* rect, GDIPCONST PointF* dstplg, Matrix** matrix);
Status  GdipCreateMatrix3I(GDIPCONST Rect* rect, GDIPCONST Point* dstplg, Matrix** matrix);
Status  GdipCloneMatrix(Matrix* matrix, Matrix** cloneMatrix);
Status  GdipDeleteMatrix(Matrix* matrix);
Status  GdipSetMatrixElements(Matrix* matrix, REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy);
Status  GdipMultiplyMatrix(Matrix* matrix, Matrix* matrix2, MatrixOrder order);
Status  GdipTranslateMatrix(Matrix* matrix, REAL offsetX, REAL offsetY, MatrixOrder order);
Status  GdipScaleMatrix(Matrix* matrix, REAL scaleX, REAL scaleY, MatrixOrder order);
Status  GdipRotateMatrix(Matrix* matrix, REAL angle, MatrixOrder order);
Status  GdipShearMatrix(Matrix* matrix, REAL shearX, REAL shearY, MatrixOrder order);
Status  GdipInvertMatrix(Matrix* matrix);
Status  GdipTransformMatrixPoints(Matrix* matrix, PointF* pts, INT count);
Status  GdipTransformMatrixPointsI(Matrix* matrix, Point* pts, INT count);
Status  GdipVectorTransformMatrixPoints(Matrix* matrix, PointF* pts, INT count);
Status  GdipVectorTransformMatrixPointsI(Matrix* matrix, Point* pts, INT count);
Status  GdipGetMatrixElements(GDIPCONST Matrix* matrix, REAL* matrixOut);
Status  GdipIsMatrixInvertible(GDIPCONST Matrix* matrix, BOOL* result);
Status  GdipIsMatrixIdentity(GDIPCONST Matrix* matrix, BOOL* result);
Status  GdipIsMatrixEqual(GDIPCONST Matrix* matrix, GDIPCONST Matrix* matrix2, BOOL* result);
//----------------------------------------------------------------------------
// Region APIs
//----------------------------------------------------------------------------
Status  GdipCreateRegion(Region** region);
Status  GdipCreateRegionRect(GDIPCONST RectF* rect, Region** region);
Status  GdipCreateRegionRectI(GDIPCONST Rect* rect, Region** region);
Status  GdipCreateRegionPath(Path* path, Region** region);
Status  GdipCreateRegionRgnData(GDIPCONST BYTE* regionData, INT size, Region** region);
Status  GdipCreateRegionHrgn(HRGN hRgn, Region** region);
Status  GdipCloneRegion(Region* region, Region** cloneRegion);
Status  GdipDeleteRegion(Region* region);
Status  GdipSetInfinite(Region* region);
Status  GdipSetEmpty(Region* region);
Status  GdipCombineRegionRect(Region* region, GDIPCONST RectF* rect, CombineMode combineMode);
Status  GdipCombineRegionRectI(Region* region, GDIPCONST Rect* rect, CombineMode combineMode);
Status  GdipCombineRegionPath(Region* region, Path* path, CombineMode combineMode);
Status  GdipCombineRegionRegion(Region* region, Region* region2, CombineMode combineMode);
Status  GdipTranslateRegion(Region* region, REAL dx, REAL dy);
Status  GdipTranslateRegionI(Region* region, INT dx, INT dy);
Status  GdipTransformRegion(Region* region, Matrix* matrix);
Status  GdipGetRegionBounds(Region* region, Graphics* graphics, RectF* rect);
Status  GdipGetRegionBoundsI(Region* region, Graphics* graphics, Rect* rect);
Status  GdipGetRegionHRgn(Region* region, Graphics* graphics, HRGN* hRgn);
Status  GdipIsEmptyRegion(Region* region, Graphics* graphics, BOOL* result);
Status  GdipIsInfiniteRegion(Region* region, Graphics* graphics, BOOL* result);
Status  GdipIsEqualRegion(Region* region, Region* region2, Graphics* graphics, BOOL* result);
Status  GdipGetRegionDataSize(Region* region, UINT* bufferSize);
Status  GdipGetRegionData(Region* region, BYTE* buffer, UINT bufferSize, UINT* sizeFilled);
Status  GdipIsVisibleRegionPoint(Region* region, REAL x, REAL y, Graphics* graphics, BOOL* result);
Status  GdipIsVisibleRegionPointI(Region* region, INT x, INT y, Graphics* graphics, BOOL* result);
Status  GdipIsVisibleRegionRect(Region* region, REAL x, REAL y, REAL width, REAL height, Graphics* graphics, BOOL* result);
Status  GdipIsVisibleRegionRectI(Region* region, INT x, INT y, INT width, INT height, Graphics* graphics, BOOL* result);
Status  GdipGetRegionScansCount(Region* region, UINT* count, Matrix* matrix);
Status  GdipGetRegionScans(Region* region, RectF* rects, INT* count, Matrix* matrix);
Status  GdipGetRegionScansI(Region* region, Rect* rects, INT* count, Matrix* matrix);
//----------------------------------------------------------------------------
// Brush APIs
//----------------------------------------------------------------------------
Status  GdipCloneBrush(Brush* brush, Brush** cloneBrush);
Status  GdipDeleteBrush(Brush* brush);
Status  GdipGetBrushType(Brush* brush, BrushType* type);
//----------------------------------------------------------------------------
// HatchBrush APIs
//----------------------------------------------------------------------------
Status  GdipCreateHatchBrush(GpHatchStyle hatchstyle, ARGB forecol, ARGB backcol, Hatch** brush);
Status  GdipGetHatchStyle(Hatch* brush, GpHatchStyle* hatchstyle);
Status  GdipGetHatchForegroundColor(Hatch* brush, ARGB* forecol);
Status  GdipGetHatchBackgroundColor(Hatch* brush, ARGB* backcol);
//----------------------------------------------------------------------------
// TextureBrush APIs
//----------------------------------------------------------------------------
Status  GdipCreateTexture(Image* image, WrapMode wrapmode, Texture** texture);
Status  GdipCreateTexture2(Image* image, WrapMode wrapmode, REAL x, REAL y, REAL width, REAL height, Texture** texture);
Status  GdipCreateTextureIA(Image* image, GDIPCONST ImageAttributes* imageAttributes, REAL x, REAL y, REAL width, REAL height, Texture** texture);
Status  GdipCreateTexture2I(Image* image, WrapMode wrapmode, INT x, INT y, INT width, INT height, Texture** texture);
Status  GdipCreateTextureIAI(Image* image, GDIPCONST ImageAttributes* imageAttributes, INT x, INT y, INT width, INT height, Texture** texture);
Status  GdipGetTextureTransform(Texture* brush, Matrix* matrix);
Status  GdipSetTextureTransform(Texture* brush, GDIPCONST Matrix* matrix);
Status  GdipResetTextureTransform(Texture* brush);
Status  GdipMultiplyTextureTransform(Texture* brush, GDIPCONST Matrix* matrix, MatrixOrder order);
Status  GdipTranslateTextureTransform(Texture* brush, REAL dx, REAL dy, MatrixOrder order);
Status  GdipScaleTextureTransform(Texture* brush, REAL sx, REAL sy, MatrixOrder order);
Status  GdipRotateTextureTransform(Texture* brush, REAL angle, MatrixOrder order);
Status  GdipSetTextureWrapMode(Texture* brush, WrapMode wrapmode);
Status  GdipGetTextureWrapMode(Texture* brush, WrapMode* wrapmode);
Status  GdipGetTextureImage(Texture* brush, Image** image);
//----------------------------------------------------------------------------
// SolidBrush APIs
//----------------------------------------------------------------------------
Status  GdipCreateSolidFill(ARGB color, SolidFill** brush);
Status  GdipSetSolidFillColor(SolidFill* brush, ARGB color);
Status  GdipGetSolidFillColor(SolidFill* brush, ARGB* color);
//----------------------------------------------------------------------------
// LineBrush APIs
//----------------------------------------------------------------------------
Status  GdipCreateLineBrush(GDIPCONST PointF* point1, GDIPCONST PointF* point2, ARGB color1, ARGB color2, WrapMode wrapMode, LineGradient** line);
Status  GdipCreateLineBrushI(GDIPCONST Point* point1, GDIPCONST Point* point2, ARGB color1, ARGB color2, WrapMode wrapMode, LineGradient** line);
Status  GdipCreateLineBrushFromRect(GDIPCONST RectF* rect, ARGB color1, ARGB color2, LinearGradientMode mode, WrapMode wrapMode, LineGradient** line);
Status  GdipCreateLineBrushFromRectI(GDIPCONST Rect* rect, ARGB color1, ARGB color2, LinearGradientMode mode, WrapMode wrapMode, LineGradient** line);
Status  GdipCreateLineBrushFromRectWithAngle(GDIPCONST RectF* rect, ARGB color1, ARGB color2, REAL angle, BOOL isAngleScalable, WrapMode wrapMode, LineGradient** line);
Status  GdipCreateLineBrushFromRectWithAngleI(GDIPCONST Rect* rect, ARGB color1, ARGB color2, REAL angle, BOOL isAngleScalable, WrapMode wrapMode, LineGradient** line);
Status  GdipSetLineColors(LineGradient* brush, ARGB color1, ARGB color2);
Status  GdipGetLineColors(LineGradient* brush, ARGB* colors);
Status  GdipGetLineRect(LineGradient* brush, RectF* rect);
Status  GdipGetLineRectI(LineGradient* brush, Rect* rect);
Status  GdipSetLineGammaCorrection(LineGradient* brush, BOOL useGammaCorrection);
Status  GdipGetLineGammaCorrection(LineGradient* brush, BOOL* useGammaCorrection);
Status  GdipGetLineBlendCount(LineGradient* brush, INT* count);
Status  GdipGetLineBlend(LineGradient* brush, REAL* blend, REAL* positions, INT count);
Status  GdipSetLineBlend(LineGradient* brush, GDIPCONST REAL* blend, GDIPCONST REAL* positions, INT count);
Status  GdipGetLinePresetBlendCount(LineGradient* brush, INT* count);
Status  GdipGetLinePresetBlend(LineGradient* brush, ARGB* blend, REAL* positions, INT count);
Status  GdipSetLinePresetBlend(LineGradient* brush, GDIPCONST ARGB* blend, GDIPCONST REAL* positions, INT count);
Status  GdipSetLineSigmaBlend(LineGradient* brush, REAL focus, REAL scale);
Status  GdipSetLineLinearBlend(LineGradient* brush, REAL focus, REAL scale);
Status  GdipSetLineWrapMode(LineGradient* brush, WrapMode wrapmode);
Status  GdipGetLineWrapMode(LineGradient* brush, WrapMode* wrapmode);
Status  GdipGetLineTransform(LineGradient* brush, Matrix* matrix);
Status  GdipSetLineTransform(LineGradient* brush, GDIPCONST Matrix* matrix);
Status  GdipResetLineTransform(LineGradient* brush);
Status  GdipMultiplyLineTransform(LineGradient* brush, GDIPCONST Matrix* matrix, MatrixOrder order);
Status  GdipTranslateLineTransform(LineGradient* brush, REAL dx, REAL dy, MatrixOrder order);
Status  GdipScaleLineTransform(LineGradient* brush, REAL sx, REAL sy, MatrixOrder order);
Status  GdipRotateLineTransform(LineGradient* brush, REAL angle, MatrixOrder order);
//---------------------------------------------------------------------------- // PathGradientBrush APIs
//----------------------------------------------------------------------------
Status  PathCreateGradient(GDIPCONST PointF* points, INT count, WrapMode wrapMode, PathGradient** polyGradient);
Status  PathCreateGradientI(GDIPCONST Point* points, INT count, WrapMode wrapMode, PathGradient** polyGradient);
Status  PathCreateGradientFromPath(GDIPCONST Path* path, PathGradient** polyGradient);
Status  PathGradientGetCenterColor(PathGradient* brush, ARGB* colors);
Status  PathGradientSetCenterColor(PathGradient* brush, ARGB colors);
Status  PathGradientGetSurroundColorsWithCount(PathGradient* brush, ARGB* color, INT* count);
Status  PathGradientSetSurroundColorsWithCount(PathGradient* brush, GDIPCONST ARGB* color, INT* count);
Status  PathGradientGetPath(PathGradient* brush, Path* path);
Status  PathGradientSetPath(PathGradient* brush, GDIPCONST Path* path);
Status  PathGradientGetCenterPoint(PathGradient* brush, PointF* points);
Status  PathGradientGetCenterPointI(PathGradient* brush, Point* points);
Status  PathGradientSetCenterPoint(PathGradient* brush, GDIPCONST PointF* points);
Status  PathGradientSetCenterPointI(PathGradient* brush, GDIPCONST Point* points);
Status  PathGradientGetRect(PathGradient* brush, RectF* rect);
Status  PathGradientGetRectI(PathGradient* brush, Rect* rect);
Status  PathGradientGetPointCount(PathGradient* brush, INT* count);
Status  PathGradientGetSurroundColorCount(PathGradient* brush, INT* count);
Status  PathGradientSetGammaCorrection(PathGradient* brush, BOOL useGammaCorrection);
Status  PathGradientGetGammaCorrection(PathGradient* brush, BOOL* useGammaCorrection);
Status  PathGradientGetBlendCount(PathGradient* brush, INT* count);
Status  PathGradientGetBlend(PathGradient* brush, REAL* blend, REAL* positions, INT count);
Status  PathGradientSetBlend(PathGradient* brush, GDIPCONST REAL* blend, GDIPCONST REAL* positions, INT count);
Status  PathGradientGetPresetBlendCount(PathGradient* brush, INT* count);
Status  PathGradientGetPresetBlend(PathGradient* brush, ARGB* blend, REAL* positions, INT count);
Status  PathGradientSetPresetBlend(PathGradient* brush, GDIPCONST ARGB* blend, GDIPCONST REAL* positions, INT count);
Status  PathGradientSetSigmaBlend(PathGradient* brush, REAL focus, REAL scale);
Status  PathGradientSetLinearBlend(PathGradient* brush, REAL focus, REAL scale);
Status  PathGradientGetWrapMode(PathGradient* brush, WrapMode* wrapmode);
Status  PathGradientSetWrapMode(PathGradient* brush, WrapMode wrapmode);
Status  PathGradientGetTransform(PathGradient* brush, Matrix* matrix);
Status  PathGradientSetTransform(PathGradient* brush, Matrix* matrix);
Status  GdipResetPathGradientTransform(PathGradient* brush);
Status  GdipMultiplyPathGradientTransform(PathGradient* brush, GDIPCONST Matrix* matrix, MatrixOrder order);
Status  GdipTranslatePathGradientTransform(PathGradient* brush, REAL dx, REAL dy, MatrixOrder order);
Status  GdipScalePathGradientTransform(PathGradient* brush, REAL sx, REAL sy, MatrixOrder order);
Status  GdipRotatePathGradientTransform(PathGradient* brush, REAL angle, MatrixOrder order);
Status  PathGradientGetFocusScales(PathGradient* brush, REAL* xScale, REAL* yScale);
Status  PathGradientSetFocusScales(PathGradient* brush, REAL xScale, REAL yScale);
//----------------------------------------------------------------------------
// Pen APIs
//----------------------------------------------------------------------------
Status  PenCreate1(ARGB color, REAL width, Unit unit, Pen** pen);
Status  PenCreate2(Brush* brush, REAL width, Unit unit, Pen** pen);
Status  GdipClonePen(Pen* pen, Pen** clonepen);
Status  GdipDeletePen(Pen* pen);
Status  PenSetWidth(Pen* pen, REAL width);
Status  PenGetWidth(Pen* pen, REAL* width);
Status  PenSetUnit(Pen* pen, Unit unit);
Status  PenGetUnit(Pen* pen, Unit* unit);
Status  PenSetLineCap197819(Pen* pen, LineCap startCap, LineCap endCap, DashCap dashCap);
Status  PenSetStartCap(Pen* pen, LineCap startCap);
Status  PenSetEndCap(Pen* pen, LineCap endCap);
Status  PenSetDashCap197819(Pen* pen, DashCap dashCap);
Status  PenGetStartCap(Pen* pen, LineCap* startCap);
Status  PenGetEndCap(Pen* pen, LineCap* endCap);
Status  PenGetDashCap197819(Pen* pen, DashCap* dashCap);
Status  PenSetLineJoin(Pen* pen, LineJoin lineJoin);
Status  PenGetLineJoin(Pen* pen, LineJoin* lineJoin);
Status  PenSetCustomStartCap(Pen* pen, CustomLineCap* customCap);
Status  PenGetCustomStartCap(Pen* pen, CustomLineCap** customCap);
Status  PenSetCustomEndCap(Pen* pen, CustomLineCap* customCap);
Status  PenGetCustomEndCap(Pen* pen, CustomLineCap** customCap);
Status  PenSetMiterLimit(Pen* pen, REAL miterLimit);
Status  PenGetMiterLimit(Pen* pen, REAL* miterLimit);
Status  PenSetMode(Pen* pen, PenAlignment penMode);
Status  PenGetMode(Pen* pen, PenAlignment* penMode);
Status  PenSetTransform(Pen* pen, Matrix* matrix);
Status  PenGetTransform(Pen* pen, Matrix* matrix);
Status  PenResetTransform(Pen* pen);
Status  GdipMultiplyPenTransform(Pen* pen, GDIPCONST Matrix* matrix, MatrixOrder order);
Status  GdipTranslatePenTransform(Pen* pen, REAL dx, REAL dy, MatrixOrder order);
Status  GdipScalePenTransform(Pen* pen, REAL sx, REAL sy, MatrixOrder order);
Status  GdipRotatePenTransform(Pen* pen, REAL angle, MatrixOrder order);
Status  PenSetColor(Pen* pen, ARGB argb);
Status  PenGetColor(Pen* pen, ARGB* argb);
Status  PenSetBrushFill(Pen* pen, Brush* brush);
Status  PenGetBrushFill(Pen* pen, Brush** brush);
Status  PenGetFillType(Pen* pen, PenType* type);
Status  PenGetDashStyle(Pen* pen, DashStyle* dashstyle);
Status  PenSetDashStyle(Pen* pen, DashStyle dashstyle);
Status  PenGetDashOffset(Pen* pen, REAL* offset);
Status  PenSetDashOffset(Pen* pen, REAL offset);
Status  PenGetDashCount(Pen* pen, INT* count);
Status  PenSetDashArray(Pen* pen, GDIPCONST REAL* dash, INT count);
Status  PenGetDashArray(Pen* pen, REAL* dash, INT count);
Status  PenGetCompoundCount(Pen* pen, INT* count);
Status  PenSetCompoundArray(Pen* pen, GDIPCONST REAL* dash, INT count);
Status  PenGetCompoundArray(Pen* pen, REAL* dash, INT count);
//----------------------------------------------------------------------------
// CustomLineCap APIs
//----------------------------------------------------------------------------
Status  GdipCreateCustomLineCap(Path* fillPath, Path* strokePath, LineCap baseCap, REAL baseInset, CustomLineCap** customCap);
Status  GdipDeleteCustomLineCap(CustomLineCap* customCap);
Status  GdipCloneCustomLineCap(CustomLineCap* customCap, CustomLineCap** clonedCap);
Status  GdipGetCustomLineCapType(CustomLineCap* customCap, CustomLineCapType* capType);
Status  GdipSetCustomLineCapStrokeCaps(CustomLineCap* customCap, LineCap startCap, LineCap endCap);
Status  GdipGetCustomLineCapStrokeCaps(CustomLineCap* customCap, LineCap* startCap, LineCap* endCap);
Status  GdipSetCustomLineCapStrokeJoin(CustomLineCap* customCap, LineJoin lineJoin);
Status  GdipGetCustomLineCapStrokeJoin(CustomLineCap* customCap, LineJoin* lineJoin);
Status  GdipSetCustomLineCapBaseCap(CustomLineCap* customCap, LineCap baseCap);
Status  GdipGetCustomLineCapBaseCap(CustomLineCap* customCap, LineCap* baseCap);
Status  GdipSetCustomLineCapBaseInset(CustomLineCap* customCap, REAL inset);
Status  GdipGetCustomLineCapBaseInset(CustomLineCap* customCap, REAL* inset);
Status  GdipSetCustomLineCapWidthScale(CustomLineCap* customCap, REAL widthScale);
Status  GdipGetCustomLineCapWidthScale(CustomLineCap* customCap, REAL* widthScale);
//----------------------------------------------------------------------------
// AdjustableArrowCap APIs
//----------------------------------------------------------------------------
Status  GdipCreateAdjustableArrowCap(REAL height, REAL width, BOOL isFilled, AdjustableArrowCap** cap);
Status  GdipSetAdjustableArrowCapHeight(AdjustableArrowCap* cap, REAL height);
Status  GdipGetAdjustableArrowCapHeight(AdjustableArrowCap* cap, REAL* height);
Status  GdipSetAdjustableArrowCapWidth(AdjustableArrowCap* cap, REAL width);
Status  GdipGetAdjustableArrowCapWidth(AdjustableArrowCap* cap, REAL* width);
Status  GdipSetAdjustableArrowCapMiddleInset(AdjustableArrowCap* cap, REAL middleInset);
Status  GdipGetAdjustableArrowCapMiddleInset(AdjustableArrowCap* cap, REAL* middleInset);
Status  GdipSetAdjustableArrowCapFillState(AdjustableArrowCap* cap, BOOL fillState);
Status  GdipGetAdjustableArrowCapFillState(AdjustableArrowCap* cap, BOOL* fillState);
//----------------------------------------------------------------------------
// Image APIs
//----------------------------------------------------------------------------
Status  GdipLoadImageFromStream(IStream* stream, Image** image);
Status  GdipLoadImageFromFile(GDIPCONST WCHAR* filename, Image** image);
Status  GdipLoadImageFromStreamICM(IStream* stream, Image** image);
Status  GdipLoadImageFromFileICM(GDIPCONST WCHAR* filename, Image** image);
Status  GdipCloneImage(Image* image, Image** cloneImage);
Status  GdipDisposeImage(Image* image);
Status  GdipSaveImageToFile(Image* image, GDIPCONST WCHAR* filename, GDIPCONST CLSID* clsidEncoder, GDIPCONST EncoderParameters* encoderParams);
Status  GdipSaveImageToStream(Image* image, IStream* stream, GDIPCONST CLSID* clsidEncoder, GDIPCONST EncoderParameters* encoderParams);
Status  GdipSaveAdd(Image* image, GDIPCONST EncoderParameters* encoderParams);
Status  GdipSaveAddImage(Image* image, Image* newImage, GDIPCONST EncoderParameters* encoderParams);
Status  GdipGetImageGraphicsContext(Image* image, Graphics** graphics);
Status  GdipGetImageBounds(Image* image, RectF* srcRect, Unit* srcUnit);
Status  GdipGetImageDimension(Image* image, REAL* width, REAL* height);
Status  GdipGetImageType(Image* image, ImageType* type);
Status  GdipGetImageWidth(Image* image, UINT* width);
Status  GdipGetImageHeight(Image* image, UINT* height);
Status  GdipGetImageHorizontalResolution(Image* image, REAL* resolution);
Status  GdipGetImageVerticalResolution(Image* image, REAL* resolution);
Status  GdipGetImageFlags(Image* image, UINT* flags);
Status  GdipGetImageRawFormat(Image* image, GUID* format);
Status  GdipGetImagePixelFormat(Image* image, PixelFormat* format);
Status  GdipGetImageThumbnail(Image* image, UINT thumbWidth, UINT thumbHeight, Image** thumbImage, GetThumbnailImageAbort callback, VOID* callbackData);
Status  GdipGetEncoderParameterListSize(Image* image, GDIPCONST CLSID* clsidEncoder, UINT* size);
Status  GdipGetEncoderParameterList(Image* image, GDIPCONST CLSID* clsidEncoder, UINT size, EncoderParameters* buffer);
Status  GdipImageGetFrameDimensionsCount(Image* image, UINT* count);
Status  GdipImageGetFrameDimensionsList(Image* image, GUID* dimensionIDs, UINT count);
Status  GdipImageGetFrameCount(Image* image, GDIPCONST GUID* dimensionID, UINT* count);
Status  GdipImageSelectActiveFrame(Image* image, GDIPCONST GUID* dimensionID, UINT frameIndex);
Status  GdipImageRotateFlip(Image* image, RotateFlipType rfType);
Status  GdipGetImagePalette(Image* image, ColorPalette* palette, INT size);
Status  GdipSetImagePalette(Image* image, GDIPCONST ColorPalette* palette);
Status  GdipGetImagePaletteSize(Image* image, INT* size);
Status  GdipGetPropertyCount(Image* image, UINT* numOfProperty);
Status  GdipGetPropertyIdList(Image* image, UINT numOfProperty, PROPID* list);
Status  GdipGetPropertyItemSize(Image* image, PROPID propId, UINT* size);
Status  GdipGetPropertyItem(Image* image, PROPID propId, UINT propSize, PropertyItem* buffer);
Status  GdipGetPropertySize(Image* image, UINT* totalBufferSize, UINT* numProperties);
Status  GdipGetAllPropertyItems(Image* image, UINT totalBufferSize, UINT numProperties, PropertyItem* allItems);
Status  GdipRemovePropertyItem(Image* image, PROPID propId);
Status  GdipSetPropertyItem(Image* image, GDIPCONST PropertyItem* item);
Status  GdipImageForceValidation(Image* image);
//----------------------------------------------------------------------------
// Bitmap APIs
//----------------------------------------------------------------------------
Status  GdipCreateBitmapFromStream(IStream* stream, Bitmap** bitmap);
Status  GdipCreateBitmapFromFile(GDIPCONST WCHAR* filename, Bitmap** bitmap);
Status  GdipCreateBitmapFromStreamICM(IStream* stream, Bitmap** bitmap);
Status  GdipCreateBitmapFromFileICM(GDIPCONST WCHAR* filename, Bitmap** bitmap);
Status  GdipCreateBitmapFromScan0(INT width, INT height, INT stride, PixelFormat format, BYTE* scan0, Bitmap** bitmap);
Status  GdipCreateBitmapFromGraphics(INT width, INT height, Graphics* target, Bitmap** bitmap);
Status  GdipCreateBitmapFromDirectDrawSurface(IDirectDrawSurface7* surface, Bitmap** bitmap);
Status  GdipCreateBitmapFromGdiDib(GDIPCONST BITMAPINFO* gdiBitmapInfo, VOID* gdiBitmapData, Bitmap** bitmap);
Status  GdipCreateBitmapFromHBITMAP(HBITMAP hbm, HPALETTE hpal, Bitmap** bitmap);
Status  GdipCreateHBITMAPFromBitmap(Bitmap* bitmap, HBITMAP* hbmReturn, ARGB background);
Status  GdipCreateBitmapFromHICON(HICON hicon, Bitmap** bitmap);
Status  GdipCreateHICONFromBitmap(Bitmap* bitmap, HICON* hbmReturn);
Status  GdipCreateBitmapFromResource(HINSTANCE hInstance, GDIPCONST WCHAR* lpBitmapName, Bitmap** bitmap);
Status  GdipCloneBitmapArea(REAL x, REAL y, REAL width, REAL height, PixelFormat format, Bitmap* srcBitmap, Bitmap** dstBitmap);
Status  GdipCloneBitmapAreaI(INT x, INT y, INT width, INT height, PixelFormat format, Bitmap* srcBitmap, Bitmap** dstBitmap);
Status  GdipBitmapLockBits(Bitmap* bitmap, GDIPCONST Rect* rect, UINT flags, PixelFormat format, BitmapData* lockedBitmapData);
Status  GdipBitmapUnlockBits(Bitmap* bitmap, BitmapData* lockedBitmapData);
Status  GdipBitmapGetPixel(Bitmap* bitmap, INT x, INT y, ARGB* color);
Status  GdipBitmapSetPixel(Bitmap* bitmap, INT x, INT y, ARGB color);
Status  GdipBitmapSetResolution(Bitmap* bitmap, REAL xdpi, REAL ydpi);
//----------------------------------------------------------------------------
// ImageAttributes APIs
//----------------------------------------------------------------------------
Status  GdipCreateImageAttributes(ImageAttributes** imageattr);
Status  GdipCloneImageAttributes(GDIPCONST ImageAttributes* imageattr, ImageAttributes** cloneImageattr);
Status  GdipDisposeImageAttributes(ImageAttributes* imageattr);
Status  GdipSetImageAttributesToIdentity(ImageAttributes* imageattr, ColorAdjustType type);
Status  GdipResetImageAttributes(ImageAttributes* imageattr, ColorAdjustType type);
Status  GdipSetImageAttributesColorMatrix(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, GDIPCONST ColorMatrix* colorMatrix, GDIPCONST ColorMatrix* grayMatrix, ColorMatrixFlags flags);
Status  GdipSetImageAttributesThreshold(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, REAL threshold);
Status  GdipSetImageAttributesGamma(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, REAL gamma);
Status  GdipSetImageAttributesNoOp(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag);
Status  GdipSetImageAttributesColorKeys(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, ARGB colorLow, ARGB colorHigh);
Status  GdipSetImageAttributesOutputChannel(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, ColorChannelFlags channelFlags);
Status  GdipSetImageAttributesOutputChannelColorProfile(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, GDIPCONST WCHAR* colorProfileFilename);
Status  GdipSetImageAttributesRemapTable(ImageAttributes* imageattr, ColorAdjustType type, BOOL enableFlag, UINT mapSize, GDIPCONST ColorMap* map);
Status  GdipSetImageAttributesWrapMode(ImageAttributes* imageAttr, WrapMode wrap, ARGB argb, BOOL clamp);
Status  GdipSetImageAttributesICMMode(ImageAttributes* imageAttr, BOOL on);
Status  GdipGetImageAttributesAdjustedPalette(ImageAttributes* imageAttr, ColorPalette* colorPalette, ColorAdjustType colorAdjustType);
//----------------------------------------------------------------------------
// Graphics APIs
//----------------------------------------------------------------------------
Status  GdipFlush(Graphics* graphics, GpFlushIntention intention);
Status  GdipCreateFromHDC(HDC hdc, Graphics** graphics);
Status  GdipCreateFromHDC2(HDC hdc, HANDLE hDevice, Graphics** graphics);
Status  GdipCreateFromHWND(HWND hwnd, Graphics** graphics);
Status  GdipCreateFromHWNDICM(HWND hwnd, Graphics** graphics);
Status  GdipDeleteGraphics(Graphics* graphics);
Status  GdipGetDC(Graphics* graphics, HDC* hdc);
Status  GdipReleaseDC(Graphics* graphics, HDC hdc);
Status  GdipSetCompositingMode(Graphics* graphics, CompositingMode compositingMode);
Status  GdipGetCompositingMode(Graphics* graphics, CompositingMode* compositingMode);
Status  GdipSetRenderingOrigin(Graphics* graphics, INT x, INT y);
Status  GdipGetRenderingOrigin(Graphics* graphics, INT* x, INT* y);
Status  GdipSetCompositingQuality(Graphics* graphics, CompositingQuality compositingQuality);
Status  GdipGetCompositingQuality(Graphics* graphics, CompositingQuality* compositingQuality);
Status  GdipSetSmoothingMode(Graphics* graphics, SmoothingMode smoothingMode);
Status  GdipGetSmoothingMode(Graphics* graphics, SmoothingMode* smoothingMode);
Status  GdipSetPixelOffsetMode(Graphics* graphics, PixelOffsetMode pixelOffsetMode);
Status  GdipGetPixelOffsetMode(Graphics* graphics, PixelOffsetMode* pixelOffsetMode);
Status  GdipSetTextRenderingHint(Graphics* graphics, TextRenderingHint mode);
Status  GdipGetTextRenderingHint(Graphics* graphics, TextRenderingHint* mode);
Status  GdipSetTextContrast(Graphics* graphics, UINT contrast);
Status  GdipGetTextContrast(Graphics* graphics, UINT* contrast);
Status  GdipSetInterpolationMode(Graphics* graphics, InterpolationMode interpolationMode);
Status  GdipGetInterpolationMode(Graphics* graphics, InterpolationMode* interpolationMode);
Status  GdipSetWorldTransform(Graphics* graphics, Matrix* matrix);
Status  GdipResetWorldTransform(Graphics* graphics);
Status  GdipMultiplyWorldTransform(Graphics* graphics, GDIPCONST Matrix* matrix, MatrixOrder order);
Status  GdipTranslateWorldTransform(Graphics* graphics, REAL dx, REAL dy, MatrixOrder order);
Status  GdipScaleWorldTransform(Graphics* graphics, REAL sx, REAL sy, MatrixOrder order);
Status  GdipRotateWorldTransform(Graphics* graphics, REAL angle, MatrixOrder order);
Status  GdipGetWorldTransform(Graphics* graphics, Matrix* matrix);
Status  GdipResetPageTransform(Graphics* graphics);
Status  GdipGetPageUnit(Graphics* graphics, Unit* unit);
Status  GdipGetPageScale(Graphics* graphics, REAL* scale);
Status  GdipSetPageUnit(Graphics* graphics, Unit unit);
Status  GdipSetPageScale(Graphics* graphics, REAL scale);
Status  GdipGetDpiX(Graphics* graphics, REAL* dpi);
Status  GdipGetDpiY(Graphics* graphics, REAL* dpi);
Status  GdipTransformPoints(Graphics* graphics, CoordinateSpace destSpace, CoordinateSpace srcSpace, PointF* points, INT count);
Status  GdipTransformPointsI(Graphics* graphics, CoordinateSpace destSpace, CoordinateSpace srcSpace, Point* points, INT count);
Status  GdipGetNearestColor(Graphics* graphics, ARGB* argb);
// Creates the Win9x Halftone Palette (even on NT) with correct Desktop colors
HPALETTE  GdipCreateHalftonePalette();
Status  GdipDrawLine(Graphics* graphics, Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2);
Status  GdipDrawLineI(Graphics* graphics, Pen* pen, INT x1, INT y1, INT x2, INT y2);
Status  GdipDrawLines(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count);
Status  GdipDrawLinesI(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count);
Status  GdipDrawArc(Graphics* graphics, Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
Status  GdipDrawArcI(Graphics* graphics, Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
Status  GdipDrawBezier(Graphics* graphics, Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4);
Status  GdipDrawBezierI(Graphics* graphics, Pen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4);
Status  GdipDrawBeziers(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count);
Status  GdipDrawBeziersI(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count);
Status  GdipDrawRectangle(Graphics* graphics, Pen* pen, REAL x, REAL y, REAL width, REAL height);
Status  GdipDrawRectangleI(Graphics* graphics, Pen* pen, INT x, INT y, INT width, INT height);
Status  GdipDrawRectangles(Graphics* graphics, Pen* pen, GDIPCONST RectF* rects, INT count);
Status  GdipDrawRectanglesI(Graphics* graphics, Pen* pen, GDIPCONST Rect* rects, INT count);
Status  GdipDrawEllipse(Graphics* graphics, Pen* pen, REAL x, REAL y, REAL width, REAL height);
Status  GdipDrawEllipseI(Graphics* graphics, Pen* pen, INT x, INT y, INT width, INT height);
Status  GdipDrawPie(Graphics* graphics, Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
Status  GdipDrawPieI(Graphics* graphics, Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
Status  GdipDrawPolygon(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count);
Status  GdipDrawPolygonI(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count);
Status  GdipDrawPath(Graphics* graphics, Pen* pen, Path* path);
Status  GdipDrawCurve(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count);
Status  GdipDrawCurveI(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count);
Status  GdipDrawCurve2(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count, REAL tension);
Status  GdipDrawCurve2I(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count, REAL tension);
Status  GdipDrawCurve3(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension);
Status  GdipDrawCurve3I(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count, INT offset, INT numberOfSegments, REAL tension);
Status  GdipDrawClosedCurve(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count);
Status  GdipDrawClosedCurveI(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count);
Status  GdipDrawClosedCurve2(Graphics* graphics, Pen* pen, GDIPCONST PointF* points, INT count, REAL tension);
Status  GdipDrawClosedCurve2I(Graphics* graphics, Pen* pen, GDIPCONST Point* points, INT count, REAL tension);
Status  GdipGraphicsClear(Graphics* graphics, ARGB color);
Status  GdipFillRectangle(Graphics* graphics, Brush* brush, REAL x, REAL y, REAL width, REAL height);
Status  GdipFillRectangleI(Graphics* graphics, Brush* brush, INT x, INT y, INT width, INT height);
Status  GdipFillRectangles(Graphics* graphics, Brush* brush, GDIPCONST RectF* rects, INT count);
Status  GdipFillRectanglesI(Graphics* graphics, Brush* brush, GDIPCONST Rect* rects, INT count);
Status  GdipFillPolygon(Graphics* graphics, Brush* brush, GDIPCONST PointF* points, INT count, GpFillMode fillMode);
Status  GdipFillPolygonI(Graphics* graphics, Brush* brush, GDIPCONST Point* points, INT count, GpFillMode fillMode);
Status  GdipFillPolygon2(Graphics* graphics, Brush* brush, GDIPCONST PointF* points, INT count);
Status  GdipFillPolygon2I(Graphics* graphics, Brush* brush, GDIPCONST Point* points, INT count);
Status  GdipFillEllipse(Graphics* graphics, Brush* brush, REAL x, REAL y, REAL width, REAL height);
Status  GdipFillEllipseI(Graphics* graphics, Brush* brush, INT x, INT y, INT width, INT height);
Status  GdipFillPie(Graphics* graphics, Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle);
Status  GdipFillPieI(Graphics* graphics, Brush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle);
Status  GdipFillPath(Graphics* graphics, Brush* brush, Path* path);
Status  GdipFillClosedCurve(Graphics* graphics, Brush* brush, GDIPCONST PointF* points, INT count);
Status  GdipFillClosedCurveI(Graphics* graphics, Brush* brush, GDIPCONST Point* points, INT count);
Status  GdipFillClosedCurve2(Graphics* graphics, Brush* brush, GDIPCONST PointF* points, INT count, REAL tension, GpFillMode fillMode);
Status  GdipFillClosedCurve2I(Graphics* graphics, Brush* brush, GDIPCONST Point* points, INT count, REAL tension, GpFillMode fillMode);
Status  GdipFillRegion(Graphics* graphics, Brush* brush, Region* region);
Status  GdipDrawImage(Graphics* graphics, Image* image, REAL x, REAL y);
Status  GdipDrawImageI(Graphics* graphics, Image* image, INT x, INT y);
Status  GdipDrawImageRect(Graphics* graphics, Image* image, REAL x, REAL y, REAL width, REAL height);
Status  GdipDrawImageRectI(Graphics* graphics, Image* image, INT x, INT y, INT width, INT height);
Status  GdipDrawImagePoints(Graphics* graphics, Image* image, GDIPCONST PointF* dstpoints, INT count);
Status  GdipDrawImagePointsI(Graphics* graphics, Image* image, GDIPCONST Point* dstpoints, INT count);
Status  GdipDrawImagePointRect(Graphics* graphics, Image* image, REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit);
Status  GdipDrawImagePointRectI(Graphics* graphics, Image* image, INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit);
Status  GdipDrawImageRectRect(Graphics* graphics, Image* image, REAL dstx, REAL dsty, REAL dstwidth, REAL dstheight, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, GDIPCONST ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
Status  GdipDrawImageRectRectI(Graphics* graphics, Image* image, INT dstx, INT dsty, INT dstwidth, INT dstheight, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, GDIPCONST ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
Status  GdipDrawImagePointsRect(Graphics* graphics, Image* image, GDIPCONST PointF* points, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, GDIPCONST ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
Status  GdipDrawImagePointsRectI(Graphics* graphics, Image* image, GDIPCONST Point* points, INT count, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, GDIPCONST ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData);
#if 0
Status  GdipEnumerateMetafileDestPoint(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST PointF& destPoint, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileDestPointI(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST Point& destPoint, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileDestRect(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST RectF& destRect, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileDestRectI(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST Rect& destRect, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileDestPoints(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST PointF* destPoints, INT count, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileDestPointsI(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST Point* destPoints, INT count, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileSrcRectDestPoint(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST PointF& destPoint, GDIPCONST RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileSrcRectDestPointI(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST Point& destPoint, GDIPCONST Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileSrcRectDestRect(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST RectF& destRect, GDIPCONST RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileSrcRectDestRectI(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST Rect& destRect, GDIPCONST Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileSrcRectDestPoints(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST PointF* destPoints, INT count, GDIPCONST RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
Status  GdipEnumerateMetafileSrcRectDestPointsI(Graphics* graphics, GDIPCONST Metafile* metafile, GDIPCONST Point* destPoints, INT count, GDIPCONST Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, GDIPCONST ImageAttributes* imageAttributes);
#endif
Status  GdipPlayMetafileRecord(GDIPCONST Metafile* metafile, EmfPlusRecordType recordType, UINT flags, UINT dataSize, GDIPCONST BYTE* data);
Status  GdipSetClipGraphics(Graphics* graphics, Graphics* srcgraphics, CombineMode combineMode);
Status  GdipSetClipRect(Graphics* graphics, REAL x, REAL y, REAL width, REAL height, CombineMode combineMode);
Status  GdipSetClipRectI(Graphics* graphics, INT x, INT y, INT width, INT height, CombineMode combineMode);
Status  GdipSetClipPath(Graphics* graphics, Path* path, CombineMode combineMode);
Status  GdipSetClipRegion(Graphics* graphics, Region* region, CombineMode combineMode);
Status  GdipSetClipHrgn(Graphics* graphics, HRGN hRgn, CombineMode combineMode);
Status  GdipResetClip(Graphics* graphics);
Status  GdipTranslateClip(Graphics* graphics, REAL dx, REAL dy);
Status  GdipTranslateClipI(Graphics* graphics, INT dx, INT dy);
Status  GdipGetClip(Graphics* graphics, Region* region);
Status  GdipGetClipBounds(Graphics* graphics, RectF* rect);
Status  GdipGetClipBoundsI(Graphics* graphics, Rect* rect);
Status  GdipIsClipEmpty(Graphics* graphics, BOOL* result);
Status  GdipGetVisibleClipBounds(Graphics* graphics, RectF* rect);
Status  GdipGetVisibleClipBoundsI(Graphics* graphics, Rect* rect);
Status  GdipIsVisibleClipEmpty(Graphics* graphics, BOOL* result);
Status  GdipIsVisiblePoint(Graphics* graphics, REAL x, REAL y, BOOL* result);
Status  GdipIsVisiblePointI(Graphics* graphics, INT x, INT y, BOOL* result);
Status  GdipIsVisibleRect(Graphics* graphics, REAL x, REAL y, REAL width, REAL height, BOOL* result);
Status  GdipIsVisibleRectI(Graphics* graphics, INT x, INT y, INT width, INT height, BOOL* result);
Status  GdipSaveGraphics(Graphics* graphics, GraphicsState* state);
Status  GdipRestoreGraphics(Graphics* graphics, GraphicsState state);
Status  GdipBeginContainer(Graphics* graphics, GDIPCONST RectF* dstrect, GDIPCONST RectF* srcrect, Unit unit, GraphicsContainer* state);
Status  GdipBeginContainerI(Graphics* graphics, GDIPCONST Rect* dstrect, GDIPCONST Rect* srcrect, Unit unit, GraphicsContainer* state);
Status  GdipBeginContainer2(Graphics* graphics, GraphicsContainer* state);
Status  GdipEndContainer(Graphics* graphics, GraphicsContainer state);
#if 0
Status GdipGetMetafileHeaderFromWmf(HMETAFILE hWmf, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, MetafileHeader* header);
Status  GdipGetMetafileHeaderFromEmf(HENHMETAFILE hEmf, MetafileHeader* header);
Status  GdipGetMetafileHeaderFromFile(GDIPCONST WCHAR* filename, MetafileHeader* header);
Status  GdipGetMetafileHeaderFromStream(IStream* stream, MetafileHeader* header);
Status  GdipGetMetafileHeaderFromMetafile(Metafile* metafile, MetafileHeader* header);
Status  GdipGetHemfFromMetafile(Metafile* metafile, HENHMETAFILE* hEmf);
Status  GdipCreateStreamOnFile(GDIPCONST WCHAR* filename, UINT access, IStream** stream);
Status  GdipCreateMetafileFromWmf(HMETAFILE hWmf, BOOL deleteWmf, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, Metafile** metafile);
Status  GdipCreateMetafileFromEmf(HENHMETAFILE hEmf, BOOL deleteEmf, Metafile** metafile);
Status  GdipCreateMetafileFromFile(GDIPCONST WCHAR* file, Metafile** metafile);
Status  GdipCreateMetafileFromWmfFile(GDIPCONST WCHAR* file, GDIPCONST WmfPlaceableFileHeader* wmfPlaceableFileHeader, Metafile** metafile);
Status  GdipCreateMetafileFromStream(IStream* stream, Metafile** metafile);
Status  GdipRecordMetafile(HDC referenceHdc, EmfType type, GDIPCONST RectF* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, Metafile** metafile);
Status  GdipRecordMetafileI(HDC referenceHdc, EmfType type, GDIPCONST Rect* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, Metafile** metafile);
Status  GdipRecordMetafileFileName(GDIPCONST WCHAR* fileName, HDC referenceHdc, EmfType type, GDIPCONST RectF* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, Metafile** metafile);
Status  GdipRecordMetafileFileNameI(GDIPCONST WCHAR* fileName, HDC referenceHdc, EmfType type, GDIPCONST Rect* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, Metafile** metafile);
Status  GdipRecordMetafileStream(IStream* stream, HDC referenceHdc, EmfType type, GDIPCONST RectF* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, Metafile** metafile);
Status  GdipRecordMetafileStreamI(IStream* stream, HDC referenceHdc, EmfType type, GDIPCONST Rect* frameRect, MetafileFrameUnit frameUnit, GDIPCONST WCHAR* description, Metafile** metafile);
Status  GdipSetMetafileDownLevelRasterizationLimit(Metafile* metafile, UINT metafileRasterizationLimitDpi);
Status  GdipGetMetafileDownLevelRasterizationLimit(GDIPCONST Metafile* metafile, UINT* metafileRasterizationLimitDpi);
#endif
Status  GdipGetImageDecodersSize(UINT* numDecoders, UINT* size);
Status  GdipGetImageDecoders(UINT numDecoders, UINT size, ImageCodecInfo* decoders);
Status  GdipGetImageEncodersSize(UINT* numEncoders, UINT* size);
Status  GdipGetImageEncoders(UINT numEncoders, UINT size, ImageCodecInfo* encoders);
Status  GdipComment(Graphics* graphics, UINT sizeData, GDIPCONST BYTE* data);
//----------------------------------------------------------------------------
// FontFamily APIs
//----------------------------------------------------------------------------
Status  GdipCreateFontFamilyFromName(GDIPCONST WCHAR* name, FontCollection* fontCollection, FontFamily** FontFamily);
Status  GdipDeleteFontFamily(FontFamily* FontFamily);
Status  GdipCloneFontFamily(FontFamily* FontFamily, FontFamily** clonedFontFamily);
Status  GdipGetGenericFontFamilySansSerif(FontFamily** nativeFamily);
Status  GdipGetGenericFontFamilySerif(FontFamily** nativeFamily);
Status  GdipGetGenericFontFamilyMonospace(FontFamily** nativeFamily);
Status  GdipGetFamilyName(GDIPCONST FontFamily* family, WCHAR name[LF_FACESIZE], LANGID language);
Status  GdipIsStyleAvailable(GDIPCONST FontFamily* family, INT style, BOOL* IsStyleAvailable);
Status  GdipFontCollectionEnumerable(FontCollection* fontCollection, Graphics* graphics, INT* numFound);
Status  GdipFontCollectionEnumerate(FontCollection* fontCollection, INT numSought, FontFamily* gpfamilies[], INT* numFound, Graphics* graphics);
Status  GdipGetEmHeight(GDIPCONST FontFamily* family, INT style, UINT16* EmHeight);
Status  GdipGetCellAscent(GDIPCONST FontFamily* family, INT style, UINT16* CellAscent);
Status  GdipGetCellDescent(GDIPCONST FontFamily* family, INT style, UINT16* CellDescent);
Status  GdipGetLineSpacing(GDIPCONST FontFamily* family, INT style, UINT16* LineSpacing);
//----------------------------------------------------------------------------
// Font APIs
//----------------------------------------------------------------------------
Status  GdipCreateFontFromDC(HDC hdc, Font** font);
Status  GdipCreateFontFromLogfontA(HDC hdc, GDIPCONST LOGFONTA* logfont, Font** font);
Status  GdipCreateFontFromLogfontW(HDC hdc, GDIPCONST LOGFONTW* logfont, Font** font);
Status  GdipCreateFont(GDIPCONST FontFamily* fontFamily, REAL emSize, INT style, Unit unit, Font** font);
Status  GdipCloneFont(Font* font, Font** cloneFont);
Status  GdipDeleteFont(Font* font);
Status  GdipGetFamily(Font* font, FontFamily** family);
Status  GdipGetFontStyle(Font* font, INT* style);
Status  GdipGetFontSize(Font* font, REAL* size);
Status  GdipGetFontUnit(Font* font, Unit* unit);
Status  GdipGetFontHeight(GDIPCONST Font* font, GDIPCONST Graphics* graphics, REAL* height);
Status  GdipGetFontHeightGivenDPI(GDIPCONST Font* font, REAL dpi, REAL* height);
Status  GdipGetLogFontA(Font* font, Graphics* graphics, LOGFONTA* logfontA);
Status  GdipGetLogFontW(Font* font, Graphics* graphics, LOGFONTW* logfontW);
Status  GdipNewInstalledFontCollection(FontCollection** fontCollection);
Status  GdipNewPrivateFontCollection(FontCollection** fontCollection);
Status  GdipDeletePrivateFontCollection(FontCollection** fontCollection);
Status  GdipGetFontCollectionFamilyCount(FontCollection* fontCollection, INT* numFound);
Status  GdipGetFontCollectionFamilyList(FontCollection* fontCollection, INT numSought, FontFamily* gpfamilies[], INT* numFound);
Status  GdipPrivateAddFontFile(FontCollection* fontCollection, GDIPCONST WCHAR* filename);
Status  GdipPrivateAddMemoryFont(FontCollection* fontCollection, GDIPCONST void* memory, INT length);
//----------------------------------------------------------------------------
// Text APIs
//----------------------------------------------------------------------------
Status  GdipDrawString(Graphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST Font* font, GDIPCONST RectF* layoutRect, GDIPCONST StringFormat* stringFormat, GDIPCONST Brush* brush);
Status  GdipMeasureString(Graphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST Font* font, GDIPCONST RectF* layoutRect, GDIPCONST StringFormat* stringFormat, RectF* boundingBox, INT* codepointsFitted, INT* linesFilled);
#if 0
Status
 GdipMeasureCharacterRanges(Graphics* graphics, GDIPCONST WCHAR* string, INT length, GDIPCONST Font* font, GDIPCONST RectF& layoutRect, GDIPCONST StringFormat* stringFormat, INT regionCount, Region** regions);
#endif
Status  GdipDrawDriverString(Graphics* graphics, GDIPCONST UINT16* text, INT length, GDIPCONST Font* font, GDIPCONST Brush* brush, GDIPCONST PointF* positions, INT flags, GDIPCONST Matrix* matrix);
Status  GdipMeasureDriverString(Graphics* graphics, GDIPCONST UINT16* text, INT length, GDIPCONST Font* font, GDIPCONST PointF* positions, INT flags, GDIPCONST Matrix* matrix, RectF* boundingBox);
//----------------------------------------------------------------------------
// String format APIs
//----------------------------------------------------------------------------
Status  GdipCreateStringFormat(INT formatAttributes, LANGID language, StringFormat** format);
Status  GdipStringFormatGetGenericDefault(StringFormat** format);
Status  GdipStringFormatGetGenericTypographic(StringFormat** format);
Status  GdipDeleteStringFormat(StringFormat* format);
Status  GdipCloneStringFormat(GDIPCONST StringFormat* format, StringFormat** newFormat);
Status  GdipSetStringFormatFlags(StringFormat* format, INT flags);
Status  GdipGetStringFormatFlags(GDIPCONST StringFormat* format, INT* flags);
Status  GdipSetStringFormatAlign(StringFormat* format, StringAlignment align);
Status  GdipGetStringFormatAlign(GDIPCONST StringFormat* format, StringAlignment* align);
Status  GdipSetStringFormatLineAlign(StringFormat* format, StringAlignment align);
Status  GdipGetStringFormatLineAlign(GDIPCONST StringFormat* format, StringAlignment* align);
Status  GdipSetStringFormatTrimming(StringFormat* format, StringTrimming trimming);
Status  GdipGetStringFormatTrimming(GDIPCONST StringFormat* format, StringTrimming* trimming);
Status  GdipSetStringFormatHotkeyPrefix(StringFormat* format, INT hotkeyPrefix);
Status  GdipGetStringFormatHotkeyPrefix(GDIPCONST StringFormat* format, INT* hotkeyPrefix);
Status  GdipSetStringFormatTabStops(StringFormat* format, REAL firstTabOffset, INT count, GDIPCONST REAL* tabStops);
Status  GdipGetStringFormatTabStops(GDIPCONST StringFormat* format, INT count, REAL* firstTabOffset, REAL* tabStops);
Status  GdipGetStringFormatTabStopCount(GDIPCONST StringFormat* format, INT* count);
Status  GdipSetStringFormatDigitSubstitution(StringFormat* format, LANGID language, StringDigitSubstitute substitute);
Status  GdipGetStringFormatDigitSubstitution(GDIPCONST StringFormat* format, LANGID* language, StringDigitSubstitute* substitute);
Status  GdipGetStringFormatMeasurableCharacterRangeCount(GDIPCONST StringFormat* format, INT* count);
Status  GdipSetStringFormatMeasurableCharacterRanges(StringFormat* format, INT rangeCount, GDIPCONST CharacterRange* ranges);
//----------------------------------------------------------------------------
// Cached Bitmap APIs
//----------------------------------------------------------------------------
Status  GdipCreateCachedBitmap(Bitmap* bitmap, Graphics* graphics, CachedBitmap** cachedBitmap);
Status  GdipDeleteCachedBitmap(CachedBitmap* cachedBitmap);
Status  GdipDrawCachedBitmap(Graphics* graphics, CachedBitmap* cachedBitmap, INT x, INT y);
UINT  GdipEmfToWmfBits(HENHMETAFILE hemf, UINT cbData16, LPBYTE pData16, INT iMapMode, INT eFlags);
Status  GdipSetImageAttributesCachedBackground(ImageAttributes* imageattr, BOOL enableFlag);
Status  GdipTestControl(GpTestControlEnum control, void* param);
Status WINAPI GdiplusNotificationHook(OUT ULONG_PTR* token);
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

