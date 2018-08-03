
/* PANOSE is 10 bytes in size, need to pack the structure properly */
#include <pshpack2.h>
typedef struct
{
    USHORT version;
    SHORT xAvgCharWidth;
    USHORT usWeightClass;
    USHORT usWidthClass;
    SHORT fsType;
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
    SHORT sFamilyClass;
    PANOSE panose;
    ULONG ulUnicodeRange1;
    ULONG ulUnicodeRange2;
    ULONG ulUnicodeRange3;
    ULONG ulUnicodeRange4;
    CHAR achVendID[4];
    USHORT fsSelection;
    USHORT usFirstCharIndex;
    USHORT usLastCharIndex;
    /* According to the Apple spec, original version didn't have the below fields,
     * version numbers were taken from the OpenType spec.
     */
    /* version 0 (TrueType 1.5) */
    USHORT sTypoAscender;
    USHORT sTypoDescender;
    USHORT sTypoLineGap;
    USHORT usWinAscent;
    USHORT usWinDescent;
    /* version 1 (TrueType 1.66) */
    ULONG ulCodePageRange1;
    ULONG ulCodePageRange2;
    /* version 2 (OpenType 1.2) */
    SHORT sxHeight;
    SHORT sCapHeight;
    USHORT usDefaultChar;
    USHORT usBreakChar;
    USHORT usMaxContext;
} TT_OS2_V2;

typedef struct
{
    ULONG Version;
    SHORT Ascender;
    SHORT Descender;
    SHORT LineGap;
    USHORT advanceWidthMax;
    SHORT minLeftSideBearing;
    SHORT minRightSideBearing;
    SHORT xMaxExtent;
    SHORT caretSlopeRise;
    SHORT caretSlopeRun;
    SHORT caretOffset;
    SHORT reserved[4];
    SHORT metricDataFormat;
    USHORT numberOfHMetrics;
} TT_HHEA;
#include <poppack.h>

#ifdef WORDS_BIGENDIAN
#define GET_BE_WORD(x) (x)
#define GET_BE_DWORD(x) (x)
#else
#define GET_BE_WORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define GET_BE_DWORD(x) MAKELONG(GET_BE_WORD(HIWORD(x)), GET_BE_WORD(LOWORD(x)));
#endif

#define MS_MAKE_TAG(ch0, ch1, ch2, ch3) \
                    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | \
                    ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))
#define MS_OS2_TAG MS_MAKE_TAG('O','S','/','2')
#define MS_HHEA_TAG MS_MAKE_TAG('h','h','e','a')

static Status clone_font_family(const FontFamily *, FontFamily **);

static FontCollection installedFontCollection = {0};

/*******************************************************************************
 * GdipCreateFont [GDIPLUS.@]
 *
 * Create a new font based off of a FontFamily
 *
 * PARAMS
 *  *fontFamily     [I] Family to base the font off of
 *  emSize          [I] Size of the font
 *  style           [I] Bitwise OR of FontStyle enumeration
 *  unit            [I] Unit emSize is measured in
 *  **font          [I] the resulting Font object
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter if fontfamily or font is NULL.
 *  FAILURE: FontFamilyNotFound if an invalid FontFamily is given
 *
 * NOTES
 *  UnitDisplay is unsupported.
 *  emSize is stored separately from lfHeight, to hold the fraction.
 */
Status  GdipCreateFont(GDIPCONST FontFamily *fontFamily,
                        REAL emSize, INT style, Unit unit, Font **font)
{
    HFONT hfont;
    OUTLINETEXTMETRICW otm;
    LOGFONTW lfw;
    HDC hdc;
    Status stat;
    int ret;

    if (!fontFamily || !font || emSize < 0.0)
        return InvalidParameter;

    TRACE("%p (%s), %f, %d, %d, %p\n", fontFamily,
            debugstr_w(fontFamily->FamilyName), emSize, style, unit, font);

    memset(&lfw, 0, sizeof(lfw));

    stat = GdipGetFamilyName(fontFamily, lfw.lfFaceName, LANG_NEUTRAL);
    if (stat != Ok) return stat;

    lfw.lfHeight = -units_to_pixels(emSize, unit, fontFamily->dpi);
    lfw.lfWeight = style & FontStyleBold ? FW_BOLD : FW_REGULAR;
    lfw.lfItalic = style & FontStyleItalic;
    lfw.lfUnderline = style & FontStyleUnderline;
    lfw.lfStrikeOut = style & FontStyleStrikeout;

    hfont = CreateFontIndirectW(&lfw);
    hdc = CreateCompatibleDC(0);
    SelectObject(hdc, hfont);
    otm.otmSize = sizeof(otm);
    ret = GetOutlineTextMetricsW(hdc, otm.otmSize, &otm);
    DeleteDC(hdc);
    DeleteObject(hfont);

    if (!ret) return NotTrueTypeFont;

    *font = GdipAlloc(sizeof(Font));
    if (!*font) return OutOfMemory;

    (*font)->unit = unit;
    (*font)->emSize = emSize;
    (*font)->otm = otm;

    stat = clone_font_family(fontFamily, &(*font)->family);
    if (stat != Ok)
    {
        GdipFree(*font);
        return stat;
    }

    TRACE("<-- %p\n", *font);

    return Ok;
}

/*******************************************************************************
 * GdipCreateFontFromLogfontW [GDIPLUS.@]
 */
Status  GdipCreateFontFromLogfontW(HDC hdc,
    GDIPCONST LOGFONTW *logfont, Font **font)
{
    HFONT hfont, oldfont;
    OUTLINETEXTMETRICW otm;
    WCHAR facename[LF_FACESIZE];
    Status stat;
    int ret;

    TRACE("(%p, %p, %p)\n", hdc, logfont, font);

    if (!hdc || !logfont || !font)
        return InvalidParameter;

    hfont = CreateFontIndirectW(logfont);
    oldfont = SelectObject(hdc, hfont);
    otm.otmSize = sizeof(otm);
    ret = GetOutlineTextMetricsW(hdc, otm.otmSize, &otm);
    GetTextFaceW(hdc, LF_FACESIZE, facename);
    SelectObject(hdc, oldfont);
    DeleteObject(hfont);

    if (!ret) return NotTrueTypeFont;

    *font = GdipAlloc(sizeof(Font));
    if (!*font) return OutOfMemory;

    (*font)->unit = UnitWorld;
    (*font)->emSize = otm.otmTextMetrics.tmAscent;
    (*font)->otm = otm;

    stat = GdipCreateFontFamilyFromName(facename, NULL, &(*font)->family);
    if (stat != Ok)
    {
        GdipFree(*font);
        return NotTrueTypeFont;
    }

    TRACE("<-- %p\n", *font);

    return Ok;
}

/*******************************************************************************
 * GdipCreateFontFromLogfontA [GDIPLUS.@]
 */
Status  GdipCreateFontFromLogfontA(HDC hdc,
    GDIPCONST LOGFONTA *lfa, Font **font)
{
    LOGFONTW lfw;

    TRACE("(%p, %p, %p)\n", hdc, lfa, font);

    if(!lfa || !font)
        return InvalidParameter;

    memcpy(&lfw, lfa, FIELD_OFFSET(LOGFONTA,lfFaceName) );

    if(!MultiByteToWideChar(CP_ACP, 0, lfa->lfFaceName, -1, lfw.lfFaceName, LF_FACESIZE))
        return GenericError;

    return GdipCreateFontFromLogfontW(hdc, &lfw, font);
}

/*******************************************************************************
 * GdipDeleteFont [GDIPLUS.@]
 */
Status  GdipDeleteFont(Font* font)
{
    TRACE("(%p)\n", font);

    if(!font)
        return InvalidParameter;

    GdipDeleteFontFamily(font->family);
    GdipFree(font);

    return Ok;
}

/*******************************************************************************
 * GdipCreateFontFromDC [GDIPLUS.@]
 */
Status  GdipCreateFontFromDC(HDC hdc, Font **font)
{
    HFONT hfont;
    LOGFONTW lfw;

    TRACE("(%p, %p)\n", hdc, font);

    if(!font)
        return InvalidParameter;

    hfont = GetCurrentObject(hdc, OBJ_FONT);
    if(!hfont)
        return GenericError;

    if(!GetObjectW(hfont, sizeof(LOGFONTW), &lfw))
        return GenericError;

    return GdipCreateFontFromLogfontW(hdc, &lfw, font);
}

/*******************************************************************************
 * GdipGetFamily [GDIPLUS.@]
 *
 * Returns the FontFamily for the specified Font
 *
 * PARAMS
 *  font    [I] Font to request from
 *  family  [O] Resulting FontFamily object
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: An element of Status
 */
Status  GdipGetFamily(Font *font, FontFamily **family)
{
    TRACE("%p %p\n", font, family);

    if (!(font && family))
        return InvalidParameter;

    return GdipCloneFontFamily(font->family, family);
}

static REAL get_font_size(const Font *font)
{
    return font->emSize;
}

/******************************************************************************
 * GdipGetFontSize [GDIPLUS.@]
 *
 * Returns the size of the font in Units
 *
 * PARAMS
 *  *font       [I] The font to retrieve size from
 *  *size       [O] Pointer to hold retrieved value
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter (font or size was NULL)
 *
 * NOTES
 *  Size returned is actually emSize -- not internal size used for drawing.
 */
Status  GdipGetFontSize(Font *font, REAL *size)
{
    TRACE("(%p, %p)\n", font, size);

    if (!(font && size)) return InvalidParameter;

    *size = get_font_size(font);
    TRACE("%s,%d => %f\n", debugstr_w(font->family->FamilyName), font->otm.otmTextMetrics.tmHeight, *size);

    return Ok;
}

static INT get_font_style(const Font *font)
{
    INT style;

    if (font->otm.otmTextMetrics.tmWeight > FW_REGULAR)
        style = FontStyleBold;
    else
        style = FontStyleRegular;
    if (font->otm.otmTextMetrics.tmItalic)
        style |= FontStyleItalic;
    if (font->otm.otmTextMetrics.tmUnderlined)
        style |= FontStyleUnderline;
    if (font->otm.otmTextMetrics.tmStruckOut)
        style |= FontStyleStrikeout;

    return style;
}

/*******************************************************************************
 * GdipGetFontStyle [GDIPLUS.@]
 *
 * Gets the font's style, returned in bitwise OR of FontStyle enumeration
 *
 * PARAMS
 *  font    [I] font to request from
 *  style   [O] resulting pointer to a FontStyle enumeration
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter
 */
Status  GdipGetFontStyle(Font *font, INT *style)
{
    TRACE("%p %p\n", font, style);

    if (!(font && style))
        return InvalidParameter;

    *style = get_font_style(font);
    TRACE("%s,%d => %d\n", debugstr_w(font->family->FamilyName), font->otm.otmTextMetrics.tmHeight, *style);

    return Ok;
}

/*******************************************************************************
 * GdipGetFontUnit  [GDIPLUS.@]
 *
 * PARAMS
 *  font    [I] Font to retrieve from
 *  unit    [O] Return value
 *
 * RETURNS
 *  FAILURE: font or unit was NULL
 *  OK: otherwise
 */
Status  GdipGetFontUnit(Font *font, Unit *unit)
{
    TRACE("(%p, %p)\n", font, unit);

    if (!(font && unit)) return InvalidParameter;

    *unit = font->unit;
    TRACE("%s,%d => %d\n", debugstr_w(font->family->FamilyName), font->otm.otmTextMetrics.tmHeight, *unit);

    return Ok;
}

/*******************************************************************************
 * GdipGetLogFontA [GDIPLUS.@]
 */
Status  GdipGetLogFontA(Font *font, Graphics *graphics,
    LOGFONTA *lfa)
{
    Status status;
    LOGFONTW lfw;

    TRACE("(%p, %p, %p)\n", font, graphics, lfa);

    status = GdipGetLogFontW(font, graphics, &lfw);
    if(status != Ok)
        return status;

    memcpy(lfa, &lfw, FIELD_OFFSET(LOGFONTA,lfFaceName) );

    if(!WideCharToMultiByte(CP_ACP, 0, lfw.lfFaceName, -1, lfa->lfFaceName, LF_FACESIZE, NULL, NULL))
        return GenericError;

    return Ok;
}

/*******************************************************************************
 * GdipGetLogFontW [GDIPLUS.@]
 */
Status  GdipGetLogFontW(Font *font, Graphics *graphics, LOGFONTW *lf)
{
    REAL angle, rel_height, height;
    Matrix matrix;
    PointF pt[3];

    TRACE("(%p, %p, %p)\n", font, graphics, lf);

    if (!font || !graphics || !lf)
        return InvalidParameter;

    matrix = graphics->worldtrans;

    if (font->unit == UnitPixel)
    {
        height = units_to_pixels(font->emSize, graphics->unit, graphics->yres);
        if (graphics->unit != UnitDisplay)
            GdipScaleMatrix(&matrix, graphics->scale, graphics->scale, MatrixOrderAppend);
    }
    else
    {
        if (graphics->unit == UnitDisplay || graphics->unit == UnitPixel)
            height = units_to_pixels(font->emSize, font->unit, graphics->xres);
        else
            height = units_to_pixels(font->emSize, font->unit, graphics->yres);
    }

    pt[0].X = 0.0;
    pt[0].Y = 0.0;
    pt[1].X = 1.0;
    pt[1].Y = 0.0;
    pt[2].X = 0.0;
    pt[2].Y = 1.0;
    GdipTransformMatrixPoints(&matrix, pt, 3);
    angle = -gdiplus_atan2((pt[1].Y - pt[0].Y), (pt[1].X - pt[0].X));
    rel_height = sqrt((pt[2].Y - pt[0].Y) * (pt[2].Y - pt[0].Y)+
                      (pt[2].X - pt[0].X) * (pt[2].X - pt[0].X));

    lf->lfHeight = -gdip_round(height * rel_height);
    lf->lfWidth = 0;
    lf->lfEscapement = lf->lfOrientation = gdip_round((angle / M_PI) * 1800.0);
    if (lf->lfEscapement < 0)
    {
        lf->lfEscapement += 3600;
        lf->lfOrientation += 3600;
    }
    lf->lfWeight = font->otm.otmTextMetrics.tmWeight;
    lf->lfItalic = font->otm.otmTextMetrics.tmItalic ? 1 : 0;
    lf->lfUnderline = font->otm.otmTextMetrics.tmUnderlined ? 1 : 0;
    lf->lfStrikeOut = font->otm.otmTextMetrics.tmStruckOut ? 1 : 0;
    lf->lfCharSet = font->otm.otmTextMetrics.tmCharSet;
    lf->lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf->lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf->lfQuality = DEFAULT_QUALITY;
    lf->lfPitchAndFamily = 0;
    strcpyW(lf->lfFaceName, font->family->FamilyName);

    TRACE("=> %s,%d\n", debugstr_w(lf->lfFaceName), lf->lfHeight);

    return Ok;
}

/*******************************************************************************
 * GdipCloneFont [GDIPLUS.@]
 */
Status  GdipCloneFont(Font *font, Font **cloneFont)
{
    Status stat;

    TRACE("(%p, %p)\n", font, cloneFont);

    if(!font || !cloneFont)
        return InvalidParameter;

    *cloneFont = GdipAlloc(sizeof(Font));
    if(!*cloneFont)    return OutOfMemory;

    **cloneFont = *font;
    stat = GdipCloneFontFamily(font->family, &(*cloneFont)->family);
    if (stat != Ok) GdipFree(*cloneFont);

    return stat;
}

/*******************************************************************************
 * GdipGetFontHeight [GDIPLUS.@]
 * PARAMS
 *  font        [I] Font to retrieve height from
 *  graphics    [I] The current graphics context
 *  height      [O] Resulting height
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: Another element of Status
 *
 * NOTES
 *  Forwards to GdipGetFontHeightGivenDPI
 */
Status  GdipGetFontHeight(GDIPCONST Font *font,
        GDIPCONST Graphics *graphics, REAL *height)
{
    REAL dpi;
    Status stat;
    REAL font_height;

    TRACE("%p %p %p\n", font, graphics, height);

    stat = GdipGetFontHeightGivenDPI(font, font->family->dpi, &font_height);
    if (stat != Ok) return stat;

    if (!graphics)
    {
        *height = font_height;
        TRACE("%s,%d => %f\n",
              debugstr_w(font->family->FamilyName), font->otm.otmTextMetrics.tmHeight, *height);
        return Ok;
    }

    stat = GdipGetDpiY((Graphics *)graphics, &dpi);
    if (stat != Ok) return stat;

    *height = pixels_to_units(font_height, graphics->unit, dpi);

    TRACE("%s,%d(unit %d) => %f\n",
          debugstr_w(font->family->FamilyName), font->otm.otmTextMetrics.tmHeight, graphics->unit, *height);
    return Ok;
}

/*******************************************************************************
 * GdipGetFontHeightGivenDPI [GDIPLUS.@]
 * PARAMS
 *  font        [I] Font to retrieve DPI from
 *  dpi         [I] DPI to assume
 *  height      [O] Return value
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter if font or height is NULL
 *
 * NOTES
 *  According to MSDN, the result is (lineSpacing)*(fontSize / emHeight)*dpi
 *  (for anything other than unit Pixel)
 */
Status  GdipGetFontHeightGivenDPI(GDIPCONST Font *font, REAL dpi, REAL *height)
{
    Status stat;
    INT style;
    UINT16 line_spacing, em_height;
    REAL font_size;

    if (!font || !height) return InvalidParameter;

    TRACE("%p (%s), %f, %p\n", font,
            debugstr_w(font->family->FamilyName), dpi, height);

    font_size = units_to_pixels(get_font_size(font), font->unit, dpi);
    style = get_font_style(font);
    stat = GdipGetLineSpacing(font->family, style, &line_spacing);
    if (stat != Ok) return stat;
    stat = GdipGetEmHeight(font->family, style, &em_height);
    if (stat != Ok) return stat;

    *height = (REAL)line_spacing * font_size / (REAL)em_height;

    TRACE("%s,%d => %f\n",
          debugstr_w(font->family->FamilyName), font->otm.otmTextMetrics.tmHeight, *height);

    return Ok;
}

/***********************************************************************
 * Borrowed from GDI32:
 *
 * Elf is really an ENUMLOGFONTEXW, and ntm is a NEWTEXTMETRICEXW.
 *     We have to use other types because of the FONTENUMPROCW definition.
 */
static INT CALLBACK is_font_installed_proc(const LOGFONTW *elf,
                            const TEXTMETRICW *ntm, DWORD type, LPARAM lParam)
{
    if (type & RASTER_FONTTYPE)
        return 1;

    *(LOGFONTW *)lParam = *elf;

    return 0;
}

struct font_metrics
{
    WCHAR facename[LF_FACESIZE];
    UINT16 em_height, ascent, descent, line_spacing; /* in font units */
    int dpi;
};

static BOOL get_font_metrics(HDC hdc, struct font_metrics *fm)
{
    OUTLINETEXTMETRICW otm;
    TT_OS2_V2 tt_os2;
    TT_HHEA tt_hori;
    LONG size;
    UINT16 line_gap;

    otm.otmSize = sizeof(otm);
    if (!GetOutlineTextMetricsW(hdc, otm.otmSize, &otm)) return FALSE;

    GetTextFaceW(hdc, LF_FACESIZE, fm->facename);

    fm->em_height = otm.otmEMSquare;
    fm->dpi = GetDeviceCaps(hdc, LOGPIXELSY);

    memset(&tt_hori, 0, sizeof(tt_hori));
    if (GetFontData(hdc, MS_HHEA_TAG, 0, &tt_hori, sizeof(tt_hori)) != GDI_ERROR)
    {
        fm->ascent = GET_BE_WORD(tt_hori.Ascender);
        fm->descent = -GET_BE_WORD(tt_hori.Descender);
        TRACE("hhea: ascent %d, descent %d\n", fm->ascent, fm->descent);
        line_gap = GET_BE_WORD(tt_hori.LineGap);
        fm->line_spacing = fm->ascent + fm->descent + line_gap;
        TRACE("line_gap %u, line_spacing %u\n", line_gap, fm->line_spacing);
        if (fm->ascent + fm->descent != 0) return TRUE;
    }

    size = GetFontData(hdc, MS_OS2_TAG, 0, NULL, 0);
    if (size == GDI_ERROR) return FALSE;

    if (size > sizeof(tt_os2)) size = sizeof(tt_os2);

    memset(&tt_os2, 0, sizeof(tt_os2));
    if (GetFontData(hdc, MS_OS2_TAG, 0, &tt_os2, size) != size) return FALSE;

    fm->ascent = GET_BE_WORD(tt_os2.usWinAscent);
    fm->descent = GET_BE_WORD(tt_os2.usWinDescent);
    TRACE("usWinAscent %u, usWinDescent %u\n", fm->ascent, fm->descent);
    if (fm->ascent + fm->descent == 0)
    {
        fm->ascent = GET_BE_WORD(tt_os2.sTypoAscender);
        fm->descent = GET_BE_WORD(tt_os2.sTypoDescender);
        TRACE("sTypoAscender %u, sTypoDescender %u\n", fm->ascent, fm->descent);
    }
    line_gap = GET_BE_WORD(tt_os2.sTypoLineGap);
    fm->line_spacing = fm->ascent + fm->descent + line_gap;
    TRACE("line_gap %u, line_spacing %u\n", line_gap, fm->line_spacing);
    return TRUE;
}

static Status find_installed_font(const WCHAR *name, struct font_metrics *fm)
{
    LOGFONTW lf;
    HDC hdc = CreateCompatibleDC(0);
    Status ret = FontFamilyNotFound;

    if(!EnumFontFamiliesW(hdc, name, is_font_installed_proc, (LPARAM)&lf))
    {
        HFONT hfont, old_font;

        hfont = CreateFontIndirectW(&lf);
        old_font = SelectObject(hdc, hfont);
        ret = get_font_metrics(hdc, fm) ? Ok : NotTrueTypeFont;
        SelectObject(hdc, old_font);
        DeleteObject(hfont);
    }

    DeleteDC(hdc);
    return ret;
}

/*******************************************************************************
 * GdipCreateFontFamilyFromName [GDIPLUS.@]
 *
 * Creates a font family object based on a supplied name
 *
 * PARAMS
 *  name               [I] Name of the font
 *  fontCollection     [I] What font collection (if any) the font belongs to (may be NULL)
 *  FontFamily         [O] Pointer to the resulting FontFamily object
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: FamilyNotFound if the requested FontFamily does not exist on the system
 *  FAILURE: Invalid parameter if FontFamily or name is NULL
 *
 * NOTES
 *   If fontCollection is NULL then the object is not part of any collection
 *
 */

Status  GdipCreateFontFamilyFromName(GDIPCONST WCHAR *name,
                                        FontCollection *fontCollection,
                                        FontFamily **FontFamily)
{
    Status stat;
    FontFamily* ffamily;
    struct font_metrics fm;

    TRACE("%s, %p %p\n", debugstr_w(name), fontCollection, FontFamily);

    if (!(name && FontFamily))
        return InvalidParameter;
    if (fontCollection)
        FIXME("No support for FontCollections yet!\n");

    stat = find_installed_font(name, &fm);
    if (stat != Ok) return stat;

    ffamily = GdipAlloc(sizeof (FontFamily));
    if (!ffamily) return OutOfMemory;

    lstrcpyW(ffamily->FamilyName, fm.facename);
    ffamily->em_height = fm.em_height;
    ffamily->ascent = fm.ascent;
    ffamily->descent = fm.descent;
    ffamily->line_spacing = fm.line_spacing;
    ffamily->dpi = fm.dpi;

    *FontFamily = ffamily;

    TRACE("<-- %p\n", ffamily);

    return Ok;
}

static Status clone_font_family(const FontFamily *family, FontFamily **clone)
{
    *clone = GdipAlloc(sizeof(FontFamily));
    if (!*clone) return OutOfMemory;

    **clone = *family;

    return Ok;
}

/*******************************************************************************
 * GdipCloneFontFamily [GDIPLUS.@]
 *
 * Creates a deep copy of a Font Family object
 *
 * PARAMS
 *  FontFamily          [I] Font to clone
 *  clonedFontFamily    [O] The resulting cloned font
 *
 * RETURNS
 *  SUCCESS: Ok
 */
Status  GdipCloneFontFamily(FontFamily* FontFamily, FontFamily** clonedFontFamily)
{
    Status status;

    if (!(FontFamily && clonedFontFamily)) return InvalidParameter;

    TRACE("%p (%s), %p\n", FontFamily,
            debugstr_w(FontFamily->FamilyName), clonedFontFamily);

    status = clone_font_family(FontFamily, clonedFontFamily);
    if (status != Ok) return status;

    TRACE("<-- %p\n", *clonedFontFamily);

    return Ok;
}

/*******************************************************************************
 * GdipGetFamilyName [GDIPLUS.@]
 *
 * Returns the family name into name
 *
 * PARAMS
 *  *family     [I] Family to retrieve from
 *  *name       [O] WCHARS of the family name
 *  LANGID      [I] charset
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter if family is NULL
 *
 * NOTES
 *   If name is a NULL ptr, then both XP and Vista will crash (so we do as well)
 */
Status  GdipGetFamilyName (GDIPCONST FontFamily *family,
                                       WCHAR *name, LANGID language)
{
    static int lang_fixme;

    if (family == NULL)
         return InvalidParameter;

    TRACE("%p, %p, %d\n", family, name, language);

    if (language != LANG_NEUTRAL && !lang_fixme++)
        FIXME("No support for handling of multiple languages!\n");

    lstrcpynW (name, family->FamilyName, LF_FACESIZE);

    return Ok;
}


/*****************************************************************************
 * GdipDeleteFontFamily [GDIPLUS.@]
 *
 * Removes the specified FontFamily
 *
 * PARAMS
 *  *FontFamily         [I] The family to delete
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter if FontFamily is NULL.
 *
 */
Status  GdipDeleteFontFamily(FontFamily *FontFamily)
{
    if (!FontFamily)
        return InvalidParameter;
    TRACE("Deleting %p (%s)\n", FontFamily, debugstr_w(FontFamily->FamilyName));

    GdipFree (FontFamily);

    return Ok;
}

Status  GdipGetCellAscent(GDIPCONST FontFamily *family,
        INT style, UINT16* CellAscent)
{
    if (!(family && CellAscent)) return InvalidParameter;

    *CellAscent = family->ascent;
    TRACE("%s => %u\n", debugstr_w(family->FamilyName), *CellAscent);

    return Ok;
}

Status  GdipGetCellDescent(GDIPCONST FontFamily *family,
        INT style, UINT16* CellDescent)
{
    TRACE("(%p, %d, %p)\n", family, style, CellDescent);

    if (!(family && CellDescent)) return InvalidParameter;

    *CellDescent = family->descent;
    TRACE("%s => %u\n", debugstr_w(family->FamilyName), *CellDescent);

    return Ok;
}

/*******************************************************************************
 * GdipGetEmHeight [GDIPLUS.@]
 *
 * Gets the height of the specified family in EmHeights
 *
 * PARAMS
 *  family      [I] Family to retrieve from
 *  style       [I] (optional) style
 *  EmHeight    [O] return value
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter
 */
Status  GdipGetEmHeight(GDIPCONST FontFamily *family, INT style, UINT16* EmHeight)
{
    if (!(family && EmHeight)) return InvalidParameter;

    TRACE("%p (%s), %d, %p\n", family, debugstr_w(family->FamilyName), style, EmHeight);

    *EmHeight = family->em_height;
    TRACE("%s => %u\n", debugstr_w(family->FamilyName), *EmHeight);

    return Ok;
}


/*******************************************************************************
 * GdipGetLineSpacing [GDIPLUS.@]
 *
 * Returns the line spacing in design units
 *
 * PARAMS
 *  family      [I] Family to retrieve from
 *  style       [I] (Optional) font style
 *  LineSpacing [O] Return value
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: InvalidParameter (family or LineSpacing was NULL)
 */
Status  GdipGetLineSpacing(GDIPCONST FontFamily *family,
        INT style, UINT16* LineSpacing)
{
    TRACE("%p, %d, %p\n", family, style, LineSpacing);

    if (!(family && LineSpacing))
        return InvalidParameter;

    if (style) FIXME("ignoring style\n");

    *LineSpacing = family->line_spacing;
    TRACE("%s => %u\n", debugstr_w(family->FamilyName), *LineSpacing);

    return Ok;
}

static INT CALLBACK font_has_style_proc(const LOGFONTW *elf,
                            const TEXTMETRICW *ntm, DWORD type, LPARAM lParam)
{
    INT fontstyle = FontStyleRegular;

    if (!ntm) return 1;

    if (ntm->tmWeight >= FW_BOLD) fontstyle |= FontStyleBold;
    if (ntm->tmItalic) fontstyle |= FontStyleItalic;
    if (ntm->tmUnderlined) fontstyle |= FontStyleUnderline;
    if (ntm->tmStruckOut) fontstyle |= FontStyleStrikeout;

    return (INT)lParam != fontstyle;
}

Status  GdipIsStyleAvailable(GDIPCONST FontFamily* family,
        INT style, BOOL* IsStyleAvailable)
{
    HDC hdc;

    TRACE("%p %d %p\n", family, style, IsStyleAvailable);

    if (!(family && IsStyleAvailable))
        return InvalidParameter;

    *IsStyleAvailable = FALSE;

    hdc = GetDC(0);

    if(!EnumFontFamiliesW(hdc, family->FamilyName, font_has_style_proc, (LPARAM)style))
        *IsStyleAvailable = TRUE;

    ReleaseDC(0, hdc);

    return Ok;
}

/*****************************************************************************
 * GdipGetGenericFontFamilyMonospace [GDIPLUS.@]
 *
 * Obtains a serif family (Courier New on Windows)
 *
 * PARAMS
 *  **nativeFamily         [I] Where the font will be stored
 *
 * RETURNS
 *  InvalidParameter if nativeFamily is NULL.
 *  Ok otherwise.
 */
Status  GdipGetGenericFontFamilyMonospace(FontFamily **nativeFamily)
{
    static const WCHAR CourierNew[] = {'C','o','u','r','i','e','r',' ','N','e','w','\0'};
    static const WCHAR LiberationMono[] = {'L','i','b','e','r','a','t','i','o','n',' ','M','o','n','o','\0'};
    Status stat;

    if (nativeFamily == NULL) return InvalidParameter;

    stat = GdipCreateFontFamilyFromName(CourierNew, NULL, nativeFamily);

    if (stat == FontFamilyNotFound)
        stat = GdipCreateFontFamilyFromName(LiberationMono, NULL, nativeFamily);

    if (stat == FontFamilyNotFound)
        ERR("Missing 'Courier New' font\n");

    return stat;
}

/*****************************************************************************
 * GdipGetGenericFontFamilySerif [GDIPLUS.@]
 *
 * Obtains a serif family (Times New Roman on Windows)
 *
 * PARAMS
 *  **nativeFamily         [I] Where the font will be stored
 *
 * RETURNS
 *  InvalidParameter if nativeFamily is NULL.
 *  Ok otherwise.
 */
Status  GdipGetGenericFontFamilySerif(FontFamily **nativeFamily)
{
    static const WCHAR TimesNewRoman[] = {'T','i','m','e','s',' ','N','e','w',' ','R','o','m','a','n','\0'};
    static const WCHAR LiberationSerif[] = {'L','i','b','e','r','a','t','i','o','n',' ','S','e','r','i','f','\0'};
    Status stat;

    TRACE("(%p)\n", nativeFamily);

    if (nativeFamily == NULL) return InvalidParameter;

    stat = GdipCreateFontFamilyFromName(TimesNewRoman, NULL, nativeFamily);

    if (stat == FontFamilyNotFound)
        stat = GdipCreateFontFamilyFromName(LiberationSerif, NULL, nativeFamily);

    if (stat == FontFamilyNotFound)
        ERR("Missing 'Times New Roman' font\n");

    return stat;
}

/*****************************************************************************
 * GdipGetGenericFontFamilySansSerif [GDIPLUS.@]
 *
 * Obtains a serif family (Microsoft Sans Serif on Windows)
 *
 * PARAMS
 *  **nativeFamily         [I] Where the font will be stored
 *
 * RETURNS
 *  InvalidParameter if nativeFamily is NULL.
 *  Ok otherwise.
 */
Status  GdipGetGenericFontFamilySansSerif(FontFamily **nativeFamily)
{
    Status stat;
    static const WCHAR MicrosoftSansSerif[] = {'M','i','c','r','o','s','o','f','t',' ','S','a','n','s',' ','S','e','r','i','f','\0'};
    static const WCHAR Tahoma[] = {'T','a','h','o','m','a','\0'};

    TRACE("(%p)\n", nativeFamily);

    if (nativeFamily == NULL) return InvalidParameter;

    stat = GdipCreateFontFamilyFromName(MicrosoftSansSerif, NULL, nativeFamily);

    if (stat == FontFamilyNotFound)
        /* FIXME: Microsoft Sans Serif is not installed on Wine. */
        stat = GdipCreateFontFamilyFromName(Tahoma, NULL, nativeFamily);

    return stat;
}

/*****************************************************************************
 * GdipGetGenericFontFamilySansSerif [GDIPLUS.@]
 */
Status  GdipNewPrivateFontCollection(FontCollection** fontCollection)
{
    TRACE("%p\n", fontCollection);

    if (!fontCollection)
        return InvalidParameter;

    *fontCollection = GdipAlloc(sizeof(FontCollection));
    if (!*fontCollection) return OutOfMemory;

    (*fontCollection)->FontFamilies = NULL;
    (*fontCollection)->count = 0;
    (*fontCollection)->allocated = 0;

    TRACE("<-- %p\n", *fontCollection);

    return Ok;
}

/*****************************************************************************
 * GdipDeletePrivateFontCollection [GDIPLUS.@]
 */
Status  GdipDeletePrivateFontCollection(FontCollection **fontCollection)
{
    INT i;

    TRACE("%p\n", fontCollection);

    if (!fontCollection)
        return InvalidParameter;

    for (i = 0; i < (*fontCollection)->count; i++) GdipFree((*fontCollection)->FontFamilies[i]);
    GdipFree(*fontCollection);

    return Ok;
}

/*****************************************************************************
 * GdipPrivateAddFontFile [GDIPLUS.@]
 */
Status  GdipPrivateAddFontFile(FontCollection* fontCollection,
        GDIPCONST WCHAR* filename)
{
    FIXME("stub: %p, %s\n", fontCollection, debugstr_w(filename));

    if (!(fontCollection && filename))
        return InvalidParameter;

    return NotImplemented;
}

/* Copied from msi/font.c */

typedef struct _tagTT_OFFSET_TABLE {
    USHORT uMajorVersion;
    USHORT uMinorVersion;
    USHORT uNumOfTables;
    USHORT uSearchRange;
    USHORT uEntrySelector;
    USHORT uRangeShift;
} TT_OFFSET_TABLE;

typedef struct _tagTT_TABLE_DIRECTORY {
    char szTag[4]; /* table name */
    ULONG uCheckSum; /* Check sum */
    ULONG uOffset; /* Offset from beginning of file */
    ULONG uLength; /* length of the table in bytes */
} TT_TABLE_DIRECTORY;

typedef struct _tagTT_NAME_TABLE_HEADER {
    USHORT uFSelector; /* format selector. Always 0 */
    USHORT uNRCount; /* Name Records count */
    USHORT uStorageOffset; /* Offset for strings storage,
                            * from start of the table */
} TT_NAME_TABLE_HEADER;

#define NAME_ID_FULL_FONT_NAME  4
#define NAME_ID_VERSION         5

typedef struct _tagTT_NAME_RECORD {
    USHORT uPlatformID;
    USHORT uEncodingID;
    USHORT uLanguageID;
    USHORT uNameID;
    USHORT uStringLength;
    USHORT uStringOffset; /* from start of storage area */
} TT_NAME_RECORD;

#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))

/*
 * Code based off of code located here
 * http://www.codeproject.com/gdi/fontnamefromfile.asp
 */
static WCHAR *load_ttf_name_id( const char *mem, DWORD_PTR size, DWORD id, WCHAR *ret, DWORD len )
{
    const TT_TABLE_DIRECTORY *tblDir;
    TT_OFFSET_TABLE ttOffsetTable;
    TT_NAME_TABLE_HEADER ttNTHeader;
    TT_NAME_RECORD ttRecord;
    DWORD ofs, pos;
    int i;

    if (sizeof(TT_OFFSET_TABLE) > size)
        return NULL;
    ttOffsetTable = *(TT_OFFSET_TABLE*)mem;
    ttOffsetTable.uNumOfTables = SWAPWORD(ttOffsetTable.uNumOfTables);
    ttOffsetTable.uMajorVersion = SWAPWORD(ttOffsetTable.uMajorVersion);
    ttOffsetTable.uMinorVersion = SWAPWORD(ttOffsetTable.uMinorVersion);

    if (ttOffsetTable.uMajorVersion != 1 || ttOffsetTable.uMinorVersion != 0)
        return NULL;

    pos = sizeof(ttOffsetTable);
    for (i = 0; i < ttOffsetTable.uNumOfTables; i++)
    {
        tblDir = (const TT_TABLE_DIRECTORY*)&mem[pos];
        pos += sizeof(*tblDir);
        if (memcmp(tblDir->szTag,"name",4)==0)
        {
            ofs = SWAPLONG(tblDir->uOffset);
            break;
        }
    }
    if (i >= ttOffsetTable.uNumOfTables)
        return NULL;

    pos = ofs + sizeof(ttNTHeader);
    if (pos > size)
        return NULL;
    ttNTHeader = *(TT_NAME_TABLE_HEADER*)&mem[ofs];
    ttNTHeader.uNRCount = SWAPWORD(ttNTHeader.uNRCount);
    ttNTHeader.uStorageOffset = SWAPWORD(ttNTHeader.uStorageOffset);
    for(i=0; i<ttNTHeader.uNRCount; i++)
    {
        ttRecord = *(TT_NAME_RECORD*)&mem[pos];
        pos += sizeof(ttRecord);
        if (pos > size)
            return NULL;

        ttRecord.uNameID = SWAPWORD(ttRecord.uNameID);
        if (ttRecord.uNameID == id)
        {
            const char *buf;

            ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
            ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);
            if (ofs + ttRecord.uStringOffset + ttNTHeader.uStorageOffset + ttRecord.uStringLength > size)
                return NULL;
            buf = mem + ofs + ttRecord.uStringOffset + ttNTHeader.uStorageOffset;
            len = MultiByteToWideChar(CP_ACP, 0, buf, ttRecord.uStringLength, ret, len-1);
            ret[len] = 0;
            return ret;
        }
    }
    return NULL;
}

static INT CALLBACK add_font_proc(const LOGFONTW *lfw, const TEXTMETRICW *ntm, DWORD type, LPARAM lParam);

/*****************************************************************************
 * GdipPrivateAddMemoryFont [GDIPLUS.@]
 */
Status  GdipPrivateAddMemoryFont(FontCollection* fontCollection,
        GDIPCONST void* memory, INT length)
{
    WCHAR buf[32], *name;
    DWORD count = 0;
    HANDLE font;
    TRACE("%p, %p, %d\n", fontCollection, memory, length);

    if (!fontCollection || !memory || !length)
        return InvalidParameter;

    name = load_ttf_name_id(memory, length, NAME_ID_FULL_FONT_NAME, buf, sizeof(buf)/sizeof(*buf));
    if (!name)
        return OutOfMemory;

    font = AddFontMemResourceEx((void*)memory, length, NULL, &count);
    TRACE("%s: %p/%u\n", debugstr_w(name), font, count);
    if (!font || !count)
        return InvalidParameter;

    if (count)
    {
        HDC hdc;
        LOGFONTW lfw;

        hdc = GetDC(0);

        lfw.lfCharSet = DEFAULT_CHARSET;
        lstrcpyW(lfw.lfFaceName, name);
        lfw.lfPitchAndFamily = 0;

        if (!EnumFontFamiliesExW(hdc, &lfw, add_font_proc, (LPARAM)fontCollection, 0))
        {
            ReleaseDC(0, hdc);
            return OutOfMemory;
        }

        ReleaseDC(0, hdc);
    }
    return Ok;
}

/*****************************************************************************
 * GdipGetFontCollectionFamilyCount [GDIPLUS.@]
 */
Status  GdipGetFontCollectionFamilyCount(
        FontCollection* fontCollection, INT* numFound)
{
    TRACE("%p, %p\n", fontCollection, numFound);

    if (!(fontCollection && numFound))
        return InvalidParameter;

    *numFound = fontCollection->count;
    return Ok;
}

/*****************************************************************************
 * GdipGetFontCollectionFamilyList [GDIPLUS.@]
 */
Status  GdipGetFontCollectionFamilyList(
        FontCollection* fontCollection, INT numSought,
        FontFamily* gpfamilies[], INT* numFound)
{
    INT i;
    Status stat=Ok;

    TRACE("%p, %d, %p, %p\n", fontCollection, numSought, gpfamilies, numFound);

    if (!(fontCollection && gpfamilies && numFound))
        return InvalidParameter;

    memset(gpfamilies, 0, sizeof(*gpfamilies) * numSought);

    for (i = 0; i < numSought && i < fontCollection->count && stat == Ok; i++)
    {
        stat = GdipCloneFontFamily(fontCollection->FontFamilies[i], &gpfamilies[i]);
    }

    if (stat == Ok)
        *numFound = i;
    else
    {
        int numToFree=i;
        for (i=0; i<numToFree; i++)
        {
            GdipDeleteFontFamily(gpfamilies[i]);
            gpfamilies[i] = NULL;
        }
    }

    return stat;
}

void free_installed_fonts(void)
{
    while (installedFontCollection.count)
        GdipDeleteFontFamily(installedFontCollection.FontFamilies[--installedFontCollection.count]);
    HeapFree(GetProcessHeap(), 0, installedFontCollection.FontFamilies);
    installedFontCollection.FontFamilies = NULL;
    installedFontCollection.allocated = 0;
}

static INT CALLBACK add_font_proc(const LOGFONTW *lfw, const TEXTMETRICW *ntm,
        DWORD type, LPARAM lParam)
{
    FontCollection* fonts = (FontCollection*)lParam;
    int i;

    if (type == RASTER_FONTTYPE)
        return 1;

    /* skip duplicates */
    for (i=0; i<fonts->count; i++)
        if (strcmpiW(lfw->lfFaceName, fonts->FontFamilies[i]->FamilyName) == 0)
            return 1;

    if (fonts->allocated == fonts->count)
    {
        INT new_alloc_count = fonts->allocated+50;
        FontFamily** new_family_list = HeapAlloc(GetProcessHeap(), 0, new_alloc_count*sizeof(void*));

        if (!new_family_list)
            return 0;

        memcpy(new_family_list, fonts->FontFamilies, fonts->count*sizeof(void*));
        HeapFree(GetProcessHeap(), 0, fonts->FontFamilies);
        fonts->FontFamilies = new_family_list;
        fonts->allocated = new_alloc_count;
    }

    if (GdipCreateFontFamilyFromName(lfw->lfFaceName, NULL, &fonts->FontFamilies[fonts->count]) == Ok)
        fonts->count++;
    else
        return 0;

    return 1;
}

Status  GdipNewInstalledFontCollection(
        FontCollection** fontCollection)
{
    TRACE("(%p)\n",fontCollection);

    if (!fontCollection)
        return InvalidParameter;

    if (installedFontCollection.count == 0)
    {
        HDC hdc;
        LOGFONTW lfw;

        hdc = GetDC(0);

        lfw.lfCharSet = DEFAULT_CHARSET;
        lfw.lfFaceName[0] = 0;
        lfw.lfPitchAndFamily = 0;

        if (!EnumFontFamiliesExW(hdc, &lfw, add_font_proc, (LPARAM)&installedFontCollection, 0))
        {
            free_installed_fonts();
            ReleaseDC(0, hdc);
            return OutOfMemory;
        }

        ReleaseDC(0, hdc);
    }

    *fontCollection = &installedFontCollection;

    return Ok;
}