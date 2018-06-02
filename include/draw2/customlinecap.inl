

Status  GdipCloneCustomLineCap(CustomLineCap* from, CustomLineCap* to)
{
    TRACE("(%p, %p)\n", from, to);

    if(!from || !to)
        return InvalidParameter;

    to->fill = from->fill;
    to->cap = from->cap;
    to->inset = from->inset;
    to->join = from->join;
    to->scale = from->scale;

    if(!PathData_SetSize(&to->pathdata, from->pathdata.Count)){
        return OutOfMemory;
    }

    memcpy(to->pathdata.Points, from->pathdata.Points, from->pathdata.Count * sizeof(PointF));
    memcpy(to->pathdata.Types, from->pathdata.Types, from->pathdata.Count);

    TRACE("<-- %p\n", *to);

    return Ok;
}

/* FIXME: Sometimes when fillPath is non-null and stroke path is null, the native
 * version of this function returns NotImplemented. I cannot figure out why. */
Status  GdipCreateCustomLineCap(const Path* fillPath, const Path* strokePath,
    LineCap baseCap, REAL baseInset, CustomLineCap *customCap)
{
    const PathData *pathdata = NULL;

    TRACE("%p %p %d %f %p\n", fillPath, strokePath, baseCap, baseInset, customCap);

    if(!customCap || !(fillPath || strokePath))
        return InvalidParameter;

    if(strokePath){
        customCap->fill = FALSE;
        pathdata = &strokePath->pathdata;
    }
    else{
        customCap->fill = TRUE;
        pathdata = &fillPath->pathdata;
    }

    if(!PathData_SetSize(&customCap->pathdata, pathdata->Count)) {
        return OutOfMemory;
    }

    memcpy(customCap->pathdata.Points, pathdata->Points, pathdata->Count
           * sizeof(PointF));
    memcpy(customCap->pathdata.Types, pathdata->Types, pathdata->Count);
    customCap->pathdata.Count = pathdata->Count;

    customCap->inset = baseInset;
    customCap->cap = baseCap;
    customCap->join = LineJoinMiter;
    customCap->scale = 1.0;

    TRACE("<-- %p\n", *customCap);

    return Ok;
}

Status  GdipDeleteCustomLineCap(CustomLineCap *customCap)
{
    TRACE("(%p)\n", customCap);

    if(!customCap)
        return InvalidParameter;

    PathData_Free(&customCap->pathdata);

    return Ok;
}

Status  GdipGetCustomLineCapStrokeJoin(const CustomLineCap* customCap, LineJoin* lineJoin)
{
    TRACE("(%p, %p)\n", customCap, lineJoin);

    if(!customCap || !lineJoin)
        return InvalidParameter;

    *lineJoin = customCap->join;

    return Ok;
}

Status  GdipGetCustomLineCapWidthScale(CustomLineCap* custom,
    REAL* widthScale)
{
    TRACE("(%p, %p)\n", custom, widthScale);

    if(!custom || !widthScale)
        return InvalidParameter;

    *widthScale = custom->scale;

    return Ok;
}

Status  GdipSetCustomLineCapStrokeCaps(CustomLineCap* custom,
    LineCap start, LineCap end)
{
    static int calls;

    TRACE("(%p,%u,%u)\n", custom, start, end);

    if(!custom)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetCustomLineCapBaseCap(CustomLineCap* custom,
    LineCap base)
{
    static int calls;

    TRACE("(%p,%u)\n", custom, base);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetCustomLineCapBaseInset(CustomLineCap* custom,
    REAL* inset)
{
    TRACE("(%p, %p)\n", custom, inset);

    if(!custom || !inset)
        return InvalidParameter;

    *inset = custom->inset;

    return Ok;
}

Status  GdipSetCustomLineCapBaseInset(CustomLineCap* custom,
    REAL inset)
{
    static int calls;

    TRACE("(%p,%0.2f)\n", custom, inset);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

/*FIXME: LineJoin completely ignored now */
Status  GdipSetCustomLineCapStrokeJoin(CustomLineCap* custom,
    LineJoin join)
{
    TRACE("(%p, %d)\n", custom, join);

    if(!custom)
        return InvalidParameter;

    custom->join = join;

    return Ok;
}

Status  GdipSetCustomLineCapWidthScale(CustomLineCap* custom,
    REAL width)
{
    static int calls;

    TRACE("(%p,%0.2f)\n", custom, width);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetCustomLineCapBaseCap(CustomLineCap *customCap, LineCap *baseCap)
{
    TRACE("(%p, %p)\n", customCap, baseCap);

    if(!customCap || !baseCap)
        return InvalidParameter;

    *baseCap = customCap->cap;

    return Ok;
}

Status  GdipCreateAdjustableArrowCap(REAL height, REAL width, BOOL fill,
    AdjustableArrowCap **cap)
{
    static int calls;

    TRACE("(%0.2f,%0.2f,%i,%p)\n", height, width, fill, cap);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetAdjustableArrowCapFillState(AdjustableArrowCap* cap, BOOL* fill)
{
    static int calls;

    TRACE("(%p,%p)\n", cap, fill);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetAdjustableArrowCapHeight(AdjustableArrowCap* cap, REAL* height)
{
    static int calls;

    TRACE("(%p,%p)\n", cap, height);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetAdjustableArrowCapMiddleInset(AdjustableArrowCap* cap, REAL* middle)
{
    static int calls;

    TRACE("(%p,%p)\n", cap, middle);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetAdjustableArrowCapWidth(AdjustableArrowCap* cap, REAL* width)
{
    static int calls;

    TRACE("(%p,%p)\n", cap, width);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetAdjustableArrowCapFillState(AdjustableArrowCap* cap, BOOL fill)
{
    static int calls;

    TRACE("(%p,%i)\n", cap, fill);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetAdjustableArrowCapHeight(AdjustableArrowCap* cap, REAL height)
{
    static int calls;

    TRACE("(%p,%0.2f)\n", cap, height);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetAdjustableArrowCapMiddleInset(AdjustableArrowCap* cap, REAL middle)
{
    static int calls;

    TRACE("(%p,%0.2f)\n", cap, middle);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetAdjustableArrowCapWidth(AdjustableArrowCap* cap, REAL width)
{
    static int calls;

    TRACE("(%p,%0.2f)\n", cap, width);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}
