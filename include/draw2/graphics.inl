
/* Mike "tamlin" Nordell 2012-09-14 for ReactOS:
 * NOTE: Wine uses per-Graphics id's ('contid' starting from zero in
 * every Graphics). Windows seems to use process-global id's, or at
 * least more unique id's.
 * This have the following implications. It:
 * 1. fails the current gdiplus test case.
 * 2. is not what Windows does.
 * 
 * We therefore "obfuscate" the 'contid' a little to more match Windows'
 * behaviour. The observable behviour should still remain the same,
 * except for handing out more "unique" id's.
 */
#define GDIP_CONTID_STEP 64
static volatile LONG g_priv_contid = GDIP_CONTID_STEP;
#define GDIP_GET_NEW_CONTID_FOR(pGpGraphics) \
   (UINT)(InterlockedExchangeAdd(&g_priv_contid,GDIP_CONTID_STEP))

/* looks-right constants */
#define ANCHOR_WIDTH (2.0)
#define MAX_ITERS (50)

static Status draw_driver_string(Graphics *graphics, GDIPCONST UINT16 *text, INT length,
                                   GDIPCONST Font *font, GDIPCONST StringFormat *format,
                                   GDIPCONST Brush *brush, GDIPCONST PointF *positions,
                                   INT flags, GDIPCONST Matrix *matrix);

/* Converts angle (in degrees) to x/y coordinates */
static void deg2xy(REAL angle, REAL x_0, REAL y_0, REAL *x, REAL *y)
{
    REAL radAngle, hypotenuse;

    radAngle = deg2rad(angle);
    hypotenuse = 50.0; /* arbitrary */

    *x = x_0 + cos(radAngle) * hypotenuse;
    *y = y_0 + sin(radAngle) * hypotenuse;
}

static COLOR get_gdi_brush_color(const Brush *brush)
{
    ARGB argb;

    switch (brush->bt)
    {
        case BrushTypeSolidColor:
        {
            const SolidFill *sf = &brush->solid;
            argb = sf->color;
            break;
        }
        case BrushTypeHatchFill:
        {
            const Hatch *hatch = &brush->hatch;
            argb = hatch->forecol;
            break;
        }
        case BrushTypeLinearGradient:
        {
            const LineGradient *line = &brush->line;
            argb = line->startcolor;
            break;
        }
        case BrushTypePathGradient:
        {
            const PathGradient *grad = &brush->grad;
            argb = grad->centercolor;
            break;
        }
        default:
            FIXME("unhandled brush type %d\n", brush->bt);
            argb = 0;
            break;
    }
    return ARGB2COLORREF(argb);
}

static void create_hatch_bitmap(const Hatch *hatch, DWORD* bits)
{
    int x, y;

    if (bits)
    {
        const uchar *hatch_data;

        if (get_hatch_data(hatch->hatchstyle, &hatch_data) == Ok)
        {
            for (y = 0; y < 8; y++)
            {
                for (x = 0; x < 8; x++)
                {
                    if (hatch_data[y] & (0x80 >> x))
                        bits[y * 8 + x] = hatch->forecol;
                    else
                        bits[y * 8 + x] = hatch->backcol;
                }
            }
        }
        else
        {
            FIXME("Unimplemented hatch style %d\n", hatch->hatchstyle);

            for (y = 0; y < 64; y++)
                bits[y] = hatch->forecol;
        }
    }

    return ;
}

static Status get_graphics_transform(Graphics *graphics, CoordinateSpace dst_space,
                                     CoordinateSpace src_space, Matrix *matrix)
{
  Status stat = Ok;
  REAL scale_x, scale_y;
  
  GdipSetMatrixElements(matrix, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  
  if (dst_space != src_space)
  {
    scale_x = units_to_pixels(1.0, graphics->unit, graphics->xres);
    scale_y = units_to_pixels(1.0, graphics->unit, graphics->yres);
    
    if(graphics->unit != UnitDisplay)
    {
      scale_x *= graphics->scale;
      scale_y *= graphics->scale;
    }
    
    /* transform from src_space to CoordinateSpacePage */
    switch (src_space)
    {
    case CoordinateSpaceWorld:
      GdipMultiplyMatrix(matrix, &graphics->worldtrans, MatrixOrderAppend);
      break;
    case CoordinateSpacePage:
      break;
    case CoordinateSpaceDevice:
      GdipScaleMatrix(matrix, 1.0/scale_x, 1.0/scale_y, MatrixOrderAppend);
      break;
    }
    
    /* transform from CoordinateSpacePage to dst_space */
    switch (dst_space)
    {
    case CoordinateSpaceWorld:
      {
        Matrix inverted_transform = graphics->worldtrans;
        stat = GdipInvertMatrix(&inverted_transform);
        if (stat == Ok)
          GdipMultiplyMatrix(matrix, &inverted_transform, MatrixOrderAppend);
        break;
      }
    case CoordinateSpacePage:
      break;
    case CoordinateSpaceDevice:
      GdipScaleMatrix(matrix, scale_x, scale_y, MatrixOrderAppend);
      break;
    }
  }
  return stat;
}

static INT prepare_dc(Graphics *graphics, Pen *pen)
{
    REAL width;
    INT save_state, i, numdashes;
    PointF pt[2];
    DWORD dash_array[MAX_DASHLEN];

    if(pen->unit == UnitPixel){
        width = pen->width;
    }
    else{
        /* Get an estimate for the amount the pen width is affected by the world
         * transform. (This is similar to what some of the wine drivers do.) */
        pt[0].X = 0.0;
        pt[0].Y = 0.0;
        pt[1].X = 1.0;
        pt[1].Y = 1.0;
        GdipTransformMatrixPoints(&graphics->worldtrans, pt, 2);
        width = sqrt((pt[1].X - pt[0].X) * (pt[1].X - pt[0].X) +
                     (pt[1].Y - pt[0].Y) * (pt[1].Y - pt[0].Y)) / sqrt(2.0);

        width *= units_to_pixels(pen->width, pen->unit == UnitWorld ? graphics->unit : pen->unit, graphics->xres);
    }

    if(pen->dash == DashStyleCustom){
        numdashes = MIN(pen->numdashes, MAX_DASHLEN);

        TRACE("dashes are: ");
        for(i = 0; i < numdashes; i++){
            dash_array[i] = gdip_round(width * pen->dashes[i]);
            TRACE("%d, ", dash_array[i]);
        }
        TRACE("\n and the pen style is %x\n", pen->style);

    }
    else
    {
    }

    return save_state;
}

/* This helper applies all the changes that the points listed in ptf need in
 * order to be drawn on the device context.  In the end, this should include at
 * least:
 *  -scaling by page unit
 *  -applying world transformation
 *  -converting from float to int
 * Native gdiplus uses gdi32 to do all this (via SetMapMode, SetViewportExtEx,
 * SetWindowExtEx, SetWorldTransform, etc.) but we cannot because we are using
 * gdi to draw, and these functions would irreparably mess with line widths.
 */
static void transform_and_round_points(Graphics *graphics, IPOINT *pti,
    PointF *ptf, INT count)
{
    REAL scale_x, scale_y;
    Matrix matrix;
    int i;

    scale_x = units_to_pixels(1.0, graphics->unit, graphics->xres);
    scale_y = units_to_pixels(1.0, graphics->unit, graphics->yres);

    /* apply page scale */
    if(graphics->unit != UnitDisplay)
    {
        scale_x *= graphics->scale;
        scale_y *= graphics->scale;
    }

    matrix = graphics->worldtrans;
    GdipScaleMatrix(&matrix, scale_x, scale_y, MatrixOrderAppend);
    GdipTransformMatrixPoints(&matrix, ptf, count);

    for(i = 0; i < count; i++){
        pti[i].x = gdip_round(ptf[i].X);
        pti[i].y = gdip_round(ptf[i].Y);
    }
}

static void gdi_alpha_blend(Graphics *graphics, INT dst_x, INT dst_y, INT dst_width, INT dst_height,
                            void* hdc, INT src_x, INT src_y, INT src_width, INT src_height)
{
  ASSERT(0);
}

static Status get_clip_hrgn(Graphics *graphics, void *hrgn)
{
  ASSERT(0);
    return Ok;
}

/* Draw non-premultiplied ARGB data to the given graphics object */
static Status alpha_blend_bmp_pixels(Graphics *graphics, INT dst_x, INT dst_y,
    const BYTE *src, INT src_width, INT src_height, INT src_stride)
{
    Bitmap *dst_bitmap = (Bitmap*)graphics->image;
    INT x, y;

    for (x=0; x<src_width; x++)
    {
        for (y=0; y<src_height; y++)
        {
            ARGB dst_color, src_color;
            GdipBitmapGetPixel(dst_bitmap, x+dst_x, y+dst_y, &dst_color);
            src_color = ((ARGB*)(src + src_stride * y))[x];
            GdipBitmapSetPixel(dst_bitmap, x+dst_x, y+dst_y, color_over(dst_color, src_color));
        }
    }

    return Ok;
}

static Status alpha_blend_hdc_pixels(Graphics *graphics, INT dst_x, INT dst_y,
    const BYTE *src, INT src_width, INT src_height, INT src_stride)
{
    //convert_32bppARGB_to_32bppPARGB(src_width, src_height, temp_bits, 4 * src_width, src, src_stride);
  //gdi_alpha_blend(graphics, dst_x, dst_y, src_width, src_height, hdc, 0, 0, src_width, src_height);
    ASSERT(0);
    return Ok;
}

static Status alpha_blend_pixels_hrgn(Graphics *graphics, INT dst_x, INT dst_y,
    const BYTE *src, INT src_width, INT src_height, INT src_stride, void* hregion)
{
  ASSERT(0);
    return Ok;
}

static Status alpha_blend_pixels(Graphics *graphics, INT dst_x, INT dst_y,
    const BYTE *src, INT src_width, INT src_height, INT src_stride)
{
  ASSERT(0);
    return Ok;
}

static ARGB blend_colors(ARGB start, ARGB end, REAL position)
{
    ARGB result=0;
    ARGB i;
    INT a1, a2, a3;

    a1 = (start >> 24) & 0xff;
    a2 = (end >> 24) & 0xff;

    a3 = (int)(a1*(1.0f - position)+a2*(position));

    result |= a3 << 24;

    for (i=0xff; i<=0xff0000; i = i << 8) {
      result |= (int)((start&i)*(1.0f - position)+(end&i)*(position))&i;
    }
    return result;
}

static ARGB blend_line_gradient(LineGradient* brush, REAL position)
{
    REAL blendfac;

    /* clamp to between 0.0 and 1.0, using the wrap mode */
    if (brush->wrap == WrapModeTile)
    {
        position = fmodf(position, 1.0f);
        if (position < 0.0f) position += 1.0f;
    }
    else /* WrapModeFlip* */
    {
        position = fmodf(position, 2.0f);
        if (position < 0.0f) position += 2.0f;
        if (position > 1.0f) position = 2.0f - position;
    }

    if (brush->blendcount == 1)
        blendfac = position;
    else
    {
        int i=1;
        REAL left_blendpos, left_blendfac, right_blendpos, right_blendfac;
        REAL range;

        /* locate the blend positions surrounding this position */
        while (position > brush->blendpos[i])
            i++;

        /* interpolate between the blend positions */
        left_blendpos = brush->blendpos[i-1];
        left_blendfac = brush->blendfac[i-1];
        right_blendpos = brush->blendpos[i];
        right_blendfac = brush->blendfac[i];
        range = right_blendpos - left_blendpos;
        blendfac = (left_blendfac * (right_blendpos - position) +
                    right_blendfac * (position - left_blendpos)) / range;
    }

    if (brush->pblendcount == 0)
        return blend_colors(brush->startcolor, brush->endcolor, blendfac);
    else
    {
        int i=1;
        ARGB left_blendcolor, right_blendcolor;
        REAL left_blendpos, right_blendpos;

        /* locate the blend colors surrounding this position */
        while (blendfac > brush->pblendpos[i])
            i++;

        /* interpolate between the blend colors */
        left_blendpos = brush->pblendpos[i-1];
        left_blendcolor = brush->pblendcolor[i-1];
        right_blendpos = brush->pblendpos[i];
        right_blendcolor = brush->pblendcolor[i];
        blendfac = (blendfac - left_blendpos) / (right_blendpos - left_blendpos);
        return blend_colors(left_blendcolor, right_blendcolor, blendfac);
    }
}

static ARGB transform_color(ARGB color, const ColorMatrix *matrix)
{
    REAL val[5], res[4];
    int i, j;
    unsigned char a, r, g, b;

    val[0] = ((color >> 16) & 0xff) / 255.0; /* red */
    val[1] = ((color >> 8) & 0xff) / 255.0; /* green */
    val[2] = (color & 0xff) / 255.0; /* blue */
    val[3] = ((color >> 24) & 0xff) / 255.0; /* alpha */
    val[4] = 1.0; /* translation */

    for (i=0; i<4; i++)
    {
        res[i] = 0.0;

        for (j=0; j<5; j++)
            res[i] += matrix->m[j][i] * val[j];
    }

    a = MIN(MAX(floorf(res[3]*255.0), 0.0), 255.0);
    r = MIN(MAX(floorf(res[0]*255.0), 0.0), 255.0);
    g = MIN(MAX(floorf(res[1]*255.0), 0.0), 255.0);
    b = MIN(MAX(floorf(res[2]*255.0), 0.0), 255.0);

    return (a << 24) | (r << 16) | (g << 8) | b;
}

static int color_is_gray(ARGB color)
{
    unsigned char r, g, b;

    r = (color >> 16) & 0xff;
    g = (color >> 8) & 0xff;
    b = color & 0xff;

    return (r == g) && (g == b);
}

static void apply_image_attributes(const ImageAttributes *attributes, LPBYTE data,
    UINT width, UINT height, INT stride, ColorAdjustType type)
{
    UINT x, y, i;

    if (attributes->colorkeys[type].enabled ||
        attributes->colorkeys[ColorAdjustTypeDefault].enabled)
    {
        const color_key *key;
        BYTE min_blue, min_green, min_red;
        BYTE max_blue, max_green, max_red;

        if (attributes->colorkeys[type].enabled)
            key = &attributes->colorkeys[type];
        else
            key = &attributes->colorkeys[ColorAdjustTypeDefault];

        min_blue = key->low&0xff;
        min_green = (key->low>>8)&0xff;
        min_red = (key->low>>16)&0xff;

        max_blue = key->high&0xff;
        max_green = (key->high>>8)&0xff;
        max_red = (key->high>>16)&0xff;

        for (x=0; x<width; x++)
            for (y=0; y<height; y++)
            {
                ARGB *src_color;
                BYTE blue, green, red;
                src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);
                blue = *src_color&0xff;
                green = (*src_color>>8)&0xff;
                red = (*src_color>>16)&0xff;
                if (blue >= min_blue && green >= min_green && red >= min_red &&
                    blue <= max_blue && green <= max_green && red <= max_red)
                    *src_color = 0x00000000;
            }
    }

    if (attributes->colorremaptables[type].enabled ||
        attributes->colorremaptables[ColorAdjustTypeDefault].enabled)
    {
        const color_remap_table *table;

        if (attributes->colorremaptables[type].enabled)
            table = &attributes->colorremaptables[type];
        else
            table = &attributes->colorremaptables[ColorAdjustTypeDefault];

        for (x=0; x<width; x++) {
            for (y=0; y<height; y++) {
                ARGB *src_color;
                src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);
                for (i=0; i<table->mapsize; i++) {
                    if (*src_color == table->colormap[i].oldColor) {
                        *src_color = table->colormap[i].newColor;
                        break;
                    }
                }
            }
        }
    }

    if (attributes->colormatrices[type].enabled ||
        attributes->colormatrices[ColorAdjustTypeDefault].enabled)
    {
        const color_matrix *colormatrices;

        if (attributes->colormatrices[type].enabled)
            colormatrices = &attributes->colormatrices[type];
        else
            colormatrices = &attributes->colormatrices[ColorAdjustTypeDefault];

        for (x=0; x<width; x++)
            for (y=0; y<height; y++)
            {
                ARGB *src_color;
                src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);

                if (colormatrices->flags == ColorMatrixFlagsDefault ||
                    !color_is_gray(*src_color))
                {
                    *src_color = transform_color(*src_color, &colormatrices->colormatrix);
                }
                else if (colormatrices->flags == ColorMatrixFlagsAltGray)
                {
                    *src_color = transform_color(*src_color, &colormatrices->graymatrix);
                }
            }
    }

    if (attributes->gamma_enabled[type] ||
        attributes->gamma_enabled[ColorAdjustTypeDefault])
    {
        REAL gamma;

        if (attributes->gamma_enabled[type])
            gamma = attributes->gamma[type];
        else
            gamma = attributes->gamma[ColorAdjustTypeDefault];

        for (x=0; x<width; x++)
            for (y=0; y<height; y++)
            {
                ARGB *src_color;
                BYTE blue, green, red;
                src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);

                blue = *src_color&0xff;
                green = (*src_color>>8)&0xff;
                red = (*src_color>>16)&0xff;

                /* FIXME: We should probably use a table for this. */
                blue = floorf(powf(blue / 255.0, gamma) * 255.0);
                green = floorf(powf(green / 255.0, gamma) * 255.0);
                red = floorf(powf(red / 255.0, gamma) * 255.0);

                *src_color = (*src_color & 0xff000000) | (red << 16) | (green << 8) | blue;
            }
    }
}

/* Given a bitmap and its source rectangle, find the smallest rectangle in the
 * bitmap that contains all the pixels we may need to draw it. */
static void get_bitmap_sample_size(InterpolationMode interpolation, WrapMode wrap,
    Bitmap* bitmap, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight,
    Rect *rect)
{
    INT left, top, right, bottom;

    switch (interpolation)
    {
    case InterpolationModeHighQualityBilinear:
    case InterpolationModeHighQualityBicubic:
    /* FIXME: Include a greater range for the prefilter? */
    case InterpolationModeBicubic:
    case InterpolationModeBilinear:
        left = (INT)(floorf(srcx));
        top = (INT)(floorf(srcy));
        right = (INT)(ceilf(srcx+srcwidth));
        bottom = (INT)(ceilf(srcy+srcheight));
        break;
    case InterpolationModeNearestNeighbor:
    default:
        left = gdip_round(srcx);
        top = gdip_round(srcy);
        right = gdip_round(srcx+srcwidth);
        bottom = gdip_round(srcy+srcheight);
        break;
    }

    if (wrap == WrapModeClamp)
    {
        if (left < 0)
            left = 0;
        if (top < 0)
            top = 0;
        if (right >= bitmap->width)
            right = bitmap->width-1;
        if (bottom >= bitmap->height)
            bottom = bitmap->height-1;
    }
    else
    {
        /* In some cases we can make the rectangle smaller here, but the logic
         * is hard to get right, and tiling suggests we're likely to use the
         * entire source image. */
        if (left < 0 || right >= bitmap->width)
        {
            left = 0;
            right = bitmap->width-1;
        }

        if (top < 0 || bottom >= bitmap->height)
        {
            top = 0;
            bottom = bitmap->height-1;
        }
    }

    rect->X = left;
    rect->Y = top;
    rect->Width = right - left + 1;
    rect->Height = bottom - top + 1;
}

static ARGB sample_bitmap_pixel(GDIPCONST Rect *src_rect, LPBYTE bits, UINT width,
    UINT height, INT x, INT y, GDIPCONST ImageAttributes *attributes)
{
    if (attributes->wrap == WrapModeClamp)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return attributes->outside_color;
    }
    else
    {
        /* Tiling. Make sure co-ordinates are positive as it simplifies the math. */
        if (x < 0)
            x = width*2 + x % (width * 2);
        if (y < 0)
            y = height*2 + y % (height * 2);

        if ((attributes->wrap & 1) == 1)
        {
            /* Flip X */
            if ((x / width) % 2 == 0)
                x = x % width;
            else
                x = width - 1 - x % width;
        }
        else
            x = x % width;

        if ((attributes->wrap & 2) == 2)
        {
            /* Flip Y */
            if ((y / height) % 2 == 0)
                y = y % height;
            else
                y = height - 1 - y % height;
        }
        else
            y = y % height;
    }

    if (x < src_rect->X || y < src_rect->Y || x >= src_rect->X + src_rect->Width || y >= src_rect->Y + src_rect->Height)
    {
        ERR("out of range pixel requested\n");
        return 0xffcd0084;
    }

    return ((DWORD*)(bits))[(x - src_rect->X) + (y - src_rect->Y) * src_rect->Width];
}

static ARGB resample_bitmap_pixel(GDIPCONST Rect *src_rect, LPBYTE bits, UINT width,
    UINT height, PointF *point, GDIPCONST ImageAttributes *attributes,
    InterpolationMode interpolation, PixelOffsetMode offset_mode)
{
    static int fixme;

    switch (interpolation)
    {
    default:
        if (!fixme++)
            FIXME("Unimplemented interpolation %i\n", interpolation);
        /* fall-through */
    case InterpolationModeBilinear:
    {
        REAL leftxf, topyf;
        INT leftx, rightx, topy, bottomy;
        ARGB topleft, topright, bottomleft, bottomright;
        ARGB top, bottom;
        float x_offset;

        leftxf = floorf(point->X);
        leftx = (INT)leftxf;
        rightx = (INT)ceilf(point->X);
        topyf = floorf(point->Y);
        topy = (INT)topyf;
        bottomy = (INT)ceilf(point->Y);

        if (leftx == rightx && topy == bottomy)
            return sample_bitmap_pixel(src_rect, bits, width, height,
                leftx, topy, attributes);

        topleft = sample_bitmap_pixel(src_rect, bits, width, height,
            leftx, topy, attributes);
        topright = sample_bitmap_pixel(src_rect, bits, width, height,
            rightx, topy, attributes);
        bottomleft = sample_bitmap_pixel(src_rect, bits, width, height,
            leftx, bottomy, attributes);
        bottomright = sample_bitmap_pixel(src_rect, bits, width, height,
            rightx, bottomy, attributes);

        x_offset = point->X - leftxf;
        top = blend_colors(topleft, topright, x_offset);
        bottom = blend_colors(bottomleft, bottomright, x_offset);

        return blend_colors(top, bottom, point->Y - topyf);
    }
    case InterpolationModeNearestNeighbor:
    {
        FLOAT pixel_offset;
        switch (offset_mode)
        {
        default:
        case PixelOffsetModeNone:
        case PixelOffsetModeHighSpeed:
            pixel_offset = 0.5;
            break;

        case PixelOffsetModeHalf:
        case PixelOffsetModeHighQuality:
            pixel_offset = 0.0;
            break;
        }
        return sample_bitmap_pixel(src_rect, bits, width, height,
            floorf(point->X + pixel_offset), floorf(point->Y + pixel_offset), attributes);
    }

    }
}

static REAL intersect_line_scanline(const PointF *p1, const PointF *p2, REAL y)
{
    return (p1->X - p2->X) * (p2->Y - y) / (p2->Y - p1->Y) + p2->X;
}

static INT brush_can_fill_path(Brush *brush)
{
    switch (brush->bt)
    {
    case BrushTypeSolidColor:
        return 1;
    case BrushTypeHatchFill:
    {
        Hatch *hatch = (Hatch*)brush;
        return ((hatch->forecol & 0xff000000) == 0xff000000) &&
               ((hatch->backcol & 0xff000000) == 0xff000000);
    }
    case BrushTypeLinearGradient:
    case BrushTypeTextureFill:
    /* Gdi32 isn't much help with these, so we should use brush_fill_pixels instead. */
    default:
        return 0;
    }
}

static void brush_fill_path(Graphics *graphics, Brush* brush)
{
  ASSERT(0);
}

static INT brush_can_fill_pixels(Brush *brush)
{
    switch (brush->bt)
    {
    case BrushTypeSolidColor:
    case BrushTypeHatchFill:
    case BrushTypeLinearGradient:
    case BrushTypeTextureFill:
    case BrushTypePathGradient:
        return 1;
    default:
        return 0;
    }
}

Status GdipTransformPoints(Graphics *graphics, CoordinateSpace dst_space,
                           CoordinateSpace src_space, PointF *points, INT count)
{
  Matrix matrix;
  Status stat;
  
  if(!graphics || !points || count <= 0)
    return InvalidParameter;
  
  if(graphics->busy)
    return ObjectBusy;
  
  TRACE("(%p, %d, %d, %p, %d)\n", graphics, dst_space, src_space, points, count);
  
  if (src_space == dst_space) return Ok;
  
  stat = get_graphics_transform(graphics, dst_space, src_space, &matrix);
  if (stat != Ok) return stat;
  
  return GdipTransformMatrixPoints(&matrix, points, count);
}

Status GdipTransformPointsI(Graphics *graphics, CoordinateSpace dst_space,
                            CoordinateSpace src_space, Point *points, INT count)
{
  PointF *pointsF = NULL;
  Status ret;
  INT i;
  
  TRACE("(%p, %d, %d, %p, %d)\n", graphics, dst_space, src_space, points, count);
  
  if(count <= 0)
    return InvalidParameter;
  
  MYREALLOC(pointsF, count);
  if(!pointsF)
    return OutOfMemory;
  
  for(i = 0; i < count; i++){
    pointsF[i].X = (REAL)points[i].X;
    pointsF[i].Y = (REAL)points[i].Y;
  }
  
  ret = GdipTransformPoints(graphics, dst_space, src_space, pointsF, count);
  
  if(ret == Ok) {
    for(i = 0; i < count; i++){
      points[i].X = gdip_round(pointsF[i].X);
      points[i].Y = gdip_round(pointsF[i].Y);
    }
  }
  FREE(pointsF);
    
    return ret;
}

static Status brush_fill_pixels(Graphics *graphics, Brush *brush,
    DWORD *argb_pixels, Rect *fill_area, UINT cdwStride)
{
    switch (brush->bt)
    {
    case BrushTypeSolidColor:
    {
        int x, y;
        SolidFill *fill = (SolidFill*)brush;
        for (x=0; x<fill_area->Width; x++)
            for (y=0; y<fill_area->Height; y++)
                argb_pixels[x + y*cdwStride] = fill->color;
        return Ok;
    }
    case BrushTypeHatchFill:
    {
        int x, y;
        Hatch *fill = (Hatch*)brush;
        const uchar *hatch_data;

        if (get_hatch_data(fill->hatchstyle, &hatch_data) != Ok)
            return NotImplemented;

        for (x=0; x<fill_area->Width; x++)
            for (y=0; y<fill_area->Height; y++)
            {
                int hx, hy;

                /* FIXME: Account for the rendering origin */
                hx = (x + fill_area->X) % 8;
                hy = (y + fill_area->Y) % 8;

                if ((hatch_data[7-hy] & (0x80 >> hx)) != 0)
                    argb_pixels[x + y*cdwStride] = fill->forecol;
                else
                    argb_pixels[x + y*cdwStride] = fill->backcol;
            }

        return Ok;
    }
    case BrushTypeLinearGradient:
    {
        LineGradient *fill = (LineGradient*)brush;
        PointF draw_points[3], line_points[3];
        Status stat;
        static const RectF box_1 = { 0.0, 0.0, 1.0, 1.0 };
        Matrix world_to_gradient[1]; /* FIXME: Store this in the brush? */
        int x, y;

        draw_points[0].X = fill_area->X;
        draw_points[0].Y = fill_area->Y;
        draw_points[1].X = fill_area->X+1;
        draw_points[1].Y = fill_area->Y;
        draw_points[2].X = fill_area->X;
        draw_points[2].Y = fill_area->Y+1;

        /* Transform the points to a co-ordinate space where X is the point's
         * position in the gradient, 0.0 being the start point and 1.0 the
         * end point. */
        stat = GdipTransformPoints(graphics, CoordinateSpaceWorld,
            CoordinateSpaceDevice, draw_points, 3);

        if (stat == Ok)
        {
            line_points[0] = fill->startpoint;
            line_points[1] = fill->endpoint;
            line_points[2].X = fill->startpoint.X + (fill->startpoint.Y - fill->endpoint.Y);
            line_points[2].Y = fill->startpoint.Y + (fill->endpoint.X - fill->startpoint.X);

            stat = GdipCreateMatrix3(&box_1, line_points, world_to_gradient);
        }

        if (stat == Ok)
        {
            stat = GdipInvertMatrix(world_to_gradient);

            if (stat == Ok)
                stat = GdipTransformMatrixPoints(world_to_gradient, draw_points, 3);

            GdipDeleteMatrix(world_to_gradient);
        }

        if (stat == Ok)
        {
            REAL x_delta = draw_points[1].X - draw_points[0].X;
            REAL y_delta = draw_points[2].X - draw_points[0].X;

            for (y=0; y<fill_area->Height; y++)
            {
                for (x=0; x<fill_area->Width; x++)
                {
                    REAL pos = draw_points[0].X + x * x_delta + y * y_delta;

                    argb_pixels[x + y*cdwStride] = blend_line_gradient(fill, pos);
                }
            }
        }

        return stat;
    }
    case BrushTypeTextureFill:
    {
#if 0
        Texture *fill = (Texture*)brush;
        PointF draw_points[3];
        Status stat;
        int x, y;
        Bitmap *bitmap;
        int src_stride;
        Rect src_area;

        bitmap = (Bitmap*)fill->image;
        src_stride = sizeof(ARGB) * bitmap->width;

        src_area.X = src_area.Y = 0;
        src_area.Width = bitmap->width;
        src_area.Height = bitmap->height;

        draw_points[0].X = fill_area->X;
        draw_points[0].Y = fill_area->Y;
        draw_points[1].X = fill_area->X+1;
        draw_points[1].Y = fill_area->Y;
        draw_points[2].X = fill_area->X;
        draw_points[2].Y = fill_area->Y+1;

        /* Transform the points to the co-ordinate space of the bitmap. */
        stat = GdipTransformPoints(graphics, CoordinateSpaceWorld,
            CoordinateSpaceDevice, draw_points, 3);

        if (stat == Ok)
        {
            Matrix world_to_texture = fill->transform;

            stat = GdipInvertMatrix(&world_to_texture);
            if (stat == Ok)
                stat = GdipTransformMatrixPoints(&world_to_texture, draw_points, 3);
        }

        if (stat == Ok && !fill->bitmap_bits)
        {
            BitmapData lockeddata;

            fill->bitmap_bits = GdipAlloc(sizeof(ARGB) * bitmap->width * bitmap->height);
            if (!fill->bitmap_bits)
                stat = OutOfMemory;

            if (stat == Ok)
            {
                lockeddata.Width = bitmap->width;
                lockeddata.Height = bitmap->height;
                lockeddata.Stride = src_stride;
                lockeddata.PixelFormat = PixelFormat32bppARGB;
                lockeddata.Scan0 = fill->bitmap_bits;

                stat = GdipBitmapLockBits(bitmap, &src_area, ImageLockModeRead|ImageLockModeUserInputBuf,
                    PixelFormat32bppARGB, &lockeddata);
            }

            if (stat == Ok)
                stat = GdipBitmapUnlockBits(bitmap, &lockeddata);

            if (stat == Ok)
                apply_image_attributes(fill->imageattributes, fill->bitmap_bits,
                    bitmap->width, bitmap->height,
                    src_stride, ColorAdjustTypeBitmap);

            if (stat != Ok)
            {
                GdipFree(fill->bitmap_bits);
                fill->bitmap_bits = NULL;
            }
        }

        if (stat == Ok)
        {
            REAL x_dx = draw_points[1].X - draw_points[0].X;
            REAL x_dy = draw_points[1].Y - draw_points[0].Y;
            REAL y_dx = draw_points[2].X - draw_points[0].X;
            REAL y_dy = draw_points[2].Y - draw_points[0].Y;

            for (y=0; y<fill_area->Height; y++)
            {
                for (x=0; x<fill_area->Width; x++)
                {
                    PointF point;
                    point.X = draw_points[0].X + x * x_dx + y * y_dx;
                    point.Y = draw_points[0].Y + y * x_dy + y * y_dy;

                    argb_pixels[x + y*cdwStride] = resample_bitmap_pixel(
                        &src_area, fill->bitmap_bits, bitmap->width, bitmap->height,
                        &point, fill->imageattributes, graphics->interpolation,
                        graphics->pixeloffset);
                }
            }
        }

        return stat;
#endif
    }
    break;
    case BrushTypePathGradient:
    {
#if 0
        PathGradient *fill = (PathGradient*)brush;
        Path *flat_path;
        Matrix world_to_device;
        Status stat;
        int i, figure_start=0;
        PointF start_point, end_point, center_point;
        BYTE type;
        REAL min_yf, max_yf, line1_xf, line2_xf;
        INT min_y, max_y, min_x, max_x;
        INT x, y;
        ARGB outer_color;
        static int transform_fixme_once;

        if (fill->focus.X != 0.0 || fill->focus.Y != 0.0)
        {
            static int once;
            if (!once++)
                FIXME("path gradient focus not implemented\n");
        }

        if (fill->gamma)
        {
            static int once;
            if (!once++)
                FIXME("path gradient gamma correction not implemented\n");
        }

        if (fill->blendcount)
        {
            static int once;
            if (!once++)
                FIXME("path gradient blend not implemented\n");
        }

        if (fill->pblendcount)
        {
            static int once;
            if (!once++)
                FIXME("path gradient preset blend not implemented\n");
        }

        if (!transform_fixme_once)
        {
            BOOL is_identity=TRUE;
            GdipIsMatrixIdentity(&fill->transform, &is_identity);
            if (!is_identity)
            {
                FIXME("path gradient transform not implemented\n");
                transform_fixme_once = 1;
            }
        }

        stat = GdipClonePath(fill->path, &flat_path);

        if (stat != Ok)
            return stat;

        stat = get_graphics_transform(graphics, CoordinateSpaceDevice,
            CoordinateSpaceWorld, &world_to_device);
        if (stat == Ok)
        {
            stat = GdipTransformPath(flat_path, &world_to_device);

            if (stat == Ok)
            {
                center_point = fill->center;
                stat = GdipTransformMatrixPoints(&world_to_device, &center_point, 1);
            }

            if (stat == Ok)
                stat = GdipFlattenPath(flat_path, NULL, 0.5);
        }

        if (stat != Ok)
        {
            PathDelete(flat_path);
            return stat;
        }

        for (i=0; i<flat_path->pathdata.Count; i++)
        {
            int start_center_line=0, end_center_line=0;
            int seen_start=0, seen_end=0, seen_center=0;
            REAL center_distance;
            ARGB start_color, end_color;
            REAL dy, dx;

            type = flat_path->pathdata.Types[i];

            if ((type&PathPointTypePathTypeMask) == PathPointTypeStart)
                figure_start = i;

            start_point = flat_path->pathdata.Points[i];

            start_color = fill->surroundcolors[MIN(i, fill->surroundcolorcount-1)];

            if ((type&PathPointTypeCloseSubpath) == PathPointTypeCloseSubpath || i+1 >= flat_path->pathdata.Count)
            {
                end_point = flat_path->pathdata.Points[figure_start];
                end_color = fill->surroundcolors[MIN(figure_start, fill->surroundcolorcount-1)];
            }
            else if ((flat_path->pathdata.Types[i+1] & PathPointTypePathTypeMask) == PathPointTypeLine)
            {
                end_point = flat_path->pathdata.Points[i+1];
                end_color = fill->surroundcolors[MIN(i+1, fill->surroundcolorcount-1)];
            }
            else
                continue;

            outer_color = start_color;

            min_yf = center_point.Y;
            if (min_yf > start_point.Y) min_yf = start_point.Y;
            if (min_yf > end_point.Y) min_yf = end_point.Y;

            if (min_yf < fill_area->Y)
                min_y = fill_area->Y;
            else
                min_y = (INT)ceil(min_yf);

            max_yf = center_point.Y;
            if (max_yf < start_point.Y) max_yf = start_point.Y;
            if (max_yf < end_point.Y) max_yf = end_point.Y;

            if (max_yf > fill_area->Y + fill_area->Height)
                max_y = fill_area->Y + fill_area->Height;
            else
                max_y = (INT)ceil(max_yf);

            dy = end_point.Y - start_point.Y;
            dx = end_point.X - start_point.X;

            /* This is proportional to the distance from start-end line to center point. */
            center_distance = dy * (start_point.X - center_point.X) +
                dx * (center_point.Y - start_point.Y);

            for (y=min_y; y<max_y; y++)
            {
                REAL yf = (REAL)y;

                if (!seen_start && yf >= start_point.Y)
                {
                    seen_start = 1;
                    start_center_line ^= 1;
                }
                if (!seen_end && yf >= end_point.Y)
                {
                    seen_end = 1;
                    end_center_line ^= 1;
                }
                if (!seen_center && yf >= center_point.Y)
                {
                    seen_center = 1;
                    start_center_line ^= 1;
                    end_center_line ^= 1;
                }

                if (start_center_line)
                    line1_xf = intersect_line_scanline(&start_point, &center_point, yf);
                else
                    line1_xf = intersect_line_scanline(&start_point, &end_point, yf);

                if (end_center_line)
                    line2_xf = intersect_line_scanline(&end_point, &center_point, yf);
                else
                    line2_xf = intersect_line_scanline(&start_point, &end_point, yf);

                if (line1_xf < line2_xf)
                {
                    min_x = (INT)ceil(line1_xf);
                    max_x = (INT)ceil(line2_xf);
                }
                else
                {
                    min_x = (INT)ceil(line2_xf);
                    max_x = (INT)ceil(line1_xf);
                }

                if (min_x < fill_area->X)
                    min_x = fill_area->X;
                if (max_x > fill_area->X + fill_area->Width)
                    max_x = fill_area->X + fill_area->Width;

                for (x=min_x; x<max_x; x++)
                {
                    REAL xf = (REAL)x;
                    REAL distance;

                    if (start_color != end_color)
                    {
                        REAL blend_amount, pdy, pdx;
                        pdy = yf - center_point.Y;
                        pdx = xf - center_point.X;
                        blend_amount = ( (center_point.Y - start_point.Y) * pdx + (start_point.X - center_point.X) * pdy ) / ( dy * pdx - dx * pdy );
                        outer_color = blend_colors(start_color, end_color, blend_amount);
                    }

                    distance = (end_point.Y - start_point.Y) * (start_point.X - xf) +
                        (end_point.X - start_point.X) * (yf - start_point.Y);

                    distance = distance / center_distance;

                    argb_pixels[(x-fill_area->X) + (y-fill_area->Y)*cdwStride] =
                        blend_colors(outer_color, fill->centercolor, distance);
                }
            }
        }

        PathDelete(flat_path);
        return stat;
#endif
    }
    break;
    default:
        return NotImplemented;
    }
}

/* GdipDrawPie/GdipFillPie helper function */
static void draw_pie(Graphics *graphics, REAL x, REAL y, REAL width,
    REAL height, REAL startAngle, REAL sweepAngle)
{
    PointF ptf[4];
    IPOINT pti[4];

    ptf[0].X = x;
    ptf[0].Y = y;
    ptf[1].X = x + width;
    ptf[1].Y = y + height;

    deg2xy(startAngle+sweepAngle, x + width / 2.0, y + width / 2.0, &ptf[2].X, &ptf[2].Y);
    deg2xy(startAngle, x + width / 2.0, y + width / 2.0, &ptf[3].X, &ptf[3].Y);

    transform_and_round_points(graphics, pti, ptf, 4);

    //Pie(graphics->hdc, pti[0].x, pti[0].y, pti[1].x, pti[1].y, pti[2].x, pti[2].y, pti[3].x, pti[3].y);
}

/* Draws the linecap the specified color and size on the hdc.  The linecap is in
 * direction of the line from x1, y1 to x2, y2 and is anchored on x2, y2. Probably
 * should not be called on an hdc that has a path you care about. */
static void draw_cap(Graphics *graphics, COLOR color, LineCap cap, REAL size,
    const CustomLineCap *custom, REAL x1, REAL y1, REAL x2, REAL y2)
{
    HGDIOBJ oldbrush = NULL, oldpen = NULL;
    Matrix matrix;
    HBRUSH brush = NULL;
    HPEN pen = NULL;
    PointF ptf[4], *custptf = NULL;
    IPOINT pt[4], *custpt = NULL;
    BYTE *tp = NULL;
    REAL theta, dsmall, dbig, dx, dy = 0.0;
    INT i, count;
    LOGBRUSH lb;
    BOOL customstroke;

    if((x1 == x2) && (y1 == y2))
        return;

    theta = gdiplus_atan2(y2 - y1, x2 - x1);

    customstroke = (cap == LineCapCustom) && custom && (!custom->fill);
    if(!customstroke){
        brush = CreateSolidBrush(color);
        lb.lbStyle = BS_SOLID;
        lb.lbColor = color;
        lb.lbHatch = 0;
        pen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT |
                           PS_JOIN_MITER, 1, &lb, 0,
                           NULL);
        oldbrush = SelectObject(graphics->hdc, brush);
        oldpen = SelectObject(graphics->hdc, pen);
    }

    switch(cap){
        case LineCapFlat:
            break;
        case LineCapSquare:
        case LineCapSquareAnchor:
        case LineCapDiamondAnchor:
            size = size * (cap & LineCapNoAnchor ? ANCHOR_WIDTH : 1.0) / 2.0;
            if(cap == LineCapDiamondAnchor){
                dsmall = cos(theta + M_PI_2) * size;
                dbig = sin(theta + M_PI_2) * size;
            }
            else{
                dsmall = cos(theta + M_PI_4) * size;
                dbig = sin(theta + M_PI_4) * size;
            }

            ptf[0].X = x2 - dsmall;
            ptf[1].X = x2 + dbig;

            ptf[0].Y = y2 - dbig;
            ptf[3].Y = y2 + dsmall;

            ptf[1].Y = y2 - dsmall;
            ptf[2].Y = y2 + dbig;

            ptf[3].X = x2 - dbig;
            ptf[2].X = x2 + dsmall;

            transform_and_round_points(graphics, pt, ptf, 4);
            Polygon(graphics->hdc, pt, 4);

            break;
        case LineCapArrowAnchor:
            size = size * 4.0 / sqrt(3.0);

            dx = cos(M_PI / 6.0 + theta) * size;
            dy = sin(M_PI / 6.0 + theta) * size;

            ptf[0].X = x2 - dx;
            ptf[0].Y = y2 - dy;

            dx = cos(- M_PI / 6.0 + theta) * size;
            dy = sin(- M_PI / 6.0 + theta) * size;

            ptf[1].X = x2 - dx;
            ptf[1].Y = y2 - dy;

            ptf[2].X = x2;
            ptf[2].Y = y2;

            transform_and_round_points(graphics, pt, ptf, 3);
            Polygon(graphics->hdc, pt, 3);

            break;
        case LineCapRoundAnchor:
            dx = dy = ANCHOR_WIDTH * size / 2.0;

            ptf[0].X = x2 - dx;
            ptf[0].Y = y2 - dy;
            ptf[1].X = x2 + dx;
            ptf[1].Y = y2 + dy;

            transform_and_round_points(graphics, pt, ptf, 2);
            Ellipse(graphics->hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);

            break;
        case LineCapTriangle:
            size = size / 2.0;
            dx = cos(M_PI_2 + theta) * size;
            dy = sin(M_PI_2 + theta) * size;

            ptf[0].X = x2 - dx;
            ptf[0].Y = y2 - dy;
            ptf[1].X = x2 + dx;
            ptf[1].Y = y2 + dy;

            dx = cos(theta) * size;
            dy = sin(theta) * size;

            ptf[2].X = x2 + dx;
            ptf[2].Y = y2 + dy;

            transform_and_round_points(graphics, pt, ptf, 3);
            Polygon(graphics->hdc, pt, 3);

            break;
        case LineCapRound:
            dx = dy = size / 2.0;

            ptf[0].X = x2 - dx;
            ptf[0].Y = y2 - dy;
            ptf[1].X = x2 + dx;
            ptf[1].Y = y2 + dy;

            dx = -cos(M_PI_2 + theta) * size;
            dy = -sin(M_PI_2 + theta) * size;

            ptf[2].X = x2 - dx;
            ptf[2].Y = y2 - dy;
            ptf[3].X = x2 + dx;
            ptf[3].Y = y2 + dy;

            transform_and_round_points(graphics, pt, ptf, 4);
            Pie(graphics->hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x,
                pt[2].y, pt[3].x, pt[3].y);

            break;
        case LineCapCustom:
            if(!custom)
                break;

            count = custom->pathdata.Count;
            custptf = GdipAlloc(count * sizeof(PointF));
            custpt = GdipAlloc(count * sizeof(IPOINT));
            tp = GdipAlloc(count);

            if(!custptf || !custpt || !tp)
                goto custend;

            memcpy(custptf, custom->pathdata.Points, count * sizeof(PointF));

            GdipSetMatrixElements(&matrix, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
            GdipScaleMatrix(&matrix, size, size, MatrixOrderAppend);
            GdipRotateMatrix(&matrix, (180.0 / M_PI) * (theta - M_PI_2),
                             MatrixOrderAppend);
            GdipTranslateMatrix(&matrix, x2, y2, MatrixOrderAppend);
            GdipTransformMatrixPoints(&matrix, custptf, count);

            transform_and_round_points(graphics, custpt, custptf, count);

            for(i = 0; i < count; i++)
                tp[i] = convert_path_point_type(custom->pathdata.Types[i]);

            if(custom->fill){
                BeginPath(graphics->hdc);
                PolyDraw(graphics->hdc, custpt, tp, count);
                EndPath(graphics->hdc);
                StrokeAndFillPath(graphics->hdc);
            }
            else
                PolyDraw(graphics->hdc, custpt, tp, count);

custend:
            GdipFree(custptf);
            GdipFree(custpt);
            GdipFree(tp);
            break;
        default:
            break;
    }

    if(!customstroke){
        SelectObject(graphics->hdc, oldbrush);
        SelectObject(graphics->hdc, oldpen);
        DeleteObject(brush);
        DeleteObject(pen);
    }
}

/* Shortens the line by the given percent by changing x2, y2.
 * If percent is > 1.0 then the line will change direction.
 * If percent is negative it can lengthen the line. */
static void shorten_line_percent(REAL x1, REAL  y1, REAL *x2, REAL *y2, REAL percent)
{
    REAL dist, theta, dx, dy;

    if((y1 == *y2) && (x1 == *x2))
        return;

    dist = sqrt((*x2 - x1) * (*x2 - x1) + (*y2 - y1) * (*y2 - y1)) * -percent;
    theta = gdiplus_atan2((*y2 - y1), (*x2 - x1));
    dx = cos(theta) * dist;
    dy = sin(theta) * dist;

    *x2 = *x2 + dx;
    *y2 = *y2 + dy;
}

/* Shortens the line by the given amount by changing x2, y2.
 * If the amount is greater than the distance, the line will become length 0.
 * If the amount is negative, it can lengthen the line. */
static void shorten_line_amt(REAL x1, REAL y1, REAL *x2, REAL *y2, REAL amt)
{
    REAL dx, dy, percent;

    dx = *x2 - x1;
    dy = *y2 - y1;
    if(dx == 0 && dy == 0)
        return;

    percent = amt / sqrt(dx * dx + dy * dy);
    if(percent >= 1.0){
        *x2 = x1;
        *y2 = y1;
        return;
    }

    shorten_line_percent(x1, y1, x2, y2, percent);
}

/* Draws lines between the given points, and if caps is true then draws an endcap
 * at the end of the last line. */
static Status draw_polyline(Graphics *graphics, Pen *pen,
    GDIPCONST PointF * pt, INT count, BOOL caps)
{
    IPOINT *pti = NULL;
    PointF *ptcopy = NULL;
    Status status = GenericError;

    if(!count)
        return Ok;

    pti = GdipAlloc(count * sizeof(IPOINT));
    ptcopy = GdipAlloc(count * sizeof(PointF));

    if(!pti || !ptcopy){
        status = OutOfMemory;
        goto end;
    }

    memcpy(ptcopy, pt, count * sizeof(PointF));

    if(caps){
        if(pen->endcap == LineCapArrowAnchor)
            shorten_line_amt(ptcopy[count-2].X, ptcopy[count-2].Y,
                             &ptcopy[count-1].X, &ptcopy[count-1].Y, pen->width);
        else if((pen->endcap == LineCapCustom) && pen->customend)
            shorten_line_amt(ptcopy[count-2].X, ptcopy[count-2].Y,
                             &ptcopy[count-1].X, &ptcopy[count-1].Y,
                             pen->customend->inset * pen->width);

        if(pen->startcap == LineCapArrowAnchor)
            shorten_line_amt(ptcopy[1].X, ptcopy[1].Y,
                             &ptcopy[0].X, &ptcopy[0].Y, pen->width);
        else if((pen->startcap == LineCapCustom) && pen->customstart)
            shorten_line_amt(ptcopy[1].X, ptcopy[1].Y,
                             &ptcopy[0].X, &ptcopy[0].Y,
                             pen->customstart->inset * pen->width);

        draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->endcap, pen->width, pen->customend,
                 pt[count - 2].X, pt[count - 2].Y, pt[count - 1].X, pt[count - 1].Y);
        draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->startcap, pen->width, pen->customstart,
                         pt[1].X, pt[1].Y, pt[0].X, pt[0].Y);
    }

    transform_and_round_points(graphics, pti, ptcopy, count);

    if(Polyline(graphics->hdc, pti, count))
        status = Ok;

end:
    GdipFree(pti);
    GdipFree(ptcopy);

    return status;
}

/* Conducts a linear search to find the bezier points that will back off
 * the endpoint of the curve by a distance of amt. Linear search works
 * better than binary in this case because there are multiple solutions,
 * and binary searches often find a bad one. I don't think this is what
 * Windows does but short of rendering the bezier without GDI's help it's
 * the best we can do. If rev then work from the start of the passed points
 * instead of the end. */
static void shorten_bezier_amt(PointF * pt, REAL amt, BOOL rev)
{
    PointF origpt[4];
    REAL percent = 0.00, dx, dy, origx, origy, diff = -1.0;
    INT i, first = 0, second = 1, third = 2, fourth = 3;

    if(rev){
        first = 3;
        second = 2;
        third = 1;
        fourth = 0;
    }

    origx = pt[fourth].X;
    origy = pt[fourth].Y;
    memcpy(origpt, pt, sizeof(PointF) * 4);

    for(i = 0; (i < MAX_ITERS) && (diff < amt); i++){
        /* reset bezier points to original values */
        memcpy(pt, origpt, sizeof(PointF) * 4);
        /* Perform magic on bezier points. Order is important here.*/
        shorten_line_percent(pt[third].X, pt[third].Y, &pt[fourth].X, &pt[fourth].Y, percent);
        shorten_line_percent(pt[second].X, pt[second].Y, &pt[third].X, &pt[third].Y, percent);
        shorten_line_percent(pt[third].X, pt[third].Y, &pt[fourth].X, &pt[fourth].Y, percent);
        shorten_line_percent(pt[first].X, pt[first].Y, &pt[second].X, &pt[second].Y, percent);
        shorten_line_percent(pt[second].X, pt[second].Y, &pt[third].X, &pt[third].Y, percent);
        shorten_line_percent(pt[third].X, pt[third].Y, &pt[fourth].X, &pt[fourth].Y, percent);

        dx = pt[fourth].X - origx;
        dy = pt[fourth].Y - origy;

        diff = sqrt(dx * dx + dy * dy);
        percent += 0.0005 * amt;
    }
}

/* Draws bezier curves between given points, and if caps is true then draws an
 * endcap at the end of the last line. */
static Status draw_polybezier(Graphics *graphics, Pen *pen,
    GDIPCONST PointF * pt, INT count, BOOL caps)
{
    IPOINT *pti;
    PointF *ptcopy;
    Status status = GenericError;

    if(!count)
        return Ok;

    pti = GdipAlloc(count * sizeof(IPOINT));
    ptcopy = GdipAlloc(count * sizeof(PointF));

    if(!pti || !ptcopy){
        status = OutOfMemory;
        goto end;
    }

    memcpy(ptcopy, pt, count * sizeof(PointF));

    if(caps){
        if(pen->endcap == LineCapArrowAnchor)
            shorten_bezier_amt(&ptcopy[count-4], pen->width, FALSE);
        else if((pen->endcap == LineCapCustom) && pen->customend)
            shorten_bezier_amt(&ptcopy[count-4], pen->width * pen->customend->inset,
                               FALSE);

        if(pen->startcap == LineCapArrowAnchor)
            shorten_bezier_amt(ptcopy, pen->width, TRUE);
        else if((pen->startcap == LineCapCustom) && pen->customstart)
            shorten_bezier_amt(ptcopy, pen->width * pen->customstart->inset, TRUE);

        /* the direction of the line cap is parallel to the direction at the
         * end of the bezier (which, if it has been shortened, is not the same
         * as the direction from pt[count-2] to pt[count-1]) */
        draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->endcap, pen->width, pen->customend,
            pt[count - 1].X - (ptcopy[count - 1].X - ptcopy[count - 2].X),
            pt[count - 1].Y - (ptcopy[count - 1].Y - ptcopy[count - 2].Y),
            pt[count - 1].X, pt[count - 1].Y);

        draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->startcap, pen->width, pen->customstart,
            pt[0].X - (ptcopy[0].X - ptcopy[1].X),
            pt[0].Y - (ptcopy[0].Y - ptcopy[1].Y), pt[0].X, pt[0].Y);
    }

    transform_and_round_points(graphics, pti, ptcopy, count);

    PolyBezier(graphics->hdc, pti, count);

    status = Ok;

end:
    GdipFree(pti);
    GdipFree(ptcopy);

    return status;
}

/* Draws a combination of bezier curves and lines between points. */
static Status draw_poly(Graphics *graphics, Pen *pen, GDIPCONST PointF * pt,
    GDIPCONST BYTE * types, INT count, BOOL caps)
{
    IPOINT *pti = GdipAlloc(count * sizeof(IPOINT));
    BYTE *tp = GdipAlloc(count);
    PointF *ptcopy = GdipAlloc(count * sizeof(PointF));
    INT i, j;
    Status status = GenericError;

    if(!count){
        status = Ok;
        goto end;
    }
    if(!pti || !tp || !ptcopy){
        status = OutOfMemory;
        goto end;
    }

    for(i = 1; i < count; i++){
        if((types[i] & PathPointTypePathTypeMask) == PathPointTypeBezier){
            if((i + 2 >= count) || !(types[i + 1] & PathPointTypeBezier)
                || !(types[i + 1] & PathPointTypeBezier)){
                ERR("Bad bezier points\n");
                goto end;
            }
            i += 2;
        }
    }

    memcpy(ptcopy, pt, count * sizeof(PointF));

    /* If we are drawing caps, go through the points and adjust them accordingly,
     * and draw the caps. */
    if(caps){
        switch(types[count - 1] & PathPointTypePathTypeMask){
            case PathPointTypeBezier:
                if(pen->endcap == LineCapArrowAnchor)
                    shorten_bezier_amt(&ptcopy[count - 4], pen->width, FALSE);
                else if((pen->endcap == LineCapCustom) && pen->customend)
                    shorten_bezier_amt(&ptcopy[count - 4],
                                       pen->width * pen->customend->inset, FALSE);

                draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->endcap, pen->width, pen->customend,
                    pt[count - 1].X - (ptcopy[count - 1].X - ptcopy[count - 2].X),
                    pt[count - 1].Y - (ptcopy[count - 1].Y - ptcopy[count - 2].Y),
                    pt[count - 1].X, pt[count - 1].Y);

                break;
            case PathPointTypeLine:
                if(pen->endcap == LineCapArrowAnchor)
                    shorten_line_amt(ptcopy[count - 2].X, ptcopy[count - 2].Y,
                                     &ptcopy[count - 1].X, &ptcopy[count - 1].Y,
                                     pen->width);
                else if((pen->endcap == LineCapCustom) && pen->customend)
                    shorten_line_amt(ptcopy[count - 2].X, ptcopy[count - 2].Y,
                                     &ptcopy[count - 1].X, &ptcopy[count - 1].Y,
                                     pen->customend->inset * pen->width);

                draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->endcap, pen->width, pen->customend,
                         pt[count - 2].X, pt[count - 2].Y, pt[count - 1].X,
                         pt[count - 1].Y);

                break;
            default:
                ERR("Bad path last point\n");
                goto end;
        }

        /* Find start of points */
        for(j = 1; j < count && ((types[j] & PathPointTypePathTypeMask)
            == PathPointTypeStart); j++);

        switch(types[j] & PathPointTypePathTypeMask){
            case PathPointTypeBezier:
                if(pen->startcap == LineCapArrowAnchor)
                    shorten_bezier_amt(&ptcopy[j - 1], pen->width, TRUE);
                else if((pen->startcap == LineCapCustom) && pen->customstart)
                    shorten_bezier_amt(&ptcopy[j - 1],
                                       pen->width * pen->customstart->inset, TRUE);

                draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->startcap, pen->width, pen->customstart,
                    pt[j - 1].X - (ptcopy[j - 1].X - ptcopy[j].X),
                    pt[j - 1].Y - (ptcopy[j - 1].Y - ptcopy[j].Y),
                    pt[j - 1].X, pt[j - 1].Y);

                break;
            case PathPointTypeLine:
                if(pen->startcap == LineCapArrowAnchor)
                    shorten_line_amt(ptcopy[j].X, ptcopy[j].Y,
                                     &ptcopy[j - 1].X, &ptcopy[j - 1].Y,
                                     pen->width);
                else if((pen->startcap == LineCapCustom) && pen->customstart)
                    shorten_line_amt(ptcopy[j].X, ptcopy[j].Y,
                                     &ptcopy[j - 1].X, &ptcopy[j - 1].Y,
                                     pen->customstart->inset * pen->width);

                draw_cap(graphics, get_gdi_brush_color(pen->brush), pen->startcap, pen->width, pen->customstart,
                         pt[j].X, pt[j].Y, pt[j - 1].X,
                         pt[j - 1].Y);

                break;
            default:
                ERR("Bad path points\n");
                goto end;
        }
    }

    transform_and_round_points(graphics, pti, ptcopy, count);

    for(i = 0; i < count; i++){
        tp[i] = convert_path_point_type(types[i]);
    }

    PolyDraw(graphics->hdc, pti, tp, count);

    status = Ok;

end:
    GdipFree(pti);
    GdipFree(ptcopy);
    GdipFree(tp);

    return status;
}

Status trace_path(Graphics *graphics, Path *path)
{
    Status result;

    BeginPath(graphics->hdc);
    result = draw_poly(graphics, NULL, path->pathdata.Points,
                       path->pathdata.Types, path->pathdata.Count, FALSE);
    EndPath(graphics->hdc);
    return result;
}

typedef struct _GraphicsContainerItem {
    struct list entry;
    GraphicsContainer contid;

    SmoothingMode smoothing;
    CompositingQuality compqual;
    InterpolationMode interpolation;
    CompositingMode compmode;
    TextRenderingHint texthint;
    REAL scale;
    Unit unit;
    PixelOffsetMode pixeloffset;
    UINT textcontrast;
    Matrix worldtrans;
    Region* clip;
    INT origin_x, origin_y;
} GraphicsContainerItem;

static Status init_container(GraphicsContainerItem** container,
        GDIPCONST Graphics* graphics){
    Status sts;

    *container = GdipAlloc(sizeof(GraphicsContainerItem));
    if(!(*container))
        return OutOfMemory;

    (*container)->contid = graphics->contid + 1;

    (*container)->smoothing = graphics->smoothing;
    (*container)->compqual = graphics->compqual;
    (*container)->interpolation = graphics->interpolation;
    (*container)->compmode = graphics->compmode;
    (*container)->texthint = graphics->texthint;
    (*container)->scale = graphics->scale;
    (*container)->unit = graphics->unit;
    (*container)->textcontrast = graphics->textcontrast;
    (*container)->pixeloffset = graphics->pixeloffset;
    (*container)->origin_x = graphics->origin_x;
    (*container)->origin_y = graphics->origin_y;
    (*container)->worldtrans = graphics->worldtrans;

    sts = GdipCloneRegion(graphics->clip, &(*container)->clip);
    if(sts != Ok){
        GdipFree(*container);
        *container = NULL;
        return sts;
    }

    return Ok;
}

static void delete_container(GraphicsContainerItem* container)
{
    GdipDeleteRegion(container->clip);
    GdipFree(container);
}

static Status restore_container(Graphics* graphics,
        GDIPCONST GraphicsContainerItem* container){
    Status sts;
    Region *newClip;

    sts = GdipCloneRegion(container->clip, &newClip);
    if(sts != Ok) return sts;

    graphics->worldtrans = container->worldtrans;

    GdipDeleteRegion(graphics->clip);
    graphics->clip = newClip;

    graphics->contid = container->contid - 1;

    graphics->smoothing = container->smoothing;
    graphics->compqual = container->compqual;
    graphics->interpolation = container->interpolation;
    graphics->compmode = container->compmode;
    graphics->texthint = container->texthint;
    graphics->scale = container->scale;
    graphics->unit = container->unit;
    graphics->textcontrast = container->textcontrast;
    graphics->pixeloffset = container->pixeloffset;
    graphics->origin_x = container->origin_x;
    graphics->origin_y = container->origin_y;

    return Ok;
}

static Status get_graphics_bounds(Graphics* graphics, RectF* rect)
{
    RECT wnd_rect;
    Status stat=Ok;
    Unit unit;

    if(graphics->hwnd) {
        if(!GetClientRect(graphics->hwnd, &wnd_rect))
            return GenericError;

        rect->X = wnd_rect.left;
        rect->Y = wnd_rect.top;
        rect->Width = wnd_rect.right - wnd_rect.left;
        rect->Height = wnd_rect.bottom - wnd_rect.top;
    }else if (graphics->image){
        stat = GdipGetImageBounds(graphics->image, rect, &unit);
        if (stat == Ok && unit != UnitPixel)
            FIXME("need to convert from unit %i\n", unit);
    }else if (GetObjectType(graphics->hdc) == OBJ_MEMDC){
        HBITMAP hbmp;
        BITMAP bmp;

        rect->X = 0;
        rect->Y = 0;

        hbmp = GetCurrentObject(graphics->hdc, OBJ_BITMAP);
        if (hbmp && GetObjectW(hbmp, sizeof(bmp), &bmp))
        {
            rect->Width = bmp.bmWidth;
            rect->Height = bmp.bmHeight;
        }
        else
        {
            /* FIXME: ??? */
            rect->Width = 1;
            rect->Height = 1;
        }
    }else{
        rect->X = 0;
        rect->Y = 0;
        rect->Width = GetDeviceCaps(graphics->hdc, HORZRES);
        rect->Height = GetDeviceCaps(graphics->hdc, VERTRES);
    }

    return stat;
}

/* on success, rgn will contain the region of the graphics object which
 * is visible after clipping has been applied */
static Status get_visible_clip_region(Graphics *graphics, Region *rgn)
{
    Status stat;
    RectF rectf;
    Region* tmp;

    if((stat = get_graphics_bounds(graphics, &rectf)) != Ok)
        return stat;

    if((stat = GdipCreateRegion(&tmp)) != Ok)
        return stat;

    if((stat = GdipCombineRegionRect(tmp, &rectf, CombineModeReplace)) != Ok)
        goto end;

    if((stat = GdipCombineRegionRegion(tmp, graphics->clip, CombineModeIntersect)) != Ok)
        goto end;

    stat = GdipCombineRegionRegion(rgn, tmp, CombineModeReplace);

end:
    GdipDeleteRegion(tmp);
    return stat;
}

void get_log_fontW(const Font *font, Graphics *graphics, LOGFONTW *lf)
{
    REAL height;

    if (font->unit == UnitPixel)
    {
        height = units_to_pixels(font->emSize, graphics->unit, graphics->yres);
    }
    else
    {
        if (graphics->unit == UnitDisplay || graphics->unit == UnitPixel)
            height = units_to_pixels(font->emSize, font->unit, graphics->xres);
        else
            height = units_to_pixels(font->emSize, font->unit, graphics->yres);
    }

    lf->lfHeight = -(height + 0.5);
    lf->lfWidth = 0;
    lf->lfEscapement = 0;
    lf->lfOrientation = 0;
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
}

static void get_font_hfont(Graphics *graphics, GDIPCONST Font *font,
                           GDIPCONST StringFormat *format, HFONT *hfont,
                           GDIPCONST Matrix *matrix)
{
    HDC hdc = CreateCompatibleDC(0);
    PointF pt[3];
    REAL angle, rel_width, rel_height, font_height;
    LOGFONTW lfw;
    HFONT unscaled_font;
    TEXTMETRICW textmet;

    if (font->unit == UnitPixel)
        font_height = font->emSize;
    else
    {
        REAL unit_scale, res;

        res = (graphics->unit == UnitDisplay || graphics->unit == UnitPixel) ? graphics->xres : graphics->yres;
        unit_scale = units_scale(font->unit, graphics->unit, res);

        font_height = font->emSize * unit_scale;
    }

    pt[0].X = 0.0;
    pt[0].Y = 0.0;
    pt[1].X = 1.0;
    pt[1].Y = 0.0;
    pt[2].X = 0.0;
    pt[2].Y = 1.0;
    if (matrix)
    {
        Matrix xform = *matrix;
        GdipTransformMatrixPoints(&xform, pt, 3);
    }
    if (graphics)
        GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, pt, 3);
    angle = -gdiplus_atan2((pt[1].Y - pt[0].Y), (pt[1].X - pt[0].X));
    rel_width = sqrt((pt[1].Y-pt[0].Y)*(pt[1].Y-pt[0].Y)+
                     (pt[1].X-pt[0].X)*(pt[1].X-pt[0].X));
    rel_height = sqrt((pt[2].Y-pt[0].Y)*(pt[2].Y-pt[0].Y)+
                      (pt[2].X-pt[0].X)*(pt[2].X-pt[0].X));

    get_log_fontW(font, graphics, &lfw);
    lfw.lfHeight = gdip_round(font_height * rel_height);
    unscaled_font = CreateFontIndirectW(&lfw);

    SelectObject(hdc, unscaled_font);
    GetTextMetricsW(hdc, &textmet);

    lfw.lfWidth = gdip_round(textmet.tmAveCharWidth * rel_width / rel_height);
    lfw.lfEscapement = lfw.lfOrientation = gdip_round((angle / M_PI) * 1800.0);

    *hfont = CreateFontIndirectW(&lfw);

    DeleteDC(hdc);
    DeleteObject(unscaled_font);
}

Status GdipCreateFromHDC(HDC hdc, Graphics **graphics)
{
    TRACE("(%p, %p)\n", hdc, graphics);

    return GdipCreateFromHDC2(hdc, NULL, graphics);
}

Status GdipCreateFromHDC2(HDC hdc, HANDLE hDevice, Graphics **graphics)
{
    Status retval;
    HBITMAP hbitmap;
    DIBSECTION dib;

    TRACE("(%p, %p, %p)\n", hdc, hDevice, graphics);

    if(hDevice != NULL) {
        FIXME("Don't know how to handle parameter hDevice\n");
        return NotImplemented;
    }

    if(hdc == NULL)
        return OutOfMemory;

    if(graphics == NULL)
        return InvalidParameter;

    *graphics = GdipAlloc(sizeof(Graphics));
    if(!*graphics)  return OutOfMemory;

    GdipSetMatrixElements(&(*graphics)->worldtrans, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

    if((retval = GdipCreateRegion(&(*graphics)->clip)) != Ok){
        GdipFree(*graphics);
        return retval;
    }

    hbitmap = GetCurrentObject(hdc, OBJ_BITMAP);
    if (hbitmap && GetObjectW(hbitmap, sizeof(dib), &dib) == sizeof(dib) &&
        dib.dsBmih.biBitCount == 32 && dib.dsBmih.biCompression == BI_RGB)
    {
        (*graphics)->alpha_hdc = 1;
    }

    (*graphics)->hdc = hdc;
    (*graphics)->hwnd = WindowFromDC(hdc);
    (*graphics)->owndc = FALSE;
    (*graphics)->smoothing = SmoothingModeDefault;
    (*graphics)->compqual = CompositingQualityDefault;
    (*graphics)->interpolation = InterpolationModeBilinear;
    (*graphics)->pixeloffset = PixelOffsetModeDefault;
    (*graphics)->compmode = CompositingModeSourceOver;
    (*graphics)->unit = UnitDisplay;
    (*graphics)->scale = 1.0;
    (*graphics)->xres = GetDeviceCaps(hdc, LOGPIXELSX);
    (*graphics)->yres = GetDeviceCaps(hdc, LOGPIXELSY);
    (*graphics)->busy = FALSE;
    (*graphics)->textcontrast = 4;
    list_init(&(*graphics)->containers);
    (*graphics)->contid = GDIP_GET_NEW_CONTID_FOR(*graphics);

    TRACE("<-- %p\n", *graphics);

    return Ok;
}

Status graphics_from_image(Image *image, Graphics **graphics)
{
    Status retval;

    *graphics = GdipAlloc(sizeof(Graphics));
    if(!*graphics)  return OutOfMemory;

    GdipSetMatrixElements(&(*graphics)->worldtrans, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

    if((retval = GdipCreateRegion(&(*graphics)->clip)) != Ok){
        GdipFree(*graphics);
        return retval;
    }

    (*graphics)->hdc = NULL;
    (*graphics)->hwnd = NULL;
    (*graphics)->owndc = FALSE;
    (*graphics)->image = image;
    (*graphics)->smoothing = SmoothingModeDefault;
    (*graphics)->compqual = CompositingQualityDefault;
    (*graphics)->interpolation = InterpolationModeBilinear;
    (*graphics)->pixeloffset = PixelOffsetModeDefault;
    (*graphics)->compmode = CompositingModeSourceOver;
    (*graphics)->unit = UnitDisplay;
    (*graphics)->scale = 1.0;
    (*graphics)->xres = image->xres;
    (*graphics)->yres = image->yres;
    (*graphics)->busy = FALSE;
    (*graphics)->textcontrast = 4;
    list_init(&(*graphics)->containers);
    (*graphics)->contid = GDIP_GET_NEW_CONTID_FOR(*graphics);

    TRACE("<-- %p\n", *graphics);

    return Ok;
}

Status GdipCreateFromHWND(HWND hwnd, Graphics **graphics)
{
    Status ret;
    HDC hdc;

    TRACE("(%p, %p)\n", hwnd, graphics);

    hdc = GetDC(hwnd);

    if((ret = GdipCreateFromHDC(hdc, graphics)) != Ok)
    {
        ReleaseDC(hwnd, hdc);
        return ret;
    }

    (*graphics)->hwnd = hwnd;
    (*graphics)->owndc = TRUE;

    return Ok;
}

/* FIXME: no icm handling */
Status GdipCreateFromHWNDICM(HWND hwnd, Graphics **graphics)
{
    TRACE("(%p, %p)\n", hwnd, graphics);

    return GdipCreateFromHWND(hwnd, graphics);
}

Status GdipCreateMetafileFromEmf(HENHMETAFILE hemf, BOOL delete,
    Metafile **metafile)
{
    ENHMETAHEADER header;
    MetafileType metafile_type;

    TRACE("(%p,%i,%p)\n", hemf, delete, metafile);

    if(!hemf || !metafile)
        return InvalidParameter;

    if (GetEnhMetaFileHeader(hemf, sizeof(header), &header) == 0)
        return GenericError;

    metafile_type = METAFILE_GetEmfType(hemf);

    if (metafile_type == MetafileTypeInvalid)
        return GenericError;

    *metafile = GdipAlloc(sizeof(Metafile));
    if (!*metafile)
        return OutOfMemory;

    (*metafile)->image.type = ImageTypeMetafile;
    (*metafile)->image.format = ImageFormatEMF;
    (*metafile)->image.frame_count = 1;
    (*metafile)->image.xres = (REAL)header.szlDevice.cx;
    (*metafile)->image.yres = (REAL)header.szlDevice.cy;
    (*metafile)->bounds.X = (REAL)header.rclBounds.left;
    (*metafile)->bounds.Y = (REAL)header.rclBounds.top;
    (*metafile)->bounds.Width = (REAL)(header.rclBounds.right - header.rclBounds.left);
    (*metafile)->bounds.Height = (REAL)(header.rclBounds.bottom - header.rclBounds.top);
    (*metafile)->unit = UnitPixel;
    (*metafile)->metafile_type = metafile_type;
    (*metafile)->hemf = hemf;
    (*metafile)->preserve_hemf = !delete;

    TRACE("<-- %p\n", *metafile);

    return Ok;
}

Status GdipCreateMetafileFromWmf(HMETAFILE hwmf, BOOL delete,
    GDIPCONST WmfPlaceableFileHeader * placeable, Metafile **metafile)
{
    UINT read;
    BYTE *copy;
    HENHMETAFILE hemf;
    Status retval = Ok;

    TRACE("(%p, %d, %p, %p)\n", hwmf, delete, placeable, metafile);

    if(!hwmf || !metafile || !placeable)
        return InvalidParameter;

    *metafile = NULL;
    read = GetMetaFileBitsEx(hwmf, 0, NULL);
    if(!read)
        return GenericError;
    copy = GdipAlloc(read);
    GetMetaFileBitsEx(hwmf, read, copy);

    hemf = SetWinMetaFileBits(read, copy, NULL, NULL);
    GdipFree(copy);

    /* FIXME: We should store and use hwmf instead of converting to hemf */
    retval = GdipCreateMetafileFromEmf(hemf, TRUE, metafile);

    if (retval == Ok)
    {
        (*metafile)->image.xres = (REAL)placeable->Inch;
        (*metafile)->image.yres = (REAL)placeable->Inch;
        (*metafile)->bounds.X = ((REAL)placeable->BoundingBox.Left) / ((REAL)placeable->Inch);
        (*metafile)->bounds.Y = ((REAL)placeable->BoundingBox.Top) / ((REAL)placeable->Inch);
        (*metafile)->bounds.Width = (REAL)(placeable->BoundingBox.Right -
                                           placeable->BoundingBox.Left);
        (*metafile)->bounds.Height = (REAL)(placeable->BoundingBox.Bottom -
                                            placeable->BoundingBox.Top);
        (*metafile)->metafile_type = MetafileTypeWmfPlaceable;
        (*metafile)->image.format = ImageFormatWMF;

        if (delete) DeleteMetaFile(hwmf);
    }
    else
        DeleteEnhMetaFile(hemf);
    return retval;
}

Status GdipCreateMetafileFromWmfFile(GDIPCONST WCHAR *file,
    GDIPCONST WmfPlaceableFileHeader * placeable, Metafile **metafile)
{
    HMETAFILE hmf = GetMetaFileW(file);

    TRACE("(%s, %p, %p)\n", debugstr_w(file), placeable, metafile);

    if(!hmf) return InvalidParameter;

    return GdipCreateMetafileFromWmf(hmf, TRUE, placeable, metafile);
}

Status GdipCreateMetafileFromFile(GDIPCONST WCHAR *file,
    Metafile **metafile)
{
    FIXME("(%p, %p): stub\n", file, metafile);
    return NotImplemented;
}

Status GdipCreateMetafileFromStream(IStream *stream,
    Metafile **metafile)
{
    FIXME("(%p, %p): stub\n", stream, metafile);
    return NotImplemented;
}

Status GdipDeleteGraphics(Graphics *graphics)
{
    GraphicsContainerItem *cont, *next;
    Status stat;
    TRACE("(%p)\n", graphics);

    if(!graphics) return InvalidParameter;
    if(graphics->busy) return ObjectBusy;

    if (graphics->image && graphics->image->type == ImageTypeMetafile)
    {
        stat = METAFILE_GraphicsDeleted((Metafile*)graphics->image);
        if (stat != Ok)
            return stat;
    }

    if(graphics->owndc)
        ReleaseDC(graphics->hwnd, graphics->hdc);

    LIST_FOR_EACH_ENTRY_SAFE(cont, next, &graphics->containers, GraphicsContainerItem, entry){
        list_remove(&cont->entry);
        delete_container(cont);
    }

    GdipDeleteRegion(graphics->clip);
    GdipFree(graphics);

    return Ok;
}

Status GdipDrawArc(Graphics *graphics, Pen *pen, REAL x,
    REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
{
    INT save_state, num_pts;
    PointF points[MAX_ARC_PTS];
    Status retval;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n", graphics, pen, x, y,
          width, height, startAngle, sweepAngle);

    if(!graphics || !pen || width <= 0 || height <= 0)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    num_pts = arc2polybezier(points, x, y, width, height, startAngle, sweepAngle);

    save_state = prepare_dc(graphics, pen);

    retval = draw_polybezier(graphics, pen, points, num_pts, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawArcI(Graphics *graphics, Pen *pen, INT x,
    INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
{
    TRACE("(%p, %p, %d, %d, %d, %d, %.2f, %.2f)\n", graphics, pen, x, y,
          width, height, startAngle, sweepAngle);

    return GdipDrawArc(graphics,pen,(REAL)x,(REAL)y,(REAL)width,(REAL)height,startAngle,sweepAngle);
}

Status GdipDrawBezier(Graphics *graphics, Pen *pen, REAL x1,
    REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4)
{
    INT save_state;
    PointF pt[4];
    Status retval;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n", graphics, pen, x1, y1,
          x2, y2, x3, y3, x4, y4);

    if(!graphics || !pen)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    pt[0].X = x1;
    pt[0].Y = y1;
    pt[1].X = x2;
    pt[1].Y = y2;
    pt[2].X = x3;
    pt[2].Y = y3;
    pt[3].X = x4;
    pt[3].Y = y4;

    save_state = prepare_dc(graphics, pen);

    retval = draw_polybezier(graphics, pen, pt, 4, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawBezierI(Graphics *graphics, Pen *pen, INT x1,
    INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4)
{
    INT save_state;
    PointF pt[4];
    Status retval;

    TRACE("(%p, %p, %d, %d, %d, %d, %d, %d, %d, %d)\n", graphics, pen, x1, y1,
          x2, y2, x3, y3, x4, y4);

    if(!graphics || !pen)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    pt[0].X = x1;
    pt[0].Y = y1;
    pt[1].X = x2;
    pt[1].Y = y2;
    pt[2].X = x3;
    pt[2].Y = y3;
    pt[3].X = x4;
    pt[3].Y = y4;

    save_state = prepare_dc(graphics, pen);

    retval = draw_polybezier(graphics, pen, pt, 4, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawBeziers(Graphics *graphics, Pen *pen,
    GDIPCONST PointF *points, INT count)
{
    INT i;
    Status ret;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    if(!graphics || !pen || !points || (count <= 0))
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    for(i = 0; i < floor(count / 4); i++){
        ret = GdipDrawBezier(graphics, pen,
                             points[4*i].X, points[4*i].Y,
                             points[4*i + 1].X, points[4*i + 1].Y,
                             points[4*i + 2].X, points[4*i + 2].Y,
                             points[4*i + 3].X, points[4*i + 3].Y);
        if(ret != Ok)
            return ret;
    }

    return Ok;
}

Status GdipDrawBeziersI(Graphics *graphics, Pen *pen,
    GDIPCONST Point *points, INT count)
{
    PointF *pts;
    Status ret;
    INT i;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    if(!graphics || !pen || !points || (count <= 0))
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    pts = GdipAlloc(sizeof(PointF) * count);
    if(!pts)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        pts[i].X = (REAL)points[i].X;
        pts[i].Y = (REAL)points[i].Y;
    }

    ret = GdipDrawBeziers(graphics,pen,pts,count);

    GdipFree(pts);

    return ret;
}

Status GdipDrawClosedCurve(Graphics *graphics, Pen *pen,
    GDIPCONST PointF *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    return GdipDrawClosedCurve2(graphics, pen, points, count, 1.0);
}

Status GdipDrawClosedCurveI(Graphics *graphics, Pen *pen,
    GDIPCONST Point *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    return GdipDrawClosedCurve2I(graphics, pen, points, count, 1.0);
}

Status GdipDrawClosedCurve2(Graphics *graphics, Pen *pen,
    GDIPCONST PointF *points, INT count, REAL tension)
{
    Path *path;
    Status stat;

    TRACE("(%p, %p, %p, %d, %.2f)\n", graphics, pen, points, count, tension);

    if(!graphics || !pen || !points || count <= 0)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if((stat = PathCreate(FillModeAlternate, &path)) != Ok)
        return stat;

    stat = PathAddClosedCurve2(path, points, count, tension);
    if(stat != Ok){
        PathDelete(path);
        return stat;
    }

    stat = GdipDrawPath(graphics, pen, path);

    PathDelete(path);

    return stat;
}

Status GdipDrawClosedCurve2I(Graphics *graphics, Pen *pen,
    GDIPCONST Point *points, INT count, REAL tension)
{
    PointF *ptf;
    Status stat;
    INT i;

    TRACE("(%p, %p, %p, %d, %.2f)\n", graphics, pen, points, count, tension);

    if(!points || count <= 0)
        return InvalidParameter;

    ptf = GdipAlloc(sizeof(PointF)*count);
    if(!ptf)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        ptf[i].X = (REAL)points[i].X;
        ptf[i].Y = (REAL)points[i].Y;
    }

    stat = GdipDrawClosedCurve2(graphics, pen, ptf, count, tension);

    GdipFree(ptf);

    return stat;
}

Status GdipDrawCurve(Graphics *graphics, Pen *pen,
    GDIPCONST PointF *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    return GdipDrawCurve2(graphics,pen,points,count,1.0);
}

Status GdipDrawCurveI(Graphics *graphics, Pen *pen,
    GDIPCONST Point *points, INT count)
{
    PointF *pointsF;
    Status ret;
    INT i;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    if(!points)
        return InvalidParameter;

    pointsF = GdipAlloc(sizeof(PointF)*count);
    if(!pointsF)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        pointsF[i].X = (REAL)points[i].X;
        pointsF[i].Y = (REAL)points[i].Y;
    }

    ret = GdipDrawCurve(graphics,pen,pointsF,count);
    GdipFree(pointsF);

    return ret;
}

/* Approximates cardinal spline with Bezier curves. */
Status GdipDrawCurve2(Graphics *graphics, Pen *pen,
    GDIPCONST PointF *points, INT count, REAL tension)
{
    /* PolyBezier expects count*3-2 points. */
    INT i, len_pt = count*3-2, save_state;
    PointF *pt;
    REAL x1, x2, y1, y2;
    Status retval;

    TRACE("(%p, %p, %p, %d, %.2f)\n", graphics, pen, points, count, tension);

    if(!graphics || !pen)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if(count < 2)
        return InvalidParameter;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    pt = GdipAlloc(len_pt * sizeof(PointF));
    if(!pt)
        return OutOfMemory;

    tension = tension * TENSION_CONST;

    calc_curve_bezier_endp(points[0].X, points[0].Y, points[1].X, points[1].Y,
        tension, &x1, &y1);

    pt[0].X = points[0].X;
    pt[0].Y = points[0].Y;
    pt[1].X = x1;
    pt[1].Y = y1;

    for(i = 0; i < count-2; i++){
        calc_curve_bezier(&(points[i]), tension, &x1, &y1, &x2, &y2);

        pt[3*i+2].X = x1;
        pt[3*i+2].Y = y1;
        pt[3*i+3].X = points[i+1].X;
        pt[3*i+3].Y = points[i+1].Y;
        pt[3*i+4].X = x2;
        pt[3*i+4].Y = y2;
    }

    calc_curve_bezier_endp(points[count-1].X, points[count-1].Y,
        points[count-2].X, points[count-2].Y, tension, &x1, &y1);

    pt[len_pt-2].X = x1;
    pt[len_pt-2].Y = y1;
    pt[len_pt-1].X = points[count-1].X;
    pt[len_pt-1].Y = points[count-1].Y;

    save_state = prepare_dc(graphics, pen);

    retval = draw_polybezier(graphics, pen, pt, len_pt, TRUE);

    GdipFree(pt);
    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawCurve2I(Graphics *graphics, Pen *pen,
    GDIPCONST Point *points, INT count, REAL tension)
{
    PointF *pointsF;
    Status ret;
    INT i;

    TRACE("(%p, %p, %p, %d, %.2f)\n", graphics, pen, points, count, tension);

    if(!points)
        return InvalidParameter;

    pointsF = GdipAlloc(sizeof(PointF)*count);
    if(!pointsF)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        pointsF[i].X = (REAL)points[i].X;
        pointsF[i].Y = (REAL)points[i].Y;
    }

    ret = GdipDrawCurve2(graphics,pen,pointsF,count,tension);
    GdipFree(pointsF);

    return ret;
}

Status GdipDrawCurve3(Graphics *graphics, Pen *pen,
    GDIPCONST PointF *points, INT count, INT offset, INT numberOfSegments,
    REAL tension)
{
    TRACE("(%p, %p, %p, %d, %d, %d, %.2f)\n", graphics, pen, points, count, offset, numberOfSegments, tension);

    if(offset >= count || numberOfSegments > count - offset - 1 || numberOfSegments <= 0){
        return InvalidParameter;
    }

    return GdipDrawCurve2(graphics, pen, points + offset, numberOfSegments + 1, tension);
}

Status GdipDrawCurve3I(Graphics *graphics, Pen *pen,
    GDIPCONST Point *points, INT count, INT offset, INT numberOfSegments,
    REAL tension)
{
    TRACE("(%p, %p, %p, %d, %d, %d, %.2f)\n", graphics, pen, points, count, offset, numberOfSegments, tension);

    if(count < 0){
        return OutOfMemory;
    }

    if(offset >= count || numberOfSegments > count - offset - 1 || numberOfSegments <= 0){
        return InvalidParameter;
    }

    return GdipDrawCurve2I(graphics, pen, points + offset, numberOfSegments + 1, tension);
}

Status GdipDrawEllipse(Graphics *graphics, Pen *pen, REAL x,
    REAL y, REAL width, REAL height)
{
    INT save_state;
    PointF ptf[2];
    IPOINT pti[2];

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f)\n", graphics, pen, x, y, width, height);

    if(!graphics || !pen)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    ptf[0].X = x;
    ptf[0].Y = y;
    ptf[1].X = x + width;
    ptf[1].Y = y + height;

    save_state = prepare_dc(graphics, pen);
    SelectObject(graphics->hdc, GetStockObject(NULL_BRUSH));

    transform_and_round_points(graphics, pti, ptf, 2);

    Ellipse(graphics->hdc, pti[0].x, pti[0].y, pti[1].x, pti[1].y);

    restore_dc(graphics, save_state);

    return Ok;
}

Status GdipDrawEllipseI(Graphics *graphics, Pen *pen, INT x,
    INT y, INT width, INT height)
{
    TRACE("(%p, %p, %d, %d, %d, %d)\n", graphics, pen, x, y, width, height);

    return GdipDrawEllipse(graphics,pen,(REAL)x,(REAL)y,(REAL)width,(REAL)height);
}


Status GdipDrawImage(Graphics *graphics, Image *image, REAL x, REAL y)
{
    UINT width, height;

    TRACE("(%p, %p, %.2f, %.2f)\n", graphics, image, x, y);

    if(!graphics || !image)
        return InvalidParameter;

    GdipGetImageWidth(image, &width);
    GdipGetImageHeight(image, &height);

    return GdipDrawImagePointRect(graphics, image, x, y,
                                  0.0, 0.0, (REAL)width, (REAL)height, UnitPixel);
}

Status GdipDrawImageI(Graphics *graphics, Image *image, INT x,
    INT y)
{
    TRACE("(%p, %p, %d, %d)\n", graphics, image, x, y);

    return GdipDrawImage(graphics, image, (REAL)x, (REAL)y);
}

Status GdipDrawImagePointRect(Graphics *graphics, Image *image,
    REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight,
    Unit srcUnit)
{
    PointF points[3];
    REAL scale_x, scale_y, width, height;

    TRACE("(%p, %p, %f, %f, %f, %f, %f, %f, %d)\n", graphics, image, x, y, srcx, srcy, srcwidth, srcheight, srcUnit);

    scale_x = units_scale(srcUnit, graphics->unit, graphics->xres);
    scale_x *= graphics->xres / image->xres;
    scale_y = units_scale(srcUnit, graphics->unit, graphics->yres);
    scale_y *= graphics->yres / image->yres;
    width = srcwidth * scale_x;
    height = srcheight * scale_y;

    points[0].X = points[2].X = x;
    points[0].Y = points[1].Y = y;
    points[1].X = x + width;
    points[2].Y = y + height;

    return GdipDrawImagePointsRect(graphics, image, points, 3, srcx, srcy,
        srcwidth, srcheight, srcUnit, NULL, NULL, NULL);
}

Status GdipDrawImagePointRectI(Graphics *graphics, Image *image,
    INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight,
    Unit srcUnit)
{
    return GdipDrawImagePointRect(graphics, image, x, y, srcx, srcy, srcwidth, srcheight, srcUnit);
}

Status GdipDrawImagePoints(Graphics *graphics, Image *image,
    GDIPCONST PointF *dstpoints, INT count)
{
    UINT width, height;

    TRACE("(%p, %p, %p, %d)\n", graphics, image, dstpoints, count);

    if(!image)
        return InvalidParameter;

    GdipGetImageWidth(image, &width);
    GdipGetImageHeight(image, &height);

    return GdipDrawImagePointsRect(graphics, image, dstpoints, count, 0, 0,
        width, height, UnitPixel, NULL, NULL, NULL);
}

Status GdipDrawImagePointsI(Graphics *graphics, Image *image,
    GDIPCONST Point *dstpoints, INT count)
{
    PointF ptf[3];

    TRACE("(%p, %p, %p, %d)\n", graphics, image, dstpoints, count);

    if (count != 3 || !dstpoints)
        return InvalidParameter;

    ptf[0].X = (REAL)dstpoints[0].X;
    ptf[0].Y = (REAL)dstpoints[0].Y;
    ptf[1].X = (REAL)dstpoints[1].X;
    ptf[1].Y = (REAL)dstpoints[1].Y;
    ptf[2].X = (REAL)dstpoints[2].X;
    ptf[2].Y = (REAL)dstpoints[2].Y;

    return GdipDrawImagePoints(graphics, image, ptf, count);
}

static BOOL CALLBACK play_metafile_proc(EmfPlusRecordType record_type, unsigned int flags,
    unsigned int dataSize, const unsigned char *pStr, void *userdata)
{
    GdipPlayMetafileRecord(userdata, record_type, flags, dataSize, pStr);
    return TRUE;
}

Status GdipDrawImagePointsRect(Graphics *graphics, Image *image,
     GDIPCONST PointF *points, INT count, REAL srcx, REAL srcy, REAL srcwidth,
     REAL srcheight, Unit srcUnit, GDIPCONST ImageAttributes* imageAttributes,
     DrawImageAbort callback, VOID * callbackData)
{
    PointF ptf[4];
    IPOINT pti[4];
    Status stat;

    TRACE("(%p, %p, %p, %d, %f, %f, %f, %f, %d, %p, %p, %p)\n", graphics, image, points,
          count, srcx, srcy, srcwidth, srcheight, srcUnit, imageAttributes, callback,
          callbackData);

    if (count > 3)
        return NotImplemented;

    if(!graphics || !image || !points || count != 3)
         return InvalidParameter;

    TRACE("%s %s %s\n", debugstr_pointf(&points[0]), debugstr_pointf(&points[1]),
        debugstr_pointf(&points[2]));

    memcpy(ptf, points, 3 * sizeof(PointF));
    ptf[3].X = ptf[2].X + ptf[1].X - ptf[0].X;
    ptf[3].Y = ptf[2].Y + ptf[1].Y - ptf[0].Y;
    if (!srcwidth || !srcheight || ptf[3].X == ptf[0].X || ptf[3].Y == ptf[0].Y)
        return Ok;
    transform_and_round_points(graphics, pti, ptf, 4);

    TRACE("%s %s %s %s\n", wine_dbgstr_point(&pti[0]), wine_dbgstr_point(&pti[1]),
        wine_dbgstr_point(&pti[2]), wine_dbgstr_point(&pti[3]));

    srcx = units_to_pixels(srcx, srcUnit, image->xres);
    srcy = units_to_pixels(srcy, srcUnit, image->yres);
    srcwidth = units_to_pixels(srcwidth, srcUnit, image->xres);
    srcheight = units_to_pixels(srcheight, srcUnit, image->yres);
    TRACE("src pixels: %f,%f %fx%f\n", srcx, srcy, srcwidth, srcheight);

    if (image->picture)
    {
        if (!graphics->hdc)
        {
            FIXME("graphics object has no HDC\n");
        }

        if(IPicture_Render(image->picture, graphics->hdc,
            pti[0].x, pti[0].y, pti[1].x - pti[0].x, pti[2].y - pti[0].y,
            srcx, srcy, srcwidth, srcheight, NULL) != S_OK)
        {
            if(callback)
                callback(callbackData);
            return GenericError;
        }
    }
    else if (image->type == ImageTypeBitmap)
    {
        Bitmap* bitmap = (Bitmap*)image;
        int use_software=0;

        TRACE("graphics: %.2fx%.2f dpi, fmt %#x, scale %f, image: %.2fx%.2f dpi, fmt %#x, color %08x\n",
            graphics->xres, graphics->yres,
            graphics->image && graphics->image->type == ImageTypeBitmap ? ((Bitmap *)graphics->image)->format : 0,
            graphics->scale, image->xres, image->yres, bitmap->pixfmt,
            imageAttributes ? imageAttributes->outside_color : 0);

        if (imageAttributes || graphics->alpha_hdc ||
            (graphics->image && graphics->image->type == ImageTypeBitmap) ||
            ptf[1].Y != ptf[0].Y || ptf[2].X != ptf[0].X ||
            ptf[1].X - ptf[0].X != srcwidth || ptf[2].Y - ptf[0].Y != srcheight ||
            srcx < 0 || srcy < 0 ||
            srcx + srcwidth > bitmap->width || srcy + srcheight > bitmap->height)
            use_software = 1;

        if (use_software)
        {
            RECT dst_area;
            Rect src_area;
            int i, x, y, src_stride, dst_stride;
            Matrix dst_to_src;
            REAL m11, m12, m21, m22, mdx, mdy;
            LPBYTE src_data, dst_data;
            BitmapData lockeddata;
            InterpolationMode interpolation = graphics->interpolation;
            PixelOffsetMode offset_mode = graphics->pixeloffset;
            PointF dst_to_src_points[3] = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
            REAL x_dx, x_dy, y_dx, y_dy;
            static const ImageAttributes defaultImageAttributes = {WrapModeClamp, 0, FALSE};

            if (!imageAttributes)
                imageAttributes = &defaultImageAttributes;

            dst_area.left = dst_area.right = pti[0].x;
            dst_area.top = dst_area.bottom = pti[0].y;
            for (i=1; i<4; i++)
            {
                if (dst_area.left > pti[i].x) dst_area.left = pti[i].x;
                if (dst_area.right < pti[i].x) dst_area.right = pti[i].x;
                if (dst_area.top > pti[i].y) dst_area.top = pti[i].y;
                if (dst_area.bottom < pti[i].y) dst_area.bottom = pti[i].y;
            }

            TRACE("dst_area: %s\n", wine_dbgstr_rect(&dst_area));

            m11 = (ptf[1].X - ptf[0].X) / srcwidth;
            m21 = (ptf[2].X - ptf[0].X) / srcheight;
            mdx = ptf[0].X - m11 * srcx - m21 * srcy;
            m12 = (ptf[1].Y - ptf[0].Y) / srcwidth;
            m22 = (ptf[2].Y - ptf[0].Y) / srcheight;
            mdy = ptf[0].Y - m12 * srcx - m22 * srcy;

            GdipSetMatrixElements(&dst_to_src, m11, m12, m21, m22, mdx, mdy);

            stat = GdipInvertMatrix(&dst_to_src);
            if (stat != Ok) return stat;

            dst_data = GdipAlloc(sizeof(ARGB) * (dst_area.right - dst_area.left) * (dst_area.bottom - dst_area.top));
            if (!dst_data) return OutOfMemory;

            dst_stride = sizeof(ARGB) * (dst_area.right - dst_area.left);

            get_bitmap_sample_size(interpolation, imageAttributes->wrap,
                bitmap, srcx, srcy, srcwidth, srcheight, &src_area);

            TRACE("src_area: %d x %d\n", src_area.Width, src_area.Height);

            src_data = GdipAlloc(sizeof(ARGB) * src_area.Width * src_area.Height);
            if (!src_data)
            {
                GdipFree(dst_data);
                return OutOfMemory;
            }
            src_stride = sizeof(ARGB) * src_area.Width;

            /* Read the bits we need from the source bitmap into an ARGB buffer. */
            lockeddata.Width = src_area.Width;
            lockeddata.Height = src_area.Height;
            lockeddata.Stride = src_stride;
            lockeddata.PixelFormat = PixelFormat32bppARGB;
            lockeddata.Scan0 = src_data;

            stat = GdipBitmapLockBits(bitmap, &src_area, ImageLockModeRead|ImageLockModeUserInputBuf,
                PixelFormat32bppARGB, &lockeddata);

            if (stat == Ok)
                stat = GdipBitmapUnlockBits(bitmap, &lockeddata);

            if (stat != Ok)
            {
                if (src_data != dst_data)
                    GdipFree(src_data);
                GdipFree(dst_data);
                return stat;
            }

            apply_image_attributes(imageAttributes, src_data,
                src_area.Width, src_area.Height,
                src_stride, ColorAdjustTypeBitmap);

            /* Transform the bits as needed to the destination. */
            GdipTransformMatrixPoints(&dst_to_src, dst_to_src_points, 3);

            x_dx = dst_to_src_points[1].X - dst_to_src_points[0].X;
            x_dy = dst_to_src_points[1].Y - dst_to_src_points[0].Y;
            y_dx = dst_to_src_points[2].X - dst_to_src_points[0].X;
            y_dy = dst_to_src_points[2].Y - dst_to_src_points[0].Y;

            for (x=dst_area.left; x<dst_area.right; x++)
            {
                for (y=dst_area.top; y<dst_area.bottom; y++)
                {
                    PointF src_pointf;
                    ARGB *dst_color;

                    src_pointf.X = dst_to_src_points[0].X + x * x_dx + y * y_dx;
                    src_pointf.Y = dst_to_src_points[0].Y + x * x_dy + y * y_dy;

                    dst_color = (ARGB*)(dst_data + dst_stride * (y - dst_area.top) + sizeof(ARGB) * (x - dst_area.left));

                    if (src_pointf.X >= srcx && src_pointf.X < srcx + srcwidth && src_pointf.Y >= srcy && src_pointf.Y < srcy+srcheight)
                        *dst_color = resample_bitmap_pixel(&src_area, src_data, bitmap->width, bitmap->height, &src_pointf,
                                                           imageAttributes, interpolation, offset_mode);
                    else
                        *dst_color = 0;
                }
            }

            GdipFree(src_data);

            stat = alpha_blend_pixels(graphics, dst_area.left, dst_area.top,
                dst_data, dst_area.right - dst_area.left, dst_area.bottom - dst_area.top, dst_stride);

            GdipFree(dst_data);

            return stat;
        }
        else
        {
            HDC hdc;
            int temp_hdc=0, temp_bitmap=0;
            HBITMAP hbitmap, old_hbm=NULL;

            if (!(bitmap->pixfmt == PixelFormat16bppRGB555 ||
                  bitmap->pixfmt == PixelFormat24bppRGB ||
                  bitmap->pixfmt == PixelFormat32bppRGB ||
                  bitmap->pixfmt == PixelFormat32bppPARGB))
            {
                BITMAPINFOHEADER bih;
                BYTE *temp_bits;
                PixelFormat dst_format;

                /* we can't draw a bitmap of this format directly */
                hdc = CreateCompatibleDC(0);
                temp_hdc = 1;
                temp_bitmap = 1;

                bih.biSize = sizeof(BITMAPINFOHEADER);
                bih.biWidth = bitmap->width;
                bih.biHeight = -bitmap->height;
                bih.biPlanes = 1;
                bih.biBitCount = 32;
                bih.biCompression = BI_RGB;
                bih.biSizeImage = 0;
                bih.biXPelsPerMeter = 0;
                bih.biYPelsPerMeter = 0;
                bih.biClrUsed = 0;
                bih.biClrImportant = 0;

                hbitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_RGB_COLORS,
                    (void**)&temp_bits, NULL, 0);

                if (bitmap->pixfmt & (PixelFormatAlpha|PixelFormatPAlpha))
                    dst_format = PixelFormat32bppPARGB;
                else
                    dst_format = PixelFormat32bppRGB;

                convert_pixels(bitmap->width, bitmap->height,
                    bitmap->width*4, temp_bits, dst_format,
                    bitmap->stride, bitmap->p_bit_data, bitmap->pixfmt,
                    bitmap->image.palette);
            }
            else
            {
                if (bitmap->hbitmap)
                    hbitmap = bitmap->hbitmap;
                else
                {
                    GdipCreateHBITMAPFromBitmap(bitmap, &hbitmap, 0);
                    temp_bitmap = 1;
                }

                hdc = bitmap->hdc;
                temp_hdc = (hdc == 0);
            }

            if (temp_hdc)
            {
                if (!hdc) hdc = CreateCompatibleDC(0);
                old_hbm = SelectObject(hdc, hbitmap);
            }

            if (bitmap->pixfmt & (PixelFormatAlpha|PixelFormatPAlpha))
            {
                gdi_alpha_blend(graphics, pti[0].x, pti[0].y, pti[1].x - pti[0].x, pti[2].y - pti[0].y,
                                hdc, srcx, srcy, srcwidth, srcheight);
            }
            else
            {
                StretchBlt(graphics->hdc, pti[0].x, pti[0].y, pti[1].x-pti[0].x, pti[2].y-pti[0].y,
                    hdc, srcx, srcy, srcwidth, srcheight, SRCCOPY);
            }

            if (temp_hdc)
            {
                SelectObject(hdc, old_hbm);
                DeleteDC(hdc);
            }

            if (temp_bitmap)
                DeleteObject(hbitmap);
        }
    }
    else if (image->type == ImageTypeMetafile && ((Metafile*)image)->hemf)
    {
        RectF rc;

        rc.X = srcx;
        rc.Y = srcy;
        rc.Width = srcwidth;
        rc.Height = srcheight;

        return GdipEnumerateMetafileSrcRectDestPoints(graphics, (Metafile*)image,
            points, count, &rc, srcUnit, play_metafile_proc, image, imageAttributes);
    }
    else
    {
        WARN("Image with nothing we can draw (metafile in wrong state?)\n");
        return InvalidParameter;
    }

    return Ok;
}

Status GdipDrawImagePointsRectI(Graphics *graphics, Image *image,
     GDIPCONST Point *points, INT count, INT srcx, INT srcy, INT srcwidth,
     INT srcheight, Unit srcUnit, GDIPCONST ImageAttributes* imageAttributes,
     DrawImageAbort callback, VOID * callbackData)
{
    PointF pointsF[3];
    INT i;

    TRACE("(%p, %p, %p, %d, %d, %d, %d, %d, %d, %p, %p, %p)\n", graphics, image, points, count,
          srcx, srcy, srcwidth, srcheight, srcUnit, imageAttributes, callback,
          callbackData);

    if(!points || count!=3)
        return InvalidParameter;

    for(i = 0; i < count; i++){
        pointsF[i].X = (REAL)points[i].X;
        pointsF[i].Y = (REAL)points[i].Y;
    }

    return GdipDrawImagePointsRect(graphics, image, pointsF, count, (REAL)srcx, (REAL)srcy,
                                   (REAL)srcwidth, (REAL)srcheight, srcUnit, imageAttributes,
                                   callback, callbackData);
}

Status GdipDrawImageRectRect(Graphics *graphics, Image *image,
    REAL dstx, REAL dsty, REAL dstwidth, REAL dstheight, REAL srcx, REAL srcy,
    REAL srcwidth, REAL srcheight, Unit srcUnit,
    GDIPCONST ImageAttributes* imageattr, DrawImageAbort callback,
    VOID * callbackData)
{
    PointF points[3];

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %d, %p, %p, %p)\n",
          graphics, image, dstx, dsty, dstwidth, dstheight, srcx, srcy,
          srcwidth, srcheight, srcUnit, imageattr, callback, callbackData);

    points[0].X = dstx;
    points[0].Y = dsty;
    points[1].X = dstx + dstwidth;
    points[1].Y = dsty;
    points[2].X = dstx;
    points[2].Y = dsty + dstheight;

    return GdipDrawImagePointsRect(graphics, image, points, 3, srcx, srcy,
               srcwidth, srcheight, srcUnit, imageattr, callback, callbackData);
}

Status GdipDrawImageRectRectI(Graphics *graphics, Image *image,
	INT dstx, INT dsty, INT dstwidth, INT dstheight, INT srcx, INT srcy,
	INT srcwidth, INT srcheight, Unit srcUnit,
	GDIPCONST ImageAttributes* imageAttributes, DrawImageAbort callback,
	VOID * callbackData)
{
    PointF points[3];

    TRACE("(%p, %p, %d, %d, %d, %d, %d, %d, %d, %d, %d, %p, %p, %p)\n",
          graphics, image, dstx, dsty, dstwidth, dstheight, srcx, srcy,
          srcwidth, srcheight, srcUnit, imageAttributes, callback, callbackData);

    points[0].X = dstx;
    points[0].Y = dsty;
    points[1].X = dstx + dstwidth;
    points[1].Y = dsty;
    points[2].X = dstx;
    points[2].Y = dsty + dstheight;

    return GdipDrawImagePointsRect(graphics, image, points, 3, srcx, srcy,
               srcwidth, srcheight, srcUnit, imageAttributes, callback, callbackData);
}

Status GdipDrawImageRect(Graphics *graphics, Image *image,
    REAL x, REAL y, REAL width, REAL height)
{
    RectF bounds;
    Unit unit;
    Status ret;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f)\n", graphics, image, x, y, width, height);

    if(!graphics || !image)
        return InvalidParameter;

    ret = GdipGetImageBounds(image, &bounds, &unit);
    if(ret != Ok)
        return ret;

    return GdipDrawImageRectRect(graphics, image, x, y, width, height,
                                 bounds.X, bounds.Y, bounds.Width, bounds.Height,
                                 unit, NULL, NULL, NULL);
}

Status GdipDrawImageRectI(Graphics *graphics, Image *image,
    INT x, INT y, INT width, INT height)
{
    TRACE("(%p, %p, %d, %d, %d, %d)\n", graphics, image, x, y, width, height);

    return GdipDrawImageRect(graphics, image, (REAL)x, (REAL)y, (REAL)width, (REAL)height);
}

Status GdipDrawLine(Graphics *graphics, Pen *pen, REAL x1,
    REAL y1, REAL x2, REAL y2)
{
    INT save_state;
    PointF pt[2];
    Status retval;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f)\n", graphics, pen, x1, y1, x2, y2);

    if(!pen || !graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    pt[0].X = x1;
    pt[0].Y = y1;
    pt[1].X = x2;
    pt[1].Y = y2;

    save_state = prepare_dc(graphics, pen);

    retval = draw_polyline(graphics, pen, pt, 2, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawLineI(Graphics *graphics, Pen *pen, INT x1,
    INT y1, INT x2, INT y2)
{
    INT save_state;
    PointF pt[2];
    Status retval;

    TRACE("(%p, %p, %d, %d, %d, %d)\n", graphics, pen, x1, y1, x2, y2);

    if(!pen || !graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    pt[0].X = (REAL)x1;
    pt[0].Y = (REAL)y1;
    pt[1].X = (REAL)x2;
    pt[1].Y = (REAL)y2;

    save_state = prepare_dc(graphics, pen);

    retval = draw_polyline(graphics, pen, pt, 2, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawLines(Graphics *graphics, Pen *pen, GDIPCONST
    PointF *points, INT count)
{
    INT save_state;
    Status retval;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    if(!pen || !graphics || (count < 2))
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    save_state = prepare_dc(graphics, pen);

    retval = draw_polyline(graphics, pen, points, count, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawLinesI(Graphics *graphics, Pen *pen, GDIPCONST
    Point *points, INT count)
{
    INT save_state;
    Status retval;
    PointF *ptf = NULL;
    int i;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    if(!pen || !graphics || (count < 2))
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    ptf = GdipAlloc(count * sizeof(PointF));
    if(!ptf) return OutOfMemory;

    for(i = 0; i < count; i ++){
        ptf[i].X = (REAL) points[i].X;
        ptf[i].Y = (REAL) points[i].Y;
    }

    save_state = prepare_dc(graphics, pen);

    retval = draw_polyline(graphics, pen, ptf, count, TRUE);

    restore_dc(graphics, save_state);

    GdipFree(ptf);
    return retval;
}

Status GdipDrawPath(Graphics *graphics, Pen *pen, Path *path)
{
    INT save_state;
    Status retval;

    TRACE("(%p, %p, %p)\n", graphics, pen, path);

    if(!pen || !graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    save_state = prepare_dc(graphics, pen);

    retval = draw_poly(graphics, pen, path->pathdata.Points,
                       path->pathdata.Types, path->pathdata.Count, TRUE);

    restore_dc(graphics, save_state);

    return retval;
}

Status GdipDrawPie(Graphics *graphics, Pen *pen, REAL x,
    REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
{
    INT save_state;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n", graphics, pen, x, y,
            width, height, startAngle, sweepAngle);

    if(!graphics || !pen)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    save_state = prepare_dc(graphics, pen);
    SelectObject(graphics->hdc, GetStockObject(NULL_BRUSH));

    draw_pie(graphics, x, y, width, height, startAngle, sweepAngle);

    restore_dc(graphics, save_state);

    return Ok;
}

Status GdipDrawPieI(Graphics *graphics, Pen *pen, INT x,
    INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
{
    TRACE("(%p, %p, %d, %d, %d, %d, %.2f, %.2f)\n", graphics, pen, x, y,
            width, height, startAngle, sweepAngle);

    return GdipDrawPie(graphics,pen,(REAL)x,(REAL)y,(REAL)width,(REAL)height,startAngle,sweepAngle);
}

Status GdipDrawRectangle(Graphics *graphics, Pen *pen, REAL x,
    REAL y, REAL width, REAL height)
{
    INT save_state;
    PointF ptf[4];
    IPOINT pti[4];

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f)\n", graphics, pen, x, y, width, height);

    if(!pen || !graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    ptf[0].X = x;
    ptf[0].Y = y;
    ptf[1].X = x + width;
    ptf[1].Y = y;
    ptf[2].X = x + width;
    ptf[2].Y = y + height;
    ptf[3].X = x;
    ptf[3].Y = y + height;

    save_state = prepare_dc(graphics, pen);
    SelectObject(graphics->hdc, GetStockObject(NULL_BRUSH));

    transform_and_round_points(graphics, pti, ptf, 4);
    Polygon(graphics->hdc, pti, 4);

    restore_dc(graphics, save_state);

    return Ok;
}

Status GdipDrawRectangleI(Graphics *graphics, Pen *pen, INT x,
    INT y, INT width, INT height)
{
    TRACE("(%p, %p, %d, %d, %d, %d)\n", graphics, pen, x, y, width, height);

    return GdipDrawRectangle(graphics,pen,(REAL)x,(REAL)y,(REAL)width,(REAL)height);
}

Status GdipDrawRectangles(Graphics *graphics, Pen *pen,
    GDIPCONST RectF* rects, INT count)
{
    PointF *ptf;
    IPOINT *pti;
    INT save_state, i;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, rects, count);

    if(!graphics || !pen || !rects || count < 1)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    ptf = GdipAlloc(4 * count * sizeof(PointF));
    pti = GdipAlloc(4 * count * sizeof(IPOINT));

    if(!ptf || !pti){
        GdipFree(ptf);
        GdipFree(pti);
        return OutOfMemory;
    }

    for(i = 0; i < count; i++){
        ptf[4 * i + 3].X = ptf[4 * i].X = rects[i].X;
        ptf[4 * i + 1].Y = ptf[4 * i].Y = rects[i].Y;
        ptf[4 * i + 2].X = ptf[4 * i + 1].X = rects[i].X + rects[i].Width;
        ptf[4 * i + 3].Y = ptf[4 * i + 2].Y = rects[i].Y + rects[i].Height;
    }

    save_state = prepare_dc(graphics, pen);
    SelectObject(graphics->hdc, GetStockObject(NULL_BRUSH));

    transform_and_round_points(graphics, pti, ptf, 4 * count);

    for(i = 0; i < count; i++)
        Polygon(graphics->hdc, &pti[4 * i], 4);

    restore_dc(graphics, save_state);

    GdipFree(ptf);
    GdipFree(pti);

    return Ok;
}

Status GdipDrawRectanglesI(Graphics *graphics, Pen *pen,
    GDIPCONST Rect* rects, INT count)
{
    RectF *rectsF;
    Status ret;
    INT i;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, rects, count);

    if(!rects || count<=0)
        return InvalidParameter;

    rectsF = GdipAlloc(sizeof(RectF) * count);
    if(!rectsF)
        return OutOfMemory;

    for(i = 0;i < count;i++){
        rectsF[i].X      = (REAL)rects[i].X;
        rectsF[i].Y      = (REAL)rects[i].Y;
        rectsF[i].Width  = (REAL)rects[i].Width;
        rectsF[i].Height = (REAL)rects[i].Height;
    }

    ret = GdipDrawRectangles(graphics, pen, rectsF, count);
    GdipFree(rectsF);

    return ret;
}

Status GdipFillClosedCurve2(Graphics *graphics, Brush *brush,
    GDIPCONST PointF *points, INT count, REAL tension, GpFillMode fill)
{
    Path *path;
    Status stat;

    TRACE("(%p, %p, %p, %d, %.2f, %d)\n", graphics, brush, points,
            count, tension, fill);

    if(!graphics || !brush || !points)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if(count == 1)    /* Do nothing */
        return Ok;

    stat = PathCreate(fill, &path);
    if(stat != Ok)
        return stat;

    stat = PathAddClosedCurve2(path, points, count, tension);
    if(stat != Ok){
        PathDelete(path);
        return stat;
    }

    stat = GdipFillPath(graphics, brush, path);
    if(stat != Ok){
        PathDelete(path);
        return stat;
    }

    PathDelete(path);

    return Ok;
}

Status GdipFillClosedCurve2I(Graphics *graphics, Brush *brush,
    GDIPCONST Point *points, INT count, REAL tension, GpFillMode fill)
{
    PointF *ptf;
    Status stat;
    INT i;

    TRACE("(%p, %p, %p, %d, %.2f, %d)\n", graphics, brush, points,
            count, tension, fill);

    if(!points || count == 0)
        return InvalidParameter;

    if(count == 1)    /* Do nothing */
        return Ok;

    ptf = GdipAlloc(sizeof(PointF)*count);
    if(!ptf)
        return OutOfMemory;

    for(i = 0;i < count;i++){
        ptf[i].X = (REAL)points[i].X;
        ptf[i].Y = (REAL)points[i].Y;
    }

    stat = GdipFillClosedCurve2(graphics, brush, ptf, count, tension, fill);

    GdipFree(ptf);

    return stat;
}

Status GdipFillClosedCurve(Graphics *graphics, Brush *brush,
    GDIPCONST PointF *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, brush, points, count);
    return GdipFillClosedCurve2(graphics, brush, points, count,
               0.5f, FillModeAlternate);
}

Status GdipFillClosedCurveI(Graphics *graphics, Brush *brush,
    GDIPCONST Point *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, brush, points, count);
    return GdipFillClosedCurve2I(graphics, brush, points, count,
               0.5f, FillModeAlternate);
}

Status GdipFillEllipse(Graphics *graphics, Brush *brush, REAL x,
    REAL y, REAL width, REAL height)
{
    Status stat;
    Path *path;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f)\n", graphics, brush, x, y, width, height);

    if(!graphics || !brush)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    stat = PathCreate(FillModeAlternate, &path);

    if (stat == Ok)
    {
        stat = PathAddEllipse(path, x, y, width, height);

        if (stat == Ok)
            stat = GdipFillPath(graphics, brush, path);

        PathDelete(path);
    }

    return stat;
}

Status GdipFillEllipseI(Graphics *graphics, Brush *brush, INT x,
    INT y, INT width, INT height)
{
    TRACE("(%p, %p, %d, %d, %d, %d)\n", graphics, brush, x, y, width, height);

    return GdipFillEllipse(graphics,brush,(REAL)x,(REAL)y,(REAL)width,(REAL)height);
}

static Status GDI32_GdipFillPath(Graphics *graphics, Brush *brush, Path *path)
{
    INT save_state;
    Status retval;

    if(!graphics->hdc || !brush_can_fill_path(brush))
        return NotImplemented;

    save_state = SaveDC(graphics->hdc);
    EndPath(graphics->hdc);
    SetPolyFillMode(graphics->hdc, (path->fill == FillModeAlternate ? ALTERNATE
                                                                    : WINDING));

    BeginPath(graphics->hdc);
    retval = draw_poly(graphics, NULL, path->pathdata.Points,
                       path->pathdata.Types, path->pathdata.Count, FALSE);

    if(retval != Ok)
        goto end;

    EndPath(graphics->hdc);
    brush_fill_path(graphics, brush);

    retval = Ok;

end:
    RestoreDC(graphics->hdc, save_state);

    return retval;
}

static Status SOFTWARE_GdipFillPath(Graphics *graphics, Brush *brush, Path *path)
{
    Status stat;
    Region *rgn;

    if (!brush_can_fill_pixels(brush))
        return NotImplemented;

    /* FIXME: This could probably be done more efficiently without regions. */

    stat = GdipCreateRegionPath(path, &rgn);

    if (stat == Ok)
    {
        stat = GdipFillRegion(graphics, brush, rgn);

        GdipDeleteRegion(rgn);
    }

    return stat;
}

Status GdipFillPath(Graphics *graphics, Brush *brush, Path *path)
{
    Status stat = NotImplemented;

    TRACE("(%p, %p, %p)\n", graphics, brush, path);

    if(!brush || !graphics || !path)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->image && !graphics->alpha_hdc)
        stat = GDI32_GdipFillPath(graphics, brush, path);

    if (stat == NotImplemented)
        stat = SOFTWARE_GdipFillPath(graphics, brush, path);

    if (stat == NotImplemented)
    {
        FIXME("Not implemented for brushtype %i\n", brush->bt);
        stat = Ok;
    }

    return stat;
}

Status GdipFillPie(Graphics *graphics, Brush *brush, REAL x,
    REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle)
{
    Status stat;
    Path *path;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)\n",
            graphics, brush, x, y, width, height, startAngle, sweepAngle);

    if(!graphics || !brush)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    stat = PathCreate(FillModeAlternate, &path);

    if (stat == Ok)
    {
        stat = PathAddPie(path, x, y, width, height, startAngle, sweepAngle);

        if (stat == Ok)
            stat = GdipFillPath(graphics, brush, path);

        PathDelete(path);
    }

    return stat;
}

Status GdipFillPieI(Graphics *graphics, Brush *brush, INT x,
    INT y, INT width, INT height, REAL startAngle, REAL sweepAngle)
{
    TRACE("(%p, %p, %d, %d, %d, %d, %.2f, %.2f)\n",
            graphics, brush, x, y, width, height, startAngle, sweepAngle);

    return GdipFillPie(graphics,brush,(REAL)x,(REAL)y,(REAL)width,(REAL)height,startAngle,sweepAngle);
}

Status GdipFillPolygon(Graphics *graphics, Brush *brush,
    GDIPCONST PointF *points, INT count, GpFillMode fillMode)
{
    Status stat;
    Path *path;

    TRACE("(%p, %p, %p, %d, %d)\n", graphics, brush, points, count, fillMode);

    if(!graphics || !brush || !points || !count)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    stat = PathCreate(fillMode, &path);

    if (stat == Ok)
    {
        stat = PathAddPolygon(path, points, count);

        if (stat == Ok)
            stat = GdipFillPath(graphics, brush, path);

        PathDelete(path);
    }

    return stat;
}

Status GdipFillPolygonI(Graphics *graphics, Brush *brush,
    GDIPCONST Point *points, INT count, GpFillMode fillMode)
{
    Status stat;
    Path *path;

    TRACE("(%p, %p, %p, %d, %d)\n", graphics, brush, points, count, fillMode);

    if(!graphics || !brush || !points || !count)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    stat = PathCreate(fillMode, &path);

    if (stat == Ok)
    {
        stat = PathAddPolygonI(path, points, count);

        if (stat == Ok)
            stat = GdipFillPath(graphics, brush, path);

        PathDelete(path);
    }

    return stat;
}

Status GdipFillPolygon2(Graphics *graphics, Brush *brush,
    GDIPCONST PointF *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, brush, points, count);

    return GdipFillPolygon(graphics, brush, points, count, FillModeAlternate);
}

Status GdipFillPolygon2I(Graphics *graphics, Brush *brush,
    GDIPCONST Point *points, INT count)
{
    TRACE("(%p, %p, %p, %d)\n", graphics, brush, points, count);

    return GdipFillPolygonI(graphics, brush, points, count, FillModeAlternate);
}

Status GdipFillRectangle(Graphics *graphics, Brush *brush,
    REAL x, REAL y, REAL width, REAL height)
{
    Status stat;
    Path *path;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f)\n", graphics, brush, x, y, width, height);

    if(!graphics || !brush)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    stat = PathCreate(FillModeAlternate, &path);

    if (stat == Ok)
    {
        stat = PathAddRectangle(path, x, y, width, height);

        if (stat == Ok)
            stat = GdipFillPath(graphics, brush, path);

        PathDelete(path);
    }

    return stat;
}

Status GdipFillRectangleI(Graphics *graphics, Brush *brush,
    INT x, INT y, INT width, INT height)
{
    TRACE("(%p, %p, %d, %d, %d, %d)\n", graphics, brush, x, y, width, height);

    return GdipFillRectangle(graphics, brush, x, y, width, height);
}

Status GdipFillRectangles(Graphics *graphics, Brush *brush, GDIPCONST RectF *rects,
    INT count)
{
    Status ret;
    INT i;

    TRACE("(%p, %p, %p, %d)\n", graphics, brush, rects, count);

    if(!rects)
        return InvalidParameter;

    for(i = 0; i < count; i++){
        ret = GdipFillRectangle(graphics, brush, rects[i].X, rects[i].Y, rects[i].Width, rects[i].Height);
        if(ret != Ok)   return ret;
    }

    return Ok;
}

Status GdipFillRectanglesI(Graphics *graphics, Brush *brush, GDIPCONST Rect *rects,
    INT count)
{
    RectF *rectsF;
    Status ret;
    INT i;

    TRACE("(%p, %p, %p, %d)\n", graphics, brush, rects, count);

    if(!rects || count <= 0)
        return InvalidParameter;

    rectsF = GdipAlloc(sizeof(RectF)*count);
    if(!rectsF)
        return OutOfMemory;

    for(i = 0; i < count; i++){
        rectsF[i].X      = (REAL)rects[i].X;
        rectsF[i].Y      = (REAL)rects[i].Y;
        rectsF[i].X      = (REAL)rects[i].Width;
        rectsF[i].Height = (REAL)rects[i].Height;
    }

    ret = GdipFillRectangles(graphics,brush,rectsF,count);
    GdipFree(rectsF);

    return ret;
}

static Status GDI32_GdipFillRegion(Graphics* graphics, Brush* brush,
    Region* region)
{
    INT save_state;
    Status status;
    HRGN hrgn;
    RECT rc;

    if(!graphics->hdc || !brush_can_fill_path(brush))
        return NotImplemented;

    status = GdipGetRegionHRgn(region, graphics, &hrgn);
    if(status != Ok)
        return status;

    save_state = SaveDC(graphics->hdc);
    EndPath(graphics->hdc);

    ExtSelectClipRgn(graphics->hdc, hrgn, RGN_AND);

    if (GetClipBox(graphics->hdc, &rc) != NULLREGION)
    {
        BeginPath(graphics->hdc);
        Rectangle(graphics->hdc, rc.left, rc.top, rc.right, rc.bottom);
        EndPath(graphics->hdc);

        brush_fill_path(graphics, brush);
    }

    RestoreDC(graphics->hdc, save_state);

    DeleteObject(hrgn);

    return Ok;
}

static Status SOFTWARE_GdipFillRegion(Graphics *graphics, Brush *brush,
    Region* region)
{
    Status stat;
    Region *temp_region;
    Matrix world_to_device;
    RectF graphics_bounds;
    DWORD *pixel_data;
    HRGN hregion;
    RECT bound_rect;
    Rect gp_bound_rect;

    if (!brush_can_fill_pixels(brush))
        return NotImplemented;

    stat = get_graphics_bounds(graphics, &graphics_bounds);

    if (stat == Ok)
        stat = GdipCloneRegion(region, &temp_region);

    if (stat == Ok)
    {
        stat = get_graphics_transform(graphics, CoordinateSpaceDevice,
            CoordinateSpaceWorld, &world_to_device);

        if (stat == Ok)
            stat = GdipTransformRegion(temp_region, &world_to_device);

        if (stat == Ok)
            stat = GdipCombineRegionRect(temp_region, &graphics_bounds, CombineModeIntersect);

        if (stat == Ok)
            stat = GdipGetRegionHRgn(temp_region, NULL, &hregion);

        GdipDeleteRegion(temp_region);
    }

    if (stat == Ok && GetRgnBox(hregion, &bound_rect) == NULLREGION)
    {
        DeleteObject(hregion);
        return Ok;
    }

    if (stat == Ok)
    {
        gp_bound_rect.X = bound_rect.left;
        gp_bound_rect.Y = bound_rect.top;
        gp_bound_rect.Width = bound_rect.right - bound_rect.left;
        gp_bound_rect.Height = bound_rect.bottom - bound_rect.top;

        pixel_data = GdipAlloc(sizeof(*pixel_data) * gp_bound_rect.Width * gp_bound_rect.Height);
        if (!pixel_data)
            stat = OutOfMemory;

        if (stat == Ok)
        {
            stat = brush_fill_pixels(graphics, brush, pixel_data,
                &gp_bound_rect, gp_bound_rect.Width);

            if (stat == Ok)
                stat = alpha_blend_pixels_hrgn(graphics, gp_bound_rect.X,
                    gp_bound_rect.Y, (BYTE*)pixel_data, gp_bound_rect.Width,
                    gp_bound_rect.Height, gp_bound_rect.Width * 4, hregion);

            GdipFree(pixel_data);
        }

        DeleteObject(hregion);
    }

    return stat;
}

/*****************************************************************************
 * GdipFillRegion [GDIPLUS.@]
 */
Status GdipFillRegion(Graphics* graphics, Brush* brush,
        Region* region)
{
    Status stat = NotImplemented;

    TRACE("(%p, %p, %p)\n", graphics, brush, region);

    if (!(graphics && brush && region))
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->image && !graphics->alpha_hdc)
        stat = GDI32_GdipFillRegion(graphics, brush, region);

    if (stat == NotImplemented)
        stat = SOFTWARE_GdipFillRegion(graphics, brush, region);

    if (stat == NotImplemented)
    {
        FIXME("not implemented for brushtype %i\n", brush->bt);
        stat = Ok;
    }

    return stat;
}

Status GdipFlush(Graphics *graphics, GpFlushIntention intention)
{
    TRACE("(%p,%u)\n", graphics, intention);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    /* We have no internal operation queue, so there's no need to clear it. */

    if (graphics->hdc)
        GdiFlush();

    return Ok;
}

/*****************************************************************************
 * GdipGetClipBounds [GDIPLUS.@]
 */
Status GdipGetClipBounds(Graphics *graphics, RectF *rect)
{
    TRACE("(%p, %p)\n", graphics, rect);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipGetRegionBounds(graphics->clip, graphics, rect);
}

/*****************************************************************************
 * GdipGetClipBoundsI [GDIPLUS.@]
 */
Status GdipGetClipBoundsI(Graphics *graphics, Rect *rect)
{
    TRACE("(%p, %p)\n", graphics, rect);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipGetRegionBoundsI(graphics->clip, graphics, rect);
}

/* FIXME: Compositing mode is not used anywhere except the getter/setter. */
Status GdipGetCompositingMode(Graphics *graphics,
    CompositingMode *mode)
{
    TRACE("(%p, %p)\n", graphics, mode);

    if(!graphics || !mode)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *mode = graphics->compmode;

    return Ok;
}

/* FIXME: Compositing quality is not used anywhere except the getter/setter. */
Status GdipGetCompositingQuality(Graphics *graphics,
    CompositingQuality *quality)
{
    TRACE("(%p, %p)\n", graphics, quality);

    if(!graphics || !quality)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *quality = graphics->compqual;

    return Ok;
}

/* FIXME: Interpolation mode is not used anywhere except the getter/setter. */
Status GdipGetInterpolationMode(Graphics *graphics,
    InterpolationMode *mode)
{
    TRACE("(%p, %p)\n", graphics, mode);

    if(!graphics || !mode)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *mode = graphics->interpolation;

    return Ok;
}

/* FIXME: Need to handle color depths less than 24bpp */
Status GdipGetNearestColor(Graphics *graphics, ARGB* argb)
{
    FIXME("(%p, %p): Passing color unmodified\n", graphics, argb);

    if(!graphics || !argb)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return Ok;
}

Status GdipGetPageScale(Graphics *graphics, REAL *scale)
{
    TRACE("(%p, %p)\n", graphics, scale);

    if(!graphics || !scale)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *scale = graphics->scale;

    return Ok;
}

Status GdipGetPageUnit(Graphics *graphics, Unit *unit)
{
    TRACE("(%p, %p)\n", graphics, unit);

    if(!graphics || !unit)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *unit = graphics->unit;

    return Ok;
}

/* FIXME: Pixel offset mode is not used anywhere except the getter/setter. */
Status GdipGetPixelOffsetMode(Graphics *graphics, PixelOffsetMode
    *mode)
{
    TRACE("(%p, %p)\n", graphics, mode);

    if(!graphics || !mode)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *mode = graphics->pixeloffset;

    return Ok;
}

/* FIXME: Smoothing mode is not used anywhere except the getter/setter. */
Status GdipGetSmoothingMode(Graphics *graphics, SmoothingMode *mode)
{
    TRACE("(%p, %p)\n", graphics, mode);

    if(!graphics || !mode)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *mode = graphics->smoothing;

    return Ok;
}

Status GdipGetTextContrast(Graphics *graphics, UINT *contrast)
{
    TRACE("(%p, %p)\n", graphics, contrast);

    if(!graphics || !contrast)
        return InvalidParameter;

    *contrast = graphics->textcontrast;

    return Ok;
}

/* FIXME: Text rendering hint is not used anywhere except the getter/setter. */
Status GdipGetTextRenderingHint(Graphics *graphics,
    TextRenderingHint *hint)
{
    TRACE("(%p, %p)\n", graphics, hint);

    if(!graphics || !hint)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *hint = graphics->texthint;

    return Ok;
}

Status GdipGetVisibleClipBounds(Graphics *graphics, RectF *rect)
{
    Region *clip_rgn;
    Status stat;

    TRACE("(%p, %p)\n", graphics, rect);

    if(!graphics || !rect)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    /* intersect window and graphics clipping regions */
    if((stat = GdipCreateRegion(&clip_rgn)) != Ok)
        return stat;

    if((stat = get_visible_clip_region(graphics, clip_rgn)) != Ok)
        goto cleanup;

    /* get bounds of the region */
    stat = GdipGetRegionBounds(clip_rgn, graphics, rect);

cleanup:
    GdipDeleteRegion(clip_rgn);

    return stat;
}

Status GdipGetVisibleClipBoundsI(Graphics *graphics, Rect *rect)
{
    RectF rectf;
    Status stat;

    TRACE("(%p, %p)\n", graphics, rect);

    if(!graphics || !rect)
        return InvalidParameter;

    if((stat = GdipGetVisibleClipBounds(graphics, &rectf)) == Ok)
    {
        rect->X = gdip_round(rectf.X);
        rect->Y = gdip_round(rectf.Y);
        rect->Width  = gdip_round(rectf.Width);
        rect->Height = gdip_round(rectf.Height);
    }

    return stat;
}

Status GdipGetWorldTransform(Graphics *graphics, Matrix *matrix)
{
    TRACE("(%p, %p)\n", graphics, matrix);

    if(!graphics || !matrix)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *matrix = graphics->worldtrans;
    return Ok;
}

Status GdipGraphicsClear(Graphics *graphics, ARGB color)
{
    SolidFill *brush;
    Status stat;
    RectF wnd_rect;

    TRACE("(%p, %x)\n", graphics, color);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if((stat = GdipCreateSolidFill(color, &brush)) != Ok)
        return stat;

    if((stat = get_graphics_bounds(graphics, &wnd_rect)) != Ok){
        GdipDeleteBrush((Brush*)brush);
        return stat;
    }

    GdipFillRectangle(graphics, (Brush*)brush, wnd_rect.X, wnd_rect.Y,
                                                 wnd_rect.Width, wnd_rect.Height);

    GdipDeleteBrush((Brush*)brush);

    return Ok;
}

Status GdipIsClipEmpty(Graphics *graphics, BOOL *res)
{
    TRACE("(%p, %p)\n", graphics, res);

    if(!graphics || !res)
        return InvalidParameter;

    return GdipIsEmptyRegion(graphics->clip, graphics, res);
}

Status GdipIsVisiblePoint(Graphics *graphics, REAL x, REAL y, BOOL *result)
{
    Status stat;
    Region* rgn;
    PointF pt;

    TRACE("(%p, %.2f, %.2f, %p)\n", graphics, x, y, result);

    if(!graphics || !result)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    pt.X = x;
    pt.Y = y;
    if((stat = GdipTransformPoints(graphics, CoordinateSpaceDevice,
                   CoordinateSpaceWorld, &pt, 1)) != Ok)
        return stat;

    if((stat = GdipCreateRegion(&rgn)) != Ok)
        return stat;

    if((stat = get_visible_clip_region(graphics, rgn)) != Ok)
        goto cleanup;

    stat = GdipIsVisibleRegionPoint(rgn, pt.X, pt.Y, graphics, result);

cleanup:
    GdipDeleteRegion(rgn);
    return stat;
}

Status GdipIsVisiblePointI(Graphics *graphics, INT x, INT y, BOOL *result)
{
    return GdipIsVisiblePoint(graphics, (REAL)x, (REAL)y, result);
}

Status GdipIsVisibleRect(Graphics *graphics, REAL x, REAL y, REAL width, REAL height, BOOL *result)
{
    Status stat;
    Region* rgn;
    PointF pts[2];

    TRACE("(%p %.2f %.2f %.2f %.2f %p)\n", graphics, x, y, width, height, result);

    if(!graphics || !result)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    pts[0].X = x;
    pts[0].Y = y;
    pts[1].X = x + width;
    pts[1].Y = y + height;

    if((stat = GdipTransformPoints(graphics, CoordinateSpaceDevice,
                    CoordinateSpaceWorld, pts, 2)) != Ok)
        return stat;

    pts[1].X -= pts[0].X;
    pts[1].Y -= pts[0].Y;

    if((stat = GdipCreateRegion(&rgn)) != Ok)
        return stat;

    if((stat = get_visible_clip_region(graphics, rgn)) != Ok)
        goto cleanup;

    stat = GdipIsVisibleRegionRect(rgn, pts[0].X, pts[0].Y, pts[1].X, pts[1].Y, graphics, result);

cleanup:
    GdipDeleteRegion(rgn);
    return stat;
}

Status GdipIsVisibleRectI(Graphics *graphics, INT x, INT y, INT width, INT height, BOOL *result)
{
    return GdipIsVisibleRect(graphics, (REAL)x, (REAL)y, (REAL)width, (REAL)height, result);
}

Status gdip_format_string(HDC hdc,
    GDIPCONST WCHAR *string, INT length, GDIPCONST Font *font,
    GDIPCONST RectF *rect, GDIPCONST StringFormat *format,
    gdip_format_string_callback callback, void *user_data)
{
    WCHAR* stringdup;
    int sum = 0, height = 0, fit, fitcpy, i, j, lret, nwidth,
        nheight, lineend, lineno = 0;
    RectF bounds;
    StringAlignment halign;
    Status stat = Ok;
    SIZE size;
    HotkeyPrefix hkprefix;
    INT *hotkeyprefix_offsets=NULL;
    INT hotkeyprefix_count=0;
    INT hotkeyprefix_pos=0, hotkeyprefix_end_pos=0;
    int seen_prefix=0;

    if(length == -1) length = lstrlenW(string);

    stringdup = GdipAlloc((length + 1) * sizeof(WCHAR));
    if(!stringdup) return OutOfMemory;

    nwidth = rect->Width;
    nheight = rect->Height;

    if (format)
        hkprefix = format->hkprefix;
    else
        hkprefix = HotkeyPrefixNone;

    if (hkprefix == HotkeyPrefixShow)
    {
        for (i=0; i<length; i++)
        {
            if (string[i] == '&')
                hotkeyprefix_count++;
        }
    }

    if (hotkeyprefix_count)
        hotkeyprefix_offsets = GdipAlloc(sizeof(INT) * hotkeyprefix_count);

    hotkeyprefix_count = 0;

    for(i = 0, j = 0; i < length; i++){
        /* FIXME: This makes the indexes passed to callback inaccurate. */
        if(!isprintW(string[i]) && (string[i] != '\n'))
            continue;

        /* FIXME: tabs should be handled using tabstops from stringformat */
        if (string[i] == '\t')
            continue;

        if (seen_prefix && hkprefix == HotkeyPrefixShow && string[i] != '&')
            hotkeyprefix_offsets[hotkeyprefix_count++] = j;
        else if (!seen_prefix && hkprefix != HotkeyPrefixNone && string[i] == '&')
        {
            seen_prefix = 1;
            continue;
        }

        seen_prefix = 0;

        stringdup[j] = string[i];
        j++;
    }

    length = j;

    if (format) halign = format->align;
    else halign = StringAlignmentNear;

    while(sum < length){
        GetTextExtentExPointW(hdc, stringdup + sum, length - sum,
                              nwidth, &fit, NULL, &size);
        fitcpy = fit;

        if(fit == 0)
            break;

        for(lret = 0; lret < fit; lret++)
            if(*(stringdup + sum + lret) == '\n')
                break;

        /* Line break code (may look strange, but it imitates windows). */
        if(lret < fit)
            lineend = fit = lret;    /* this is not an off-by-one error */
        else if(fit < (length - sum)){
            if(*(stringdup + sum + fit) == ' ')
                while(*(stringdup + sum + fit) == ' ')
                    fit++;
            else
                while(*(stringdup + sum + fit - 1) != ' '){
                    fit--;

                    if(*(stringdup + sum + fit) == '\t')
                        break;

                    if(fit == 0){
                        fit = fitcpy;
                        break;
                    }
                }
            lineend = fit;
            while(*(stringdup + sum + lineend - 1) == ' ' ||
                  *(stringdup + sum + lineend - 1) == '\t')
                lineend--;
        }
        else
            lineend = fit;

        GetTextExtentExPointW(hdc, stringdup + sum, lineend,
                              nwidth, &j, NULL, &size);

        bounds.Width = size.cx;

        if(height + size.cy > nheight)
            bounds.Height = nheight - (height + size.cy);
        else
            bounds.Height = size.cy;

        bounds.Y = rect->Y + height;

        switch (halign)
        {
        case StringAlignmentNear:
        default:
            bounds.X = rect->X;
            break;
        case StringAlignmentCenter:
            bounds.X = rect->X + (rect->Width/2) - (bounds.Width/2);
            break;
        case StringAlignmentFar:
            bounds.X = rect->X + rect->Width - bounds.Width;
            break;
        }

        for (hotkeyprefix_end_pos=hotkeyprefix_pos; hotkeyprefix_end_pos<hotkeyprefix_count; hotkeyprefix_end_pos++)
            if (hotkeyprefix_offsets[hotkeyprefix_end_pos] >= sum + lineend)
                break;

        stat = callback(hdc, stringdup, sum, lineend,
            font, rect, format, lineno, &bounds,
            &hotkeyprefix_offsets[hotkeyprefix_pos],
            hotkeyprefix_end_pos-hotkeyprefix_pos, user_data);

        if (stat != Ok)
            break;

        sum += fit + (lret < fitcpy ? 1 : 0);
        height += size.cy;
        lineno++;

        hotkeyprefix_pos = hotkeyprefix_end_pos;

        if(height > nheight)
            break;

        /* Stop if this was a linewrap (but not if it was a linebreak). */
        if ((lret == fitcpy) && format &&
            (format->attr & (StringFormatFlagsNoWrap | StringFormatFlagsLineLimit)))
            break;
    }

    GdipFree(stringdup);
    GdipFree(hotkeyprefix_offsets);

    return stat;
}

struct measure_ranges_args {
    Region **regions;
    REAL rel_width, rel_height;
};

static Status measure_ranges_callback(HDC hdc,
    GDIPCONST WCHAR *string, INT index, INT length, GDIPCONST Font *font,
    GDIPCONST RectF *rect, GDIPCONST StringFormat *format,
    INT lineno, const RectF *bounds, INT *underlined_indexes,
    INT underlined_index_count, void *user_data)
{
    int i;
    Status stat = Ok;
    struct measure_ranges_args *args = user_data;

    for (i=0; i<format->range_count; i++)
    {
        INT range_start = MAX(index, format->character_ranges[i].First);
        INT range_end = MIN(index+length, format->character_ranges[i].First+format->character_ranges[i].Length);
        if (range_start < range_end)
        {
            RectF range_rect;
            SIZE range_size;

            range_rect.Y = bounds->Y / args->rel_height;
            range_rect.Height = bounds->Height / args->rel_height;

            GetTextExtentExPointW(hdc, string + index, range_start - index,
                                  INT_MAX, NULL, NULL, &range_size);
            range_rect.X = (bounds->X + range_size.cx) / args->rel_width;

            GetTextExtentExPointW(hdc, string + index, range_end - index,
                                  INT_MAX, NULL, NULL, &range_size);
            range_rect.Width = (bounds->X + range_size.cx) / args->rel_width - range_rect.X;

            stat = GdipCombineRegionRect(args->regions[i], &range_rect, CombineModeUnion);
            if (stat != Ok)
                break;
        }
    }

    return stat;
}

Status GdipMeasureCharacterRanges(Graphics* graphics,
        GDIPCONST WCHAR* string, INT length, GDIPCONST Font* font,
        GDIPCONST RectF* layoutRect, GDIPCONST StringFormat *stringFormat,
        INT regionCount, Region** regions)
{
    Status stat;
    int i;
    HFONT gdifont, oldfont;
    struct measure_ranges_args args;
    HDC hdc, temp_hdc=NULL;
    PointF pt[3];
    RectF scaled_rect;
    REAL margin_x;

    TRACE("(%p %s %d %p %s %p %d %p)\n", graphics, debugstr_w(string),
            length, font, debugstr_rectf(layoutRect), stringFormat, regionCount, regions);

    if (!(graphics && string && font && layoutRect && stringFormat && regions))
        return InvalidParameter;

    if (regionCount < stringFormat->range_count)
        return InvalidParameter;

    if(!graphics->hdc)
    {
        hdc = temp_hdc = CreateCompatibleDC(0);
        if (!temp_hdc) return OutOfMemory;
    }
    else
        hdc = graphics->hdc;

    if (stringFormat->attr)
        TRACE("may be ignoring some format flags: attr %x\n", stringFormat->attr);

    pt[0].X = 0.0;
    pt[0].Y = 0.0;
    pt[1].X = 1.0;
    pt[1].Y = 0.0;
    pt[2].X = 0.0;
    pt[2].Y = 1.0;
    GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, pt, 3);
    args.rel_width = sqrt((pt[1].Y-pt[0].Y)*(pt[1].Y-pt[0].Y)+
                     (pt[1].X-pt[0].X)*(pt[1].X-pt[0].X));
    args.rel_height = sqrt((pt[2].Y-pt[0].Y)*(pt[2].Y-pt[0].Y)+
                      (pt[2].X-pt[0].X)*(pt[2].X-pt[0].X));

    margin_x = stringFormat->generic_typographic ? 0.0 : font->emSize / 6.0;
    margin_x *= units_scale(font->unit, graphics->unit, graphics->xres);

    scaled_rect.X = (layoutRect->X + margin_x) * args.rel_width;
    scaled_rect.Y = layoutRect->Y * args.rel_height;
    if (stringFormat->attr & StringFormatFlagsNoClip)
    {
        scaled_rect.Width = (REAL)(1 << 23);
        scaled_rect.Height = (REAL)(1 << 23);
    }
    else
    {
        scaled_rect.Width = layoutRect->Width * args.rel_width;
        scaled_rect.Height = layoutRect->Height * args.rel_height;
    }
    if (scaled_rect.Width >= 0.5)
    {
        scaled_rect.Width -= margin_x * 2.0 * args.rel_width;
        if (scaled_rect.Width < 0.5) return Ok; /* doesn't fit */
    }

    get_font_hfont(graphics, font, stringFormat, &gdifont, NULL);
    oldfont = SelectObject(hdc, gdifont);

    for (i=0; i<stringFormat->range_count; i++)
    {
        stat = GdipSetEmpty(regions[i]);
        if (stat != Ok)
            return stat;
    }

    args.regions = regions;

    stat = gdip_format_string(hdc, string, length, font, &scaled_rect, stringFormat,
        measure_ranges_callback, &args);

    SelectObject(hdc, oldfont);
    DeleteObject(gdifont);

    if (temp_hdc)
        DeleteDC(temp_hdc);

    return stat;
}

struct measure_string_args {
    RectF *bounds;
    INT *codepointsfitted;
    INT *linesfilled;
    REAL rel_width, rel_height;
};

static Status measure_string_callback(HDC hdc,
    GDIPCONST WCHAR *string, INT index, INT length, GDIPCONST Font *font,
    GDIPCONST RectF *rect, GDIPCONST StringFormat *format,
    INT lineno, const RectF *bounds, INT *underlined_indexes,
    INT underlined_index_count, void *user_data)
{
    struct measure_string_args *args = user_data;
    REAL new_width, new_height;

    new_width = bounds->Width / args->rel_width;
    new_height = (bounds->Height + bounds->Y) / args->rel_height - args->bounds->Y;

    if (new_width > args->bounds->Width)
        args->bounds->Width = new_width;

    if (new_height > args->bounds->Height)
        args->bounds->Height = new_height;

    if (args->codepointsfitted)
        *args->codepointsfitted = index + length;

    if (args->linesfilled)
        (*args->linesfilled)++;

    return Ok;
}

/* Find the smallest rectangle that bounds the text when it is printed in rect
 * according to the format options listed in format. If rect has 0 width and
 * height, then just find the smallest rectangle that bounds the text when it's
 * printed at location (rect->X, rect-Y). */
Status GdipMeasureString(Graphics *graphics,
    GDIPCONST WCHAR *string, INT length, GDIPCONST Font *font,
    GDIPCONST RectF *rect, GDIPCONST StringFormat *format, RectF *bounds,
    INT *codepointsfitted, INT *linesfilled)
{
    HFONT oldfont, gdifont;
    struct measure_string_args args;
    HDC temp_hdc=NULL, hdc;
    PointF pt[3];
    RectF scaled_rect;
    REAL margin_x;
    INT lines, glyphs, format_flags = format ? format->attr : 0;

    TRACE("(%p, %s, %i, %p, %s, %p, %p, %p, %p)\n", graphics,
        debugstr_wn(string, length), length, font, debugstr_rectf(rect), format,
        bounds, codepointsfitted, linesfilled);

    if(!graphics || !string || !font || !rect || !bounds)
        return InvalidParameter;

    if(!graphics->hdc)
    {
        hdc = temp_hdc = CreateCompatibleDC(0);
        if (!temp_hdc) return OutOfMemory;
    }
    else
        hdc = graphics->hdc;

    if(linesfilled) *linesfilled = 0;
    if(codepointsfitted) *codepointsfitted = 0;

    if(format)
        TRACE("may be ignoring some format flags: attr %x\n", format->attr);

    pt[0].X = 0.0;
    pt[0].Y = 0.0;
    pt[1].X = 1.0;
    pt[1].Y = 0.0;
    pt[2].X = 0.0;
    pt[2].Y = 1.0;
    GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, pt, 3);
    args.rel_width = sqrt((pt[1].Y-pt[0].Y)*(pt[1].Y-pt[0].Y)+
                     (pt[1].X-pt[0].X)*(pt[1].X-pt[0].X));
    args.rel_height = sqrt((pt[2].Y-pt[0].Y)*(pt[2].Y-pt[0].Y)+
                      (pt[2].X-pt[0].X)*(pt[2].X-pt[0].X));

    margin_x = (format && format->generic_typographic) ? 0.0 : font->emSize / 6.0;
    margin_x *= units_scale(font->unit, graphics->unit, graphics->xres);

    scaled_rect.X = (rect->X + margin_x) * args.rel_width;
    scaled_rect.Y = rect->Y * args.rel_height;
    scaled_rect.Width = rect->Width * args.rel_width;
    scaled_rect.Height = rect->Height * args.rel_height;

    if ((format_flags & StringFormatFlagsNoClip) ||
        scaled_rect.Width >= 1 << 23 || scaled_rect.Width < 0.5) scaled_rect.Width = 1 << 23;
    if ((format_flags & StringFormatFlagsNoClip) ||
        scaled_rect.Height >= 1 << 23 || scaled_rect.Height < 0.5) scaled_rect.Height = 1 << 23;

    if (scaled_rect.Width >= 0.5)
    {
        scaled_rect.Width -= margin_x * 2.0 * args.rel_width;
        if (scaled_rect.Width < 0.5) return Ok; /* doesn't fit */
    }

    if (scaled_rect.Width >= 1 << 23 || scaled_rect.Width < 0.5) scaled_rect.Width = 1 << 23;
    if (scaled_rect.Height >= 1 << 23 || scaled_rect.Height < 0.5) scaled_rect.Height = 1 << 23;

    get_font_hfont(graphics, font, format, &gdifont, NULL);
    oldfont = SelectObject(hdc, gdifont);

    bounds->X = rect->X;
    bounds->Y = rect->Y;
    bounds->Width = 0.0;
    bounds->Height = 0.0;

    args.bounds = bounds;
    args.codepointsfitted = &glyphs;
    args.linesfilled = &lines;
    lines = glyphs = 0;

    gdip_format_string(hdc, string, length, font, &scaled_rect, format,
        measure_string_callback, &args);

    if (linesfilled) *linesfilled = lines;
    if (codepointsfitted) *codepointsfitted = glyphs;

    if (lines)
        bounds->Width += margin_x * 2.0;

    SelectObject(hdc, oldfont);
    DeleteObject(gdifont);

    if (temp_hdc)
        DeleteDC(temp_hdc);

    return Ok;
}

struct draw_string_args {
    Graphics *graphics;
    GDIPCONST Brush *brush;
    REAL x, y, rel_width, rel_height, ascent;
};

static Status draw_string_callback(HDC hdc,
    GDIPCONST WCHAR *string, INT index, INT length, GDIPCONST Font *font,
    GDIPCONST RectF *rect, GDIPCONST StringFormat *format,
    INT lineno, const RectF *bounds, INT *underlined_indexes,
    INT underlined_index_count, void *user_data)
{
    struct draw_string_args *args = user_data;
    PointF position;
    Status stat;

    position.X = args->x + bounds->X / args->rel_width;
    position.Y = args->y + bounds->Y / args->rel_height + args->ascent;

    stat = draw_driver_string(args->graphics, &string[index], length, font, format,
        args->brush, &position,
        DriverStringOptionsCmapLookup|DriverStringOptionsRealizedAdvance, NULL);

    if (stat == Ok && underlined_index_count)
    {
        OUTLINETEXTMETRICW otm;
        REAL underline_y, underline_height;
        int i;

        GetOutlineTextMetricsW(hdc, sizeof(otm), &otm);

        underline_height = otm.otmsUnderscoreSize / args->rel_height;
        underline_y = position.Y - otm.otmsUnderscorePosition / args->rel_height - underline_height / 2;

        for (i=0; i<underlined_index_count; i++)
        {
            REAL start_x, end_x;
            SIZE text_size;
            INT ofs = underlined_indexes[i] - index;

            GetTextExtentExPointW(hdc, string + index, ofs, INT_MAX, NULL, NULL, &text_size);
            start_x = text_size.cx / args->rel_width;

            GetTextExtentExPointW(hdc, string + index, ofs+1, INT_MAX, NULL, NULL, &text_size);
            end_x = text_size.cx / args->rel_width;

            GdipFillRectangle(args->graphics, (Brush*)args->brush, position.X+start_x, underline_y, end_x-start_x, underline_height);
        }
    }

    return stat;
}

Status GdipDrawString(Graphics *graphics, GDIPCONST WCHAR *string,
    INT length, GDIPCONST Font *font, GDIPCONST RectF *rect,
    GDIPCONST StringFormat *format, GDIPCONST Brush *brush)
{
    HRGN rgn = NULL;
    HFONT gdifont;
    PointF pt[3], rectcpy[4];
    IPOINT corners[4];
    REAL rel_width, rel_height, margin_x;
    INT save_state, format_flags = 0;
    REAL offsety = 0.0;
    struct draw_string_args args;
    RectF scaled_rect;
    HDC hdc, temp_hdc=NULL;
    TEXTMETRICW textmetric;

    TRACE("(%p, %s, %i, %p, %s, %p, %p)\n", graphics, debugstr_wn(string, length),
        length, font, debugstr_rectf(rect), format, brush);

    if(!graphics || !string || !font || !brush || !rect)
        return InvalidParameter;

    if(graphics->hdc)
    {
        hdc = graphics->hdc;
    }
    else
    {
        hdc = temp_hdc = CreateCompatibleDC(0);
    }

    if(format){
        TRACE("may be ignoring some format flags: attr %x\n", format->attr);

        format_flags = format->attr;

        /* Should be no need to explicitly test for StringAlignmentNear as
         * that is default behavior if no alignment is passed. */
        if(format->vertalign != StringAlignmentNear){
            RectF bounds, in_rect = *rect;
            in_rect.Height = 0.0; /* avoid height clipping */
            GdipMeasureString(graphics, string, length, font, &in_rect, format, &bounds, 0, 0);

            TRACE("bounds %s\n", debugstr_rectf(&bounds));

            if(format->vertalign == StringAlignmentCenter)
                offsety = (rect->Height - bounds.Height) / 2;
            else if(format->vertalign == StringAlignmentFar)
                offsety = (rect->Height - bounds.Height);
        }
        TRACE("vertical align %d, offsety %f\n", format->vertalign, offsety);
    }

    save_state = SaveDC(hdc);

    pt[0].X = 0.0;
    pt[0].Y = 0.0;
    pt[1].X = 1.0;
    pt[1].Y = 0.0;
    pt[2].X = 0.0;
    pt[2].Y = 1.0;
    GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, pt, 3);
    rel_width = sqrt((pt[1].Y-pt[0].Y)*(pt[1].Y-pt[0].Y)+
                     (pt[1].X-pt[0].X)*(pt[1].X-pt[0].X));
    rel_height = sqrt((pt[2].Y-pt[0].Y)*(pt[2].Y-pt[0].Y)+
                      (pt[2].X-pt[0].X)*(pt[2].X-pt[0].X));

    rectcpy[3].X = rectcpy[0].X = rect->X;
    rectcpy[1].Y = rectcpy[0].Y = rect->Y;
    rectcpy[2].X = rectcpy[1].X = rect->X + rect->Width;
    rectcpy[3].Y = rectcpy[2].Y = rect->Y + rect->Height;
    transform_and_round_points(graphics, corners, rectcpy, 4);

    margin_x = (format && format->generic_typographic) ? 0.0 : font->emSize / 6.0;
    margin_x *= units_scale(font->unit, graphics->unit, graphics->xres);

    scaled_rect.X = margin_x * rel_width;
    scaled_rect.Y = 0.0;
    scaled_rect.Width = rel_width * rect->Width;
    scaled_rect.Height = rel_height * rect->Height;

    if ((format_flags & StringFormatFlagsNoClip) ||
        scaled_rect.Width >= 1 << 23 || scaled_rect.Width < 0.5) scaled_rect.Width = 1 << 23;
    if ((format_flags & StringFormatFlagsNoClip) ||
        scaled_rect.Height >= 1 << 23 || scaled_rect.Height < 0.5) scaled_rect.Height = 1 << 23;

    if (scaled_rect.Width >= 0.5)
    {
        scaled_rect.Width -= margin_x * 2.0 * rel_width;
        if (scaled_rect.Width < 0.5) return Ok; /* doesn't fit */
    }

    if (scaled_rect.Width >= 1 << 23 || scaled_rect.Width < 0.5) scaled_rect.Width = 1 << 23;
    if (scaled_rect.Height >= 1 << 23 || scaled_rect.Height < 0.5) scaled_rect.Height = 1 << 23;

    if (!(format_flags & StringFormatFlagsNoClip) &&
        scaled_rect.Width != 1 << 23 && scaled_rect.Height != 1 << 23)
    {
        /* FIXME: If only the width or only the height is 0, we should probably still clip */
        rgn = CreatePolygonRgn(corners, 4, ALTERNATE);
        SelectClipRgn(hdc, rgn);
    }

    get_font_hfont(graphics, font, format, &gdifont, NULL);
    SelectObject(hdc, gdifont);

    args.graphics = graphics;
    args.brush = brush;

    args.x = rect->X;
    args.y = rect->Y + offsety;

    args.rel_width = rel_width;
    args.rel_height = rel_height;

    GetTextMetricsW(hdc, &textmetric);
    args.ascent = textmetric.tmAscent / rel_height;

    gdip_format_string(hdc, string, length, font, &scaled_rect, format,
        draw_string_callback, &args);

    DeleteObject(rgn);
    DeleteObject(gdifont);

    RestoreDC(hdc, save_state);

    DeleteDC(temp_hdc);

    return Ok;
}

Status GdipResetClip(Graphics *graphics)
{
    TRACE("(%p)\n", graphics);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipSetInfinite(graphics->clip);
}

Status GdipResetWorldTransform(Graphics *graphics)
{
    TRACE("(%p)\n", graphics);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipSetMatrixElements(&graphics->worldtrans, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

Status GdipRestoreGraphics(Graphics *graphics, GraphicsState state)
{
    return GdipEndContainer(graphics, state);
}

Status GdipRotateWorldTransform(Graphics *graphics, REAL angle,
    MatrixOrder order)
{
    TRACE("(%p, %.2f, %d)\n", graphics, angle, order);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipRotateMatrix(&graphics->worldtrans, angle, order);
}

Status GdipSaveGraphics(Graphics *graphics, GraphicsState *state)
{
    return GdipBeginContainer2(graphics, state);
}

Status GdipBeginContainer2(Graphics *graphics,
        GraphicsContainer *state)
{
    GraphicsContainerItem *container;
    Status sts;

    TRACE("(%p, %p)\n", graphics, state);

    if(!graphics || !state)
        return InvalidParameter;

    sts = init_container(&container, graphics);
    if(sts != Ok)
        return sts;

    list_add_head(&graphics->containers, &container->entry);
    *state = graphics->contid = container->contid;

    return Ok;
}

Status GdipBeginContainer(Graphics *graphics, GDIPCONST RectF *dstrect, GDIPCONST RectF *srcrect, Unit unit, GraphicsContainer *state)
{
    FIXME("(%p, %p, %p, %d, %p): stub\n", graphics, dstrect, srcrect, unit, state);
    return NotImplemented;
}

Status GdipBeginContainerI(Graphics *graphics, GDIPCONST Rect *dstrect, GDIPCONST Rect *srcrect, Unit unit, GraphicsContainer *state)
{
    FIXME("(%p, %p, %p, %d, %p): stub\n", graphics, dstrect, srcrect, unit, state);
    return NotImplemented;
}

Status GdipComment(Graphics *graphics, UINT sizeData, GDIPCONST BYTE *data)
{
    FIXME("(%p, %d, %p): stub\n", graphics, sizeData, data);
    return NotImplemented;
}

Status GdipEndContainer(Graphics *graphics, GraphicsContainer state)
{
    Status sts;
    GraphicsContainerItem *container, *container2;

    TRACE("(%p, %x)\n", graphics, state);

    if(!graphics)
        return InvalidParameter;

    LIST_FOR_EACH_ENTRY(container, &graphics->containers, GraphicsContainerItem, entry){
        if(container->contid == state)
            break;
    }

    /* did not find a matching container */
    if(&container->entry == &graphics->containers)
        return Ok;

    sts = restore_container(graphics, container);
    if(sts != Ok)
        return sts;

    /* remove all of the containers on top of the found container */
    LIST_FOR_EACH_ENTRY_SAFE(container, container2, &graphics->containers, GraphicsContainerItem, entry){
        if(container->contid == state)
            break;
        list_remove(&container->entry);
        delete_container(container);
    }

    list_remove(&container->entry);
    delete_container(container);

    return Ok;
}

Status GdipScaleWorldTransform(Graphics *graphics, REAL sx,
    REAL sy, MatrixOrder order)
{
    TRACE("(%p, %.2f, %.2f, %d)\n", graphics, sx, sy, order);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipScaleMatrix(&graphics->worldtrans, sx, sy, order);
}

Status GdipSetClipGraphics(Graphics *graphics, Graphics *srcgraphics,
    CombineMode mode)
{
    TRACE("(%p, %p, %d)\n", graphics, srcgraphics, mode);

    if(!graphics || !srcgraphics)
        return InvalidParameter;

    return GdipCombineRegionRegion(graphics->clip, srcgraphics->clip, mode);
}

Status GdipSetCompositingMode(Graphics *graphics,
    CompositingMode mode)
{
    TRACE("(%p, %d)\n", graphics, mode);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    graphics->compmode = mode;

    return Ok;
}

Status GdipSetCompositingQuality(Graphics *graphics,
    CompositingQuality quality)
{
    TRACE("(%p, %d)\n", graphics, quality);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    graphics->compqual = quality;

    return Ok;
}

Status GdipSetInterpolationMode(Graphics *graphics,
    InterpolationMode mode)
{
    TRACE("(%p, %d)\n", graphics, mode);

    if(!graphics || mode == InterpolationModeInvalid || mode > InterpolationModeHighQualityBicubic)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (mode == InterpolationModeDefault || mode == InterpolationModeLowQuality)
        mode = InterpolationModeBilinear;

    if (mode == InterpolationModeHighQuality)
        mode = InterpolationModeHighQualityBicubic;

    graphics->interpolation = mode;

    return Ok;
}

Status GdipSetPageScale(Graphics *graphics, REAL scale)
{
    TRACE("(%p, %.2f)\n", graphics, scale);

    if(!graphics || (scale <= 0.0))
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    graphics->scale = scale;

    return Ok;
}

Status GdipSetPageUnit(Graphics *graphics, Unit unit)
{
    TRACE("(%p, %d)\n", graphics, unit);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if(unit == UnitWorld)
        return InvalidParameter;

    graphics->unit = unit;

    return Ok;
}

Status GdipSetPixelOffsetMode(Graphics *graphics, PixelOffsetMode
    mode)
{
    TRACE("(%p, %d)\n", graphics, mode);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    graphics->pixeloffset = mode;

    return Ok;
}

Status GdipSetRenderingOrigin(Graphics *graphics, INT x, INT y)
{
    static int calls;

    TRACE("(%p,%i,%i)\n", graphics, x, y);

    if (!(calls++))
        FIXME("value is unused in rendering\n");

    if (!graphics)
        return InvalidParameter;

    graphics->origin_x = x;
    graphics->origin_y = y;

    return Ok;
}

Status GdipGetRenderingOrigin(Graphics *graphics, INT *x, INT *y)
{
    TRACE("(%p,%p,%p)\n", graphics, x, y);

    if (!graphics || !x || !y)
        return InvalidParameter;

    *x = graphics->origin_x;
    *y = graphics->origin_y;

    return Ok;
}

Status GdipSetSmoothingMode(Graphics *graphics, SmoothingMode mode)
{
    TRACE("(%p, %d)\n", graphics, mode);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    graphics->smoothing = mode;

    return Ok;
}

Status GdipSetTextContrast(Graphics *graphics, UINT contrast)
{
    TRACE("(%p, %d)\n", graphics, contrast);

    if(!graphics)
        return InvalidParameter;

    graphics->textcontrast = contrast;

    return Ok;
}

Status GdipSetTextRenderingHint(Graphics *graphics,
    TextRenderingHint hint)
{
    TRACE("(%p, %d)\n", graphics, hint);

    if(!graphics || hint > TextRenderingHintClearTypeGridFit)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    graphics->texthint = hint;

    return Ok;
}

Status GdipSetWorldTransform(Graphics *graphics, Matrix *matrix)
{
    TRACE("(%p, %p)\n", graphics, matrix);

    if(!graphics || !matrix)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    TRACE("%f,%f,%f,%f,%f,%f\n",
          matrix->matrix[0], matrix->matrix[1], matrix->matrix[2],
          matrix->matrix[3], matrix->matrix[4], matrix->matrix[5]);

    graphics->worldtrans = *matrix;

    return Ok;
}

Status GdipTranslateWorldTransform(Graphics *graphics, REAL dx,
    REAL dy, MatrixOrder order)
{
    TRACE("(%p, %.2f, %.2f, %d)\n", graphics, dx, dy, order);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipTranslateMatrix(&graphics->worldtrans, dx, dy, order);
}

/*****************************************************************************
 * GdipSetClipHrgn [GDIPLUS.@]
 */
Status GdipSetClipHrgn(Graphics *graphics, HRGN hrgn, CombineMode mode)
{
    Region *region;
    Status status;

    TRACE("(%p, %p, %d)\n", graphics, hrgn, mode);

    if(!graphics)
        return InvalidParameter;

    status = GdipCreateRegionHrgn(hrgn, &region);
    if(status != Ok)
        return status;

    status = GdipSetClipRegion(graphics, region, mode);

    GdipDeleteRegion(region);
    return status;
}

Status GdipSetClipPath(Graphics *graphics, Path *path, CombineMode mode)
{
    TRACE("(%p, %p, %d)\n", graphics, path, mode);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipCombineRegionPath(graphics->clip, path, mode);
}

Status GdipSetClipRect(Graphics *graphics, REAL x, REAL y,
                                    REAL width, REAL height,
                                    CombineMode mode)
{
    RectF rect;

    TRACE("(%p, %.2f, %.2f, %.2f, %.2f, %d)\n", graphics, x, y, width, height, mode);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    rect.X = x;
    rect.Y = y;
    rect.Width  = width;
    rect.Height = height;

    return GdipCombineRegionRect(graphics->clip, &rect, mode);
}

Status GdipSetClipRectI(Graphics *graphics, INT x, INT y,
                                     INT width, INT height,
                                     CombineMode mode)
{
    TRACE("(%p, %d, %d, %d, %d, %d)\n", graphics, x, y, width, height, mode);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipSetClipRect(graphics, (REAL)x, (REAL)y, (REAL)width, (REAL)height, mode);
}

Status GdipSetClipRegion(Graphics *graphics, Region *region,
                                      CombineMode mode)
{
    TRACE("(%p, %p, %d)\n", graphics, region, mode);

    if(!graphics || !region)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipCombineRegionRegion(graphics->clip, region, mode);
}

Status GdipSetMetafileDownLevelRasterizationLimit(Metafile *metafile,
    UINT limitDpi)
{
    static int calls;

    TRACE("(%p,%u)\n", metafile, limitDpi);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status GdipDrawPolygon(Graphics *graphics,Pen *pen,GDIPCONST PointF *points,
    INT count)
{
    INT save_state;
    IPOINT *pti;

    TRACE("(%p, %p, %d)\n", graphics, points, count);

    if(!graphics || !pen || count<=0)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (!graphics->hdc)
    {
        FIXME("graphics object has no HDC\n");
        return Ok;
    }

    pti = GdipAlloc(sizeof(IPOINT) * count);

    save_state = prepare_dc(graphics, pen);
    SelectObject(graphics->hdc, GetStockObject(NULL_BRUSH));

    transform_and_round_points(graphics, pti, (PointF*)points, count);
    Polygon(graphics->hdc, pti, count);

    restore_dc(graphics, save_state);
    GdipFree(pti);

    return Ok;
}

Status GdipDrawPolygonI(Graphics *graphics,Pen *pen,GDIPCONST Point *points,
    INT count)
{
    Status ret;
    PointF *ptf;
    INT i;

    TRACE("(%p, %p, %p, %d)\n", graphics, pen, points, count);

    if(count<=0)    return InvalidParameter;
    ptf = GdipAlloc(sizeof(PointF) * count);

    for(i = 0;i < count; i++){
        ptf[i].X = (REAL)points[i].X;
        ptf[i].Y = (REAL)points[i].Y;
    }

    ret = GdipDrawPolygon(graphics,pen,ptf,count);
    GdipFree(ptf);

    return ret;
}

Status GdipGetDpiX(Graphics *graphics, REAL* dpi)
{
    TRACE("(%p, %p)\n", graphics, dpi);

    if(!graphics || !dpi)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *dpi = graphics->xres;
    return Ok;
}

Status GdipGetDpiY(Graphics *graphics, REAL* dpi)
{
    TRACE("(%p, %p)\n", graphics, dpi);

    if(!graphics || !dpi)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    *dpi = graphics->yres;
    return Ok;
}

Status GdipMultiplyWorldTransform(Graphics *graphics, GDIPCONST Matrix *matrix,
    MatrixOrder order)
{
    Matrix m;
    Status ret;

    TRACE("(%p, %p, %d)\n", graphics, matrix, order);

    if(!graphics || !matrix)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    m = graphics->worldtrans;

    ret = GdipMultiplyMatrix(&m, matrix, order);
    if(ret == Ok)
        graphics->worldtrans = m;

    return ret;
}

/* Color used to fill bitmaps so we can tell which parts have been drawn over by gdi32. */
static const COLOR DC_BACKGROUND_KEY = 0x0c0b0d;

Status GdipGetDC(Graphics *graphics, HDC *hdc)
{
    Status stat=Ok;

    TRACE("(%p, %p)\n", graphics, hdc);

    if(!graphics || !hdc)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if (graphics->image && graphics->image->type == ImageTypeMetafile)
    {
        stat = METAFILE_GetDC((Metafile*)graphics->image, hdc);
    }
    else if (!graphics->hdc || graphics->alpha_hdc ||
        (graphics->image && graphics->image->type == ImageTypeBitmap && ((Bitmap*)graphics->image)->format & PixelFormatAlpha))
    {
        /* Create a fake HDC and fill it with a constant color. */
        HDC temp_hdc;
        HBITMAP hbitmap;
        RectF bounds;
        BITMAPINFOHEADER bmih;
        int i;

        stat = get_graphics_bounds(graphics, &bounds);
        if (stat != Ok)
            return stat;

        graphics->temp_hbitmap_width = bounds.Width;
        graphics->temp_hbitmap_height = bounds.Height;

        bmih.biSize = sizeof(bmih);
        bmih.biWidth = graphics->temp_hbitmap_width;
        bmih.biHeight = -graphics->temp_hbitmap_height;
        bmih.biPlanes = 1;
        bmih.biBitCount = 32;
        bmih.biCompression = BI_RGB;
        bmih.biSizeImage = 0;
        bmih.biXPelsPerMeter = 0;
        bmih.biYPelsPerMeter = 0;
        bmih.biClrUsed = 0;
        bmih.biClrImportant = 0;

        hbitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bmih, DIB_RGB_COLORS,
            (void**)&graphics->temp_bits, NULL, 0);
        if (!hbitmap)
            return GenericError;

        temp_hdc = CreateCompatibleDC(0);
        if (!temp_hdc)
        {
            DeleteObject(hbitmap);
            return GenericError;
        }

        for (i=0; i<(graphics->temp_hbitmap_width * graphics->temp_hbitmap_height); i++)
            ((DWORD*)graphics->temp_bits)[i] = DC_BACKGROUND_KEY;

        SelectObject(temp_hdc, hbitmap);

        graphics->temp_hbitmap = hbitmap;
        *hdc = graphics->temp_hdc = temp_hdc;
    }
    else
    {
        *hdc = graphics->hdc;
    }

    if (stat == Ok)
        graphics->busy = TRUE;

    return stat;
}

Status GdipReleaseDC(Graphics *graphics, HDC hdc)
{
    Status stat=Ok;

    TRACE("(%p, %p)\n", graphics, hdc);

    if(!graphics || !hdc || !graphics->busy)
        return InvalidParameter;

    if (graphics->image && graphics->image->type == ImageTypeMetafile)
    {
        stat = METAFILE_ReleaseDC((Metafile*)graphics->image, hdc);
    }
    else if (graphics->temp_hdc == hdc)
    {
        DWORD* pos;
        int i;

        /* Find the pixels that have changed, and mark them as opaque. */
        pos = (DWORD*)graphics->temp_bits;
        for (i=0; i<(graphics->temp_hbitmap_width * graphics->temp_hbitmap_height); i++)
        {
            if (*pos != DC_BACKGROUND_KEY)
            {
                *pos |= 0xff000000;
            }
            pos++;
        }

        /* Write the changed pixels to the real target. */
        alpha_blend_pixels(graphics, 0, 0, graphics->temp_bits,
            graphics->temp_hbitmap_width, graphics->temp_hbitmap_height,
            graphics->temp_hbitmap_width * 4);

        /* Clean up. */
        DeleteDC(graphics->temp_hdc);
        DeleteObject(graphics->temp_hbitmap);
        graphics->temp_hdc = NULL;
        graphics->temp_hbitmap = NULL;
    }
    else if (hdc != graphics->hdc)
    {
        stat = InvalidParameter;
    }

    if (stat == Ok)
        graphics->busy = FALSE;

    return stat;
}

Status GdipGetClip(Graphics *graphics, Region *region)
{
    Region *clip;
    Status status;

    TRACE("(%p, %p)\n", graphics, region);

    if(!graphics || !region)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    if((status = GdipCloneRegion(graphics->clip, &clip)) != Ok)
        return status;

    /* free everything except root node and header */
    delete_element(&region->node);
    memcpy(region, clip, sizeof(Region));
    GdipFree(clip);

    return Ok;
}

HPALETTE  GdipCreateHalftonePalette(void)
{
    static int calls;

    TRACE("\n");

    if (!calls++)
      FIXME("stub\n");

    return NULL;
}

/*****************************************************************************
 * GdipTranslateClip [GDIPLUS.@]
 */
Status GdipTranslateClip(Graphics *graphics, REAL dx, REAL dy)
{
    TRACE("(%p, %.2f, %.2f)\n", graphics, dx, dy);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipTranslateRegion(graphics->clip, dx, dy);
}

/*****************************************************************************
 * GdipTranslateClipI [GDIPLUS.@]
 */
Status GdipTranslateClipI(Graphics *graphics, INT dx, INT dy)
{
    TRACE("(%p, %d, %d)\n", graphics, dx, dy);

    if(!graphics)
        return InvalidParameter;

    if(graphics->busy)
        return ObjectBusy;

    return GdipTranslateRegion(graphics->clip, (REAL)dx, (REAL)dy);
}


/*****************************************************************************
 * GdipMeasureDriverString [GDIPLUS.@]
 */
Status GdipMeasureDriverString(Graphics *graphics, GDIPCONST UINT16 *text, INT length,
                                            GDIPCONST Font *font, GDIPCONST PointF *positions,
                                            INT flags, GDIPCONST Matrix *matrix, RectF *boundingBox)
{
    static const INT unsupported_flags = ~(DriverStringOptionsCmapLookup|DriverStringOptionsRealizedAdvance);
    HFONT hfont;
    HDC hdc;
    REAL min_x, min_y, max_x, max_y, x, y;
    int i;
    TEXTMETRICW textmetric;
    const WORD *glyph_indices;
    WORD *dynamic_glyph_indices=NULL;
    REAL rel_width, rel_height, ascent, descent;
    PointF pt[3];

    TRACE("(%p %p %d %p %p %d %p %p)\n", graphics, text, length, font, positions, flags, matrix, boundingBox);

    if (!graphics || !text || !font || !positions || !boundingBox)
        return InvalidParameter;

    if (length == -1)
        length = strlenW(text);

    if (length == 0)
    {
        boundingBox->X = 0.0;
        boundingBox->Y = 0.0;
        boundingBox->Width = 0.0;
        boundingBox->Height = 0.0;
    }

    if (flags & unsupported_flags)
        FIXME("Ignoring flags %x\n", flags & unsupported_flags);

    get_font_hfont(graphics, font, NULL, &hfont, matrix);

    hdc = CreateCompatibleDC(0);
    SelectObject(hdc, hfont);

    GetTextMetricsW(hdc, &textmetric);

    pt[0].X = 0.0;
    pt[0].Y = 0.0;
    pt[1].X = 1.0;
    pt[1].Y = 0.0;
    pt[2].X = 0.0;
    pt[2].Y = 1.0;
    if (matrix)
    {
        Matrix xform = *matrix;
        GdipTransformMatrixPoints(&xform, pt, 3);
    }
    GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, pt, 3);
    rel_width = sqrt((pt[1].Y-pt[0].Y)*(pt[1].Y-pt[0].Y)+
                     (pt[1].X-pt[0].X)*(pt[1].X-pt[0].X));
    rel_height = sqrt((pt[2].Y-pt[0].Y)*(pt[2].Y-pt[0].Y)+
                      (pt[2].X-pt[0].X)*(pt[2].X-pt[0].X));

    if (flags & DriverStringOptionsCmapLookup)
    {
        glyph_indices = dynamic_glyph_indices = GdipAlloc(sizeof(WORD) * length);
        if (!glyph_indices)
        {
            DeleteDC(hdc);
            DeleteObject(hfont);
            return OutOfMemory;
        }

        GetGlyphIndicesW(hdc, text, length, dynamic_glyph_indices, 0);
    }
    else
        glyph_indices = text;

    min_x = max_x = x = positions[0].X;
    min_y = max_y = y = positions[0].Y;

    ascent = textmetric.tmAscent / rel_height;
    descent = textmetric.tmDescent / rel_height;

    for (i=0; i<length; i++)
    {
        int char_width;
        ABC abc;

        if (!(flags & DriverStringOptionsRealizedAdvance))
        {
            x = positions[i].X;
            y = positions[i].Y;
        }

        GetCharABCWidthsW(hdc, glyph_indices[i], glyph_indices[i], &abc);
        char_width = abc.abcA + abc.abcB + abc.abcC;

        if (min_y > y - ascent) min_y = y - ascent;
        if (max_y < y + descent) max_y = y + descent;
        if (min_x > x) min_x = x;

        x += char_width / rel_width;

        if (max_x < x) max_x = x;
    }

    GdipFree(dynamic_glyph_indices);
    DeleteDC(hdc);
    DeleteObject(hfont);

    boundingBox->X = min_x;
    boundingBox->Y = min_y;
    boundingBox->Width = max_x - min_x;
    boundingBox->Height = max_y - min_y;

    return Ok;
}

static Status GDI32_GdipDrawDriverString(Graphics *graphics, GDIPCONST UINT16 *text, INT length,
                                           GDIPCONST Font *font, GDIPCONST StringFormat *format,
                                           GDIPCONST Brush *brush, GDIPCONST PointF *positions,
                                           INT flags, GDIPCONST Matrix *matrix)
{
    static const INT unsupported_flags = ~(DriverStringOptionsRealizedAdvance|DriverStringOptionsCmapLookup);
    INT save_state;
    PointF pt;
    HFONT hfont;
    UINT eto_flags=0;

    if (flags & unsupported_flags)
        FIXME("Ignoring flags %x\n", flags & unsupported_flags);

    if (!(flags & DriverStringOptionsCmapLookup))
        eto_flags |= ETO_GLYPH_INDEX;

    save_state = SaveDC(graphics->hdc);
    SetBkMode(graphics->hdc, TRANSPARENT);
    SetTextColor(graphics->hdc, get_gdi_brush_color(brush));

    pt = positions[0];
    GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, &pt, 1);

    get_font_hfont(graphics, font, format, &hfont, matrix);
    SelectObject(graphics->hdc, hfont);

    SetTextAlign(graphics->hdc, TA_BASELINE|TA_LEFT);

    ExtTextOutW(graphics->hdc, gdip_round(pt.X), gdip_round(pt.Y), eto_flags, NULL, text, length, NULL);

    RestoreDC(graphics->hdc, save_state);

    DeleteObject(hfont);

    return Ok;
}

static Status SOFTWARE_GdipDrawDriverString(Graphics *graphics, GDIPCONST UINT16 *text, INT length,
                                        GDIPCONST Font *font, GDIPCONST StringFormat *format,
                                        GDIPCONST Brush *brush, GDIPCONST PointF *positions,
                                        INT flags, GDIPCONST Matrix *matrix)
{
    static const INT unsupported_flags = ~(DriverStringOptionsCmapLookup|DriverStringOptionsRealizedAdvance);
    Status stat;
    PointF *real_positions, real_position;
    IPOINT *pti;
    HFONT hfont;
    HDC hdc;
    int min_x=INT_MAX, min_y=INT_MAX, max_x=INT_MIN, max_y=INT_MIN, i, x, y;
    DWORD max_glyphsize=0;
    GLYPHMETRICS glyphmetrics;
    static const MAT2 identity = {{0,1}, {0,0}, {0,0}, {0,1}};
    BYTE *glyph_mask;
    BYTE *text_mask;
    int text_mask_stride;
    BYTE *pixel_data;
    int pixel_data_stride;
    Rect pixel_area;
    UINT ggo_flags = GGO_GRAY8_BITMAP;

    if (length <= 0)
        return Ok;

    if (!(flags & DriverStringOptionsCmapLookup))
        ggo_flags |= GGO_GLYPH_INDEX;

    if (flags & unsupported_flags)
        FIXME("Ignoring flags %x\n", flags & unsupported_flags);

    pti = GdipAlloc(sizeof(IPOINT) * length);
    if (!pti)
        return OutOfMemory;

    if (flags & DriverStringOptionsRealizedAdvance)
    {
        real_position = positions[0];

        transform_and_round_points(graphics, pti, &real_position, 1);
    }
    else
    {
        real_positions = GdipAlloc(sizeof(PointF) * length);
        if (!real_positions)
        {
            GdipFree(pti);
            return OutOfMemory;
        }

        memcpy(real_positions, positions, sizeof(PointF) * length);

        transform_and_round_points(graphics, pti, real_positions, length);

        GdipFree(real_positions);
    }

    get_font_hfont(graphics, font, format, &hfont, matrix);

    hdc = CreateCompatibleDC(0);
    SelectObject(hdc, hfont);

    /* Get the boundaries of the text to be drawn */
    for (i=0; i<length; i++)
    {
        DWORD glyphsize;
        int left, top, right, bottom;

        glyphsize = GetGlyphOutlineW(hdc, text[i], ggo_flags,
            &glyphmetrics, 0, NULL, &identity);

        if (glyphsize == GDI_ERROR)
        {
            ERR("GetGlyphOutlineW failed\n");
            GdipFree(pti);
            DeleteDC(hdc);
            DeleteObject(hfont);
            return GenericError;
        }

        if (glyphsize > max_glyphsize)
            max_glyphsize = glyphsize;

        left = pti[i].x + glyphmetrics.gmptGlyphOrigin.x;
        top = pti[i].y - glyphmetrics.gmptGlyphOrigin.y;
        right = pti[i].x + glyphmetrics.gmptGlyphOrigin.x + glyphmetrics.gmBlackBoxX;
        bottom = pti[i].y - glyphmetrics.gmptGlyphOrigin.y + glyphmetrics.gmBlackBoxY;

        if (left < min_x) min_x = left;
        if (top < min_y) min_y = top;
        if (right > max_x) max_x = right;
        if (bottom > max_y) max_y = bottom;

        if (i+1 < length && (flags & DriverStringOptionsRealizedAdvance) == DriverStringOptionsRealizedAdvance)
        {
            pti[i+1].x = pti[i].x + glyphmetrics.gmCellIncX;
            pti[i+1].y = pti[i].y + glyphmetrics.gmCellIncY;
        }
    }

    glyph_mask = GdipAlloc(max_glyphsize);
    text_mask = GdipAlloc((max_x - min_x) * (max_y - min_y));
    text_mask_stride = max_x - min_x;

    if (!(glyph_mask && text_mask))
    {
        GdipFree(glyph_mask);
        GdipFree(text_mask);
        GdipFree(pti);
        DeleteDC(hdc);
        DeleteObject(hfont);
        return OutOfMemory;
    }

    /* Generate a mask for the text */
    for (i=0; i<length; i++)
    {
        int left, top, stride;

        GetGlyphOutlineW(hdc, text[i], ggo_flags,
            &glyphmetrics, max_glyphsize, glyph_mask, &identity);

        left = pti[i].x + glyphmetrics.gmptGlyphOrigin.x;
        top = pti[i].y - glyphmetrics.gmptGlyphOrigin.y;
        stride = (glyphmetrics.gmBlackBoxX + 3) & (~3);

        for (y=0; y<glyphmetrics.gmBlackBoxY; y++)
        {
            BYTE *glyph_val = glyph_mask + y * stride;
            BYTE *text_val = text_mask + (left - min_x) + (top - min_y + y) * text_mask_stride;
            for (x=0; x<glyphmetrics.gmBlackBoxX; x++)
            {
                *text_val = MIN(64, *text_val + *glyph_val);
                glyph_val++;
                text_val++;
            }
        }
    }

    GdipFree(pti);
    DeleteDC(hdc);
    DeleteObject(hfont);
    GdipFree(glyph_mask);

    /* get the brush data */
    pixel_data = GdipAlloc(4 * (max_x - min_x) * (max_y - min_y));
    if (!pixel_data)
    {
        GdipFree(text_mask);
        return OutOfMemory;
    }

    pixel_area.X = min_x;
    pixel_area.Y = min_y;
    pixel_area.Width = max_x - min_x;
    pixel_area.Height = max_y - min_y;
    pixel_data_stride = pixel_area.Width * 4;

    stat = brush_fill_pixels(graphics, (Brush*)brush, (DWORD*)pixel_data, &pixel_area, pixel_area.Width);
    if (stat != Ok)
    {
        GdipFree(text_mask);
        GdipFree(pixel_data);
        return stat;
    }

    /* multiply the brush data by the mask */
    for (y=0; y<pixel_area.Height; y++)
    {
        BYTE *text_val = text_mask + text_mask_stride * y;
        BYTE *pixel_val = pixel_data + pixel_data_stride * y + 3;
        for (x=0; x<pixel_area.Width; x++)
        {
            *pixel_val = (*pixel_val) * (*text_val) / 64;
            text_val++;
            pixel_val+=4;
        }
    }

    GdipFree(text_mask);

    /* draw the result */
    stat = alpha_blend_pixels(graphics, min_x, min_y, pixel_data, pixel_area.Width,
        pixel_area.Height, pixel_data_stride);

    GdipFree(pixel_data);

    return stat;
}

static Status draw_driver_string(Graphics *graphics, GDIPCONST UINT16 *text, INT length,
                                   GDIPCONST Font *font, GDIPCONST StringFormat *format,
                                   GDIPCONST Brush *brush, GDIPCONST PointF *positions,
                                   INT flags, GDIPCONST Matrix *matrix)
{
    Status stat = NotImplemented;

    if (length == -1)
        length = strlenW(text);

    if (graphics->hdc && !graphics->alpha_hdc &&
        ((flags & DriverStringOptionsRealizedAdvance) || length <= 1) &&
        brush->bt == BrushTypeSolidColor &&
        (((SolidFill*)brush)->color & 0xff000000) == 0xff000000)
        stat = GDI32_GdipDrawDriverString(graphics, text, length, font, format,
                                          brush, positions, flags, matrix);
    if (stat == NotImplemented)
        stat = SOFTWARE_GdipDrawDriverString(graphics, text, length, font, format,
                                             brush, positions, flags, matrix);
    return stat;
}

/*****************************************************************************
 * GdipDrawDriverString [GDIPLUS.@]
 */
Status GdipDrawDriverString(Graphics *graphics, GDIPCONST UINT16 *text, INT length,
                                         GDIPCONST Font *font, GDIPCONST Brush *brush,
                                         GDIPCONST PointF *positions, INT flags,
                                         GDIPCONST Matrix *matrix )
{
    TRACE("(%p %s %p %p %p %d %p)\n", graphics, debugstr_wn(text, length), font, brush, positions, flags, matrix);

    if (!graphics || !text || !font || !brush || !positions)
        return InvalidParameter;

    return draw_driver_string(graphics, text, length, font, NULL,
                              brush, positions, flags, matrix);
}

Status GdipRecordMetafileStream(IStream *stream, HDC hdc, EmfType type, GDIPCONST Rect *frameRect,
                                        MetafileFrameUnit frameUnit, GDIPCONST WCHAR *desc, Metafile **metafile)
{
    FIXME("(%p %p %d %p %d %p %p): stub\n", stream, hdc, type, frameRect, frameUnit, desc, metafile);
    return NotImplemented;
}

/*****************************************************************************
 * GdipIsVisibleClipEmpty [GDIPLUS.@]
 */
Status GdipIsVisibleClipEmpty(Graphics *graphics, BOOL *res)
{
    Status stat;
    Region* rgn;

    TRACE("(%p, %p)\n", graphics, res);

    if((stat = GdipCreateRegion(&rgn)) != Ok)
        return stat;

    if((stat = get_visible_clip_region(graphics, rgn)) != Ok)
        goto cleanup;

    stat = GdipIsEmptyRegion(rgn, graphics, res);

cleanup:
    GdipDeleteRegion(rgn);
    return stat;
}

Status GdipResetPageTransform(Graphics *graphics)
{
    static int calls;

    TRACE("(%p) stub\n", graphics);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}
