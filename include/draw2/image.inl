
#define PIXELFORMATBPP(x) ((x) ? ((x) >> 8) & 255 : 24)


#include "draw/pixel.inl"

typedef struct {
  COLOR (*getpixel)(const BYTE* row, UINT x, const COLOR* palette);
  void (*setpixel)(BYTE r, BYTE g, BYTE b, BYTE a, BYTE* row, UINT x, const COLOR* palette, int Count);
} pixel_fun_t;

pixel_fun_t* get_pixel_fun(PixFmt fmt) {
  uint id = GetPixFmtId(fmt);
  static int inited = 0;
  static pixel_fun_t pixel_fun[PixFmtMax] = {0};
  if (!inited) {
    inited = 1;
#define PIXFMTDEF(_ID, _B, _NAME, _FLAG)  pixel_fun[_ID].getpixel = getpixel_ ## _B ##bpp## _NAME; \
    pixel_fun[_ID].setpixel = setpixel_ ## _B ##bpp## _NAME;
    PIXFMTDEF_DEF(PIXFMTDEF)
#undef PIXFMTDEF
  }
  return id<PixFmtMax ? pixel_fun+id : NULL;
}

Status  GdipBitmapGetPixel(Bitmap* bitmap, INT x, INT y, ARGB *color)
{
  BYTE* row;
  pixel_fun_t* fun = get_pixel_fun(bitmap->pixfmt);
  TRACE("%p %d %d %p\n", bitmap, x, y, color);
  if (!bitmap || !color || x < 0 || y < 0 || x >= bitmap->width || y >= bitmap->height) {
    return InvalidParameter;
  }
  row = bitmap->p_bit_data + bitmap->stride * y;
  if (fun && fun->getpixel) {
    *color = fun->getpixel(row, x, bitmap->palette);
  } else {
    printf("not implemented for format 0x%x\n", bitmap->pixfmt);
  }
  return Ok;
}

Status  GdipBitmapSetPixel(Bitmap* bitmap, INT x, INT y, ARGB color)
{
    BYTE a, r, g, b;
    BYTE *row;
    pixel_fun_t* fun = get_pixel_fun(bitmap->pixfmt);
    
    TRACE("bitmap:%p, x:%d, y:%d, color:%08x\n", bitmap, x, y, color);

    if(!bitmap || x < 0 || y < 0 || x >= bitmap->width || y >= bitmap->height)
        return InvalidParameter;

    a = color>>24;
    r = color>>16;
    g = color>>8;
    b = color;

    row = bitmap->p_bit_data + bitmap->stride * y;
  if (fun && fun->setpixel) {
      fun->setpixel(r,g,b,a,row, x, bitmap->palette, bitmap->pal_count);
    } else {
      FIXME("not implemented for format 0x%x\n", bitmap->pixfmt);
      return NotImplemented;
    }

    return Ok;
}

Status convert_pixels(INT width, INT height,
                      INT dst_stride, BYTE *dst_bits, PixelFormat dst_format, const COLOR *dst_palette, int dst_Count,
                      INT src_stride, const BYTE *src_bits, PixelFormat src_format, const COLOR *src_palette, int src_Count)
{
  INT x, y;
  if (src_format == dst_format || (dst_format == PixelFormat32bppRGB && PIXELFORMATBPP(src_format) == 32))
  {
    UINT widthbytes = PIXELFORMATBPP(src_format) * width / 8;
    if (dst_bits && src_bits) {
      for (y=0; y<height; y++) {
        memcpy(dst_bits+dst_stride*y, src_bits+src_stride*y, widthbytes);
      }
    }
    return Ok;
  } else {
    pixel_fun_t* src_fun = get_pixel_fun(src_format);
    pixel_fun_t* dst_fun = get_pixel_fun(dst_format);
    if (src_fun && src_fun->getpixel && dst_fun && dst_fun->setpixel) {
      if (dst_bits && src_bits) {
        for (x=0; x<width; x++) {
          for (y=0; y<height; y++) {
            COLOR argb;
            BYTE *color = (BYTE *)&argb;
            argb = src_fun->getpixel(src_bits+src_stride*y, x, src_palette);
            dst_fun->setpixel(color[2], color[1], color[0], color[3], dst_bits+dst_stride*y, x, dst_palette, dst_Count);
          }
        }
      }
      return Ok;
    }
  }
  
  return NotImplemented;
}

#define LINESIZE(width, bpp) ((((width) * (bpp) + 7) / 8) + 3) & ~3

/* This function returns a pointer to an array of pixels that represents the
 * bitmap. The *entire* bitmap is locked according to the lock mode specified by
 * flags.  It is correct behavior that a user who calls this function with write
 * privileges can write to the whole bitmap (not just the area in rect).
 *
 * FIXME: only used portion of format is bits per pixel. */
Status  GdipBitmapLockBits(Bitmap* bitmap, GDIPCONST Rect* rect,
    UINT flags, PixelFormat format, BitmapData* lockeddata)
{
    INT bitspp = PIXELFORMATBPP(format);
    Rect act_rect; /* actual rect to be used */
    Status stat;

    TRACE("%p %p %d 0x%x %p\n", bitmap, rect, flags, format, lockeddata);

    if(!lockeddata || !bitmap)
        return InvalidParameter;

    if(rect){
        if(rect->X < 0 || rect->Y < 0 || (rect->X + rect->Width > bitmap->width) ||
          (rect->Y + rect->Height > bitmap->height) || !flags)
            return InvalidParameter;

        act_rect = *rect;
    }
    else{
        act_rect.X = act_rect.Y = 0;
        act_rect.Width  = bitmap->width;
        act_rect.Height = bitmap->height;
    }

    if(bitmap->lockmode)
    {
        WARN("bitmap is already locked and cannot be locked again\n");
        return WrongState;
    }

    if (bitmap->p_bit_data && bitmap->pixfmt == format && !(flags & ImageLockModeUserInputBuf))
    {
        /* no conversion is necessary; just use the bits directly */
        lockeddata->Width = act_rect.Width;
        lockeddata->Height = act_rect.Height;
        lockeddata->PixelFormat = format;
        lockeddata->Reserved = flags;
        lockeddata->Stride = bitmap->stride;
        lockeddata->Scan0 = bitmap->p_bit_data + (bitspp / 8) * act_rect.X +
                            bitmap->stride * act_rect.Y;

        bitmap->lockmode = (flags | ImageLockModeRead);
        bitmap->numlocks++;

        return Ok;
    }

    /* Make sure we can convert to the requested format. */
    if (flags & ImageLockModeRead)
    {
        stat = convert_pixels(0, 0, 0, NULL, format, NULL, 0, 0, NULL, bitmap->pixfmt, NULL, 0);
        if (stat == NotImplemented)
        {
            FIXME("cannot read bitmap from %x to %x\n", bitmap->pixfmt, format);
            return NotImplemented;
        }
    }

    /* If we're opening for writing, make sure we'll be able to write back in
     * the original format. */
    if (flags & ImageLockModeWrite)
    {
        stat = convert_pixels(0, 0, 0, NULL, bitmap->pixfmt, NULL, 0, 0, NULL, format, NULL, 0);
        if (stat == NotImplemented)
        {
            FIXME("cannot write bitmap from %x to %x\n", format, bitmap->pixfmt);
            return NotImplemented;
        }
    }

    lockeddata->Width  = act_rect.Width;
    lockeddata->Height = act_rect.Height;
    lockeddata->PixelFormat = format;
    lockeddata->Reserved = flags;
#if 0
    if(!(flags & ImageLockModeUserInputBuf))
    {
        lockeddata->Stride = (((act_rect.Width * bitspp + 7) / 8) + 3) & ~3;

        bitmap->bitmapbits = GdipAlloc(lockeddata->Stride * act_rect.Height);

        if (!bitmap->bitmapbits) return OutOfMemory;

        lockeddata->Scan0  = bitmap->bitmapbits;
    }

    if (flags & ImageLockModeRead)
    {
        static int fixme=0;

        if (!fixme && (PIXELFORMATBPP(bitmap->pixfmt) * act_rect.X) % 8 != 0)
        {
            FIXME("Cannot copy rows that don't start at a whole byte.\n");
            fixme = 1;
        }

        stat = convert_pixels(act_rect.Width, act_rect.Height,
            lockeddata->Stride, lockeddata->Scan0, format, NULL, 0, bitmap->stride,
            bitmap->p_bit_data + bitmap->stride * act_rect.Y + PIXELFORMATBPP(bitmap->pixfmt) * act_rect.X / 8,
            bitmap->pixfmt, bitmap->palette, bitmap->pal_count);

        if (stat != Ok)
        {
            pfree(bitmap->bitmapbits);
            bitmap->bitmapbits = NULL;
            return stat;
        }
    }
#endif

    bitmap->lockmode = flags | ImageLockModeRead;
    bitmap->numlocks++;
    bitmap->lockx = act_rect.X;
    bitmap->locky = act_rect.Y;

    return Ok;
}

Status  GdipBitmapSetResolution(Bitmap* bitmap, REAL xdpi, REAL ydpi)
{
  ASSERT(0);
#if 0
    TRACE("(%p, %.2f, %.2f)\n", bitmap, xdpi, ydpi);

    if (!bitmap || xdpi == 0.0 || ydpi == 0.0)
        return InvalidParameter;

    bitmap->image.xres = xdpi;
    bitmap->image.yres = ydpi;
#endif
    return Ok;
}

Status  GdipBitmapUnlockBits(Bitmap* bitmap,
    BitmapData* lockeddata)
{
    Status stat = Ok;
    static int fixme=0;
#if 0
    TRACE("(%p,%p)\n", bitmap, lockeddata);

    if(!bitmap || !lockeddata)
        return InvalidParameter;

    if(!bitmap->lockmode)
        return WrongState;

    if(!(lockeddata->Reserved & ImageLockModeWrite)){
        if(!(--bitmap->numlocks))
            bitmap->lockmode = 0;

        GdipFree(bitmap->bitmapbits);
        bitmap->bitmapbits = NULL;
        return Ok;
    }

    if (!bitmap->bitmapbits && !(lockeddata->Reserved & ImageLockModeUserInputBuf))
    {
        /* we passed a direct reference; no need to do anything */
        bitmap->lockmode = 0;
        bitmap->numlocks = 0;
        return Ok;
    }

    if (!fixme && (PIXELFORMATBPP(bitmap->pixfmt) * bitmap->lockx) % 8 != 0)
    {
        FIXME("Cannot copy rows that don't start at a whole byte.\n");
        fixme = 1;
    }

    stat = convert_pixels(lockeddata->Width, lockeddata->Height,
        bitmap->stride,
        bitmap->p_bit_data + bitmap->stride * bitmap->locky + PIXELFORMATBPP(bitmap->pixfmt) * bitmap->lockx / 8,
        bitmap->pixfmt,
        lockeddata->Stride, lockeddata->Scan0, lockeddata->PixelFormat, NULL);

    if (stat != Ok)
    {
        ERR("failed to convert pixels; this should never happen\n");
    }

    GdipFree(bitmap->bitmapbits);
    bitmap->bitmapbits = NULL;
    bitmap->lockmode = 0;
    bitmap->numlocks = 0;
    
#endif
    return stat;
}

static void generate_halftone_palette(COLOR *entries, UINT count)
{
  static const BYTE halftone_values[6]={0x00,0x33,0x66,0x99,0xcc,0xff};
  UINT i;
  
  for (i=0; i<8 && i<count; i++)
  {
    entries[i] = 0xff000000;
    if (i&1) entries[i] |= 0x800000;
    if (i&2) entries[i] |= 0x8000;
    if (i&4) entries[i] |= 0x80;
  }
  
  if (8 < count)
    entries[i] = 0xffc0c0c0;
  
  for (i=9; i<16 && i<count; i++)
  {
    entries[i] = 0xff000000;
    if (i&1) entries[i] |= 0xff0000;
    if (i&2) entries[i] |= 0xff00;
    if (i&4) entries[i] |= 0xff;
  }
  
  for (i=16; i<40 && i<count; i++)
  {
    entries[i] = 0;
  }
  
  for (i=40; i<256 && i<count; i++)
  {
    entries[i] = 0xff000000;
    entries[i] |= halftone_values[(i-40)%6];
    entries[i] |= halftone_values[((i-40)/6)%6] << 8;
    entries[i] |= halftone_values[((i-40)/36)%6] << 16;
  }
}

Status  GdipCreateBitmapFromScan0(INT width, INT height, INT stride, PixelFormat format, const void* scan0, Bitmap* bitmap)
{
    INT row_size, dib_stride;
    int bpp = PIXELFORMATBPP(format);

    TRACE("%d %d %d 0x%x %p %p\n", width, height, stride, format, scan0, bitmap);

    if (!bitmap) return InvalidParameter;

    if(width <= 0 || height <= 0 || (scan0 && (stride % 4))){
        return InvalidParameter;
    }

    if(scan0 && !stride)
        return InvalidParameter;

    row_size = (width * bpp +7) / 8;
    dib_stride = (row_size + 3) & ~3;

    if(stride == 0)
        stride = dib_stride;
    
    bitmap->imgnumbers = 1;
    bitmap->current_frame = 0;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->pixfmt = format;
    bitmap->stride = dib_stride;

    /* Not a GDI format; don't try to make an HBITMAP. */
    {
      INT i, size = dib_stride * height;
      const BYTE* bits = (const BYTE*)scan0;
      bitmap->p_bit_data = (BYTE*)prealloc(bitmap->p_bit_data, size);
      if (!bitmap->p_bit_data) return OutOfMemory;
      
      if (stride < 0) {
        bits += stride * (1 - height);
      }
      for (i=0; i<height; ++i) {
        memcpy(bitmap->p_bit_data + i*dib_stride, bits + i*stride, row_size);
      }
    }
    

    /* set format-related flags */
    if (format == PixelFormat1bppIndexed ||
        format == PixelFormat4bppIndexed ||
        format == PixelFormat8bppIndexed)
    {
        bitmap->pal_count = 1 << bpp;

        if (format == PixelFormat1bppIndexed)
        {
            bitmap->palette[0] = 0xff000000;
            bitmap->palette[1] = 0xffffffff;
        }
        else
        {
            generate_halftone_palette(bitmap->palette, bitmap->pal_count);
        }
    }

    TRACE("<-- %p\n", *bitmap);

    return Ok;
}

static Status free_image_data(Image *image)
{
  if(!image)
    return InvalidParameter;
  
  INFOSTR_free(image);
  return Ok;
}

Status  GdipDisposeImage(Image *image)
{
  Status status;
  
  TRACE("%p\n", image);
  
  status = free_image_data(image);
  if (status != Ok) return status;
  return Ok;
}

Status  GdipCloneBitmapArea(REAL x, REAL y, REAL width, REAL height,
    PixelFormat format, Bitmap* srcBitmap, Bitmap* dstBitmap)
{
    BitmapData lockeddata_src, lockeddata_dst;
    int i;
    UINT row_size;
    Rect area;
    Status stat;

    TRACE("(%f,%f,%f,%f,0x%x,%p,%p)\n", x, y, width, height, format, srcBitmap, dstBitmap);

    if (!srcBitmap || !dstBitmap || x < 0 || y < 0 ||
        x + width > srcBitmap->width || y + height > srcBitmap->height)
    {
        TRACE("<-- InvalidParameter\n");
        return InvalidParameter;
    }

    if (format == PixelFormatDontCare) {
        format = srcBitmap->pixfmt;
    }

    area.X = gdip_round(x);
    area.Y = gdip_round(y);
    area.Width = gdip_round(width);
    area.Height = gdip_round(height);

    stat = GdipBitmapLockBits(srcBitmap, &area, ImageLockModeRead, format,
        &lockeddata_src);
    if (stat != Ok) return stat;

    stat = GdipCreateBitmapFromScan0(lockeddata_src.Width, lockeddata_src.Height,
        0, lockeddata_src.PixelFormat, NULL, dstBitmap);
    if (stat == Ok)
    {
        stat = GdipBitmapLockBits(dstBitmap, NULL, ImageLockModeWrite,
            lockeddata_src.PixelFormat, &lockeddata_dst);

        if (stat == Ok)
        {
            /* copy the image data */
            row_size = (lockeddata_src.Width * PIXELFORMATBPP(lockeddata_src.PixelFormat) +7)/8;
            for (i=0; i<lockeddata_src.Height; i++)
                memcpy((BYTE*)lockeddata_dst.Scan0+lockeddata_dst.Stride*i,
                       (BYTE*)lockeddata_src.Scan0+lockeddata_src.Stride*i,
                       row_size);

            GdipBitmapUnlockBits(dstBitmap, &lockeddata_dst);
        }

        if (stat != Ok) {
            GdipDisposeImage(dstBitmap);
        }
    }

    GdipBitmapUnlockBits(srcBitmap, &lockeddata_src);

    return stat;
}

Status  GdipCloneBitmapAreaI(INT x, INT y, INT width, INT height,
    PixelFormat format, Bitmap* srcBitmap, Bitmap* dstBitmap)
{
    TRACE("(%i,%i,%i,%i,0x%x,%p,%p)\n", x, y, width, height, format, srcBitmap, dstBitmap);

    return GdipCloneBitmapArea(x, y, width, height, format, srcBitmap, dstBitmap);
}

Status  GdipCloneImage(const Image *image, Image *cloneImage)
{
    Status stat = GenericError;
    Bitmap *bitmap = (Bitmap*)image;
    BitmapData lockeddata_src, lockeddata_dst;
    int i;
    UINT row_size;

    TRACE("%p, %p\n", image, cloneImage);

    if (!image || !cloneImage)
      return InvalidParameter;

    stat = GdipBitmapLockBits(bitmap, NULL, ImageLockModeRead, bitmap->pixfmt,
        &lockeddata_src);
    if (stat != Ok) return stat;

    stat = GdipCreateBitmapFromScan0(lockeddata_src.Width, lockeddata_src.Height,
        0, lockeddata_src.PixelFormat, NULL, cloneImage);

    if (stat == Ok)
    {
        stat = GdipBitmapLockBits(cloneImage, NULL, ImageLockModeWrite,
            lockeddata_src.PixelFormat, &lockeddata_dst);

        if (stat == Ok)
        {
            /* copy the image data */
            row_size = (lockeddata_src.Width * PIXELFORMATBPP(lockeddata_src.PixelFormat) +7)/8;
            for (i=0; i<lockeddata_src.Height; i++)
                memcpy((BYTE*)lockeddata_dst.Scan0+lockeddata_dst.Stride*i,
                       (BYTE*)lockeddata_src.Scan0+lockeddata_src.Stride*i,
                       row_size);

            GdipBitmapUnlockBits(cloneImage, &lockeddata_dst);
        }

        if (stat != Ok)
            GdipDisposeImage(cloneImage);
    }

    GdipBitmapUnlockBits(bitmap, &lockeddata_src);

    return stat;
}

Status  GdipCreateBitmapFromFile(GDIPCONST WCHAR* filename,
    Bitmap **bitmap)
{
    Status stat = Ok;

    TRACE("(%s) %p\n", debugstr_w(filename), bitmap);

    if(!filename || !bitmap)
        return InvalidParameter;

    return stat;
}

Status  GdipCreateBitmapFromGdiDib(GDIPCONST wBITMAPINFO* info,
                                               VOID *bits, Bitmap *bitmap)
{
    DWORD height;
    int stride;
    PixelFormat format;

    FIXME("(%p, %p, %p) - partially implemented\n", info, bits, bitmap);

    if (!info || !bits || !bitmap)
        return InvalidParameter;

    height = abs(info->bmiHeader.biHeight);
    stride = ((info->bmiHeader.biWidth * info->bmiHeader.biBitCount + 31) >> 3) & ~3;

    if(info->bmiHeader.biHeight > 0) /* bottom-up */
    {
        bits = (BYTE*)bits + (height - 1) * stride;
        stride = -stride;
    }

    switch(info->bmiHeader.biBitCount) {
    case 1:
        format = PixelFormat1bppIndexed;
        break;
    case 4:
        format = PixelFormat4bppIndexed;
        break;
    case 8:
        format = PixelFormat8bppIndexed;
        break;
    case 16:
        format = PixelFormat16bppRGB555;
        break;
    case 24:
        format = PixelFormat24bppRGB;
        break;
    case 32:
        format = PixelFormat32bppRGB;
        break;
    default:
        FIXME("don't know how to handle %d bpp\n", info->bmiHeader.biBitCount);
        return InvalidParameter;
    }

    return GdipCreateBitmapFromScan0(info->bmiHeader.biWidth, height, stride, format,
                                     bits, bitmap);

}

/* FIXME: no icm */
Status  GdipCreateBitmapFromFileICM(GDIPCONST WCHAR* filename,
    Bitmap **bitmap)
{
    TRACE("(%s) %p\n", debugstr_w(filename), bitmap);

    return GdipCreateBitmapFromFile(filename, bitmap);
}

Status  GdipCreateBitmapFromResource(void* hInstance,
    GDIPCONST WCHAR* lpBitmapName, Bitmap* bitmap)
{
    Status stat = InvalidParameter;

    ASSERT(0);
    return stat;
}

Status  GdipCreateHBITMAPFromBitmap(Bitmap* bitmap,
    void* hbmReturn, ARGB background)
{
  Status stat = InvalidParameter;
  
    ASSERT(0);
    return stat;
}


/* FIXME: this should create a bitmap in the given size with the attributes
 * (resolution etc.) of the graphics object */
Status  GdipCreateBitmapFromGraphics(INT width, INT height,
    Graphics* target, Bitmap* bitmap)
{
    static int calls;
    Status ret;

    TRACE("(%d, %d, %p, %p)\n", width, height, target, bitmap);

    if(!target || !bitmap)
        return InvalidParameter;

    if(!(calls++))
        FIXME("hacked stub\n");

    ret = GdipCreateBitmapFromScan0(width, height, 0, PixelFormat24bppRGB,
                                    NULL, bitmap);

    return ret;
}

#if 0
Status  GdipCreateBitmapFromHICON(HICON hicon, Bitmap* bitmap)
{
    Status stat;
    ICONINFO iinfo;
    BITMAP bm;
    int ret;
    UINT width, height;
    Rect rect;
    BitmapData lockeddata;
    HDC screendc;
    BOOL has_alpha;
    int x, y;
    BYTE *bits;
    BITMAPINFOHEADER bih;
    DWORD *src;
    BYTE *dst_row;
    DWORD *dst;

    TRACE("%p, %p\n", hicon, bitmap);

    if(!bitmap || !GetIconInfo(hicon, &iinfo))
        return InvalidParameter;

    /* get the size of the icon */
    ret = GetObjectA(iinfo.hbmColor ? iinfo.hbmColor : iinfo.hbmMask, sizeof(bm), &bm);
    if (ret == 0) {
        DeleteObject(iinfo.hbmColor);
        DeleteObject(iinfo.hbmMask);
        return GenericError;
    }

    width = bm.bmWidth;

    if (iinfo.hbmColor)
        height = abs(bm.bmHeight);
    else /* combined bitmap + mask */
        height = abs(bm.bmHeight) / 2;

    bits = HeapAlloc(GetProcessHeap(), 0, 4*width*height);
    if (!bits) {
        DeleteObject(iinfo.hbmColor);
        DeleteObject(iinfo.hbmMask);
        return OutOfMemory;
    }

    stat = GdipCreateBitmapFromScan0(width, height, 0, PixelFormat32bppARGB, NULL, bitmap);
    if (stat != Ok) {
        DeleteObject(iinfo.hbmColor);
        DeleteObject(iinfo.hbmMask);
        HeapFree(GetProcessHeap(), 0, bits);
        return stat;
    }

    rect.X = 0;
    rect.Y = 0;
    rect.Width = width;
    rect.Height = height;

    stat = GdipBitmapLockBits(*bitmap, &rect, ImageLockModeWrite, PixelFormat32bppARGB, &lockeddata);
    if (stat != Ok) {
        DeleteObject(iinfo.hbmColor);
        DeleteObject(iinfo.hbmMask);
        HeapFree(GetProcessHeap(), 0, bits);
        GdipDisposeImage((Image*)*bitmap);
        return stat;
    }

    bih.biSize = sizeof(bih);
    bih.biWidth = width;
    bih.biHeight = -height;
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    screendc = GetDC(0);
    if (iinfo.hbmColor)
    {
        GetDIBits(screendc, iinfo.hbmColor, 0, height, bits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

        if (bm.bmBitsPixel == 32)
        {
            has_alpha = FALSE;

            /* If any pixel has a non-zero alpha, ignore hbmMask */
            src = (DWORD*)bits;
            for (x=0; x<width && !has_alpha; x++)
                for (y=0; y<height && !has_alpha; y++)
                    if ((*src++ & 0xff000000) != 0)
                        has_alpha = TRUE;
        }
        else has_alpha = FALSE;
    }
    else
    {
        GetDIBits(screendc, iinfo.hbmMask, 0, height, bits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
        has_alpha = FALSE;
    }

    /* copy the image data to the Bitmap */
    src = (DWORD*)bits;
    dst_row = lockeddata.Scan0;
    for (y=0; y<height; y++)
    {
        memcpy(dst_row, src, width*4);
        src += width;
        dst_row += lockeddata.Stride;
    }

    if (!has_alpha)
    {
        if (iinfo.hbmMask)
        {
            /* read alpha data from the mask */
            if (iinfo.hbmColor)
                GetDIBits(screendc, iinfo.hbmMask, 0, height, bits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
            else
                GetDIBits(screendc, iinfo.hbmMask, height, height, bits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

            src = (DWORD*)bits;
            dst_row = lockeddata.Scan0;
            for (y=0; y<height; y++)
            {
                dst = (DWORD*)dst_row;
                for (x=0; x<height; x++)
                {
                    DWORD src_value = *src++;
                    if (src_value)
                        *dst++ = 0;
                    else
                        *dst++ |= 0xff000000;
                }
                dst_row += lockeddata.Stride;
            }
        }
        else
        {
            /* set constant alpha of 255 */
            dst_row = bits;
            for (y=0; y<height; y++)
            {
                dst = (DWORD*)dst_row;
                for (x=0; x<height; x++)
                    *dst++ |= 0xff000000;
                dst_row += lockeddata.Stride;
            }
        }
    }

    ReleaseDC(0, screendc);

    DeleteObject(iinfo.hbmColor);
    DeleteObject(iinfo.hbmMask);

    GdipBitmapUnlockBits(*bitmap, &lockeddata);

    HeapFree(GetProcessHeap(), 0, bits);

    return Ok;
}

Status  GdipCreateBitmapFromStream(IStream* stream, Bitmap **bitmap)
{
    Status stat;

    TRACE("%p %p\n", stream, bitmap);

    stat = GdipLoadImageFromStream(stream, (Image**) bitmap);

    if(stat != Ok)
        return stat;

    if(bitmap->image.type != ImageTypeBitmap){
        GdipDisposeImage(&bitmap->image);
        *bitmap = NULL;
        return GenericError; /* FIXME: what error to return? */
    }

    return Ok;
}

/* FIXME: no icm */
Status  GdipCreateBitmapFromStreamICM(IStream* stream,
    Bitmap **bitmap)
{
    TRACE("%p %p\n", stream, bitmap);

    return GdipCreateBitmapFromStream(stream, bitmap);
}

Status  GdipCreateCachedBitmap(Bitmap *bitmap, Graphics *graphics,
    CachedBitmap **cachedbmp)
{
    Status stat;

    TRACE("%p %p %p\n", bitmap, graphics, cachedbmp);

    if(!bitmap || !graphics || !cachedbmp)
        return InvalidParameter;

    *cachedbmp = GdipAlloc(sizeof(CachedBitmap));
    if(!*cachedbmp)
        return OutOfMemory;

    stat = GdipCloneImage(&(bitmap->image), &(*cachedbmp)->image);
    if(stat != Ok){
        GdipFree(*cachedbmp);
        return stat;
    }

    return Ok;
}

Status  GdipCreateHICONFromBitmap(Bitmap *bitmap, HICON *hicon)
{
    Status stat;
    BitmapData lockeddata;
    ULONG andstride, xorstride, bitssize;
    LPBYTE andbits, xorbits, androw, xorrow, srcrow;
    UINT x, y;

    TRACE("(%p, %p)\n", bitmap, hicon);

    if (!bitmap || !hicon)
        return InvalidParameter;

    stat = GdipBitmapLockBits(bitmap, NULL, ImageLockModeRead,
        PixelFormat32bppPARGB, &lockeddata);
    if (stat == Ok)
    {
        andstride = ((lockeddata.Width+31)/32)*4;
        xorstride = lockeddata.Width*4;
        bitssize = (andstride + xorstride) * lockeddata.Height;

        andbits = GdipAlloc(bitssize);

        if (andbits)
        {
            xorbits = andbits + andstride * lockeddata.Height;

            for (y=0; y<lockeddata.Height; y++)
            {
                srcrow = ((LPBYTE)lockeddata.Scan0) + lockeddata.Stride * y;

                androw = andbits + andstride * y;
                for (x=0; x<lockeddata.Width; x++)
                    if (srcrow[3+4*x] >= 128)
                        androw[x/8] |= 1 << (7-x%8);

                xorrow = xorbits + xorstride * y;
                memcpy(xorrow, srcrow, xorstride);
            }

            *hicon = CreateIcon(NULL, lockeddata.Width, lockeddata.Height, 1, 32,
                andbits, xorbits);

            GdipFree(andbits);
        }
        else
            stat = OutOfMemory;

        GdipBitmapUnlockBits(bitmap, &lockeddata);
    }

    return stat;
}

Status  GdipDeleteCachedBitmap(CachedBitmap *cachedbmp)
{
    TRACE("%p\n", cachedbmp);

    if(!cachedbmp)
        return InvalidParameter;

    GdipDisposeImage(cachedbmp->image);
    GdipFree(cachedbmp);

    return Ok;
}

Status  GdipDrawCachedBitmap(Graphics *graphics,
    CachedBitmap *cachedbmp, INT x, INT y)
{
    TRACE("%p %p %d %d\n", graphics, cachedbmp, x, y);

    if(!graphics || !cachedbmp)
        return InvalidParameter;

    return GdipDrawImage(graphics, cachedbmp->image, (REAL)x, (REAL)y);
}

Status  GdipEmfToWmfBits(HENHMETAFILE hemf, UINT cbData16,
    LPBYTE pData16, INT iMapMode, INT eFlags)
{
    FIXME("(%p, %d, %p, %d, %d): stub\n", hemf, cbData16, pData16, iMapMode, eFlags);
    return NotImplemented;
}

/* Internal utility function: Replace the image data of dst with that of src,
 * and free src. */
static void move_bitmap(Bitmap *dst, Bitmap *src, BOOL clobber_palette)
{
    if (clobber_palette)
    {
        dst->palette = src->palette;
    }

    dst->width = src->width;
    dst->height = src->height;
    dst->pixfmt = src->pixfmt;
    dst->bits = src->bits;
    dst->stride = src->stride;
    dst->frame_count = src->frame_count;
    dst->image.current_frame = src->image.current_frame;
    dst->image.format = src->image.format;

    src->image.type = ~0;
    INFOSTR_free(src);
}

Status  GdipFindFirstImageItem(Image *image, ImageItemData* item)
{
    static int calls;

    TRACE("(%p,%p)\n", image, item);

    if(!image || !item)
        return InvalidParameter;

    if (!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetImageItemData(Image *image, ImageItemData *item)
{
    static int calls;

    TRACE("(%p,%p)\n", image, item);

    if (!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipGetImageBounds(Image *image, RectF *srcRect,
    Unit *srcUnit)
{
    TRACE("%p %p %p\n", image, srcRect, srcUnit);

    if(!image || !srcRect || !srcUnit)
        return InvalidParameter;
    if(image->type == ImageTypeMetafile){
        *srcRect = ((Metafile*)image)->bounds;
        *srcUnit = ((Metafile*)image)->unit;
    }
    else if(image->type == ImageTypeBitmap){
        srcRect->X = srcRect->Y = 0.0;
        srcRect->Width = (REAL) ((Bitmap*)image)->width;
        srcRect->Height = (REAL) ((Bitmap*)image)->height;
        *srcUnit = UnitPixel;
    }
    else{
        srcRect->X = srcRect->Y = 0.0;
        srcRect->Width = ipicture_pixel_width(image->picture);
        srcRect->Height = ipicture_pixel_height(image->picture);
        *srcUnit = UnitPixel;
    }

    TRACE("returning (%f, %f) (%f, %f) unit type %d\n", srcRect->X, srcRect->Y,
          srcRect->Width, srcRect->Height, *srcUnit);

    return Ok;
}

Status  GdipGetImageDimension(Image *image, REAL *width,
    REAL *height)
{
    TRACE("%p %p %p\n", image, width, height);

    if(!image || !height || !width)
        return InvalidParameter;

    *height = (image)->height;
    *width = (image)->width;
    
    TRACE("returning (%f, %f)\n", *height, *width);
    return Ok;
}

Status  GdipGetImageGraphicsContext(Image *image,
    Graphics **graphics)
{
    HDC hdc;
    Status stat;

    TRACE("%p %p\n", image, graphics);

    if(!image || !graphics)
        return InvalidParameter;

    if (image->type == ImageTypeBitmap && ((Bitmap*)image)->hbitmap)
    {
        hdc = ((Bitmap*)image)->hdc;

        if(!hdc){
            hdc = CreateCompatibleDC(0);
            SelectObject(hdc, ((Bitmap*)image)->hbitmap);
            ((Bitmap*)image)->hdc = hdc;
        }

        stat = GdipCreateFromHDC(hdc, graphics);

        if (stat == Ok)
        {
            (*graphics)->image = image;
            (*graphics)->xres = image->xres;
            (*graphics)->yres = image->yres;
        }
    }
    else if (image->type == ImageTypeMetafile)
        stat = METAFILE_GetGraphicsContext((Metafile*)image, graphics);
    else
        stat = graphics_from_image(image, graphics);

    return stat;
}

Status  GdipGetImageHeight(Image *image, UINT *height)
{
    TRACE("%p %p\n", image, height);

    if(!image || !height)
        return InvalidParameter;

    *height = ((Bitmap*)image)->height;

    TRACE("returning %d\n", *height);

    return Ok;
}

Status  GdipGetImageHorizontalResolution(Image *image, REAL *res)
{
    if(!image || !res)
        return InvalidParameter;

    *res = image->xres;

    TRACE("(%p) <-- %0.2f\n", image, *res);

    return Ok;
}

Status  GdipGetImagePaletteSize(Image *image, INT *size)
{
    TRACE("%p %p\n", image, size);

    if(!image || !size)
        return InvalidParameter;

    if (!image->palette || image->palette->Count == 0)
        *size = sizeof(ColorPalette);
    else
        *size = sizeof(UINT)*2 + sizeof(ARGB)*image->palette->Count;

    TRACE("<-- %u\n", *size);

    return Ok;
}

/* FIXME: test this function for non-bitmap types */
Status  GdipGetImagePixelFormat(Image *image, PixelFormat *format)
{
    TRACE("%p %p\n", image, format);

    if(!image || !format)
        return InvalidParameter;

    if(image->type != ImageTypeBitmap)
        *format = PixelFormat24bppRGB;
    else
        *format = ((Bitmap*) image)->pixfmt;

    return Ok;
}

Status  GdipGetImageRawFormat(Image *image, GUID *format)
{
    TRACE("(%p, %p)\n", image, format);

    if(!image || !format)
        return InvalidParameter;

    memcpy(format, &image->pixfmt, sizeof(GUID));

    return Ok;
}

Status  GdipGetImageType(Image *image, ImageType *type)
{
    TRACE("%p %p\n", image, type);

    if(!image || !type)
        return InvalidParameter;

    *type = image->type;

    return Ok;
}

Status  GdipGetImageVerticalResolution(Image *image, REAL *res)
{
    if(!image || !res)
        return InvalidParameter;

    *res = image->yres;

    TRACE("(%p) <-- %0.2f\n", image, *res);

    return Ok;
}

Status  GdipGetImageWidth(Image *image, UINT *width)
{
    TRACE("%p %p\n", image, width);

    if(!image || !width)
        return InvalidParameter;

    if(image->type == ImageTypeMetafile)
        *width = units_to_pixels(((Metafile*)image)->bounds.Width, ((Metafile*)image)->unit, image->xres);
    else if(image->type == ImageTypeBitmap)
        *width = ((Bitmap*)image)->width;
    else
        *width = ipicture_pixel_width(image->picture);

    TRACE("returning %d\n", *width);

    return Ok;
}

Status  GdipGetMetafileHeaderFromMetafile(Metafile * metafile,
    MetafileHeader * header)
{
    static int calls;

    TRACE("(%p, %p)\n", metafile, header);

    if(!metafile || !header)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    memset(header, 0, sizeof(MetafileHeader));

    return Ok;
}

Status  GdipGetMetafileHeaderFromEmf(HENHMETAFILE hEmf,
    MetafileHeader *header)
{
    static int calls;

    if(!hEmf || !header)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    memset(header, 0, sizeof(MetafileHeader));

    return Ok;
}

Status  GdipGetMetafileHeaderFromFile(GDIPCONST WCHAR *filename,
    MetafileHeader *header)
{
    static int calls;

    TRACE("(%s,%p)\n", debugstr_w(filename), header);

    if(!filename || !header)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    memset(header, 0, sizeof(MetafileHeader));

    return Ok;
}

Status  GdipGetMetafileHeaderFromStream(IStream *stream,
    MetafileHeader *header)
{
    static int calls;

    TRACE("(%p,%p)\n", stream, header);

    if(!stream || !header)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    memset(header, 0, sizeof(MetafileHeader));

    return Ok;
}

Status  GdipGetPropertyCount(Image *image, UINT *num)
{
    TRACE("(%p, %p)\n", image, num);

    if (!image || !num) return InvalidParameter;

    *num = 0;

    if (image->type == ImageTypeBitmap)
    {
        if (((Bitmap *)image)->prop_item)
        {
            *num = ((Bitmap *)image)->prop_count;
            return Ok;
        }

        if (((Bitmap *)image)->metadata_reader)
            IWICMetadataReader_GetCount(((Bitmap *)image)->metadata_reader, num);
    }

    return Ok;
}

Status  GdipGetPropertyIdList(Image *image, UINT num, PROPID *list)
{
    HRESULT hr;
    IWICMetadataReader *reader;
    IWICEnumMetadataItem *enumerator;
    UINT prop_count, i, items_returned;

    TRACE("(%p, %u, %p)\n", image, num, list);

    if (!image || !list) return InvalidParameter;

    if (image->type != ImageTypeBitmap)
    {
        FIXME("Not implemented for type %d\n", image->type);
        return NotImplemented;
    }

    if (((Bitmap *)image)->prop_item)
    {
        if (num != ((Bitmap *)image)->prop_count) return InvalidParameter;

        for (i = 0; i < num; i++)
        {
            list[i] = ((Bitmap *)image)->prop_item[i].id;
        }

        return Ok;
    }

    reader = ((Bitmap *)image)->metadata_reader;
    if (!reader)
    {
        if (num != 0) return InvalidParameter;
        return Ok;
    }

    hr = IWICMetadataReader_GetCount(reader, &prop_count);
    if (FAILED(hr)) return hresult_to_status(hr);

    if (num != prop_count) return InvalidParameter;

    hr = IWICMetadataReader_GetEnumerator(reader, &enumerator);
    if (FAILED(hr)) return hresult_to_status(hr);

    IWICEnumMetadataItem_Reset(enumerator);

    for (i = 0; i < num; i++)
    {
        PROPVARIANT id;

        hr = IWICEnumMetadataItem_Next(enumerator, 1, NULL, &id, NULL, &items_returned);
        if (hr != S_OK) break;

        if (id.vt != VT_UI2)
        {
            FIXME("not supported propvariant type for id: %u\n", id.vt);
            list[i] = 0;
            continue;
        }
        list[i] = id.u.uiVal;
    }

    IWICEnumMetadataItem_Release(enumerator);

    return hr == S_OK ? Ok : hresult_to_status(hr);
}

static UINT propvariant_size(PROPVARIANT *value)
{
    switch (value->vt & ~VT_VECTOR)
    {
    case VT_EMPTY:
        return 0;
    case VT_I1:
    case VT_UI1:
        if (!(value->vt & VT_VECTOR)) return 1;
        return value->u.caub.cElems;
    case VT_I2:
    case VT_UI2:
        if (!(value->vt & VT_VECTOR)) return 2;
        return value->u.caui.cElems * 2;
    case VT_I4:
    case VT_UI4:
    case VT_R4:
        if (!(value->vt & VT_VECTOR)) return 4;
        return value->u.caul.cElems * 4;
    case VT_I8:
    case VT_UI8:
    case VT_R8:
        if (!(value->vt & VT_VECTOR)) return 8;
        return value->u.cauh.cElems * 8;
    case VT_LPSTR:
        return value->u.pszVal ? strlen(value->u.pszVal) + 1 : 0;
    case VT_BLOB:
        return value->u.blob.cbSize;
    default:
        FIXME("not supported variant type %d\n", value->vt);
        return 0;
    }
}

Status  GdipGetPropertyItemSize(Image *image, PROPID propid, UINT *size)
{
    HRESULT hr;
    IWICMetadataReader *reader;
    PROPVARIANT id, value;

    TRACE("(%p,%#x,%p)\n", image, propid, size);

    if (!size || !image) return InvalidParameter;

    if (image->type != ImageTypeBitmap)
    {
        FIXME("Not implemented for type %d\n", image->type);
        return NotImplemented;
    }

    if (((Bitmap *)image)->prop_item)
    {
        UINT i;

        for (i = 0; i < ((Bitmap *)image)->prop_count; i++)
        {
            if (propid == ((Bitmap *)image)->prop_item[i].id)
            {
                *size = sizeof(PropertyItem) + ((Bitmap *)image)->prop_item[i].length;
                return Ok;
            }
        }

        return PropertyNotFound;
    }

    reader = ((Bitmap *)image)->metadata_reader;
    if (!reader) return PropertyNotFound;

    id.vt = VT_UI2;
    id.u.uiVal = propid;
    hr = IWICMetadataReader_GetValue(reader, NULL, &id, &value);
    if (FAILED(hr)) return PropertyNotFound;

    *size = propvariant_size(&value);
    if (*size) *size += sizeof(PropertyItem);
    PropVariantClear(&value);

    return Ok;
}


Status  GdipGetPropertyItem(Image *image, PROPID propid, UINT size,
                                        PropertyItem *buffer)
{
    Status stat;
    HRESULT hr;
    IWICMetadataReader *reader;
    PROPVARIANT id, value;

    TRACE("(%p,%#x,%u,%p)\n", image, propid, size, buffer);

    if (!image || !buffer) return InvalidParameter;

    if (image->type != ImageTypeBitmap)
    {
        FIXME("Not implemented for type %d\n", image->type);
        return NotImplemented;
    }

    if (((Bitmap *)image)->prop_item)
    {
        UINT i;

        for (i = 0; i < ((Bitmap *)image)->prop_count; i++)
        {
            if (propid == ((Bitmap *)image)->prop_item[i].id)
            {
                if (size != sizeof(PropertyItem) + ((Bitmap *)image)->prop_item[i].length)
                    return InvalidParameter;

                *buffer = ((Bitmap *)image)->prop_item[i];
                buffer->value = buffer + 1;
                memcpy(buffer->value, ((Bitmap *)image)->prop_item[i].value, buffer->length);
                return Ok;
            }
        }

        return PropertyNotFound;
    }

    reader = ((Bitmap *)image)->metadata_reader;
    if (!reader) return PropertyNotFound;

    id.vt = VT_UI2;
    id.u.uiVal = propid;
    hr = IWICMetadataReader_GetValue(reader, NULL, &id, &value);
    if (FAILED(hr)) return PropertyNotFound;

    stat = propvariant_to_item(&value, buffer, size, propid);
    PropVariantClear(&value);

    return stat;
}

Status  GdipGetPropertySize(Image *image, UINT *size, UINT *count)
{
    HRESULT hr;
    IWICMetadataReader *reader;
    IWICEnumMetadataItem *enumerator;
    UINT prop_count, prop_size, i;
    PROPVARIANT id, value;

    TRACE("(%p,%p,%p)\n", image, size, count);

    if (!image || !size || !count) return InvalidParameter;

    if (image->type != ImageTypeBitmap)
    {
        FIXME("Not implemented for type %d\n", image->type);
        return NotImplemented;
    }

    if (((Bitmap *)image)->prop_item)
    {
        *count = ((Bitmap *)image)->prop_count;
        *size = 0;

        for (i = 0; i < ((Bitmap *)image)->prop_count; i++)
        {
            *size += sizeof(PropertyItem) + ((Bitmap *)image)->prop_item[i].length;
        }

        return Ok;
    }

    reader = ((Bitmap *)image)->metadata_reader;
    if (!reader) return PropertyNotFound;

    hr = IWICMetadataReader_GetCount(reader, &prop_count);
    if (FAILED(hr)) return hresult_to_status(hr);

    hr = IWICMetadataReader_GetEnumerator(reader, &enumerator);
    if (FAILED(hr)) return hresult_to_status(hr);

    IWICEnumMetadataItem_Reset(enumerator);

    prop_size = 0;

    PropVariantInit(&id);
    PropVariantInit(&value);

    for (i = 0; i < prop_count; i++)
    {
        UINT items_returned, item_size;

        hr = IWICEnumMetadataItem_Next(enumerator, 1, NULL, &id, &value, &items_returned);
        if (hr != S_OK) break;

        item_size = propvariant_size(&value);
        if (item_size) prop_size += sizeof(PropertyItem) + item_size;

        PropVariantClear(&id);
        PropVariantClear(&value);
    }

    IWICEnumMetadataItem_Release(enumerator);

    if (hr != S_OK) return PropertyNotFound;

    *count = prop_count;
    *size = prop_size;
    return Ok;
}

Status  GdipGetAllPropertyItems(Image *image, UINT size,
                                            UINT count, PropertyItem *buf)
{
    Status status;
    HRESULT hr;
    IWICMetadataReader *reader;
    IWICEnumMetadataItem *enumerator;
    UINT prop_count, prop_size, i;
    PROPVARIANT id, value;
    char *item_value;

    TRACE("(%p,%u,%u,%p)\n", image, size, count, buf);

    if (!image || !buf) return InvalidParameter;

    if (image->type != ImageTypeBitmap)
    {
        FIXME("Not implemented for type %d\n", image->type);
        return NotImplemented;
    }

    status = GdipGetPropertySize(image, &prop_size, &prop_count);
    if (status != Ok) return status;

    if (prop_count != count || prop_size != size) return InvalidParameter;

    if (((Bitmap *)image)->prop_item)
    {
        memcpy(buf, ((Bitmap *)image)->prop_item, prop_size);

        item_value = (char *)(buf + prop_count);

        for (i = 0; i < prop_count; i++)
        {
            buf[i].value = item_value;
            item_value += buf[i].length;
        }

        return Ok;
    }

    reader = ((Bitmap *)image)->metadata_reader;
    if (!reader) return PropertyNotFound;

    hr = IWICMetadataReader_GetEnumerator(reader, &enumerator);
    if (FAILED(hr)) return hresult_to_status(hr);

    IWICEnumMetadataItem_Reset(enumerator);

    item_value = (char *)(buf + prop_count);

    PropVariantInit(&id);
    PropVariantInit(&value);

    for (i = 0; i < prop_count; i++)
    {
        PropertyItem *item;
        UINT items_returned, item_size;

        hr = IWICEnumMetadataItem_Next(enumerator, 1, NULL, &id, &value, &items_returned);
        if (hr != S_OK) break;

        if (id.vt != VT_UI2)
        {
            FIXME("not supported propvariant type for id: %u\n", id.vt);
            continue;
        }

        item_size = propvariant_size(&value);
        if (item_size)
        {
            item = HeapAlloc(GetProcessHeap(), 0, item_size + sizeof(*item));

            propvariant_to_item(&value, item, item_size + sizeof(*item), id.u.uiVal);
            buf[i].id = item->id;
            buf[i].type = item->type;
            buf[i].length = item_size;
            buf[i].value = item_value;
            memcpy(item_value, item->value, item_size);
            item_value += item_size;

            HeapFree(GetProcessHeap(), 0, item);
        }

        PropVariantClear(&id);
        PropVariantClear(&value);
    }

    IWICEnumMetadataItem_Release(enumerator);

    if (hr != S_OK) return PropertyNotFound;

    return Ok;
}

struct image_format_dimension
{
    const GUID *format;
    const GUID *dimension;
};

static const struct image_format_dimension image_format_dimensions[] =
{
    {&ImageFormatGIF, &FrameDimensionTime},
    {&ImageFormatIcon, &FrameDimensionResolution},
    {NULL}
};

Status  GdipImageGetFrameCount(Image *image,
    GDIPCONST GUID* dimensionID, UINT* count)
{
    TRACE("(%p,%s,%p)\n", image, debugstr_guid(dimensionID), count);

    if(!image || !count)
        return InvalidParameter;

    if (!dimensionID ||
        IsEqualGUID(dimensionID, &image->pixfmt) ||
        IsEqualGUID(dimensionID, &FrameDimensionPage) ||
        IsEqualGUID(dimensionID, &FrameDimensionTime))
    {
        *count = image->frame_count;
        return Ok;
    }

    return InvalidParameter;
}

Status  GdipImageGetFrameDimensionsCount(Image *image,
    UINT* count)
{
    TRACE("(%p, %p)\n", image, count);

    /* Native gdiplus 1.1 does not yet support multiple frame dimensions. */

    if(!image || !count)
        return InvalidParameter;

    *count = 1;

    return Ok;
}

Status  GdipImageGetFrameDimensionsList(Image* image,
    GUID* dimensionIDs, UINT count)
{
    int i;
    const GUID *result=NULL;

    TRACE("(%p,%p,%u)\n", image, dimensionIDs, count);

    if(!image || !dimensionIDs || count != 1)
        return InvalidParameter;

    for (i=0; image_format_dimensions[i].format; i++)
    {
        if (IsEqualGUID(&image->pixfmt, image_format_dimensions[i].format))
        {
            result = image_format_dimensions[i].dimension;
            break;
        }
    }

    if (!result)
        result = &FrameDimensionPage;

    memcpy(dimensionIDs, result, sizeof(GUID));

    return Ok;
}

Status  GdipLoadImageFromFile(GDIPCONST WCHAR* filename,
                                          Image **image)
{
    Status stat;
    IStream *stream;

    TRACE("(%s) %p\n", debugstr_w(filename), image);

    if (!filename || !image)
        return InvalidParameter;

    stat = GdipCreateStreamOnFile(filename, GENERIC_READ, &stream);

    if (stat != Ok)
        return stat;

    stat = GdipLoadImageFromStream(stream, image);

    IStream_Release(stream);

    return stat;
}

/* FIXME: no icm handling */
Status  GdipLoadImageFromFileICM(GDIPCONST WCHAR* filename,Image **image)
{
    TRACE("(%s) %p\n", debugstr_w(filename), image);

    return GdipLoadImageFromFile(filename, image);
}

static void add_property(Bitmap *bitmap, PropertyItem *item)
{
    UINT prop_size, prop_count;
    PropertyItem *prop_item;

    if (bitmap->prop_item == NULL)
    {
        prop_size = prop_count = 0;
        prop_item = GdipAlloc(item->length + sizeof(PropertyItem));
        if (!prop_item) return;
    }
    else
    {
        UINT i;
        char *item_value;

        GdipGetPropertySize((Image *)bitmap, &prop_size, &prop_count);

        prop_item = GdipAlloc(prop_size + item->length + sizeof(PropertyItem));
        if (!prop_item) return;
        memcpy(prop_item, bitmap->prop_item, sizeof(PropertyItem) * bitmap->prop_count);
        prop_size -= sizeof(PropertyItem) * bitmap->prop_count;
        memcpy(prop_item + prop_count + 1, bitmap->prop_item + prop_count, prop_size);

        item_value = (char *)(prop_item + prop_count + 1);

        for (i = 0; i < prop_count; i++)
        {
            prop_item[i].value = item_value;
            item_value += prop_item[i].length;
        }
    }

    prop_item[prop_count].id = item->id;
    prop_item[prop_count].type = item->type;
    prop_item[prop_count].length = item->length;
    prop_item[prop_count].value = (char *)(prop_item + prop_count + 1) + prop_size;
    memcpy(prop_item[prop_count].value, item->value, item->length);

    GdipFree(bitmap->prop_item);
    bitmap->prop_item = prop_item;
    bitmap->prop_count++;
}

static BOOL get_bool_property(IWICMetadataReader *reader, const GUID *guid, const WCHAR *prop_name)
{
    HRESULT hr;
    GUID format;
    PROPVARIANT id, value;
    BOOL ret = FALSE;

    IWICMetadataReader_GetMetadataFormat(reader, &format);
    if (!IsEqualGUID(&format, guid)) return FALSE;

    PropVariantInit(&id);
    PropVariantInit(&value);

    id.vt = VT_LPWSTR;
    id.u.pwszVal = HeapAlloc(GetProcessHeap(), 0, (lstrlenW(prop_name) + 1) * sizeof(WCHAR));
    if (!id.u.pwszVal) return FALSE;
    lstrcpyW(id.u.pwszVal, prop_name);
    hr = IWICMetadataReader_GetValue(reader, NULL, &id, &value);
    if (hr == S_OK && value.vt == VT_BOOL)
        ret = value.u.boolVal;

    PropVariantClear(&id);
    PropVariantClear(&value);

    return ret;
}

static PropertyItem *get_property(IWICMetadataReader *reader, const GUID *guid, const WCHAR *prop_name)
{
    HRESULT hr;
    GUID format;
    PROPVARIANT id, value;
    PropertyItem *item = NULL;

    IWICMetadataReader_GetMetadataFormat(reader, &format);
    if (!IsEqualGUID(&format, guid)) return NULL;

    PropVariantInit(&id);
    PropVariantInit(&value);

    id.vt = VT_LPWSTR;
    id.u.pwszVal = HeapAlloc(GetProcessHeap(), 0, (lstrlenW(prop_name) + 1) * sizeof(WCHAR));
    if (!id.u.pwszVal) return NULL;
    lstrcpyW(id.u.pwszVal, prop_name);
    hr = IWICMetadataReader_GetValue(reader, NULL, &id, &value);
    if (hr == S_OK)
    {
        UINT item_size = propvariant_size(&value);
        if (item_size)
        {
            item_size += sizeof(*item);
            item = GdipAlloc(item_size);
            if (propvariant_to_item(&value, item, item_size, 0) != Ok)
            {
                GdipFree(item);
                item = NULL;
            }
        }
    }

    PropVariantClear(&id);
    PropVariantClear(&value);

    return item;
}

static PropertyItem *get_gif_comment(IWICMetadataReader *reader)
{
    static const WCHAR textentryW[] = { 'T','e','x','t','E','n','t','r','y',0 };
    PropertyItem *comment;

    comment = get_property(reader, &GUID_MetadataFormatGifComment, textentryW);
    if (comment)
        comment->id = PropertyTagExifUserComment;

    return comment;
}

static PropertyItem *get_gif_loopcount(IWICMetadataReader *reader)
{
    static const WCHAR applicationW[] = { 'A','p','p','l','i','c','a','t','i','o','n',0 };
    static const WCHAR dataW[] = { 'D','a','t','a',0 };
    PropertyItem *appext = NULL, *appdata = NULL, *loop = NULL;

    appext = get_property(reader, &GUID_MetadataFormatAPE, applicationW);
    if (appext)
    {
        if (appext->type == PropertyTagTypeByte && appext->length == 11 &&
            (!memcmp(appext->value, "NETSCAPE2.0", 11) || !memcmp(appext->value, "ANIMEXTS1.0", 11)))
        {
            appdata = get_property(reader, &GUID_MetadataFormatAPE, dataW);
            if (appdata)
            {
                if (appdata->type == PropertyTagTypeByte && appdata->length == 4)
                {
                    BYTE *data = appdata->value;
                    if (data[0] == 3 && data[1] == 1)
                    {
                        loop = GdipAlloc(sizeof(*loop) + sizeof(SHORT));
                        if (loop)
                        {
                            loop->type = PropertyTagTypeShort;
                            loop->id = PropertyTagLoopCount;
                            loop->length = sizeof(SHORT);
                            loop->value = loop + 1;
                            *(SHORT *)loop->value = data[2] | (data[3] << 8);
                        }
                    }
                }
            }
        }
    }

    GdipFree(appext);
    GdipFree(appdata);

    return loop;
}

static PropertyItem *get_gif_background(IWICMetadataReader *reader)
{
    static const WCHAR backgroundW[] = { 'B','a','c','k','g','r','o','u','n','d','C','o','l','o','r','I','n','d','e','x',0 };
    PropertyItem *background;

    background = get_property(reader, &GUID_MetadataFormatLSD, backgroundW);
    if (background)
        background->id = PropertyTagIndexBackground;

    return background;
}

static PropertyItem *get_gif_palette(IWICBitmapDecoder *decoder, IWICMetadataReader *reader)
{
    static const WCHAR global_flagW[] = { 'G','l','o','b','a','l','C','o','l','o','r','T','a','b','l','e','F','l','a','g',0 };
    HRESULT hr;
    IWICImagingFactory *factory;
    IWICPalette *palette;
    UINT count = 0;
    WICColor colors[256];

    if (!get_bool_property(reader, &GUID_MetadataFormatLSD, global_flagW))
        return NULL;

    hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                          &IID_IWICImagingFactory, (void **)&factory);
    if (hr != S_OK) return NULL;

    hr = IWICImagingFactory_CreatePalette(factory, &palette);
    if (hr == S_OK)
    {
        hr = IWICBitmapDecoder_CopyPalette(decoder, palette);
        if (hr == S_OK)
            IWICPalette_GetColors(palette, 256, colors, &count);

        IWICPalette_Release(palette);
    }

    IWICImagingFactory_Release(factory);

    if (count)
    {
        PropertyItem *pal;
        UINT i;
        BYTE *rgb;

        pal = GdipAlloc(sizeof(*pal) + count * 3);
        if (!pal) return NULL;
        pal->type = PropertyTagTypeByte;
        pal->id = PropertyTagGlobalPalette;
        pal->value = pal + 1;
        pal->length = count * 3;

        rgb = pal->value;

        for (i = 0; i < count; i++)
        {
            rgb[i*3] = (colors[i] >> 16) & 0xff;
            rgb[i*3 + 1] = (colors[i] >> 8) & 0xff;
            rgb[i*3 + 2] = colors[i] & 0xff;
        }

        return pal;
    }

    return NULL;
}

static PropertyItem *get_gif_transparent_idx(IWICMetadataReader *reader)
{
    static const WCHAR transparency_flagW[] = { 'T','r','a','n','s','p','a','r','e','n','c','y','F','l','a','g',0 };
    static const WCHAR colorW[] = { 'T','r','a','n','s','p','a','r','e','n','t','C','o','l','o','r','I','n','d','e','x',0 };
    PropertyItem *index = NULL;

    if (get_bool_property(reader, &GUID_MetadataFormatGCE, transparency_flagW))
    {
        index = get_property(reader, &GUID_MetadataFormatGCE, colorW);
        if (index)
            index->id = PropertyTagIndexTransparent;
    }
    return index;
}

static LONG get_gif_frame_delay(IWICBitmapFrameDecode *frame)
{
    static const WCHAR delayW[] = { 'D','e','l','a','y',0 };
    HRESULT hr;
    IWICMetadataBlockReader *block_reader;
    IWICMetadataReader *reader;
    UINT block_count, i;
    PropertyItem *delay;
    LONG value = 0;

    hr = IWICBitmapFrameDecode_QueryInterface(frame, &IID_IWICMetadataBlockReader, (void **)&block_reader);
    if (hr == S_OK)
    {
        hr = IWICMetadataBlockReader_GetCount(block_reader, &block_count);
        if (hr == S_OK)
        {
            for (i = 0; i < block_count; i++)
            {
                hr = IWICMetadataBlockReader_GetReaderByIndex(block_reader, i, &reader);
                if (hr == S_OK)
                {
                    delay = get_property(reader, &GUID_MetadataFormatGCE, delayW);
                    if (delay)
                    {
                        if (delay->type == PropertyTagTypeShort && delay->length == 2)
                            value = *(SHORT *)delay->value;

                        GdipFree(delay);
                    }
                    IWICMetadataReader_Release(reader);
                }
            }
        }
        IWICMetadataBlockReader_Release(block_reader);
    }

    return value;
}

static void gif_metadata_reader(Bitmap *bitmap, IWICBitmapDecoder *decoder, UINT active_frame)
{
    HRESULT hr;
    IWICBitmapFrameDecode *frame;
    IWICMetadataBlockReader *block_reader;
    IWICMetadataReader *reader;
    UINT frame_count, block_count, i;
    PropertyItem *delay = NULL, *comment = NULL, *background = NULL;
    PropertyItem *transparent_idx = NULL, *loop = NULL, *palette = NULL;

    IWICBitmapDecoder_GetFrameCount(decoder, &frame_count);
    if (frame_count > 1)
    {
        delay = GdipAlloc(sizeof(*delay) + frame_count * sizeof(LONG));
        if (delay)
        {
            LONG *value;

            delay->type = PropertyTagTypeLong;
            delay->id = PropertyTagFrameDelay;
            delay->length = frame_count * sizeof(LONG);
            delay->value = delay + 1;

            value = delay->value;

            for (i = 0; i < frame_count; i++)
            {
                hr = IWICBitmapDecoder_GetFrame(decoder, i, &frame);
                if (hr == S_OK)
                {
                    value[i] = get_gif_frame_delay(frame);
                    IWICBitmapFrameDecode_Release(frame);
                }
                else value[i] = 0;
            }
        }
    }

    hr = IWICBitmapDecoder_QueryInterface(decoder, &IID_IWICMetadataBlockReader, (void **)&block_reader);
    if (hr == S_OK)
    {
        hr = IWICMetadataBlockReader_GetCount(block_reader, &block_count);
        if (hr == S_OK)
        {
            for (i = 0; i < block_count; i++)
            {
                hr = IWICMetadataBlockReader_GetReaderByIndex(block_reader, i, &reader);
                if (hr == S_OK)
                {
                    if (!comment)
                        comment = get_gif_comment(reader);

                    if (frame_count > 1 && !loop)
                        loop = get_gif_loopcount(reader);

                    if (!background)
                        background = get_gif_background(reader);

                    if (!palette)
                        palette = get_gif_palette(decoder, reader);

                    IWICMetadataReader_Release(reader);
                }
            }
        }
        IWICMetadataBlockReader_Release(block_reader);
    }

    if (frame_count > 1 && !loop)
    {
        loop = GdipAlloc(sizeof(*loop) + sizeof(SHORT));
        if (loop)
        {
            loop->type = PropertyTagTypeShort;
            loop->id = PropertyTagLoopCount;
            loop->length = sizeof(SHORT);
            loop->value = loop + 1;
            *(SHORT *)loop->value = 1;
        }
    }

    if (delay) add_property(bitmap, delay);
    if (comment) add_property(bitmap, comment);
    if (loop) add_property(bitmap, loop);
    if (palette) add_property(bitmap, palette);
    if (background) add_property(bitmap, background);

    GdipFree(delay);
    GdipFree(comment);
    GdipFree(loop);
    GdipFree(palette);
    GdipFree(background);

    /* Win7 gdiplus always returns transparent color index from frame 0 */
    hr = IWICBitmapDecoder_GetFrame(decoder, 0, &frame);
    if (hr != S_OK) return;

    hr = IWICBitmapFrameDecode_QueryInterface(frame, &IID_IWICMetadataBlockReader, (void **)&block_reader);
    if (hr == S_OK)
    {
        hr = IWICMetadataBlockReader_GetCount(block_reader, &block_count);
        if (hr == S_OK)
        {
            for (i = 0; i < block_count; i++)
            {
                hr = IWICMetadataBlockReader_GetReaderByIndex(block_reader, i, &reader);
                if (hr == S_OK)
                {
                    if (!transparent_idx)
                        transparent_idx = get_gif_transparent_idx(reader);

                    IWICMetadataReader_Release(reader);
                }
            }
        }
        IWICMetadataBlockReader_Release(block_reader);
    }

    if (transparent_idx) add_property(bitmap, transparent_idx);
    GdipFree(transparent_idx);

    IWICBitmapFrameDecode_Release(frame);
}

typedef void (*metadata_reader_func)(Bitmap *bitmap, IWICBitmapDecoder *decoder, UINT frame);


static Status decode_image_icon(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    return decode_image_wic(stream, &CLSID_WICIcoDecoder, active_frame, NULL, image);
}

static Status decode_image_bmp(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    Status status;
    Bitmap* bitmap;

    status = decode_image_wic(stream, &CLSID_WICBmpDecoder, active_frame, NULL, image);

    bitmap = (Bitmap*)*image;

    if (status == Ok && bitmap->pixfmt == PixelFormat32bppARGB)
    {
        /* WIC supports bmp files with alpha, but gdiplus does not */
        bitmap->pixfmt = PixelFormat32bppRGB;
    }

    return status;
}

static Status decode_image_jpeg(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    return decode_image_wic(stream, &CLSID_WICJpegDecoder, active_frame, NULL, image);
}

static Status decode_image_png(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    return decode_image_wic(stream, &CLSID_WICPngDecoder, active_frame, NULL, image);
}

static Status decode_image_gif(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    return decode_image_wic(stream, &CLSID_WICGifDecoder, active_frame, gif_metadata_reader, image);
}

static Status decode_image_tiff(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    return decode_image_wic(stream, &CLSID_WICTiffDecoder, active_frame, NULL, image);
}

static Status decode_image_olepicture_metafile(IStream* stream, REFCLSID clsid, UINT active_frame, Image **image)
{
    return Ok;
}

typedef Status (*encode_image_func)(Image *image, IStream* stream,
    GDIPCONST CLSID* clsid, GDIPCONST EncoderParameters* params);

typedef Status (*decode_image_func)(IStream *stream, REFCLSID clsid, UINT active_frame, Image **image);

typedef struct image_codec {
    ImageCodecInfo info;
    encode_image_func encode_func;
    decode_image_func decode_func;
} image_codec;

typedef enum {
    BMP,
    JPEG,
    GIF,
    TIFF,
    EMF,
    WMF,
    PNG,
    ICO,
    NUM_CODECS
} ImageFormat;

static const struct image_codec codecs[NUM_CODECS];

static Status get_decoder_info(IStream* stream, const struct image_codec **result)
{
    BYTE signature[8];
    const BYTE *pattern, *mask;
    LARGE_INTEGER seek;
    HRESULT hr;
    UINT bytesread;
    int i, j, sig;

    /* seek to the start of the stream */
    seek.QuadPart = 0;
    hr = IStream_Seek(stream, seek, STREAM_SEEK_SET, NULL);
    if (FAILED(hr)) return hresult_to_status(hr);

    /* read the first 8 bytes */
    /* FIXME: This assumes all codecs have signatures <= 8 bytes in length */
    hr = IStream_Read(stream, signature, 8, &bytesread);
    if (FAILED(hr)) return hresult_to_status(hr);
    if (hr == S_FALSE || bytesread == 0) return GenericError;

    for (i = 0; i < NUM_CODECS; i++) {
        if ((codecs[i].info.Flags & ImageCodecFlagsDecoder) &&
            bytesread >= codecs[i].info.SigSize)
        {
            for (sig=0; sig<codecs[i].info.SigCount; sig++)
            {
                pattern = &codecs[i].info.SigPattern[codecs[i].info.SigSize*sig];
                mask = &codecs[i].info.SigMask[codecs[i].info.SigSize*sig];
                for (j=0; j<codecs[i].info.SigSize; j++)
                    if ((signature[j] & mask[j]) != pattern[j])
                        break;
                if (j == codecs[i].info.SigSize)
                {
                    *result = &codecs[i];
                    return Ok;
                }
            }
        }
    }

    TRACE("no match for %i byte signature %x %x %x %x %x %x %x %x\n", bytesread,
        signature[0],signature[1],signature[2],signature[3],
        signature[4],signature[5],signature[6],signature[7]);

    return GenericError;
}

Status  GdipImageSelectActiveFrame(Image *image, GDIPCONST GUID *dimensionID,
                                               UINT frame)
{
    Status stat;
    LARGE_INTEGER seek;
    HRESULT hr;
    const struct image_codec *codec = NULL;
    Image *new_image;

    TRACE("(%p,%s,%u)\n", image, debugstr_guid(dimensionID), frame);

    if (!image || !dimensionID)
        return InvalidParameter;

    if (frame >= image->frame_count)
    {
        WARN("requested frame %u, but image has only %u\n", frame, image->frame_count);
        return InvalidParameter;
    }

    if (image->type != ImageTypeBitmap && image->type != ImageTypeMetafile)
    {
        WARN("invalid image type %d\n", image->type);
        return InvalidParameter;
    }

    if (image->current_frame == frame)
        return Ok;

    if (!image->stream)
    {
        TRACE("image doesn't have an associated stream\n");
        return Ok;
    }

    /* choose an appropriate image decoder */
    stat = get_decoder_info(image->stream, &codec);
    if (stat != Ok)
    {
        WARN("can't find decoder info\n");
        return stat;
    }

    /* seek to the start of the stream */
    seek.QuadPart = 0;
    hr = IStream_Seek(image->stream, seek, STREAM_SEEK_SET, NULL);
    if (FAILED(hr))
        return hresult_to_status(hr);

    /* call on the image decoder to do the real work */
    stat = codec->decode_func(image->stream, &codec->info.Clsid, frame, &new_image);

    if (stat == Ok)
    {
        memcpy(&new_image->pixfmt, &codec->info.FormatID, sizeof(GUID));
        free_image_data(image);
        if (image->type == ImageTypeBitmap)
            *(Bitmap *)image = *(Bitmap *)new_image;
        else if (image->type == ImageTypeMetafile)
            *(Metafile *)image = *(Metafile *)new_image;
        new_image->type = ~0;
        GdipFree(new_image);
        return Ok;
    }

    return stat;
}

Status  GdipLoadImageFromStream(IStream *stream, Image **image)
{
    Status stat;
    LARGE_INTEGER seek;
    HRESULT hr;
    const struct image_codec *codec=NULL;

    /* choose an appropriate image decoder */
    stat = get_decoder_info(stream, &codec);
    if (stat != Ok) return stat;

    /* seek to the start of the stream */
    seek.QuadPart = 0;
    hr = IStream_Seek(stream, seek, STREAM_SEEK_SET, NULL);
    if (FAILED(hr)) return hresult_to_status(hr);

    /* call on the image decoder to do the real work */
    stat = codec->decode_func(stream, &codec->info.Clsid, 0, image);

    /* take note of the original data format */
    if (stat == Ok)
    {
        memcpy(&(*image)->pixfmt, &codec->info.FormatID, sizeof(GUID));
        return Ok;
    }

    return stat;
}

/* FIXME: no ICM */
Status  GdipLoadImageFromStreamICM(IStream* stream, Image **image)
{
    TRACE("%p %p\n", stream, image);

    return GdipLoadImageFromStream(stream, image);
}

Status  GdipRemovePropertyItem(Image *image, PROPID propId)
{
    static int calls;

    TRACE("(%p,%u)\n", image, propId);

    if(!image)
        return InvalidParameter;

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

Status  GdipSetPropertyItem(Image *image, GDIPCONST PropertyItem* item)
{
    static int calls;

    if (!image || !item) return InvalidParameter;

    TRACE("(%p,%p:%#x,%u,%u,%p)\n", image, item, item->id, item->type, item->length, item->value);

    if(!(calls++))
        FIXME("not implemented\n");

    return Ok;
}

Status  GdipCreateStreamOnFile(const WCHAR * filename, UINT access, IStream **stream)
{
  DWORD dwMode;
  HRESULT ret;
  TRACE("(%s, %u, %p)\n", debugstr_w(filename), access, stream);
  return Ok;
}

Status  GdipSaveImageToFile(Image *image, GDIPCONST WCHAR* filename,
                                        GDIPCONST CLSID *clsidEncoder,
                                        GDIPCONST EncoderParameters *encoderParams)
{
    Status stat;
    IStream *stream;

    TRACE("%p (%s) %p %p\n", image, debugstr_w(filename), clsidEncoder, encoderParams);

    if (!image || !filename|| !clsidEncoder)
        return InvalidParameter;

    stat = GdipCreateStreamOnFile(filename, GENERIC_WRITE, &stream);
    if (stat != Ok)
        return GenericError;

    stat = GdipSaveImageToStream(image, stream, clsidEncoder, encoderParams);

    IStream_Release(stream);
    return stat;
}

/*************************************************************************
 * Encoding functions -
 *   These functions encode an image in different image file formats.
 */
#define BITMAP_FORMAT_BMP   0x4d42 /* "BM" */
#define BITMAP_FORMAT_JPEG  0xd8ff
#define BITMAP_FORMAT_GIF   0x4947
#define BITMAP_FORMAT_PNG   0x5089
#define BITMAP_FORMAT_APM   0xcdd7

/*****************************************************************************
 * GdipSaveImageToStream [GDIPLUS.@]
 */
Status  GdipSaveImageToStream(Image *image, IStream* stream,
    GDIPCONST CLSID* clsid, GDIPCONST EncoderParameters* params)
{
    Status stat;
    encode_image_func encode_image;
    int i;

    TRACE("%p %p %p %p\n", image, stream, clsid, params);

    if(!image || !stream)
        return InvalidParameter;

    /* select correct encoder */
    encode_image = NULL;
    for (i = 0; i < NUM_CODECS; i++) {
        if ((codecs[i].info.Flags & ImageCodecFlagsEncoder) &&
            IsEqualCLSID(clsid, &codecs[i].info.Clsid))
            encode_image = codecs[i].encode_func;
    }
    if (encode_image == NULL)
        return UnknownImageFormat;

    stat = encode_image(image, stream, clsid, params);

    return stat;
}

/*****************************************************************************
 * GdipSaveAdd [GDIPLUS.@]
 */
Status  GdipSaveAdd(Image *image, GDIPCONST EncoderParameters *params)
{
    FIXME("(%p,%p): stub\n", image, params);
    return Ok;
}

/*****************************************************************************
 * GdipGetImagePalette [GDIPLUS.@]
 */
Status  GdipGetImagePalette(Image *image, ColorPalette *palette, INT size)
{
    INT count;

    TRACE("(%p,%p,%i)\n", image, palette, size);

    if (!image || !palette)
        return InvalidParameter;

    count = image->palette ? image->palette->Count : 0;

    if (size < (sizeof(UINT)*2+sizeof(ARGB)*count))
    {
        TRACE("<-- InsufficientBuffer\n");
        return InsufficientBuffer;
    }

    if (image->palette)
    {
        palette->Flags = image->palette->Flags;
        palette->Count = image->palette->Count;
        memcpy(palette->Entries, image->palette->Entries, sizeof(ARGB)*image->palette->Count);
    }
    else
    {
        palette->Flags = 0;
        palette->Count = 0;
    }
    return Ok;
}

/*****************************************************************************
 * GdipSetImagePalette [GDIPLUS.@]
 */
Status  GdipSetImagePalette(Image *image,
    GDIPCONST ColorPalette *palette)
{
    ColorPalette *new_palette;

    TRACE("(%p,%p)\n", image, palette);

    if(!image || !palette || palette->Count > 256)
        return InvalidParameter;

    new_palette = GdipAlloc(2 * sizeof(UINT) + palette->Count * sizeof(ARGB));
    if (!new_palette) return OutOfMemory;

    GdipFree(image->palette);
    image->palette = new_palette;
    image->palette->Flags = palette->Flags;
    image->palette->Count = palette->Count;
    memcpy(image->palette->Entries, palette->Entries, sizeof(ARGB)*palette->Count);

    return Ok;
}

/*************************************************************************
 * Encoders -
 *   Structures that represent which formats we support for encoding.
 */

/* ImageCodecInfo creation routines taken from libgdiplus */
static const char bmp_codecname[] = "BMP"; /* BMP */
static const char bmp_extension[] = "*.BMP;*.DIB;*.RLE"; /* *.BMP;*.DIB;*.RLE */
static const char bmp_mimetype[] = "image/bmp"; /* image/bmp */
static const char bmp_format[] = "BMP"; /* BMP */
static const BYTE bmp_sig_pattern[] = { 0x42, 0x4D };
static const BYTE bmp_sig_mask[] = { 0xFF, 0xFF };

static const char jpeg_codecname[] = "JPEG";
static const char jpeg_extension[] = "*.JPG;*.JPEG;*.JPE;*.JFIF";
static const char jpeg_mimetype[] = "image/jpeg";
static const char jpeg_format[] = "JPEG";
static const BYTE jpeg_sig_pattern[] = { 0xFF, 0xD8 };
static const BYTE jpeg_sig_mask[] = { 0xFF, 0xFF };

static const char gif_codecname[] = "GIF";
static const char gif_extension[] = "*.GIF";
static const char gif_mimetype[] = "image/gif";
static const char gif_format[] = "GIF";
static const BYTE gif_sig_pattern[12] = "GIF87aGIF89a";
static const BYTE gif_sig_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static const char tiff_codecname[] = "TIFF";
static const char tiff_extension[] = "*.TIFF;*.TIF";
static const char tiff_mimetype[] = "image/tiff";
static const char tiff_format[] = "TIFF";
static const BYTE tiff_sig_pattern[] = {0x49,0x49,42,0,0x4d,0x4d,0,42};
static const BYTE tiff_sig_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static const char emf_codecname[] = "EMF";
static const char emf_extension[] = "*.EMF";
static const char emf_mimetype[] = "image/x-emf";
static const char emf_format[] = "EMF";
static const BYTE emf_sig_pattern[] = { 0x01, 0x00, 0x00, 0x00 };
static const BYTE emf_sig_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };

static const char wmf_codecname[] = "WMF";
static const char wmf_extension[] = "*.WMF";
static const char wmf_mimetype[] = "image/x-wmf";
static const char wmf_format[] = "WMF";
static const BYTE wmf_sig_pattern[] = { 0xd7, 0xcd };
static const BYTE wmf_sig_mask[] = { 0xFF, 0xFF };

static const char png_codecname[] = "PNG";
static const char png_extension[] = "*.PNG";
static const char png_mimetype[] = "image/png";
static const char png_format[] = "PNG";
static const BYTE png_sig_pattern[] = { 137, 80, 78, 71, 13, 10, 26, 10, };
static const BYTE png_sig_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static const char ico_codecname[] = "ICO";
static const char ico_extension[] = "*.ICO";
static const char ico_mimetype[] = "image/x-icon";
static const char ico_format[] = "ICO";
static const BYTE ico_sig_pattern[] = { 0x00, 0x00, 0x01, 0x00 };
static const BYTE ico_sig_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };

static const struct image_codec codecs[NUM_CODECS] = {
    {
        { /* BMP */
            /* Clsid */              { 0x557cf400, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cabU, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          bmp_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  bmp_format,
            /* FilenameExtension */  bmp_extension,
            /* MimeType */           bmp_mimetype,
            /* Flags */              ImageCodecFlagsEncoder | ImageCodecFlagsDecoder | ImageCodecFlagsSupportBitmap,
            /* Version */            1,
            /* SigCount */           1,
            /* SigSize */            2,
            /* SigPattern */         bmp_sig_pattern,
            /* SigMask */            bmp_sig_mask,
        },
        encode_image_BMP,
        decode_image_bmp
    },
    {
        { /* JPEG */
            /* Clsid */              { 0x557cf401, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3caeU, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          jpeg_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  jpeg_format,
            /* FilenameExtension */  jpeg_extension,
            /* MimeType */           jpeg_mimetype,
            /* Flags */              ImageCodecFlagsEncoder | ImageCodecFlagsDecoder | ImageCodecFlagsSupportBitmap,
            /* Version */            1,
            /* SigCount */           1,
            /* SigSize */            2,
            /* SigPattern */         jpeg_sig_pattern,
            /* SigMask */            jpeg_sig_mask,
        },
        encode_image_jpeg,
        decode_image_jpeg
    },
    {
        { /* GIF */
            /* Clsid */              { 0x557cf402, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cb0U, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          gif_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  gif_format,
            /* FilenameExtension */  gif_extension,
            /* MimeType */           gif_mimetype,
            /* Flags */              ImageCodecFlagsDecoder | ImageCodecFlagsSupportBitmap,
            /* Version */            1,
            /* SigCount */           2,
            /* SigSize */            6,
            /* SigPattern */         gif_sig_pattern,
            /* SigMask */            gif_sig_mask,
        },
        NULL,
        decode_image_gif
    },
    {
        { /* TIFF */
            /* Clsid */              { 0x557cf405, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cb1U, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          tiff_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  tiff_format,
            /* FilenameExtension */  tiff_extension,
            /* MimeType */           tiff_mimetype,
            /* Flags */              ImageCodecFlagsDecoder | ImageCodecFlagsEncoder | ImageCodecFlagsSupportBitmap,
            /* Version */            1,
            /* SigCount */           2,
            /* SigSize */            4,
            /* SigPattern */         tiff_sig_pattern,
            /* SigMask */            tiff_sig_mask,
        },
        encode_image_tiff,
        decode_image_tiff
    },
    {
        { /* EMF */
            /* Clsid */              { 0x557cf403, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cacU, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          emf_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  emf_format,
            /* FilenameExtension */  emf_extension,
            /* MimeType */           emf_mimetype,
            /* Flags */              ImageCodecFlagsDecoder | ImageCodecFlagsSupportVector,
            /* Version */            1,
            /* SigCount */           1,
            /* SigSize */            4,
            /* SigPattern */         emf_sig_pattern,
            /* SigMask */            emf_sig_mask,
        },
        NULL,
        decode_image_olepicture_metafile
    },
    {
        { /* WMF */
            /* Clsid */              { 0x557cf404, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cadU, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          wmf_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  wmf_format,
            /* FilenameExtension */  wmf_extension,
            /* MimeType */           wmf_mimetype,
            /* Flags */              ImageCodecFlagsDecoder | ImageCodecFlagsSupportVector,
            /* Version */            1,
            /* SigCount */           1,
            /* SigSize */            2,
            /* SigPattern */         wmf_sig_pattern,
            /* SigMask */            wmf_sig_mask,
        },
        NULL,
        decode_image_olepicture_metafile
    },
    {
        { /* PNG */
            /* Clsid */              { 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cafU, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          png_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  png_format,
            /* FilenameExtension */  png_extension,
            /* MimeType */           png_mimetype,
            /* Flags */              ImageCodecFlagsEncoder | ImageCodecFlagsDecoder | ImageCodecFlagsSupportBitmap,
            /* Version */            1,
            /* SigCount */           1,
            /* SigSize */            8,
            /* SigPattern */         png_sig_pattern,
            /* SigMask */            png_sig_mask,
        },
        encode_image_png,
        decode_image_png
    },
    {
        { /* ICO */
            /* Clsid */              { 0x557cf407, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x0, 0x0, 0xf8, 0x1e, 0xf3, 0x2e } },
            /* FormatID */           { 0xb96b3cabU, 0x0728U, 0x11d3U, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e} },
            /* CodecName */          ico_codecname,
            /* DllName */            NULL,
            /* FormatDescription */  ico_format,
            /* FilenameExtension */  ico_extension,
            /* MimeType */           ico_mimetype,
            /* Flags */              ImageCodecFlagsDecoder | ImageCodecFlagsSupportBitmap,
            /* Version */            1,
            /* SigCount */           1,
            /* SigSize */            4,
            /* SigPattern */         ico_sig_pattern,
            /* SigMask */            ico_sig_mask,
        },
        NULL,
        decode_image_icon
    },
};

/*****************************************************************************
 * GdipGetImageDecodersSize [GDIPLUS.@]
 */
Status  GdipGetImageDecodersSize(UINT *numDecoders, UINT *size)
{
    int decoder_count=0;
    int i;
    TRACE("%p %p\n", numDecoders, size);

    if (!numDecoders || !size)
        return InvalidParameter;

    for (i=0; i<NUM_CODECS; i++)
    {
        if (codecs[i].info.Flags & ImageCodecFlagsDecoder)
            decoder_count++;
    }

    *numDecoders = decoder_count;
    *size = decoder_count * sizeof(ImageCodecInfo);

    return Ok;
}

/*****************************************************************************
 * GdipGetImageDecoders [GDIPLUS.@]
 */
Status  GdipGetImageDecoders(UINT numDecoders, UINT size, ImageCodecInfo *decoders)
{
    int i, decoder_count=0;
    TRACE("%u %u %p\n", numDecoders, size, decoders);

    if (!decoders ||
        size != numDecoders * sizeof(ImageCodecInfo))
        return GenericError;

    for (i=0; i<NUM_CODECS; i++)
    {
        if (codecs[i].info.Flags & ImageCodecFlagsDecoder)
        {
            if (decoder_count == numDecoders) return GenericError;
            memcpy(&decoders[decoder_count], &codecs[i].info, sizeof(ImageCodecInfo));
            decoder_count++;
        }
    }

    if (decoder_count < numDecoders) return GenericError;

    return Ok;
}

/*****************************************************************************
 * GdipGetImageEncodersSize [GDIPLUS.@]
 */
Status  GdipGetImageEncodersSize(UINT *numEncoders, UINT *size)
{
    int encoder_count=0;
    int i;
    TRACE("%p %p\n", numEncoders, size);

    if (!numEncoders || !size)
        return InvalidParameter;

    for (i=0; i<NUM_CODECS; i++)
    {
        if (codecs[i].info.Flags & ImageCodecFlagsEncoder)
            encoder_count++;
    }

    *numEncoders = encoder_count;
    *size = encoder_count * sizeof(ImageCodecInfo);

    return Ok;
}

/*****************************************************************************
 * GdipGetImageEncoders [GDIPLUS.@]
 */
Status  GdipGetImageEncoders(UINT numEncoders, UINT size, ImageCodecInfo *encoders)
{
    int i, encoder_count=0;
    TRACE("%u %u %p\n", numEncoders, size, encoders);

    if (!encoders ||
        size != numEncoders * sizeof(ImageCodecInfo))
        return GenericError;

    for (i=0; i<NUM_CODECS; i++)
    {
        if (codecs[i].info.Flags & ImageCodecFlagsEncoder)
        {
            if (encoder_count == numEncoders) return GenericError;
            memcpy(&encoders[encoder_count], &codecs[i].info, sizeof(ImageCodecInfo));
            encoder_count++;
        }
    }

    if (encoder_count < numEncoders) return GenericError;

    return Ok;
}

Status  GdipGetEncoderParameterListSize(Image *image,
    GDIPCONST CLSID* clsidEncoder, UINT *size)
{
    static int calls;

    TRACE("(%p,%s,%p)\n", image, debugstr_guid(clsidEncoder), size);

    if(!(calls++))
        FIXME("not implemented\n");

    *size = 0;

    return NotImplemented;
}

static PixelFormat get_16bpp_format(HBITMAP hbm)
{
    BITMAPV4HEADER bmh;
    HDC hdc;
    PixelFormat result;

    hdc = CreateCompatibleDC(NULL);

    memset(&bmh, 0, sizeof(bmh));
    bmh.bV4Size = sizeof(bmh);
    bmh.bV4Width = 1;
    bmh.bV4Height = 1;
    bmh.bV4V4Compression = BI_BITFIELDS;
    bmh.bV4BitCount = 16;

    GetDIBits(hdc, hbm, 0, 0, NULL, (BITMAPINFO*)&bmh, DIB_RGB_COLORS);

    if (bmh.bV4RedMask == 0x7c00 &&
        bmh.bV4GreenMask == 0x3e0 &&
        bmh.bV4BlueMask == 0x1f)
    {
        result = PixelFormat16bppRGB555;
    }
    else if (bmh.bV4RedMask == 0xf800 &&
        bmh.bV4GreenMask == 0x7e0 &&
        bmh.bV4BlueMask == 0x1f)
    {
        result = PixelFormat16bppRGB565;
    }
    else
    {
        FIXME("unrecognized bitfields %x,%x,%x\n", bmh.bV4RedMask,
            bmh.bV4GreenMask, bmh.bV4BlueMask);
        result = PixelFormatUndefined;
    }

    DeleteDC(hdc);

    return result;
}

/*****************************************************************************
 * GdipCreateBitmapFromHBITMAP [GDIPLUS.@]
 */
Status  GdipCreateBitmapFromHBITMAP(HBITMAP hbm, HPALETTE hpal, Bitmap* bitmap)
{
    BITMAP bm;
    Status retval;
    PixelFormat format;
    BitmapData lockeddata;

    TRACE("%p %p %p\n", hbm, hpal, bitmap);

    if(!hbm || !bitmap)
        return InvalidParameter;

    if (GetObjectA(hbm, sizeof(bm), &bm) != sizeof(bm))
            return InvalidParameter;

    /* TODO: Figure out the correct format for 16, 32, 64 bpp */
    switch(bm.bmBitsPixel) {
        case 1:
            format = PixelFormat1bppIndexed;
            break;
        case 4:
            format = PixelFormat4bppIndexed;
            break;
        case 8:
            format = PixelFormat8bppIndexed;
            break;
        case 16:
            format = get_16bpp_format(hbm);
            if (format == PixelFormatUndefined)
                return InvalidParameter;
            break;
        case 24:
            format = PixelFormat24bppRGB;
            break;
        case 32:
            format = PixelFormat32bppRGB;
            break;
        case 48:
            format = PixelFormat48bppRGB;
            break;
        default:
            FIXME("don't know how to handle %d bpp\n", bm.bmBitsPixel);
            return InvalidParameter;
    }

    retval = GdipCreateBitmapFromScan0(bm.bmWidth, bm.bmHeight, 0,
        format, NULL, bitmap);

    if (retval == Ok)
    {
        retval = GdipBitmapLockBits(*bitmap, NULL, ImageLockModeWrite,
            format, &lockeddata);
        if (retval == Ok)
        {
            HDC hdc;
            BITMAPINFO *pbmi;
            INT src_height;

            hdc = CreateCompatibleDC(NULL);
            pbmi = GdipAlloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));

            if (pbmi)
            {
                pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                pbmi->bmiHeader.biBitCount = 0;

                GetDIBits(hdc, hbm, 0, 0, NULL, pbmi, DIB_RGB_COLORS);

                src_height = abs(pbmi->bmiHeader.biHeight);
                pbmi->bmiHeader.biHeight = -src_height;

                GetDIBits(hdc, hbm, 0, src_height, lockeddata.Scan0, pbmi, DIB_RGB_COLORS);

                GdipFree(pbmi);
            }
            else
                retval = OutOfMemory;

            DeleteDC(hdc);

            GdipBitmapUnlockBits(*bitmap, &lockeddata);
        }

        if (retval == Ok && hpal)
        {
            WORD num_palette_entries;
            PALETTEENTRY *palette_entries=NULL;
            ColorPalette *palette=NULL;
            int i;

            if (!GetObjectW(hpal, sizeof(num_palette_entries), &num_palette_entries))
                retval = GenericError;

            if (retval == Ok)
            {
                palette_entries = GdipAlloc(sizeof(PALETTEENTRY) * num_palette_entries);
                palette = GdipAlloc(sizeof(ColorPalette) + sizeof(ARGB) * (num_palette_entries-1));

                if (!palette_entries || !palette)
                    retval = OutOfMemory;
            }

            if (retval == Ok)
            {
                if (!GetPaletteEntries(hpal, 0, num_palette_entries, palette_entries))
                    retval = GenericError;
            }

            if (retval == Ok)
            {
                palette->Flags = 0;
                palette->Count = num_palette_entries;

                for (i=0; i<num_palette_entries; i++)
                {
                    PALETTEENTRY * entry = &palette_entries[i];
                    palette->Entries[i] = 0xff000000 | entry->peRed << 16 |
                        entry->peGreen << 8 | entry->peBlue;
                }

                retval = GdipSetImagePalette((Image*)*bitmap, palette);
            }

            GdipFree(palette_entries);
            GdipFree(palette);
        }

        if (retval != Ok)
        {
            GdipDisposeImage((Image*)*bitmap);
            *bitmap = NULL;
        }
    }

    return retval;
}

Status  GdipDeleteEffect(CGpEffect *effect)
{
    FIXME("(%p): stub\n", effect);
    /* note: According to Jose Roca's GDI+ Docs, this is not implemented
     * in Windows's gdiplus */
    return NotImplemented;
}

/*****************************************************************************
 * GdipSetEffectParameters [GDIPLUS.@]
 */
Status  GdipSetEffectParameters(CGpEffect *effect,
    const VOID *params, const UINT size)
{
    static int calls;

    TRACE("(%p,%p,%u)\n", effect, params, size);

    if(!(calls++))
        FIXME("not implemented\n");

    return NotImplemented;
}

/*****************************************************************************
 * GdipGetImageFlags [GDIPLUS.@]
 */
Status  GdipGetImageFlags(Image *image, UINT *flags)
{
    TRACE("%p %p\n", image, flags);

    if(!image || !flags)
        return InvalidParameter;

    *flags = image->flags;

    return Ok;
}

Status  GdipTestControl(GpTestControlEnum control, void *param)
{
    TRACE("(%d, %p)\n", control, param);

    switch(control){
        case TestControlForceBilinear:
            if(param)
                FIXME("TestControlForceBilinear not handled\n");
            break;
        case TestControlNoICM:
            if(param)
                FIXME("TestControlNoICM not handled\n");
            break;
        case TestControlGetBuildNumber:
            *((DWORD*)param) = 3102;
            break;
    }

    return Ok;
}

Status  GdipRecordMetafileFileName(GDIPCONST WCHAR* fileName,
                            HDC hdc, EmfType type, GDIPCONST RectF *pFrameRect,
                            MetafileFrameUnit frameUnit, GDIPCONST WCHAR *desc,
                            Metafile **metafile)
{
    FIXME("%s %p %d %p %d %s %p stub!\n", debugstr_w(fileName), hdc, type, pFrameRect,
                                 frameUnit, debugstr_w(desc), metafile);

    return NotImplemented;
}

Status  GdipRecordMetafileFileNameI(GDIPCONST WCHAR* fileName, HDC hdc, EmfType type,
                            GDIPCONST Rect *pFrameRect, MetafileFrameUnit frameUnit,
                            GDIPCONST WCHAR *desc, Metafile **metafile)
{
    FIXME("%s %p %d %p %d %s %p stub!\n", debugstr_w(fileName), hdc, type, pFrameRect,
                                 frameUnit, debugstr_w(desc), metafile);

    return NotImplemented;
}

Status  GdipImageForceValidation(Image *image)
{
    TRACE("%p\n", image);

    return Ok;
}

/*****************************************************************************
 * GdipGetImageThumbnail [GDIPLUS.@]
 */
Status  GdipGetImageThumbnail(Image *image, UINT width, UINT height,
                            Image **ret_image, GetThumbnailImageAbort cb,
                            VOID * cb_data)
{
    Status stat;
    Graphics *graphics;
    UINT srcwidth, srcheight;

    TRACE("(%p %u %u %p %p %p)\n",
        image, width, height, ret_image, cb, cb_data);

    if (!image || !ret_image)
        return InvalidParameter;

    if (!width) width = 120;
    if (!height) height = 120;

    GdipGetImageWidth(image, &srcwidth);
    GdipGetImageHeight(image, &srcheight);

    stat = GdipCreateBitmapFromScan0(width, height, 0, PixelFormat32bppPARGB,
        NULL, (Bitmap**)ret_image);

    if (stat == Ok)
    {
        stat = GdipGetImageGraphicsContext(*ret_image, &graphics);

        if (stat == Ok)
        {
            stat = GdipDrawImageRectRectI(graphics, image,
                0, 0, width, height, 0, 0, srcwidth, srcheight, UnitPixel,
                NULL, NULL, NULL);

            GdipDeleteGraphics(graphics);
        }

        if (stat != Ok)
        {
            GdipDisposeImage(*ret_image);
            *ret_image = NULL;
        }
    }

    return stat;
}

/*****************************************************************************
 * GdipImageRotateFlip [GDIPLUS.@]
 */
Status  GdipImageRotateFlip(Image *image, RotateFlipType type)
{
    Bitmap *new_bitmap;
    Bitmap *bitmap;
    int bpp, bytesperpixel;
    int rotate_90, flip_x, flip_y;
    int src_x_offset, src_y_offset;
    LPBYTE src_origin;
    UINT x, y, width, height;
    BitmapData src_lock, dst_lock;
    Status stat;

    TRACE("(%p, %u)\n", image, type);

    if (!image)
        return InvalidParameter;

    rotate_90 = type&1;
    flip_x = (type&6) == 2 || (type&6) == 4;
    flip_y = (type&3) == 1 || (type&3) == 2;

    if (image->type != ImageTypeBitmap)
    {
        FIXME("Not implemented for type %i\n", image->type);
        return NotImplemented;
    }

    bitmap = (Bitmap*)image;
    bpp = PIXELFORMATBPP(bitmap->pixfmt);

    if (bpp < 8)
    {
        FIXME("Not implemented for %i bit images\n", bpp);
        return NotImplemented;
    }

    if (rotate_90)
    {
        width = bitmap->height;
        height = bitmap->width;
    }
    else
    {
        width = bitmap->width;
        height = bitmap->height;
    }

    bytesperpixel = bpp/8;

    stat = GdipCreateBitmapFromScan0(width, height, 0, bitmap->pixfmt, NULL, &new_bitmap);

    if (stat != Ok)
        return stat;

    stat = GdipBitmapLockBits(bitmap, NULL, ImageLockModeRead, bitmap->pixfmt, &src_lock);

    if (stat == Ok)
    {
        stat = GdipBitmapLockBits(new_bitmap, NULL, ImageLockModeWrite, bitmap->pixfmt, &dst_lock);

        if (stat == Ok)
        {
            LPBYTE src_row, src_pixel;
            LPBYTE dst_row, dst_pixel;

            src_origin = src_lock.Scan0;
            if (flip_x) src_origin += bytesperpixel * (bitmap->width - 1);
            if (flip_y) src_origin += src_lock.Stride * (bitmap->height - 1);

            if (rotate_90)
            {
                if (flip_y) src_x_offset = -src_lock.Stride;
                else src_x_offset = src_lock.Stride;
                if (flip_x) src_y_offset = -bytesperpixel;
                else src_y_offset = bytesperpixel;
            }
            else
            {
                if (flip_x) src_x_offset = -bytesperpixel;
                else src_x_offset = bytesperpixel;
                if (flip_y) src_y_offset = -src_lock.Stride;
                else src_y_offset = src_lock.Stride;
            }

            src_row = src_origin;
            dst_row = dst_lock.Scan0;
            for (y=0; y<height; y++)
            {
                src_pixel = src_row;
                dst_pixel = dst_row;
                for (x=0; x<width; x++)
                {
                    /* FIXME: This could probably be faster without memcpy. */
                    memcpy(dst_pixel, src_pixel, bytesperpixel);
                    dst_pixel += bytesperpixel;
                    src_pixel += src_x_offset;
                }
                src_row += src_y_offset;
                dst_row += dst_lock.Stride;
            }

            GdipBitmapUnlockBits(new_bitmap, &dst_lock);
        }

        GdipBitmapUnlockBits(bitmap, &src_lock);
    }

    if (stat == Ok)
        move_bitmap(bitmap, new_bitmap, FALSE);
    else
        GdipDisposeImage((Image*)new_bitmap);

    return stat;
}

/*****************************************************************************
 * GdipConvertToEmfPlusToFile [GDIPLUS.@]
 */

Status  GdipConvertToEmfPlusToFile(const Graphics* refGraphics,
                                               Metafile* metafile, BOOL* conversionSuccess,
                                               const WCHAR* filename, EmfType emfType,
                                               const WCHAR* description, Metafile** out_metafile)
{
    FIXME("stub: %p, %p, %p, %p, %u, %p, %p\n", refGraphics, metafile, conversionSuccess, filename, emfType, description, out_metafile);
    return NotImplemented;
}

#endif
