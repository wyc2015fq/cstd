
/*
    Unix stuff
    Code from http://www.johndcook.com/blog/2009/01/19/stand-alone-error-function-erf/
*/
double erf(double x)
{
    const double a1 =  0.254829592;
    const double a2 = -0.284496736;
    const double a3 =  1.421413741;
    const double a4 = -1.453152027;
    const double a5 =  1.061405429;
    const double p  =  0.3275911;
    double t, y, sign;

    /* Save the sign of x */
    sign = 1;
    if (x < 0)
        sign = -1;
    x = abs(x);

    /* A & S 7.1.26 */
    t = 1.0/(1.0 + p*x);
    y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return sign*y;
}

/******************************************************************************
 * GdipCreateHatchBrush [GDIPLUS.@]
 */
Status  GdipCreateHatchBrush(HatchStyle hatchstyle, ARGB forecol, ARGB backcol, Brush *brush)
{
    Hatch* hatch = &brush->hatch;
    TRACE("(%d, %d, %d, %p)\n", hatchstyle, forecol, backcol, brush);

    if(!brush)  return InvalidParameter;

    if (!brush) return OutOfMemory;

    (brush)->bt = BrushTypeHatchFill;
    (hatch)->forecol = forecol;
    (hatch)->backcol = backcol;
    (hatch)->hatchstyle = hatchstyle;
    TRACE("<-- %p\n", *brush);

    return Ok;
}

static const uchar HatchBrushes[][8] = {
#define HATCHSTYLE_DEF(data0, data1, data2, data3, data4, data5, data6, data7, name, str) {data0, data1, data2, data3, data4, data5, data6, data7},
  HATCHSTYLE_DEF_DEF(HATCHSTYLE_DEF)
#undef HATCHSTYLE_DEF
};

Status get_hatch_data(HatchStyle hatchstyle, const uchar **result)
{
    if (hatchstyle < sizeof(HatchBrushes) / sizeof(HatchBrushes[0]))
    {
        *result = HatchBrushes[hatchstyle];
        return Ok;
    }
    else
        return NotImplemented;
}

/******************************************************************************
 * GdipCreateLineBrush [GDIPLUS.@]
 */
Status  GdipCreateLineBrush(GDIPCONST PointF* startpoint,
    GDIPCONST PointF* endpoint, ARGB startcolor, ARGB endcolor,
    WrapMode wrap, Brush* brush)
{
  LineGradient* line = &brush->line;
    TRACE("(%s, %s, %x, %x, %d, %p)\n", debugstr_pointf(startpoint),
          debugstr_pointf(endpoint), startcolor, endcolor, wrap, line);

    if(!line || !startpoint || !endpoint || wrap == WrapModeClamp)
        return InvalidParameter;

    if (startpoint->X == endpoint->X && startpoint->Y == endpoint->Y)
        return OutOfMemory;

    if(!line)  return OutOfMemory;

    brush->bt = BrushTypeLinearGradient;

    line->startpoint.X = startpoint->X;
    line->startpoint.Y = startpoint->Y;
    line->endpoint.X = endpoint->X;
    line->endpoint.Y = endpoint->Y;
    line->startcolor = startcolor;
    line->endcolor = endcolor;
    line->wrap = wrap;
    line->gamma = FALSE;

    line->rect.X = (startpoint->X < endpoint->X ? startpoint->X: endpoint->X);
    line->rect.Y = (startpoint->Y < endpoint->Y ? startpoint->Y: endpoint->Y);
    line->rect.Width  = fabs(startpoint->X - endpoint->X);
    line->rect.Height = fabs(startpoint->Y - endpoint->Y);

    if (line->rect.Width == 0)
    {
        line->rect.X -= line->rect.Height / 2.0f;
        line->rect.Width = line->rect.Height;
    }
    else if (line->rect.Height == 0)
    {
        line->rect.Y -= line->rect.Width / 2.0f;
        line->rect.Height = line->rect.Width;
    }

    line->blendcount = 1;
    MYREALLOC(line->blendfac, 1);
    MYREALLOC(line->blendpos, 1);

    if (!line->blendfac || !line->blendpos)
    {
        FREE(line->blendfac);
        FREE(line->blendpos);
        return OutOfMemory;
    }

    line->blendfac[0] = 1.0f;
    line->blendpos[0] = 1.0f;

    line->pblendcolor = NULL;
    line->pblendpos = NULL;
    line->pblendcount = 0;

    TRACE("<-- %p\n", line);

    return Ok;
}

Status  GdipCreateLineBrushI(GDIPCONST Point* startpoint,
    GDIPCONST Point* endpoint, ARGB startcolor, ARGB endcolor,
    WrapMode wrap, Brush* brush)
{
    PointF stF;
    PointF endF;

    TRACE("(%p, %p, %x, %x, %d, %p)\n", startpoint, endpoint,
          startcolor, endcolor, wrap, brush);

    if(!startpoint || !endpoint)
        return InvalidParameter;

    stF.X  = (REAL)startpoint->X;
    stF.Y  = (REAL)startpoint->Y;
    endF.X = (REAL)endpoint->X;
    endF.Y = (REAL)endpoint->Y;

    return GdipCreateLineBrush(&stF, &endF, startcolor, endcolor, wrap, brush);
}

Status  GdipCreateLineBrushFromRect(GDIPCONST RectF* rect,
    ARGB startcolor, ARGB endcolor, LinearGradientMode mode, WrapMode wrap,
    Brush* brush)
{
    PointF start, end;
    Status stat;

    TRACE("(%p, %x, %x, %d, %d, %p)\n", rect, startcolor, endcolor, mode,
          wrap, brush);

    if(!brush || !rect)
        return InvalidParameter;

    switch (mode)
    {
    case LinearGradientModeHorizontal:
        start.X = rect->X;
        start.Y = rect->Y;
        end.X = rect->X + rect->Width;
        end.Y = rect->Y;
        break;
    case LinearGradientModeVertical:
        start.X = rect->X;
        start.Y = rect->Y;
        end.X = rect->X;
        end.Y = rect->Y + rect->Height;
        break;
    case LinearGradientModeForwardDiagonal:
        start.X = rect->X;
        start.Y = rect->Y;
        end.X = rect->X + rect->Width;
        end.Y = rect->Y + rect->Height;
        break;
    case LinearGradientModeBackwardDiagonal:
        start.X = rect->X + rect->Width;
        start.Y = rect->Y;
        end.X = rect->X;
        end.Y = rect->Y + rect->Height;
        break;
    default:
        return InvalidParameter;
    }

    stat = GdipCreateLineBrush(&start, &end, startcolor, endcolor, wrap, brush);

    if (stat == Ok)
        brush->line.rect = *rect;

    return stat;
}

Status  GdipCreateLineBrushFromRectI(GDIPCONST Rect* rect,
    ARGB startcolor, ARGB endcolor, LinearGradientMode mode, WrapMode wrap,
    Brush *brush)
{
    RectF rectF;

    TRACE("(%p, %x, %x, %d, %d, %p)\n", rect, startcolor, endcolor, mode,
          wrap, brush);

    rectF.X      = (REAL) rect->X;
    rectF.Y      = (REAL) rect->Y;
    rectF.Width  = (REAL) rect->Width;
    rectF.Height = (REAL) rect->Height;

    return GdipCreateLineBrushFromRect(&rectF, startcolor, endcolor, mode, wrap, brush);
}

/******************************************************************************
 * GdipCreateLineBrushFromRectWithAngle [GDIPLUS.@]
 */
Status  GdipCreateLineBrushFromRectWithAngle(GDIPCONST RectF* rect,
    ARGB startcolor, ARGB endcolor, REAL angle, BOOL isAngleScalable, WrapMode wrap,
    Brush* brush)
{
    Status stat;
    LinearGradientMode mode;
    REAL width, height, exofs, eyofs;
    REAL sin_angle, cos_angle, sin_cos_angle;
    LineGradient* line = &brush->line;

    TRACE("(%p, %x, %x, %.2f, %d, %d, %p)\n", rect, startcolor, endcolor, angle, isAngleScalable,
          wrap, brush);

    sin_angle = sinf(deg2rad(angle));
    cos_angle = cosf(deg2rad(angle));
    sin_cos_angle = sin_angle * cos_angle;

    if (isAngleScalable)
    {
        width = height = 1.0;
    }
    else
    {
        width = rect->Width;
        height = rect->Height;
    }

    if (sin_cos_angle >= 0)
        mode = LinearGradientModeForwardDiagonal;
    else
        mode = LinearGradientModeBackwardDiagonal;

    stat = GdipCreateLineBrushFromRect(rect, startcolor, endcolor, mode, wrap, brush);

    if (stat == Ok)
    {
        if (sin_cos_angle >= 0)
        {
            exofs = width * sin_cos_angle + height * cos_angle * cos_angle;
            eyofs = width * sin_angle * sin_angle + height * sin_cos_angle;
        }
        else
        {
            exofs = width * sin_angle * sin_angle + height * sin_cos_angle;
            eyofs = -width * sin_cos_angle + height * sin_angle * sin_angle;
        }

        if (isAngleScalable)
        {
            exofs = exofs * rect->Width;
            eyofs = eyofs * rect->Height;
        }

        if (sin_angle >= 0)
        {
            line->endpoint.X = rect->X + exofs;
            line->endpoint.Y = rect->Y + eyofs;
        }
        else
        {
            line->endpoint.X = line->startpoint.X;
            line->endpoint.Y = line->startpoint.Y;
            line->startpoint.X = rect->X + exofs;
            line->startpoint.Y = rect->Y + eyofs;
        }
    }

    return stat;
}

Status  GdipCreateLineBrushFromRectWithAngleI(GDIPCONST Rect* rect,
    ARGB startcolor, ARGB endcolor, REAL angle, BOOL isAngleScalable, WrapMode wrap,
    Brush *brush)
{
    TRACE("(%p, %x, %x, %.2f, %d, %d, %p)\n", rect, startcolor, endcolor, angle, isAngleScalable,
          wrap, brush);

    return GdipCreateLineBrushFromRectI(rect, startcolor, endcolor, LinearGradientModeForwardDiagonal,
                                        wrap, brush);
}

static Status create_path_gradient(const Path *path, ARGB centercolor, Brush* brush)
{
    PathGradient *grad;
    RectF bounds;

    if(!path || !brush)
        return InvalidParameter;
    
    grad = &brush->grad;

    if (path->pathdata.Count < 2)
        return OutOfMemory;

    PathGetWorldBounds(path, &bounds, NULL, NULL);
    GdipSetMatrixElements(&grad->transform, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

    MYREALLOC(grad->blendfac, 1);
    MYREALLOC(grad->blendpos, 1);
    MYREALLOC(grad->surroundcolors, 1);
    if(!grad->blendfac || !grad->blendpos || !grad->surroundcolors){
        FREE(grad->blendfac);
        FREE(grad->blendpos);
        FREE(grad->surroundcolors);
        return OutOfMemory;
    }
    grad->blendfac[0] = 1.0;
    grad->blendpos[0] = 1.0;
    grad->blendcount  = 1;

    brush->bt = BrushTypePathGradient;
    grad->centercolor = centercolor;
    grad->wrap = WrapModeClamp;
    grad->gamma = FALSE;
    /* FIXME: this should be set to the "centroid" of the path by default */
    grad->center.X = bounds.X + bounds.Width / 2;
    grad->center.Y = bounds.Y + bounds.Height / 2;
    grad->focus.X = 0.0;
    grad->focus.Y = 0.0;
    grad->surroundcolors[0] = 0xffffffff;
    grad->surroundcolorcount = 1;

    TRACE("<-- %p\n", *grad);

    return Ok;
}

Status  PathCreateGradient(GDIPCONST PointF* points,
    INT count, WrapMode wrap, Brush* brush)
{
  Status stat;
  PathGradient *grad = &brush->grad;
    Path *path = grad->path;

    TRACE("(%p, %d, %d, %p)\n", points, count, wrap, grad);

    if(!grad)
        return InvalidParameter;

    if(!points || count <= 0)
        return OutOfMemory;

    stat = PathCreate(FillModeAlternate, path);

    if (stat == Ok)
    {
        stat = PathAddLine2(path, points, count);

        if (stat == Ok)
            stat = create_path_gradient(path, 0xff000000, brush);

        if (stat != Ok)
            PathDelete(path);
    }

    if (stat == Ok)
        grad->wrap = wrap;

    return stat;
}

Status  PathCreateGradientI(GDIPCONST Point* points,
    INT count, WrapMode wrap, Brush* brush)
{
  Status stat;
  PathGradient *grad = &brush->grad;
    Path *path = grad->path;

    TRACE("(%p, %d, %d, %p)\n", points, count, wrap, grad);

    if(!grad)
        return InvalidParameter;

    if(!points || count <= 0)
        return OutOfMemory;

    stat = PathCreate(FillModeAlternate, path);

    if (stat == Ok)
    {
        stat = PathAddLine2I(path, points, count);

        if (stat == Ok)
            stat = create_path_gradient(path, 0xff000000, brush);

        if (stat != Ok)
            PathDelete(path);
    }

    if (stat == Ok)
        grad->wrap = wrap;

    return stat;
}

/******************************************************************************
 * PathCreateGradientFromPath [GDIPLUS.@]
 */
Status  PathCreateGradientFromPath(GDIPCONST Path* path, Brush* brush)
{
  Status stat;
  PathGradient *grad = &brush->grad;
    Path *new_path = grad->path;

    TRACE("(%p, %p)\n", path, brush);

    if(!brush)
        return InvalidParameter;

    if (!path)
        return OutOfMemory;

    stat = GdipClonePath(path, new_path);

    if (stat == Ok)
    {
        stat = create_path_gradient(new_path, 0xffffffff, brush);

        if (stat != Ok)
            PathDelete(new_path);
    }

    return stat;
}

/******************************************************************************
 * GdipCreateSolidFill [GDIPLUS.@]
 */
Status  GdipCreateSolidFill(ARGB color, Brush* brush)
{
    SolidFill* sf = &brush->solid;
    TRACE("(%x, %p)\n", color, brush);

    if(!sf)  return InvalidParameter;

    brush->bt = BrushTypeSolidColor;
    sf->color = color;

    TRACE("<-- %p\n", *sf);

    return Ok;
}

/******************************************************************************
 * GdipCreateTextureIA [GDIPLUS.@]
 *
 * FIXME: imageattr ignored
 */
Status  GdipCreateTextureIA(Image *image, ImageAttributes *imageattr, REAL x, REAL y, REAL width, REAL height, Brush* brush)
{
    Texture *texture = &brush->texture;

    TRACE("(%p, %p, %.2f, %.2f, %.2f, %.2f, %p)\n", image, imageattr, x, y, width, height,
           texture);

    if(!image || !texture || x < 0.0 || y < 0.0 || width < 0.0 || height < 0.0)
        return InvalidParameter;

    RectFSet(&texture->rectf, x, y, width, height);
    GdipSetMatrixElements(&texture->transform, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    texture->imageattributes = imageattr;

    brush->bt = BrushTypeTextureFill;
    texture->image = image;

    return Ok;
}

/******************************************************************************
 * GdipCreateTexture [GDIPLUS.@]
 *
 * PARAMS
 *  image       [I] image to use
 *  wrapmode    [I] optional
 *  texture     [O] pointer to the resulting texturebrush
 *
 * RETURNS
 *  SUCCESS: Ok
 *  FAILURE: element of Status
 */
Status  GdipCreateTexture(Image *image, WrapMode wrapmode, Brush* brush)
{
    Texture *texture = &brush->texture;
    Status stat;

    TRACE("%p, %d %p\n", image, wrapmode, texture);

    if (!(image && texture))
        return InvalidParameter;

    texture->image = image;
    texture->wrap = wrapmode;
    stat = GdipCreateTextureIA(image, NULL, 0, 0, image->width, image->height, brush);

    return stat;
}

/******************************************************************************
 * GdipCreateTexture2 [GDIPLUS.@]
 */
Status  GdipCreateTexture2(Image *image, WrapMode wrapmode,
        REAL x, REAL y, REAL width, REAL height, Brush* brush)
{
    Texture *texture = &brush->texture;
    Status stat;

    TRACE("%p %d %f %f %f %f %p\n", image, wrapmode,
            x, y, width, height, texture);
    
    
    texture->wrap = wrapmode;
    stat = GdipCreateTextureIA(image, NULL, x, y, width, height, brush);

    return stat;
}

/******************************************************************************
 * GdipCreateTextureIAI [GDIPLUS.@]
 */
Status  GdipCreateTextureIAI(Image *image, GDIPCONST ImageAttributes *imageattr,
    INT x, INT y, INT width, INT height, Brush* brush)
{
    TRACE("(%p, %p, %d, %d, %d, %d, %p)\n", image, imageattr, x, y, width, height,
           brush);

    return GdipCreateTextureIA(image,NULL,(REAL)x,(REAL)y,(REAL)width,(REAL)height,brush);
}

Status  GdipCreateTexture2I(Image *image, WrapMode wrapmode,
        INT x, INT y, INT width, INT height, Brush* brush)
{
    Texture *texture = &brush->texture;
    Status stat;

    TRACE("%p %d %d %d %d %d %p\n", image, wrapmode, x, y, width, height,
            texture);

    
        texture->wrap = wrapmode;

        stat = GdipCreateTextureIA(image, NULL, x, y, width, height, brush);
    

    return stat;
}

Status  GdipGetBrushType(Brush *brush, BrushType *type)
{
    TRACE("(%p, %p)\n", brush, type);

    if(!brush || !type)  return InvalidParameter;

    *type = brush->bt;

    return Ok;
}

Status  GdipGetHatchBackgroundColor(Hatch *brush, ARGB *backcol)
{
    TRACE("(%p, %p)\n", brush, backcol);

    if(!brush || !backcol)  return InvalidParameter;

    *backcol = brush->backcol;

    return Ok;
}

Status  GdipGetHatchForegroundColor(Hatch *brush, ARGB *forecol)
{
    TRACE("(%p, %p)\n", brush, forecol);

    if(!brush || !forecol)  return InvalidParameter;

    *forecol = brush->forecol;

    return Ok;
}

Status  GdipGetHatchStyle(Hatch *brush, HatchStyle *hatchstyle)
{
    TRACE("(%p, %p)\n", brush, hatchstyle);

    if(!brush || !hatchstyle)  return InvalidParameter;

    *hatchstyle = brush->hatchstyle;

    return Ok;
}

Status  GdipDeleteBrush(Brush *brush)
{
    TRACE("(%p)\n", brush);

    if(!brush)  return InvalidParameter;

    switch(brush->bt)
    {
        case BrushTypePathGradient:
            break;
        case BrushTypeLinearGradient:
            break;
        case BrushTypeTextureFill:
            break;
        default:
            break;
    }
    return Ok;
}

Status  PathGradientGetRect(PathGradient *grad, RectF *rect)
{
    Status stat;

    TRACE("(%p, %p)\n", grad, rect);

    if(!grad || !rect)
        return InvalidParameter;

    stat = PathGetWorldBounds(grad->path, rect, NULL, NULL);

    return stat;
}

Status  PathGradientGetRectI(PathGradient *grad, Rect *rect)
{
    RectF rectf;
    Status stat;

    TRACE("(%p, %p)\n", grad, rect);

    if(!grad || !rect)
        return InvalidParameter;

    stat = PathGradientGetRect(grad, &rectf);
    if(stat != Ok)  return stat;

    rect->X = gdip_round(rectf.X);
    rect->Y = gdip_round(rectf.Y);
    rect->Width  = gdip_round(rectf.Width);
    rect->Height = gdip_round(rectf.Height);

    return Ok;
}

Status  PathGradientGetSurroundColorsWithCount(PathGradient
    *grad, ARGB *argb, INT *count)
{
    INT i;

    TRACE("(%p,%p,%p)\n", grad, argb, count);

    if(!grad || !argb || !count || (*count < grad->path->pathdata.Count))
        return InvalidParameter;

    for (i=0; i<grad->path->pathdata.Count; i++)
    {
        if (i < grad->surroundcolorcount)
            argb[i] = grad->surroundcolors[i];
        else
            argb[i] = grad->surroundcolors[grad->surroundcolorcount-1];
    }

    *count = grad->surroundcolorcount;

    return Ok;
}

/******************************************************************************
 * GdipMultiplyTextureTransform [GDIPLUS.@]
 */
Status  GdipMultiplyTextureTransform(Texture* brush,
    GDIPCONST Matrix *matrix, MatrixOrder order)
{
    TRACE("(%p, %p, %d)\n", brush, matrix, order);

    if(!brush || !matrix)
        return InvalidParameter;

    return GdipMultiplyMatrix(&brush->transform, matrix, order);
}

/******************************************************************************
 * GdipResetTextureTransform [GDIPLUS.@]
 */
Status  GdipResetTextureTransform(Texture* brush)
{
    TRACE("(%p)\n", brush);

    if(!brush)
        return InvalidParameter;

    return GdipSetMatrixElements(&brush->transform, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

/******************************************************************************
 * GdipScaleTextureTransform [GDIPLUS.@]
 */
Status  GdipScaleTextureTransform(Texture* brush,
    REAL sx, REAL sy, MatrixOrder order)
{
    TRACE("(%p, %.2f, %.2f, %d)\n", brush, sx, sy, order);

    if(!brush)
        return InvalidParameter;

    return GdipScaleMatrix(&brush->transform, sx, sy, order);
}

Status  GdipSetLineBlend(Brush *brush, REAL *factors, REAL* positions, INT count)
{
    LineGradient *line = &brush->line;

    TRACE("(%p, %p, %p, %i)\n", brush, factors, positions, count);

    if(!brush || !factors || !positions || count <= 0 ||
       (count >= 2 && (positions[0] != 0.0f || positions[count-1] != 1.0f)))
        return InvalidParameter;

    line->blendcount = count;
    line->blendfac = factors;
    line->blendpos = positions;

    return Ok;
}

Status  GdipGetLineBlend(LineGradient *brush, REAL *factors,
    REAL *positions, INT count)
{
    TRACE("(%p, %p, %p, %i)\n", brush, factors, positions, count);

    if (!brush || !factors || !positions || count <= 0)
        return InvalidParameter;

    if (count < brush->blendcount)
        return InsufficientBuffer;

    memcpy(factors, brush->blendfac, brush->blendcount * sizeof(REAL));
    memcpy(positions, brush->blendpos, brush->blendcount * sizeof(REAL));

    return Ok;
}

Status  GdipGetLineBlendCount(LineGradient *brush, INT *count)
{
    TRACE("(%p, %p)\n", brush, count);

    if (!brush || !count)
        return InvalidParameter;

    *count = brush->blendcount;

    return Ok;
}

Status  GdipSetLineGammaCorrection(LineGradient* line,
    BOOL usegamma)
{
    TRACE("(%p, %d)\n", line, usegamma);

    if(!line)
        return InvalidParameter;

    line->gamma = usegamma;

    return Ok;
}

Status  GdipSetLineSigmaBlend(Brush* brush, REAL focus, REAL scale, REAL* factors, REAL* positions)
{
    //REAL factors[33];
    //REAL positions[33];
    int num_points = 0;
    int i;
    const int precision = 16;
    REAL erf_range; /* we use values erf(-erf_range) through erf(+erf_range) */
    REAL min_erf;
    REAL scale_erf;

    TRACE("(%p, %0.2f, %0.2f)\n", brush, focus, scale);

    if(!brush || focus < 0.0 || focus > 1.0 || scale < 0.0 || scale > 1.0)
        return InvalidParameter;

    /* we want 2 standard deviations */
    erf_range = 2.0 / sqrt(2);

    /* calculate the constants we need to normalize the error function to be
        between 0.0 and scale over the range we need */
    min_erf = erf(-erf_range);
    scale_erf = scale / (-2.0 * min_erf);

    if (focus != 0.0)
    {
        positions[0] = 0.0;
        factors[0] = 0.0;
        for (i=1; i<precision; i++)
        {
            positions[i] = focus * i / precision;
            factors[i] = scale_erf * (erf(2 * erf_range * i / precision - erf_range) - min_erf);
        }
        num_points += precision;
    }

    positions[num_points] = focus;
    factors[num_points] = scale;
    num_points += 1;

    if (focus != 1.0)
    {
        for (i=1; i<precision; i++)
        {
            positions[i+num_points-1] = (focus + ((1.0-focus) * i / precision));
            factors[i+num_points-1] = scale_erf * (erf(erf_range - 2 * erf_range * i / precision) - min_erf);
        }
        num_points += precision;
        positions[num_points-1] = 1.0;
        factors[num_points-1] = 0.0;
    }

    return GdipSetLineBlend(brush, factors, positions, num_points);
}

Status  GdipSetLineWrapMode(LineGradient* line,
    WrapMode wrap)
{
    TRACE("(%p, %d)\n", line, wrap);

    if(!line || wrap == WrapModeClamp)
        return InvalidParameter;

    line->wrap = wrap;

    return Ok;
}

Status  PathGradientSetBlend(Brush *brush, REAL *fac, REAL *pos, INT count)
{
    PathGradient *grad = &brush->grad;

    TRACE("(%p,%p,%p,%i)\n", brush, fac, pos, count);

    if(!brush || !fac || !pos || count <= 0 ||
       (count >= 2 && (pos[0] != 0.0f || pos[count-1] != 1.0f)))
        return InvalidParameter;

    grad->blendcount = count;
    grad->blendfac = fac;
    grad->blendpos = pos;

    return Ok;
}

Status  PathGradientSetLinearBlend(Brush *brush,
    REAL focus, REAL scale, REAL* factors, REAL* positions)
{
    int num_points = 0;

    TRACE("(%p,%0.2f,%0.2f)\n", brush, focus, scale);

    if (!brush) return InvalidParameter;

    if (focus != 0.0)
    {
        factors[num_points] = 0.0;
        positions[num_points] = 0.0;
        num_points++;
    }

    factors[num_points] = scale;
    positions[num_points] = focus;
    num_points++;

    if (focus != 1.0)
    {
        factors[num_points] = 0.0;
        positions[num_points] = 1.0;
        num_points++;
    }

    return PathGradientSetBlend(brush, factors, positions, num_points);
}

Status  PathGradientSetPresetBlend(Brush* brush, ARGB *color, REAL *pos, INT count)
{
    PathGradient *grad = &brush->grad;
    TRACE("(%p,%p,%p,%i)\n", brush, color, pos, count);

    if (!brush || !color || !pos || count < 2 ||
        pos[0] != 0.0f || pos[count-1] != 1.0f)
    {
        return InvalidParameter;
    }

    grad->pblendcolor = color;
    grad->pblendpos = pos;
    grad->pblendcount = count;

    return Ok;
}

Status  PathGradientSetCenterColor(PathGradient *grad,
    ARGB argb)
{
    TRACE("(%p, %x)\n", grad, argb);

    if(!grad)
        return InvalidParameter;

    grad->centercolor = argb;
    return Ok;
}

Status  PathGradientSetCenterPoint(PathGradient *grad,
    PointF *point)
{
    TRACE("(%p, %s)\n", grad, debugstr_pointf(point));

    if(!grad || !point)
        return InvalidParameter;

    grad->center.X = point->X;
    grad->center.Y = point->Y;

    return Ok;
}

Status  PathGradientSetCenterPointI(PathGradient *grad, Point *point)
{
    PointF ptf;

    TRACE("(%p, %p)\n", grad, point);

    if(!point)
        return InvalidParameter;

    ptf.X = (REAL)point->X;
    ptf.Y = (REAL)point->Y;

    return PathGradientSetCenterPoint(grad,&ptf);
}

Status  PathGradientSetFocusScales(PathGradient *grad,
    REAL x, REAL y)
{
    TRACE("(%p, %.2f, %.2f)\n", grad, x, y);

    if(!grad)
        return InvalidParameter;

    grad->focus.X = x;
    grad->focus.Y = y;

    return Ok;
}

Status  PathGradientSetGammaCorrection(PathGradient *grad,
    BOOL gamma)
{
    TRACE("(%p, %d)\n", grad, gamma);

    if(!grad)
        return InvalidParameter;

    grad->gamma = gamma;

    return Ok;
}

Status  PathGradientSetSigmaBlend(Brush *brush, REAL focus, REAL scale, REAL* factors, REAL* positions)
{
    //REAL factors[33];
    //REAL positions[33];
    int num_points = 0;
    int i;
    const int precision = 16;
    REAL erf_range; /* we use values erf(-erf_range) through erf(+erf_range) */
    REAL min_erf;
    REAL scale_erf;

    TRACE("(%p,%0.2f,%0.2f)\n", brush, focus, scale);

    if(!brush || focus < 0.0 || focus > 1.0 || scale < 0.0 || scale > 1.0)
        return InvalidParameter;

    /* we want 2 standard deviations */
    erf_range = 2.0 / sqrt(2);

    /* calculate the constants we need to normalize the error function to be
        between 0.0 and scale over the range we need */
    min_erf = erf(-erf_range);
    scale_erf = scale / (-2.0 * min_erf);

    if (focus != 0.0)
    {
        positions[0] = 0.0;
        factors[0] = 0.0;
        for (i=1; i<precision; i++)
        {
            positions[i] = focus * i / precision;
            factors[i] = scale_erf * (erf(2 * erf_range * i / precision - erf_range) - min_erf);
        }
        num_points += precision;
    }

    positions[num_points] = focus;
    factors[num_points] = scale;
    num_points += 1;

    if (focus != 1.0)
    {
        for (i=1; i<precision; i++)
        {
            positions[i+num_points-1] = (focus + ((1.0-focus) * i / precision));
            factors[i+num_points-1] = scale_erf * (erf(erf_range - 2 * erf_range * i / precision) - min_erf);
        }
        num_points += precision;
        positions[num_points-1] = 1.0;
        factors[num_points-1] = 0.0;
    }

    return PathGradientSetBlend(brush, factors, positions, num_points);
}

Status  PathGradientSetSurroundColorsWithCount(Brush *brush, ARGB *argb, INT count)
{
    PathGradient* grad = &brush->grad;
    INT i;

    TRACE("(%p,%p,%p)\n", brush, argb, count);

    if(!brush || !argb || !count || (count <= 0) ||
        (count > grad->path->pathdata.Count))
        return InvalidParameter;

    /* If all colors are the same, only store 1 color. */
    if (count > 1)
    {
      for (i=1; i < count; i++) {
            if (argb[i] != argb[i-1])
                break;
      }

        if (i == count)
            count = 1;
    }

    grad->surroundcolors = argb;
    grad->surroundcolorcount = count;

    return Ok;
}

Status  PathGradientSetWrapMode(PathGradient *grad,
    WrapMode wrap)
{
    TRACE("(%p, %d)\n", grad, wrap);

    if(!grad)
        return InvalidParameter;

    grad->wrap = wrap;

    return Ok;
}

Status  PathGradientSetTransform(PathGradient *grad,
    Matrix *matrix)
{
    TRACE("(%p,%p)\n", grad, matrix);

    if (!grad || !matrix)
        return InvalidParameter;

    grad->transform = *matrix;

    return Ok;
}

Status  PathGradientGetTransform(PathGradient *grad,
    Matrix *matrix)
{
    TRACE("(%p,%p)\n", grad, matrix);

    if (!grad || !matrix)
        return InvalidParameter;

    *matrix = grad->transform;

    return Ok;
}

Status  GdipMultiplyPathGradientTransform(PathGradient *grad,
    GDIPCONST Matrix *matrix, MatrixOrder order)
{
    TRACE("(%p,%p,%i)\n", grad, matrix, order);

    if (!grad)
        return InvalidParameter;

    return GdipMultiplyMatrix(&grad->transform, matrix, order);
}

Status  GdipResetPathGradientTransform(PathGradient *grad)
{
    TRACE("(%p)\n", grad);

    if (!grad)
        return InvalidParameter;

    return GdipSetMatrixElements(&grad->transform, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

Status  GdipRotatePathGradientTransform(PathGradient *grad,
    REAL angle, MatrixOrder order)
{
    TRACE("(%p,%0.2f,%i)\n", grad, angle, order);

    if (!grad)
        return InvalidParameter;

    return GdipRotateMatrix(&grad->transform, angle, order);
}

Status  GdipScalePathGradientTransform(PathGradient *grad,
    REAL sx, REAL sy, MatrixOrder order)
{
    TRACE("(%p,%0.2f,%0.2f,%i)\n", grad, sx, sy, order);

    if (!grad)
        return InvalidParameter;

    return GdipScaleMatrix(&grad->transform, sx, sy, order);
}

Status  GdipTranslatePathGradientTransform(PathGradient *grad,
    REAL dx, REAL dy, MatrixOrder order)
{
    TRACE("(%p,%0.2f,%0.2f,%i)\n", grad, dx, dy, order);

    if (!grad)
        return InvalidParameter;

    return GdipTranslateMatrix(&grad->transform, dx, dy, order);
}

Status  GdipSetSolidFillColor(SolidFill *sf, ARGB argb)
{
    TRACE("(%p, %x)\n", sf, argb);

    if(!sf)
        return InvalidParameter;

    sf->color = argb;
    return Ok;
}

/******************************************************************************
 * GdipSetTextureTransform [GDIPLUS.@]
 */
Status  GdipSetTextureTransform(Texture *texture,
    GDIPCONST Matrix *matrix)
{
    TRACE("(%p, %p)\n", texture, matrix);

    if(!texture || !matrix)
        return InvalidParameter;

    texture->transform = *matrix;

    return Ok;
}

/******************************************************************************
 * GdipSetTextureWrapMode [GDIPLUS.@]
 *
 * WrapMode not used, only stored
 */
Status  GdipSetTextureWrapMode(Texture *brush, WrapMode wrapmode)
{
    TRACE("(%p, %d)\n", brush, wrapmode);

    if(!brush)
        return InvalidParameter;

    brush->imageattributes->wrap = wrapmode;

    return Ok;
}

Status  GdipSetLineColors(LineGradient *brush, ARGB color1,
    ARGB color2)
{
    TRACE("(%p, %x, %x)\n", brush, color1, color2);

    if(!brush)
        return InvalidParameter;

    brush->startcolor = color1;
    brush->endcolor   = color2;

    return Ok;
}

Status  GdipGetLineColors(LineGradient *brush, ARGB *colors)
{
    TRACE("(%p, %p)\n", brush, colors);

    if(!brush || !colors)
        return InvalidParameter;

    colors[0] = brush->startcolor;
    colors[1] = brush->endcolor;

    return Ok;
}

/******************************************************************************
 * GdipRotateTextureTransform [GDIPLUS.@]
 */
Status  GdipRotateTextureTransform(Texture* brush, REAL angle,
    MatrixOrder order)
{
    TRACE("(%p, %.2f, %d)\n", brush, angle, order);

    if(!brush)
        return InvalidParameter;

    return GdipRotateMatrix(&brush->transform, angle, order);
}

Status  GdipSetLineLinearBlend(Brush *brush, REAL focus, REAL scale, REAL* factors, REAL* positions)
{
  LineGradient* line = &brush->line;
    //REAL factors[3];
    //REAL positions[3];
    int num_points = 0;

    TRACE("(%p,%.2f,%.2f)\n", brush, focus, scale);

    if (!brush) return InvalidParameter;

    if (focus != 0.0)
    {
        factors[num_points] = 0.0;
        positions[num_points] = 0.0;
        num_points++;
    }

    factors[num_points] = scale;
    positions[num_points] = focus;
    num_points++;

    if (focus != 1.0)
    {
        factors[num_points] = 0.0;
        positions[num_points] = 1.0;
        num_points++;
    }

    return GdipSetLineBlend(brush, factors, positions, num_points);
}

Status  GdipSetLinePresetBlend(LineGradient *brush, ARGB *color, REAL* pos, INT count)
{
    TRACE("(%p,%p,%p,%i)\n", brush, color, pos, count);

    if (!brush || !color || !pos || count < 2 ||
        pos[0] != 0.0f || pos[count-1] != 1.0f)
    {
        return InvalidParameter;
    }

    brush->pblendcolor = color;
    brush->pblendpos = pos;
    brush->pblendcount = count;

    return Ok;
}

Status  GdipGetLinePresetBlendCount(LineGradient *brush,
    INT *count)
{
    if (!brush || !count)
        return InvalidParameter;

    *count = brush->pblendcount;

    return Ok;
}

Status  GdipResetLineTransform(LineGradient *brush)
{
    static int calls;

    TRACE("(%p)\n", brush);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetLineTransform(LineGradient *brush,
    GDIPCONST Matrix *matrix)
{
    static int calls;

    TRACE("(%p,%p)\n", brush,  matrix);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetLineTransform(LineGradient *brush, Matrix *matrix)
{
    static int calls;

    TRACE("(%p,%p)\n", brush, matrix);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipScaleLineTransform(LineGradient *brush, REAL sx, REAL sy,
    MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%0.2f,%0.2f,%u)\n", brush, sx, sy, order);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipMultiplyLineTransform(LineGradient *brush,
    GDIPCONST Matrix *matrix, MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%p,%u)\n", brush, matrix, order);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipTranslateLineTransform(LineGradient* brush,
        REAL dx, REAL dy, MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%f,%f,%d)\n", brush, dx, dy, order);

    if(!(calls++))
        FIXME("not implemented\n");

    return Ok;
}

/******************************************************************************
 * GdipTranslateTextureTransform [GDIPLUS.@]
 */
Status  GdipTranslateTextureTransform(Texture* brush, REAL dx, REAL dy,
    MatrixOrder order)
{
    TRACE("(%p, %.2f, %.2f, %d)\n", brush, dx, dy, order);

    if(!brush)
        return InvalidParameter;

    return GdipTranslateMatrix(&brush->transform, dx, dy, order);
}

Status  GdipGetLineRect(LineGradient *brush, RectF *rect)
{
    TRACE("(%p, %p)\n", brush, rect);

    if(!brush || !rect)
        return InvalidParameter;

    *rect = brush->rect;

    return Ok;
}

Status  GdipGetLineRectI(LineGradient *brush, Rect *rect)
{
    RectF  rectF;
    Status ret;

    TRACE("(%p, %p)\n", brush, rect);

    if(!rect)
        return InvalidParameter;

    ret = GdipGetLineRect(brush, &rectF);

    if(ret == Ok){
        rect->X      = gdip_round(rectF.X);
        rect->Y      = gdip_round(rectF.Y);
        rect->Width  = gdip_round(rectF.Width);
        rect->Height = gdip_round(rectF.Height);
    }

    return ret;
}

Status  GdipRotateLineTransform(LineGradient* brush,
    REAL angle, MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%0.2f,%u)\n", brush, angle, order);

    if(!brush)
        return InvalidParameter;

    if(!(calls++))
        FIXME("(%p, %.2f, %d) stub\n", brush, angle, order);

    return NotImplemented;
}
