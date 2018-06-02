
Status  PenCreate2(Brush *brush, REAL width, Unit unit, Pen *pen)
{
  TRACE("(%p, %.2f, %d, %p)\n", brush, width, unit, pen);
  
  if(!pen || !brush)
    return InvalidParameter;
  
  pen->width = width;
  pen->unit = unit;
  pen->endcap = LineCapFlat;
  pen->join = LineJoinMiter;
  pen->miterlimit = 10.0;
  pen->dash = DashStyleSolid;
  pen->offset = 0.0;
  pen->customstart = NULL;
  pen->customend = NULL;
  pen->align = PenAlignmentInset;
  
  if(!((pen->unit == UnitWorld) || (pen->unit == UnitPixel))) {
    FIXME("UnitWorld, UnitPixel only supported units\n");
    return NotImplemented;
  }
  
  *pen->brush = *brush;
  
  TRACE("<-- %p\n", *pen);
  
  return Ok;
}

Status  PenCreate1(ARGB color, REAL width, Unit unit, Pen *pen)
{
    Status status;

    TRACE("(%x, %.2f, %d, %p)\n", color, width, unit, pen);

    GdipCreateSolidFill(color, pen->brush);
    status = PenCreate2(pen->brush, width, unit, pen);
    return status;
}

Status  PenResetTransform(Pen *pen)
{
    static int calls;

    TRACE("(%p)\n", pen);

    if(!pen)
        return InvalidParameter;

    if(!(calls++))
        FIXME("(%p) stub\n", pen);

    return NotImplemented;
}

Status  PenSetTransform(Pen *pen, Matrix *matrix)
{
    static int calls;

    TRACE("(%p,%p)\n", pen, matrix);

    if(!pen || !matrix)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  PenGetTransform(Pen *pen, Matrix *matrix)
{
    static int calls;

    TRACE("(%p,%p)\n", pen, matrix);

    if(!pen || !matrix)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipTranslatePenTransform(Pen *pen, REAL dx, REAL dy, MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%0.2f,%0.2f,%u)\n", pen, dx, dy, order);

    if(!pen)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipScalePenTransform(Pen *pen, REAL sx, REAL sy, MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%0.2f,%0.2f,%u)\n", pen, sx, sy, order);

    if(!pen)
        return InvalidParameter;

    if(!(calls++))
        FIXME("(%p, %.2f, %.2f, %d) stub\n", pen, sx, sy, order);

    return NotImplemented;
}

Status  GdipRotatePenTransform(Pen *pen, REAL angle, MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%0.2f,%u)\n", pen, angle, order);

    if(!pen)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipMultiplyPenTransform(Pen *pen, GDIPCONST Matrix *matrix,
    MatrixOrder order)
{
    static int calls;

    TRACE("(%p,%p,%u)\n", pen, matrix, order);

    if(!pen)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  PenSetBrushFill(Pen *pen, Brush *brush)
{
    TRACE("(%p, %p)\n", pen, brush);

    if(!pen || !brush)
        return InvalidParameter;

    *pen->brush = *brush;
    return Ok;
}

Status  PenSetColor(Pen *pen, ARGB argb)
{
    TRACE("(%p, %x)\n", pen, argb);

    if(!pen)
        return InvalidParameter;

    if(pen->brush->bt != BrushTypeSolidColor)
        return NotImplemented;

    return GdipSetSolidFillColor(&pen->brush->solid, argb);
}

Status  PenGetCompoundCount(Pen *pen, INT *count)
{
    FIXME("(%p, %p): stub\n", pen, count);

    if (!pen || !count)
        return InvalidParameter;

    return NotImplemented;
}

Status  PenSetCompoundArray(Pen *pen, GDIPCONST REAL *dash,
    INT count)
{
    FIXME("(%p, %p, %i): stub\n", pen, dash, count);

    if (!pen || !dash || count < 2 || count%2 == 1)
        return InvalidParameter;

    return NotImplemented;
}

Status  PenSetCustomEndCap(Pen *pen, CustomLineCap* customCap)
{
    TRACE("(%p, %p)\n", pen, customCap);

    /* native crashes on pen == NULL, customCap != NULL */
    if(!customCap) return InvalidParameter;

    pen->endcap = LineCapCustom;
    pen->customend = customCap;
    return Ok;
}

Status  PenSetCustomStartCap(Pen *pen, CustomLineCap* customCap)
{
    TRACE("(%p, %p)\n", pen, customCap);

    /* native crashes on pen == NULL, customCap != NULL */
    if(!customCap) return InvalidParameter;

        pen->startcap = LineCapCustom;
        pen->customstart = customCap;

    return Ok;
}

Status  PenSetDashStyle(Pen *pen, DashStyle dash)
{
  TRACE("(%p, %d)\n", pen, dash);
  
  if(!pen)
    return InvalidParameter;
  
  pen->dash = dash;
  return Ok;
}

Status  PenSetDashArray(Pen *pen, REAL *dash, INT count)
{
    INT i;
    REAL sum = 0;

    TRACE("(%p, %p, %d)\n", pen, dash, count);

    if(!pen || !dash)
        return InvalidParameter;

    if(count <= 0)
        return OutOfMemory;

    for(i = 0; i < count; i++) {
        sum += dash[i];
        if(dash[i] < 0.0) {
            return InvalidParameter;
        }
    }

    if(sum == 0.0 && count)
        return InvalidParameter;

    pen->dashes = dash;

    pen->dash = DashStyleCustom;
    memcpy(pen->dashes, dash, count * sizeof(REAL));
    pen->numdashes = count;

    return Ok;
}

Status  PenSetDashCap197819(Pen *pen, DashCap dashCap)
{
    TRACE("(%p, %d)\n", pen, dashCap);

    if(!pen)
        return InvalidParameter;

    pen->dashcap = dashCap;

    return Ok;
}

/* FIXME: dash offset not used */
Status  PenSetDashOffset(Pen *pen, REAL offset)
{
    TRACE("(%p, %.2f)\n", pen, offset);

    if(!pen)
        return InvalidParameter;

    pen->offset = offset;

    return Ok;
}

Status  PenSetEndCap(Pen *pen, LineCap cap)
{
    TRACE("(%p, %d)\n", pen, cap);

    if(!pen)    return InvalidParameter;

    /* The old custom cap gets deleted even if the new style is LineCapCustom. */
    GdipDeleteCustomLineCap(pen->customend);
    pen->customend = NULL;
    pen->endcap = cap;

    return Ok;
}

/* FIXME: startcap, dashcap not used. */
Status  PenSetLineCap197819(Pen *pen, LineCap start,
    LineCap end, DashCap dash)
{
    TRACE("%p, %d, %d, %d)\n", pen, start, end, dash);

    if(!pen)
        return InvalidParameter;

    GdipDeleteCustomLineCap(pen->customend);
    GdipDeleteCustomLineCap(pen->customstart);
    pen->customend = NULL;
    pen->customstart = NULL;

    pen->startcap = start;
    pen->endcap = end;
    pen->dashcap = dash;

    return Ok;
}

/* FIXME: Miter line joins behave a bit differently than they do in windows.
 * Both kinds of miter joins clip if the angle is less than 11 degrees. */
Status  PenSetLineJoin(Pen *pen, LineJoin join)
{
    TRACE("(%p, %d)\n", pen, join);

    if(!pen)    return InvalidParameter;

    pen->join = join;
    return Ok;
}

Status  PenSetMiterLimit(Pen *pen, REAL limit)
{
    TRACE("(%p, %.2f)\n", pen, limit);

    if(!pen)
        return InvalidParameter;

    pen->miterlimit = limit;

    return Ok;
}

Status  PenSetStartCap(Pen *pen, LineCap cap)
{
    TRACE("(%p, %d)\n", pen, cap);

    if(!pen)    return InvalidParameter;

    GdipDeleteCustomLineCap(pen->customstart);
    pen->customstart = NULL;
    pen->startcap = cap;

    return Ok;
}

Status  PenSetWidth(Pen *pen, REAL width)
{
    TRACE("(%p, %.2f)\n", pen, width);

    if(!pen)    return InvalidParameter;

    pen->width = width;

    return Ok;
}

Status  PenSetMode(Pen *pen, PenAlignment mode)
{
    TRACE("(%p, %d)\n", pen, mode);

    if(!pen)    return InvalidParameter;

    pen->align = mode;

    return Ok;
}
