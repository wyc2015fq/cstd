
typedef float REAL;
typedef uint32 ARGB;
typedef uint64 ARGB64;
typedef UINT GraphicsState;
typedef UINT GraphicsContainer;
typedef struct {
  int X, Y, Width, Height;
} Rect;
typedef struct {
  REAL X, Y, Width, Height;
} RectF;
typedef struct {
  int X, Y;
} Point;
Point iPoint(int x, int y) {Point pt;pt.X = x, pt.Y = y; return pt;}
typedef struct {
  REAL X, Y;
} PointF;
typedef struct {
  int Width, Height;
} Size;
typedef struct {
  REAL Width, Height;
} SizeF;
typedef struct {
  INT Count;
  PointF* Points;
  BYTE* Types;
} PathData;
void PathData_Free(PathData* pathdata) {
  pfree(pathdata->Types);
  pathdata->Types = NULL;
  pathdata->Points = NULL;
  pathdata->Count = 0;
}
#define PathData_SetSize(pathdata, n)  PathData_SetSize_(pathdata, n, __FILE__, __LINE__)
int PathData_SetSize_(PathData* pathdata, int n, const char* file, int line) {
  if (n>0) {
    int sz = (sizeof(PointF)+sizeof(BYTE))*n;
    BYTE* p = (BYTE*)prealloc_(pathdata->Types, sz, file, line);
    if (p) {
      pathdata->Count = n;
      pathdata->Types = p;
      pathdata->Points = (PointF*)(p+n);
    }
    return p!=NULL;
  } else {
    PathData_Free(pathdata);
  }
  return 1;
}
typedef struct {
  INT First, Length;
} CharacterRange;
Status PointI2F(PointF** pptsF, const Point *points, INT count) {
  int i;
  PointF *ptsF = (PointF*)prealloc(*pptsF, sizeof(PointF) * count);
  if(!ptsF)
    return OutOfMemory;
  
  for(i = 0; i < count; i++){
    ptsF[i].X = (REAL)points[i].X;
    ptsF[i].Y = (REAL)points[i].Y;
  }
  *pptsF = ptsF;
  return Ok;
}


#define MAX_ARC_PTS (13)
#define MAX_DASHLEN (16) /* this is a limitation of gdi */
#define INCH_HIMETRIC (2540)

#define VERSION_MAGIC 0xdbc01001
#define TENSION_CONST (0.3)

typedef float REAL;

static inline INT gdip_round(REAL x)
{
    return (INT) floorf(x + 0.5);
}

static inline INT ceilr(REAL x)
{
    return (INT) ceilf(x);
}

static inline REAL deg2rad1(REAL degrees)
{
    return M_PI * degrees / 180.0;
}

static inline ARGB color_over(ARGB bg, ARGB fg)
{
    BYTE b, g, r, a;
    BYTE bg_alpha, fg_alpha;

    fg_alpha = (fg>>24)&0xff;

    if (fg_alpha == 0xff) return fg;

    if (fg_alpha == 0) return bg;

    bg_alpha = (((bg>>24)&0xff) * (0xff-fg_alpha)) / 0xff;

    if (bg_alpha == 0) return fg;

    a = bg_alpha + fg_alpha;
    b = ((bg&0xff)*bg_alpha + (fg&0xff)*fg_alpha)/a;
    g = (((bg>>8)&0xff)*bg_alpha + ((fg>>8)&0xff)*fg_alpha)/a;
    r = (((bg>>16)&0xff)*bg_alpha + ((fg>>16)&0xff)*fg_alpha)/a;

    return (a<<24)|(r<<16)|(g<<8)|b;
}

struct Matrix{
    REAL matrix[6];
};

struct AdjustableArrowCap {
  int x;
};
struct CustomLineCap{
  PathData pathdata;
  BOOL fill;      /* TRUE for fill, FALSE for stroke */
  LineCap cap;  /* as far as I can tell, this value is ignored */
  REAL inset;     /* how much to adjust the end of the line */
  LineJoin join;
  REAL scale;
};

typedef struct {
  PathData pathdata;
  FillMode fill;
  BOOL newfigure; /* whether the next drawing action starts a new figure */
  INT datalen; /* size of the arrays in pathdata */
} Path;

struct PathIterator{
  PathData pathdata;
  INT subpath_pos;    /* for NextSubpath methods */
  INT marker_pos;     /* for NextMarker methods */
  INT pathtype_pos;   /* for NextPathType methods */
};

struct AdustableArrowCap{
  CustomLineCap cap;
};

#if 0
struct Bitmap1 {
  INT width;
  INT height;
  PixelFormat format;
  ImageLockMode lockmode;
  INT numlocks;
  BYTE *bitmapbits;   /* pointer to the buffer we passed in BitmapLockBits */
  //HBITMAP hbitmap;
  //HDC hdc;
  BYTE *bits; /* actual image bits if this is a DIB */
  INT stride; /* stride of bits if this is a DIB */
  BYTE *own_bits; /* image bits that need to be freed with this object */
  INT lockx, locky; /* X and Y coordinates of the rect when a bitmap is locked for writing. */
  //IWICMetadataReader *metadata_reader; /* NULL if there is no metadata */
  UINT prop_count;
  //PropertyItem *prop_item; /* cached image properties */
};

struct Image1 {
  ImageType1 type;
  UINT flags;
  UINT frame_count, current_frame;
  ColorPalette *palette;
  REAL xres, yres;
};
#endif

struct CachedBitmap{
    Image *image;
};

struct Font{
    //FontFamily *family;
    //OUTLINETEXTMETRICW otm;
    REAL emSize; /* in font units */
    Unit unit;
};

struct StringFormat{
    INT attr;
    StringAlignment align;
    StringTrimming trimming;
    HotkeyPrefix hkprefix;
    StringAlignment vertalign;
    StringDigitSubstitute digitsub;
    INT tabcount;
    REAL firsttab;
    REAL *tabs;
    CharacterRange *character_ranges;
    INT range_count;
    BOOL generic_typographic;
};
typedef ushort WCHAR;
#define LF_FACESIZE 32
struct FontFamily {
  WCHAR FamilyName[LF_FACESIZE];
  UINT16 em_height, ascent, descent, line_spacing; /* in font units */
  int dpi;
};

struct FontCollection{
    FontFamily **FontFamilies;
    INT count;
    INT allocated;
};

/* internal use */
typedef enum RegionType
{
    RegionDataRect          = 0x10000000,
    RegionDataPath          = 0x10000001,
    RegionDataEmptyRect     = 0x10000002,
    RegionDataInfiniteRect  = 0x10000003,
} RegionType;

struct region_element
{
    DWORD type; /* Rectangle, Path, SpecialRectangle, or CombineMode */
    union
    {
        RectF rect;
        struct
        {
            Path path[1];
            struct
            {
                DWORD size;
                DWORD magic;
                DWORD count;
                DWORD flags;
            } pathheader;
        } pathdata;
        struct
        {
            struct region_element *left;  /* the original region */
            struct region_element *right; /* what *left was combined with */
        } combine;
    } elementdata;
};

struct Region{
    struct
    {
        DWORD size;
        DWORD checksum;
        DWORD magic;
        DWORD num_children;
    } header;
    region_element node;
};

typedef struct {
  BOOL enabled;
  ARGB low;
  ARGB high;
} color_key;

typedef struct {
  BOOL enabled;
  ColorMatrixFlags flags;
  ColorMatrix colormatrix;
  ColorMatrix graymatrix;
} color_matrix;

typedef struct {
  BOOL enabled;
  INT mapsize;
  ColorMap *colormap;
} color_remap_table;

typedef struct {
  ARGB outside_color;
  BOOL clamp;
  WrapMode wrap;
  color_key colorkeys[ColorAdjustTypeCount];
  color_matrix colormatrices[ColorAdjustTypeCount];
  color_remap_table colorremaptables[ColorAdjustTypeCount];
  BOOL gamma_enabled[ColorAdjustTypeCount];
  REAL gamma[ColorAdjustTypeCount];
} ImageAttributes;

struct SolidFill {
  ARGB color;
};

struct Hatch {
  HatchStyle hatchstyle;
  ARGB forecol;
  ARGB backcol;
};
struct PathGradient {
  Path* path;
  ARGB centercolor;
  WrapMode wrap;
  BOOL gamma;
  PointF center;
  PointF focus;
  REAL* blendfac;  /* blend factors */
  REAL* blendpos;  /* blend positions */
  INT blendcount;
  ARGB *surroundcolors;
  INT surroundcolorcount;
  ARGB* pblendcolor; /* preset blend colors */
  REAL* pblendpos; /* preset blend positions */
  INT pblendcount;
  Matrix transform;
};

Status  GdipClonePath(const Path* path, Path* clone)
{
  TRACE("(%p, %p)\n", path, clone);
  
  if(!path || !clone)
    return InvalidParameter;
  
  if(!PathData_SetSize(&clone->pathdata, path->datalen)){
    return OutOfMemory;
  }
  
  memcpy((clone)->pathdata.Points, path->pathdata.Points, path->datalen * sizeof(PointF));
  memcpy((clone)->pathdata.Types, path->pathdata.Types, path->datalen);
  
  return Ok;
}

Status PathGradient_Set(PathGradient* pg, Path* path, ARGB centercolor,WrapMode wrap,BOOL gamma,
                     PointF center,PointF focus, REAL* blendfac, REAL* blendpos,
                     INT blendcount, ARGB *surroundcolors,INT surroundcolorcount, ARGB* pblendcolor,
                     REAL* pblendpos,INT pblendcount, Matrix transform) {
  pg->path = path;
  pg->centercolor = centercolor;
  pg->wrap = wrap;
  pg->gamma = gamma;
  pg->center = center;
  pg->focus = focus;
  pg->center = center;
  pg->transform = transform;
  pg->blendcount = blendcount;
  pg->pblendcount = pblendcount;
  pg->surroundcolorcount = surroundcolorcount;
  pg->blendfac = blendfac;
  pg->blendpos = blendpos;
  pg->surroundcolors= surroundcolors;
  pg->pblendcolor= pblendcolor;
  pg->pblendpos= pblendpos;
  return Ok;
}
Status PathGradient_Clone(PathGradient* d, const PathGradient* s) {
  return PathGradient_Set(d, s->path, s->centercolor,s->wrap,s->gamma,
    s->center,s->focus,s->blendfac,s->blendpos,
    s->blendcount,s->surroundcolors,s->surroundcolorcount,s->pblendcolor,
                     s->pblendpos,s->pblendcount, s->transform);
}

struct LineGradient{
  PointF startpoint;
  PointF endpoint;
  ARGB startcolor;
  ARGB endcolor;
  RectF rect;
  WrapMode wrap;
  BOOL gamma;
  REAL* blendfac;  /* blend factors */
  REAL* blendpos;  /* blend positions */
  INT blendcount;
  ARGB* pblendcolor; /* preset blend colors */
  REAL* pblendpos; /* preset blend positions */
  INT pblendcount;
};

Status LineGradient_Set(LineGradient* lg, PointF startpoint,PointF endpoint,ARGB startcolor,ARGB endcolor,RectF rect,
                        WrapMode wrap,BOOL gamma,REAL* blendfac,REAL* blendpos,INT blendcount,
                        ARGB* pblendcolor,REAL* pblendpos, INT pblendcount) {
  lg->startpoint = startpoint;
  lg->endpoint = endpoint;
  lg->startcolor = startcolor;
  lg->endcolor = endcolor;
  lg->rect = rect;
  lg->wrap = wrap;
  lg->gamma = gamma;
  lg->blendcount = blendcount;
  lg->pblendcount = pblendcount;
  lg->blendfac = blendfac;
  lg->pblendcolor = pblendcolor;
  lg->pblendpos = pblendpos;
  return Ok;
}
Status LineGradient_Clone(LineGradient* d, const LineGradient* s) {
  return LineGradient_Set(d, s->startpoint,s->endpoint,s->startcolor,s->endcolor,s->rect,
    s->wrap,s->gamma,s->blendfac,s->blendpos,s->blendcount,
                        s->pblendcolor,s->pblendpos, s->pblendcount);
}

struct Texture {
  WrapMode wrap;
  Matrix transform;
  Image* image;
  RectF rectf;
  ImageAttributes* imageattributes;
  //BYTE *bitmap_bits; /* image bits converted to ARGB and run through imageattributes */
};

void RectFSet(RectF* prectf, REAL x, REAL y, REAL width, REAL height) {
  prectf->X = x;
  prectf->Y = y;
  prectf->Width = width;
  prectf->Height = height;
}

struct Brush {
  union {
    SolidFill solid;
    Hatch hatch;
    PathGradient grad;
    LineGradient line;
    Texture texture;
  };
  BrushType bt;
};

struct Pen {
    //UINT style;
    Unit unit;
    REAL width;
    LineCap endcap;
    LineCap startcap;
    DashCap dashcap;
    CustomLineCap *customstart;
    CustomLineCap *customend;
    LineJoin join;
    REAL miterlimit;
    DashStyle dash;
    REAL *dashes;
    INT numdashes;
    REAL offset;    /* dash offset */
    Brush brush[1];
    PenAlignment align;
};

#if 0
struct Graphics {
    BOOL alpha_hdc;
    Image *image;
    img_t im[1];
    SmoothingMode smoothing;
    CompositingQuality compqual;
    InterpolationMode interpolation;
    PixelOffsetMode pixeloffset;
    CompositingMode compmode;
    TextRenderingHint texthint;
    Unit unit;    /* page unit */
    REAL scale;     /* page scale */
    REAL xres, yres;
    Matrix worldtrans; /* world transform */
    BOOL busy;      /* hdc handle obtained by GdipGetDC */
    Region *clip;
    UINT textcontrast; /* not used yet. get/set only */
    struct list containers;
    GraphicsContainer contid; /* last-issued container ID */
    INT origin_x, origin_y;
    /* For giving the caller an HDC when we technically can't: */
    //HBITMAP temp_hbitmap;
    int temp_hbitmap_width;
    int temp_hbitmap_height;
    BYTE *temp_bits;
};

struct Metafile{
    Image image;
    RectF bounds;
    Unit unit;
    MetafileType metafile_type;
    HENHMETAFILE hemf;
    int preserve_hemf; /* if true, hemf belongs to the app and should not be deleted */

    /* recording */
    HDC record_dc;
    Graphics *record_graphics;
    BYTE *comment_data;
    DWORD comment_data_size;
    DWORD comment_data_length;

    /* playback */
    Graphics *playback_graphics;
    HDC playback_dc;
    PointF playback_points[3];
    HANDLETABLE *handle_table;
    int handle_count;
};
#endif

#define CONST const
typedef Status (*gdip_format_string_callback)(void* hdc,
    CONST WCHAR *string, INT index, INT length, CONST Font *font,
    CONST RectF *rect, CONST StringFormat *format,
    INT lineno, const RectF *bounds, INT *underlined_indexes,
    INT underlined_index_count, void *user_data);

Status gdip_format_string(void* hdc,
    CONST WCHAR *string, INT length, CONST Font *font,
    CONST RectF *rect, CONST StringFormat *format,
    gdip_format_string_callback callback, void *user_data);

struct MyLOGFONT {};

typedef softgc Graphics;
void get_log_fontW(const Font *, Graphics *, MyLOGFONT *);

