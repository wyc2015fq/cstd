
#ifndef __GDI_GRAPHICS_H__
#define __GDI_GRAPHICS_H__

struct Region;
struct GraphicsPath;
struct Image;
struct Bitmap;
struct Metafile;
struct Font;
struct FontFamily;
struct FontCollection;
struct CachedBitmap;
struct Brush;
struct Pen;

//#include "string_format.hpp"
struct StringFormat {};
struct ImageAttributes {};
struct Effect {};
typedef BOOL(*EnumerateMetafileProc)(EmfPlusRecordType, UINT, UINT, const BYTE*, VOID*);
typedef BOOL(*ImageAbort)(VOID *);
typedef ImageAbort DrawImageAbort;
typedef ImageAbort GetThumbnailImageAbort;

struct Graphics
{
  virtual VOID Flush(IN FlushIntention intention = FlushIntentionFlush) = 0;

  //------------------------------------------------------------------------
  // GDI Interop methods
  //------------------------------------------------------------------------

  // Locks the graphics until ReleaseDC is called

  virtual HDC GetHDC() = 0;
  virtual void Release() = 0;

  //------------------------------------------------------------------------
  // Rendering modes
  //------------------------------------------------------------------------
  virtual CError SetRenderingOrigin(IN INT x, IN INT y) = 0;
  virtual CError GetRenderingOrigin(OUT INT *x, OUT INT *y) = 0;
  virtual CError SetCompositingMode(IN CompositingMode compositingMode) = 0;
  virtual CompositingMode GetCompositingMode() const = 0;
  virtual CError SetCompositingQuality(IN CompositingQuality compositingQuality) = 0;
  virtual CompositingQuality GetCompositingQuality() const;
  virtual CError SetTextRenderingHint(IN TextRenderingHint newMode) = 0;
  virtual TextRenderingHint GetTextRenderingHint() const = 0;
  virtual CError SetTextContrast(IN UINT contrast) = 0;
  virtual UINT GetTextContrast() const = 0;
  virtual InterpolationMode GetInterpolationMode() const;
  virtual CError SetInterpolationMode(IN InterpolationMode interpolationMode);
  virtual SmoothingMode GetSmoothingMode() const;
  virtual CError SetSmoothingMode(IN SmoothingMode smoothingMode);
  virtual PixelOffsetMode GetPixelOffsetMode() const;
  virtual CError SetPixelOffsetMode(IN PixelOffsetMode pixelOffsetMode);

  //------------------------------------------------------------------------
  // Manipulate current world transform
  //------------------------------------------------------------------------
  virtual CError SetTransform(IN const Matrix* matrix) = 0;
  virtual CError ResetTransform();
  virtual CError MultiplyTransform(IN const Matrix* matrix, IN MatrixOrder order = MatrixOrderPrepend);
  virtual CError TranslateTransform(IN REAL dx, IN REAL dy, IN MatrixOrder order = MatrixOrderPrepend);
  virtual CError ScaleTransform(IN REAL sx, IN REAL sy, IN MatrixOrder order = MatrixOrderPrepend);
  virtual CError RotateTransform(IN REAL angle, IN MatrixOrder order = MatrixOrderPrepend);
  virtual CError GetTransform(OUT Matrix* matrix) const;
  virtual CError SetPageUnit(IN Unit unit);
  virtual CError SetPageScale(IN REAL scale);
  virtual Unit GetPageUnit() const;
  virtual REAL GetPageScale() const;
  virtual REAL GetDpiX() const;
  virtual REAL GetDpiY() const;
  virtual CError TransformPoints(IN CoordinateSpace destSpace,
    IN CoordinateSpace srcSpace,
    IN OUT PointF* pts,
    IN INT count) const;
  virtual CError TransformPoints(IN CoordinateSpace destSpace,
    IN CoordinateSpace srcSpace,
    IN OUT Point* pts,
    IN INT count) const;


  //------------------------------------------------------------------------
  // GetNearestColor (for <= 8bpp surfaces). Note: Alpha is ignored.
  //------------------------------------------------------------------------
  virtual CError GetNearestColor(IN OUT Color* color) const;
  virtual CError DrawLine(IN const Pen* pen,
    IN REAL x1,
    IN REAL y1,
    IN REAL x2,
    IN REAL y2) = 0;

  virtual CError DrawLine(IN const Pen* pen,
    IN const PointF& pt1,
    IN const PointF& pt2)
  {
    return DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y);
  }

  virtual CError DrawLines(IN const Pen* pen,
    IN const PointF* points,
    IN INT count) = 0;


  virtual CError DrawLine(IN const Pen* pen,
    IN INT x1,
    IN INT y1,
    IN INT x2,
    IN INT y2) = 0;

  virtual CError DrawLine(IN const Pen* pen, IN const Point& pt1, IN const Point& pt2) {
    return DrawLine(pen, pt1.X, pt1.Y, pt2.X, pt2.Y);
  }

  virtual CError DrawLines(IN const Pen* pen,
    IN const Point* points,
    IN INT count) = 0;

  virtual CError DrawArc(IN const Pen* pen,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height,
    IN REAL startAngle,
    IN REAL sweepAngle) = 0;

  virtual CError DrawArc(IN const Pen* pen,
    IN const RectF& rect,
    IN REAL startAngle,
    IN REAL sweepAngle)
  {
    return DrawArc(pen, rect.X, rect.Y, rect.Width, rect.Height,
      startAngle, sweepAngle);
  }

  virtual CError DrawArc(IN const Pen* pen,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height,
    IN REAL startAngle,
    IN REAL sweepAngle) = 0;

  virtual CError DrawArc(IN const Pen* pen,
    IN const Rect& rect,
    IN REAL startAngle,
    IN REAL sweepAngle)
  {
    return DrawArc(pen,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height,
      startAngle,
      sweepAngle);
  }

  virtual CError DrawBezier(IN const Pen* pen,
    IN REAL x1,
    IN REAL y1,
    IN REAL x2,
    IN REAL y2,
    IN REAL x3,
    IN REAL y3,
    IN REAL x4,
    IN REAL y4) = 0;

  CError DrawBezier(IN const Pen* pen,
    IN const PointF& pt1,
    IN const PointF& pt2,
    IN const PointF& pt3,
    IN const PointF& pt4)
  {
    return DrawBezier(pen,
      pt1.X,
      pt1.Y,
      pt2.X,
      pt2.Y,
      pt3.X,
      pt3.Y,
      pt4.X,
      pt4.Y);
  }

  virtual CError DrawBeziers(IN const Pen* pen,
    IN const PointF* points,
    IN INT count) = 0;


  virtual CError DrawBezier(IN const Pen* pen,
    IN INT x1,
    IN INT y1,
    IN INT x2,
    IN INT y2,
    IN INT x3,
    IN INT y3,
    IN INT x4,
    IN INT y4) = 0;

  CError DrawBezier(IN const Pen* pen,
    IN const Point& pt1,
    IN const Point& pt2,
    IN const Point& pt3,
    IN const Point& pt4)
  {
    return DrawBezier(pen,
      pt1.X,
      pt1.Y,
      pt2.X,
      pt2.Y,
      pt3.X,
      pt3.Y,
      pt4.X,
      pt4.Y);
  }

  virtual CError DrawBeziers(IN const Pen* pen,
    IN const Point* points,
    IN INT count) = 0;

  CError DrawRectangle(IN const Pen* pen,
    IN const RectF& rect)
  {
    return DrawRectangle(pen, rect.X, rect.Y, rect.Width, rect.Height);
  }

  virtual CError DrawRectangle(IN const Pen* pen,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height) = 0;

  virtual CError DrawRectangles(IN const Pen* pen,
    IN const RectF* rects,
    IN INT count) = 0;

  CError DrawRectangle(IN const Pen* pen,
    IN const Rect& rect)
  {
    return DrawRectangle(pen,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height);
  }

  virtual CError DrawRectangle(IN const Pen* pen,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height) = 0;

  virtual CError DrawRectangles(IN const Pen* pen,
    IN const Rect* rects,
    IN INT count) = 0;

  CError DrawEllipse(IN const Pen* pen,
    IN const RectF& rect)
  {
    return DrawEllipse(pen, rect.X, rect.Y, rect.Width, rect.Height);
  }

  virtual CError DrawEllipse(IN const Pen* pen,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height) = 0;

  CError DrawEllipse(IN const Pen* pen,
    IN const Rect& rect)
  {
    return DrawEllipse(pen,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height);
  }

  virtual CError DrawEllipse(IN const Pen* pen,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height) = 0;

  CError DrawPie(IN const Pen* pen,
    IN const RectF& rect,
    IN REAL startAngle,
    IN REAL sweepAngle)
  {
    return DrawPie(pen,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height,
      startAngle,
      sweepAngle);
  }

  virtual CError DrawPie(IN const Pen* pen,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height,
    IN REAL startAngle,
    IN REAL sweepAngle) = 0;

  CError DrawPie(IN const Pen* pen,
    IN const Rect& rect,
    IN REAL startAngle,
    IN REAL sweepAngle)
  {
    return DrawPie(pen,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height,
      startAngle,
      sweepAngle);
  }

  virtual CError DrawPie(IN const Pen* pen,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height,
    IN REAL startAngle,
    IN REAL sweepAngle) = 0;

  virtual CError DrawPolygon(IN const Pen* pen,
    IN const PointF* points,
    IN INT count) = 0;

  virtual CError DrawPolygon(IN const Pen* pen,
    IN const Point* points,
    IN INT count) = 0;

  virtual CError DrawPath(IN const Pen* pen,
    IN const GraphicsPath* path) = 0;

  virtual CError DrawCurve(IN const Pen* pen,
    IN const PointF* points,
    IN INT count) = 0;

  virtual CError DrawCurve(IN const Pen* pen,
    IN const PointF* points,
    IN INT count,
    IN REAL tension) = 0;

  virtual CError DrawCurve(IN const Pen* pen,
    IN const PointF* points,
    IN INT count,
    IN INT offset,
    IN INT numberOfSegments,
    IN REAL tension = 0.5f) = 0;

  virtual CError DrawCurve(IN const Pen* pen,
    IN const Point* points,
    IN INT count) = 0;

  virtual CError DrawCurve(IN const Pen* pen,
    IN const Point* points,
    IN INT count,
    IN REAL tension) = 0;

  virtual CError DrawCurve(IN const Pen* pen,
    IN const Point* points,
    IN INT count,
    IN INT offset,
    IN INT numberOfSegments,
    IN REAL tension = 0.5f) = 0;

  virtual CError DrawClosedCurve(IN const Pen* pen,
    IN const PointF* points,
    IN INT count) = 0;

  virtual CError DrawClosedCurve(IN const Pen *pen,
    IN const PointF* points,
    IN INT count,
    IN REAL tension) = 0;

  virtual CError DrawClosedCurve(IN const Pen* pen,
    IN const Point* points,
    IN INT count) = 0;

  virtual CError DrawClosedCurve(IN const Pen *pen,
    IN const Point* points,
    IN INT count,
    IN REAL tension) = 0;

  virtual CError Clear(IN const Color &color) = 0;

  CError FillRectangle(IN const Brush* brush,
    IN const RectF& rect)
  {
    return FillRectangle(brush, rect.X, rect.Y, rect.Width, rect.Height);
  }

  virtual CError FillRectangle(IN const Brush* brush,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height) = 0;

  virtual CError FillRectangles(IN const Brush* brush,
    IN const RectF* rects,
    IN INT count) = 0;

  CError FillRectangle(IN const Brush* brush,
    IN const Rect& rect)
  {
    return FillRectangle(brush,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height);
  }

  virtual CError FillRectangle(IN const Brush* brush,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height) = 0;

  virtual CError FillRectangles(IN const Brush* brush,
    IN const Rect* rects,
    IN INT count) = 0;

  CError FillPolygon(IN const Brush* brush,
    IN const PointF* points,
    IN INT count)
  {
    return FillPolygon(brush, points, count, FillModeAlternate);
  }

  virtual CError FillPolygon(IN const Brush* brush,
    IN const PointF* points,
    IN INT count,
    IN FillMode fillMode) = 0;

  CError FillPolygon(IN const Brush* brush,
    IN const Point* points,
    IN INT count)
  {
    return FillPolygon(brush, points, count, FillModeAlternate);
  }

  virtual CError FillPolygon(IN const Brush* brush,
    IN const Point* points,
    IN INT count,
    IN FillMode fillMode)
    = 0;

  CError FillEllipse(IN const Brush* brush,
    IN const RectF& rect)
  {
    return FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
  }

  virtual CError FillEllipse(IN const Brush* brush,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height) = 0;

  CError FillEllipse(IN const Brush* brush,
    IN const Rect& rect)
  {
    return FillEllipse(brush, rect.X, rect.Y, rect.Width, rect.Height);
  }

  virtual CError FillEllipse(IN const Brush* brush,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height) = 0;

  CError FillPie(IN const Brush* brush,
    IN const RectF& rect,
    IN REAL startAngle,
    IN REAL sweepAngle)
  {
    return FillPie(brush, rect.X, rect.Y, rect.Width, rect.Height,
      startAngle, sweepAngle);
  }

  virtual CError FillPie(IN const Brush* brush,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height,
    IN REAL startAngle,
    IN REAL sweepAngle) = 0;

  CError FillPie(IN const Brush* brush,
    IN const Rect& rect,
    IN REAL startAngle,
    IN REAL sweepAngle)
  {
    return FillPie(brush, rect.X, rect.Y, rect.Width, rect.Height,
      startAngle, sweepAngle);
  }

  virtual CError FillPie(IN const Brush* brush,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height,
    IN REAL startAngle,
    IN REAL sweepAngle) = 0;

  virtual CError FillPath(IN const Brush* brush,
    IN const GraphicsPath* path) = 0;

  virtual CError FillClosedCurve(IN const Brush* brush,
    IN const PointF* points,
    IN INT count) = 0;

  virtual CError FillClosedCurve(IN const Brush* brush,
    IN const PointF* points,
    IN INT count,
    IN FillMode fillMode,
    IN REAL tension = 0.5f) = 0;

  virtual CError FillClosedCurve(IN const Brush* brush,
    IN const Point* points, IN INT count) = 0;

  virtual CError FillClosedCurve(IN const Brush* brush,
    IN const Point* points,
    IN INT count,
    IN FillMode fillMode,
    IN REAL tension = 0.5f) = 0;

  virtual CError FillRegion(IN const Brush* brush,
    IN const Region* region) = 0;

  virtual CError DrawString(
    IN const WCHAR *string,
    IN INT length,
    IN const Font *font,
    IN const RectF &layoutRect,
    IN const StringFormat *stringFormat,
    IN const Brush *brush
  ) = 0;

  virtual CError DrawString(
    const WCHAR *string,
    INT length,
    const Font *font,
    const PointF &origin,
    const Brush *brush
  )
    = 0;

  virtual CError DrawString(
    const WCHAR *string,
    INT length,
    const Font *font,
    const PointF &origin,
    const StringFormat *stringFormat,
    const Brush *brush
  )
    = 0;

  virtual CError MeasureString(
    IN const WCHAR *string,
    IN INT length,
    IN const Font *font,
    IN const RectF &layoutRect,
    IN const StringFormat *stringFormat,
    OUT RectF *boundingBox,
    OUT INT *codepointsFitted = 0,
    OUT INT *linesFilled = 0
  ) const
    = 0;

  virtual CError
    MeasureString(
      IN const WCHAR *string,
      IN INT length,
      IN const Font *font,
      IN const SizeF &layoutRectSize,
      IN const StringFormat *stringFormat,
      OUT SizeF *size,
      OUT INT *codepointsFitted = 0,
      OUT INT *linesFilled = 0
    ) const
    = 0;

  virtual CError MeasureString(
    IN const WCHAR *string,
    IN INT length,
    IN const Font *font,
    IN const PointF &origin,
    IN const StringFormat *stringFormat,
    OUT RectF *boundingBox
  ) const
    = 0;

  virtual CError MeasureString(
    IN const WCHAR *string,
    IN INT length,
    IN const Font *font,
    IN const RectF &layoutRect,
    OUT RectF *boundingBox
  ) const
    = 0;

  virtual CError MeasureString(
    IN const WCHAR *string,
    IN INT length,
    IN const Font *font,
    IN const PointF &origin,
    OUT RectF *boundingBox
  ) const
    = 0;


  virtual CError
    MeasureCharacterRanges(
      IN const WCHAR *string,
      IN INT length,
      IN const Font *font,
      IN const RectF &layoutRect,
      IN const StringFormat *stringFormat,
      IN INT regionCount,
      OUT Region *regions
    ) const
    = 0;

  virtual CError DrawDriverString(
    IN const UINT16 *text,
    IN INT length,
    IN const Font *font,
    IN const Brush *brush,
    IN const PointF *positions,
    IN INT flags,
    IN const Matrix *matrix
  )
    = 0;

  virtual CError MeasureDriverString(
    IN const UINT16 *text,
    IN INT length,
    IN const Font *font,
    IN const PointF *positions,
    IN INT flags,
    IN const Matrix *matrix,
    OUT RectF *boundingBox
  ) const
    = 0;

  // Draw a cached bitmap on this graphics destination offset by
  // x, y. Note this will fail with WrongState if the CachedBitmap
  // native format differs from this Graphics.

  virtual CError DrawCachedBitmap(IN CachedBitmap *cb,
    IN INT x,
    IN INT y)
    = 0;

  CError DrawImage(IN Image* image,
    IN const PointF& point)
  {
    return DrawImage(image, point.X, point.Y);
  }

  virtual CError DrawImage(IN Image* image,
    IN REAL x,
    IN REAL y)
    = 0;

  CError DrawImage(IN Image* image,
    IN const RectF& rect)
  {
    return DrawImage(image, rect.X, rect.Y, rect.Width, rect.Height);
  }

  virtual CError DrawImage(IN Image* image,
    IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height)
    = 0;

  CError DrawImage(IN Image* image,
    IN const Point& point)
  {
    return DrawImage(image, point.X, point.Y);
  }

  virtual CError DrawImage(IN Image* image,
    IN INT x,
    IN INT y)
    = 0;

  CError DrawImage(IN Image* image,
    IN const Rect& rect)
  {
    return DrawImage(image,
      rect.X,
      rect.Y,
      rect.Width,
      rect.Height);
  }

  virtual CError DrawImage(IN Image* image,
    IN INT x,
    IN INT y,
    IN INT width,
    IN INT height) = 0;


  virtual CError DrawImage(IN Image* image,
    IN const PointF* destPoints,
    IN INT count)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN const Point* destPoints,
    IN INT count)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN REAL x,
    IN REAL y,
    IN REAL srcx,
    IN REAL srcy,
    IN REAL srcwidth,
    IN REAL srcheight,
    IN Unit srcUnit)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN const RectF& destRect,
    IN REAL srcx,
    IN REAL srcy,
    IN REAL srcwidth,
    IN REAL srcheight,
    IN Unit srcUnit,
    IN const ImageAttributes* imageAttributes = NULL,
    IN DrawImageAbort callback = NULL,
    IN VOID* callbackData = NULL)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN const PointF* destPoints,
    IN INT count,
    IN REAL srcx,
    IN REAL srcy,
    IN REAL srcwidth,
    IN REAL srcheight,
    IN Unit srcUnit,
    IN const ImageAttributes* imageAttributes = NULL,
    IN DrawImageAbort callback = NULL,
    IN VOID* callbackData = NULL)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN INT x,
    IN INT y,
    IN INT srcx,
    IN INT srcy,
    IN INT srcwidth,
    IN INT srcheight,
    IN Unit srcUnit)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN const Rect& destRect,
    IN INT srcx,
    IN INT srcy,
    IN INT srcwidth,
    IN INT srcheight,
    IN Unit srcUnit,
    IN const ImageAttributes* imageAttributes = NULL,
    IN DrawImageAbort callback = NULL,
    IN VOID* callbackData = NULL)
    = 0;

  virtual CError DrawImage(IN Image* image,
    IN const Point* destPoints,
    IN INT count,
    IN INT srcx,
    IN INT srcy,
    IN INT srcwidth,
    IN INT srcheight,
    IN Unit srcUnit,
    IN const ImageAttributes* imageAttributes = NULL,
    IN DrawImageAbort callback = NULL,
    IN VOID* callbackData = NULL)
    = 0;

  virtual CError DrawImage(
    IN Image *image,
    IN const RectF &destRect,
    IN const RectF &sourceRect,
    IN Unit srcUnit,
    IN const ImageAttributes *imageAttributes = NULL
  )
    = 0;

  virtual CError DrawImage(
    IN Image *image,
    IN RectF *sourceRect,
    IN Matrix *xForm,
    IN Effect *effect,
    IN ImageAttributes *imageAttributes,
    IN Unit srcUnit
  )
    = 0;

  // The following methods are for playing an EMF+ to a graphics
  // via the enumeration interface. Each record of the EMF+ is
  // sent to the callback (along with the callbackData). Then
  // the callback can invoke the Metafile::PlayRecord method
  // to play the particular record.

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const PointF & destPoint,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const Point & destPoint,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  ) = 0;


  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const RectF & destRect,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const Rect & destRect,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const PointF * destPoints,
    IN INT count,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const Point * destPoints,
    IN INT count,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const PointF & destPoint,
    IN const RectF & srcRect,
    IN Unit srcUnit,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const Point & destPoint,
    IN const Rect & srcRect,
    IN Unit srcUnit,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const RectF & destRect,
    IN const RectF & srcRect,
    IN Unit srcUnit,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const Rect & destRect,
    IN const Rect & srcRect,
    IN Unit srcUnit,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const PointF * destPoints,
    IN INT count,
    IN const RectF & srcRect,
    IN Unit srcUnit,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError EnumerateMetafile(
    IN const Metafile * metafile,
    IN const Point * destPoints,
    IN INT count,
    IN const Rect & srcRect,
    IN Unit srcUnit,
    IN EnumerateMetafileProc callback,
    IN VOID * callbackData = NULL,
    IN const ImageAttributes * imageAttributes = NULL
  )
    = 0;

  virtual CError SetClip(IN const Graphics* g,
    IN CombineMode combineMode = CombineModeReplace)
    = 0;

  virtual CError SetClip(IN const RectF& rect,
    IN CombineMode combineMode = CombineModeReplace)
    = 0;

  virtual CError SetClip(IN const Rect& rect,
    IN CombineMode combineMode = CombineModeReplace)
    = 0;

  virtual CError SetClip(IN const GraphicsPath* path,
    IN CombineMode combineMode = CombineModeReplace)
    = 0;

  virtual CError SetClip(IN const Region* region,
    IN CombineMode combineMode = CombineModeReplace)
    = 0;

  // This is different than the other SetClip methods because it assumes
  // that the HRGN is already in device units, so it doesn't transform
  // the coordinates in the HRGN.

  virtual CError SetClip(IN HRGN hRgn,
    IN CombineMode combineMode = CombineModeReplace)
    = 0;

  virtual CError IntersectClip(IN const RectF& rect)
    = 0;

  virtual CError IntersectClip(IN const Rect& rect)
    = 0;

  virtual CError IntersectClip(IN const Region* region)
    = 0;

  virtual CError ExcludeClip(IN const RectF& rect)
    = 0;

  virtual CError ExcludeClip(IN const Rect& rect)
    = 0;

  virtual CError ExcludeClip(IN const Region* region)
    = 0;

  virtual CError ResetClip()
    = 0;

  virtual CError TranslateClip(IN REAL dx,
    IN REAL dy)
    = 0;

  virtual CError TranslateClip(IN INT dx,
    IN INT dy)
    = 0;

  virtual CError GetClip(OUT Region* region) const
    = 0;

  virtual CError GetClipBounds(OUT RectF* rect) const
    = 0;

  virtual CError GetClipBounds(OUT Rect* rect) const
    = 0;

  virtual BOOL IsClipEmpty() const
    = 0;

  virtual CError GetVisibleClipBounds(OUT RectF *rect) const
    = 0;

  virtual CError GetVisibleClipBounds(OUT Rect *rect) const
    = 0;

  virtual BOOL IsVisibleClipEmpty() const
    = 0;

  virtual BOOL IsVisible(IN INT x,
    IN INT y) const
    = 0;

  virtual BOOL IsVisible(IN const Point& point) const
    = 0;

  BOOL IsVisible(IN INT x,
    IN INT y,
    IN INT width,
    IN INT height) const
  {
    return IsVisible(Rect(x, y, width, height));
  }

  virtual BOOL IsVisible(IN const Rect& rect) const
    = 0;

  BOOL IsVisible(IN REAL x,
    IN REAL y) const
  {
    return IsVisible(PointF(x, y));
  }

  virtual BOOL IsVisible(IN const PointF& point) const
    = 0;

  BOOL IsVisible(IN REAL x,
    IN REAL y,
    IN REAL width,
    IN REAL height) const
  {
    return IsVisible(RectF(x, y, width, height));
  }

  virtual BOOL IsVisible(IN const RectF& rect) const
    = 0;

  virtual GraphicsState Save() const = 0;

  virtual CError Restore(IN GraphicsState gstate) = 0;

  virtual GraphicsContainer BeginContainer(IN const RectF &dstrect,
    IN const RectF &srcrect,
    IN Unit unit) = 0;

  virtual GraphicsContainer BeginContainer(IN const Rect &dstrect,
    IN const Rect &srcrect,
    IN Unit unit) = 0;

  virtual GraphicsContainer BeginContainer() = 0;

  virtual CError EndContainer(IN GraphicsContainer state) = 0;

  // Only valid when recording metafiles.

  virtual CError AddMetafileComment(IN const BYTE * data,
    IN UINT sizeData)
    = 0;

  virtual HPALETTE GetHalftonePalette()
    = 0;

};


struct GraphicsFactory {
  virtual Graphics* createGraphics() = 0;
  virtual Graphics* fromHDC(IN HDC hdc) = 0;
  virtual Graphics* fromHWND(IN HWND hwnd, IN BOOL icm = FALSE) = 0;
  virtual Graphics* fromImage(IN Image *image) = 0;
};


#endif // __GDI_GRAPHICS_H__

