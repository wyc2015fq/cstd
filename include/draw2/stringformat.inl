
Status  GdipCreateStringFormat(INT attr, LANGID lang, StringFormat *format)
{
    TRACE("(%i, %x, %p)\n", attr, lang, format);

    if(!format)
        return InvalidParameter;

    if(!format)   return OutOfMemory;

    format->attr = attr;
    format->lang = lang;
    format->digitlang = LANG_NEUTRAL;
    format->trimming = StringTrimmingCharacter;
    format->digitsub = StringDigitSubstituteUser;
    format->character_ranges = NULL;
    format->range_count = 0;
    format->generic_typographic = FALSE;
    /* tabstops */
    format->tabcount = 0;
    format->firsttab = 0.0;
    format->tabs = NULL;

    TRACE("<-- %p\n", *format);

    return Ok;
}

Status  GdipDeleteStringFormat(StringFormat *format)
{
    if(!format)
        return InvalidParameter;

    GdipFree(format->character_ranges);
    GdipFree(format->tabs);
    GdipFree(format);

    return Ok;
}

Status  GdipStringFormatGetGenericDefault(StringFormat *format)
{
    Status stat;

    if (!format)
        return InvalidParameter;

    stat = GdipCreateStringFormat(0, LANG_NEUTRAL, format);
    if(stat != Ok)
        return stat;

    format->align     = StringAlignmentNear;
    format->vertalign = StringAlignmentNear;

    return Ok;
}

Status  GdipGetStringFormatAlign(StringFormat *format,
    StringAlignment *align)
{
    if(!format || !align)
        return InvalidParameter;

    *align = format->align;

    return Ok;
}

Status  GdipGetStringFormatDigitSubstitution(GDIPCONST StringFormat *format,
    LANGID *language, StringDigitSubstitute *substitute)
{
    if(!format)
        return InvalidParameter;

    if(language)    *language   = format->digitlang;
    if(substitute)  *substitute = format->digitsub;

    return Ok;
}

Status  GdipGetStringFormatFlags(GDIPCONST StringFormat* format,
        INT* flags)
{
    if (!(format && flags))
        return InvalidParameter;

    *flags = format->attr;

    return Ok;
}

Status  GdipGetStringFormatHotkeyPrefix(GDIPCONST StringFormat
    *format, INT *hkpx)
{
    if(!format || !hkpx)
        return InvalidParameter;

    *hkpx = (INT)format->hkprefix;

    return Ok;
}

Status  GdipGetStringFormatLineAlign(StringFormat *format,
    StringAlignment *align)
{
    if(!format || !align)
        return InvalidParameter;

    *align = format->vertalign;

    return Ok;
}

Status  GdipGetStringFormatMeasurableCharacterRangeCount(
    GDIPCONST StringFormat *format, INT *count)
{
    if (!(format && count))
        return InvalidParameter;

    TRACE("%p %p\n", format, count);

    *count = format->range_count;

    return Ok;
}

Status  GdipGetStringFormatTabStopCount(GDIPCONST StringFormat *format,
    INT *count)
{
    if(!format || !count)
        return InvalidParameter;

    *count = format->tabcount;

    return Ok;
}

Status  GdipGetStringFormatTabStops(GDIPCONST StringFormat *format, INT count,
    REAL *firsttab, REAL *tabs)
{
    if(!format || !firsttab || !tabs)
        return InvalidParameter;

    /* native simply crashes on count < 0 */
    if(count != 0)
        memcpy(tabs, format->tabs, sizeof(REAL)*count);

    *firsttab = format->firsttab;

    return Ok;
}

Status  GdipGetStringFormatTrimming(StringFormat *format,
    StringTrimming *trimming)
{
    if(!format || !trimming)
        return InvalidParameter;

    *trimming = format->trimming;

    return Ok;
}

Status  GdipSetStringFormatAlign(StringFormat *format,
    StringAlignment align)
{
    TRACE("(%p, %i)\n", format, align);

    if(!format)
        return InvalidParameter;

    format->align = align;

    return Ok;
}

/*FIXME: digit substitution actually not implemented, get/set only */
Status  GdipSetStringFormatDigitSubstitution(StringFormat *format,
    LANGID language, StringDigitSubstitute substitute)
{
    TRACE("(%p, %x, %i)\n", format, language, substitute);

    if(!format)
        return InvalidParameter;

    format->digitlang = language;
    format->digitsub  = substitute;

    return Ok;
}

Status  GdipSetStringFormatHotkeyPrefix(StringFormat *format,
    INT hkpx)
{
    TRACE("(%p, %i)\n", format, hkpx);

    if(!format || hkpx < 0 || hkpx > 2)
        return InvalidParameter;

    format->hkprefix = (HotkeyPrefix) hkpx;

    return Ok;
}

Status  GdipSetStringFormatLineAlign(StringFormat *format,
    StringAlignment align)
{
    TRACE("(%p, %i)\n", format, align);

    if(!format)
        return InvalidParameter;

    format->vertalign = align;

    return Ok;
}

Status  GdipSetStringFormatMeasurableCharacterRanges(
    StringFormat *format, INT rangeCount, GDIPCONST CharacterRange *ranges)
{
    CharacterRange *new_ranges;

    if (!(format && ranges))
        return InvalidParameter;

    TRACE("%p, %d, %p\n", format, rangeCount, ranges);

    new_ranges = GdipAlloc(rangeCount * sizeof(CharacterRange));
    if (!new_ranges)
        return OutOfMemory;

    GdipFree(format->character_ranges);
    format->character_ranges = new_ranges;
    memcpy(format->character_ranges, ranges, sizeof(CharacterRange) * rangeCount);
    format->range_count = rangeCount;

    return Ok;
}

Status  GdipSetStringFormatTabStops(StringFormat *format, REAL firsttab,
    INT count, GDIPCONST REAL *tabs)
{
    TRACE("(%p, %0.2f, %i, %p)\n", format, firsttab, count, tabs);

    if(!format || !tabs)
        return InvalidParameter;

    if(count > 0){
        if(firsttab < 0.0)  return NotImplemented;
        /* first time allocation */
        if(format->tabcount == 0){
            format->tabs = GdipAlloc(sizeof(REAL)*count);
            if(!format->tabs)
                return OutOfMemory;
        }
        /* reallocation */
        if((format->tabcount < count) && (format->tabcount > 0)){
            REAL *ptr;
            ptr = HeapReAlloc(GetProcessHeap(), 0, format->tabs, sizeof(REAL)*count);
            if(!ptr)
                return OutOfMemory;
            format->tabs = ptr;
        }
        format->firsttab = firsttab;
        format->tabcount = count;
        memcpy(format->tabs, tabs, sizeof(REAL)*count);
    }

    return Ok;
}

Status  GdipSetStringFormatTrimming(StringFormat *format,
    StringTrimming trimming)
{
    TRACE("(%p, %i)\n", format, trimming);

    if(!format)
        return InvalidParameter;

    format->trimming = trimming;

    return Ok;
}

Status  GdipSetStringFormatFlags(StringFormat *format, INT flags)
{
    TRACE("(%p, %x)\n", format, flags);

    if(!format)
        return InvalidParameter;

    format->attr = flags;

    return Ok;
}

Status  GdipCloneStringFormat(GDIPCONST StringFormat *format, StringFormat *newFormat)
{
    if(!format || !newFormat)
        return InvalidParameter;

    *newFormat = GdipAlloc(sizeof(StringFormat));
    if(!*newFormat)    return OutOfMemory;

    **newFormat = *format;

    if(format->tabcount > 0){
        (*newFormat)->tabs = GdipAlloc(sizeof(REAL) * format->tabcount);
        if(!(*newFormat)->tabs){
            GdipFree(*newFormat);
            return OutOfMemory;
        }
        memcpy((*newFormat)->tabs, format->tabs, sizeof(REAL) * format->tabcount);
    }
    else
        (*newFormat)->tabs = NULL;

    if(format->range_count > 0){
        (*newFormat)->character_ranges = GdipAlloc(sizeof(CharacterRange) * format->range_count);
        if(!(*newFormat)->character_ranges){
            GdipFree((*newFormat)->tabs);
            GdipFree(*newFormat);
            return OutOfMemory;
        }
        memcpy((*newFormat)->character_ranges, format->character_ranges,
               sizeof(CharacterRange) * format->range_count);
    }
    else
        (*newFormat)->character_ranges = NULL;

    TRACE("%p %p\n",format,newFormat);

    return Ok;
}

Status  GdipStringFormatGetGenericTypographic(StringFormat *format)
{
    Status stat;

    if(!format)
        return InvalidParameter;

    stat = GdipCreateStringFormat(StringFormatFlagsNoFitBlackBox |
                                  StringFormatFlagsLineLimit |
                                  StringFormatFlagsNoClip, LANG_NEUTRAL, format);
    if(stat != Ok)
        return stat;

    format->digitlang = LANG_NEUTRAL;
    format->digitsub  = StringDigitSubstituteUser;
    format->trimming  = StringTrimmingNone;
    format->hkprefix  = HotkeyPrefixNone;
    format->align     = StringAlignmentNear;
    format->vertalign = StringAlignmentNear;
    format->generic_typographic = TRUE;

    TRACE("%p => %p\n", format, *format);

    return Ok;
}
