#ifndef _IMUI_FONT_INL_
#define _IMUI_FONT_INL_

///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4456) // declaration of 'xx' hides previous local declaration
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast" // warning : use of old-style cast // yes, they are more terse.
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits" // warning: comparison is always true due to limited range of data type [-Wtype-limits]
#endif
#define STBRP_ASSERT(x) ASSERT(x)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#include "inc/stb_rect_pack.h"
#define STBTT_malloc(x,u) ((void)(u), myMemAlloc(x))
#define STBTT_free(x,u) ((void)(u), myMemFree(x))
#define STBTT_assert(x) ASSERT(x)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF
#endif
#undef STBTT_STATIC
#undef STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
#pragma warning (pop)
#endif
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// ProggyClean.ttf
// Copyright (c) 2004, 2005 Tristan Grimmer
// MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
// Download and more information at http://upperbounds.net
//-----------------------------------------------------------------------------
// File: 'ProggyClean.ttf' (41208 bytes)
// Exported using binary_to_compressed_c.cpp
//-----------------------------------------------------------------------------
// Retrieve list of range (2 int per range, values are inclusive)
const ushort* ImFontConfig_GetGlyphRangesDefault()
{
  static const ushort ranges[] = {
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0,
  };
  return &ranges[0];
}
const ushort* ImFontConfig_GetGlyphRangesKorean()
{
  static const ushort ranges[] = {
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x3131, 0x3163, // Korean alphabets
    0xAC00, 0xD79D, // Korean characters
    0,
  };
  return &ranges[0];
}
const ushort* ImFontConfig_GetGlyphRangesChinese()
{
  static const ushort ranges[] = {
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
    0x31F0, 0x31FF, // Katakana Phonetic Extensions
    0xFF00, 0xFFEF, // Half-width characters
    0x4e00, 0x9FAF, // CJK Ideograms
    0,
  };
  return &ranges[0];
}
const ushort* ImFontConfig_GetGlyphRangesJapanese()
{
  // Store the 1946 ideograms code points as successive offsets from the initial unicode codepoint 0x4E00. Each offset has an implicit +1.
  // This encoding helps us reduce the source code size.
  static const short offsets_from_0x4E00[] = {
    -1, 0, 1, 3, 0, 0, 0, 0, 1, 0, 5, 1, 1, 0, 7, 4, 6, 10, 0, 1, 9, 9, 7, 1, 3, 19, 1, 10, 7, 1, 0, 1, 0, 5, 1, 0, 6, 4, 2, 6, 0, 0, 12, 6, 8, 0, 3, 5, 0, 1, 0, 9, 0, 0, 8, 1, 1, 3, 4, 5, 13, 0, 0, 8, 2, 17,
    4, 3, 1, 1, 9, 6, 0, 0, 0, 2, 1, 3, 2, 22, 1, 9, 11, 1, 13, 1, 3, 12, 0, 5, 9, 2, 0, 6, 12, 5, 3, 12, 4, 1, 2, 16, 1, 1, 4, 6, 5, 3, 0, 6, 13, 15, 5, 12, 8, 14, 0, 0, 6, 15, 3, 6, 0, 18, 8, 1, 6, 14, 1,
    5, 4, 12, 24, 3, 13, 12, 10, 24, 0, 0, 0, 1, 0, 1, 1, 2, 9, 10, 2, 2, 0, 0, 3, 3, 1, 0, 3, 8, 0, 3, 2, 4, 4, 1, 6, 11, 10, 14, 6, 15, 3, 4, 15, 1, 0, 0, 5, 2, 2, 0, 0, 1, 6, 5, 5, 6, 0, 3, 6, 5, 0, 0, 1, 0,
    11, 2, 2, 8, 4, 7, 0, 10, 0, 1, 2, 17, 19, 3, 0, 2, 5, 0, 6, 2, 4, 4, 6, 1, 1, 11, 2, 0, 3, 1, 2, 1, 2, 10, 7, 6, 3, 16, 0, 8, 24, 0, 0, 3, 1, 1, 3, 0, 1, 6, 0, 0, 0, 2, 0, 1, 5, 15, 0, 1, 0, 0, 2, 11, 19,
    1, 4, 19, 7, 6, 5, 1, 0, 0, 0, 0, 5, 1, 0, 1, 9, 0, 0, 5, 0, 2, 0, 1, 0, 3, 0, 11, 3, 0, 2, 0, 0, 0, 0, 0, 9, 3, 6, 4, 12, 0, 14, 0, 0, 29, 10, 8, 0, 14, 37, 13, 0, 31, 16, 19, 0, 8, 30, 1, 20, 8, 3, 48,
    21, 1, 0, 12, 0, 10, 44, 34, 42, 54, 11, 18, 82, 0, 2, 1, 2, 12, 1, 0, 6, 2, 17, 2, 12, 7, 0, 7, 17, 4, 2, 6, 24, 23, 8, 23, 39, 2, 16, 23, 1, 0, 5, 1, 2, 15, 14, 5, 6, 2, 11, 0, 8, 6, 2, 2, 2, 14,
    20, 4, 15, 3, 4, 11, 10, 10, 2, 5, 2, 1, 30, 2, 1, 0, 0, 22, 5, 5, 0, 3, 1, 5, 4, 1, 0, 0, 2, 2, 21, 1, 5, 1, 2, 16, 2, 1, 3, 4, 0, 8, 4, 0, 0, 5, 14, 11, 2, 16, 1, 13, 1, 7, 0, 22, 15, 3, 1, 22, 7, 14,
    22, 19, 11, 24, 18, 46, 10, 20, 64, 45, 3, 2, 0, 4, 5, 0, 1, 4, 25, 1, 0, 0, 2, 10, 0, 0, 0, 1, 0, 1, 2, 0, 0, 9, 1, 2, 0, 0, 0, 2, 5, 2, 1, 1, 5, 5, 8, 1, 1, 1, 5, 1, 4, 9, 1, 3, 0, 1, 0, 1, 1, 2, 0, 0,
    2, 0, 1, 8, 22, 8, 1, 0, 0, 0, 0, 4, 2, 1, 0, 9, 8, 5, 0, 9, 1, 30, 24, 2, 6, 4, 39, 0, 14, 5, 16, 6, 26, 179, 0, 2, 1, 1, 0, 0, 0, 5, 2, 9, 6, 0, 2, 5, 16, 7, 5, 1, 1, 0, 2, 4, 4, 7, 15, 13, 14, 0, 0,
    3, 0, 1, 0, 0, 0, 2, 1, 6, 4, 5, 1, 4, 9, 0, 3, 1, 8, 0, 0, 10, 5, 0, 43, 0, 2, 6, 8, 4, 0, 2, 0, 0, 9, 6, 0, 9, 3, 1, 6, 20, 14, 6, 1, 4, 0, 7, 2, 3, 0, 2, 0, 5, 0, 3, 1, 0, 3, 9, 7, 0, 3, 4, 0, 4, 9, 1, 6, 0,
    9, 0, 0, 2, 3, 10, 9, 28, 3, 6, 2, 4, 1, 2, 32, 4, 1, 18, 2, 0, 3, 1, 5, 30, 10, 0, 2, 2, 2, 0, 7, 9, 8, 11, 10, 11, 7, 2, 13, 7, 5, 10, 0, 3, 40, 2, 0, 1, 6, 12, 0, 4, 5, 1, 5, 11, 11, 21, 4, 8, 3, 7,
    8, 8, 33, 5, 23, 0, 0, 19, 8, 8, 2, 3, 0, 6, 1, 1, 1, 5, 1, 27, 4, 2, 5, 0, 3, 5, 6, 3, 1, 0, 3, 1, 12, 5, 3, 3, 2, 0, 7, 7, 2, 1, 0, 4, 0, 1, 1, 2, 0, 10, 10, 6, 2, 5, 9, 7, 5, 15, 15, 21, 6, 11, 5, 20,
    4, 3, 5, 5, 2, 5, 0, 2, 1, 0, 1, 7, 28, 0, 9, 0, 5, 12, 5, 5, 18, 30, 0, 12, 3, 3, 21, 16, 25, 32, 9, 3, 14, 11, 24, 5, 66, 9, 1, 2, 0, 5, 9, 1, 5, 1, 8, 0, 8, 3, 3, 0, 1, 15, 1, 4, 8, 1, 2, 7, 0, 7, 2,
    8, 3, 7, 5, 3, 7, 10, 2, 1, 0, 0, 2, 25, 0, 6, 4, 0, 10, 0, 4, 2, 4, 1, 12, 5, 38, 4, 0, 4, 1, 10, 5, 9, 4, 0, 14, 4, 2, 5, 18, 20, 21, 1, 3, 0, 5, 0, 7, 0, 3, 7, 1, 3, 1, 1, 8, 1, 0, 0, 0, 3, 2, 5, 2, 11,
    6, 0, 13, 1, 3, 9, 1, 12, 0, 16, 6, 2, 1, 0, 2, 1, 12, 6, 13, 11, 2, 0, 28, 1, 7, 8, 14, 13, 8, 13, 0, 2, 0, 5, 4, 8, 10, 2, 37, 42, 19, 6, 6, 7, 4, 14, 11, 18, 14, 80, 7, 6, 0, 4, 72, 12, 36, 27,
    7, 7, 0, 14, 17, 19, 164, 27, 0, 5, 10, 7, 3, 13, 6, 14, 0, 2, 2, 5, 3, 0, 6, 13, 0, 0, 10, 29, 0, 4, 0, 3, 13, 0, 3, 1, 6, 51, 1, 5, 28, 2, 0, 8, 0, 20, 2, 4, 0, 25, 2, 10, 13, 10, 0, 16, 4, 0, 1, 0,
    2, 1, 7, 0, 1, 8, 11, 0, 0, 1, 2, 7, 2, 23, 11, 6, 6, 4, 16, 2, 2, 2, 0, 22, 9, 3, 3, 5, 2, 0, 15, 16, 21, 2, 9, 20, 15, 15, 5, 3, 9, 1, 0, 0, 1, 7, 7, 5, 4, 2, 2, 2, 38, 24, 14, 0, 0, 15, 5, 6, 24, 14,
    5, 5, 11, 0, 21, 12, 0, 3, 8, 4, 11, 1, 8, 0, 11, 27, 7, 2, 4, 9, 21, 59, 0, 1, 39, 3, 60, 62, 3, 0, 12, 11, 0, 3, 30, 11, 0, 13, 88, 4, 15, 5, 28, 13, 1, 4, 48, 17, 17, 4, 28, 32, 46, 0, 16, 0,
    18, 11, 1, 8, 6, 38, 11, 2, 6, 11, 38, 2, 0, 45, 3, 11, 2, 7, 8, 4, 30, 14, 17, 2, 1, 1, 65, 18, 12, 16, 4, 2, 45, 123, 12, 56, 33, 1, 4, 3, 4, 7, 0, 0, 0, 3, 2, 0, 16, 4, 2, 4, 2, 0, 7, 4, 5, 2, 26,
    2, 25, 6, 11, 6, 1, 16, 2, 6, 17, 77, 15, 3, 35, 0, 1, 0, 5, 1, 0, 38, 16, 6, 3, 12, 3, 3, 3, 0, 9, 3, 1, 3, 5, 2, 9, 0, 18, 0, 25, 1, 3, 32, 1, 72, 46, 6, 2, 7, 1, 3, 14, 17, 0, 28, 1, 40, 13, 0, 20,
    15, 40, 6, 38, 24, 12, 43, 1, 1, 9, 0, 12, 6, 0, 6, 2, 4, 19, 3, 7, 1, 48, 0, 9, 5, 0, 5, 6, 9, 6, 10, 15, 2, 11, 19, 3, 9, 2, 0, 1, 10, 1, 27, 8, 1, 3, 6, 1, 14, 0, 26, 0, 27, 16, 3, 4, 9, 6, 2, 23,
    9, 10, 5, 25, 2, 1, 6, 1, 1, 48, 15, 9, 15, 14, 3, 4, 26, 60, 29, 13, 37, 21, 1, 6, 4, 0, 2, 11, 22, 23, 16, 16, 2, 2, 1, 3, 0, 5, 1, 6, 4, 0, 0, 4, 0, 0, 8, 3, 0, 2, 5, 0, 7, 1, 7, 3, 13, 2, 4, 10,
    3, 0, 2, 31, 0, 18, 3, 0, 12, 10, 4, 1, 0, 7, 5, 7, 0, 5, 4, 12, 2, 22, 10, 4, 2, 15, 2, 8, 9, 0, 23, 2, 197, 51, 3, 1, 1, 4, 13, 4, 3, 21, 4, 19, 3, 10, 5, 40, 0, 4, 1, 1, 10, 4, 1, 27, 34, 7, 21,
    2, 17, 2, 9, 6, 4, 2, 3, 0, 4, 2, 7, 8, 2, 5, 1, 15, 21, 3, 4, 4, 2, 2, 17, 22, 1, 5, 22, 4, 26, 7, 0, 32, 1, 11, 42, 15, 4, 1, 2, 5, 0, 19, 3, 1, 8, 6, 0, 10, 1, 9, 2, 13, 30, 8, 2, 24, 17, 19, 1, 4,
    4, 25, 13, 0, 10, 16, 11, 39, 18, 8, 5, 30, 82, 1, 6, 8, 18, 77, 11, 13, 20, 75, 11, 112, 78, 33, 3, 0, 0, 60, 17, 84, 9, 1, 1, 12, 30, 10, 49, 5, 32, 158, 178, 5, 5, 6, 3, 3, 1, 3, 1, 4, 7, 6,
    19, 31, 21, 0, 2, 9, 5, 6, 27, 4, 9, 8, 1, 76, 18, 12, 1, 4, 0, 3, 3, 6, 3, 12, 2, 8, 30, 16, 2, 25, 1, 5, 5, 4, 3, 0, 6, 10, 2, 3, 1, 0, 5, 1, 19, 3, 0, 8, 1, 5, 2, 6, 0, 0, 0, 19, 1, 2, 0, 5, 1, 2, 5,
    1, 3, 7, 0, 4, 12, 7, 3, 10, 22, 0, 9, 5, 1, 0, 2, 20, 1, 1, 3, 23, 30, 3, 9, 9, 1, 4, 191, 14, 3, 15, 6, 8, 50, 0, 1, 0, 0, 4, 0, 0, 1, 0, 2, 4, 2, 0, 2, 3, 0, 2, 0, 2, 2, 8, 7, 0, 1, 1, 1, 3, 3, 17, 11,
    91, 1, 9, 3, 2, 13, 4, 24, 15, 41, 3, 13, 3, 1, 20, 4, 125, 29, 30, 1, 0, 4, 12, 2, 21, 4, 5, 5, 19, 11, 0, 13, 11, 86, 2, 18, 0, 7, 1, 8, 8, 2, 2, 22, 1, 2, 6, 5, 2, 0, 1, 2, 8, 0, 2, 0, 5, 2, 1, 0,
    2, 10, 2, 0, 5, 9, 2, 1, 2, 0, 1, 0, 4, 0, 0, 10, 2, 5, 3, 0, 6, 1, 0, 1, 4, 4, 33, 3, 13, 17, 3, 18, 6, 4, 7, 1, 5, 78, 0, 4, 1, 13, 7, 1, 8, 1, 0, 35, 27, 15, 3, 0, 0, 0, 1, 11, 5, 41, 38, 15, 22, 6,
    14, 14, 2, 1, 11, 6, 20, 63, 5, 8, 27, 7, 11, 2, 2, 40, 58, 23, 50, 54, 56, 293, 8, 8, 1, 5, 1, 14, 0, 1, 12, 37, 89, 8, 8, 8, 2, 10, 6, 0, 0, 0, 4, 5, 2, 1, 0, 1, 1, 2, 7, 0, 3, 3, 0, 4, 6, 0, 3, 2,
    19, 3, 8, 0, 0, 0, 4, 4, 16, 0, 4, 1, 5, 1, 3, 0, 3, 4, 6, 2, 17, 10, 10, 31, 6, 4, 3, 6, 10, 126, 7, 3, 2, 2, 0, 9, 0, 0, 5, 20, 13, 0, 15, 0, 6, 0, 2, 5, 8, 64, 50, 3, 2, 12, 2, 9, 0, 0, 11, 8, 20,
    109, 2, 18, 23, 0, 0, 9, 61, 3, 0, 28, 41, 77, 27, 19, 17, 81, 5, 2, 14, 5, 83, 57, 252, 14, 154, 263, 14, 20, 8, 13, 6, 57, 39, 38,
  };
  static ushort base_ranges[] = {
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
    0x31F0, 0x31FF, // Katakana Phonetic Extensions
    0xFF00, 0xFFEF, // Half-width characters
  };
  static bool full_ranges_unpacked = false;
  static ushort full_ranges[countof(base_ranges) + countof(offsets_from_0x4E00) * 2 + 1];
  if (!full_ranges_unpacked) {
    // Unpack
    int n, codepoint = 0x4e00;
    ushort* dst;
    memcpy(full_ranges, base_ranges, sizeof(base_ranges));
    dst = full_ranges + countof(base_ranges);;
    for (n = 0; n < countof(offsets_from_0x4E00); n++, dst += 2) {
      dst[0] = dst[1] = (ushort)(codepoint += (offsets_from_0x4E00[n] + 1));
    }
    dst[0] = 0;
    full_ranges_unpacked = true;
  }
  return &full_ranges[0];
}
const ushort* ImFontConfig_GetGlyphRangesCyrillic()
{
  static const ushort ranges[] = {
    0x0020, 0x00FF, // Basic Latin + Latin Supplement
    0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
    0x2DE0, 0x2DFF, // Cyrillic Extended-A
    0xA640, 0xA69F, // Cyrillic Extended-B
    0,
  };
  return &ranges[0];
}
const ushort* ImFontConfig_GetGlyphRangesThai()
{
  static const ushort ranges[] = {
    0x0020, 0x00FF, // Basic Latin
    0x0E00, 0x0E7F, // Thai
    0,
  };
  return &ranges[0];
}
static const char* GetDefaultCompressedFontDataTTFBase85()
{
  static const char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
      "7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
      "2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
      "`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
      "i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
      "kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
      "*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
      "tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
      "ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
      "x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
      "CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
      "U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
      "'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
      "_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
      "Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
      "/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
      "%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
      "OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
      "h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
      "o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
      "j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
      "sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
      "eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
      "M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
      "LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
      "%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
      "Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
      "a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
      "$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
      "nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
      "7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
      ")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
      "D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
      "P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
      "bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
      "h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
      "V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
      "sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
      ".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
      "$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
      "hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
      "@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
      "w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
      "u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
      "d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
      "6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
      "b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
      ":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
      "tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
      "$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
      ":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
      "7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
      "u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
      "LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
      ":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
      "_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
      "hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
      "^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
      "+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
      "9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
      "CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
      "hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
      "8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
      "S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
      "0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
      "+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
      "M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
      "?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
      "Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
      ">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
      "[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
      "wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
      "Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
      "MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
      "i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
      "1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
      "iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
      "URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
      ";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
      "w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
      "d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
      "A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
      "/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
      "m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
      "TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
      "GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
      "O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";
  return proggy_clean_ttf_compressed_data_base85;
}
// Default font TTF is compressed with stb_compress then base85 encoded (see extra_fonts/binary_to_compressed_c.cpp for encoder)
static unsigned int Decode85Byte(char c)
{
  return c >= '\\' ? c - 36 : c - 35;
}
static void Decode85(const unsigned char* src, unsigned char* dst)
{
  while (*src) {
    unsigned int tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
    dst[0] = ((tmp >> 0) & 0xFF);
    dst[1] = ((tmp >> 8) & 0xFF);
    dst[2] = ((tmp >> 16) & 0xFF);
    dst[3] = ((tmp >> 24) & 0xFF); // We can't assume little-endianess.
    src += 5;
    dst += 4;
  }
}
//-----------------------------------------------------------------------------
// DEFAULT FONT DATA
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array.
// Use the program in extra_fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------


int ImFontConfig_Init_MemoryCompressedTTF(ImFontConfig* cfg, const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ushort* glyph_ranges)
{
  const unsigned int buf_decompressed_size = stb_decompress_length((unsigned char*)compressed_ttf_data);
  unsigned char* buf_decompressed_data = NULL;
  myReAlloc(buf_decompressed_data, buf_decompressed_size);
  stb_decompress(buf_decompressed_data, (unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);
  cfg->FontData = (void*)buf_decompressed_data;
  cfg->FontDataSize = buf_decompressed_size;
  cfg->SizePixels = size_pixels;
  cfg->GlyphRanges = glyph_ranges;
  cfg->OversampleH = cfg->OversampleV = 1;
  // Count glyphs
  if (!cfg->GlyphRanges) {
    cfg->GlyphRanges = ImFontConfig_GetGlyphRangesDefault();
  }
  return 0;//ImFontConfig_CreateFromMemoryTTF(cfg, buf_decompressed_data, (int)buf_decompressed_size, size_pixels, glyph_ranges);
}
int ImFontConfig_Init_MemoryCompressedBase85TTF(ImFontConfig* cfg, const char* compressed_ttf_data_base85, float size_pixels, const ushort* glyph_ranges)
{
  int ret = 0, compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
  char* compressed_ttf = NULL;
  myReAlloc(compressed_ttf, compressed_ttf_size);
  Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
  ret = ImFontConfig_Init_MemoryCompressedTTF(cfg, compressed_ttf, compressed_ttf_size, size_pixels, glyph_ranges);
  myMemFree(compressed_ttf);
  return ret;
}
void ImFontConfig_Free(ImFontConfig* cfg)
{
  myMemFree(cfg->FontData);
  memset(cfg, 0, sizeof(*cfg));
}
void ImFont_GrowIndex(ImFont* g, int new_size)
{
  int i;
  int old_size = g->IndexLookup_Size;
  int n = new_size - old_size;
  ASSERT(g->IndexXAdvance_Size == g->IndexLookup_Size);
  if (n <= 0) {
    return;
  }
  myAllocPush(g->IndexXAdvance, g->IndexXAdvance_Size, n);
  myAllocPush(g->IndexLookup, g->IndexLookup_Size, n);
  for (i = old_size; i < new_size; i++) {
    g->IndexXAdvance[i] = -1.0f;
    g->IndexLookup[i] = (unsigned short) - 1;
  }
}
ImFontGlyph* ImFont_FindGlyph(const ImFont* g, unsigned short c)
{
  if (c < g->IndexLookup_Size) {
    const unsigned short i = g->IndexLookup[c];
    if (i != (unsigned short) - 1) {
      return &g->Glyphs[i];
    }
  }
  return g->FallbackGlyph;
}
void ImFont_BuildLookupTable(ImFont* g)
{
  int i;
  int max_codepoint = 0;
  for (i = 0; i != g->Glyphs_Size; i++) {
    max_codepoint = ImMax(max_codepoint, (int)g->Glyphs[i].Codepoint);
  }
  ASSERT(g->Glyphs_Size < 0xFFFF); // -1 is reserved
  g->IndexXAdvance_Size = 0;
  g->IndexLookup_Size = 0;
  ImFont_GrowIndex(g, max_codepoint + 1);
  for (i = 0; i < g->Glyphs_Size; i++) {
    int codepoint = (int)g->Glyphs[i].Codepoint;
    g->IndexXAdvance[codepoint] = g->Glyphs[i].XAdvance;
    g->IndexLookup[codepoint] = (unsigned short)i;
  }
  // Create a glyph to handle TAB
  // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
  if (ImFont_FindGlyph(g, (unsigned short)' ')) {
    ImFontGlyph* tab_glyph = g->Glyphs + g->Glyphs_Size - 1;
    if (tab_glyph->Codepoint != '\t') { // So we can call this function multiple times
      tab_glyph = (ImFontGlyph*)myAllocPush(g->Glyphs, g->Glyphs_Size, 1);
    }
    *tab_glyph = *ImFont_FindGlyph(g, (unsigned short)' ');
    tab_glyph->Codepoint = '\t';
    tab_glyph->XAdvance *= 4;
    g->IndexXAdvance[(int)tab_glyph->Codepoint] = (float)tab_glyph->XAdvance;
    g->IndexLookup[(int)tab_glyph->Codepoint] = (unsigned short)(g->Glyphs_Size - 1);
  }
  g->FallbackGlyph = NULL;
  g->FallbackGlyph = ImFont_FindGlyph(g, g->FallbackChar);
  g->FallbackXAdvance = g->FallbackGlyph ? g->FallbackGlyph->XAdvance : 0.0f;
  for (i = 0; i < max_codepoint + 1; i++) {
    if (g->IndexXAdvance[i] < 0.0f) {
      g->IndexXAdvance[i] = g->FallbackXAdvance;
    }
  }
}
void ImFont_ClearTexData(ImFont* g)
{
  if (g->TexPixelsAlpha8) {
    myMemFree(g->TexPixelsAlpha8);
    g->TexPixelsAlpha8 = NULL;
  }
}
typedef struct {
  stbrp_rect* rects;
  int Size;
} stbrp_rect_vec;
// Enumeration for GetMouseCursor()
typedef enum {
  ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput, // When hovering over InputText, etc.
    ImGuiMouseCursor_Move, // Unused
    ImGuiMouseCursor_ResizeNS, // Unused
    ImGuiMouseCursor_ResizeEW, // When hovering over a column
    ImGuiMouseCursor_ResizeNESW, // Unused
    ImGuiMouseCursor_ResizeNWSE, // When hovering over the bottom-right corner of a window
    ImGuiMouseCursor_Count_
} ImGuiMouseCursor_;
void ImFont_RenderCustomTexData(ImFont* g, int pass, stbrp_rect_vec* p_rects)
{
  // A work of art lies ahead! (. = white layer, X = black layer, others are blank)
  // The white texels on the top left are the ones we'll use everywhere in ImGui to render filled shapes.
  enum { TEX_DATA_W = 90, TEX_DATA_H = 27};
  const char texture_data[TEX_DATA_W * TEX_DATA_H + 1] = {
    "..- -XXXXXXX- X - X -XXXXXXX - XXXXXXX"
    "..- -X.....X- X.X - X.X -X.....X - X.....X"
    "--- -XXX.XXX- X...X - X...X -X....X - X....X"
    "X - X.X - X.....X - X.....X -X...X - X...X"
    "XX - X.X -X.......X- X.......X -X..X.X - X.X..X"
    "X.X - X.X -XXXX.XXXX- XXXX.XXXX -X.X X.X - X.X X.X"
    "X..X - X.X - X.X - X.X -XX X.X - X.X XX"
    "X...X - X.X - X.X - XX X.X XX - X.X - X.X "
    "X....X - X.X - X.X - X.X X.X X.X - X.X - X.X "
    "X.....X - X.X - X.X - X..X X.X X..X - X.X - X.X "
    "X......X - X.X - X.X - X...XXXXXX.XXXXXX...X - X.X XX-XX X.X "
    "X.......X - X.X - X.X -X.....................X- X.X X.X-X.X X.X "
    "X........X - X.X - X.X - X...XXXXXX.XXXXXX...X - X.X..X-X..X.X "
    "X.........X -XXX.XXX- X.X - X..X X.X X..X - X...X-X...X "
    "X..........X-X.....X- X.X - X.X X.X X.X - X....X-X....X "
    "X......XXXXX-XXXXXXX- X.X - XX X.X XX - X.....X-X.....X "
    "X...X..X --------- X.X - X.X - XXXXXXX-XXXXXXX "
    "X..X X..X - -XXXX.XXXX- XXXX.XXXX ------------------------------------"
    "X.X X..X - -X.......X- X.......X - XX XX - "
    "XX X..X - - X.....X - X.....X - X.X X.X - "
    " X..X - X...X - X...X - X..X X..X - "
    " XX - X.X - X.X - X...XXXXXXXXXXXXX...X - "
    "------------ - X - X -X.....................X- "
    " ----------------------------------- X...XXXXXXXXXXXXX...X - "
    " - X..X X..X - "
    " - X.X X.X - "
    " - XX XX - "
  };
  stbrp_rect_vec* rects = p_rects;
  if (pass == 0) {
    // Request rectangles
    stbrp_rect* r = (stbrp_rect*)myAllocPush(rects->rects, rects->Size, 1);
    memset(r, 0, sizeof(*r));
    r->w = (TEX_DATA_W * 2) + 1;
    r->h = TEX_DATA_H + 1;
  }
  else if (pass == 1) {
    // Render/copy pixels
    const stbrp_rect* r = rects->rects;
    int x, y, n;
    const ImVec2 cursor_datas[ImGuiMouseCursor_Count_][3] = {
      // Pos ... Size ... Offset ......
      { 0, 3, 12, 19, 0, 0 }, // ImGuiMouseCursor_Arrow
      { 13, 0, 7, 16, 4, 8 }, // ImGuiMouseCursor_TextInput
      { 31, 0, 23, 23, 11, 11 }, // ImGuiMouseCursor_Move
      { 21, 0, 9, 23, 5, 11 }, // ImGuiMouseCursor_ResizeNS
      { 55, 18, 23, 9, 11, 5 }, // ImGuiMouseCursor_ResizeEW
      { 73, 0, 17, 17, 9, 9 }, // ImGuiMouseCursor_ResizeNESW
      { 55, 0, 17, 17, 9, 9 }, // ImGuiMouseCursor_ResizeNWSE
    };
    const ImVec2 tex_uv_scale = fVec2(1.0f / g->TexWidth, 1.0f / g->TexHeight);
    for (y = 0, n = 0; y < TEX_DATA_H; y++) {
      for (x = 0; x < TEX_DATA_W; x++, n++) {
        const int offset0 = (int)(r->x + x) + (int)(r->y + y) * g->TexWidth;
        const int offset1 = offset0 + 1 + TEX_DATA_W;
        g->TexPixelsAlpha8[offset0] = texture_data[n] == '.' ? 0xFF : 0x00;
        g->TexPixelsAlpha8[offset1] = texture_data[n] == 'X' ? 0xFF : 0x00;
      }
    }
    g->TexUvWhitePixel = fVec2((r->x + 0.5f) * tex_uv_scale.x, (r->y + 0.5f) * tex_uv_scale.y);
    //imsave("C:\\asdf.bmp", TEX_DATA_H, TEX_DATA_W, g->TexPixelsAlpha8, TEX_DATA_W, 1);
    // Setup mouse cursors
    //imwrite();
#if 0
    for (int type = 0; type < ImGuiMouseCursor_Count_; type++) {
      ImGuiMouseCursorData& cursor_data = GImGui->MouseCursorData[type];
      ImVec2 pos = cursor_datas[type][0] + fVec2((float)r->x, (float)r->y);
      const ImVec2 size = cursor_datas[type][1];
      cursor_data.Type = type;
      cursor_data.Size = size;
      cursor_data.HotOffset = cursor_datas[type][2];
      cursor_data.TexUvMin[0] = (pos) * tex_uv_scale;
      cursor_data.TexUvMax[0] = (pos + size) * tex_uv_scale;
      pos.x += TEX_DATA_W + 1;
      cursor_data.TexUvMin[1] = (pos) * tex_uv_scale;
      cursor_data.TexUvMax[1] = (pos + size) * tex_uv_scale;
    }
#endif
  }
}
bool ImFont_Build(ImFont* g, const ImFontConfig* ConfigData, int ConfigData_Size)
{
  int i, input_i;
  int total_glyph_count = 0;
  int total_glyph_range_count = 0;
  typedef struct {
    stbtt_fontinfo FontInfo;
    stbrp_rect* Rects;
    stbtt_pack_range* Ranges;
    int RangesCount;
  } ImFontTempBuildData;
  ImFontTempBuildData* tmp_array = NULL;
  ASSERT(ConfigData_Size > 0);
  g->TexID = NULL;
  g->TexWidth = g->TexHeight = 0;
  g->TexUvWhitePixel = fVec2(0, 0);
  ImFont_ClearTexData(g);
  // Initialize font information early (so we can error without any cleanup) + count glyphs
  myReAlloc(tmp_array, ConfigData_Size);
  for (input_i = 0; input_i < ConfigData_Size; input_i++) {
    const ImFontConfig* cfg = ConfigData + input_i;
    ImFontTempBuildData* tmp = tmp_array + input_i;
    const ushort* in_range;
    const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg->FontData, cfg->FontNo);
    ASSERT(font_offset >= 0);
    if (!stbtt_InitFont(&tmp->FontInfo, (unsigned char*)cfg->FontData, font_offset)) {
      return false;
    }
    for (in_range = cfg->GlyphRanges; in_range[0] && in_range[1]; in_range += 2) {
      total_glyph_count += (in_range[1] - in_range[0]) + 1;
      total_glyph_range_count++;
    }
  }
  // Start packing. We need a known width for the skyline algorithm. Using a cheap heuristic here to decide of width. User can override g->TexDesiredWidth if they wish.
  // After packing is done, width shouldn't matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
  g->TexWidth = (g->TexDesiredWidth > 0) ? g->TexDesiredWidth : (total_glyph_count > 4000) ? 4096 : (total_glyph_count > 2000) ? 2048 : (total_glyph_count > 1000) ? 1024 : 512;
  g->TexHeight = 0;
  {
    const int max_tex_height = 1024 * 32;
    stbtt_pack_context spc;
    stbrp_rect_vec extra_rects[1] = {0};
    stbtt_PackBegin(&spc, NULL, g->TexWidth, max_tex_height, 0, 1, NULL);
    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    ImFont_RenderCustomTexData(g, 0, extra_rects);
    stbtt_PackSetOversampling(&spc, 1, 1);
    stbrp_pack_rects((stbrp_context*)spc.pack_info, &extra_rects->rects[0], extra_rects->Size);
    for (i = 0; i < extra_rects->Size; i++) {
      if (extra_rects->rects[i].was_packed) {
        g->TexHeight = ImMax(g->TexHeight, extra_rects->rects[i].y + extra_rects->rects[i].h);
      }
    }
    {
      // Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
      int buf_packedchars_n = 0, buf_rects_n = 0, buf_ranges_n = 0;
      stbtt_packedchar* buf_packedchars = (stbtt_packedchar*)myMemAlloc(total_glyph_count * sizeof(stbtt_packedchar));
      stbrp_rect* buf_rects = (stbrp_rect*)myMemAlloc(total_glyph_count * sizeof(stbrp_rect));
      stbtt_pack_range* buf_ranges = (stbtt_pack_range*)myMemAlloc(total_glyph_range_count * sizeof(stbtt_pack_range));
      memset(buf_packedchars, 0, total_glyph_count * sizeof(stbtt_packedchar));
      memset(buf_rects, 0, total_glyph_count * sizeof(stbrp_rect)); // Unnecessary but let's clear this for the sake of sanity.
      memset(buf_ranges, 0, total_glyph_range_count * sizeof(stbtt_pack_range));
      // First font pass: pack all glyphs (no rendering at this point, we are working with rectangles in an infinitely tall texture at this point)
      for (input_i = 0; input_i < ConfigData_Size; input_i++) {
        const ImFontConfig* cfg = ConfigData + input_i;
        ImFontTempBuildData* tmp = tmp_array + input_i;
        // Setup ranges
        int glyph_count = 0;
        int n, glyph_ranges_count = 0;
        const ushort* in_range;
        for (in_range = cfg->GlyphRanges; in_range[0] && in_range[1]; in_range += 2) {
          glyph_count += (in_range[1] - in_range[0]) + 1;
          glyph_ranges_count++;
        }
        tmp->Ranges = buf_ranges + buf_ranges_n;
        tmp->RangesCount = glyph_ranges_count;
        buf_ranges_n += glyph_ranges_count;
        for (i = 0; i < glyph_ranges_count; i++) {
          const ushort* in_range = &cfg->GlyphRanges[i * 2];
          stbtt_pack_range* range = tmp->Ranges + i;
          range->font_size = cfg->SizePixels;
          range->first_unicode_codepoint_in_range = in_range[0];
          range->num_chars = (in_range[1] - in_range[0]) + 1;
          range->chardata_for_range = buf_packedchars + buf_packedchars_n;
          buf_packedchars_n += range->num_chars;
        }
        // Pack
        tmp->Rects = buf_rects + buf_rects_n;
        buf_rects_n += glyph_count;
        stbtt_PackSetOversampling(&spc, cfg->OversampleH, cfg->OversampleV);
        n = stbtt_PackFontRangesGatherRects(&spc, &tmp->FontInfo, tmp->Ranges, tmp->RangesCount, tmp->Rects);
        stbrp_pack_rects((stbrp_context*)spc.pack_info, tmp->Rects, n);
        // Extend texture height
        for (i = 0; i < n; i++) {
          if (tmp->Rects[i].was_packed) {
            g->TexHeight = ImMax(g->TexHeight, tmp->Rects[i].y + tmp->Rects[i].h);
          }
        }
      }
      ASSERT(buf_rects_n == total_glyph_count);
      ASSERT(buf_packedchars_n == total_glyph_count);
      ASSERT(buf_ranges_n == total_glyph_range_count);
      // Create texture
      g->TexHeight = ImUpperPowerOfTwo(g->TexHeight);
      g->TexPixelsAlpha8 = (unsigned char*)myMemAlloc(g->TexWidth * g->TexHeight);
      memset(g->TexPixelsAlpha8, 0, g->TexWidth * g->TexHeight);
      spc.pixels = g->TexPixelsAlpha8;
      spc.height = g->TexHeight;
      // Second pass: render characters
      for (input_i = 0; input_i < ConfigData_Size; input_i++) {
        const ImFontConfig* cfg = ConfigData + input_i;
        ImFontTempBuildData* tmp = tmp_array + input_i;
        stbtt_PackSetOversampling(&spc, cfg->OversampleH, cfg->OversampleV);
        stbtt_PackFontRangesRenderIntoRects(&spc, &tmp->FontInfo, tmp->Ranges, tmp->RangesCount, tmp->Rects);
        tmp->Rects = NULL;
      }
      // End packing
      stbtt_PackEnd(&spc);
      myMemFree(buf_rects);
      buf_rects = NULL;
      // Third pass: setup ImFont and glyphs for runtime
      for (input_i = 0; input_i < ConfigData_Size; input_i++) {
        const ImFontConfig* cfg = ConfigData + input_i;
        ImFontTempBuildData* tmp = tmp_array + input_i;
        ImFont* dst_font = g;
        float off_y, ascent, descent, font_scale = stbtt_ScaleForPixelHeight(&tmp->FontInfo, cfg->SizePixels);
        int char_idx, unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&tmp->FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);
        ascent = unscaled_ascent * font_scale;
        descent = unscaled_descent * font_scale;
        if (!cfg->MergeMode) {
          dst_font->FontSize = cfg->SizePixels;
          dst_font->Ascent = ascent;
          dst_font->Descent = descent;
          dst_font->Glyphs_Size = 0;
        }
        dst_font->ConfigDataCount++;
        off_y = (cfg->MergeMode && cfg->MergeGlyphCenterV) ? (ascent - dst_font->Ascent) * 0.5f : 0.0f;
        dst_font->FallbackGlyph = NULL; // Always clear fallback so FindGlyph can return NULL. It will be set again in BuildLookupTable()
        for (i = 0; i < tmp->RangesCount; i++) {
          stbtt_pack_range* range = tmp->Ranges + i;
          for (char_idx = 0; char_idx < range->num_chars; char_idx += 1) {
            const stbtt_packedchar* pc = &range->chardata_for_range[char_idx];
            stbtt_aligned_quad q;
            float dummy_x = 0.0f, dummy_y = 0.0f;
            int codepoint;
            ImFontGlyph* glyph = NULL;
            if (!pc->x0 && !pc->x1 && !pc->y0 && !pc->y1) {
              continue;
            }
            codepoint = range->first_unicode_codepoint_in_range + char_idx;
            if (cfg->MergeMode && ImFont_FindGlyph(dst_font, (unsigned short)codepoint)) {
              continue;
            }
            stbtt_GetPackedQuad(range->chardata_for_range, g->TexWidth, g->TexHeight, char_idx, &dummy_x, &dummy_y, &q, 0);
            glyph = (ImFontGlyph*)myAllocPush(dst_font->Glyphs, dst_font->Glyphs_Size, 1);
            glyph->Codepoint = (ushort)codepoint;
            glyph->X0 = q.x0;
            glyph->Y0 = q.y0;
            glyph->X1 = q.x1;
            glyph->Y1 = q.y1;
            glyph->U0 = q.s0;
            glyph->V0 = q.t0;
            glyph->U1 = q.s1;
            glyph->V1 = q.t1;
            glyph->Y0 += (float)(int)(dst_font->Ascent + off_y + 0.5f);
            glyph->Y1 += (float)(int)(dst_font->Ascent + off_y + 0.5f);
            glyph->XAdvance = (pc->xadvance + cfg->GlyphExtraSpacing.x); // Bake spacing into XAdvance
            if (cfg->PixelSnapH) {
              glyph->XAdvance = (float)(int)(glyph->XAdvance + 0.5f);
            }
          }
        }
        ImFont_BuildLookupTable(g);
      }
      // Cleanup temporaries
      myMemFree(buf_packedchars);
      myMemFree(buf_ranges);
      myMemFree(tmp_array);
      // Render into our custom data block
      ImFont_RenderCustomTexData(g, 1, extra_rects);
    }
  }
  return true;
}
void ImFont_Init_Impl(ImFont* g, ImFontConfig* ConfigData, int ConfigData_Size)
{
  // Convert to RGBA32 format on demand
  // Although it is likely to be the most commonly used format, our font rendering is 1 channel / 8 bpp

    // Build atlas on demand
    if (g->TexPixelsAlpha8 == NULL) {
      ImFont_Build(g, ConfigData, ConfigData_Size);
    }
    //int imsave(const char* filename, int height, int width, const uchar* data, int step, int channels);
    //imsave("C:\\aaa1.bmp", g->TexHeight, g->TexWidth, (uchar*)g->TexPixelsAlpha8, g->TexWidth, 1);
  
}
int ImFont_Init_Default(ImFont* g)
{
  const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
  ImFontConfig cfg[1] = {0};
  // <default>
  ImFontConfig_Init_MemoryCompressedBase85TTF(cfg, ttf_compressed_base85, 13.0f, ImFontConfig_GetGlyphRangesDefault());
  ImFont_Init_Impl(g, cfg, 1);
  ImFontConfig_Free(cfg);
  return 0;
}
int test_font()
{
  ImFont f[1] = {0};
  ImFont_Init_Default(f);
  return 0;
}
#endif // _IMUI_FONT_INL_

