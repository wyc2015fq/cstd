
Status  GdipCreateImageAttributes(ImageAttributes *imageattr)
{
  if(!imageattr)
    return InvalidParameter;
  
  if(!imageattr)    return OutOfMemory;
  
  imageattr->wrap = WrapModeClamp;
  
  TRACE("<-- %p\n", *imageattr);
  
  return Ok;
}

Status  GdipCloneImageAttributes(GDIPCONST ImageAttributes *imageattr,
    ImageAttributes *cloneImageattr)
{
    Status stat;

    TRACE("(%p, %p)\n", imageattr, cloneImageattr);

    if(!imageattr || !cloneImageattr)
        return InvalidParameter;

    stat = GdipCreateImageAttributes(cloneImageattr);

    if (stat == Ok)
        *cloneImageattr = *imageattr;

    return stat;
}

Status  GdipDisposeImageAttributes(ImageAttributes *imageattr)
{
    int i;

    TRACE("(%p)\n", imageattr);

    if(!imageattr)
        return InvalidParameter;

    for (i=0; i<ColorAdjustTypeCount; i++) {
        pfree(imageattr->colorremaptables[i].colormap);
    }

    return Ok;
}

Status  GdipSetImageAttributesColorKeys(ImageAttributes *imageattr,
    ColorAdjustType type, BOOL enableFlag, ARGB colorLow, ARGB colorHigh)
{
    TRACE("(%p,%u,%i,%08x,%08x)\n", imageattr, type, enableFlag, colorLow, colorHigh);

    if(!imageattr || type >= ColorAdjustTypeCount)
        return InvalidParameter;

    imageattr->colorkeys[type].enabled = enableFlag;
    imageattr->colorkeys[type].low = colorLow;
    imageattr->colorkeys[type].high = colorHigh;

    return Ok;
}

Status  GdipSetImageAttributesColorMatrix(ImageAttributes *imageattr,
    ColorAdjustType type, BOOL enableFlag, GDIPCONST ColorMatrix* colorMatrix,
    GDIPCONST ColorMatrix* grayMatrix, ColorMatrixFlags flags)
{
    TRACE("(%p,%u,%i,%p,%p,%u)\n", imageattr, type, enableFlag, colorMatrix,
        grayMatrix, flags);

    if(!imageattr || type >= ColorAdjustTypeCount || flags > ColorMatrixFlagsAltGray)
        return InvalidParameter;

    if (enableFlag)
    {
        if (!colorMatrix)
            return InvalidParameter;

        if (flags == ColorMatrixFlagsAltGray)
        {
            if (!grayMatrix)
                return InvalidParameter;

            imageattr->colormatrices[type].graymatrix = *grayMatrix;
        }

        imageattr->colormatrices[type].colormatrix = *colorMatrix;
        imageattr->colormatrices[type].flags = flags;
    }

    imageattr->colormatrices[type].enabled = enableFlag;

    return Ok;
}

Status  GdipSetImageAttributesWrapMode(ImageAttributes *imageAttr,
    WrapMode wrap, ARGB argb, BOOL clamp)
{
    TRACE("(%p,%u,%08x,%i)\n", imageAttr, wrap, argb, clamp);

    if(!imageAttr || wrap > WrapModeClamp)
        return InvalidParameter;

    imageAttr->wrap = wrap;
    imageAttr->outside_color = argb;
    imageAttr->clamp = clamp;

    return Ok;
}

Status  GdipSetImageAttributesCachedBackground(ImageAttributes *imageAttr,
    BOOL enableFlag)
{
    static int calls;

    TRACE("(%p,%i)\n", imageAttr, enableFlag);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetImageAttributesGamma(ImageAttributes *imageAttr,
    ColorAdjustType type, BOOL enableFlag, REAL gamma)
{
    TRACE("(%p,%u,%i,%0.2f)\n", imageAttr, type, enableFlag, gamma);

    if (!imageAttr || (enableFlag && gamma <= 0.0) || type >= ColorAdjustTypeCount)
        return InvalidParameter;

    imageAttr->gamma_enabled[type] = enableFlag;
    imageAttr->gamma[type] = gamma;

    return Ok;
}

Status  GdipSetImageAttributesNoOp(ImageAttributes *imageAttr,
    ColorAdjustType type, BOOL enableFlag)
{
    static int calls;

    TRACE("(%p,%u,%i)\n", imageAttr, type, enableFlag);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetImageAttributesOutputChannel(ImageAttributes *imageAttr,
    ColorAdjustType type, BOOL enableFlag, ColorChannelFlags channelFlags)
{
    static int calls;

    TRACE("(%p,%u,%i,%x)\n", imageAttr, type, enableFlag, channelFlags);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetImageAttributesOutputChannelColorProfile(ImageAttributes *imageAttr,
    ColorAdjustType type, BOOL enableFlag,
    GDIPCONST WCHAR *colorProfileFilename)
{
    static int calls;

    TRACE("(%p,%u,%i,%s)\n", imageAttr, type, enableFlag, debugstr_w(colorProfileFilename));

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetImageAttributesRemapTable(ImageAttributes *imageAttr,
    ColorAdjustType type, BOOL enableFlag, UINT mapSize,
    GDIPCONST ColorMap *map)
{
  color_remap_table* table = imageAttr->colorremaptables + type;
    TRACE("(%p,%u,%i,%u,%p)\n", imageAttr, type, enableFlag, mapSize, map);

    if(!imageAttr || type >= ColorAdjustTypeCount)
	return InvalidParameter;

    if (enableFlag)
    {
        if(!map || !mapSize)
          return InvalidParameter;

        MYREALLOC(table->colormap, mapSize);

        if (!table->colormap)
            return OutOfMemory;

        memcpy(table->colormap, map, sizeof(*map) * mapSize);
        table->mapsize = mapSize;
    }
    else
    {
        FREE(table->colormap);
        table->colormap = NULL;
    }

    table->enabled = enableFlag;

    return Ok;
}

Status  GdipSetImageAttributesThreshold(ImageAttributes *imageAttr,
    ColorAdjustType type, BOOL enableFlag, REAL threshold)
{
    static int calls;

    TRACE("(%p,%u,%i,%0.2f)\n", imageAttr, type, enableFlag, threshold);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetImageAttributesToIdentity(ImageAttributes *imageAttr,
    ColorAdjustType type)
{
    static int calls;

    TRACE("(%p,%u)\n", imageAttr, type);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}
