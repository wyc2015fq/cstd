

typedef struct EmfPlusRecordHeader
{
    WORD Type;
    WORD Flags;
    DWORD Size;
    DWORD DataSize;
} EmfPlusRecordHeader;

typedef struct EmfPlusHeader
{
    EmfPlusRecordHeader Header;
    DWORD Version;
    DWORD EmfPlusFlags;
    DWORD LogicalDpiX;
    DWORD LogicalDpiY;
} EmfPlusHeader;

static Status METAFILE_AllocateRecord(Metafile *metafile, DWORD size, void **result)
{
    DWORD size_needed;
    EmfPlusRecordHeader *record;

    if (!metafile->comment_data_size)
    {
        DWORD data_size = MAX(256, size * 2 + 4);
        metafile->comment_data = GdipAlloc(data_size);

        if (!metafile->comment_data)
            return OutOfMemory;

        memcpy(metafile->comment_data, "EMF+", 4);

        metafile->comment_data_size = data_size;
        metafile->comment_data_length = 4;
    }

    size_needed = size + metafile->comment_data_length;

    if (size_needed > metafile->comment_data_size)
    {
        DWORD data_size = size_needed * 2;
        BYTE *new_data = GdipAlloc(data_size);

        if (!new_data)
            return OutOfMemory;

        memcpy(new_data, metafile->comment_data, metafile->comment_data_length);

        metafile->comment_data_size = data_size;
        GdipFree(metafile->comment_data);
        metafile->comment_data = new_data;
    }

    *result = metafile->comment_data + metafile->comment_data_length;
    metafile->comment_data_length += size;

    record = (EmfPlusRecordHeader*)*result;
    record->Size = size;
    record->DataSize = size - sizeof(EmfPlusRecordHeader);

    return Ok;
}

static void METAFILE_WriteRecords(Metafile *metafile)
{
    if (metafile->comment_data_length > 4)
    {
        GdiComment(metafile->record_dc, metafile->comment_data_length, metafile->comment_data);
        metafile->comment_data_length = 4;
    }
}

static Status METAFILE_WriteHeader(Metafile *metafile, HDC hdc)
{
    Status stat;

    if (metafile->metafile_type == MetafileTypeEmfPlusOnly || metafile->metafile_type == MetafileTypeEmfPlusDual)
    {
        EmfPlusHeader *header;

        stat = METAFILE_AllocateRecord(metafile, sizeof(EmfPlusHeader), (void**)&header);
        if (stat != Ok)
            return stat;

        header->Header.Type = EmfPlusRecordTypeHeader;

        if (metafile->metafile_type == MetafileTypeEmfPlusDual)
            header->Header.Flags = 1;
        else
            header->Header.Flags = 0;

        header->Version = 0xDBC01002;

        if (GetDeviceCaps(hdc, TECHNOLOGY) == DT_RASDISPLAY)
            header->EmfPlusFlags = 1;
        else
            header->EmfPlusFlags = 0;

        header->LogicalDpiX = GetDeviceCaps(hdc, LOGPIXELSX);
        header->LogicalDpiY = GetDeviceCaps(hdc, LOGPIXELSY);

        METAFILE_WriteRecords(metafile);
    }

    return Ok;
}

static Status METAFILE_WriteEndOfFile(Metafile *metafile)
{
    Status stat;

    if (metafile->metafile_type == MetafileTypeEmfPlusOnly || metafile->metafile_type == MetafileTypeEmfPlusDual)
    {
        EmfPlusRecordHeader *record;

        stat = METAFILE_AllocateRecord(metafile, sizeof(EmfPlusRecordHeader), (void**)&record);
        if (stat != Ok)
            return stat;

        record->Type = EmfPlusRecordTypeEndOfFile;
        record->Flags = 0;

        METAFILE_WriteRecords(metafile);
    }

    return Ok;
}

Status  GdipRecordMetafile(HDC hdc, EmfType type, GDIPCONST RectF *frameRect,
                                       MetafileFrameUnit frameUnit, GDIPCONST WCHAR *desc, Metafile **metafile)
{
    HDC record_dc;
    REAL framerect_factor_x, framerect_factor_y;
    RECT rc;
    Status stat;

    TRACE("(%p %d %p %d %p %p)\n", hdc, type, frameRect, frameUnit, desc, metafile);

    if (!hdc || type < EmfTypeEmfOnly || type > EmfTypeEmfPlusDual || !metafile)
        return InvalidParameter;

    if (!frameRect)
    {
        FIXME("not implemented for NULL rect\n");
        return NotImplemented;
    }

    switch (frameUnit)
    {
    case MetafileFrameUnitPixel:
        framerect_factor_x = 2540.0 / GetDeviceCaps(hdc, LOGPIXELSX);
        framerect_factor_y = 2540.0 / GetDeviceCaps(hdc, LOGPIXELSY);
        break;
    case MetafileFrameUnitPoint:
        framerect_factor_x = framerect_factor_y = 2540.0 / 72.0;
        break;
    case MetafileFrameUnitInch:
        framerect_factor_x = framerect_factor_y = 2540.0;
        break;
    case MetafileFrameUnitDocument:
        framerect_factor_x = framerect_factor_y = 2540.0 / 300.0;
        break;
    case MetafileFrameUnitMillimeter:
        framerect_factor_x = framerect_factor_y = 100.0;
        break;
    case MetafileFrameUnitGdi:
        framerect_factor_x = framerect_factor_y = 1.0;
        break;
    default:
        return InvalidParameter;
    }

    rc.left = framerect_factor_x * frameRect->X;
    rc.top = framerect_factor_y * frameRect->Y;
    rc.right = rc.left + framerect_factor_x * frameRect->Width;
    rc.bottom = rc.top + framerect_factor_y * frameRect->Height;

    record_dc = CreateEnhMetaFileW(hdc, NULL, &rc, desc);

    if (!record_dc)
        return GenericError;

    *metafile = GdipAlloc(sizeof(Metafile));
    if(!*metafile)
    {
        DeleteEnhMetaFile(CloseEnhMetaFile(record_dc));
        return OutOfMemory;
    }

    (*metafile)->image.type = ImageTypeMetafile;
    (*metafile)->image.picture = NULL;
    (*metafile)->image.flags   = ImageFlagsNone;
    (*metafile)->image.palette = NULL;
    (*metafile)->bounds = *frameRect;
    (*metafile)->unit = frameUnit;
    (*metafile)->metafile_type = type;
    (*metafile)->record_dc = record_dc;
    (*metafile)->comment_data = NULL;
    (*metafile)->comment_data_size = 0;
    (*metafile)->comment_data_length = 0;
    (*metafile)->hemf = NULL;

    stat = METAFILE_WriteHeader(*metafile, hdc);

    if (stat != Ok)
    {
        DeleteEnhMetaFile(CloseEnhMetaFile(record_dc));
        GdipFree(*metafile);
        *metafile = NULL;
        return OutOfMemory;
    }

    return stat;
}

/*****************************************************************************
 * GdipRecordMetafileI [GDIPLUS.@]
 */
Status  GdipRecordMetafileI(HDC hdc, EmfType type, GDIPCONST Rect *frameRect,
                                        MetafileFrameUnit frameUnit, GDIPCONST WCHAR *desc, Metafile **metafile)
{
    RectF frameRectF, *pFrameRectF;

    TRACE("(%p %d %p %d %p %p)\n", hdc, type, frameRect, frameUnit, desc, metafile);

    if (frameRect)
    {
        frameRectF.X = frameRect->X;
        frameRectF.Y = frameRect->Y;
        frameRectF.Width = frameRect->Width;
        frameRectF.Height = frameRect->Height;
        pFrameRectF = &frameRectF;
    }
    else
        pFrameRectF = NULL;

    return GdipRecordMetafile(hdc, type, pFrameRectF, frameUnit, desc, metafile);
}

Status METAFILE_GetGraphicsContext(Metafile* metafile, Graphics **result)
{
    Status stat;

    if (!metafile->record_dc || metafile->record_graphics)
        return InvalidParameter;

    stat = graphics_from_image((Image*)metafile, &metafile->record_graphics);

    if (stat == Ok)
        *result = metafile->record_graphics;

    return stat;
}

Status METAFILE_GetDC(Metafile* metafile, HDC *hdc)
{
    if (metafile->metafile_type == MetafileTypeEmfPlusOnly || metafile->metafile_type == MetafileTypeEmfPlusDual)
    {
        EmfPlusRecordHeader *record;
        Status stat;

        stat = METAFILE_AllocateRecord(metafile, sizeof(EmfPlusRecordHeader), (void**)&record);
        if (stat != Ok)
            return stat;

        record->Type = EmfPlusRecordTypeGetDC;
        record->Flags = 0;

        METAFILE_WriteRecords(metafile);
    }

    *hdc = metafile->record_dc;

    return Ok;
}

Status METAFILE_ReleaseDC(Metafile* metafile, HDC hdc)
{
    if (hdc != metafile->record_dc)
        return InvalidParameter;

    return Ok;
}

Status METAFILE_GraphicsDeleted(Metafile* metafile)
{
    Status stat;

    stat = METAFILE_WriteEndOfFile(metafile);
    metafile->record_graphics = NULL;

    metafile->hemf = CloseEnhMetaFile(metafile->record_dc);
    metafile->record_dc = NULL;

    GdipFree(metafile->comment_data);
    metafile->comment_data = NULL;
    metafile->comment_data_size = 0;

    return stat;
}

Status  GdipGetHemfFromMetafile(Metafile *metafile, HENHMETAFILE *hEmf)
{
    TRACE("(%p,%p)\n", metafile, hEmf);

    if (!metafile || !hEmf || !metafile->hemf)
        return InvalidParameter;

    *hEmf = metafile->hemf;
    metafile->hemf = NULL;

    return Ok;
}

static Status METAFILE_PlaybackGetDC(Metafile *metafile)
{
    Status stat = Ok;

    stat = GdipGetDC(metafile->playback_graphics, &metafile->playback_dc);

    if (stat == Ok)
    {
        /* The result of GdipGetDC always expects device co-ordinates, but the
         * device co-ordinates of the source metafile do not correspond to
         * device co-ordinates of the destination. Therefore, we set up the DC
         * so that the metafile's bounds map to the destination points where we
         * are drawing this metafile. */
        SetMapMode(metafile->playback_dc, MM_ANISOTROPIC);

        SetWindowOrgEx(metafile->playback_dc, metafile->bounds.X, metafile->bounds.Y, NULL);
        SetWindowExtEx(metafile->playback_dc, metafile->bounds.Width, metafile->bounds.Height, NULL);

        SetViewportOrgEx(metafile->playback_dc, metafile->playback_points[0].X, metafile->playback_points[0].Y, NULL);
        SetViewportExtEx(metafile->playback_dc,
            metafile->playback_points[1].X - metafile->playback_points[0].X,
            metafile->playback_points[2].Y - metafile->playback_points[0].Y, NULL);
    }

    return stat;
}

static void METAFILE_PlaybackReleaseDC(Metafile *metafile)
{
    if (metafile->playback_dc)
    {
        GdipReleaseDC(metafile->playback_graphics, metafile->playback_dc);
        metafile->playback_dc = NULL;
    }
}

Status  GdipPlayMetafileRecord(GDIPCONST Metafile *metafile,
    EmfPlusRecordType recordType, UINT flags, UINT dataSize, GDIPCONST BYTE *data)
{
    TRACE("(%p,%x,%x,%d,%p)\n", metafile, recordType, flags, dataSize, data);

    if (!metafile || (dataSize && !data) || !metafile->playback_graphics)
        return InvalidParameter;

    if (recordType >= 1 && recordType <= 0x7a)
    {
        /* regular EMF record */
        if (metafile->playback_dc)
        {
            ENHMETARECORD *record;

            record = GdipAlloc(dataSize + 8);

            if (record)
            {
                record->iType = recordType;
                record->nSize = dataSize;
                memcpy(record->dParm, data, dataSize);

                PlayEnhMetaFileRecord(metafile->playback_dc, metafile->handle_table,
                    record, metafile->handle_count);

                GdipFree(record);
            }
            else
                return OutOfMemory;
        }
    }
    else
    {
        METAFILE_PlaybackReleaseDC((Metafile*)metafile);

        switch(recordType)
        {
        case EmfPlusRecordTypeHeader:
        case EmfPlusRecordTypeEndOfFile:
            break;
        case EmfPlusRecordTypeGetDC:
            METAFILE_PlaybackGetDC((Metafile*)metafile);
            break;
        default:
            FIXME("Not implemented for record type %x\n", recordType);
            return NotImplemented;
        }
    }

    return Ok;
}

struct enum_metafile_data
{
    EnumerateMetafileProc callback;
    void *callback_data;
    Metafile *metafile;
};

static int CALLBACK enum_metafile_proc(HDC hDC, HANDLETABLE *lpHTable, const ENHMETARECORD *lpEMFR,
    int nObj, LPARAM lpData)
{
    BOOL ret;
    struct enum_metafile_data *data = (struct enum_metafile_data*)lpData;
    const BYTE* pStr;

    data->metafile->handle_table = lpHTable;
    data->metafile->handle_count = nObj;

    /* First check for an EMF+ record. */
    if (lpEMFR->iType == EMR_GDICOMMENT)
    {
        const EMRGDICOMMENT *comment = (const EMRGDICOMMENT*)lpEMFR;

        if (comment->cbData >= 4 && memcmp(comment->Data, "EMF+", 4) == 0)
        {
            int offset = 4;

            while (offset + sizeof(EmfPlusRecordHeader) <= comment->cbData)
            {
                const EmfPlusRecordHeader *record = (const EmfPlusRecordHeader*)&comment->Data[offset];

                if (record->DataSize)
                    pStr = (const BYTE*)(record+1);
                else
                    pStr = NULL;

                ret = data->callback(record->Type, record->Flags, record->DataSize,
                    pStr, data->callback_data);

                if (!ret)
                    return 0;

                offset += record->Size;
            }

            return 1;
        }
    }

    if (lpEMFR->nSize != 8)
        pStr = (const BYTE*)lpEMFR->dParm;
    else
        pStr = NULL;

    return data->callback(lpEMFR->iType, 0, lpEMFR->nSize-8,
        pStr, data->callback_data);
}

Status  GdipEnumerateMetafileSrcRectDestPoints(Graphics *graphics,
    GDIPCONST Metafile *metafile, GDIPCONST PointF *destPoints, INT count,
    GDIPCONST RectF *srcRect, Unit srcUnit, EnumerateMetafileProc callback,
    VOID *callbackData, GDIPCONST ImageAttributes *imageAttributes)
{
    struct enum_metafile_data data;
    Status stat;
    Metafile *real_metafile = (Metafile*)metafile; /* whoever made this const was joking */

    TRACE("(%p,%p,%p,%i,%p,%i,%p,%p,%p)\n", graphics, metafile,
        destPoints, count, srcRect, srcUnit, callback, callbackData,
        imageAttributes);

    if (!graphics || !metafile || !destPoints || count != 3 || !srcRect)
        return InvalidParameter;

    if (!metafile->hemf)
        return InvalidParameter;

    if (metafile->playback_graphics)
        return ObjectBusy;

    TRACE("%s %i -> %s %s %s\n", debugstr_rectf(srcRect), srcUnit,
        debugstr_pointf(&destPoints[0]), debugstr_pointf(&destPoints[1]),
        debugstr_pointf(&destPoints[2]));

    data.callback = callback;
    data.callback_data = callbackData;
    data.metafile = real_metafile;

    real_metafile->playback_graphics = graphics;
    real_metafile->playback_dc = NULL;

    memcpy(real_metafile->playback_points, destPoints, sizeof(PointF) * 3);
    stat = GdipTransformPoints(graphics, CoordinateSpaceDevice, CoordinateSpaceWorld, real_metafile->playback_points, 3);

    if (stat == Ok && metafile->metafile_type == MetafileTypeEmf)
        stat = METAFILE_PlaybackGetDC((Metafile*)metafile);

    if (stat == Ok)
        EnumEnhMetaFile(0, metafile->hemf, enum_metafile_proc, &data, NULL);

    METAFILE_PlaybackReleaseDC((Metafile*)metafile);

    real_metafile->playback_graphics = NULL;

    return stat;
}

static int CALLBACK get_metafile_type_proc(HDC hDC, HANDLETABLE *lpHTable, const ENHMETARECORD *lpEMFR,
    int nObj, LPARAM lpData)
{
    MetafileType *result = (MetafileType*)lpData;

    if (lpEMFR->iType == EMR_GDICOMMENT)
    {
        const EMRGDICOMMENT *comment = (const EMRGDICOMMENT*)lpEMFR;

        if (comment->cbData >= 4 && memcmp(comment->Data, "EMF+", 4) == 0)
        {
            const EmfPlusRecordHeader *header = (const EmfPlusRecordHeader*)&comment->Data[4];

            if (4 + sizeof(EmfPlusRecordHeader) <= comment->cbData &&
                header->Type == EmfPlusRecordTypeHeader)
            {
                if ((header->Flags & 1) == 1)
                    *result = MetafileTypeEmfPlusDual;
                else
                    *result = MetafileTypeEmfPlusOnly;
            }
        }
        else
            *result = MetafileTypeEmf;
    }
    else
        *result = MetafileTypeEmf;

    return FALSE;
}

MetafileType METAFILE_GetEmfType(HENHMETAFILE hemf)
{
    MetafileType result = MetafileTypeInvalid;
    EnumEnhMetaFile(NULL, hemf, get_metafile_type_proc, &result, NULL);
    return result;
}
