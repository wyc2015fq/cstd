
//////////////////////////////////////////////////////////////////////////////
typedef COLOR ARGB;
typedef ARGB Color;
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
  WrapModeSame, //普通平铺
  WrapModeTile, //普通平铺
    WrapModeTileFlip, //翻转并平铺
    WrapModeClamp, //不进行平铺，只在以（0，0）位置为原点显示一张图片
    WrapModeReflect = WrapModeTileFlip,
    WrapModeRepeat = WrapModeTile,
    WrapModeDefault = WrapModeSame,
    //WrapModeTileFlipX = WrapModeTileFlip, // 1
    //WrapModeTileFlipY = WrapModeTileFlip<<8, // 2
  //WrapModeTileFlipXY = WrapModeTileFlip|(WrapModeTileFlip<<8), // 3
} WrapMode;

CC_INLINE int WrapPix1D(int x, int w, WrapMode wrap) {
  switch (wrap) {
  case WrapModeSame:
    return BOUND(x, 0, w-1);
    break;
  case WrapModeTile:
    return x % w;
    break;
  case WrapModeTileFlip:
    {
      int w2 = w*2;
      x = x % (w2);
      if (x >= w) {
        x = w2 - 1- x;
      }
      return x;
    }
  case WrapModeClamp:
    return (x >= 0 && x < w) ? x : -1;
    break;
  }
  return -1;
}
//
CC_INLINE int wrap_index(int* p, int n, int size, int idx, int step, int wrap_mode)
{
  int i, size2;
  switch (wrap_mode) {
  case WrapModeClamp:
    for (i = 0; i < n; ++i) {
      p[i] = (idx >= 0 && idx < size) ? idx * step : -1;
      ++idx;
    }
    break;
  case WrapModeTile:
    for (i = 0; i < n; ++i) {
      idx %= size;
      p[i] = idx * step;
      ++idx;
    }
    break;
  case WrapModeReflect:
    size2 = size * 2;
    for (i = 0; i < n; ++i) {
      idx %= size2;
      p[i] = ((idx >= size) ? size2 - idx - 1 : idx) * step;
      ++idx;
    }
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}

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
  PathPointTypeBezier2    = 2,    // quadratic Beizer
  PathPointTypeBezier3    = 3,    // cubic Bezier
  PathPointTypeBezier4    = 4,    // quartic (4th order) Beizer
  PathPointTypeBezier5    = 5,    // quintic (5th order) Bezier
  PathPointTypeBezier6    = 6     // hexaic (6th order) Bezier
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
  int Size;
  int Count;
  FPOINT* Points;
  BYTE* Types; // PathPointType
  FillMode fill;
  int curlen; /* whether the next drawing action starts a new figure */
} Path;

#define PATH_LOOP_BEGIN1  {\
int i, pos=0;  \
for (i=1; i<Count; ++i) {  \
  int end = 0, closed = 0;  \
  if (Types[i] & PathPointTypeCloseSubpath) {  \
    closed = true;  \
    end = i + 1;  \
  } else if (PathPointTypeStart==(Types[i] & PathPointTypePathTypeMask) || i==(Count-1)) {  \
    closed = false;  \
    end = i + (i==(Count-1));  \
  }  \
  if (end>0) {


#define PATH_LOOP_BEGIN  { \
int i, pos=0;  \
  for (i=1; i<Count; ++i) { \
    int t = Types[i] & PathPointTypePathTypeMask; \
    if ((i==(Count-1) || PathPointTypeStart==t)) { \
      int end = 1+i-(PathPointTypeStart==t); \
      bool closed = !!(Types[end-1] & PathPointTypeCloseSubpath);

#define PATH_LOOP_END      pos = end;  }  } }


#define PATH_COPY_BEGIN(path, path2) {  \
Path flatcopy[1] = {0}; \
if (path==path2) { PathClone(path, flatcopy); path = flatcopy; } {

#define PATH_COPY_END()  } PathFree(flatcopy); }

#define PATH_FLATCOPY_BEGIN {  \
Path flatcopy[1] = {0}; \
if (!PathIsFlatten(path)) { \
  PathFlatten(path, flatness, flatcopy); \
  path = flatcopy; \
} else if (path==path2) { \
  PathClone(path, flatcopy); \
path = flatcopy; } {int _Count = path->Count

#define PATH_FLATCOPY_END  } PathFree(flatcopy); }


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
  
  penstyle->innerMiterLimit = (1.01);
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

//////////////////////////////////////////////////////////////////////////////
// color def
#if 0
#define COLOR_DEFDEF(COLOR_DEF) \
  COLOR_DEF(212,208,200,CLR_SCROLLBAR)\
  COLOR_DEF(58,110,165,CLR_BACKGROUND)\
  COLOR_DEF(10,36,106,CLR_ACTIVECAPTION)\
  COLOR_DEF(128,128,128,CLR_INACTIVECAPTION)\
  COLOR_DEF(212,208,200,CLR_MENU)\
  COLOR_DEF(255,255,255,CLR_WINDOW)\
  COLOR_DEF(0,0,0,CLR_WINDOWFRAME)\
  COLOR_DEF(0,0,0,CLR_MENUTEXT)\
  COLOR_DEF(0,0,0,CLR_WINDOWTEXT)\
  COLOR_DEF(255,255,255,CLR_CAPTIONTEXT)\
  COLOR_DEF(212,208,200,CLR_ACTIVEBORDER)\
  COLOR_DEF(212,208,200,CLR_INACTIVEBORDER)\
  COLOR_DEF(128,128,128,CLR_APPWORKSPACE)\
  COLOR_DEF(10,36,106,CLR_HIGHLIGHT)\
  COLOR_DEF(255,255,255,CLR_HIGHLIGHTTEXT)\
  COLOR_DEF(212,208,200,CLR_BTNFACE)\
  COLOR_DEF(128,128,128,CLR_BTNSHADOW)\
  COLOR_DEF(128,128,128,CLR_GRAYTEXT)\
  COLOR_DEF(0,0,0,CLR_BTNTEXT)\
  COLOR_DEF(212,208,200,CLR_INACTIVECAPTIONTEXT)\
  COLOR_DEF(255,255,255,CLR_BTNHIGHLIGHT)\
  COLOR_DEF(64,64,64,CLR_3DDKSHADOW)\
  COLOR_DEF(212,208,200,CLR_3DLIGHT)\
  COLOR_DEF(0,0,0,CLR_INFOTEXT)\
  COLOR_DEF(255,255,225,CLR_INFOBK)\
  COLOR_DEF(181,181,181,CLR_UNKNOWN)\
  COLOR_DEF(0,0,128,CLR_HOTLIGHT)\
  COLOR_DEF(166,202,240,CLR_GRADIENTACTIVECAPTION)\
  COLOR_DEF(192,192,192,CLR_GRADIENTINACTIVECAPTION)\
  COLOR_DEF(49,106,197,CLR_MENUHILIGHT)\
COLOR_DEF(236,233,216,CLR_MENUBAR)


enum {
#define COLOR_DEF(r, g, b, id) id,
  COLOR_DEFDEF(COLOR_DEF)
#undef COLOR_DEF
    CLR_DESKTOP = CLR_BACKGROUND,
    CLR_3DFACE = CLR_BTNFACE,
    CLR_3DSHADOW = CLR_BTNSHADOW,
    CLR_3DHIGHLIGHT = CLR_BTNHIGHLIGHT,
    CLR_3DHILIGHT = CLR_BTNHIGHLIGHT,
    CLR_BTNHILIGHT = CLR_BTNHIGHLIGHT,
};
static COLOR myGetSysColor(int nIndex)
{
  enum { NUM_SYSCOLORS = 31 };
  static const COLOR SysColors[NUM_SYSCOLORS] = {
#define COLOR_DEF(r, g, b, id) _RGB(r, g, b),
    COLOR_DEFDEF(COLOR_DEF)
#undef COLOR_DEF
  };
  if (nIndex >= 0 && nIndex < NUM_SYSCOLORS) {
    return SysColors[nIndex];
  }
  return 0;
}
#endif

#define COLOR_DEFDEF(SYSCOLORDEF) \
  SYSCOLORDEF(SCROLLBAR, FFD4D0C8) \
  SYSCOLORDEF(BACKGROUND, FF3A6EA5) \
  SYSCOLORDEF(ACTIVECAPTION, FF0A246A) \
  SYSCOLORDEF(INACTIVECAPTION, FF808080) \
  SYSCOLORDEF(MENU, FFD4D0C8) \
  SYSCOLORDEF(WINDOW, FFFFFFFF) \
  SYSCOLORDEF(WINDOWFRAME, FF000000) \
  SYSCOLORDEF(MENUTEXT, FF000000) \
  SYSCOLORDEF(WINDOWTEXT, FF000000) \
  SYSCOLORDEF(CAPTIONTEXT, FFFFFFFF) \
  SYSCOLORDEF(ACTIVEBORDER, FFD4D0C8) \
  SYSCOLORDEF(INACTIVEBORDER, FFD4D0C8) \
  SYSCOLORDEF(APPWORKSPACE, FF808080) \
  SYSCOLORDEF(HIGHLIGHT, FF0A246A) \
  SYSCOLORDEF(HIGHLIGHTTEXT, FFFFFFFF) \
  SYSCOLORDEF(BTNFACE, FFD4D0C8) \
  SYSCOLORDEF(BTNSHADOW, FF808080) \
  SYSCOLORDEF(GRAYTEXT, FF808080) \
  SYSCOLORDEF(BTNTEXT, FF000000) \
  SYSCOLORDEF(INACTIVECAPTIONTEXT, FFD4D0C8) \
  SYSCOLORDEF(BTNHIGHLIGHT, FFFFFFFF) \
  SYSCOLORDEF(3DDKSHADOW, FF404040) \
  SYSCOLORDEF(3DLIGHT, FFD4D0C8) \
  SYSCOLORDEF(INFOTEXT, FF000000) \
  SYSCOLORDEF(INFOBK, FFFFFFE1) \
  SYSCOLORDEF(HOTLIGHT, FF000080) \
  SYSCOLORDEF(GRADIENTACTIVECAPTION, FFA6CAF0) \
  SYSCOLORDEF(GRADIENTINACTIVECAPTION, FFC0C0C0) \
  SYSCOLORDEF(DESKTOP, FF3A6EA5) \
  SYSCOLORDEF(3DFACE, FFD4D0C8) \
  SYSCOLORDEF(3DSHADOW, FF808080) \
  SYSCOLORDEF(3DHIGHLIGHT, FFFFFFFF) \
  SYSCOLORDEF(3DHILIGHT, FFFFFFFF) \
SYSCOLORDEF(BTNHILIGHT, FFFFFFFF)

enum {
#define SYSCOLORDEF(a, b)  CLR_##a=0x##b,
COLOR_DEFDEF(SYSCOLORDEF)
#undef SYSCOLORDEF
};
#define _GetSysColor(_COL)  (uint32)(_COL)
#define myGetSysColor(_COL)  (uint32)(_COL)

#define COLORDEF_DEF(COLORDEF) \
  COLORDEF(AliceBlue,_RGB(240,248,255))\
  COLORDEF(AntiqueWhite,_RGB(250,235,215))\
  COLORDEF(Aqua,_RGB(0,255,255))\
  COLORDEF(Aquamarine,_RGB(127,255,212))\
  COLORDEF(Azure,_RGB(240,255,255))\
  COLORDEF(Beige,_RGB(245,245,220))\
  COLORDEF(Bisque,_RGB(255,228,196))\
  COLORDEF(Black,_RGB(0,0,0))\
  COLORDEF(BlanchedAlmond,_RGB(255,255,205))\
  COLORDEF(Blue,_RGB(0,0,255))\
  COLORDEF(BlueViolet,_RGB(138,43,226))\
  COLORDEF(Brown,_RGB(165,42,42))\
  COLORDEF(Burlywood,_RGB(222,184,135))\
  COLORDEF(CadetBlue,_RGB(95,158,160))\
  COLORDEF(Chartreuse,_RGB(127,255,0))\
  COLORDEF(Chocolate,_RGB(210,105,30))\
  COLORDEF(Coral,_RGB(255,127,80))\
  COLORDEF(CornflowerBlue,_RGB(100,149,237))\
  COLORDEF(Cornsilk,_RGB(255,248,220))\
  COLORDEF(Crimson,_RGB(220,20,60))\
  COLORDEF(Cyan,_RGB(0,255,255))\
  COLORDEF(DarkBlue,_RGB(0,0,139))\
  COLORDEF(DarkCyan,_RGB(0,139,139))\
  COLORDEF(DarkGoldenRod,_RGB(184,134,11))\
  COLORDEF(DarkGray,_RGB(169,169,169))\
  COLORDEF(DarkGreen,_RGB(0,100,0))\
  COLORDEF(DarkKhaki,_RGB(189,183,107))\
  COLORDEF(DarkMagenta,_RGB(139,0,139))\
  COLORDEF(DarkOliveGreen,_RGB(85,107,47))\
  COLORDEF(DarkOrange,_RGB(255,140,0))\
  COLORDEF(DarkOrchid,_RGB(153,50,204))\
  COLORDEF(DarkRed,_RGB(139,0,0))\
  COLORDEF(DarkSalmon,_RGB(233,150,122))\
  COLORDEF(DarkSeaGreen,_RGB(143,188,143))\
  COLORDEF(DarkSlateBlue,_RGB(72,61,139))\
  COLORDEF(DarkSlateGray,_RGB(47,79,79))\
  COLORDEF(DarkTurquoise,_RGB(0,206,209))\
  COLORDEF(DarkViolet,_RGB(148,0,211))\
  COLORDEF(DeepPink,_RGB(255,20,147))\
  COLORDEF(DeepSkyBlue,_RGB(0,191,255))\
  COLORDEF(DimGray,_RGB(105,105,105))\
  COLORDEF(DodgerBlue,_RGB(30,144,255))\
  COLORDEF(FireBrick,_RGB(178,34,34))\
  COLORDEF(FloralWhite,_RGB(255,250,240))\
  COLORDEF(ForestGreen,_RGB(34,139,34))\
  COLORDEF(Fuchsia,_RGB(255,0,255))\
  COLORDEF(Gainsboro,_RGB(220,220,220))\
  COLORDEF(GhostWhite,_RGB(248,248,255))\
  COLORDEF(Gold,_RGB(255,215,0))\
  COLORDEF(GoldenRod,_RGB(218,165,32))\
  COLORDEF(Gray,_RGB(127,127,127))\
  COLORDEF(Green,_RGB(0,128,0))\
  COLORDEF(GreenYellow,_RGB(173,255,47))\
  COLORDEF(HoneyDew,_RGB(240,255,240))\
  COLORDEF(HotPink,_RGB(255,105,180))\
  COLORDEF(IndianRed,_RGB(205,92,92))\
  COLORDEF(Indigo,_RGB(75,0,130))\
  COLORDEF(Ivory,_RGB(255,255,240))\
  COLORDEF(Khaki,_RGB(240,230,140))\
  COLORDEF(Lavender,_RGB(230,230,250))\
  COLORDEF(LavenderBlush,_RGB(255,240,245))\
  COLORDEF(Lawngreen,_RGB(124,252,0))\
  COLORDEF(LemonChiffon,_RGB(255,250,205))\
  COLORDEF(LightBlue,_RGB(173,216,230))\
  COLORDEF(LightCoral,_RGB(240,128,128))\
  COLORDEF(LightCyan,_RGB(224,255,255))\
  COLORDEF(LightGoldenRodYellow,_RGB(250,250,210))\
  COLORDEF(LightGreen,_RGB(144,238,144))\
  COLORDEF(LightGrey,_RGB(211,211,211))\
  COLORDEF(LightPink,_RGB(255,182,193))\
  COLORDEF(LightSalmon,_RGB(255,160,122))\
  COLORDEF(LightSeaGreen,_RGB(32,178,170))\
  COLORDEF(LightSkyBlue,_RGB(135,206,250))\
  COLORDEF(LightSlateGray,_RGB(119,136,153))\
  COLORDEF(LightSteelBlue,_RGB(176,196,222))\
  COLORDEF(LightYellow,_RGB(255,255,224))\
  COLORDEF(Lime,_RGB(0,255,0))\
  COLORDEF(LimeGreen,_RGB(50,205,50))\
  COLORDEF(Linen,_RGB(250,240,230))\
  COLORDEF(Magenta,_RGB(255,0,255))\
  COLORDEF(Maroon,_RGB(128,0,0))\
  COLORDEF(MediumAquamarine,_RGB(102,205,170))\
  COLORDEF(MediumBlue,_RGB(0,0,205))\
  COLORDEF(MediumOrchid,_RGB(186,85,211))\
  COLORDEF(MediumPurple,_RGB(147,112,219))\
  COLORDEF(MediumSeaGreen,_RGB(60,179,113))\
  COLORDEF(MediumSlateBlue,_RGB(123,104,238))\
  COLORDEF(MediumSpringGreen,_RGB(0,250,154))\
  COLORDEF(MediumTurquoise,_RGB(72,209,204))\
  COLORDEF(MediumVioletRed,_RGB(199,21,133))\
  COLORDEF(MidnightBlue,_RGB(25,25,112))\
  COLORDEF(MintCream,_RGB(245,255,250))\
  COLORDEF(MistyRose,_RGB(255,228,225))\
  COLORDEF(Moccasin,_RGB(255,228,181))\
  COLORDEF(NavajoWhite,_RGB(255,222,173))\
  COLORDEF(Navy,_RGB(0,0,128))\
  COLORDEF(OldLace,_RGB(253,245,230))\
  COLORDEF(Olive,_RGB(128,128,0))\
  COLORDEF(OliveDrab,_RGB(107,142,35))\
  COLORDEF(Orange,_RGB(255,165,0))\
  COLORDEF(OrangeRed,_RGB(255,69,0))\
  COLORDEF(Orchid,_RGB(218,112,214))\
  COLORDEF(PaleGoldenRod,_RGB(238,232,170))\
  COLORDEF(PaleGreen,_RGB(152,251,152))\
  COLORDEF(PaleTurquoise,_RGB(175,238,238))\
  COLORDEF(PaleVioletRed,_RGB(219,112,147))\
  COLORDEF(PapayaWhip,_RGB(255,239,213))\
  COLORDEF(PeachPuff,_RGB(255,218,185))\
  COLORDEF(Peru,_RGB(205,133,63))\
  COLORDEF(Pink,_RGB(255,192,203))\
  COLORDEF(Plum,_RGB(221,160,221))\
  COLORDEF(PowderBlue,_RGB(176,224,230))\
  COLORDEF(Purple,_RGB(128,0,128))\
  COLORDEF(Red,_RGB(255,0,0))\
  COLORDEF(RosyBrown,_RGB(188,143,143))\
  COLORDEF(RoyalBlue,_RGB(65,105,225))\
  COLORDEF(SaddleBrown,_RGB(139,69,19))\
  COLORDEF(Salmon,_RGB(250,128,114))\
  COLORDEF(SandyBrown,_RGB(244,164,96))\
  COLORDEF(SeaGreen,_RGB(46,139,87))\
  COLORDEF(SeaShell,_RGB(255,245,238))\
  COLORDEF(Sienna,_RGB(160,82,45))\
  COLORDEF(Silver,_RGB(192,192,192))\
  COLORDEF(SkyBlue,_RGB(135,206,235))\
  COLORDEF(SlateBlue,_RGB(106,90,205))\
  COLORDEF(SlateGray,_RGB(112,128,144))\
  COLORDEF(Snow,_RGB(255,250,250))\
  COLORDEF(SpringGreen,_RGB(0,255,127))\
  COLORDEF(SteelBlue,_RGB(70,130,180))\
  COLORDEF(Tan,_RGB(210,180,140))\
  COLORDEF(Teal,_RGB(0,128,128))\
  COLORDEF(Thistle,_RGB(216,191,216))\
  COLORDEF(Tomato,_RGB(255,99,71))\
  COLORDEF(Turquoise,_RGB(64,224,208))\
  COLORDEF(Violet,_RGB(238,130,238))\
  COLORDEF(Wheat,_RGB(245,222,179))\
  COLORDEF(White,_RGB(255,255,255))\
  COLORDEF(WhiteSmoke,_RGB(245,245,245))\
  COLORDEF(Yellow,_RGB(255,255,0))\
  COLORDEF(YellowGreen,_RGB(139,205,50))

enum {
#define COLORDEF(name, a) Color_##name = a,
  COLORDEF_DEF(COLORDEF)
#undef COLORDEF
};


// Color
// Construct an opaque Color object with
// the specified Red, Green, Blue values.
// Color values are not premultiplied.
// Common color constants
#define COLORTABLEDEF_DEF(COLORTABLEDEF) \
COLORTABLEDEF(AliceBlue, 0xFFF0F8FF) \
COLORTABLEDEF(AntiqueWhite, 0xFFFAEBD7) \
COLORTABLEDEF(Aqua, 0xFF00FFFF) \
COLORTABLEDEF(Aquamarine, 0xFF7FFFD4) \
COLORTABLEDEF(Azure, 0xFFF0FFFF) \
COLORTABLEDEF(Beige, 0xFFF5F5DC) \
COLORTABLEDEF(Bisque, 0xFFFFE4C4) \
COLORTABLEDEF(BlanchedAlmond, 0xFFFFEBCD) \
COLORTABLEDEF(Blue, 0xFF0000FF) \
COLORTABLEDEF(BlueViolet, 0xFF8A2BE2) \
COLORTABLEDEF(Brown, 0xFFA52A2A) \
COLORTABLEDEF(BurlyWood, 0xFFDEB887) \
COLORTABLEDEF(CadetBlue, 0xFF5F9EA0) \
COLORTABLEDEF(Chartreuse, 0xFF7FFF00) \
COLORTABLEDEF(Chocolate, 0xFFD2691E) \
COLORTABLEDEF(Coral, 0xFFFF7F50) \
COLORTABLEDEF(CornflowerBlue, 0xFF6495ED) \
COLORTABLEDEF(Cornsilk, 0xFFFFF8DC) \
COLORTABLEDEF(Crimson, 0xFFDC143C) \
COLORTABLEDEF(Cyan, 0xFF00FFFF) \
COLORTABLEDEF(DarkBlue, 0xFF00008B) \
COLORTABLEDEF(DarkCyan, 0xFF008B8B) \
COLORTABLEDEF(DarkGoldenrod, 0xFFB8860B) \
COLORTABLEDEF(DarkGray, 0xFFA9A9A9) \
COLORTABLEDEF(DarkGreen, 0xFF006400) \
COLORTABLEDEF(DarkKhaki, 0xFFBDB76B) \
COLORTABLEDEF(DarkMagenta, 0xFF8B008B) \
COLORTABLEDEF(DarkOliveGreen, 0xFF556B2F) \
COLORTABLEDEF(DarkOrange, 0xFFFF8C00) \
COLORTABLEDEF(DarkOrchid, 0xFF9932CC) \
COLORTABLEDEF(DarkRed, 0xFF8B0000) \
COLORTABLEDEF(DarkSalmon, 0xFFE9967A) \
COLORTABLEDEF(DarkSeaGreen, 0xFF8FBC8B) \
COLORTABLEDEF(DarkSlateBlue, 0xFF483D8B) \
COLORTABLEDEF(DarkSlateGray, 0xFF2F4F4F) \
COLORTABLEDEF(DarkTurquoise, 0xFF00CED1) \
COLORTABLEDEF(DarkViolet, 0xFF9400D3) \
COLORTABLEDEF(DeepPink, 0xFFFF1493) \
COLORTABLEDEF(DeepSkyBlue, 0xFF00BFFF) \
COLORTABLEDEF(DimGray, 0xFF696969) \
COLORTABLEDEF(DodgerBlue, 0xFF1E90FF) \
COLORTABLEDEF(Firebrick, 0xFFB22222) \
COLORTABLEDEF(FloralWhite, 0xFFFFFAF0) \
COLORTABLEDEF(ForestGreen, 0xFF228B22) \
COLORTABLEDEF(Fuchsia, 0xFFFF00FF) \
COLORTABLEDEF(Gainsboro, 0xFFDCDCDC) \
COLORTABLEDEF(GhostWhite, 0xFFF8F8FF) \
COLORTABLEDEF(Gold, 0xFFFFD700) \
COLORTABLEDEF(Goldenrod, 0xFFDAA520) \
COLORTABLEDEF(Gray, 0xFF808080) \
COLORTABLEDEF(Green, 0xFF008000) \
COLORTABLEDEF(GreenYellow, 0xFFADFF2F) \
COLORTABLEDEF(Honeydew, 0xFFF0FFF0) \
COLORTABLEDEF(HotPink, 0xFFFF69B4) \
COLORTABLEDEF(IndianRed, 0xFFCD5C5C) \
COLORTABLEDEF(Indigo, 0xFF4B0082) \
COLORTABLEDEF(Ivory, 0xFFFFFFF0) \
COLORTABLEDEF(Khaki, 0xFFF0E68C) \
COLORTABLEDEF(Lavender, 0xFFE6E6FA) \
COLORTABLEDEF(LavenderBlush, 0xFFFFF0F5) \
COLORTABLEDEF(LawnGreen, 0xFF7CFC00) \
COLORTABLEDEF(LemonChiffon, 0xFFFFFACD) \
COLORTABLEDEF(LightBlue, 0xFFADD8E6) \
COLORTABLEDEF(LightCoral, 0xFFF08080) \
COLORTABLEDEF(LightCyan, 0xFFE0FFFF) \
COLORTABLEDEF(LightGoldenrodYellow, 0xFFFAFAD2) \
COLORTABLEDEF(LightGray, 0xFFD3D3D3) \
COLORTABLEDEF(LightGreen, 0xFF90EE90) \
COLORTABLEDEF(LightPink, 0xFFFFB6C1) \
COLORTABLEDEF(LightSalmon, 0xFFFFA07A) \
COLORTABLEDEF(LightSeaGreen, 0xFF20B2AA) \
COLORTABLEDEF(LightSkyBlue, 0xFF87CEFA) \
COLORTABLEDEF(LightSlateGray, 0xFF778899) \
COLORTABLEDEF(LightSteelBlue, 0xFFB0C4DE) \
COLORTABLEDEF(LightYellow, 0xFFFFFFE0) \
COLORTABLEDEF(Lime, 0xFF00FF00) \
COLORTABLEDEF(LimeGreen, 0xFF32CD32) \
COLORTABLEDEF(Linen, 0xFFFAF0E6) \
COLORTABLEDEF(Magenta, 0xFFFF00FF) \
COLORTABLEDEF(Maroon, 0xFF800000) \
COLORTABLEDEF(MediumAquamarine, 0xFF66CDAA) \
COLORTABLEDEF(MediumBlue, 0xFF0000CD) \
COLORTABLEDEF(MediumOrchid, 0xFFBA55D3) \
COLORTABLEDEF(MediumPurple, 0xFF9370DB) \
COLORTABLEDEF(MediumSeaGreen, 0xFF3CB371) \
COLORTABLEDEF(MediumSlateBlue, 0xFF7B68EE) \
COLORTABLEDEF(MediumSpringGreen, 0xFF00FA9A) \
COLORTABLEDEF(MediumTurquoise, 0xFF48D1CC) \
COLORTABLEDEF(MediumVioletRed, 0xFFC71585) \
COLORTABLEDEF(MidnightBlue, 0xFF191970) \
COLORTABLEDEF(MintCream, 0xFFF5FFFA) \
COLORTABLEDEF(MistyRose, 0xFFFFE4E1) \
COLORTABLEDEF(Moccasin, 0xFFFFE4B5) \
COLORTABLEDEF(NavajoWhite, 0xFFFFDEAD) \
COLORTABLEDEF(Navy, 0xFF000080) \
COLORTABLEDEF(OldLace, 0xFFFDF5E6) \
COLORTABLEDEF(Olive, 0xFF808000) \
COLORTABLEDEF(OliveDrab, 0xFF6B8E23) \
COLORTABLEDEF(Orange, 0xFFFFA500) \
COLORTABLEDEF(OrangeRed, 0xFFFF4500) \
COLORTABLEDEF(Orchid, 0xFFDA70D6) \
COLORTABLEDEF(PaleGoldenrod, 0xFFEEE8AA) \
COLORTABLEDEF(PaleGreen, 0xFF98FB98) \
COLORTABLEDEF(PaleTurquoise, 0xFFAFEEEE) \
COLORTABLEDEF(PaleVioletRed, 0xFFDB7093) \
COLORTABLEDEF(PapayaWhip, 0xFFFFEFD5) \
COLORTABLEDEF(PeachPuff, 0xFFFFDAB9) \
COLORTABLEDEF(Peru, 0xFFCD853F) \
COLORTABLEDEF(Pink, 0xFFFFC0CB) \
COLORTABLEDEF(Plum, 0xFFDDA0DD) \
COLORTABLEDEF(PowderBlue, 0xFFB0E0E6) \
COLORTABLEDEF(Purple, 0xFF800080) \
COLORTABLEDEF(Red, 0xFFFF0000) \
COLORTABLEDEF(RosyBrown, 0xFFBC8F8F) \
COLORTABLEDEF(RoyalBlue, 0xFF4169E1) \
COLORTABLEDEF(SaddleBrown, 0xFF8B4513) \
COLORTABLEDEF(Salmon, 0xFFFA8072) \
COLORTABLEDEF(SandyBrown, 0xFFF4A460) \
COLORTABLEDEF(SeaGreen, 0xFF2E8B57) \
COLORTABLEDEF(SeaShell, 0xFFFFF5EE) \
COLORTABLEDEF(Sienna, 0xFFA0522D) \
COLORTABLEDEF(Silver, 0xFFC0C0C0) \
COLORTABLEDEF(SkyBlue, 0xFF87CEEB) \
COLORTABLEDEF(SlateBlue, 0xFF6A5ACD) \
COLORTABLEDEF(SlateGray, 0xFF708090) \
COLORTABLEDEF(Snow, 0xFFFFFAFA) \
COLORTABLEDEF(SpringGreen, 0xFF00FF7F) \
COLORTABLEDEF(SteelBlue, 0xFF4682B4) \
COLORTABLEDEF(Tan, 0xFFD2B48C) \
COLORTABLEDEF(Teal, 0xFF008080) \
COLORTABLEDEF(Thistle, 0xFFD8BFD8) \
COLORTABLEDEF(Tomato, 0xFFFF6347) \
COLORTABLEDEF(Transparent, 0x00FFFFFF) \
COLORTABLEDEF(Turquoise, 0xFF40E0D0) \
COLORTABLEDEF(Violet, 0xFFEE82EE) \
COLORTABLEDEF(Wheat, 0xFFF5DEB3) \
COLORTABLEDEF(White, 0xFFFFFFFF) \
COLORTABLEDEF(WhiteSmoke, 0xFFF5F5F5) \
COLORTABLEDEF(Yellow, 0xFFFFFF00) \
COLORTABLEDEF(YellowGreen, 0xFF9ACD32) \
COLORTABLEDEF(PureGreen, 0xFF00FF00) \
COLORTABLEDEF(DeepGreen, 0xFF00C000) \
COLORTABLEDEF(Black, 0xFF000000)

enum {
#define COLORTABLEDEF(name, b)  Color##name = b,
COLORTABLEDEF_DEF(COLORTABLEDEF)
#undef COLORTABLEDEF
};

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
  pColor = (COLOR*)(bIsRight ? (pBits + (al) * (y + 1) - x) : (pBits + (al) * y + x));
  *pColor = PixelAlpha2(*pColor, percent);
  
  if (clrBase == 0) {
    return 0;
  }
  
  *pColor = _RGB(MIN(255, (3 * GetRV(*pColor) + GetBV(clrBase)) / 4),
    MIN(255, (3 * GetGV(*pColor) + GetGV(clrBase)) / 4),
    MIN(255, (3 * GetBV(*pColor) + GetRV(clrBase)) / 4));
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

#define WINBMPCN 4


typedef enum {
    //KEY_ENTER = '\n', // FOR TEXT EDIT
    //KEY_TAB, // FOR TABBING THROUGH FIELDS
    KEY_LEFT, // FOR TEXT EDIT
    KEY_RIGHT,// FOR TEXT EDIT
    KEY_UP, // FOR TEXT EDIT
    KEY_DOWN, // FOR TEXT EDIT
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_HOME, // FOR TEXT EDIT
    KEY_END, // FOR TEXT EDIT
    KEY_DELETE, // FOR TEXT EDIT
    KEY_BACK, // FOR TEXT EDIT
    KEY_ESCAPE, // FOR TEXT EDIT
    //KEY_CTRL,
    //KEY_SHIFT,
    //KEY_ALT,
    //KEY_SUPER,
    KEY_COUNT
} KeyCode;
typedef enum {
  MOUSE_LBUTTON,
  MOUSE_MBUTTON,
  MOUSE_RBUTTON,
} MouseCode;
enum {
  MSG_FLAG_LBUTTON = 1,
    MSG_FLAG_RBUTTON = 2,
    MSG_FLAG_MBUTTON = 4,
    MSG_FLAG_CTRLKEY = 8,
    MSG_FLAG_SHIFTKEY = 16,
    MSG_FLAG_ALTKEY = 32
};
typedef enum WindowMsg_ {
  MSG_NOP,
    MSG_TIMER,
#if 1
    MSG_LBUTTONDOWN,
    MSG_LBUTTONUP,
    MSG_RBUTTONDOWN,
    MSG_RBUTTONUP,
    MSG_MBUTTONDOWN,
    MSG_MBUTTONUP,
    MSG_LBUTTONDBLCLK,
    MSG_RBUTTONDBLCLK,
    MSG_MBUTTONDBLCLK,
#else
    MSG_MOUSEDOWN,
    MSG_MOUSEUP,
#endif
    MSG_MOUSEWHEEL,
    MSG_MOUSEWHEELUP,
    MSG_MOUSEWHEELDOWN,
    MSG_MOUSEMOVE,
    MSG_KEYDOWN,
    MSG_SIZE,
    MSG_KEYUP,
    MSG_CHAR,
    MSG_CLOSE,
} WindowMsg;

typedef struct {
  IRECT rc;
  ISIZE sz;
} ctrl_t;

typedef struct {
  IRECT margin, border, padding, content;
} box_t;

typedef struct {
  IRECT rect;
  IRECT rcLT;
  IRECT rcRB;
  IPOINT Size;
  IPOINT Pos;
  BOOL horz;
  int irc;
  int nrcs;
  int gap;
  int ilsz;
  int layoutflags;
} layerinfo_t;

typedef struct {
  int deep;
  layerinfo_t layinfo[16];
  IRECT rcs[100];
  int nrcs;
  int gap;
  int layoutflags;
} layer_t;
#define CFONT_BIT_INIT(fb, PIX, BX, BY, X, Y, S) ((fb)->bit=PIX, (fb)->bx=BX, (fb)->by=BY, (fb)->x=X, (fb)->y=Y, (fb)->step=S)
#define CMAXFONT 8
#define CFONT_BOLD 1<<0
#define CFONT_ITALIC 1<<1
#define CFONT_BOLDITALIC 1<<2
#define CFONT_UNDERLINE 1<<3
#define CFONT_STRIKEOUT 1<<4
#define MAXNAMELEN 256
typedef struct font_bit_t {
  int bx, by; // 盒子 box_x box_y
  int x, y; // 原点
  int step;
  uchar* bit;
} font_bit_t;
typedef struct font_t {
  char name[32];
  short h, w, bl, bpp; // height, width, baseline, bit per pix
  short style; // 风格
  void* user;
  int (*getbit)(const struct font_t* fo, int ch, font_bit_t* bit); // 成功返回 bx
  int (*release)(const struct font_t* fo);
  struct font_t* next;
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
  ushort IndexLookup[1<<16]; // Sparse. Index glyphs by Unicode code-point.
  int IndexLookup_Size;
  float IndexXAdvance[1<<16]; // Sparse. Glyphs->XAdvance in a directly indexable way (more cache-friendly, for CalcTextSize functions which are often bottleneck in large UI).
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
} matrix_t;
typedef FPOINT4 vector_t;
//typedef vector_t point_t;

typedef struct {
  matrix_t world; // 世界坐标变换
  matrix_t view; // 摄影机坐标变换
  matrix_t projection; // 投影变换
  matrix_t transform; // transform = world * view * projection
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

#define GCTYPEDEF_DEF(GCTYPEDEF) \
GCTYPEDEF(GC_INT8, char) \
GCTYPEDEF(GC_BYTE, uchar) \
GCTYPEDEF(GC_INT16, short) \
GCTYPEDEF(GC_UINT16, unsigned short) \
GCTYPEDEF(GC_INT32, int) \
GCTYPEDEF(GC_UINT32, unsigned int) \
GCTYPEDEF(GC_FLOAT, float) \
GCTYPEDEF(GC_DOUBLE, double)

typedef enum {
#define GCTYPEDEF(NAME, TYPE)  NAME,
  GCTYPEDEF_DEF(GCTYPEDEF)
#undef GCTYPEDEF
} GCTYPE;

CC_INLINE void arr2fpoint(FPOINT* pt, const void* ps, GCTYPE type, int step, int count) {
  int i;
  const char* p = (const char*)ps;
switch(type) {
#define GCTYPEDEF(NAME, TYPE) case NAME: { ASSERT(step>=(int)sizeof(TYPE)); for (i=0; i<count; ++i, p+=step) {pt[i].x = ((const TYPE*)p)[0], pt[i].y = ((const TYPE*)p)[1];} } break;
  GCTYPEDEF_DEF(GCTYPEDEF)
#undef GCTYPEDEF
default:
  ASSERT(0);
  break;
  }
}
typedef enum {
  textAlign_left,
    textAlign_start,
    textAlign_end,
    textAlign_right,
    textAlign_center,
} textAlign_;

typedef struct brush_t brush_t;
struct brush_t {
  union {
    struct {
      COLOR clr;
    } solid;
    struct {
      int fill_area_x, fill_area_y;
      COLOR clrs[2];
      const uchar* hatch_data;
      uchar datay;
    } hatch;
    struct {
      int it, ix, iy;
      COLOR clrs[256];
    } lut_gradient;
    struct {
      int iA[12];
    } colormatrix;
    struct {
      COLOR bgclr;
      texture_t tex[1];
      int im23[6];
      IRECT rcSrc;
      COLOR palette[256];
    } texture;
    struct {
      int ix, iy, invrx, invry;
      COLOR clrs[256];
    } lut_radial;
    struct {
      int ix, iy, invrx, invry;
      COLOR clrs[256];
    } lut_swirled;
    struct {
      int ix, iy;
      int invr[256];
      COLOR clrs[256];
      COLOR clrcenter;
    } path_gradient;
    struct {
      int ix, iy;
      int invr[256];
      COLOR clrs[256];
    } path_radial;
  };
  bool blend;
  WrapMode wrapX;
  WrapMode wrapY;
  int (*fillline)(const brush_t* br, COLOR* pix_x, int b, int e, int y);
};

#if 1
#define gcfun(g, name)   g->fun->f##name
#else
#define gcfun(g, name)   cmd##name
#endif


typedef enum {
  //填充规则，
  GcOptFill=1,//表示用非零绕数规则
  GcOptStroke=2, //表示描线
  GcOptEO = 4, // 奇偶填充
  GcOptEOFill=GcOptEO|GcOptFill,//表示用奇偶规则
  GcOptFillStroke=GcOptFill|GcOptStroke, //表示填充+
  GcOptEOFillStroke=GcOptEOFill|GcOptStroke, //表示描线，不是填充
  GcOptFill2 = (SmoothingModeNone<<8) | GcOptFill,
} GcOptType;

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
GCFUNDEF(void, Polygons, (gc_t* g, const void* pt, const int* ln, int c, GCTYPE type, int step)) \
GCFUNDEF(void, Scale, (gc_t* g, float scalewidth, float scaleheight)) \
GCFUNDEF(void, Rotate, (gc_t* g, float angle)) \
GCFUNDEF(void, Translate, (gc_t* g, float x, float y)) \
GCFUNDEF(void, Transform, (gc_t* g, float a, float b, float c, float d, float e, float f)) \
GCFUNDEF(void, SetTransform, (gc_t* g, float a, float b, float c, float d, float e, float f)) \
  GCFUNDEF(void, Clip, (gc_t* g))    \
  GCFUNDEF(void, DashStyle, (gc_t* g, DashStyle dashStyle)) \

#endif

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
  
typedef enum {
  ImDrawCmd_NOP,
#define GCFUNDEF(RET, NAME, ARG)   ImDrawCmd_##NAME,
    GCFUNDEF_DEF(GCFUNDEF)
#undef GCFUNDEF
} DrawCmd;

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
typedef struct render_drv_t_ render_drv_t;
struct render_drv_t_ {
  void* x;
  IRECT rcDirty;
  int (*UnInit)(render_drv_t* drv);
  int (*Render)(render_drv_t* drv, const gc_t* ctx);
  int (*SetSize)(render_drv_t* drv, int w, int h);
  int (*SetTexture)(render_drv_t* drv, texture_t* tex, PixFmt fmt, int height, int width, void* data, int step);
};
typedef struct window_t_ window_t; //
struct window_t_ {
  IRECT rc;
  char text[16];
  bool show;
  ctrl_t client[1];
  window_t* prev;
  window_t* next;
  window_t* parent;
  window_t* child;
  gc_t g[1];
};
typedef struct window_native_t window_native_t;
typedef struct window_dc_t window_dc_t;
typedef struct window_host_t_ window_host_t;
typedef struct sysio_t_ sysio_t;
struct window_host_t_ {
  window_native_t* x;
  void* user;
  IRECT rect;
  bool open;
  char name[64];
  bool flip_y;
  render_drv_t drv[1];
  window_t root[1];
  window_t tooltip[1];
  window_t* hot_win;
  int (*event_fun)(sysio_t* io);
  window_host_t* prev, * next;
};
typedef struct sysmouse_t_ {
  IPOINT pos[10];
  int64 clicked_time[5];
  bool double_clicked[5];
  bool down[5];
  //void* downid[5];
  double dbclick_time;
  double wheelDelta;
  int code;
} sysmouse_t;
//#define keyshift()  io->keydown[KEY_SHIFT]
//#define keyctrl()   io->keydown[KEY_CTRL]
struct sysio_t_ {
  WindowMsg msg;
  int last_key;
  int last_char;
  bool keyshift;
  bool keyctrl;
  bool keyalt;
  uchar keydown[256];
  bool tick;
  int time;
  sysmouse_t mouse[1];
  window_host_t* root;
  window_host_t* cur_host;
  window_host_t* msg_host;
  window_t* cur_win;
  window_t* foc_win;
  //texfont_t font[1];
  font_t* font;
  //gc_t g[1];
  //layer_t la[1];
  void* hotid;
  void* hitid;
  int hitvalue;
  void* focid;
  const char** res;
  const char* cursor;
  gc_t* g;
};
#define ISHOT(c) (io->hotid==(c))
#define SETHOT(c) (io->hotid=(c))
#define SETHIT(c) (io->hitid = (c))
#define SETHOT2(c, hot) (io->hotid=(hot) ? (c) : NULL)
#define ISHIT(c) (io->hitid==(c))
#define SETHIT1(c) (io->hitid=(c))
#define SETHITV(v) (io->hitvalue = v)
#define GETHITV() (io->hitvalue)
#define ISFOCUS(c) (io->focid==(c))
#define SETFOCUS(c) (io->focid=(c))
#define SETFOCUSWIN(c) (io->foc_win=(c))
#define ISFOCUSWIN(c) (io->foc_win==(c))
#define FORCE_REDRAW(rc) (gcSetDirty(g, rc, false))
//#define ISHOVEREDWIN(c) (io->cur_host->hot_win==(c))
//#define ISHOVEREDWIN2() (io->cur_host->hot_win==io->cur_win)
#define ISHOVEREDWIN2() (1)

//#define SYSIO  sysio_t*io = sys_getio(); window_host_t* host = io->cur_host; gc_t* g = io->g; layer_t* la = io->la; sysmouse_t* mouse = io->mouse; int x = mouse->pos->x, y = mouse->pos->y
#define SYSIO  sysio_t*io = sys_getio(); gc_t* g = io->g; sysmouse_t* mouse = io->mouse; int x = mouse->pos->x, y = mouse->pos->y

CC_INLINE sysio_t* sys_getio() {
  static sysio_t io[1];
  return io;
}
static int sys_waitio(int delay);
static window_native_t* sys_create_window(int flag, const char* name, int x, int y, int w, int h, window_host_t* host);
static int sys_get_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient);
static int sys_set_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient);
static int sys_set_window_text(window_native_t* w, const char* text);
static int sys_del_dc(window_dc_t* dc);
static int sys_window_bitblt(window_native_t* dst_win, int x, int y, int w, int h, window_dc_t* src_dc, int src_x, int src_y);
static int sys_dc_set_size(window_dc_t** pdc, int h, int w, int cn);
static int sys_dc_get_image(window_dc_t* pdc, img_t* im);
////////////////////////////////////////////////////////////////////////////////
typedef struct {
  window_native_t* x;
  window_dc_t* hMemDC;
  img_t im[1];
  img_t im1[1];
} null_drv_t;


////////////////////////////////////////////////////////////////////////////////
CC_INLINE int img_setbitmap_cn4(int h, int w, const void* data, int step, int cn, PixFmt fmt, bool flip_y, bool scaling, uchar* outdata, int outstep, const void* pal) {
  int i, j;
  const uchar* srcarr = (const uchar*)data;
  double smin[4] = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX};
  double smax[4] = {DBL_MIN, DBL_MIN, DBL_MIN, DBL_MIN};
  double su[4] = {1, 1, 1, 1};
  if (h<=0 || w<=0) {
    return 0;
  }
  switch(fmt) {
  case PF_64bppGrayDouble:
    {
      typedef double arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 255. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_32bppGrayFloat:
    {
      typedef float arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 255. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_16bppGray:
    {
      typedef short arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 256. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_8bppGray:
    {
      typedef uchar arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 256. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_1bppIndexed:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4) {
        //int t = (src1[j>>3]>>(j&7))&1;
        int t = (src1[j>>3]>>(7-(j&7)))&1;
        int gry = t ? 255 : 0;
        dst1[0] = gry;
        dst1[1] = gry;
        dst1[2] = gry;
        dst1[3] = 255;
      }
    }
    break;
  case PF_24bppRGB:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
        dst1[0] = src1[0];
        dst1[1] = src1[1];
        dst1[2] = src1[2];
        dst1[3] = 255;
      }
    }
    break;
  case PF_32bppPARGB:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
        uchar a = src1[3];
        dst1[0] = src1[0]*a/255;
        dst1[1] = src1[1]*a/255;
        dst1[2] = src1[2]*a/255;
        dst1[3] = 255;
      }
    }
  break;
  case PF_32bppARGB:
  case PF_32bppRGB:
    for (i = 0; i < h; ++i) {
      uint32* dst1 = (uint32*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const uint32* src1 = (const uint32*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        *dst1++ = *src1++;
      }
    }
    break;
  case PF_128bppARGBFloat:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        dst1[0] = (uchar)(BOUND(src1[0], 0, 1)*255);
        dst1[1] = (uchar)(BOUND(src1[1], 0, 1)*255);
        dst1[2] = (uchar)(BOUND(src1[2], 0, 1)*255);
        dst1[3] = (uchar)(BOUND(src1[3], 0, 1)*255);
        dst1 += 4;
        src1 += 4;
      }
    }
    break;
  case PF_128bppABGRFloat:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        dst1[0] = (uchar)(BOUND(src1[2], 0, 1)*255);
        dst1[1] = (uchar)(BOUND(src1[1], 0, 1)*255);
        dst1[2] = (uchar)(BOUND(src1[0], 0, 1)*255);
        dst1[3] = (uchar)(BOUND(src1[3], 0, 1)*255);
        dst1 += 4;
        src1 += 4;
      }
    }
    break;
  case PF_96bppBGRFloat:
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const float* src1 = (const float*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1+=3) {
          MINMAX_UPDATE(src1[0], smin[0], smax[0]);
          MINMAX_UPDATE(src1[1], smin[1], smax[1]);
          MINMAX_UPDATE(src1[2], smin[2], smax[2]);
        }
      }
      su[0] = 256. / (smax[0] - smin[0]);
      su[1] = 256. / (smax[1] - smin[1]);
      su[2] = 256. / (smax[2] - smin[2]);
    }
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        double t0 = ((src1[0] - smin[0]) * su[0]);
        double t1 = ((src1[1] - smin[1]) * su[1]);
        double t2 = ((src1[2] - smin[2]) * su[2]);
        dst1[0] = (uchar)(BOUND(t0, 0, 255));
        dst1[1] = (uchar)(BOUND(t1, 0, 255));
        dst1[2] = (uchar)(BOUND(t2, 0, 255));
        dst1[3] = 255;
        dst1 += 4;
        src1 += 3;
      }
    }
    break;
  case PF_8bppIndexed:
    if (pal) {
      const uint32* upal = (const uint32*)pal;
      for (i = 0; i < h; ++i) {
        uint32* dst1 = (uint32*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j) {
          uchar t = src1[j];
          dst1[j] = upal[t];
        }
      }
    } else {
      for (i = 0; i < h; ++i) {
        uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j) {
          uchar t = src1[j];
          dst1[0] = t;
          dst1[1] = t;
          dst1[2] = t;
          dst1[3] = 255;
          dst1 += 4;
        }
      }
    }
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}

#define ERR printf
#define FIXME printf
#define TENSION_CONST (0.3)

#define MAX_ARC_PTS (13)
#define MAX_DASHLEN (16) /* this is a limitation of gdi */


typedef Status (*gdip_format_string_callback)(const WCHAR *string, INT index, INT length, const ttffont_t *f,
                                              const FRECT *rect, const StringFormat *format,
                                              INT lineno, const FRECT *bounds, INT *underlined_indexes,
                                              INT underlined_index_count, void *user_data);


typedef int (*get_glyph_f)(uint code, const float* matrix, void* userdata);


CC_INLINE INT gdip_round(float x)
{
  return (INT) floorf(x + 0.5);
}

CC_INLINE INT ceilr(float x)
{
  return (INT) ceilf(x);
}


/* Like atan2, but puts angle in correct quadrant if dx is 0. */
CC_INLINE float gdiplus_atan2(float dy, float dx)
{
  if((dx == 0.0) && (dy != 0.0))
    return dy > 0.0 ? M_PI_2 : -M_PI_2;
  
  return atan2(dy, dx);
}


enum { CC_WINDOW_AUTOSIZE = 1 };
// YV
enum {
  CC_WND_PROP_FULLSCREEN = 0,
    CC_WND_PROP_AUTOSIZE = 1,
    CC_WINDOW_NORMAL = 0,
    CC_WINDOW_FULLSCREEN = 1
};

CC_INLINE int mouse_proc_default(int event, int x, int y, int flags, void* param) {
  sysio_t* io = sys_getio();
  io->msg = (WindowMsg)event;
  io->mouse->pos->x = x;
  io->mouse->pos->y = y;
  UNUSED(param);
  UNUSED(flags);
  switch (io->msg) {
  case MSG_LBUTTONDOWN:
    io->msg = (WindowMsg)event;
    break;
  case MSG_LBUTTONUP:
    io->msg = (WindowMsg)event;
    break;
  default:
    break;
  }
  return 0;
}

#define event_def_def(event_def) \
  event_def(abort,图像加载被中断) \
  event_def(blur,元素失去焦点) \
  event_def(change,用户改变域的内容) \
  event_def(click,鼠标点击某个对象) \
  event_def(dblclick,鼠标双击某个对象) \
  event_def(error,当加载文档或图像时发生某个错误) \
  event_def(focus,元素获得焦点) \
  event_def(keydown,某个键盘的键被按下) \
  event_def(keypress,某个键盘的键被按下或按住) \
  event_def(keyup,某个键盘的键被松开) \
  event_def(load,某个页面或图像被完成加载) \
  event_def(mousedown,某个鼠标按键被按下) \
  event_def(mousemove,鼠标被移动) \
  event_def(mouseout,鼠标从某元素移开) \
  event_def(mouseover,鼠标被移到某元素之上) \
  event_def(mouseup,某个鼠标按键被松开) \
  event_def(reset,重置按钮被点击) \
  event_def(resize,窗口或框架被调整尺寸) \
  event_def(select,文本被选定) \
  event_def(submit,提交按钮被点击) \
  event_def(unload,用户退出页面)

