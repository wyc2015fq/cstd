

#ifndef __MFC_GDI_H__
#define __MFC_GDI_H__

//#include "typedef.h"
#include "color.hpp"
#include "mfc/DumpContext.hpp"
#include "PixelFormats.h"
#include "imaging.h"

typedef struct { void* x; }* Handle;


struct CGdiObject {
	virtual void Releses() = 0;
};

struct CPen : public CGdiObject
{
	virtual int GetLogPen(LOGPEN* pLogPen) = 0;
	virtual int GetExtLogPen(EXTLOGPEN* pLogPen) = 0;
	virtual void Dump(CDumpContext& dc) const = 0;
};

CPen* gdiCreatePen(int nPenStyle, int nWidth, COLORREF crColor);
CPen* gdiCreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
CPen* gdiCreatePenIndirect(LPLOGPEN lpLogPen);



#include "types.h"
#include "matrix.hpp"


struct Region;
struct Graphics;
struct GraphicsPath;

struct Region
{
	virtual void Releses() = 0;
	virtual Region* Clone() const = 0;

	virtual CError MakeInfinite() = 0;
	virtual CError MakeEmpty() = 0;

	virtual UINT GetDataSize() const = 0;

	// buffer     - where to put the data
	// bufferSize - how big the buffer is (should be at least as big as GetDataSize())
	// sizeFilled - if not NULL, this is an OUT param that says how many bytes
	//              of data were written to the buffer.

	virtual CError GetData(OUT BYTE* buffer, IN UINT bufferSize, OUT UINT* sizeFilled = NULL) const = 0;
	virtual CError Intersect(IN const Rect& rect) = 0;
	virtual CError Intersect(IN const RectF& rect) = 0;
	virtual CError Intersect(IN const GraphicsPath* path) = 0;
	virtual CError Intersect(IN const Region* region) = 0;
	virtual CError Union(IN const Rect& rect) = 0;
	virtual CError Union(IN const RectF& rect) = 0;
	virtual CError Union(IN const GraphicsPath* path) = 0;
	virtual CError Union(IN const Region* region) = 0;
	virtual CError Xor(IN const Rect& rect) = 0;
	virtual CError Xor(IN const RectF& rect) = 0;
	virtual CError Xor(IN const GraphicsPath* path) = 0;
	virtual CError Xor(IN const Region* region) = 0;
	virtual CError Exclude(IN const Rect& rect) = 0;
	virtual CError Exclude(IN const RectF& rect) = 0;
	virtual CError Exclude(IN const GraphicsPath* path) = 0;
	virtual CError Exclude(IN const Region* region) = 0;
	virtual CError Complement(IN const Rect& rect) = 0;
	virtual CError Complement(IN const RectF& rect) = 0;
	virtual CError Complement(IN const GraphicsPath* path) = 0;
	virtual CError Complement(IN const Region* region) = 0;
	virtual CError Translate(IN REAL dx, IN REAL dy) = 0;
	virtual CError Translate(IN INT dx, IN INT dy) = 0;
	virtual CError Transform(IN const Matrix* matrix) = 0;
	virtual CError GetBounds(OUT Rect* rect, IN const Graphics* g) const = 0;
	virtual CError GetBounds(OUT RectF* rect, IN const Graphics* g) const = 0;
	virtual HRGN GetHandle(IN const Graphics * g) const = 0;
	virtual BOOL IsEmpty(IN const Graphics *g) const = 0;
	virtual BOOL IsInfinite(IN const Graphics *g) const = 0;
	
	BOOL IsVisible(IN INT x, IN INT y, IN const Graphics* g = NULL) const
	{
		return IsVisible(Point(x, y), g);
	}

	virtual BOOL IsVisible(IN const Point& point, IN const Graphics* g = NULL) const = 0;

	BOOL IsVisible(IN REAL x, IN REAL y, IN const Graphics* g = NULL) const
	{
		return IsVisible(PointF(x, y), g);
	}

	virtual BOOL IsVisible(IN const PointF& point, IN const Graphics* g = NULL) const = 0;

	BOOL IsVisible(IN INT x,
		IN INT y,
		IN INT width,
		IN INT height,
		IN const Graphics* g) const
	{
		return IsVisible(Rect(x, y, width, height), g);
	}

	virtual BOOL IsVisible(IN const Rect& rect, IN const Graphics* g = NULL) const = 0;

	BOOL IsVisible(IN REAL x,
		IN REAL y,
		IN REAL width,
		IN REAL height,
		IN const Graphics* g = NULL) const
	{
		return IsVisible(RectF(x, y, width, height), g);
	}

	virtual BOOL IsVisible(IN const RectF& rect,
		IN const Graphics* g = NULL) const;

	virtual BOOL Equals(IN const Region* region, IN const Graphics* g) const = 0;

	virtual UINT GetRegionScansCount(IN const Matrix* matrix) const = 0;
	virtual CError GetRegionScans(IN const Matrix* matrix, OUT RectF* rects, OUT INT* count) const = 0;
	virtual CError GetRegionScans(IN const Matrix* matrix, OUT Rect*  rects, OUT INT* count) const = 0;
	virtual CError GetLastStatus() const = 0;

	virtual Region* Clone() = 0;
	virtual Region& Equal(const Region* region) = 0;
	////
};

struct RegionFactory {
	virtual Region* createRegion() = 0;
	virtual Region* createRegion(IN const RectF& rect) = 0;
	virtual Region* createRegion(IN const Rect& rect) = 0;
	virtual Region* createRegion(IN const GraphicsPath* path) = 0;
	virtual Region* createRegion(IN const BYTE* regionData, IN INT size) = 0;
};


struct Font;
struct Graphics;
struct GraphicsPath;
struct FontCollection;

//--------------------------------------------------------------------------
// FontFamily
//--------------------------------------------------------------------------

struct FontFamily
{
	virtual void Releses() = 0;

	virtual CError GetFamilyName(__out_ecount(LF_FACESIZE) LPWSTR    name, IN LANGID language = 0) const = 0;
	virtual FontFamily * Clone() const = 0;
	virtual BOOL    IsAvailable() const = 0;
	virtual BOOL    IsStyleAvailable(IN INT style) const = 0;

	virtual UINT16  GetEmHeight(IN INT style) const = 0;
	virtual UINT16  GetCellAscent(IN INT style) const = 0;
	virtual UINT16  GetCellDescent(IN INT style) const = 0;
	virtual UINT16  GetLineSpacing(IN INT style) const = 0;
};

struct FontFamilyFactory {
	virtual FontFamily* createFontFamily() = 0;
	virtual	FontFamily* createFontFamily(IN const WCHAR *name, IN const FontCollection *fontCollection = NULL) = 0;
	virtual const FontFamily *GenericSansSerif() = 0;
	virtual const FontFamily *GenericSerif() = 0;
	virtual const FontFamily *GenericMonospace() = 0;
};


//--------------------------------------------------------------------------
// Font
//--------------------------------------------------------------------------

struct Font
{
	virtual CError GetLogFontA(IN const Graphics* g, OUT  LOGFONTA * logfontA) const = 0;
	virtual CError GetLogFontW(IN const Graphics* g, OUT LOGFONTW * logfontW) const = 0;

	virtual Font* Clone() const = 0;
	
	virtual BOOL        IsAvailable()   const = 0;
	virtual INT         GetStyle()      const = 0;
	virtual REAL        GetSize()       const = 0;
	virtual Unit        GetUnit()       const = 0;
	virtual CError      GetLastStatus() const = 0;
	virtual REAL        GetHeight(IN const Graphics *graphics) const = 0;
	virtual REAL        GetHeight(IN REAL dpi) const = 0;
	virtual CError GetFamily(OUT FontFamily *family) const = 0;
	virtual Font& Equal(const Font & o) = 0;
};

struct FontFactory {
	virtual Font* createFont(IN HDC hdc) = 0;
	virtual Font* createFont(IN HDC hdc, IN const LOGFONTA* logfont) = 0;
	virtual Font* createFont(IN HDC hdc, IN const LOGFONTW* logfont) = 0;
	virtual Font* createFont(IN HDC hdc, IN const HFONT hfont) = 0;
	virtual Font* createFont(
		IN const FontFamily * family,
		IN REAL         emSize,
		IN INT          style = FontStyleRegular,
		IN Unit         unit = UnitPoint
	) = 0;

	virtual Font* createFont(
		IN const WCHAR *           familyName,
		IN REAL                    emSize,
		IN INT                     style = FontStyleRegular,
		IN Unit                    unit = UnitPoint,
		IN const FontCollection *  fontCollection = NULL
	) = 0;
};


//--------------------------------------------------------------------------
// Font Collection
//--------------------------------------------------------------------------

struct FontCollection
{
	virtual INT GetFamilyCount() const = 0;

	virtual CError GetFamilies(
		IN INT           numSought,
		OUT FontFamily * gpfamilies,
		OUT INT        * numFound
	) const = 0;
	virtual CError AddFontFile(IN const WCHAR* filename) = 0;
	virtual CError AddMemoryFont(IN const VOID* memory, IN INT length) = 0;
};


struct FontCollectionFactory {
	virtual FontCollection* createFontCollection() = 0;
	virtual FontCollection* createInstalledFontCollection() = 0;
	virtual FontCollection* createPrivateFontCollection() = 0;
};


//--------------------------------------------------------------------------
// Abstract base struct for Image and Metafile
//--------------------------------------------------------------------------

struct Image
{
	virtual Image* Clone() = 0;
	virtual ImageType GetType() const = 0;
	virtual CError GetPhysicalDimension(OUT SizeF* size) = 0;
	virtual CError GetBounds(OUT RectF* srcRect, OUT Unit* srcUnit) = 0;
	virtual UINT GetWidth() = 0;
	virtual UINT GetHeight() = 0;
	virtual REAL GetHorizontalResolution() = 0;
	virtual REAL GetVerticalResolution() = 0;
	virtual UINT GetFlags() = 0;
	virtual CError GetRawFormat(OUT GUID *format) = 0;
	virtual PixelFormat GetPixelFormat() = 0;

	virtual INT GetPaletteSize() = 0;
	virtual CError GetPalette(OUT ColorPalette* palette, IN INT size) = 0;
	virtual CError SetPalette(IN const ColorPalette* palette) = 0;
	virtual UINT GetFrameDimensionsCount() = 0;
	virtual CError GetFrameDimensionsList(OUT GUID* dimensionIDs, IN UINT count) = 0;
	virtual UINT GetFrameCount(IN const GUID* dimensionID) = 0;
	virtual CError SelectActiveFrame(IN const GUID* dimensionID, IN UINT frameIndex) = 0;

#if 0
	virtual CError RotateFlip(IN RotateFlipType rotateFlipType) = 0;
	virtual UINT GetPropertyCount() = 0;
	virtual CError GetPropertyIdList(IN UINT numOfProperty, OUT PROPID* list) = 0;
	virtual UINT GetPropertyItemSize(IN PROPID propId) = 0;
	virtual Image* GetThumbnailImage(IN UINT thumbWidth, IN UINT thumbHeight, IN GetThumbnailImageAbort callback = NULL, IN VOID* callbackData = NULL) = 0;
	virtual CError GetPropertyItem(IN PROPID propId, IN UINT propSize, OUT PropertyItem* buffer) = 0;
	virtual CError GetPropertySize(OUT UINT* totalBufferSize, OUT UINT* numProperties) = 0;
	virtual CError GetAllPropertyItems(IN UINT totalBufferSize, IN UINT numProperties, OUT PropertyItem* allItems) = 0;
	virtual CError RemovePropertyItem(IN PROPID propId) = 0;
	virtual CError SetPropertyItem(IN const PropertyItem* item) = 0;
	virtual UINT  GetEncoderParameterListSize(IN const CLSID* clsidEncoder) = 0;
	virtual CError GetEncoderParameterList(IN const CLSID* clsidEncoder, IN UINT size, OUT EncoderParameters* buffer) = 0;
#endif
};

struct ImageFactory {
	virtual Image* crateImage() = 0;
};

struct ImageIO {
	virtual CError loadImage(Image* image, IN const WCHAR* filename, IN BOOL useEmbeddedColorManagement = FALSE) = 0;
	virtual CError loadImage(Image* image, IN IStream* stream,IN BOOL useEmbeddedColorManagement = FALSE);

	virtual CError saveImage(Image* image, IN const WCHAR* filename, IN const EncoderParameters *encoderParams = NULL) = 0;
	virtual CError saveImage(Image* image, IN IStream* stream, IN const EncoderParameters *encoderParams = NULL) = 0;
	virtual CError saveAddImage(IN Image* newImage, IN const EncoderParameters* encoderParams);
};

struct Bitmap : public Image
{
public:
	friend struct Image;
	friend struct CachedBitmap;

	Bitmap(
		IN const WCHAR *filename,
		IN BOOL useEmbeddedColorManagement = FALSE
	);

	Bitmap(
		IN IStream *stream,
		IN BOOL useEmbeddedColorManagement = FALSE
	);

	static Bitmap* FromFile(
		IN const WCHAR *filename,
		IN BOOL useEmbeddedColorManagement = FALSE
	);

	static Bitmap* FromStream(
		IN IStream *stream,
		IN BOOL useEmbeddedColorManagement = FALSE
	);

	Bitmap(IN INT width,
		IN INT height,
		IN INT stride, PixelFormat format,
		IN BYTE* scan0);
	Bitmap(IN INT width,
		IN INT height,
		IN PixelFormat format = PixelFormat32bppARGB);
	Bitmap(IN INT width,
		IN INT height,
		IN  Graphics* target);

	Bitmap* Clone(IN const Rect& rect,
		IN PixelFormat format);
	Bitmap* Clone(IN INT x,
		IN INT y,
		IN INT width,
		IN INT height,
		IN PixelFormat format);
	Bitmap* Clone(IN const RectF& rect,
		IN PixelFormat format);
	Bitmap* Clone(IN REAL x,
		IN REAL y,
		IN REAL width,
		IN REAL height,
		IN PixelFormat format);

	CError LockBits(IN const Rect* rect,
		IN UINT flags,
		IN PixelFormat format,
		OUT BitmapData* lockedBitmapData);
	CError UnlockBits(IN BitmapData* lockedBitmapData);
	CError GetPixel(IN INT x,
		IN INT y,
		OUT Color *color);
	CError SetPixel(IN INT x,
		IN INT y,
		IN const Color &color);

	CError ConvertFormat(
		PixelFormat format,
		enum DitherType dithertype,
		enum PaletteType palettetype,
		ColorPalette *palette,
		REAL alphaThresholdPercent
	);

	// The palette must be allocated and count must be set to the number of
	// entries in the palette. If there are not enough, the API will fail.

	static CError InitializePalette(
		IN OUT ColorPalette *palette,  // Palette to initialize.
		PaletteType palettetype,       // palette enumeration type.
		INT optimalColors,             // how many optimal colors
		BOOL useTransparentColor,      // add a transparent color to the palette.
		Bitmap *bitmap                 // optional bitmap for median cut.
	);
#if 0
	CError ApplyEffect(Effect *effect, RECT *ROI);

	static CError
		ApplyEffect(
			IN  Bitmap **inputs,
			IN  INT numInputs,
			IN  Effect *effect,
			IN  RECT *ROI,           // optional parameter.
			OUT RECT *outputRect,    // optional parameter.
			OUT Bitmap **output
		);

	CError GetHistogram(
		IN HistogramFormat format,
		IN UINT NumberOfEntries,
		__out_bcount(sizeof(UINT) * 256) UINT *channel0,
		__out_bcount(sizeof(UINT) * 256) UINT *channel1,
		__out_bcount(sizeof(UINT) * 256) UINT *channel2,
		__out_bcount(sizeof(UINT) * 256) UINT *channel3
	);

	static CError GetHistogramSize(
		IN HistogramFormat format,
		OUT UINT *NumberOfEntries
	);

	CError SetResolution(IN REAL xdpi,
		IN REAL ydpi);

	Bitmap(IN IDirectDrawSurface7* surface);
	Bitmap(IN const BITMAPINFO* gdiBitmapInfo,
		IN VOID* gdiBitmapData);
	Bitmap(IN HBITMAP hbm,
		IN HPALETTE hpal);
	Bitmap(IN HICON hicon);
	Bitmap(IN HINSTANCE hInstance,
		IN const WCHAR * bitmapName);
	static Bitmap* FromDirectDrawSurface7(IN IDirectDrawSurface7* surface);
	static Bitmap* FromBITMAPINFO(IN const BITMAPINFO* gdiBitmapInfo,
		IN VOID* gdiBitmapData);
	static Bitmap* FromHBITMAP(IN HBITMAP hbm,
		IN HPALETTE hpal);
	static Bitmap* FromHICON(IN HICON hicon);
	static Bitmap* FromResource(IN HINSTANCE hInstance,
		IN const WCHAR * bitmapName);

	CError GetHBITMAP(IN const Color& colorBackground,
		OUT HBITMAP *hbmReturn);
	CError GetHICON(HICON *hicon);


	Bitmap(const Bitmap &);
	Bitmap& operator=(const Bitmap &);

#endif
};

struct CustomLineCap
{
public:
	friend struct Pen;

	CustomLineCap(
		IN const GraphicsPath* fillPath,
		IN const GraphicsPath* strokePath,
		IN LineCap baseCap = LineCapFlat,
		IN REAL baseInset = 0
	);
	virtual ~CustomLineCap();

	CustomLineCap* Clone() const;

	// This changes both the start and end cap.

	CError SetStrokeCap(IN LineCap strokeCap)
	{
		return SetStrokeCaps(strokeCap, strokeCap);
	}

	CError SetStrokeCaps(IN LineCap startCap,
		IN LineCap endCap);
	CError GetStrokeCaps(OUT LineCap* startCap,
		OUT LineCap* endCap) const;
	CError SetStrokeJoin(IN LineJoin lineJoin);
	LineJoin GetStrokeJoin() const;
	CError SetBaseCap(IN LineCap baseCap);
	LineCap GetBaseCap() const;
	CError SetBaseInset(IN REAL inset);
	REAL GetBaseInset() const;
	CError SetWidthScale(IN REAL widthScale);
	REAL GetWidthScale() const;
	CError GetLastStatus() const;


	CustomLineCap();


	CustomLineCap(const CustomLineCap &);
	CustomLineCap& operator=(const CustomLineCap &);


};

struct CachedBitmap
{
	friend Graphics;

public:
	CachedBitmap(IN Bitmap *bitmap,
		IN Graphics *graphics);
	virtual ~CachedBitmap();

	CError GetLastStatus() const;


	CachedBitmap(const CachedBitmap &);
	CachedBitmap& operator=(const CachedBitmap &);

};

#if 0

struct Metafile : public Image
{
public:
	friend struct Image;

	// Playback a metafile from a HMETAFILE
	// If deleteWmf is TRUE, then when the metafile is deleted,
	// the hWmf will also be deleted.  Otherwise, it won't be.

	static CError GetMetafileHeader(
		IN HMETAFILE                       hWmf,
		IN const WmfPlaceableFileHeader *  wmfPlaceableFileHeader,
		OUT MetafileHeader *               header
	);

	static CError GetMetafileHeader(
		IN HENHMETAFILE        hEmf,
		OUT MetafileHeader *   header
	);

	static CError GetMetafileHeader(
		IN const WCHAR*        filename,
		OUT MetafileHeader *   header
	);

	static CError GetMetafileHeader(
		IN IStream *           stream,
		OUT MetafileHeader *   header
	);

	CError GetMetafileHeader(
		OUT MetafileHeader *    header
	) const;

	// Once this method is called, the Metafile object is in an invalid state
	// and can no longer be used.  It is the responsiblity of the caller to
	// invoke DeleteEnhMetaFile to delete this hEmf.

	HENHMETAFILE GetHENHMETAFILE();

	// Used in conjuction with Graphics::EnumerateMetafile to play an EMF+
	// The data must be DWORD aligned if it's an EMF or EMF+.  It must be
	// WORD aligned if it's a WMF.

	CError PlayRecord(
		IN EmfPlusRecordType   recordType,
		IN UINT                flags,
		IN UINT                dataSize,
		IN const BYTE *        data
	) const;

	// If you're using a printer HDC for the metafile, but you want the
	// metafile rasterized at screen resolution, then use this API to set
	// the rasterization dpi of the metafile to the screen resolution,
	// e.g. 96 dpi or 120 dpi.

	CError SetDownLevelRasterizationLimit(
		IN UINT     metafileRasterizationLimitDpi
	);

	UINT GetDownLevelRasterizationLimit() const;

	static UINT Metafile::EmfToWmfBits(
		IN HENHMETAFILE       hemf,
		IN UINT               cbData16,
		OUT LPBYTE            pData16,
		IN INT                iMapMode = MM_ANISOTROPIC,
		IN INT                eFlags = EmfToWmfBitsFlagsDefault
	);

	CError ConvertToEmfPlus(
		IN const Graphics* refGraphics,
		IN OUT INT* conversionFailureFlag = NULL,
		IN EmfType emfType = EmfTypeEmfPlusOnly,
		IN const WCHAR* description = NULL
	);
	CError ConvertToEmfPlus(
		IN const Graphics* refGraphics,
		IN const WCHAR* filename,
		IN OUT INT* conversionFailureFlag = NULL,
		IN EmfType emfType = EmfTypeEmfPlusOnly,
		IN const WCHAR* description = NULL
	);
	CError ConvertToEmfPlus(
		IN const Graphics* refGraphics,
		IN IStream* stream,
		IN OUT INT* conversionFailureFlag = NULL,
		IN EmfType emfType = EmfTypeEmfPlusOnly,
		IN const WCHAR* description = NULL
	);

};

struct MetafileIO {

	Metafile(
		IN HMETAFILE                      hWmf,
		IN const WmfPlaceableFileHeader * wmfPlaceableFileHeader,
		IN BOOL                           deleteWmf = FALSE
	);

	// Playback a metafile from a HENHMETAFILE
	// If deleteEmf is TRUE, then when the metafile is deleted,
	// the hEmf will also be deleted.  Otherwise, it won't be.

	Metafile(
		IN HENHMETAFILE hEmf,
		IN BOOL deleteEmf = FALSE
	);

	Metafile(IN const WCHAR* filename);

	// Playback a WMF metafile from a file.

	Metafile(
		IN const WCHAR*                   filename,
		IN const WmfPlaceableFileHeader * wmfPlaceableFileHeader
	);

	Metafile(IN IStream* stream);

	// Record a metafile to memory.

	Metafile(
		IN HDC                 referenceHdc,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	// Record a metafile to memory.

	Metafile(
		IN HDC                 referenceHdc,
		IN const RectF &       frameRect,
		IN MetafileFrameUnit   frameUnit = MetafileFrameUnitGdi,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	// Record a metafile to memory.

	Metafile(
		IN HDC                 referenceHdc,
		IN const Rect &        frameRect,
		IN MetafileFrameUnit   frameUnit = MetafileFrameUnitGdi,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	Metafile(
		IN const WCHAR*        fileName,
		IN HDC                 referenceHdc,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	Metafile(
		IN const WCHAR*        fileName,
		IN HDC                 referenceHdc,
		IN const RectF &       frameRect,
		IN MetafileFrameUnit   frameUnit = MetafileFrameUnitGdi,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	Metafile(
		IN const WCHAR*        fileName,
		IN HDC                 referenceHdc,
		IN const Rect &        frameRect,
		IN MetafileFrameUnit   frameUnit = MetafileFrameUnitGdi,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	Metafile(
		IN IStream *           stream,
		IN HDC                 referenceHdc,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	Metafile(
		IN IStream *           stream,
		IN HDC                 referenceHdc,
		IN const RectF &       frameRect,
		IN MetafileFrameUnit   frameUnit = MetafileFrameUnitGdi,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);

	Metafile(
		IN IStream *           stream,
		IN HDC                 referenceHdc,
		IN const Rect &        frameRect,
		IN MetafileFrameUnit   frameUnit = MetafileFrameUnitGdi,
		IN EmfType             type = EmfTypeEmfPlusDual,
		IN const WCHAR *       description = NULL
	);
};
#endif


#endif // __MFC_GDI_H__
