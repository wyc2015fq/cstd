
#ifndef __GDI_STRING_FORMAT_HPP__
#define __GDI_STRING_FORMAT_HPP__

struct Graphics;
struct GraphicsPath;

struct StringFormat
{
    StringFormat(
        IN INT     formatFlags = 0,
        IN LANGID  language = LANG_NEUTRAL
    )
    {
        nativeFormat = NULL;
        lastError = DllExports::GdipCreateStringFormat(
            formatFlags,
            language,
            &nativeFormat
        );
    }

    static const StringFormat *GenericDefault();
    static const StringFormat *GenericTypographic();

    StringFormat(
        IN const StringFormat *format
    )
    {
        nativeFormat = NULL;
        lastError = DllExports::GdipCloneStringFormat(
            format ? format->nativeFormat : NULL,
            &nativeFormat
        );
    }

    StringFormat *Clone() const
    {
        GpStringFormat *clonedStringFormat = NULL;

        lastError = DllExports::GdipCloneStringFormat(
            nativeFormat,
            &clonedStringFormat
        );

        if (lastError == Ok)
            return new StringFormat(clonedStringFormat, lastError);
        else
            return NULL;
    }

    ~StringFormat()
    {
        DllExports::GdipDeleteStringFormat(nativeFormat);
    }

    CError SetFormatFlags(IN INT flags)
    {
        return SetStatus(DllExports::GdipSetStringFormatFlags(
            nativeFormat,
            flags
        ));
    }

    INT GetFormatFlags() const
    {
        INT flags;
        SetStatus(DllExports::GdipGetStringFormatFlags(nativeFormat, &flags));
        return flags;
    }

    CError SetAlignment(IN StringAlignment align)
    {
        return SetStatus(DllExports::GdipSetStringFormatAlign(
            nativeFormat,
            align
        ));
    }

    StringAlignment GetAlignment() const
    {
        StringAlignment alignment;
        SetStatus(DllExports::GdipGetStringFormatAlign(
            nativeFormat,
            &alignment
        ));
        return alignment;
    }

    CError SetLineAlignment(IN StringAlignment align)
    {
        return SetStatus(DllExports::GdipSetStringFormatLineAlign(
            nativeFormat,
            align
        ));
    }

    StringAlignment GetLineAlignment() const
    {
        StringAlignment alignment;
        SetStatus(DllExports::GdipGetStringFormatLineAlign(
            nativeFormat,
            &alignment
        ));
        return alignment;
    }

    CError SetHotkeyPrefix(IN HotkeyPrefix hotkeyPrefix)
    {
        return SetStatus(DllExports::GdipSetStringFormatHotkeyPrefix(
            nativeFormat,
            (INT)hotkeyPrefix
        ));
    }

    HotkeyPrefix GetHotkeyPrefix() const
    {
        HotkeyPrefix hotkeyPrefix;
        SetStatus(DllExports::GdipGetStringFormatHotkeyPrefix(
            nativeFormat,
            (INT*)&hotkeyPrefix
        ));
        return hotkeyPrefix;
    }

    CError SetTabStops(
        IN REAL    firstTabOffset,
        IN INT     count,
        IN const REAL    *tabStops
    )
    {
        return SetStatus(DllExports::GdipSetStringFormatTabStops(
            nativeFormat,
            firstTabOffset,
            count,
            tabStops
        ));
    }

    INT GetTabStopCount() const
    {
        INT count;
        SetStatus(DllExports::GdipGetStringFormatTabStopCount(nativeFormat, &count));
        return count;
    }

    CError GetTabStops(
        IN INT     count,
        OUT REAL   *firstTabOffset,
        OUT REAL   *tabStops
    ) const
    {
        return SetStatus(DllExports::GdipGetStringFormatTabStops(
            nativeFormat,
            count,
            firstTabOffset,
            tabStops
        ));
    }

    CError SetDigitSubstitution(
        IN LANGID                language,
        IN StringDigitSubstitute substitute
    )
    {
        return SetStatus(DllExports::GdipSetStringFormatDigitSubstitution(
            nativeFormat,
            language,
            substitute
        ));
    }

    LANGID GetDigitSubstitutionLanguage(
    ) const
    {
        LANGID language;
        SetStatus(DllExports::GdipGetStringFormatDigitSubstitution(
            nativeFormat,
            &language,
            NULL
        ));
        return language;
    }

    StringDigitSubstitute GetDigitSubstitutionMethod(
    ) const
    {
        StringDigitSubstitute substitute;
        SetStatus(DllExports::GdipGetStringFormatDigitSubstitution(
            nativeFormat,
            NULL,
            &substitute
        ));
        return substitute;
    }

    CError SetTrimming(IN StringTrimming trimming)
    {
        return SetStatus(DllExports::GdipSetStringFormatTrimming(
            nativeFormat,
            trimming
        ));
    }

    StringTrimming StringFormat::GetTrimming() const
    {
        StringTrimming trimming;
        SetStatus(DllExports::GdipGetStringFormatTrimming(
            nativeFormat,
            &trimming
        ));
        return trimming;
    }

    CError SetMeasurableCharacterRanges(
        IN INT                  rangeCount,
        IN const CharacterRange *ranges
    )
    {
        return SetStatus(DllExports::GdipSetStringFormatMeasurableCharacterRanges(
            nativeFormat,
            rangeCount,
            ranges
        ));
    }

    INT GetMeasurableCharacterRangeCount()
    {
        INT count;
        SetStatus(DllExports::GdipGetStringFormatMeasurableCharacterRangeCount(
            nativeFormat,
            &count
        ));
        return count;
    }

    CError GetLastStatus() const
    {
        CError lastStatus = lastError;
        lastError = Ok;

        return lastStatus;
    }

protected:

    CError SetStatus(GpStatus newStatus) const
    {
        if (newStatus == Ok)
        {
            return Ok;
        }
        else
        {
            return lastError = newStatus;
        }
    }

    StringFormat(const StringFormat &source)
    {
        nativeFormat = NULL;
        lastError = DllExports::GdipCloneStringFormat(
            source.nativeFormat,
            &nativeFormat
        );
    }

    StringFormat& operator=(const StringFormat &source)
    {
        DllExports::GdipDeleteStringFormat(nativeFormat);
        lastError = DllExports::GdipCloneStringFormat(
            source.nativeFormat,
            &nativeFormat
        );
        return *this;
    }

    StringFormat(GpStringFormat * clonedStringFormat, CError status)
    {
        lastError = status;
        nativeFormat = clonedStringFormat;

    }

    GpStringFormat *nativeFormat;
    mutable CError  lastError;
};

static BYTE GenericTypographicStringFormatBuffer[sizeof(StringFormat)] = {0};
static BYTE GenericDefaultStringFormatBuffer[sizeof(StringFormat)] = {0};

inline const StringFormat *StringFormat::GenericDefault()
{
    StringFormat * genericDefaultStringFormat =
        (StringFormat*)GenericDefaultStringFormatBuffer;

    genericDefaultStringFormat->lastError =
        DllExports::GdipStringFormatGetGenericDefault(
            &(genericDefaultStringFormat->nativeFormat)
        );

    return genericDefaultStringFormat;
}

inline const StringFormat *StringFormat::GenericTypographic()
{
    StringFormat * genericTypographicStringFormat =
        (StringFormat*)GenericTypographicStringFormatBuffer;

    genericTypographicStringFormat->lastError =
        DllExports::GdipStringFormatGetGenericTypographic(
            &genericTypographicStringFormat->nativeFormat
        );

    return genericTypographicStringFormat;
}

#endif // __GDI_STRING_FORMAT_HPP__

