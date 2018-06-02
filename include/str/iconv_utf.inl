/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (uint32)0x0000FFFD
#define UNI_MAX_BMP (uint32)0x0000FFFF
#define UNI_MAX_UTF16 (uint32)0x0010FFFF
#define UNI_MAX_UTF32 (uint32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (uint32)0x0010FFFF
typedef enum {
  conversionOK, /* conversion successful */
  sourceExhausted, /* partial character in source, but hit end */
  targetExhausted, /* insuff. room in target for conversion */
  sourceIllegal /* source sequence is illegal/malformed */
} ConversionResult;
typedef enum {
  strictConversion = 0,
  lenientConversion
} ConversionFlags;
static const int halfShift = 10; /* used for shifting by 10 bits */
static const uint32 halfBase = 0x0010000UL;
static const uint32 halfMask = 0x3FFUL;
#define UNI_SUR_HIGH_START (uint32)0xD800
#define UNI_SUR_HIGH_END (uint32)0xDBFF
#define UNI_SUR_LOW_START (uint32)0xDC00
#define UNI_SUR_LOW_END (uint32)0xDFFF
CC_INLINE ConversionResult ConvertUTF32toUTF16(const uint32** sourceStart, const uint32* sourceEnd, uint16** targetStart, uint16* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const uint32* source = *sourceStart;
  uint16* target = *targetStart;
  while (source < sourceEnd) {
    uint32 ch;
    if (target >= targetEnd) {
      result = targetExhausted;
      break;
    }
    ch = *source++;
    if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
      /* UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values */
      if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
        if (flags == strictConversion) {
          --source; /* return to the illegal value itself */
          result = sourceIllegal;
          break;
        }
        else {
          *target++ = UNI_REPLACEMENT_CHAR;
        }
      }
      else {
        *target++ = (uint16)ch; /* normal case */
      }
    }
    else if (ch > UNI_MAX_LEGAL_UTF32) {
      if (flags == strictConversion) {
        result = sourceIllegal;
      }
      else {
        *target++ = UNI_REPLACEMENT_CHAR;
      }
    }
    else {
      /* target is a character in range 0xFFFF - 0x10FFFF. */
      if (target + 1 >= targetEnd) {
        --source; /* Back up source pointer! */
        result = targetExhausted;
        break;
      }
      ch -= halfBase;
      *target++ = (uint16)((ch >> halfShift) + UNI_SUR_HIGH_START);
      *target++ = (uint16)((ch & halfMask) + UNI_SUR_LOW_START);
    }
  }
  *sourceStart = source;
  *targetStart = target;
  return result;
}
CC_INLINE ConversionResult ConvertUTF16toUTF32(const uint16** sourceStart, const uint16* sourceEnd, uint32** targetStart, uint32* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const uint16* source = *sourceStart;
  uint32* target = *targetStart;
  uint32 ch, ch2;
  while (source < sourceEnd) {
    const uint16* oldSource = source; /* In case we have to back up because of target overflow. */
    ch = *source++;
    /* If we have a surrogate pair, convert to uint32 first. */
    if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
      /* If the 16 bits following the high surrogate are in the source buffer... */
      if (source < sourceEnd) {
        ch2 = *source;
        /* If it's a low surrogate, convert to uint32. */
        if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
          ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
              + (ch2 - UNI_SUR_LOW_START) + halfBase;
          ++source;
        }
        else if (flags == strictConversion) { /* it's an unpaired high surrogate */
          --source; /* return to the illegal value itself */
          result = sourceIllegal;
          break;
        }
      }
      else { /* We don't have the 16 bits following the high surrogate. */
        --source; /* return to the high surrogate */
        result = sourceExhausted;
        break;
      }
    }
    else if (flags == strictConversion) {
      /* UTF-16 surrogate values are illegal in UTF-32 */
      if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
        --source; /* return to the illegal value itself */
        result = sourceIllegal;
        break;
      }
    }
    if (target >= targetEnd) {
      source = oldSource; /* Back up source pointer! */
      result = targetExhausted;
      break;
    }
    *target++ = ch;
  }
  *sourceStart = source;
  *targetStart = target;
#ifdef CVTUTF_DEBUG
  if (result == sourceIllegal) {
    fprintf(stderr, "ConvertUTF16toUTF32 illegal seq 0x%04x,%04x/n", ch, ch2);
    fflush(stderr);
  }
#endif
  return result;
}
/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
static const char trailingBytesForUTF8[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};
/*
 * Magic values subtracted from a buffer value during uint8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const uint32 offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL};
/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow. There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const uint8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
/* The interface converts a whole buffer to avoid function-call overhead.
 * Constants have been gathered. Loops & conditionals have been removed as
 * much as possible for efficiency, in favor of drop-through switches.
 * (See "Note A" at the bottom of the file for equivalent code.)
 * If your compiler supports it, the "isLegalUTF8" call can be turned
 * into an inline function.
 */
CC_INLINE ConversionResult ConvertUTF16toUTF8(const uint16** sourceStart, const uint16* sourceEnd, uint8** targetStart, uint8* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const uint16* source = *sourceStart;
  uint8* target = *targetStart;
  while (source < sourceEnd) {
    uint32 ch;
    unsigned short bytesToWrite = 0;
    const uint32 byteMask = 0xBF;
    const uint32 byteMark = 0x80;
    const uint16* oldSource = source; /* In case we have to back up because of target overflow. */
    ch = *source++;
    /* If we have a surrogate pair, convert to uint32 first. */
    if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
      /* If the 16 bits following the high surrogate are in the source buffer... */
      if (source < sourceEnd) {
        uint32 ch2 = *source;
        /* If it's a low surrogate, convert to uint32. */
        if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
          ch = ((ch - UNI_SUR_HIGH_START) << halfShift)
              + (ch2 - UNI_SUR_LOW_START) + halfBase;
          ++source;
        }
        else if (flags == strictConversion) { /* it's an unpaired high surrogate */
          --source; /* return to the illegal value itself */
          result = sourceIllegal;
          break;
        }
      }
      else { /* We don't have the 16 bits following the high surrogate. */
        --source; /* return to the high surrogate */
        result = sourceExhausted;
        break;
      }
    }
    else if (flags == strictConversion) {
      /* UTF-16 surrogate values are illegal in UTF-32 */
      if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
        --source; /* return to the illegal value itself */
        result = sourceIllegal;
        break;
      }
    }
    /* Figure out how many bytes the result will require */
    if (ch < (uint32)0x80) {
      bytesToWrite = 1;
    }
    else if (ch < (uint32)0x800) {
      bytesToWrite = 2;
    }
    else if (ch < (uint32)0x10000) {
      bytesToWrite = 3;
    }
    else if (ch < (uint32)0x110000) {
      bytesToWrite = 4;
    }
    else {
      bytesToWrite = 3;
      ch = UNI_REPLACEMENT_CHAR;
    }
    target += bytesToWrite;
    if (target > targetEnd) {
      source = oldSource; /* Back up source pointer! */
      target -= bytesToWrite;
      result = targetExhausted;
      break;
    }
    switch (bytesToWrite) { /* note: everything falls through. */
    case 4:
      *--target = (uint8)((ch | byteMark) & byteMask);
      ch >>= 6;
    case 3:
      *--target = (uint8)((ch | byteMark) & byteMask);
      ch >>= 6;
    case 2:
      *--target = (uint8)((ch | byteMark) & byteMask);
      ch >>= 6;
    case 1:
      *--target = (uint8)(ch | firstByteMark[bytesToWrite]);
    }
    target += bytesToWrite;
  }
  *sourceStart = source;
  *targetStart = target;
  return result;
}
/*
 * Utility routine to tell whether a sequence of bytes is legal UTF-8.
 * This must be called with the length pre-determined by the first byte.
 * If not calling this from ConvertUTF8to*, then the length can be set by:
 * length = trailingBytesForUTF8[*source]+1;
 * and the sequence is illegal right away if there aren't that many bytes
 * available.
 * If presented with a length > 4, this returns FALSE. The Unicode
 * definition of UTF-8 goes up to 4-byte sequences.
 */
static BOOL isLegalUTF8(const uint8* source, int length)
{
  uint8 a;
  const uint8* srcptr = source + length;
  switch (length) {
  default:
    return FALSE;
    /* Everything else falls through when "TRUE"... */
  case 4:
    if ((a = (*--srcptr)) < 0x80 || a > 0xBF) {
      return FALSE;
    }
  case 3:
    if ((a = (*--srcptr)) < 0x80 || a > 0xBF) {
      return FALSE;
    }
  case 2:
    if ((a = (*--srcptr)) > 0xBF) {
      return FALSE;
    }
    switch (*source) {
      /* no fall-through in this inner switch */
    case 0xE0:
      if (a < 0xA0) {
        return FALSE;
      }
      break;
    case 0xED:
      if (a > 0x9F) {
        return FALSE;
      }
      break;
    case 0xF0:
      if (a < 0x90) {
        return FALSE;
      }
      break;
    case 0xF4:
      if (a > 0x8F) {
        return FALSE;
      }
      break;
    default:
      if (a < 0x80) {
        return FALSE;
      }
    }
  case 1:
    if (*source >= 0x80 && *source < 0xC2) {
      return FALSE;
    }
  }
  if (*source > 0xF4) {
    return FALSE;
  }
  return TRUE;
}
/*
 * Exported function to return whether a UTF-8 sequence is legal or not.
 * This is not used here; it's just exported.
 */
CC_INLINE BOOL isLegalUTF8Sequence(const uint8* source, const uint8* sourceEnd)
{
  int length = trailingBytesForUTF8[*source] + 1;
  if (source + length > sourceEnd) {
    return FALSE;
  }
  return isLegalUTF8(source, length);
}
CC_INLINE ConversionResult ConvertUTF8toUTF16(const uint8** sourceStart, const uint8* sourceEnd, uint16** targetStart, uint16* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const uint8* source = *sourceStart;
  uint16* target = *targetStart;
  while (source < sourceEnd) {
    uint32 ch = 0;
    unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
    if (source + extraBytesToRead >= sourceEnd) {
      result = sourceExhausted;
      break;
    }
    /* Do this check whether lenient or strict */
    if (! isLegalUTF8(source, extraBytesToRead + 1)) {
      result = sourceIllegal;
      break;
    }
    /*
    * The cases all fall through. See "Note A" below.
    */
    switch (extraBytesToRead) {
    case 5:
      ch += *source++;
      ch <<= 6; /* remember, illegal UTF-8 */
    case 4:
      ch += *source++;
      ch <<= 6; /* remember, illegal UTF-8 */
    case 3:
      ch += *source++;
      ch <<= 6;
    case 2:
      ch += *source++;
      ch <<= 6;
    case 1:
      ch += *source++;
      ch <<= 6;
    case 0:
      ch += *source++;
    }
    ch -= offsetsFromUTF8[extraBytesToRead];
    if (target >= targetEnd) {
      source -= (extraBytesToRead + 1); /* Back up source pointer! */
      result = targetExhausted;
      break;
    }
    if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
      /* UTF-16 surrogate values are illegal in UTF-32 */
      if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
        if (flags == strictConversion) {
          source -= (extraBytesToRead + 1); /* return to the illegal value itself */
          result = sourceIllegal;
          break;
        }
        else {
          *target++ = UNI_REPLACEMENT_CHAR;
        }
      }
      else {
        *target++ = (uint16)ch; /* normal case */
      }
    }
    else if (ch > UNI_MAX_UTF16) {
      if (flags == strictConversion) {
        result = sourceIllegal;
        source -= (extraBytesToRead + 1); /* return to the start */
        break; /* Bail out; shouldn't continue */
      }
      else {
        *target++ = UNI_REPLACEMENT_CHAR;
      }
    }
    else {
      /* target is a character in range 0xFFFF - 0x10FFFF. */
      if (target + 1 >= targetEnd) {
        source -= (extraBytesToRead + 1); /* Back up source pointer! */
        result = targetExhausted;
        break;
      }
      ch -= halfBase;
      *target++ = (uint16)((ch >> halfShift) + UNI_SUR_HIGH_START);
      *target++ = (uint16)((ch & halfMask) + UNI_SUR_LOW_START);
    }
  }
  *sourceStart = source;
  *targetStart = target;
  return result;
}
CC_INLINE ConversionResult ConvertUTF32toUTF8(const uint32** sourceStart, const uint32* sourceEnd, uint8** targetStart, uint8* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const uint32* source = *sourceStart;
  uint8* target = *targetStart;
  while (source < sourceEnd) {
    uint32 ch;
    unsigned short bytesToWrite = 0;
    const uint32 byteMask = 0xBF;
    const uint32 byteMark = 0x80;
    ch = *source++;
    if (flags == strictConversion) {
      /* UTF-16 surrogate values are illegal in UTF-32 */
      if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
        --source; /* return to the illegal value itself */
        result = sourceIllegal;
        break;
      }
    }
    /*
    * Figure out how many bytes the result will require. Turn any
    * illegally large uint32 things (> Plane 17) into replacement chars.
    */
    if (ch < (uint32)0x80) {
      bytesToWrite = 1;
    }
    else if (ch < (uint32)0x800) {
      bytesToWrite = 2;
    }
    else if (ch < (uint32)0x10000) {
      bytesToWrite = 3;
    }
    else if (ch <= UNI_MAX_LEGAL_UTF32) {
      bytesToWrite = 4;
    }
    else {
      bytesToWrite = 3;
      ch = UNI_REPLACEMENT_CHAR;
      result = sourceIllegal;
    }
    target += bytesToWrite;
    if (target > targetEnd) {
      --source; /* Back up source pointer! */
      target -= bytesToWrite;
      result = targetExhausted;
      break;
    }
    switch (bytesToWrite) { /* note: everything falls through. */
    case 4:
      *--target = (uint8)((ch | byteMark) & byteMask);
      ch >>= 6;
    case 3:
      *--target = (uint8)((ch | byteMark) & byteMask);
      ch >>= 6;
    case 2:
      *--target = (uint8)((ch | byteMark) & byteMask);
      ch >>= 6;
    case 1:
      *--target = (uint8)(ch | firstByteMark[bytesToWrite]);
    }
    target += bytesToWrite;
  }
  *sourceStart = source;
  *targetStart = target;
  return result;
}
CC_INLINE ConversionResult ConvertUTF8toUTF32(const uint8** sourceStart, const uint8* sourceEnd, uint32** targetStart, uint32* targetEnd, ConversionFlags flags)
{
  ConversionResult result = conversionOK;
  const uint8* source = *sourceStart;
  uint32* target = *targetStart;
  while (source < sourceEnd) {
    uint32 ch = 0;
    unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
    if (source + extraBytesToRead >= sourceEnd) {
      result = sourceExhausted;
      break;
    }
    /* Do this check whether lenient or strict */
    if (! isLegalUTF8(source, extraBytesToRead + 1)) {
      result = sourceIllegal;
      break;
    }
    /*
    * The cases all fall through. See "Note A" below.
    */
    switch (extraBytesToRead) {
    case 5:
      ch += *source++;
      ch <<= 6;
    case 4:
      ch += *source++;
      ch <<= 6;
    case 3:
      ch += *source++;
      ch <<= 6;
    case 2:
      ch += *source++;
      ch <<= 6;
    case 1:
      ch += *source++;
      ch <<= 6;
    case 0:
      ch += *source++;
    }
    ch -= offsetsFromUTF8[extraBytesToRead];
    if (target >= targetEnd) {
      source -= (extraBytesToRead + 1); /* Back up the source pointer! */
      result = targetExhausted;
      break;
    }
    if (ch <= UNI_MAX_LEGAL_UTF32) {
      /*
      * UTF-16 surrogate values are illegal in UTF-32, and anything
      * over Plane 17 (> 0x10FFFF) is illegal.
      */
      if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
        if (flags == strictConversion) {
          source -= (extraBytesToRead + 1); /* return to the illegal value itself */
          result = sourceIllegal;
          break;
        }
        else {
          *target++ = UNI_REPLACEMENT_CHAR;
        }
      }
      else {
        *target++ = ch;
      }
    }
    else { /* i.e., ch > UNI_MAX_LEGAL_UTF32 */
      result = sourceIllegal;
      *target++ = UNI_REPLACEMENT_CHAR;
    }
  }
  *sourceStart = source;
  *targetStart = target;
  return result;
}
/* ---------------------------------------------------------------------
 Note A.
 The fall-through switches in UTF-8 reading code save a
 temp variable, some decrements & conditionals. The switches
 are equivalent to the following loop:
 {
 int tmpBytesToRead = extraBytesToRead+1;
 do {
 ch += *source++;
 --tmpBytesToRead;
 if (tmpBytesToRead) ch <<= 6;
 } while (tmpBytesToRead > 0);
 }
 In UTF-8 writing code, the switches on "bytesToWrite" are
 similarly unrolled loops.
 --------------------------------------------------------------------- */

