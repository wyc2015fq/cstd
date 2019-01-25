
#ifndef _STD_GDI_C_H_
#define _STD_GDI_C_H_

#include "types_c.h"
#include "mem_c.h"
#include "geo_c.h"
#include "algo.h"
#include "bitmap_c.h"
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
// Various hatch styles
//--------------------------------------------------------------------------

#define HATCHSTYLEDEF_DEF(HATCHSTYLEDEF) \
  HATCHSTYLEDEF( 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, BackwardDiagonal, "从右上到左下的对角线的线条图案。" ) \
  HATCHSTYLEDEF( 0x08, 0x08, 0x08, 0xff, 0x08, 0x08, 0x08, 0x08, Cross, "指定交叉的水平线和垂直线。" ) \
  HATCHSTYLEDEF( 0x99, 0x33, 0x66, 0xcc, 0x99, 0x33, 0x66, 0xcc, DarkDownwardDiagonal, "指定从顶点到底点向右倾斜的对角线，其两边夹角比 ForwardDiagonal 小 50%，宽度是其两倍。此阴影图案不是锯齿消除的。" ) \
  HATCHSTYLEDEF( 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, DarkHorizontal, "指定水平线的两边夹角比 Horizontal 小 50% 并且宽度是 Horizontal 的两倍。" ) \
  HATCHSTYLEDEF( 0xcc, 0x66, 0x33, 0x99, 0xcc, 0x66, 0x33, 0x99, DarkUpwardDiagonal, "指定从顶点到底点向左倾斜的对角线，其两边夹角比 BackwardDiagonal 小 50%，宽度是其两倍，但这些直线不是锯齿消除的。" ) \
  HATCHSTYLEDEF( 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, DarkVertical, "指定垂直线的两边夹角比 Vertical 小 50% 并且宽度是其两倍。" ) \
  HATCHSTYLEDEF( 0x00, 0x11, 0x22, 0x44, 0x88, 0x00, 0x00, 0x00, DashedDownwardDiagonal, "指定虚线对角线，这些对角线从顶点到底点向右倾斜。" ) \
  HATCHSTYLEDEF( 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, DashedHorizontal, "指定虚线水平线。" ) \
  HATCHSTYLEDEF( 0x00, 0x88, 0x44, 0x22, 0x11, 0x00, 0x00, 0x00, DashedUpwardDiagonal, "指定虚线对角线，这些对角线从顶点到底点向左倾斜。" ) \
  HATCHSTYLEDEF( 0x20, 0x20, 0x20, 0x20, 0x02, 0x02, 0x02, 0x02, DashedVertical, "指定虚线垂直线。" ) \
  HATCHSTYLEDEF( 0x81, 0x42, 0x24, 0x18, 0x08, 0x04, 0x02, 0x01, DiagonalBrick, "指定具有分层砖块外观的阴影，它从顶点到底点向左倾斜。" ) \
  HATCHSTYLEDEF( 0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81, DiagonalCross, "交叉对角线的图案。" ) \
  HATCHSTYLEDEF( 0x00, 0x01, 0x02, 0x01, 0x00, 0x20, 0x10, 0x20, Divot, "指定具有草皮层外观的阴影。" ) \
  HATCHSTYLEDEF( 0x11, 0x00, 0x40, 0x00, 0x11, 0x00, 0x04, 0x00, DottedDiamond, "指定互相交叉的正向对角线和反向对角线，每一对角线都是由点构成的。" ) \
  HATCHSTYLEDEF( 0x55, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, DottedGrid, "指定互相交叉的水平线和垂直线，每一直线都是由点构成的。" ) \
  HATCHSTYLEDEF( 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, ForwardDiagonal, "从左上到右下的对角线的线条图案。" ) \
  HATCHSTYLEDEF( 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, Horizontal, "水平线的图案。" ) \
  HATCHSTYLEDEF( 0x01, 0xff, 0x10, 0x10, 0x10, 0xff, 0x01, 0x01, HorizontalBrick, "指定具有水平分层砖块外观的阴影。" ) \
  HATCHSTYLEDEF( 0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0, LargeCheckerBoard, "指定具有棋盘外观的阴影，棋盘所具有的方格大小是 SmallCheckerBoard 大小的两倍。" ) \
  HATCHSTYLEDEF( 0x1b, 0x18, 0x81, 0xb1, 0x36, 0x06, 0x60, 0x63, LargeConfetti, "指定具有五彩纸屑外观的阴影，并且它是由比 SmallConfetti 更大的片构成的。" ) \
  HATCHSTYLEDEF( 0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81, LargeGrid, "指定阴影样式 Cross。" ) \
  HATCHSTYLEDEF( 0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88, LightDownwardDiagonal, "" ) \
  HATCHSTYLEDEF( 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, LightHorizontal, "指定水平线，其两边夹角比 Horizontal 小 50%。" ) \
  HATCHSTYLEDEF( 0x88, 0x44, 0x22, 0x11, 0x88, 0x44, 0x22, 0x11, LightUpwardDiagonal, "指定从顶点到底点向左倾斜的对角线，其两边夹角比 BackwardDiagonal 小 50%，但这些直线不是锯齿消除的。" ) \
  HATCHSTYLEDEF( 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, LightVertical, "指定垂直线的两边夹角比 Vertical 小 50%。" ) \
  HATCHSTYLEDEF( 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, NarrowHorizontal, "指定水平线的两边夹角比阴影样式 Horizontal 小 75%（或者比 LightHorizontal 小 25%）。" ) \
  HATCHSTYLEDEF( 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, NarrowVertical, "指定垂直线的两边夹角比阴影样式 Vertical 小 75%（或者比 LightVertical 小 25%）。" ) \
  HATCHSTYLEDEF( 0x11, 0x0a, 0x04, 0x0a, 0x11, 0xa0, 0x40, 0xa0, OutlinedDiamond, "指定互相交叉的正向对角线和反向对角线，但这些对角线不是锯齿消除的。" ) \
  HATCHSTYLEDEF( 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x80, Percent05, "指定 5% 阴影。前景色与背景色的比例为 5:100。" ) \
  HATCHSTYLEDEF( 0x00, 0x02, 0x00, 0x88, 0x00, 0x20, 0x00, 0x88, Percent10, "指定 10% 阴影。前景色与背景色的比例为 10:100。 " ) \
  HATCHSTYLEDEF( 0x00, 0x22, 0x00, 0xcc, 0x00, 0x22, 0x00, 0xcc, Percent20, "指定 20% 阴影。前景色与背景色的比例为 20:100。" ) \
  HATCHSTYLEDEF( 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, 0x00, 0xcc, Percent25, "指定 25% 阴影。前景色与背景色的比例为 25:100。" ) \
  HATCHSTYLEDEF( 0x00, 0xcc, 0x04, 0xcc, 0x00, 0xcc, 0x40, 0xcc, Percent30, "指定 30% 阴影。前景色与背景色的比例为 30:100。" ) \
  HATCHSTYLEDEF( 0x44, 0xcc, 0x22, 0xcc, 0x44, 0xcc, 0x22, 0xcc, Percent40, "指定 40% 阴影。前景色与背景色的比例为 40:100。" ) \
  HATCHSTYLEDEF( 0x55, 0xcc, 0x55, 0xcc, 0x55, 0xcc, 0x55, 0xcc, Percent50, "指定 50% 阴影。前景色与背景色的比例为 50:100。" ) \
  HATCHSTYLEDEF( 0x55, 0xcd, 0x55, 0xee, 0x55, 0xdc, 0x55, 0xee, Percent60, "指定 60% 阴影。前景色与背景色的比例为 60:100。" ) \
  HATCHSTYLEDEF( 0x55, 0xdd, 0x55, 0xff, 0x55, 0xdd, 0x55, 0xff, Percent70, "指定 70% 阴影。前景色与背景色的比例为 70:100。" ) \
  HATCHSTYLEDEF( 0x55, 0xff, 0x55, 0xff, 0x55, 0xff, 0x55, 0xff, Percent75, "指定 75% 阴影。前景色与背景色的比例为 75:100。" ) \
  HATCHSTYLEDEF( 0x55, 0xff, 0x59, 0xff, 0x55, 0xff, 0x99, 0xff, Percent80, "指定 80% 阴影。前景色与背景色的比例为 80:100。" ) \
  HATCHSTYLEDEF( 0x77, 0xff, 0xdd, 0xff, 0x77, 0xff, 0xfd, 0xff, Percent90, "指定 90% 阴影。前景色与背景色的比例为 90:100。" ) \
  HATCHSTYLEDEF( 0x0f, 0x0f, 0x0f, 0x0f, 0x55, 0xaa, 0x55, 0xaa, Plaid, "指定具有格子花呢材料外观的阴影。" ) \
  HATCHSTYLEDEF( 0x01, 0x02, 0x02, 0x03, 0x84, 0x48, 0x30, 0xc0, Shingle, "指定带有对角分层鹅卵石外观的阴影，它从顶点到底点向右倾斜。" ) \
  HATCHSTYLEDEF( 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc, SmallCheckerBoard, "指定带有棋盘外观的阴影。" ) \
  HATCHSTYLEDEF( 0x01, 0x20, 0x02, 0x10, 0x80, 0x04, 0x40, 0x08, SmallConfetti, "指定带有五?手叫纪夤鄣囊跤?" ) \
  HATCHSTYLEDEF( 0xff, 0x22, 0x22, 0x22, 0xff, 0x22, 0x22, 0x22, SmallGrid, "指定互相交叉的水平线和垂直线，其两边夹角比阴影样式 Cross 小 50%。" ) \
  HATCHSTYLEDEF( 0x08, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c, 0x08, 0x00, SolidDiamond, "指定具有对角放置的棋盘外观的阴影。" ) \
  HATCHSTYLEDEF( 0xc7, 0xc7, 0xbb, 0x64, 0x7c, 0x7c, 0xbb, 0x46, Sphere, "指定具有球体彼此相邻放置的外观的阴影。" ) \
  HATCHSTYLEDEF( 0xff, 0x66, 0xff, 0x99, 0xff, 0x66, 0xff, 0x99, Trellis, "指定具有格架外观的阴影。" ) \
  HATCHSTYLEDEF( 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, Vertical, "垂直线的图案。" ) \
  HATCHSTYLEDEF( 0x30, 0x49, 0x06, 0x00, 0x30, 0x49, 0x06, 0x00, Wave, "指定由代字号“~”构成的水平线。" ) \
  HATCHSTYLEDEF( 0x11, 0xa2, 0x44, 0x8a, 0x11, 0xa8, 0x44, 0x0a, Weave, "指定具有织物外观的阴影。" ) \
  HATCHSTYLEDEF( 0xc1, 0x83, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, WideDownwardDiagonal, "指定从顶点到底点向右倾斜的对角线，其间距与阴影样式 ForwardDiagonal 相同，宽度是其三倍，但它们不是锯齿消除的。" ) \
  HATCHSTYLEDEF( 0xe0, 0x70, 0x38, 0x1c, 0x0e, 0x07, 0x83, 0xc1, WideUpwardDiagonal, "指定从顶点到底点向左倾斜的对角线，其间距与阴影样式 BackwardDiagonal 相同，宽度是其三倍，但它们不是锯齿消除的。" ) \
  HATCHSTYLEDEF( 0x48, 0x84, 0x03, 0x30, 0x48, 0x84, 0x03, 0x30, ZigZag, "指定由 Z 字形构成的水平线。" )




//#define HATCHSTYLEDEF(data0, data1, data2, data3, data4, data5, data6, data7, name, str) const uchar Hatch##name [] = {data0, data1, data2, data3, data4, data5, data6, data7};
//HATCHSTYLE_DEF_DEF(HATCHSTYLEDEF)
//#undef HATCHSTYLEDEF

typedef enum {
#define HATCHSTYLE_DEF(data0, data1, data2, data3, data4, data5, data6, data7, name, str) HatchStyle##name, 
  HATCHSTYLEDEF_DEF(HATCHSTYLE_DEF)
#undef HATCHSTYLE_DEF
  HatchStyleTotal, // must be after all unique hatch styles
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
  LineCapButt = 0,
  LineCapSquare = 1,
  LineCapRound = 2,
  LineCapTriangle = 3,
  LineCapNoAnchor, // corresponds to flat cap
  LineCapSquareAnchor, // corresponds to square cap
  LineCapRoundAnchor, // corresponds to round cap
  LineCapDiamondAnchor, // corresponds to triangle cap
  LineCapArrowAnchor, // no correspondence
  LineCapCustom, // custom cap
                 //LineCapAnchorMask = 0xf0 // mask to check for anchor or not.
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
  LineJoinMiterClipped = 3,
  LineJoinMiterRevert = 4,
  LineJoinMiterRound = 5,
  InnerJoinMiter = LineJoinMiter,
  InnerJoinBevel = LineJoinBevel,
  InnerJoinRound = LineJoinRound,
  InnerJoinJag,
} LineJoin;
typedef LineJoin InnerJoin;
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
                                    PathPointTypeBezier2 = 2,    // quadratic Beizer
                                    PathPointTypeBezier3 = 3,    // cubic Bezier
                                    PathPointTypeBezier4 = 4,    // quartic (4th order) Beizer
                                    PathPointTypeBezier5 = 5,    // quintic (5th order) Bezier
                                    PathPointTypeBezier6 = 6     // hexaic (6th order) Bezier
} PathPointType;
#define m_path_moveto(x1, y1)   PathPointTypeStart, x1, y1
#define m_path_lineto(x1, y1)   PathPointTypeLine, x1, y1
#define m_path_linetoc(x1, y1)  PathPointTypeLine|PathPointTypeCloseSubpath, x1, y1
#define m_path_curve3(x1, y1, x2, y2)  PathPointTypeBezier2, x1, y1, PathPointTypeLine, x2, y2
#define m_path_curve3c(x1, y1, x2, y2)  PathPointTypeBezier2, x1, y1, PathPointTypeLine|PathPointTypeCloseSubpath, x2, y2
typedef struct path_point {
  int cmd;
  float x, y;
} path_point;

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
} ImageType1;
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
  PenAlignmentCenter,
  PenAlignmentInset,
  PenAlignmentOutset,
  PenAlignmentLeft,
  PenAlignmentRight,
} PenAlignment;
//--------------------------------------------------------------------------
// Brush types
//--------------------------------------------------------------------------
typedef enum {
  BrushTypeSolidColor = 0,
  BrushTypeHatchFill = 1,
  BrushTypeTextureFill = 2,
  BrushTypePathGradient = 3,
  BrushTypeLinearGradient = 4,
  PenTypeSolidColor = BrushTypeSolidColor,
  PenTypeHatchFill = BrushTypeHatchFill,
  PenTypeTextureFill = BrushTypeTextureFill,
  PenTypePathGradient = BrushTypePathGradient,
  PenTypeLinearGradient = BrushTypeLinearGradient,
  PenTypeUnknown = -1
} BrushType;
//--------------------------------------------------------------------------
// Pen's Fill types
//--------------------------------------------------------------------------
typedef BrushType PenType;


typedef struct {
  //Path path[1];
  BOOL fill;      /* TRUE for fill, FALSE for stroke */
  LineCap cap;  /* as far as I can tell, this value is ignored */
  float inset;     /* how much to adjust the end of the line */
  LineJoin join;
  float scale;
} CustomLineCap;

typedef struct {
  float lineWidth;
  float miterLimit;

  float innerMiterLimit;
  float approxScale;

  float dashPhase;
  float dashPattern[32];
  int numdash;
  DashStyle dashStyle;
  PenAlignment penAlignment;
  InnerJoin innerJoin;
  LineJoin lineJoin;
  LineCap startCap, endCap;
  DashCap dashCap;
} PenStyle;
CC_INLINE void PenStyle_Init(PenStyle* penstyle) {
  penstyle->startCap = penstyle->endCap = LineCapRound;
  penstyle->lineJoin = LineJoinRound;
  penstyle->innerJoin = InnerJoinRound;
  penstyle->penAlignment = PenAlignmentInset;
  penstyle->penAlignment = PenAlignmentCenter;
  penstyle->dashStyle = DashStyleSolid;
  penstyle->lineWidth = 1.f;
  penstyle->miterLimit = 4.f;
  penstyle->dashPhase = 0;
  penstyle->numdash = 0;

  penstyle->innerMiterLimit = (1.01f);
  penstyle->approxScale = (1.0);
}
CC_INLINE void PenStyleSetLineDash(PenStyle* penstyle, float dashPhase, const float* dashPattern, int numdash) {
  if (dashPattern && numdash>0) {
    penstyle->dashStyle = DashStyleCustom;
    penstyle->numdash = numdash;
    penstyle->dashPhase = -dashPhase;
    numdash = MIN((int)countof(penstyle->dashPattern), numdash);
    MEMCPY(penstyle->dashPattern, dashPattern, numdash);
  }
}
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
  PaletteFlagsHasAlpha = 0x0001,
  PaletteFlagsGrayScale = 0x0002,
  PaletteFlagsHalftone = 0x0004
} PaletteFlags;
typedef struct {
  UINT Flags; // Palette flags
  UINT Count; // Number of color entries
  ARGB Entries[1]; // Palette color entries
} ColorPalette;
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
  PixFmt PixelFormat;
  uchar* Scan0;
  UINT Reserved;
} BitmapData;
// Image flags
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
  float m[5][5];
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

//----------------------------------------------------------------------------
// Color Map
//----------------------------------------------------------------------------
typedef struct {
  Color oldColor;
  Color newColor;
} ColorMap;
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

typedef struct {
  INT First, Length;
} CharacterRange;
typedef struct {
  INT attr;
  StringAlignment align;
  StringTrimming trimming;
  HotkeyPrefix hkprefix;
  StringAlignment vertalign;
  StringDigitSubstitute digitsub;
  INT tabcount;
  float firsttab;
  float *tabs;
  CharacterRange *character_ranges;
  INT range_count;
  BOOL generic_typographic;
} StringFormat;

// Shift count and bit mask for A, R, G, B components
enum {
  AlphaShift = 24,
  RedShift = 16,
  GreenShift = 8,
  BlueShift = 0
};
enum {
  AlphaMask = 0xff000000,
  RedMask = 0x00ff0000,
  GreenMask = 0x0000ff00,
  BlueMask = 0x000000ff
};
// Assemble A, R, G, B values into a 32-bit integer
#define MakeARGB(a,r,g,b) (((ARGB) (b) << BlueShift) | ((ARGB) (g) << GreenShift) | ((ARGB) (r) << RedShift) | ((ARGB) (a) << AlphaShift))
#if 0
#define GetAlpha(Argb) (BYTE) (Argb >> AlphaShift)
#define GetRed(Argb) (BYTE) (Argb >> RedShift)
#define GetGreen(Argb) (BYTE) (Argb >> GreenShift)
#define GetBlue(Argb) (BYTE) (Argb)
#endif
#define SetFromCOLORREF(rgb) MakeARGB(255, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb))
#define ToCOLORREF(Argb) _RGB(GetRed(Argb), GetGreen(Argb), GetBlue(Argb))
CC_INLINE COLOR Blend(const COLOR bg, const COLOR src)
{
  COLOR a = src >> 24;

  if (0 == a) {
    return bg;
  }
  else {
    COLOR rb = (((src & 0x00ff00ff) * a) + ((bg & 0x00ff00ff) * (0xff - a))) & 0xff00ff00;
    COLOR g = (((src & 0x0000ff00) * a) + ((bg & 0x0000ff00) * (0xff - a))) & 0x00ff0000;
    return (src & 0xff000000) | ((rb | g) >> 8);
  }
}
CC_INLINE COLOR PixelAlpha2(COLOR srcPixel, int percent)
{
  // My formula for calculating the transpareny is as
  // follows (for each single color):
  //
  // percent
  // destPixel = sourcePixel * ( ------- )
  // 100
  //
  // This is not real alpha blending, as it only modifies the brightness,
  // but not the color (a real alpha blending had to mix the source and
  // destination pixels, e.g. mixing green and red makes yellow).
  // For our nice "menu" shadows its good enough.
  COLOR clrFinal = _RGB(
    MIN(255, (GetRV(srcPixel) * percent) / 100),
    MIN(255, (GetGV(srcPixel) * percent) / 100),
    MIN(255, (GetBV(srcPixel) * percent) / 100));
  // TRACE ("%d %d %d\n", GetRValue (clrFinal), GetGValue (clrFinal), GetBValue (clrFinal));
  return (clrFinal);
}
CC_INLINE int SetAlphaPixel(COLOR* pBits, int al, int x, int y, int percent, COLOR clrBase, BOOL bIsRight)
{
  COLOR* pColor;
  pColor = (COLOR*)(bIsRight ? (pBits + (al) * (y + 1) - x) : (pBits + (al)* y + x));
  *pColor = PixelAlpha2(*pColor, percent);

  if (clrBase == 0) {
    return 0;
  }

  *pColor = _RGB(MIN(255, (3 * GetRV(*pColor) + GetBV(clrBase)) / 4),
    MIN(255, (3 * GetGV(*pColor) + GetGV(clrBase)) / 4),
    MIN(255, (3 * GetBV(*pColor) + GetRV(clrBase)) / 4));
  return 0;
}

/////////////////////////////////////////////////////////////////////

#define TT_LINE       0
#define TT_QSPLINE    1
#define TT_CSPLINE    2

typedef struct {
  char x;
  char y;
} pointi8;

typedef struct {
  uchar type;
  uchar  npt;
  pointi8 pt[1];
} ttpoly;

typedef struct {
  ushort cb;
  pointi8 start;
} ttpolygon;


typedef struct {
  uchar   gmBlackBoxX;
  uchar   gmBlackBoxY;
  pointi8 gmptGlyphOrigin;
  short   gmCellIncX;
  short   gmCellIncY;
} glyphmetrics;

typedef struct {
  float size;
  float* IndexXAdvance;
  int IndexXAdvance_Size;
  float FallbackXAdvance;
} FontInfo;

typedef struct ttffont_t_ ttffont_t;
struct ttffont_t_ {
  void* x;
  char name[32];
  FontInfo info[1];
  int(*get_glyph)(const ttffont_t* f, uint code, char* obuf, int olen);
  int(*del)(ttffont_t* f);
};
#define ttffont_get_glyph(f, code, obuf, olen)  (f)->get_glyph(f, code, obuf, olen)
#define ttffont_del(f)  (f)->del(f)
///////////////////////////////////////////////////////////

typedef FPOINT2 ImVec2;

#define CFONT_BIT_INIT(fb, PIX, BX, BY, X, Y, S) ((fb)->bit=PIX, (fb)->bx=BX, (fb)->by=BY, (fb)->x=X, (fb)->y=Y, (fb)->step=S)
#define CMAXFONT 8
#define CFONT_BOLD 1<<0
#define CFONT_ITALIC 1<<1
#define CFONT_BOLDITALIC 1<<2
#define CFONT_UNDERLINE 1<<3
#define CFONT_STRIKEOUT 1<<4
#define MAXNAMELEN 256
typedef struct font_bit_t1 {
  int bx, by; // 盒子 box_x box_y
  int x, y; // 原点
  int step;
} font_bit_t1;
typedef struct {
  short x, y, cx, cy;
  unsigned char type, padding;
} ttf_vertex;
struct font_t;
#define FONT_FUN_DEF(DEF) \
  DEF(int, Release, (const struct font_t* fo)) \
DEF(int,GetCodepointShape, (const font_t* fo, int unicode_codepoint, ttf_vertex** vertices)) \
DEF(float,ScaleForPixelHeight, (const font_t* fo, float height)) \
DEF(void,GetFontVMetrics,(const font_t* fo, int* ascent, int* descent, int* lineGap)) \
DEF(void,GetCodepointHMetrics,(const struct font_t* fo, int codepoint, int* advanceWidth, int* leftSideBearing)) \
DEF(void,GetCodepointBitmapBoxSubpixel,(const font_t* font, int codepoint, float shift_x, float shift_y, int* ix0, int* iy0, int* ix1, int* iy1)) \
DEF(void,MakeCodepointBitmapSubpixel,(const font_t* fo, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint))
typedef struct font_fun_t {
#define FONT_FUN_DEF_DEF(RET, NAME, ARGS)  RET (*NAME)ARGS;
  FONT_FUN_DEF(FONT_FUN_DEF_DEF)
#undef FONT_FUN_DEF_DEF
} font_fun_t;
typedef struct font_t {
  struct font_fun_t* fun;
} font_t;
typedef struct {
  ushort Codepoint;
  float XAdvance;
  float X0, Y0, X1, Y1;
  float U0, V0, U1, V1; // Texture coordinates
} font_glyph_t;
typedef struct {
  void* FontData;
  int FontDataSize; // TTF data size
                    //bool FontDataOwnedByAtlas; // true // TTF data ownership taken by the container ImFontAtlas (will delete memory itself). Set to true
  int FontNo; // 0 // Index of font within TTF file
  float SizePixels; // Size in pixels for rasterizer
  int OversampleH, OversampleV; // 3, 1 // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
  bool PixelSnapH; // false // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
  ImVec2 GlyphExtraSpacing; // 0, 0 // Extra spacing (in pixels) between glyphs
  const ushort* GlyphRanges; // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
  bool MergeMode; // false // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
  bool MergeGlyphCenterV; // false // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline
                          // [Internal]
                          //char Name[32]; // Name (strictly for debugging)
} font_config_t;
typedef struct texfont_t_ texfont_t;
struct texfont_t_ {
  font_glyph_t* Glyphs;
  int Glyphs_Size;
  ushort IndexLookup[1 << 16]; // Sparse. Index glyphs by Unicode code-point.
  int IndexLookup_Size;
  float IndexXAdvance[1 << 16]; // Sparse. Glyphs->XAdvance in a directly indexable way (more cache-friendly, for CalcTextSize functions which are often bottleneck in large UI).
  int IndexXAdvance_Size;
  const font_glyph_t* FallbackGlyph; // == FindGlyph(FontFallbackChar)
  ushort FallbackChar; // = '?' // Replacement glyph if one isn't found. Only set via SetFallbackChar()
                       // Members: Hot ~62/78 bytes
  float size; // <user set> // Height of characters, set during loading (don't change after loading)
  float show_size;
  //float Scale; // = 1.f // Base font scale, multiplied by the per-window font scale which you can adjust with SetFontScale()
  ImVec2 DisplayOffset; // = (0.f,1.f) // Offset font rendering by xx pixels
  float FallbackXAdvance; // == FallbackGlyph->XAdvance
                          // Members: Cold ~18/26 bytes
  short ConfigDataCount; // ~ 1 // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
                         //ImFontAtlas* ContainerAtlas; // What we has been loaded into
  float Ascent, Descent; // Ascent: distance from top to bottom of e.g. 'A' [0..size]
                         // (Access texture data via GetTexData*() calls which will setup a default font for you.)
  texture_t tex[1];
  ImVec2 TexUvWhitePixel;
  //int Fonts_Size;
  //ImFontConfig* ConfigData; // Internal data
  //int ConfigData_Size;
  texfont_t* next, *prev;
};
///////////////////////////////////////////////////////////
// Primitives supported by draw-primitive API
#define GCMODEDEF_DEF(GCMODEDEF) \
GCMODEDEF(GC_POINTS        ) \
GCMODEDEF(GC_LINES         ) \
GCMODEDEF(GC_LINE_STRIP    ) \
GCMODEDEF(GC_TRIANGLES     ) \
GCMODEDEF(GC_TRIANGLE_STRIP) \
GCMODEDEF(GC_TRIANGLE_FAN  ) \
GCMODEDEF(GC_QUADS         ) \
  GCMODEDEF(GC_QUAD_STRIP    ) \
  GCMODEDEF(GC_POLYGON    )

typedef enum {
#define GCMODEDEF(x)   x,
  GCMODEDEF_DEF(GCMODEDEF)
#undef GCMODEDEF
  GC_TYPE_MASK = 0xf,
}GcMode;
//
#define GC_STATE_MASK  (3<<3)
#define GC_WIREFRAME  (0<<3)// 渲染线框
#define GC_TEXTURE    (1<<3)// 渲染纹理
#define GC_COLOR      (2<<3)// 渲染颜色

typedef struct {
  float m[4][4];
} matrix44;
typedef FPOINT4 vector_t;
//typedef vector_t point_t;

typedef struct {
  matrix44 world; // 世界坐标变换
  matrix44 view; // 摄影机坐标变换
  matrix44 projection; // 投影变换
  matrix44 transform; // transform = world * view * projection
  int w, h;
} transform_t;

typedef struct {
  transform_t transform; // 坐标变换器
  int state; // 渲染状态
  int type; // 渲染状态
  uint32 background; // 背景颜色
  uint32 clrfg; // 线框颜色
  int n;
  uint16* idx;
  ImVertex* vtx;
  texture_t* tex;
} info3d_t;

///////////////////////////////////////////////////////////

typedef enum {
  //填充规则，
  GcOptFill = 1,//表示用非零绕数规则
  GcOptStroke = 2, //表示描线
  GcOptEO = 4, // 奇偶填充
  GcOptEOFill = GcOptEO | GcOptFill,//表示用奇偶规则
  GcOptFillStroke = GcOptFill | GcOptStroke, //表示填充+
  GcOptEOFillStroke = GcOptEOFill | GcOptStroke, //表示描线，不是填充
  GcOptFill2 = (SmoothingModeNone << 8) | GcOptFill,
} GcOptType;

#if 1
#define gcfun(g, name)   g->fun->f##name
#else
#define gcfun(g, name)   cmd##name
#endif


//#define gcClip(g)                                                      gcfun(g, Clip))(g)
//#define gcBeginPath(g)                                                 gcfun(g, BeginPath)(g)
//#define gcClosePath(g)                                                 gcfun(g, ClosePath)(g)
//#define gcResetPath(g)                                                 gcfun(g, ResetPath)(g)
//#define gcClosePath(g)                                                 gcfun(g, ClosePath)(g)
//#define gcFill(g)                                                      gcfun(g, Fill)(g)
//#define gcStroke(g)                                                    gcfun(g, Stroke)(g)
//#define gcMoveTo(g, x, y)                                              gcfun(g, MoveTo)(g, x, y)
//#define gcLineTo(g, x, y)                                              gcfun(g, LineTo)(g, x, y)
#define gcPrimRectUV(g, pr, npr, tex)                                    gcfun(g, PrimRectUV)(g, pr, npr, tex)
#define gcSolid(g, opt, clr)                                                  gcfun(g, Solid)(g, opt, clr)
#define gcGradient(g, opt, x0, y0, x1, y1, pos, clrs, count)                  gcfun(g, Gradient)(g, opt, x0, y0, x1, y1, pos, clrs, count)
#define gcRadial(g, opt, x1, y1, rx, ry, pos, clrs, count)                    gcfun(g, Radial)(g, opt, x1, y1, rx, ry, pos, clrs, count)
#define gcHatch(g, opt, hatchstyle, forecol, backcol, x, y)                   gcfun(g, Hatch)(g, opt, hatchstyle, forecol, backcol, x, y)
#define gcPathGradient(g, opt, Points, Types, Count, clrs, nclrs, ptcenter, clrcenter)  gcfun(g, PathGradient)(g, opt, Points, Types, Count, clrs, nclrs, ptcenter, clrcenter)
#define gcPathRadial(g, opt, Points, Types, Count, clrs, ptcenter)            gcfun(g, PathRadial)(g, opt, Points, Types, Count, clrs, ptcenter)
#define gcPattern(g, opt, tex, prcSrc, bgclr, palette, m23)                   gcfun(g, Pattern)(g, opt, tex, prcSrc, bgclr, palette, m23)
#define gcWrapMode(g, wrapX, wrapY)                                      gcfun(g, WrapMode)(g, wrapX, wrapY)
#define gcLineWidth(g, lineWidth)                                        gcfun(g, LineWidth)(g, lineWidth)
//#define gcDashStyle(g, dashStyle)                                        gcfun(g, DashStyle)(g, dashStyle)
#define gcPenAlignment(g, penAlignment)                                  gcfun(g, PenAlignment)(g, penAlignment)
#define gcInnerJoin(g, innerJoin)                                        gcfun(g, InnerJoin)(g, innerJoin)
#define gcLineJoin(g, lineJoin)                                          gcfun(g, LineJoin)(g, lineJoin)
#define gcLineCap(g, startCap, endCap)                                   gcfun(g, LineCap)(g, startCap, endCap)
#define gcLineDash(g, dashPhase, dashPattern, numdash)                   gcfun(g, LineDash)(g, dashPhase, dashPattern, numdash)
//#define gcLineStyle(g, lineStyle)                                        gcfun(g, LineStyle)(g, lineStyle)
#define gcMiterLimit(g, miterLimit)                                      gcfun(g, MiterLimit)(g, miterLimit)
//#define gcFillRect(g, x, y, w, h)                                        gcfun(g, FillRect)(g, x, y, w, h)
//#define gcStrokeRect(g, x, y, w, h)                                      gcfun(g, StrokeRect)(g, x, y, w, h)
//#define gcClearRect(g, x, y, w, h)                                       gcfun(g, ClearRect)(g, x, y, w, h)
#define gcClipRect(g, prc)                                                 gcfun(g, ClipRect)(g, prc)
#define gcPushClipRect(g, prc)                                             gcClipRect(g, prc)
#define gcPopClipRect(g)                                                   gcClipRect(g, NULL)
//#define gcArcTo(g, x1, y1, x2, y2, radius)                               gcfun(g, ArcTo)(g, x1, y1, x2, y2, radius)
//#define gcArc(g, x, y, w, h, startAngle, endAngle)                       gcfun(g, Arc)(g, x, y, w, h, startAngle, endAngle)
//#define gcQuadraticCurveTo(g, cpx, cpy, x, y)                            gcfun(g, QuadraticCurveTo)(g, cpx, cpy, x, y)
//#define gcBezierCurveTo(g, cpx1, cpy1, cpx2, cpy2, x, y)                 gcfun(g, BezierCurveTo)(g, cpx1, cpy1, cpx2, cpy2, x, y)
//#define gcPolygons(g, pt, ln, c, type, step)                             gcfun(g, Polygons)(g, pt, ln, c, type, step)

#define gcRect(g, opt, x, y, w, h)                                       gcfun(g, Rect)(g, opt, x, y, w, h)
#define gcEllipse(g, opt, x, y, w, h)                                    gcfun(g, Ellipse)(g, opt, x, y, w, h)
#define gcPath(g, opt, Points, Types, Count)                             gcfun(g, Path)(g, opt, Points, Types, Count)
#define gcPrimitive(g, info)                                             gcfun(g, Primitive)(g, info)
#define gcQuadImage(g, vtx4, tex)                                        gcfun(g, QuadImage)(g, vtx4, tex)
#define gcRectImage(g, vtx2, tex)                                        gcfun(g, RectImage)(g, vtx2, tex)
#define gcSetDirty(g, rc, reset)                                         gcfun(g, SetDirty)(g, rc, reset)
//#define gcScale(g, scalewidth, scaleheight)                            gcfun(g, Scale)(g, scalewidth, scaleheight)
//#define gcRotate(g, angle)                                             gcfun(g, Rotate)(g, angle)
//#define gcTranslate(g, x, y)                                           gcfun(g, Translate)(g, x, y)
//#define gcTransform(g, a, b, c, d, e, f)                               gcfun(g, Transform)(g, a, b, c, d, e, f)
//#define gcSetTransform(g, a, b, c, d, e, f)                            gcfun(g, SetTransform)(g, a, b, c, d, e, f)
#define gcTextAlign(g, textAlign)                                      gcfun(g, TextAlign)(g, textAlign)
#define gcText(g, opt, text, text_end, font, size, x, y, w, h, fmt)   gcfun(g, Text)(g, opt, text, text_end, font, size, x, y, w, h, fmt)

#define gcFillColor(g, clr) gcSolid(g, GcOptFill, clr)
#define gcPenColor(g, clr) gcSolid(g, GcOptStroke, clr)
#define gcSolidBrush(g, clr)  gcSolid(g, GcOptFill, clr)
#define gcSolidPen(g, clr)  gcSolid(g, GcOptStroke, clr)
#define gcHatchBrush(g, hatchstyle, forecol, backcol, x, y)  gcHatch(g, GcOptFill, hatchstyle, forecol, backcol, x, y)
#define gcHatchPen(g, hatchstyle, forecol, backcol, x, y)  gcHatch(g, GcOptStroke, hatchstyle, forecol, backcol, x, y)
#define gcGradientBrush(g, x0, y0, x1, y1, blendPositions, presetColors, count)  gcGradient(g, GcOptFill, x0, y0, x1, y1, blendPositions, presetColors, count)
#define gcGradientPen(g, x0, y0, x1, y1, blendPositions, presetColors, count)  gcGradient(g, GcOptStroke, x0, y0, x1, y1, blendPositions, presetColors, count)
#define gcRadialBrush(g, x1, y1, rx, ry, blendPositions, presetColors, count)  gcRadial(g, GcOptFill, x1, y1, rx, ry, blendPositions, presetColors, count)
//#define gcRadialPen(g, x1, y1, rx, ry, blendPositions, presetColors, count)  gcRadial(g, GcOptStroke, x1, y1, rx, ry, blendPositions, presetColors, count)

#define gcPathRadialBrush(g, Points, Types, Count, clrs, ptcenter)  gcPathRadial(g, GcOptFill, Points, Types, Count, clrs, ptcenter)
#define gcPathGradientBrush(g, Points, Types, Count, clrs, nclrs, ptcenter, clrcenter)  gcPathGradient(g, GcOptFill, Points, Types, Count, clrs, nclrs, ptcenter, clrcenter)
#define gcPatternBrush(g, tex, prcSrc, bgclr, palette, m23)  gcPattern(g, GcOptFill, tex, prcSrc, bgclr, palette, m23)
#define gcPatternPen(g, tex, prcSrc, bgclr, palette, m23)  gcPattern(g, GcOptStroke, tex, prcSrc, bgclr, palette, m23)

#define gcHatchBrush2(g, hatchstyle, forecol, backcol) gcHatch(g, GcOptFill, hatchstyle, forecol, backcol, 0, 0)
#define gcPathGradientBrush2(g, path, clrs, nclrs, ptcenter, clrcenter)  gcPathGradientBrush(g, path->Points, path->Types, path->Count, clrs, nclrs, ptcenter, clrcenter)
#define gcPathRadialBrush2(g, path, clrs, ptcenter)  gcPathRadialBrush(g, path->Points, path->Types, path->Count, clrs, ptcenter)
#define gcLinearGradientBrush gcGradientBrush

#define gcLineCap1(g, cap)  gcLineCap(g, cap, cap)
//enum {opAddPath, opFill, opStroke};
#if 0
GCFUNDEF(void, BeginPath, (gc_t* g))    \
GCFUNDEF(void, ClosePath, (gc_t* g))    \
GCFUNDEF(void, Fill, (gc_t* g))    \
GCFUNDEF(void, Stroke, (gc_t* g))    \
GCFUNDEF(void, ResetPath, (gc_t* g))    \
GCFUNDEF(void, MoveTo, (gc_t* g, float x, float y))    \
GCFUNDEF(void, LineTo, (gc_t* g, float x, float y))    \
GCFUNDEF(void, FillRect, (gc_t* g, float x, float y, float w, float h)) \
GCFUNDEF(void, StrokeRect, (gc_t* g, float x, float y, float w, float h)) \
GCFUNDEF(void, ClearRect, (gc_t* g, float x, float y, float w, float h)) \
GCFUNDEF(void, ArcTo, (gc_t* g, float x1, float y1, float x2, float y2, float radius)) \
GCFUNDEF(void, Arc, (gc_t* g, float x, float y, float w, float h, float startAngle, float endAngle))\
GCFUNDEF(void, QuadraticCurveTo, (gc_t* g, float cpx, float cpy, float x, float y)) \
GCFUNDEF(void, BezierCurveTo, (gc_t* g, float cpx1, float cpy1, float cpx2, float cpy2, float x, float y)) \
GCFUNDEF(void, Polygons, (gc_t* g, const void* pt, const int* ln, int c, TypeId type, int step)) \
GCFUNDEF(void, Scale, (gc_t* g, float scalewidth, float scaleheight)) \
GCFUNDEF(void, Rotate, (gc_t* g, float angle)) \
GCFUNDEF(void, Translate, (gc_t* g, float x, float y)) \
GCFUNDEF(void, Transform, (gc_t* g, float a, float b, float c, float d, float e, float f)) \
GCFUNDEF(void, SetTransform, (gc_t* g, float a, float b, float c, float d, float e, float f)) \
GCFUNDEF(void, Clip, (gc_t* g))    \
GCFUNDEF(void, DashStyle, (gc_t* g, DashStyle dashStyle)) \

#endif

///////////////////////////////////////////////////////////

#define GCFUNDEF_DEF(GCFUNDEF) \
  GCFUNDEF(void, ClipRect, (gc_t* g, const IRECT* prc)) \
  GCFUNDEF(void, PrimRectUV, (gc_t* g, const PrimRectUV* pr, int npr, texture_t* tex)) \
  GCFUNDEF(void, Solid, (gc_t* g, int opt, COLOR clr)) \
  GCFUNDEF(void, Hatch, (gc_t* g, int opt, HatchStyle hatchstyle, COLOR forecol, COLOR backcol, int x, int y)) \
  GCFUNDEF(void, Gradient, (gc_t* g, int opt, float x0, float y0, float x1, float y1, const float* pos, const COLOR* clrs, int count)) \
  GCFUNDEF(void, Radial, (gc_t* g, int opt, float x1, float y1, float rx, float ry, const float* blendPositions, const COLOR* presetColors, int count)) \
  GCFUNDEF(void, PathGradient, (gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count, const COLOR* clrs, int nclrs, const FPOINT* ptcenter, COLOR clrcenter)) \
  GCFUNDEF(void, PathRadial, (gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count, const COLOR* clrs, const FPOINT* ptcenter)) \
  GCFUNDEF(void, Pattern, (gc_t* g, int opt, const texture_t* tex, const IRECT* prcSrc, COLOR bgclr, const COLOR* palette, const float* m23)) \
  GCFUNDEF(void, WrapMode, (gc_t* g, WrapMode wrapX, WrapMode wrapY)) \
  GCFUNDEF(void, LineWidth, (gc_t* g, float lineWidth)) \
  GCFUNDEF(void, PenAlignment, (gc_t* g, PenAlignment penAlignment)) \
  GCFUNDEF(void, InnerJoin, (gc_t* g, InnerJoin innerJoin)) \
  GCFUNDEF(void, LineJoin, (gc_t* g, LineJoin lineJoin)) \
  GCFUNDEF(void, LineCap, (gc_t* g, LineCap startCap, LineCap endCap)) \
  GCFUNDEF(void, LineDash, (gc_t* g, float dashPhase, const float* dashPattern, int numdash)) \
  GCFUNDEF(void, MiterLimit, (gc_t* g, float miterLimit)) \
  GCFUNDEF(void, Rect, (gc_t* g, int opt, float x, float y, float w, float h)) \
  GCFUNDEF(void, Ellipse, (gc_t* g, int opt, float x, float y, float w, float h)) \
  GCFUNDEF(void, Path, (gc_t* g, int opt, const FPOINT* Points, const BYTE* Types, int Count)) \
  GCFUNDEF(void, Text, (gc_t* g, int opt, const char* text, const char* text_end, font_t* font, float size, float x, float y, float w, float h, int fmt)) \
  GCFUNDEF(void, Primitive, (gc_t* g, info3d_t* info)) \
  GCFUNDEF(void, QuadImage, (gc_t* g, const ImVertex* vtx4, const texture_t* tex)) \
  GCFUNDEF(void, RectImage, (gc_t* g, const ImVertex* vtx2, const texture_t* tex)) \
  GCFUNDEF(void, TextAlign, (gc_t* g, int textAlign)) \
  GCFUNDEF(void, SetDirty, (gc_t* g, IRECT rc, BOOL reset)) \

#define gcWrapMode2(g, w)  gcWrapMode(g, w, w)

typedef struct gc_t gc_t;
typedef struct {
#define GCFUNDEF(RET, NAME, ARG)   RET (*f##NAME) ARG;
  GCFUNDEF_DEF(GCFUNDEF)
#undef GCFUNDEF
} gcfun_t;
struct gc_t {
  char* buf;
  int len;
  void* x;
  gcfun_t* fun;
};
////////////////////////////////////////////////////////////////////////////////////////
#include "path_c.h"
////////////////////////////////////////////////////////////////////////////////////////
CC_INLINE void gcRoundRect(gc_t* g, int opt, float x, float y, float w, float h, float lt, float rt, float lb, float rb)
{
#if 0
  gcArc2(g, x + lt, y + lt, lt, lt, 180, 270);
  gcLineTo(g, w - rt + x, y);
  gcArc2(g, w - rt + x, rt + y, rt, rt, 270, 360);
  gcLineTo(g, w + x, h + y - rb);
  gcArc2(g, w - rb + x, h - rb + y, rb, rb, 0, 90);
  gcLineTo(g, lb + x, h + y);
  gcArc2(g, lb + x, h - lb + y, lb, lb, 90, 180);
#endif
}
CC_INLINE void gcFillRoundRect(gc_t* g, float x, float y, float w, float h, float lt, float rt, float lb, float rb)
{
  gcRoundRect(g, GcOptFill, x, y, w, h, lt, rt, lb, rb);
}
CC_INLINE void gcStrokeRoundRect(gc_t* g, float x, float y, float w, float h, float lt, float rt, float lb, float rb)
{
  gcRoundRect(g, GcOptStroke, x, y, w, h, lt, rt, lb, rb);
}
#define gcFillRoundRect2(g, x, y, w, h, r, flag) gcFillRoundRect(g, x, y, w, h, (flag)&1?0:r, (flag)&2?0:r, (flag)&4?0:r, (flag)&8?0:r)
#define gcStrokeRoundRect2(g, x, y, w, h, r, flag) gcStrokeRoundRect(g, x, y, w, h, (flag)&1?0:r, (flag)&2?0:r, (flag)&4?0:r, (flag)&8?0:r)
//CC_INLINE int ImDraw_edgeRect(gc_t* g, float x, float y, float w, float h)
#undef VA_ARG_EQ
#undef VA_BEG
#undef VA_END
#undef VA_BEG1
#undef VA_END1
#define gcFillRect(g, x, y, w, h)  gcRect(g, GcOptFill, x, y, w, h)
#define gcFillRect2(g, rc)  gcRect(g, GcOptFill, rc.l, rc.t, RCW(&rc), RCH(&rc))
#define gcStrokeRect(g, x, y, w, h)  gcRect(g, GcOptStroke, x, y, w, h)
#define gcStrokeRect2(g, rc)  gcRect(g, GcOptStroke, rc.l, rc.t, RCW(&rc), RCH(&rc))
CC_INLINE void gcSolidRect(gc_t* g, float x, float y, float w, float h, COLOR clr)
{
  gcSolidBrush(g, clr);
  gcFillRect(g, x, y, w, h);
}
#define gcStrokeColor(g, clr)  gcPenColor(g, clr)
#define gcHatchBrush1(g, hatchstyle, forecol, backcol) gcHatchBrush(g, 0, 0, forecol, backcol, hatchstyle)
#define gcFillGradient(g, x0, y0, x1, y1, blendPositions, presetColors, count) gcGradientBrush(g, x0, y0, x1, y1, blendPositions, presetColors, count)
#define gcStrokeGradient(g, x0, y0, x1, y1, blendPositions, presetColors, count) gcGradientBrush(g, x0, y0, x1, y1, blendPositions, presetColors, count)
CC_INLINE void gcGradientRect(gc_t* g, IRECT rc, BOOL horz, const float* blendPositions, const COLOR* presetColors, int count)
{
  gcFillGradient(g, (float)rc.l, (float)rc.t, (float)(horz ? rc.r : rc.l), (float)(horz ? rc.t : rc.b), blendPositions, presetColors, count);
  gcFillRect(g, (float)rc.l, (float)rc.t, (float)RCW(&rc), (float)RCH(&rc));
}
CC_INLINE void gcSolidRect2(gc_t* g, IRECT rc, COLOR clr)
{
  gcSolidBrush(g, clr);
  gcFillRect(g, (float)rc.l, (float)rc.t, (float)RCW(&rc), (float)RCH(&rc));
}
CC_INLINE void gcGradientRect2(gc_t* g, IRECT rc, COLOR clr1, COLOR clr2, BOOL horz)
{
  if (clr1 == clr2) {
    gcSolidRect2(g, rc, clr1);
  }
  else {
    COLOR clrs[] = { clr1, clr2 };
    gcGradientRect(g, rc, horz, NULL, clrs, 2);
  }
}

#define gcDrawPath(g, Points, Types, Count)  gcPath(g, GcOptStroke, Points, Types, Count)
#define gcDrawPath2(g, path)  gcPath(g, GcOptStroke, path->Points, path->Types, path->Count)
CC_INLINE void gcDrawLine(gc_t* g, float x1, float y1, float x2, float y2) {
  FPOINT pt[] = { x1, y1, x2, y2 };
  BYTE ty[] = { PathPointTypeStart, PathPointTypeLine };
  gcDrawPath(g, pt, ty, countof(pt));
}
CC_INLINE void gcDrawLine3(gc_t* g, float x1, float y1, float x2, float y2, float x3, float y3) {
  FPOINT pt[] = { x1, y1, x2, y2, x3, y3 };
  BYTE ty[] = { PathPointTypeStart, PathPointTypeLine, PathPointTypeLine };
  gcDrawPath(g, pt, ty, countof(pt));
}
CC_INLINE void gcDrawLine4(gc_t* g, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
  FPOINT pt[] = { x1, y1, x2, y2, x3, y3, x4, y4 };
  BYTE ty[] = { PathPointTypeStart, PathPointTypeLine, PathPointTypeLine, PathPointTypeLine };
  gcDrawPath(g, pt, ty, countof(pt));
}
CC_INLINE void gcDrawLine5(gc_t* g, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5) {
  FPOINT pt[] = { x1, y1, x2, y2, x3, y3, x4, y4, x5, y5 };
  BYTE ty[] = { PathPointTypeStart, PathPointTypeLine, PathPointTypeLine, PathPointTypeLine, PathPointTypeLine };
  gcDrawPath(g, pt, ty, countof(pt));
}
//#define GCDRAWPOLYLINE(g, closed, PT)  {FPOINT pt[] = PT; gcDrawPolygon(g, pt, countof(pt), closed, GC_FLOAT, sizeof(*pt));}
#if 0
CC_INLINE void gcDrawPolyline(gc_t* g, int closed, int n) {
  FPOINT pt[100];
  ASSERT(n<100);
  va_list arglist;
  int i;
  va_start(arglist, fmt);
  for (i = 0; i<n; ++i) {
    pt[i].x = va_arg(arglist, double);
    pt[i].y = va_arg(arglist, double);
  }
  va_end(arglist);
  gcDrawPolygon(g, pt, n, closed, GC_FLOAT, sizeof(*pt));
}
#endif
void gcSolidLine(gc_t* g, float x1, float y1, float x2, float y2, COLOR line, double wline)
{
  if (GetAV(line) && wline > 0) {
    gcStrokeColor(g, line);
    gcLineWidth(g, (float)wline);
    gcDrawLine(g, x1, y1, x2, y2);
  }
}
#define gcPath2(g, opt, path)  gcPath(g, opt, path->Points, path->Types, path->Count)
#define gcFillPath2(g, path)  gcPath2(g, GcOptFill, path)
#define gcStrokePath2(g, path)  gcPath2(g, GcOptStroke, path)
#define gcFillPath(g, Points, Types, Count)   gcPath(g, GcOptFill, Points, Types, Count)

CC_INLINE void gcPolygons(gc_t* g, int opt, const void* pt, const int* lens, int c, TypeId type, int step)
{
  Path path[1] = { 0 };
  PathAddPolygonX(path, pt, lens, c, type, step);
  gcPath2(g, opt, path);
  PathFree(path);
}
CC_INLINE void gcPolygon(gc_t* g, int opt, const void* pt, int n, int closed, TypeId type, int step)
{
  if (!closed) {
    n = -abs(n);
  }
  gcPolygons(g, opt, pt, &n, 1, type, step);
}
CC_INLINE void gcFillPolygon(gc_t* g, const void* pt, int n, int closed, TypeId type, int step)
{
  gcPolygon(g, GcOptFill, pt, n, closed, type, step);
}

CC_INLINE void gcDrawPolygon(gc_t* g, const void* pt, int n, int closed, TypeId type, int step)
{
  gcPolygon(g, GcOptStroke, pt, n, closed, type, step);
}
void gcSolidPolygons(gc_t* g, const void* pt, const int* ln, int c, TypeId type, int step, COLOR fill, COLOR line, double wline)
{
  if (fill) {
    gcSolidBrush(g, fill);
    gcPolygons(g, GcOptFill, pt, ln, c, type, step);
  }
  if (line) {
    gcStrokeColor(g, line);
    gcLineWidth(g, (float)wline);
    gcPolygons(g, GcOptStroke, pt, ln, c, type, step);
  }
}
void gcSolidPolygon(gc_t* g, const void* pt, int n, bool closed, TypeId type, int step, COLOR fill, COLOR line, double wline)
{
  if (!closed) {
    n = -abs(n);
  }
  gcSolidPolygons(g, pt, &n, 1, type, step, fill, line, wline);
}
CC_INLINE void gcEllipse2(gc_t* g, int opt, float x, float y, float rx, float ry)
{
  gcEllipse(g, opt, x - rx, y - ry, rx * 2, ry * 2);
}
CC_INLINE void gcCircle(gc_t* g, int opt, float x, float y, float r)
{
  gcEllipse2(g, opt, x, y, r, r);
}
CC_INLINE void gcIsPointInPath(gc_t* g, float x, float y)
{
  ASSERT(0);
  // 如果指定的点位于当前路径中，则返回 true，否则返回 false
}
void gcSolidCircle(gc_t* g, float x, float y, float radius, COLOR fill, COLOR line, double wline)
{
  int opt = 0;
  if (GetAV(fill)) {
    gcSolidBrush(g, fill);
    opt |= GcOptFill;
  }
  if (GetAV(line) && wline > 0) {
    gcPenColor(g, line);
    gcLineWidth(g, (float)wline);
    opt |= GcOptStroke;
  }
  gcCircle(g, opt, x, y, radius);
}
CC_INLINE void gcDrawRectangle(gc_t* g, float x, float y, float w, float h) {
  gcRect(g, GcOptStroke, x, y, w, h);
}
#define gcFillRectangle(g, x, y, w, h)   gcRect(g, GcOptFill, x, y, w, h)
#define gcFillRectangle2(g, r) gcRect(g, GcOptFill, r.l, r.t, RCW(&r), RCH(&r))
CC_INLINE void gcDrawEllipse(gc_t* g, float x, float y, float w, float h) {
  gcEllipse(g, GcOptStroke, x, y, w, h);
}
CC_INLINE void gcFillEllipse(gc_t* g, float x, float y, float w, float h) {
  gcEllipse(g, GcOptFill, x, y, w, h);
}
#define gcFillEllipseRect(g, rc)  gcFillEllipse(g, rc.l, rc.t, RCW(&rc), RCH(&rc))
CC_INLINE void gcFillEllipse2(gc_t* g, float x, float y, float rx, float ry) {
  gcEllipse2(g, GcOptFill, x, y, rx, ry);
}
void gcPen(gc_t* g, COLOR clr, float wline) {
  gcPenColor(g, clr);
  gcLineWidth(g, wline);
}
CC_INLINE void gcTextureBrush(gc_t* g, const texture_t* tex, const float* m23) {
  gcPatternBrush(g, tex, NULL, _RGB(0, 0, 0), NULL, m23);
}
CC_INLINE void gcTextureBrushRect(gc_t* g, const texture_t* tex, int x, int y, int w, int h, const float* m23) {
  IRECT rc = iRECT2(x, y, w, h);
  gcPatternBrush(g, tex, &rc, _RGB(0, 0, 0), NULL, m23);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////




#endif // _STD_GDI_C_H_
