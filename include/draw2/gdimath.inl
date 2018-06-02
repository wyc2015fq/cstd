
typedef float REAL;

static char dbg_buf[4096];
char* dbg_sprintf(const char* fmt, ...) {
  myva_list ARGLIST;
  myva_start(ARGLIST, fmt);
  cstr_vnprintf(dbg_buf, countof(dbg_buf), fmt, ARGLIST, NULL);
  myva_end(ARGLIST);
  return dbg_buf;
}

const char* debugstr_w(const WCHAR* wstr) {
  int len = sys_wc2mb(CE_GB2312, wstr, wcslen(wstr), dbg_buf, countof(dbg_buf));
  return dbg_buf;
}

const char *debugstr_rectf(CONST RectF* rc)
{
  if (!rc) return "(null)";
  return dbg_sprintf("(%0.2f,%0.2f,%0.2f,%0.2f)", rc->X, rc->Y, rc->Width, rc->Height);
}

const char *debugstr_pointf(CONST PointF* pt)
{
  if (!pt) return "(null)";
  return dbg_sprintf("(%0.2f,%0.2f)", pt->X, pt->Y);
}


static const REAL mm_per_inch = 25.4f;
static const REAL point_per_inch = 72.0f;

/* Calculates the bezier points needed to fill in the arc portion starting at
 * angle start and ending at end.  These two angles should be no more than 90
 * degrees from each other.  x1, y1, x2, y2 describes the bounding box (upper
 * left and width and height).  Angles must be in radians. write_first indicates
 * that the first bezier point should be written out (usually this is false).
 * pt is the array of PointFs that gets written to.
 **/
static void add_arc_part(PointF * pt, REAL x1, REAL y1, REAL x2, REAL y2,
    REAL start, REAL end, BOOL write_first)
{
    REAL center_x, center_y, rad_x, rad_y, cos_start, cos_end,
        sin_start, sin_end, a, half;
    INT i;

    rad_x = x2 / 2.0;
    rad_y = y2 / 2.0;
    center_x = x1 + rad_x;
    center_y = y1 + rad_y;

    cos_start = cos(start);
    cos_end = cos(end);
    sin_start = sin(start);
    sin_end = sin(end);

    half = (end - start) / 2.0;
    a = 4.0 / 3.0 * (1 - cos(half)) / sin(half);

    if(write_first){
        pt[0].X = cos_start;
        pt[0].Y = sin_start;
    }
    pt[1].X = cos_start - a * sin_start;
    pt[1].Y = sin_start + a * cos_start;

    pt[3].X = cos_end;
    pt[3].Y = sin_end;
    pt[2].X = cos_end + a * sin_end;
    pt[2].Y = sin_end - a * cos_end;

    /* expand the points back from the unit circle to the ellipse */
    for(i = (write_first ? 0 : 1); i < 4; i ++){
        pt[i].X = pt[i].X * rad_x + center_x;
        pt[i].Y = pt[i].Y * rad_y + center_y;
    }
}

/* Like atan2, but puts angle in correct quadrant if dx is 0. */
REAL gdiplus_atan2(REAL dy, REAL dx)
{
    if((dx == 0.0) && (dy != 0.0))
        return dy > 0.0 ? M_PI_2 : -M_PI_2;

    return atan2(dy, dx);
}

/* We plot the curve as if it is on a circle then stretch the points.  This
 * adjusts the angles so that when we stretch the points they will end in the
 * right place. This is only complicated because atan and atan2 do not behave
 * conveniently. */
static void unstretch_angle(REAL * angle, REAL rad_x, REAL rad_y)
{
    REAL stretched;
    INT revs_off;

    *angle = deg2rad(*angle);

    if(fabs(cos(*angle)) < 0.00001 || fabs(sin(*angle)) < 0.00001)
        return;

    stretched = gdiplus_atan2(sin(*angle) / fabs(rad_y), cos(*angle) / fabs(rad_x));
    revs_off = gdip_round(*angle / (2.0 * M_PI)) - gdip_round(stretched / (2.0 * M_PI));
    stretched += ((REAL)revs_off) * M_PI * 2.0;
    *angle = stretched;
}

/* Stores the bezier points that correspond to the arc in points.  If points is
 * null, just return the number of points needed to represent the arc. */
INT arc2polybezier(PointF * points, REAL x1, REAL y1, REAL x2, REAL y2,
    REAL startAngle, REAL sweepAngle)
{
    INT i;
    REAL end_angle, start_angle, endAngle;

    endAngle = startAngle + sweepAngle;
    unstretch_angle(&startAngle, x2 / 2.0, y2 / 2.0);
    unstretch_angle(&endAngle, x2 / 2.0, y2 / 2.0);

    /* start_angle and end_angle are the iterative variables */
    start_angle = startAngle;

    for(i = 0; i < MAX_ARC_PTS - 1; i += 3){
        /* check if we've overshot the end angle */
        if( sweepAngle > 0.0 )
        {
            if (start_angle >= endAngle) break;
            end_angle = MIN(start_angle + M_PI_2, endAngle);
        }
        else
        {
            if (start_angle <= endAngle) break;
            end_angle = MAX(start_angle - M_PI_2, endAngle);
        }

        if (points)
            add_arc_part(&points[i], x1, y1, x2, y2, start_angle, end_angle, i == 0);

        start_angle += M_PI_2 * (sweepAngle < 0.0 ? -1.0 : 1.0);
    }

    if (i == 0) return 0;
    else return i+1;
}

COLOR ARGB2COLORREF(ARGB color)
{
    /*
    Packing of these color structures:
    COLOR:   00bbggrr
    ARGB:       aarrggbb
    FIXME:doesn't handle alpha channel
    */
    return ((color & 0x0000ff) << 16) +
           (color & 0x00ff00) +
           ((color & 0xff0000) >> 16);
}

#if 0
HBITMAP ARGB2BMP(ARGB color)
{
    BITMAPINFO bi;
    HBITMAP result;
    RGBQUAD *bits;
    int alpha;

    if ((color & 0xff000000) == 0xff000000) return 0;

    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = 1;
    bi.bmiHeader.biHeight = 1;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;
    bi.bmiHeader.biXPelsPerMeter = 0;
    bi.bmiHeader.biYPelsPerMeter = 0;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;

    result = CreateDIBSection(0, &bi, DIB_RGB_COLORS, (void*)&bits, NULL, 0);

    bits[0].rgbReserved = alpha = (color>>24)&0xff;
    bits[0].rgbRed = ((color>>16)&0xff)*alpha/255;
    bits[0].rgbGreen = ((color>>8)&0xff)*alpha/255;
    bits[0].rgbBlue = (color&0xff)*alpha/255;

    return result;
}
#endif

#define FIXME printf

/* converts a given unit to its value in pixels */
REAL units_to_pixels(REAL units, Unit unit, REAL dpi)
{
    switch (unit)
    {
    case UnitPixel:
    case UnitWorld:
    case UnitDisplay:
        return units;
    case UnitPoint:
        return units * dpi / point_per_inch;
    case UnitInch:
        return units * dpi;
    case UnitDocument:
        return units * dpi / 300.0; /* Per MSDN */
    case UnitMillimeter:
        return units * dpi / mm_per_inch;
    default:
        FIXME("Unhandled unit type: %d\n", unit);
        return 0;
    }
}

/* converts value in pixels to a given unit */
REAL pixels_to_units(REAL pixels, Unit unit, REAL dpi)
{
    switch (unit)
    {
    case UnitPixel:
    case UnitWorld:
    case UnitDisplay:
        return pixels;
    case UnitPoint:
        return pixels * point_per_inch / dpi;
    case UnitInch:
        return pixels / dpi;
    case UnitDocument:
        return pixels * 300.0 / dpi;
    case UnitMillimeter:
        return pixels * mm_per_inch / dpi;
    default:
        FIXME("Unhandled unit type: %d\n", unit);
        return 0;
    }
}

REAL units_scale(Unit from, Unit to, REAL dpi)
{
    REAL pixels = units_to_pixels(1.0, from, dpi);
    return pixels_to_units(pixels, to, dpi);
}

#define CONST const
/* Calculates Bezier points from cardinal spline points. */
void calc_curve_bezier(CONST PointF *pts, REAL tension, REAL *x1,
    REAL *y1, REAL *x2, REAL *y2)
{
    REAL xdiff, ydiff;

    /* calculate tangent */
    xdiff = pts[2].X - pts[0].X;
    ydiff = pts[2].Y - pts[0].Y;

    /* apply tangent to get control points */
    *x1 = pts[1].X - tension * xdiff;
    *y1 = pts[1].Y - tension * ydiff;
    *x2 = pts[1].X + tension * xdiff;
    *y2 = pts[1].Y + tension * ydiff;
}

/* Calculates Bezier points from cardinal spline endpoints. */
void calc_curve_bezier_endp(REAL xend, REAL yend, REAL xadj, REAL yadj,
    REAL tension, REAL *x, REAL *y)
{
    /* tangent at endpoints is the line from the endpoint to the adjacent point */
    *x = gdip_round(tension * (xadj - xend) + xend);
    *y = gdip_round(tension * (yadj - yend) + yend);
}

/* make sure path has enough space for len more points */
BOOL lengthen_path(Path *path, INT len)
{
    /* initial allocation */
    if(path->datalen == 0){
        path->datalen = len * 2;
        PathData_SetSize(&path->pathdata, path->datalen);
    }
    /* reallocation, double size of arrays */
    else if(path->datalen - path->pathdata.Count < len){
        while(path->datalen - path->pathdata.Count < len)
            path->datalen *= 2;
        
        PathData_SetSize(&path->pathdata, path->datalen);
    }

    return TRUE;
}

void convert_32bppARGB_to_32bppPARGB(UINT width, UINT height,
    BYTE *dst_bits, INT dst_stride, const BYTE *src_bits, INT src_stride)
{
    UINT x, y;
    for (y=0; y<height; y++)
    {
        const BYTE *src=src_bits+y*src_stride;
        BYTE *dst=dst_bits+y*dst_stride;
        for (x=0; x<width; x++)
        {
            BYTE alpha=src[3];
            *dst++ = *src++ * alpha / 255;
            *dst++ = *src++ * alpha / 255;
            *dst++ = *src++ * alpha / 255;
            *dst++ = *src++;
        }
    }
}
